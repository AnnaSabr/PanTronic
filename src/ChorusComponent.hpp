/**
 * @file ChorusComponent.hpp
 * @brief GUI component for chorus effect parameters
 */

#pragma once

#include "JuceHeader.h"

/**
 * @class ChorusComponent
 * @brief GUI component for controlling chorus effect parameters
 *
 * This class provides a user-friendly interface with four rotary sliders
 * to control chorus parameters (Rate, Depth, Feedback, Mix).
 * The component features a mystical design theme with gradients
 * and glow effects.
 *
 * @inherit juce::Component
 */
class ChorusComponent : public juce::Component {
public:
    /**
     * @brief Constructor
     *
     * Initializes all sliders and labels with default values and
     * configures event handlers for parameter changes.
     */
    ChorusComponent();

    /**
     * @brief Destructor
     */
    ~ChorusComponent() override;

    /**
     * @brief Paints the chorus component
     *
     * Renders a mystical background with gradients, glow effects
     * and the "CHORUS" title. Uses special colors and effects that
     * distinguish it from the reverb design.
     *
     * @param g Graphics context for drawing
     */
    void paint(juce::Graphics& g) override;

    /**
     * @brief Organizes the layout of child components
     *
     * Positions the four sliders (Rate, Depth, Feedback, Mix) evenly
     * across the available space with corresponding labels below.
     */
    void resized() override;

    /**
     * @brief Sets the LFO rate of the chorus effect
     *
     * @param rate LFO frequency in Hz (0.1 to 10.0)
     */
    void setRate(float rate);

    /**
     * @brief Sets the modulation depth of the chorus effect
     *
     * @param depth Modulation depth normalized (0.0 to 1.0)
     */
    void setDepth(float depth);

    /**
     * @brief Sets the feedback amount of the chorus effect
     *
     * @param feedback Feedback level normalized (0.0 to 0.95)
     */
    void setFeedback(float feedback);

    /**
     * @brief Sets the dry/wet mix ratio
     *
     * @param mix Mix ratio normalized (0.0 = dry only, 1.0 = wet only)
     */
    void setMix(float mix);

    /**
     * @brief Callback function for parameter changes
     *
     * Called when chorus parameters change through user interaction.
     * Lambda function with signature: void(float rate, float depth, float feedback, float mix)
     */
    std::function<void(float rate, float depth, float feedback, float mix)> onParameterChanged;

private:
    /**
     * @brief Calls the parameter changed callback
     *
     * Central method called for all parameter changes that
     * notifies external callback with all current values.
     */
    void updateParameters();

    /**
     * @brief Rotary slider for LFO rate (0.1 - 10.0 Hz)
     */
    juce::Slider rateSlider;

    /**
     * @brief Rotary slider for modulation depth (0.0 - 1.0)
     */
    juce::Slider depthSlider;

    /**
     * @brief Rotary slider for feedback level (0.0 - 0.95)
     */
    juce::Slider feedbackSlider;

    /**
     * @brief Rotary slider for dry/wet mix (0.0 - 1.0)
     */
    juce::Slider mixSlider;

    /**
     * @brief Label for rate parameter
     */
    juce::Label rateLabel;

    /**
     * @brief Label for depth parameter
     */
    juce::Label depthLabel;

    /**
     * @brief Label for feedback parameter
     */
    juce::Label feedbackLabel;

    /**
     * @brief Label for mix parameter
     */
    juce::Label mixLabel;

    /**
     * @brief Current rate value (LFO frequency in Hz)
     */
    float currentRate = 0.5f;

    /**
     * @brief Current depth value (modulation depth 0.0-1.0)
     */
    float currentDepth = 0.5f;

    /**
     * @brief Current feedback value (0.0-0.95)
     */
    float currentFeedback = 0.3f;

    /**
     * @brief Current mix value (dry/wet 0.0-1.0)
     */
    float currentMix = 0.5f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ChorusComponent)
};