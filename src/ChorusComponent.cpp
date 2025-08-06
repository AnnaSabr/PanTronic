/**
 * @file ChorusComponent.cpp
 * @brief Implementation of the ChorusComponent class
 */

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

ChorusComponent::~ChorusComponent()
{
}

void ChorusComponent::paint(juce::Graphics& g)
{
    auto area = getLocalBounds().toFloat();
    auto headerArea = area.removeFromTop(25);
    auto contentArea = area.reduced(5);

    // Mystical background with gradient (slightly different from Reverb for distinction)
    auto backgroundGradient = juce::ColourGradient(
        juce::Colour(0xff2d3e54).brighter(0.05f), 0, 0,          // Brighter blue at top
        juce::Colour(0xff0a0f1c).darker(0.05f), 0, getHeight(),  // Dark blue at bottom
        false
    );
    // Chorus gets a slightly different mystical accent
    backgroundGradient.addColour(0.4, juce::Colour(0xff4a3472).withAlpha(0.2f));
    backgroundGradient.addColour(0.8, juce::Colour(0xff1a2332).withAlpha(0.3f));

    g.setGradientFill(backgroundGradient);
    g.fillRoundedRectangle(area.reduced(1), 8.0f);

    // Chorus-specific glow effect (slightly more intense)
    for (float i = 4.0f; i > 0; i -= 0.6f) {
        auto alpha = (4.0f - i) / 4.0f * 0.1f;
        g.setColour(juce::Colour(0xff64b5f6).withAlpha(alpha));
        g.drawRoundedRectangle(area.expanded(i), 8.0f + i, 1.0f);
    }

    // Main border with varying glow
    g.setColour(juce::Colour(0xff64b5f6).withAlpha(0.7f));
    g.drawRoundedRectangle(area.reduced(1), 8.0f, 1.5f);

    // Header area with chorus-specific gradient
    auto headerGradient = juce::ColourGradient(
        juce::Colour(0xff64b5f6).withAlpha(0.25f), headerArea.getTopLeft(),
        juce::Colour(0xff4a3472).withAlpha(0.15f), headerArea.getBottomRight(),
        false
    );
    g.setGradientFill(headerGradient);
    g.fillRoundedRectangle(headerArea.reduced(1), 6.0f);

    // Header border with slight pulse effect
    g.setColour(juce::Colour(0xff64b5f6).withAlpha(0.5f));
    g.drawRoundedRectangle(headerArea.reduced(1), 6.0f, 1.0f);

    // "CHORUS" title with enhanced glow effect
    g.setFont(juce::Font(14.0f, juce::Font::bold));

    // More intense text glow for CHORUS
    g.setColour(juce::Colour(0xff64b5f6).withAlpha(0.5f));

    // Main text with slight shadow effect
    g.setColour(juce::Colour(0xff0a0f1c).withAlpha(0.4f)); // Shadow
    g.drawText("CHORUS", headerArea.translated(1, 1), juce::Justification::centred);

    g.setColour(juce::Colour(0xffc5d1de)); // Main text
    g.drawText("CHORUS", headerArea, juce::Justification::centred);

    // Chorus-specific corner lights (positioned differently than Reverb)
    auto cornerSize = 12.0f;
    auto cornerAlpha = 0.06f;

    auto cornerGlow = juce::ColourGradient(
        juce::Colour(0xff4a3472).withAlpha(cornerAlpha), 0, 0,
        juce::Colours::transparentBlack, cornerSize, cornerSize,
        true
    );

    g.setGradientFill(cornerGlow);
    // Central side lights for chorus effect
    float midY = contentArea.getCentreY();
    g.fillEllipse(contentArea.getX() - cornerSize/2, midY - cornerSize/2, cornerSize, cornerSize);
    g.fillEllipse(contentArea.getRight() - cornerSize/2, midY - cornerSize/2, cornerSize, cornerSize);

    // Subtle inner highlight line with chorus-specific color
    g.setColour(juce::Colour(0xff64b5f6).withAlpha(0.18f));
    g.drawRoundedRectangle(area.reduced(3), 6.0f, 1.0f);

    // Additional accent line at the top edge
    g.setColour(juce::Colour(0xff4a3472).withAlpha(0.3f));
    g.drawLine(area.getX() + 10, area.getY() + 2, area.getRight() - 10, area.getY() + 2, 1.0f);
}

void ChorusComponent::resized()
{
    auto bounds = getLocalBounds().reduced(10);
    bounds.removeFromTop(25); // Space for title
    auto sliderWidth = bounds.getWidth() / 4; // Four sliders total, not five

    // Position Rate slider and label
    auto rateArea = bounds.removeFromLeft(sliderWidth);
    rateLabel.setBounds(rateArea.removeFromBottom(20));
    rateSlider.setBounds(rateArea);

    // Position Depth slider and label
    auto depthArea = bounds.removeFromLeft(sliderWidth);
    depthLabel.setBounds(depthArea.removeFromBottom(20));
    depthSlider.setBounds(depthArea);

    // Position Feedback slider and label
    auto feedbackArea = bounds.removeFromLeft(sliderWidth);
    feedbackLabel.setBounds(feedbackArea.removeFromBottom(20));
    feedbackSlider.setBounds(feedbackArea);

    // Position Mix slider and label
    auto mixArea = bounds.removeFromLeft(sliderWidth);
    mixLabel.setBounds(mixArea.removeFromBottom(20));
    mixSlider.setBounds(mixArea);
}

void ChorusComponent::setRate(float rate)
{
    currentRate = rate;
    rateSlider.setValue(rate, juce::dontSendNotification);
}

void ChorusComponent::setDepth(float depth)
{
    currentDepth = depth;
    depthSlider.setValue(depth, juce::dontSendNotification);
}

void ChorusComponent::setFeedback(float feedback)
{
    currentFeedback = feedback;
    feedbackSlider.setValue(feedback, juce::dontSendNotification);
}

void ChorusComponent::setMix(float mix)
{
    currentMix = mix;
    mixSlider.setValue(mix, juce::dontSendNotification);
}

void ChorusComponent::updateParameters()
{
    // Notify external listeners if callback is set
    if (onParameterChanged)
    {
        onParameterChanged(currentRate, currentDepth, currentFeedback, currentMix);
    }
}