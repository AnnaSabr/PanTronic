#include "ReverbComponent.hpp"

ReverbComponent::ReverbComponent()
{
    // Room Size Knob Setup
    roomSizeSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    roomSizeSlider.setRange(0.0, 1.0, 0.01);
    roomSizeSlider.setValue(0.5);
    roomSizeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    roomSizeSlider.onValueChange = [this] { parameterChanged(); };
    addAndMakeVisible(roomSizeSlider);

    roomSizeLabel.setText("Room Size", juce::dontSendNotification);
    roomSizeLabel.setJustificationType(juce::Justification::centred);
    roomSizeLabel.setFont(12.0f);
    addAndMakeVisible(roomSizeLabel);

    // Damping Knob Setup
    dampingSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    dampingSlider.setRange(0.0, 1.0, 0.01);
    dampingSlider.setValue(0.5);
    dampingSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    dampingSlider.onValueChange = [this] { parameterChanged(); };
    addAndMakeVisible(dampingSlider);

    dampingLabel.setText("Damping", juce::dontSendNotification);
    dampingLabel.setJustificationType(juce::Justification::centred);
    dampingLabel.setFont(12.0f);
    addAndMakeVisible(dampingLabel);

    // Wet Level Knob Setup
    wetLevelSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    wetLevelSlider.setRange(0.0, 1.0, 0.01);
    wetLevelSlider.setValue(0.33);
    wetLevelSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    wetLevelSlider.onValueChange = [this] { parameterChanged(); };
    addAndMakeVisible(wetLevelSlider);

    wetLevelLabel.setText("Wet Level", juce::dontSendNotification);
    wetLevelLabel.setJustificationType(juce::Justification::centred);
    wetLevelLabel.setFont(12.0f);
    addAndMakeVisible(wetLevelLabel);

    // Dry Level Knob Setup
    dryLevelSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    dryLevelSlider.setRange(0.0, 1.0, 0.01);
    dryLevelSlider.setValue(0.4);
    dryLevelSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    dryLevelSlider.onValueChange = [this] { parameterChanged(); };
    addAndMakeVisible(dryLevelSlider);

    dryLevelLabel.setText("Dry Level", juce::dontSendNotification);
    dryLevelLabel.setJustificationType(juce::Justification::centred);
    dryLevelLabel.setFont(12.0f);
    addAndMakeVisible(dryLevelLabel);

    // Width Knob Setup
    widthSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    widthSlider.setRange(0.0, 1.0, 0.01);
    widthSlider.setValue(1.0);
    widthSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    widthSlider.onValueChange = [this] { parameterChanged(); };
    addAndMakeVisible(widthSlider);

    widthLabel.setText("Width", juce::dontSendNotification);
    widthLabel.setJustificationType(juce::Justification::centred);
    widthLabel.setFont(12.0f);
    addAndMakeVisible(widthLabel);
}

ReverbComponent::~ReverbComponent(){
}

void ReverbComponent::paint(juce::Graphics& g) {
    auto area = getLocalBounds().toFloat();
    auto headerArea = area.removeFromTop(25);
    auto contentArea = area.reduced(5);

    // Mystischer Hintergrund mit Verlauf
    auto backgroundGradient = juce::ColourGradient(
        juce::Colour(0xff1a2332).brighter(0.1f), 0, 0,           // Mittleres Blau oben
        juce::Colour(0xff0a0f1c).darker(0.1f), 0, getHeight(),   // Dunkles Blau unten
        false
    );
    // Mystischen Lila-Akzent in der Mitte hinzufügen
    backgroundGradient.addColour(0.5, juce::Colour(0xff4a3472).withAlpha(0.15f));

    g.setGradientFill(backgroundGradient);
    g.fillRoundedRectangle(area.reduced(1), 8.0f);

    // Subtiler Glow-Effekt um die gesamte Komponente
    for (float i = 3.0f; i > 0; i -= 0.5f) {
        auto alpha = (3.0f - i) / 3.0f * 0.08f;
        g.setColour(juce::Colour(0xff64b5f6).withAlpha(alpha));
        g.drawRoundedRectangle(area.expanded(i), 8.0f + i, 1.0f);
    }

    // Hauptumrandung mit mystischem Glow
    g.setColour(juce::Colour(0xff64b5f6).withAlpha(0.6f));
    g.drawRoundedRectangle(area.reduced(1), 8.0f, 1.5f);

    // Header-Bereich mit eigenem Verlauf
    auto headerGradient = juce::ColourGradient(
        juce::Colour(0xff4a3472).withAlpha(0.3f), headerArea.getTopLeft(),
        juce::Colour(0xff64b5f6).withAlpha(0.1f), headerArea.getBottomRight(),
        false
    );
    g.setGradientFill(headerGradient);
    g.fillRoundedRectangle(headerArea.reduced(1), 6.0f);

    // Header-Umrandung
    g.setColour(juce::Colour(0xff64b5f6).withAlpha(0.4f));
    g.drawRoundedRectangle(headerArea.reduced(1), 6.0f, 1.0f);

    // Intensiverer Text-Glow für CHORUS
    g.setColour(juce::Colour(0xff64b5f6).withAlpha(0.5f));

    // Haupttext mit leichtem Schatten-Effekt
    g.setColour(juce::Colour(0xff0a0f1c).withAlpha(0.4f)); // Schatten
    g.drawText("REVERB", headerArea.translated(1, 1), juce::Justification::centred);

    g.setColour(juce::Colour(0xffc5d1de)); // Haupttext
    g.drawText("REVERB", headerArea, juce::Justification::centred);

    // Optionale mystische Akzente in den Ecken des Content-Bereichs
    auto cornerSize = 15.0f;
    auto cornerAlpha = 0.05f;

    // Lichteffekte in den Ecken
    auto cornerGlow = juce::ColourGradient(
        juce::Colour(0xff64b5f6).withAlpha(cornerAlpha), 0, 0,
        juce::Colours::transparentBlack, cornerSize, cornerSize,
        true
    );

    g.setGradientFill(cornerGlow);
    // Obere linke Ecke
    g.fillEllipse(contentArea.getTopLeft().x, contentArea.getTopLeft().y, cornerSize, cornerSize);
    // Obere rechte Ecke
    g.fillEllipse(contentArea.getTopRight().x - cornerSize, contentArea.getTopRight().y, cornerSize, cornerSize);
    // Untere linke Ecke
    g.fillEllipse(contentArea.getBottomLeft().x, contentArea.getBottomLeft().y - cornerSize, cornerSize, cornerSize);
    // Untere rechte Ecke
    g.fillEllipse(contentArea.getBottomRight().x - cornerSize, contentArea.getBottomRight().y - cornerSize, cornerSize, cornerSize);

    // Subtile innere Highlightlinie
    g.setColour(juce::Colour(0xff64b5f6).withAlpha(0.15f));
    g.drawRoundedRectangle(area.reduced(3), 6.0f, 1.0f);
}

void ReverbComponent::resized()
{
    auto bounds = getLocalBounds().reduced(10);
    bounds.removeFromTop(25);
    auto sliderWidth = bounds.getWidth() / 5;
    //auto sliderHeight = bounds.getHeight() - 30; // Space for labels

    // Room Size
    auto roomSizeArea = bounds.removeFromLeft(sliderWidth);
    roomSizeLabel.setBounds(roomSizeArea.removeFromBottom(20));
    roomSizeSlider.setBounds(roomSizeArea);

    // Damping
    auto dampingArea = bounds.removeFromLeft(sliderWidth);
    dampingLabel.setBounds(dampingArea.removeFromBottom(20));
    dampingSlider.setBounds(dampingArea);

    // Wet Level
    auto wetLevelArea = bounds.removeFromLeft(sliderWidth);
    wetLevelLabel.setBounds(wetLevelArea.removeFromBottom(20));
    wetLevelSlider.setBounds(wetLevelArea);

    // Dry Level
    auto dryLevelArea = bounds.removeFromLeft(sliderWidth);
    dryLevelLabel.setBounds(dryLevelArea.removeFromBottom(20));
    dryLevelSlider.setBounds(dryLevelArea);

    // Width
    auto widthArea = bounds.removeFromLeft(sliderWidth);
    widthLabel.setBounds(widthArea.removeFromBottom(20));
    widthSlider.setBounds(widthArea);
}

void ReverbComponent::setRoomSize(float roomSize)
{
    roomSizeSlider.setValue(roomSize, juce::dontSendNotification);
}

void ReverbComponent::setDamping(float damping)
{
    dampingSlider.setValue(damping, juce::dontSendNotification);
}

void ReverbComponent::setWetLevel(float wetLevel)
{
    wetLevelSlider.setValue(wetLevel, juce::dontSendNotification);
}

void ReverbComponent::setDryLevel(float dryLevel)
{
    dryLevelSlider.setValue(dryLevel, juce::dontSendNotification);
}

void ReverbComponent::setWidth(float width)
{
    widthSlider.setValue(width, juce::dontSendNotification);
}

void ReverbComponent::parameterChanged()
{
    if (onParameterChanged)
    {
        onParameterChanged(
            static_cast<float>(roomSizeSlider.getValue()),
            static_cast<float>(dampingSlider.getValue()),
            static_cast<float>(wetLevelSlider.getValue()),
            static_cast<float>(dryLevelSlider.getValue()),
            static_cast<float>(widthSlider.getValue())
        );
    }
}