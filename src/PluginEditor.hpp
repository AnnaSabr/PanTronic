#pragma once

#include "PluginProcessor.hpp"
#include "WaveformComponent.hpp"
#include "ADSRComponent.hpp"
#include "ReverbComponent.hpp"

//==============================================================================
class AvSynthAudioProcessorEditor final : public juce::AudioProcessorEditor,
                                          public juce::AudioProcessorValueTreeState::Listener {
  public:
    explicit AvSynthAudioProcessorEditor(AvSynthAudioProcessor &);
    ~AvSynthAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics &) override;
    void resized() override;

    // AudioProcessorValueTreeState::Listener
    void parameterChanged(const juce::String& parameterID, float newValue) override;

  private:
    std::vector<juce::Component *> GetComps();
    void setupADSRComponent();
    void setupReverbComponent();

  private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AvSynthAudioProcessor &processorRef;

    juce::Label gainLabel;
    juce::Label frequencyLabel;
    juce::Label oscTypeLabel;
    juce::Label lowCutFreqLabel;
    juce::Label highCutFreqLabel;
    juce::Label adsrLabel;
    juce::Label reverbLabel;

    juce::Slider gainSlider;
    juce::AudioProcessorValueTreeState::SliderAttachment gainAttachment;
    juce::Slider frequencySlider;
    juce::AudioProcessorValueTreeState::SliderAttachment frequencyAttachment;
    juce::ComboBox oscTypeComboBox;
    juce::AudioProcessorValueTreeState::ComboBoxAttachment oscTypeAttachment;
    juce::Slider lowCutFreqSlider;
    juce::AudioProcessorValueTreeState::SliderAttachment lowCutFreqAttachment;
    juce::Slider highCutFreqSlider;
    juce::AudioProcessorValueTreeState::SliderAttachment highCutFreqAttachment;

    // ADSR Component
    ADSRComponent adsrComponent;

    // Reverb Component
    ReverbComponent reverbComponent;

    juce::MidiKeyboardComponent keyboardComponent;
    WaveformComponent waveformComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AvSynthAudioProcessorEditor)
};