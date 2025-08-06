#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

/**
 * @file MysticalLookAndFeel.hpp
 * @brief Custom look-and-feel implementation for mystical-themed GUI
 * @author Your Name
 * @date 2024
 */

/**
 * @class MysticalLookAndFeel
 * @brief Custom JUCE look-and-feel providing mystical visual styling
 *
 * This class extends JUCE's LookAndFeel_V4 to provide a cohesive mystical
 * visual theme throughout the synthesizer interface. It implements custom
 * rendering for sliders, buttons, combo boxes, and labels with:
 * - Dark blue color palette with purple accents
 * - Gradient backgrounds and mystical glow effects
 * - Enhanced visual feedback for user interactions
 * - Atmospheric lighting and shadow effects
 *
 * @inherits juce::LookAndFeel_V4
 */
class MysticalLookAndFeel : public juce::LookAndFeel_V4
{
public:
    /**
     * @brief Constructor
     *
     * Initializes the mystical color scheme and sets default colors
     * for all standard JUCE components (windows, labels, sliders, etc.).
     */
    MysticalLookAndFeel();

    /**
     * @brief Default destructor
     */
    ~MysticalLookAndFeel() override = default;

    //==============================================================================
    // Slider Customization

    /**
     * @brief Custom slider track rendering
     * @param g Graphics context for drawing
     * @param x X position of slider
     * @param y Y position of slider
     * @param width Width of slider
     * @param height Height of slider
     * @param sliderPos Current position of slider thumb
     * @param minSliderPos Minimum thumb position
     * @param maxSliderPos Maximum thumb position
     * @param style Slider style (linear horizontal/vertical, etc.)
     * @param slider Reference to the slider component
     *
     * Renders slider tracks with:
     * - Gradient background from dark to medium blue
     * - Active track portion with mystical purple-to-glow gradient
     * - Subtle glow effects around active portions
     * - Rounded corners for modern appearance
     */
    void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
                         float sliderPos, float minSliderPos, float maxSliderPos,
                         const juce::Slider::SliderStyle style, juce::Slider& slider) override;

    /**
     * @brief Custom slider thumb rendering
     * @param g Graphics context for drawing
     * @param x X position of slider
     * @param y Y position of slider
     * @param width Width of slider
     * @param height Height of slider
     * @param sliderPos Current position of slider thumb
     * @param minSliderPos Minimum thumb position
     * @param maxSliderPos Maximum thumb position
     * @param style Slider style
     * @param slider Reference to the slider component
     *
     * Renders slider thumbs as:
     * - Circular elements with radial gradients
     * - Outer glow effects for mystical appearance
     * - Multi-layer design with outer ring and inner highlight
     * - Enhanced glow when interacted with
     */
    void drawLinearSliderThumb(juce::Graphics& g, int x, int y, int width, int height,
                              float sliderPos, float minSliderPos, float maxSliderPos,
                              const juce::Slider::SliderStyle style, juce::Slider& slider) override;

    //==============================================================================
    // ComboBox Customization

    /**
     * @brief Custom combo box rendering
     * @param g Graphics context for drawing
     * @param width Width of combo box
     * @param height Height of combo box
     * @param isButtonDown Whether the dropdown button is pressed
     * @param buttonX X position of dropdown button
     * @param buttonY Y position of dropdown button
     * @param buttonW Width of dropdown button
     * @param buttonH Height of dropdown button
     * @param box Reference to the combo box component
     *
     * Renders combo boxes with:
     * - Gradient backgrounds matching the mystical theme
     * - Glow effects when focused or pressed
     * - Custom dropdown arrow with mystical styling
     * - Rounded corners and atmospheric borders
     */
    void drawComboBox(juce::Graphics& g, int width, int height, bool isButtonDown,
                     int buttonX, int buttonY, int buttonW, int buttonH,
                     juce::ComboBox& box) override;

    //==============================================================================
    // Button Customization

    /**
     * @brief Custom button background rendering
     * @param g Graphics context for drawing
     * @param button Reference to the button component
     * @param backgroundColour Base background color (may be overridden)
     * @param shouldDrawButtonAsHighlighted Whether button should appear highlighted
     * @param shouldDrawButtonAsDown Whether button should appear pressed
     *
     * Renders button backgrounds with:
     * - Mystical purple base color with state-dependent variations
     * - Vertical gradients for dimensional appearance
     * - Glow effects during hover and press states
     * - Rounded corners with mystical blue borders
     */
    void drawButtonBackground(juce::Graphics& g, juce::Button& button,
                            const juce::Colour& backgroundColour,
                            bool shouldDrawButtonAsHighlighted,
                            bool shouldDrawButtonAsDown) override;

    /**
     * @brief Custom button text rendering
     * @param g Graphics context for drawing
     * @param button Reference to the text button
     * @param shouldDrawButtonAsHighlighted Whether button appears highlighted
     * @param shouldDrawButtonAsDown Whether button appears pressed
     *
     * Renders button text with:
     * - Standard mystical text color
     * - Subtle glow effects when highlighted
     * - Multi-layer text rendering for atmospheric depth
     * - Proper contrast for readability
     */
    void drawButtonText(juce::Graphics& g, juce::TextButton& button,
                       bool shouldDrawButtonAsHighlighted,
                       bool shouldDrawButtonAsDown) override;

    //==============================================================================
    // Label Customization

    /**
     * @brief Custom label text rendering
     * @param g Graphics context for drawing
     * @param label Reference to the label component
     *
     * Renders labels with:
     * - Mystical light blue/white text color
     * - Subtle glow effects for atmospheric appearance
     * - Proper alpha handling for disabled states
     * - Offset shadow effects for depth perception
     */
    void drawLabel(juce::Graphics& g, juce::Label& label) override;

    //==============================================================================
    // Window Customization

    /**
     * @brief Custom resizable window frame rendering
     * @param g Graphics context for drawing
     * @param w Width of the frame
     * @param h Height of the frame
     * @param border Border size specification
     *
     * Renders window backgrounds with:
     * - Complex multi-point gradient backgrounds
     * - Multiple color stops for atmospheric depth
     * - Subtle border glows
     * - Dark mystical base with purple and blue accents
     */
    void drawResizableFrame(juce::Graphics& g, int w, int h, const juce::BorderSize<int>& border) override;

private:
    //==============================================================================
    // Color Definitions

    static const juce::Colour darkBlue;        ///< Primary dark blue background color (0xff0a0f1c)
    static const juce::Colour mediumBlue;      ///< Medium blue for component backgrounds (0xff1a2332)
    static const juce::Colour lightBlue;       ///< Light blue for accents (0xff2d3e54)
    static const juce::Colour mysticalPurple;  ///< Mystical purple for highlights (0xff4a3472)
    static const juce::Colour glowColor;       ///< Bright glow color for effects (0xff64b5f6)
    static const juce::Colour textColor;       ///< Light text color for readability (0xffc5d1de)

    //==============================================================================
    // Helper Methods for Visual Effects

    /**
     * @brief Creates a vertical gradient between two colors
     * @param area Rectangle area for the gradient
     * @param topColor Color at the top of the gradient
     * @param bottomColor Color at the bottom of the gradient
     * @return juce::ColourGradient Configured vertical gradient
     *
     * Utility method for creating consistent vertical gradients
     * used throughout the mystical interface styling.
     */
    juce::ColourGradient createVerticalGradient(const juce::Rectangle<float>& area,
                                               const juce::Colour& topColor,
                                               const juce::Colour& bottomColor) const;

    /**
     * @brief Creates a radial gradient from center to edges
     * @param area Rectangle area for the gradient
     * @param centerColor Color at the center of the gradient
     * @param edgeColor Color at the edges of the gradient
     * @return juce::ColourGradient Configured radial gradient
     *
     * Utility method for creating radial gradients, particularly
     * useful for button highlights and circular elements.
     */
    juce::ColourGradient createRadialGradient(const juce::Rectangle<float>& area,
                                             const juce::Colour& centerColor,
                                             const juce::Colour& edgeColor) const;

    /**
     * @brief Draws a mystical glow effect around a rectangular area
     * @param g Graphics context for drawing
     * @param area Rectangle to draw the glow around
     * @param glowColor Color of the glow effect
     * @param glowRadius Radius/intensity of the glow (default: 8.0f)
     *
     * Creates atmospheric glow effects by drawing multiple
     * concentric rounded rectangles with decreasing opacity.
     * Used for buttons, sliders, and other interactive elements.
     */
    void drawGlowEffect(juce::Graphics& g, const juce::Rectangle<float>& area,
                       const juce::Colour& glowColor, float glowRadius = 8.0f) const;
};