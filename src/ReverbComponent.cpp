#include "ReverbComponent.hpp"

ReverbComponent::ReverbComponent()
{
    // Room Size Slider Setup
    roomSizeSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    roomSizeSlider.setRange(0.0, 1.0, 0.01);
    roomSizeSlider.setValue(0.5);
    roomSizeSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 60, 20);
    roomSizeSlider.onValueChange = [this] { parameterChanged(); };
    addAndMakeVisible(roomSizeSlider);

    roomSizeLabel.setText("Room Size", juce::dontSendNotification);
    roomSizeLabel.attachToComponent(&roomSizeSlider, true);
    addAndMakeVisible(roomSizeLabel);

    // Damping Slider Setup
    dampingSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    dampingSlider.setRange(0.0, 1.0, 0.01);
    dampingSlider.setValue(0.5);
    dampingSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 60, 20);
    dampingSlider.onValueChange = [this] { parameterChanged(); };
    addAndMakeVisible(dampingSlider);

    dampingLabel.setText("Damping", juce::dontSendNotification);
    dampingLabel.attachToComponent(&dampingSlider, true);
    addAndMakeVisible(dampingLabel);

    // Wet Level Slider Setup
    wetLevelSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    wetLevelSlider.setRange(0.0, 1.0, 0.01);
    wetLevelSlider.setValue(0.33);
    wetLevelSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 60, 20);
    wetLevelSlider.onValueChange = [this] { parameterChanged(); };
    addAndMakeVisible(wetLevelSlider);

    wetLevelLabel.setText("Wet Level", juce::dontSendNotification);
    wetLevelLabel.attachToComponent(&wetLevelSlider, true);
    addAndMakeVisible(wetLevelLabel);

    // Dry Level Slider Setup
    dryLevelSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    dryLevelSlider.setRange(0.0, 1.0, 0.01);
    dryLevelSlider.setValue(0.4);
    dryLevelSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 60, 20);
    dryLevelSlider.onValueChange = [this] { parameterChanged(); };
    addAndMakeVisible(dryLevelSlider);

    dryLevelLabel.setText("Dry Level", juce::dontSendNotification);
    dryLevelLabel.attachToComponent(&dryLevelSlider, true);
    addAndMakeVisible(dryLevelLabel);

    // Width Slider Setup
    widthSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    widthSlider.setRange(0.0, 1.0, 0.01);
    widthSlider.setValue(1.0);
    widthSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 60, 20);
    widthSlider.onValueChange = [this] { parameterChanged(); };
    addAndMakeVisible(widthSlider);

    widthLabel.setText("Width", juce::dontSendNotification);
    widthLabel.attachToComponent(&widthSlider, true);
    addAndMakeVisible(widthLabel);
}

ReverbComponent::~ReverbComponent()
{
}

void ReverbComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkgrey.darker());

    g.setColour(juce::Colours::white);
    g.setFont(14.0f);
    g.drawText("Reverb", getLocalBounds().removeFromTop(20), juce::Justification::centred);
}

void ReverbComponent::resized()
{
    auto bounds = getLocalBounds();
    bounds.removeFromTop(25); // Space for title
    bounds.reduce(10, 5);

    const int sliderHeight = 30;
    const int spacing = 5;

    roomSizeSlider.setBounds(bounds.removeFromTop(sliderHeight));
    bounds.removeFromTop(spacing);

    dampingSlider.setBounds(bounds.removeFromTop(sliderHeight));
    bounds.removeFromTop(spacing);

    wetLevelSlider.setBounds(bounds.removeFromTop(sliderHeight));
    bounds.removeFromTop(spacing);

    dryLevelSlider.setBounds(bounds.removeFromTop(sliderHeight));
    bounds.removeFromTop(spacing);

    widthSlider.setBounds(bounds.removeFromTop(sliderHeight));
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