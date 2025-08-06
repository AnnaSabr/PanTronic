/**
 * @file ReverbComponent.hpp
 * @brief Header file for the reverb control component
 *
 * This file contains the declaration of the ReverbComponent class, which provides
 * a user interface for controlling reverb effect parameters.
 *
 * @author AvSynth Development Team
 * @version 1.0
 * @date 2024
 */

#pragma once

#include <JuceHeader.h>

/**
 * @class ReverbComponent
 * @brief User interface component for reverb effect controls
 *
 * This class provides a graphical interface with sliders for controlling various
 * reverb parameters including room size, damping, wet/dry levels, and stereo width.
 * Features a mystical-themed visual design with gradients and glow effects.
 */
class ReverbComponent : public juce::Component
{
public:
    /**
     * @brief Constructor for ReverbComponent
     *
     * Initializes all slider controls and labels for the reverb parameters.
     * Sets up default values and styling for the user interface elements.
     */
    ReverbComponent();

    /**
     * @brief Destructor for ReverbComponent
     */
    ~ReverbComponent() override;

    /**
     * @brief Paint method for custom drawing of the component
     *
     * Renders a mystical-themed background with gradients, glow effects,
     * and the "REVERB" title header.
     *
     * @param g Graphics context for drawing
     */
    void paint(juce::Graphics& g) override;

    /**
     * @brief Resized method to layout child components
     *
     * Arranges the five reverb parameter sliders and their labels
     * in a horizontal layout within the component bounds.
     */
    void resized() override;

    // Setter methods for reverb parameters

    /**
     * @brief Sets the room size parameter value
     * @param roomSize Room size value (0.0 to 1.0)
     */
    void setRoomSize(float roomSize);

    /**
     * @brief Sets the damping parameter value
     * @param damping Damping value (0.0 to 1.0)
     */
    void setDamping(float damping);

    /**
     * @brief Sets the wet level parameter value
     * @param wetLevel Wet level value (0.0 to 1.0)
     */
    void setWetLevel(float wetLevel);

    /**
     * @brief Sets the dry level parameter value
     * @param dryLevel Dry level value (0.0 to 1.0)
     */
    void setDryLevel(float dryLevel);

    /**
     * @brief Sets the width parameter value
     * @param width Stereo width value (0.0 to 1.0)
     */
    void setWidth(float width);

    /**
     * @brief Callback function for parameter changes
     *
     * This function object is called whenever any reverb parameter changes.
     * It passes all current parameter values to the registered callback.
     */
    std::function<void(float roomSize, float damping, float wetLevel, float dryLevel, float width)> onParameterChanged;

private:
    /**
     * @brief Internal method called when any parameter changes
     *
     * Collects all current slider values and invokes the onParameterChanged callback
     * if it has been set by the parent component.
     */
    void parameterChanged();

    // Slider controls for reverb parameters
    juce::Slider roomSizeSlider;   ///< Slider for room size control
    juce::Slider dampingSlider;    ///< Slider for damping control
    juce::Slider wetLevelSlider;   ///< Slider for wet level control
    juce::Slider dryLevelSlider;   ///< Slider for dry level control
    juce::Slider widthSlider;      ///< Slider for stereo width control

    // Labels for the sliders
    juce::Label roomSizeLabel;     ///< Label for room size slider
    juce::Label dampingLabel;      ///< Label for damping slider
    juce::Label wetLevelLabel;     ///< Label for wet level slider
    juce::Label dryLevelLabel;     ///< Label for dry level slider
    juce::Label widthLabel;        ///< Label for width slider

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ReverbComponent)
};