/**
 * @file ADSRComponent.hpp
 * @brief Interactive ADSR envelope GUI component for JUCE
 */

#pragma once

#include "JuceHeader.h"

/**
 * @class ADSRComponent
 * @brief An interactive graphical component for displaying and editing ADSR parameters
 *
 * This class provides a visually appealing and interactive interface for editing
 * ADSR envelope parameters (Attack, Decay, Sustain, Release). Users can modify the
 * parameters by dragging control points in real-time.
 *
 * @inherit juce::Component
 */
class ADSRComponent : public juce::Component
{
public:
    /**
     * @brief Constructor
     *
     * Initializes the ADSRComponent with default values and sets up basic configuration.
     */
    ADSRComponent();

    /**
     * @brief Destructor
     */
    ~ADSRComponent() override;

    /**
     * @brief Paints the ADSR component
     *
     * Renders the ADSR curve with filled area, control points and grid lines.
     * Uses gradients and visual effects for an appealing presentation.
     *
     * @param g Graphics context for drawing
     */
    void paint(juce::Graphics& g) override;

    /**
     * @brief Called when the component size changes
     */
    void resized() override;

    /**
     * @brief Handles mouse click events
     *
     * Detects which control point was clicked and starts drag mode.
     *
     * @param event Mouse event with position and buttons
     */
    void mouseDown(const juce::MouseEvent& event) override;

    /**
     * @brief Handles mouse drag events
     *
     * Updates ADSR parameters based on mouse movement and
     * calls the parameter changed callback.
     *
     * @param event Mouse event with current position
     */
    void mouseDrag(const juce::MouseEvent& event) override;

    /**
     * @brief Handles mouse release events
     *
     * Ends the current drag mode and updates the display.
     *
     * @param event Mouse event
     */
    void mouseUp(const juce::MouseEvent& event) override;

    /**
     * @brief Handles mouse movement without clicked buttons
     *
     * Changes mouse cursor when hovering over control points.
     *
     * @param event Mouse event with position
     */
    void mouseMove(const juce::MouseEvent& event) override;

    /**
     * @brief Sets the attack parameter
     *
     * @param attack Attack time normalized (0.0 to 1.0)
     */
    void setAttack(float attack);

    /**
     * @brief Sets the decay parameter
     *
     * @param decay Decay time normalized (0.0 to 1.0)
     */
    void setDecay(float decay);

    /**
     * @brief Sets the sustain parameter
     *
     * @param sustain Sustain level normalized (0.0 to 1.0)
     */
    void setSustain(float sustain);

    /**
     * @brief Sets the release parameter
     *
     * @param release Release time normalized (0.0 to 1.0)
     */
    void setRelease(float release);

    /**
     * @brief Gets the current attack value
     * @return Attack time normalized (0.0 to 1.0)
     */
    float getAttack() const { return attackValue; }

    /**
     * @brief Gets the current decay value
     * @return Decay time normalized (0.0 to 1.0)
     */
    float getDecay() const { return decayValue; }

    /**
     * @brief Gets the current sustain value
     * @return Sustain level normalized (0.0 to 1.0)
     */
    float getSustain() const { return sustainValue; }

    /**
     * @brief Gets the current release value
     * @return Release time normalized (0.0 to 1.0)
     */
    float getRelease() const { return releaseValue; }

    /**
     * @brief Callback function for parameter changes
     *
     * Called when ADSR parameters change through user interaction.
     * Lambda function with signature: void(float attack, float decay, float sustain, float release)
     */
    std::function<void(float attack, float decay, float sustain, float release)> onParameterChanged;

    /**
     * @brief Updates the color scheme of the component
     *
     * @param primary Primary color for curve and control points
     * @param secondary Secondary color for grid and accents
     */
    void updateColors(juce::Colour primary, juce::Colour secondary);

private:
    /**
     * @brief Attack parameter normalized (0.0 to 1.0)
     */
    float attackValue = 0.1f;

    /**
     * @brief Decay parameter normalized (0.0 to 1.0)
     */
    float decayValue = 0.3f;

    /**
     * @brief Sustain parameter normalized (0.0 to 1.0)
     */
    float sustainValue = 0.7f;

    /**
     * @brief Release parameter normalized (0.0 to 1.0)
     */
    float releaseValue = 0.5f;

    /**
     * @enum DragMode
     * @brief Enumeration of different drag modes for ADSR parameters
     */
    enum class DragMode {
        None,       ///< No active drag mode
        Attack,     ///< Attack parameter is being dragged
        Decay,      ///< Decay parameter is being dragged
        Sustain,    ///< Sustain parameter is being dragged
        Release     ///< Release parameter is being dragged
    };

    /**
     * @brief Current drag mode
     */
    DragMode currentDragMode = DragMode::None;

    /**
     * @brief Last mouse position for drag calculation
     */
    juce::Point<float> lastMousePos;

    /**
     * @brief Primary color for curve and control points
     */
    juce::Colour primaryColor = juce::Colours::darkblue;

    /**
     * @brief Secondary color for grid and accents
     */
    juce::Colour secondaryColor = juce::Colours::darkblue;

    /**
     * @brief Creates the graphical path for the ADSR curve
     *
     * Calculates and connects all ADSR control points to a continuous path.
     *
     * @return juce::Path Graphical path of the ADSR curve
     */
    juce::Path createADSRPath() const;

    /**
     * @brief Determines which control point is hit at a position
     *
     * @param position Position to test
     * @return DragMode Corresponding drag mode or None if no hit
     */
    DragMode getHitTest(juce::Point<float> position) const;

    /**
     * @brief Calculates the screen position of the attack control point
     * @return juce::Point<float> Position of attack point
     */
    juce::Point<float> getAttackPoint() const;

    /**
     * @brief Calculates the screen position of the decay control point
     * @return juce::Point<float> Position of decay point
     */
    juce::Point<float> getDecayPoint() const;

    /**
     * @brief Calculates the screen position of the sustain control point
     * @return juce::Point<float> Position of sustain point
     */
    juce::Point<float> getSustainPoint() const;

    /**
     * @brief Calculates the screen position of the release control point
     * @return juce::Point<float> Position of release point
     */
    juce::Point<float> getReleasePoint() const;

    /**
     * @brief Maps screen coordinates to ADSR values
     *
     * @param screenValue Screen coordinate
     * @param isTime true for time parameters, false for level parameters
     * @return float Normalized ADSR value (0.0 to 1.0)
     */
    float mapToADSRValue(float screenValue, bool isTime) const;

    /**
     * @brief Maps ADSR values to screen coordinates
     *
     * @param adsrValue Normalized ADSR value (0.0 to 1.0)
     * @param isTime true for time parameters, false for level parameters
     * @return float Screen coordinate
     */
    float mapFromADSRValue(float adsrValue, bool isTime) const;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ADSRComponent)
};