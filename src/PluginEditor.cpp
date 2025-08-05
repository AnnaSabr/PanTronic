#include "PluginEditor.hpp"
#include "PluginProcessor.hpp"
#include "ADSRComponent.hpp"
#include "ReverbComponent.hpp"
#include "ChorusComponent.hpp"
#include <magic_enum/magic_enum.hpp>

//==============================================================================
AvSynthAudioProcessorEditor::AvSynthAudioProcessorEditor(AvSynthAudioProcessor &p)
    : AudioProcessorEditor(&p), processorRef(p),
      gainSlider(juce::Slider::LinearHorizontal, juce::Slider::TextEntryBoxPosition::TextBoxLeft),
      gainAttachment(p.parameters, magic_enum::enum_name<AvSynthAudioProcessor::Parameters::Gain>().data(), gainSlider),

      frequencySlider(juce::Slider::LinearHorizontal, juce::Slider::TextEntryBoxPosition::TextBoxLeft),
      frequencyAttachment(p.parameters, magic_enum::enum_name<AvSynthAudioProcessor::Parameters::Frequency>().data(),
                          frequencySlider),

      oscTypeComboBox(),
      oscTypeAttachment(p.parameters, magic_enum::enum_name<AvSynthAudioProcessor::Parameters::OscType>().data(),
                        oscTypeComboBox),

      lowCutFreqSlider(juce::Slider::LinearHorizontal, juce::Slider::TextEntryBoxPosition::TextBoxLeft),
      lowCutFreqAttachment(p.parameters, magic_enum::enum_name<AvSynthAudioProcessor::Parameters::LowPassFreq>().data(),
                           lowCutFreqSlider),

      highCutFreqSlider(juce::Slider::LinearHorizontal, juce::Slider::TextEntryBoxPosition::TextBoxLeft),
      highCutFreqAttachment(p.parameters, magic_enum::enum_name<AvSynthAudioProcessor::Parameters::HighPassFreq>().data(),
                            highCutFreqSlider),

      keyboardComponent(p.keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard),

      waveformComponent(p.circularBuffer, p.bufferWritePos),
      spectrumComponent(p.circularBuffer, p.bufferWritePos){

    setLookAndFeel(&mysticalLookAndFeel);

    juce::ignoreUnused(processorRef);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    auto *oscTypeParam = dynamic_cast<juce::AudioParameterChoice *>(
        p.parameters.getParameter(magic_enum::enum_name<AvSynthAudioProcessor::Parameters::OscType>().data()));

    if (oscTypeParam != nullptr) {
        oscTypeComboBox.clear();
        auto &choices = oscTypeParam->choices;
        for (int i = 0; i < choices.size(); ++i) {
            oscTypeComboBox.addItem(choices[i], i + 1);
        }
        oscTypeComboBox.setSelectedId(oscTypeParam->getIndex() + 1, juce::dontSendNotification);
    }




    gainSlider.setColour(juce::Slider::thumbColourId, juce::Colour(0xff64b5f6));
    frequencySlider.setColour(juce::Slider::thumbColourId, juce::Colour(0xff64b5f6));

    // Keyboard-Farben anpassen (diese Farben existieren in JUCE)
    keyboardComponent.setColour(juce::MidiKeyboardComponent::whiteNoteColourId, juce::Colour(0xff2d3e54));
    keyboardComponent.setColour(juce::MidiKeyboardComponent::blackNoteColourId, juce::Colour(0xff0a0f1c));
    keyboardComponent.setColour(juce::MidiKeyboardComponent::keySeparatorLineColourId, juce::Colour(0xff64b5f6).withAlpha(0.3f));
    keyboardComponent.setColour(juce::MidiKeyboardComponent::mouseOverKeyOverlayColourId, juce::Colour(0xff64b5f6).withAlpha(0.3f));
    keyboardComponent.setColour(juce::MidiKeyboardComponent::keyDownOverlayColourId, juce::Colour(0xff4a3472).withAlpha(0.8f));

    // Weitere mystische Anpassungen für das LookAndFeel
    mysticalLookAndFeel.setColour(juce::MidiKeyboardComponent::textLabelColourId, juce::Colour(0xffc5d1de));
    mysticalLookAndFeel.setColour(juce::MidiKeyboardComponent::shadowColourId, juce::Colour(0xff0a0f1c).withAlpha(0.5f));





    // ADSR Component Setup
    setupADSRComponent();

    // Chorus Component Setup
    setupChorusComponent();

    // Reverb Component Setup
    setupReverbComponent();

    // Flute Preset Button Setup
    flutePresetButton.setButtonText("Flute Preset");
    flutePresetButton.onClick = [this] { loadFlutePreset(); };

    for (const auto component : GetComps()) {
        addAndMakeVisible(component);
    }
    setSize(800, 900);
    setResizable(true, true);
}

void AvSynthAudioProcessorEditor::setupADSRComponent() {
    // Initiale Werte aus den Parametern laden
    auto attackParam = processorRef.parameters.getParameter(magic_enum::enum_name<AvSynthAudioProcessor::Parameters::Attack>().data());
    auto decayParam = processorRef.parameters.getParameter(magic_enum::enum_name<AvSynthAudioProcessor::Parameters::Decay>().data());
    auto sustainParam = processorRef.parameters.getParameter(magic_enum::enum_name<AvSynthAudioProcessor::Parameters::Sustain>().data());
    auto releaseParam = processorRef.parameters.getParameter(magic_enum::enum_name<AvSynthAudioProcessor::Parameters::Release>().data());

    if (attackParam) adsrComponent.setAttack(attackParam->getValue());
    if (decayParam) adsrComponent.setDecay(decayParam->getValue());
    if (sustainParam) adsrComponent.setSustain(sustainParam->getValue());
    if (releaseParam) adsrComponent.setRelease(releaseParam->getValue());

    // Callback für Parameter-Änderungen
    adsrComponent.onParameterChanged = [this](float attack, float decay, float sustain, float release) {
        // Parameter im AudioProcessor aktualisieren
        auto* attackParam = processorRef.parameters.getParameter(magic_enum::enum_name<AvSynthAudioProcessor::Parameters::Attack>().data());
        auto* decayParam = processorRef.parameters.getParameter(magic_enum::enum_name<AvSynthAudioProcessor::Parameters::Decay>().data());
        auto* sustainParam = processorRef.parameters.getParameter(magic_enum::enum_name<AvSynthAudioProcessor::Parameters::Sustain>().data());
        auto* releaseParam = processorRef.parameters.getParameter(magic_enum::enum_name<AvSynthAudioProcessor::Parameters::Release>().data());

        if (auto* floatParam = dynamic_cast<juce::AudioParameterFloat*>(attackParam)) {
            // Attack: 0.001s - 5s (logarithmisch skaliert)
            float scaledAttack = 0.001f + attack * attack * 4.999f; // Quadratische Skalierung für bessere Kontrolle
            floatParam->setValueNotifyingHost(floatParam->convertTo0to1(scaledAttack));
        }

        if (auto* floatParam = dynamic_cast<juce::AudioParameterFloat*>(decayParam)) {
            // Decay: 0.001s - 5s
            float scaledDecay = 0.001f + decay * decay * 4.999f;
            floatParam->setValueNotifyingHost(floatParam->convertTo0to1(scaledDecay));
        }

        if (auto* floatParam = dynamic_cast<juce::AudioParameterFloat*>(sustainParam)) {
            // Sustain: 0.0 - 1.0 (linear)
            floatParam->setValueNotifyingHost(sustain);
        }

        if (auto* floatParam = dynamic_cast<juce::AudioParameterFloat*>(releaseParam)) {
            // Release: 0.001s - 5s
            float scaledRelease = 0.001f + release * release * 4.999f;
            floatParam->setValueNotifyingHost(floatParam->convertTo0to1(scaledRelease));
        }
    };

    // Parameter-Listener hinzufügen um die ADSR-Component zu aktualisieren
    processorRef.parameters.addParameterListener(magic_enum::enum_name<AvSynthAudioProcessor::Parameters::Attack>().data(), this);
    processorRef.parameters.addParameterListener(magic_enum::enum_name<AvSynthAudioProcessor::Parameters::Decay>().data(), this);
    processorRef.parameters.addParameterListener(magic_enum::enum_name<AvSynthAudioProcessor::Parameters::Sustain>().data(), this);
    processorRef.parameters.addParameterListener(magic_enum::enum_name<AvSynthAudioProcessor::Parameters::Release>().data(), this);
}

void AvSynthAudioProcessorEditor::setupReverbComponent() {
    // Initiale Werte aus den Parametern laden
    auto roomSizeParam = processorRef.parameters.getParameter(magic_enum::enum_name<AvSynthAudioProcessor::Parameters::ReverbRoomSize>().data());
    auto dampingParam = processorRef.parameters.getParameter(magic_enum::enum_name<AvSynthAudioProcessor::Parameters::ReverbDamping>().data());
    auto wetLevelParam = processorRef.parameters.getParameter(magic_enum::enum_name<AvSynthAudioProcessor::Parameters::ReverbWetLevel>().data());
    auto dryLevelParam = processorRef.parameters.getParameter(magic_enum::enum_name<AvSynthAudioProcessor::Parameters::ReverbDryLevel>().data());
    auto widthParam = processorRef.parameters.getParameter(magic_enum::enum_name<AvSynthAudioProcessor::Parameters::ReverbWidth>().data());

    if (roomSizeParam) reverbComponent.setRoomSize(roomSizeParam->getValue());
    if (dampingParam) reverbComponent.setDamping(dampingParam->getValue());
    if (wetLevelParam) reverbComponent.setWetLevel(wetLevelParam->getValue());
    if (dryLevelParam) reverbComponent.setDryLevel(dryLevelParam->getValue());
    if (widthParam) reverbComponent.setWidth(widthParam->getValue());

    // Callback für Parameter-Änderungen
    reverbComponent.onParameterChanged = [this](float roomSize, float damping, float wetLevel, float dryLevel, float width) {
        // Parameter im AudioProcessor aktualisieren
        auto* roomSizeParam = processorRef.parameters.getParameter(magic_enum::enum_name<AvSynthAudioProcessor::Parameters::ReverbRoomSize>().data());
        auto* dampingParam = processorRef.parameters.getParameter(magic_enum::enum_name<AvSynthAudioProcessor::Parameters::ReverbDamping>().data());
        auto* wetLevelParam = processorRef.parameters.getParameter(magic_enum::enum_name<AvSynthAudioProcessor::Parameters::ReverbWetLevel>().data());
        auto* dryLevelParam = processorRef.parameters.getParameter(magic_enum::enum_name<AvSynthAudioProcessor::Parameters::ReverbDryLevel>().data());
        auto* widthParam = processorRef.parameters.getParameter(magic_enum::enum_name<AvSynthAudioProcessor::Parameters::ReverbWidth>().data());

        if (auto* floatParam = dynamic_cast<juce::AudioParameterFloat*>(roomSizeParam)) {
            floatParam->setValueNotifyingHost(roomSize);
        }

        if (auto* floatParam = dynamic_cast<juce::AudioParameterFloat*>(dampingParam)) {
            floatParam->setValueNotifyingHost(damping);
        }

        if (auto* floatParam = dynamic_cast<juce::AudioParameterFloat*>(wetLevelParam)) {
            floatParam->setValueNotifyingHost(wetLevel);
        }

        if (auto* floatParam = dynamic_cast<juce::AudioParameterFloat*>(dryLevelParam)) {
            floatParam->setValueNotifyingHost(dryLevel);
        }

        if (auto* floatParam = dynamic_cast<juce::AudioParameterFloat*>(widthParam)) {
            floatParam->setValueNotifyingHost(width);
        }
    };

    // Parameter-Listener hinzufügen um die Reverb-Component zu aktualisieren
    processorRef.parameters.addParameterListener(magic_enum::enum_name<AvSynthAudioProcessor::Parameters::ReverbRoomSize>().data(), this);
    processorRef.parameters.addParameterListener(magic_enum::enum_name<AvSynthAudioProcessor::Parameters::ReverbDamping>().data(), this);
    processorRef.parameters.addParameterListener(magic_enum::enum_name<AvSynthAudioProcessor::Parameters::ReverbWetLevel>().data(), this);
    processorRef.parameters.addParameterListener(magic_enum::enum_name<AvSynthAudioProcessor::Parameters::ReverbDryLevel>().data(), this);
    processorRef.parameters.addParameterListener(magic_enum::enum_name<AvSynthAudioProcessor::Parameters::ReverbWidth>().data(), this);
}

void AvSynthAudioProcessorEditor::setupChorusComponent() {
    // Initiale Werte aus den Parametern laden
    auto rateParam = processorRef.parameters.getParameter(magic_enum::enum_name<AvSynthAudioProcessor::Parameters::ChorusRate>().data());
    auto depthParam = processorRef.parameters.getParameter(magic_enum::enum_name<AvSynthAudioProcessor::Parameters::ChorusDepth>().data());
    auto feedbackParam = processorRef.parameters.getParameter(magic_enum::enum_name<AvSynthAudioProcessor::Parameters::ChorusFeedback>().data());
    auto mixParam = processorRef.parameters.getParameter(magic_enum::enum_name<AvSynthAudioProcessor::Parameters::ChorusMix>().data());

    if (rateParam) chorusComponent.setRate(rateParam->getValue());
    if (depthParam) chorusComponent.setDepth(depthParam->getValue());
    if (feedbackParam) chorusComponent.setFeedback(feedbackParam->getValue());
    if (mixParam) chorusComponent.setMix(mixParam->getValue());

    // Callback für Parameter-Änderungen
    chorusComponent.onParameterChanged = [this](float rate, float depth, float feedback, float mix) {
        // Parameter im AudioProcessor aktualisieren
        auto* rateParam = processorRef.parameters.getParameter(magic_enum::enum_name<AvSynthAudioProcessor::Parameters::ChorusRate>().data());
        auto* depthParam = processorRef.parameters.getParameter(magic_enum::enum_name<AvSynthAudioProcessor::Parameters::ChorusDepth>().data());
        auto* feedbackParam = processorRef.parameters.getParameter(magic_enum::enum_name<AvSynthAudioProcessor::Parameters::ChorusFeedback>().data());
        auto* mixParam = processorRef.parameters.getParameter(magic_enum::enum_name<AvSynthAudioProcessor::Parameters::ChorusMix>().data());

        if (auto* floatParam = dynamic_cast<juce::AudioParameterFloat*>(rateParam)) {
            floatParam->setValueNotifyingHost(floatParam->convertTo0to1(rate));
        }

        if (auto* floatParam = dynamic_cast<juce::AudioParameterFloat*>(depthParam)) {
            floatParam->setValueNotifyingHost(depth);
        }

        if (auto* floatParam = dynamic_cast<juce::AudioParameterFloat*>(feedbackParam)) {
            floatParam->setValueNotifyingHost(feedback);
        }

        if (auto* floatParam = dynamic_cast<juce::AudioParameterFloat*>(mixParam)) {
            floatParam->setValueNotifyingHost(mix);
        }
    };

    // Parameter-Listener hinzufügen um die Chorus-Component zu aktualisieren
    processorRef.parameters.addParameterListener(magic_enum::enum_name<AvSynthAudioProcessor::Parameters::ChorusRate>().data(), this);
    processorRef.parameters.addParameterListener(magic_enum::enum_name<AvSynthAudioProcessor::Parameters::ChorusDepth>().data(), this);
    processorRef.parameters.addParameterListener(magic_enum::enum_name<AvSynthAudioProcessor::Parameters::ChorusFeedback>().data(), this);
    processorRef.parameters.addParameterListener(magic_enum::enum_name<AvSynthAudioProcessor::Parameters::ChorusMix>().data(), this);
}

AvSynthAudioProcessorEditor::~AvSynthAudioProcessorEditor() {

    setLookAndFeel(nullptr);
    // Parameter-Listener entfernen
    processorRef.parameters.removeParameterListener(magic_enum::enum_name<AvSynthAudioProcessor::Parameters::Attack>().data(), this);
    processorRef.parameters.removeParameterListener(magic_enum::enum_name<AvSynthAudioProcessor::Parameters::Decay>().data(), this);
    processorRef.parameters.removeParameterListener(magic_enum::enum_name<AvSynthAudioProcessor::Parameters::Sustain>().data(), this);
    processorRef.parameters.removeParameterListener(magic_enum::enum_name<AvSynthAudioProcessor::Parameters::Release>().data(), this);

    processorRef.parameters.removeParameterListener(magic_enum::enum_name<AvSynthAudioProcessor::Parameters::ReverbRoomSize>().data(), this);
    processorRef.parameters.removeParameterListener(magic_enum::enum_name<AvSynthAudioProcessor::Parameters::ReverbDamping>().data(), this);
    processorRef.parameters.removeParameterListener(magic_enum::enum_name<AvSynthAudioProcessor::Parameters::ReverbWetLevel>().data(), this);
    processorRef.parameters.removeParameterListener(magic_enum::enum_name<AvSynthAudioProcessor::Parameters::ReverbDryLevel>().data(), this);
    processorRef.parameters.removeParameterListener(magic_enum::enum_name<AvSynthAudioProcessor::Parameters::ReverbWidth>().data(), this);

    // Chorus Parameter-Listener entfernen
    processorRef.parameters.removeParameterListener(magic_enum::enum_name<AvSynthAudioProcessor::Parameters::ChorusRate>().data(), this);
    processorRef.parameters.removeParameterListener(magic_enum::enum_name<AvSynthAudioProcessor::Parameters::ChorusDepth>().data(), this);
    processorRef.parameters.removeParameterListener(magic_enum::enum_name<AvSynthAudioProcessor::Parameters::ChorusFeedback>().data(), this);
    processorRef.parameters.removeParameterListener(magic_enum::enum_name<AvSynthAudioProcessor::Parameters::ChorusMix>().data(), this);
}

//==============================================================================
void AvSynthAudioProcessorEditor::paint(juce::Graphics &g) {
    // Mystischer Hintergrund mit mehreren Schichten
    auto area = getLocalBounds().toFloat();

    // Haupthintergrund
    auto backgroundGradient = juce::ColourGradient(
        juce::Colour(0xff0a0f1c).darker(0.3f), 0, 0,
        juce::Colour(0xff2d3e54).darker(0.1f), getWidth(), getHeight(), false);
    backgroundGradient.addColour(0.3, juce::Colour(0xff4a3472).withAlpha(0.1f));
    backgroundGradient.addColour(0.7, juce::Colour(0xff0a0f1c).brighter(0.05f));

    g.setGradientFill(backgroundGradient);
    g.fillAll();

    // Subtile Lichteffekte in den Ecken
    auto cornerGlow = juce::ColourGradient(
        juce::Colour(0xff64b5f6).withAlpha(0.05f), area.getTopLeft(),
        juce::Colours::transparentBlack, area.getCentre(), true);

    g.setGradientFill(cornerGlow);
    g.fillEllipse(area.getTopLeft().x - 50, area.getTopLeft().y - 50, 100, 100);
    g.fillEllipse(area.getTopRight().x - 50, area.getTopRight().y - 50, 100, 100);
    g.fillEllipse(area.getBottomLeft().x - 50, area.getBottomLeft().y - 50, 100, 100);
    g.fillEllipse(area.getBottomRight().x - 50, area.getBottomRight().y - 50, 100, 100);
}

void AvSynthAudioProcessorEditor::resized() {

    const int maxSliderWidth = 400;
    // Labels beschriften
    gainLabel.setText("Gain", juce::dontSendNotification);
    gainLabel.attachToComponent(&gainSlider, true);

    frequencyLabel.setText("Frequenz", juce::dontSendNotification);
    frequencyLabel.attachToComponent(&frequencySlider, true);

    oscTypeLabel.setText("Oszillator", juce::dontSendNotification);
    oscTypeLabel.attachToComponent(&oscTypeComboBox, true);

    spectrumLabel.setText("Frequency Spectrum", juce::dontSendNotification);
    spectrumLabel.attachToComponent(&spectrumComponent, true);

    lowCutFreqLabel.setText("Low Pass", juce::dontSendNotification);
    lowCutFreqLabel.attachToComponent(&lowCutFreqSlider, true);

    highCutFreqLabel.setText("High Pass", juce::dontSendNotification);
    highCutFreqLabel.attachToComponent(&highCutFreqSlider, true);

    adsrLabel.setText("ADSR Envelope", juce::dontSendNotification);
    adsrLabel.attachToComponent(&adsrComponent, true);

    reverbLabel.setText("Reverb", juce::dontSendNotification);
    reverbLabel.attachToComponent(&reverbComponent, true);

    chorusLabel.setText("Chorus Effect", juce::dontSendNotification);
    chorusLabel.attachToComponent(&chorusComponent, true);

    auto bounds = getLocalBounds().reduced(10);

    // Main controls
    auto presetButtonArea = bounds.removeFromTop(30);
    auto gainSliderArea = bounds.removeFromTop(40);
    auto frequencySliderArea = bounds.removeFromTop(40);
    auto oscTypeComboBoxArea = bounds.removeFromTop(40);
    auto lowCutFreqArea = bounds.removeFromTop(40);
    auto highCutFreqArea = bounds.removeFromTop(40);

    // ADSR Section
    auto adsrArea = bounds.removeFromTop(170); // Platz für ADSR Component + Label

    // Effects Section - Chorus und Reverb nebeneinander
    auto effectsArea = bounds.removeFromTop(200); // Platz für beide Effects + Labels
    auto chorusArea = effectsArea.removeFromLeft(effectsArea.getWidth() / 2); // Linke Hälfte für Chorus
    auto reverbArea = effectsArea; // Rechte Hälfte für Reverb

    // Keyboard and waveform
    auto keyboardArea = bounds.removeFromTop(80);

    auto visualizationArea = bounds;
    auto waveformArea = visualizationArea.removeFromTop(visualizationArea.getHeight() / 2);
    auto spectrumArea = visualizationArea;  // Rest für Spektrum

    // Set bounds for all components
    gainSlider.setBounds(gainSliderArea.removeFromLeft(std::min(maxSliderWidth, gainSliderArea.getWidth())));
    frequencySlider.setBounds(frequencySliderArea.removeFromLeft(std::min(maxSliderWidth, frequencySliderArea.getWidth())));
    oscTypeComboBox.setBounds(oscTypeComboBoxArea.removeFromLeft(std::min(maxSliderWidth, oscTypeComboBoxArea.getWidth())));
    lowCutFreqSlider.setBounds(lowCutFreqArea.removeFromLeft(std::min(maxSliderWidth, gainSliderArea.getWidth())));
    highCutFreqSlider.setBounds(highCutFreqArea.removeFromLeft(std::min(maxSliderWidth, frequencySliderArea.getWidth())));

    flutePresetButton.setBounds(presetButtonArea.removeFromLeft(120));

    // ADSR component
    adsrComponent.setBounds(adsrArea);

    // Effects components nebeneinander
    chorusComponent.setBounds(chorusArea);
    reverbComponent.setBounds(reverbArea);

    keyboardComponent.setBounds(keyboardArea);
    waveformComponent.setBounds(waveformArea);
    spectrumComponent.setBounds(spectrumArea);
}


//Hier werden alle Komponenten für die GUI hinzugefügt
std::vector<juce::Component *> AvSynthAudioProcessorEditor::GetComps() {
    return {&waveformComponent, &spectrumComponent, &spectrumLabel, &gainLabel, &gainSlider, &frequencySlider, &oscTypeComboBox,
            &lowCutFreqSlider, &highCutFreqSlider, &keyboardComponent, &highCutFreqLabel,
            &frequencyLabel, &oscTypeLabel, &lowCutFreqLabel, &adsrComponent, &adsrLabel, &reverbComponent, &reverbLabel, &flutePresetButton, &chorusComponent, &chorusLabel};
}

// AudioProcessorValueTreeState::Listener implementation
void AvSynthAudioProcessorEditor::parameterChanged(const juce::String& parameterID, float newValue) {
    // ADSR Component basierend auf Parameter-Änderungen aktualisieren
    if (parameterID == magic_enum::enum_name<AvSynthAudioProcessor::Parameters::Attack>().data()) {
        // Rück-Skalierung von Parameter-Wert zu normalisierten Wert (0-1)
        float normalizedValue = juce::jmap(newValue, 0.001f, 5.0f, 0.0f, 1.0f);
        normalizedValue = std::sqrt(juce::jlimit(0.0f, 1.0f, normalizedValue)); // Rückgängig quadratische Skalierung
        adsrComponent.setAttack(normalizedValue);
    }
    else if (parameterID == magic_enum::enum_name<AvSynthAudioProcessor::Parameters::Decay>().data()) {
        float normalizedValue = juce::jmap(newValue, 0.001f, 5.0f, 0.0f, 1.0f);
        normalizedValue = std::sqrt(juce::jlimit(0.0f, 1.0f, normalizedValue));
        adsrComponent.setDecay(normalizedValue);
    }
    else if (parameterID == magic_enum::enum_name<AvSynthAudioProcessor::Parameters::Sustain>().data()) {
        adsrComponent.setSustain(newValue); // Sustain ist bereits linear 0-1
    }
    else if (parameterID == magic_enum::enum_name<AvSynthAudioProcessor::Parameters::Release>().data()) {
        float normalizedValue = juce::jmap(newValue, 0.001f, 5.0f, 0.0f, 1.0f);
        normalizedValue = std::sqrt(juce::jlimit(0.0f, 1.0f, normalizedValue));
        adsrComponent.setRelease(normalizedValue);
    }
    // Reverb Component basierend auf Parameter-Änderungen aktualisieren
    else if (parameterID == magic_enum::enum_name<AvSynthAudioProcessor::Parameters::ReverbRoomSize>().data()) {
        reverbComponent.setRoomSize(newValue);
    }
    else if (parameterID == magic_enum::enum_name<AvSynthAudioProcessor::Parameters::ReverbDamping>().data()) {
        reverbComponent.setDamping(newValue);
    }
    else if (parameterID == magic_enum::enum_name<AvSynthAudioProcessor::Parameters::ReverbWetLevel>().data()) {
        reverbComponent.setWetLevel(newValue);
    }
    else if (parameterID == magic_enum::enum_name<AvSynthAudioProcessor::Parameters::ReverbDryLevel>().data()) {
        reverbComponent.setDryLevel(newValue);
    }
    else if (parameterID == magic_enum::enum_name<AvSynthAudioProcessor::Parameters::ReverbWidth>().data()) {
        reverbComponent.setWidth(newValue);
    }// Chorus Component basierend auf Parameter-Änderungen aktualisieren
    else if (parameterID == magic_enum::enum_name<AvSynthAudioProcessor::Parameters::ChorusRate>().data()) {
        chorusComponent.setRate(newValue);
    }
    else if (parameterID == magic_enum::enum_name<AvSynthAudioProcessor::Parameters::ChorusDepth>().data()) {
        chorusComponent.setDepth(newValue);
    }
    else if (parameterID == magic_enum::enum_name<AvSynthAudioProcessor::Parameters::ChorusFeedback>().data()) {
        chorusComponent.setFeedback(newValue);
    }
    else if (parameterID == magic_enum::enum_name<AvSynthAudioProcessor::Parameters::ChorusMix>().data()) {
        chorusComponent.setMix(newValue);
    }
}

// Flute Preset Methods
void AvSynthAudioProcessorEditor::setFlutePreset() {
    // Flöten-typische ADSR-Werte
    float fluteAttack = 0.08f;   // Sanfter, aber nicht zu langsamer Einsatz
    float fluteDecay = 0.3f;     // Mittlere Decay-Zeit
    float fluteSustain = 0.75f;  // Hoher Sustain-Level
    float fluteRelease = 0.4f;   // Mittlere Release-Zeit

    // Parameter setzen
    auto* attackParam = processorRef.parameters.getParameter(
        magic_enum::enum_name<AvSynthAudioProcessor::Parameters::Attack>().data());
    auto* decayParam = processorRef.parameters.getParameter(
        magic_enum::enum_name<AvSynthAudioProcessor::Parameters::Decay>().data());
    auto* sustainParam = processorRef.parameters.getParameter(
        magic_enum::enum_name<AvSynthAudioProcessor::Parameters::Sustain>().data());
    auto* releaseParam = processorRef.parameters.getParameter(
        magic_enum::enum_name<AvSynthAudioProcessor::Parameters::Release>().data());

    if (auto* floatParam = dynamic_cast<juce::AudioParameterFloat*>(attackParam)) {
        floatParam->setValueNotifyingHost(floatParam->convertTo0to1(fluteAttack));
    }
    if (auto* floatParam = dynamic_cast<juce::AudioParameterFloat*>(decayParam)) {
        floatParam->setValueNotifyingHost(floatParam->convertTo0to1(fluteDecay));
    }
    if (auto* floatParam = dynamic_cast<juce::AudioParameterFloat*>(sustainParam)) {
        floatParam->setValueNotifyingHost(fluteSustain);
    }
    if (auto* floatParam = dynamic_cast<juce::AudioParameterFloat*>(releaseParam)) {
        floatParam->setValueNotifyingHost(floatParam->convertTo0to1(fluteRelease));
    }
}

void AvSynthAudioProcessorEditor::setFluteFilterPreset() {
    // Flöten haben meist wenig sehr hohe Frequenzen
    float fluteLowPass = 8000.0f;  // Sanfte Höhen-Beschneidung
    float fluteHighPass = 80.0f;   // Leichte Bass-Beschneidung

    auto* lowPassParam = processorRef.parameters.getParameter(
        magic_enum::enum_name<AvSynthAudioProcessor::Parameters::LowPassFreq>().data());
    auto* highPassParam = processorRef.parameters.getParameter(
        magic_enum::enum_name<AvSynthAudioProcessor::Parameters::HighPassFreq>().data());

    if (auto* floatParam = dynamic_cast<juce::AudioParameterFloat*>(lowPassParam)) {
        floatParam->setValueNotifyingHost(floatParam->convertTo0to1(fluteLowPass));
    }
    if (auto* floatParam = dynamic_cast<juce::AudioParameterFloat*>(highPassParam)) {
        floatParam->setValueNotifyingHost(floatParam->convertTo0to1(fluteHighPass));
    }
}

void AvSynthAudioProcessorEditor::setFluteReverbPreset() {
    // Flöten-typische Reverb-Werte
    float fluteRoomSize = 0.6f;    // Mittlere Raumgröße (Konzertsaal)
    float fluteDamping = 0.4f;     // Wenig Dämpfung für Klarheit
    float fluteWetLevel = 0.25f;   // Moderater Reverb-Anteil
    float fluteDryLevel = 0.8f;    // Hoher Dry-Anteil für Direktheit
    float fluteWidth = 0.9f;       // Breites Stereo-Bild

    auto* roomSizeParam = processorRef.parameters.getParameter(
        magic_enum::enum_name<AvSynthAudioProcessor::Parameters::ReverbRoomSize>().data());
    auto* dampingParam = processorRef.parameters.getParameter(
        magic_enum::enum_name<AvSynthAudioProcessor::Parameters::ReverbDamping>().data());
    auto* wetLevelParam = processorRef.parameters.getParameter(
        magic_enum::enum_name<AvSynthAudioProcessor::Parameters::ReverbWetLevel>().data());
    auto* dryLevelParam = processorRef.parameters.getParameter(
        magic_enum::enum_name<AvSynthAudioProcessor::Parameters::ReverbDryLevel>().data());
    auto* widthParam = processorRef.parameters.getParameter(
        magic_enum::enum_name<AvSynthAudioProcessor::Parameters::ReverbWidth>().data());

    if (auto* floatParam = dynamic_cast<juce::AudioParameterFloat*>(roomSizeParam)) {
        floatParam->setValueNotifyingHost(fluteRoomSize);
    }
    if (auto* floatParam = dynamic_cast<juce::AudioParameterFloat*>(dampingParam)) {
        floatParam->setValueNotifyingHost(fluteDamping);
    }
    if (auto* floatParam = dynamic_cast<juce::AudioParameterFloat*>(wetLevelParam)) {
        floatParam->setValueNotifyingHost(fluteWetLevel);
    }
    if (auto* floatParam = dynamic_cast<juce::AudioParameterFloat*>(dryLevelParam)) {
        floatParam->setValueNotifyingHost(fluteDryLevel);
    }
    if (auto* floatParam = dynamic_cast<juce::AudioParameterFloat*>(widthParam)) {
        floatParam->setValueNotifyingHost(fluteWidth);
    }
}

void AvSynthAudioProcessorEditor::loadFlutePreset() {
    // Oszillator auf Flöte setzen
    auto* oscParam = processorRef.parameters.getParameter(
        magic_enum::enum_name<AvSynthAudioProcessor::Parameters::OscType>().data());
    if (auto* choiceParam = dynamic_cast<juce::AudioParameterChoice*>(oscParam)) {
        choiceParam->setValueNotifyingHost(choiceParam->convertTo0to1(4)); // Flute = Index 4
    }

    setFlutePreset();        // ADSR
    setFluteFilterPreset();  // Filter
    setFluteReverbPreset();  // Reverb
}









