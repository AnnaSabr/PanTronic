#include "ChorusComponent.hpp"

ChorusComponent::ChorusComponent()
{
    // Setup Rate Slider (LFO Speed)
    rateSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    rateSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    rateSlider.setRange(0.1, 10.0, 0.1);
    rateSlider.setValue(currentRate);
    rateSlider.onValueChange = [this] {
        currentRate = static_cast<float>(rateSlider.getValue());
        updateParameters();
    };
    addAndMakeVisible(rateSlider);

    rateLabel.setText("Rate", juce::dontSendNotification);
    rateLabel.setJustificationType(juce::Justification::centred);
    rateLabel.setFont(12.0f);
    addAndMakeVisible(rateLabel);

    // Setup Depth Slider (Modulation Depth)
    depthSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    depthSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    depthSlider.setRange(0.0, 1.0, 0.01);
    depthSlider.setValue(currentDepth);
    depthSlider.onValueChange = [this] {
        currentDepth = static_cast<float>(depthSlider.getValue());
        updateParameters();
    };
    addAndMakeVisible(depthSlider);
    depthLabel.setText("Depth", juce::dontSendNotification);
    depthLabel.setJustificationType(juce::Justification::centred);
    depthLabel.setFont(12.0f);
    addAndMakeVisible(depthLabel);

    // Setup Feedback Slider
    feedbackSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    feedbackSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    feedbackSlider.setRange(0.0, 0.95, 0.01);
    feedbackSlider.setValue(currentFeedback);
    feedbackSlider.onValueChange = [this] {
        currentFeedback = static_cast<float>(feedbackSlider.getValue());
        updateParameters();
    };
    addAndMakeVisible(feedbackSlider);

    feedbackLabel.setText("Feedback", juce::dontSendNotification);
    feedbackLabel.setJustificationType(juce::Justification::centred);
    feedbackLabel.setFont(12.0f);
    addAndMakeVisible(feedbackLabel);
    // Setup Mix Slider (Dry/Wet)

    mixSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    mixSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    mixSlider.setRange(0.0, 1.0, 0.01);
    mixSlider.setValue(currentMix);
    mixSlider.onValueChange = [this] {

        currentMix = static_cast<float>(mixSlider.getValue());

        updateParameters();

    };
    addAndMakeVisible(mixSlider);
    mixLabel.setText("Mix", juce::dontSendNotification);
    mixLabel.setJustificationType(juce::Justification::centred);
    mixLabel.setFont(12.0f);
    addAndMakeVisible(mixLabel);
}

ChorusComponent::~ChorusComponent(){

}

void ChorusComponent::paint(juce::Graphics& g){
    g.fillAll(juce::Colours::darkgrey.darker());
    g.setColour(juce::Colours::white.withAlpha(0.8f));
    g.drawRoundedRectangle(getLocalBounds().toFloat().reduced(2), 5.0f, 2.0f);
    g.setFont(14.0f);
    g.setColour(juce::Colours::white);
    g.drawText("CHORUS", getLocalBounds().removeFromTop(20), juce::Justification::centred);
}

void ChorusComponent::resized(){
    auto bounds = getLocalBounds().reduced(10);
    bounds.removeFromTop(25); // Space for title
    auto sliderWidth = bounds.getWidth() / 5;
    //auto sliderHeight = bounds.getHeight() - 30; // Space for labels
    // Rate
    auto rateArea = bounds.removeFromLeft(sliderWidth);
    rateLabel.setBounds(rateArea.removeFromBottom(20));
    rateSlider.setBounds(rateArea);
    // Depth
    auto depthArea = bounds.removeFromLeft(sliderWidth);
    depthLabel.setBounds(depthArea.removeFromBottom(20));
    depthSlider.setBounds(depthArea);
    // Feedback
    auto feedbackArea = bounds.removeFromLeft(sliderWidth);
    feedbackLabel.setBounds(feedbackArea.removeFromBottom(20));
    feedbackSlider.setBounds(feedbackArea);
    // Mix
    auto mixArea = bounds.removeFromLeft(sliderWidth);
    mixLabel.setBounds(mixArea.removeFromBottom(20));
    mixSlider.setBounds(mixArea);
}

void ChorusComponent::setRate(float rate){
    currentRate = rate;
    rateSlider.setValue(rate, juce::dontSendNotification);
}

void ChorusComponent::setDepth(float depth){
    currentDepth = depth;
    depthSlider.setValue(depth, juce::dontSendNotification);
}

void ChorusComponent::setFeedback(float feedback){
    currentFeedback = feedback;
    feedbackSlider.setValue(feedback, juce::dontSendNotification);
}

void ChorusComponent::setMix(float mix){
    currentMix = mix;
    mixSlider.setValue(mix, juce::dontSendNotification);
}

void ChorusComponent::updateParameters()
{
    if (onParameterChanged){
        onParameterChanged(currentRate, currentDepth, currentFeedback, currentMix);

    }
}