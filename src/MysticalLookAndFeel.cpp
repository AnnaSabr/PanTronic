/**
 * @file MysticalLookAndFeel.cpp
 * @brief Implementation of the mystical-themed custom LookAndFeel for the AvSynth plugin
 * @author AvSynth Development Team
 * @date 2024
 *
 * This file implements the MysticalLookAndFeel class, which provides a custom visual theme
 * for the AvSynth audio plugin. The theme features a dark, mystical aesthetic with
 * gradient backgrounds, glow effects, and carefully chosen color schemes that create
 * an atmospheric and professional appearance.
 */

#include "MysticalLookAndFeel.hpp"

/**
 * @class MysticalLookAndFeel
 * @brief Custom JUCE LookAndFeel implementation with mystical/magical visual theme
 *
 * This class extends JUCE's LookAndFeel_V4 to provide a cohesive visual theme
 * for the AvSynth synthesizer plugin. The design emphasizes dark colors with
 * mystical purple and blue accents, gradient backgrounds, and subtle glow effects
 * to create an immersive and professional audio production environment.
 *
 * Key Design Elements:
 * - Dark base colors with mystical purple/blue accents
 * - Gradient fills for depth and visual interest
 * - Subtle glow effects for interactive elements
 * - Consistent color palette across all components
 * - Responsive visual feedback for user interactions
 *
 * The theme is optimized for extended use in audio production environments,
 * reducing eye strain while maintaining visual appeal and clear component hierarchy.
 */

// Static color definitions - Core color palette for the mystical theme

/**
 * @brief Very dark blue - Primary background color
 * @details Used as the base color for backgrounds and inactive areas.
 *          Provides excellent contrast for text and controls while maintaining
 *          the dark, mystical aesthetic.
 */
const juce::Colour MysticalLookAndFeel::darkBlue = juce::Colour(0xff0a0f1c);

/**
 * @brief Medium blue - Secondary background and component color
 * @details Used for component backgrounds, inactive states, and subtle accents.
 *          Provides visual hierarchy and depth to the interface.
 */
const juce::Colour MysticalLookAndFeel::mediumBlue = juce::Colour(0xff1a2332);

/**
 * @brief Light blue - Tertiary background color
 * @details Used for elevated components and subtle highlighting.
 *          Creates visual separation between interface layers.
 */
const juce::Colour MysticalLookAndFeel::lightBlue = juce::Colour(0xff2d3e54);

/**
 * @brief Mystical purple - Accent color for interactive elements
 * @details Primary accent color used for buttons, active states, and emphasis.
 *          Provides the "mystical" character to the visual theme.
 */
const juce::Colour MysticalLookAndFeel::mysticalPurple = juce::Colour(0xff4a3472);

/**
 * @brief Bright glow color - Highlights and active states
 * @details Used for glow effects, active elements, and user interaction feedback.
 *          Creates visual interest and guides user attention.
 */
const juce::Colour MysticalLookAndFeel::glowColor = juce::Colour(0xff64b5f6);

/**
 * @brief Light text color - Primary text and labels
 * @details Optimized text color providing excellent readability against
 *          dark backgrounds while maintaining the overall theme aesthetic.
 */
const juce::Colour MysticalLookAndFeel::textColor = juce::Colour(0xffc5d1de);

/**
 * @brief Constructor - Initializes the mystical theme color scheme
 *
 * Sets up the complete color palette for all JUCE components used in the plugin.
 * Establishes consistent theming across sliders, buttons, combo boxes, labels,
 * and other interface elements. The color assignments create a cohesive visual
 * experience that enhances usability while maintaining the mystical aesthetic.
 *
 * Color assignments include:
 * - Background colors for windows and components
 * - Text colors for optimal readability
 * - Accent colors for interactive elements
 * - Outline and highlight colors for visual feedback
 */
MysticalLookAndFeel::MysticalLookAndFeel()
{
    // Set foundational colors for all components
    setColour(juce::ResizableWindow::backgroundColourId, darkBlue);
    setColour(juce::Label::textColourId, textColor);
    setColour(juce::Slider::thumbColourId, glowColor);
    setColour(juce::Slider::trackColourId, mediumBlue);
    setColour(juce::Slider::backgroundColourId, darkBlue);
    setColour(juce::ComboBox::backgroundColourId, mediumBlue);
    setColour(juce::ComboBox::textColourId, textColor);
    setColour(juce::ComboBox::outlineColourId, glowColor);
    setColour(juce::TextButton::buttonColourId, mysticalPurple);
    setColour(juce::TextButton::textColourOffId, textColor);
    setColour(juce::TextButton::textColourOnId, juce::Colours::white);
}

/**
 * @brief Custom slider track rendering with mystical gradients and glow effects
 * @param g Graphics context for drawing operations
 * @param x X position of the slider
 * @param y Y position of the slider
 * @param width Total width of the slider
 * @param height Total height of the slider
 * @param sliderPos Current position of the slider thumb
 * @param minSliderPos Minimum possible slider position
 * @param maxSliderPos Maximum possible slider position
 * @param style Slider style (not used in this implementation)
 * @param slider Reference to the slider component
 *
 * Renders horizontal linear sliders with a sophisticated visual design:
 * - Background track with subtle gradient for depth
 * - Active portion with mystical gradient (glow to purple)
 * - Subtle glow effect around active area
 * - Rounded corners for modern appearance
 * - Semi-transparent outline for definition
 *
 * The design provides clear visual feedback about the current value
 * while maintaining the mystical aesthetic throughout the interaction.
 */
void MysticalLookAndFeel::drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
                                          float sliderPos, float minSliderPos, float maxSliderPos,
                                          const juce::Slider::SliderStyle style, juce::Slider& slider)
{
    auto trackWidth = juce::jmin(6.0f, (float) height * 0.25f);
    auto trackArea = juce::Rectangle<float>(x, y + height * 0.5f - trackWidth * 0.5f, width, trackWidth);

    // Background track with subtle gradient for depth
    auto backgroundGradient = createVerticalGradient(trackArea, darkBlue.brighter(0.1f), darkBlue.darker(0.2f));
    g.setGradientFill(backgroundGradient);
    g.fillRoundedRectangle(trackArea, trackWidth * 0.5f);

    // Active track section with mystical gradient
    auto activeTrackWidth = sliderPos - x;
    if (activeTrackWidth > 0)
    {
        auto activeTrackArea = trackArea.withWidth(activeTrackWidth);
        auto activeGradient = createVerticalGradient(activeTrackArea, glowColor.withAlpha(0.8f), mysticalPurple);
        g.setGradientFill(activeGradient);
        g.fillRoundedRectangle(activeTrackArea, trackWidth * 0.5f);

        // Add glow effect for visual impact
        drawGlowEffect(g, activeTrackArea, glowColor.withAlpha(0.3f), 4.0f);
    }

    // Subtle outline for definition
    g.setColour(glowColor.withAlpha(0.5f));
    g.drawRoundedRectangle(trackArea, trackWidth * 0.5f, 1.0f);
}

/**
 * @brief Custom slider thumb rendering with radial gradients and glow effects
 * @param g Graphics context for drawing operations
 * @param x X position of the slider
 * @param y Y position of the slider
 * @param width Total width of the slider
 * @param height Total height of the slider
 * @param sliderPos Current position of the slider thumb
 * @param minSliderPos Minimum possible slider position
 * @param maxSliderPos Maximum possible slider position
 * @param style Slider style (not used in this implementation)
 * @param slider Reference to the slider component
 *
 * Creates an attractive circular thumb with multiple visual layers:
 * - Outer glow effect for prominence and mystical feel
 * - Radial gradient fill from bright center to darker edge
 * - Bright outer ring for definition and contrast
 * - Inner highlight ring for dimensional appearance
 *
 * The thumb design ensures excellent visibility and provides satisfying
 * visual feedback during user interaction while maintaining theme consistency.
 */
void MysticalLookAndFeel::drawLinearSliderThumb(juce::Graphics& g, int x, int y, int width, int height,
                                               float sliderPos, float minSliderPos, float maxSliderPos,
                                               const juce::Slider::SliderStyle style, juce::Slider& slider)
{
    auto thumbSize = juce::jmin(height, 20);
    auto thumbArea = juce::Rectangle<float>(sliderPos - thumbSize * 0.5f, y + height * 0.5f - thumbSize * 0.5f,
                                           thumbSize, thumbSize);

    // Create glow effect around the thumb
    drawGlowEffect(g, thumbArea.expanded(4.0f), glowColor.withAlpha(0.4f), 8.0f);

    // Thumb with radial gradient for dimensional appearance
    auto thumbGradient = createRadialGradient(thumbArea, glowColor, mysticalPurple.darker());
    g.setGradientFill(thumbGradient);
    g.fillEllipse(thumbArea);

    // Outer definition ring
    g.setColour(glowColor);
    g.drawEllipse(thumbArea, 2.0f);

    // Inner highlight ring for depth
    g.setColour(juce::Colours::white.withAlpha(0.6f));
    g.drawEllipse(thumbArea.reduced(3.0f), 1.0f);
}

/**
 * @brief Custom combo box rendering with mystical styling and interaction feedback
 * @param g Graphics context for drawing operations
 * @param width Total width of the combo box
 * @param height Total height of the combo box
 * @param isButtonDown Whether the combo box button is currently pressed
 * @param buttonX X position of the dropdown button area
 * @param buttonY Y position of the dropdown button area
 * @param buttonW Width of the dropdown button area
 * @param buttonH Height of the dropdown button area
 * @param box Reference to the combo box component
 *
 * Renders combo boxes with sophisticated visual styling:
 * - Gradient background from lighter to darker blue for depth
 * - Glow effect when focused or pressed for interaction feedback
 * - Rounded corners with subtle outline for modern appearance
 * - Custom dropdown arrow with theme-consistent coloring
 * - Responsive visual states for different interaction modes
 *
 * The design ensures clear visual hierarchy and provides intuitive
 * feedback for user interactions while maintaining theme consistency.
 */
void MysticalLookAndFeel::drawComboBox(juce::Graphics& g, int width, int height, bool isButtonDown,
                                      int buttonX, int buttonY, int buttonW, int buttonH,
                                      juce::ComboBox& box)
{
    auto cornerSize = 4.0f;
    auto area = juce::Rectangle<float>(0, 0, width, height);

    // Background with gradient for visual depth
    auto backgroundGradient = createVerticalGradient(area, mediumBlue.brighter(0.1f), mediumBlue.darker(0.2f));
    g.setGradientFill(backgroundGradient);
    g.fillRoundedRectangle(area, cornerSize);

    // Glow effect for focus and interaction states
    if (box.hasKeyboardFocus(true) || isButtonDown)
    {
        drawGlowEffect(g, area, glowColor.withAlpha(0.5f), 6.0f);
    }

    // Outline for definition
    g.setColour(glowColor.withAlpha(0.7f));
    g.drawRoundedRectangle(area.reduced(1.0f), cornerSize, 1.5f);

    // Custom dropdown arrow
    auto arrowArea = juce::Rectangle<float>(width - 20, 0, 20, height);
    auto arrowPath = juce::Path();
    arrowPath.addTriangle(arrowArea.getCentreX() - 4, arrowArea.getCentreY() - 2,
                         arrowArea.getCentreX() + 4, arrowArea.getCentreY() - 2,
                         arrowArea.getCentreX(), arrowArea.getCentreY() + 3);

    g.setColour(textColor);
    g.fillPath(arrowPath);
}

/**
 * @brief Custom button background rendering with mystical gradients and interaction states
 * @param g Graphics context for drawing operations
 * @param button Reference to the button component
 * @param backgroundColour Base background color (may be overridden by theme)
 * @param shouldDrawButtonAsHighlighted Whether button should show highlighted state
 * @param shouldDrawButtonAsDown Whether button should show pressed state
 *
 * Creates visually appealing buttons with multiple interaction states:
 * - Base state: Mystical purple gradient background
 * - Highlighted state: Brighter gradient with glow effect
 * - Pressed state: Darker gradient for pressed feedback
 * - Rounded corners with gradient outlines
 * - Adaptive glow effects based on interaction state
 *
 * The button design provides clear visual feedback for all interaction
 * states while maintaining consistency with the mystical theme.
 */
void MysticalLookAndFeel::drawButtonBackground(juce::Graphics& g, juce::Button& button,
                                              const juce::Colour& backgroundColour,
                                              bool shouldDrawButtonAsHighlighted,
                                              bool shouldDrawButtonAsDown)
{
    auto area = button.getLocalBounds().toFloat();
    auto cornerSize = 6.0f;

    // Determine button color based on interaction state
    auto buttonColor = mysticalPurple;
    if (shouldDrawButtonAsDown)
        buttonColor = buttonColor.darker(0.3f);
    else if (shouldDrawButtonAsHighlighted)
        buttonColor = buttonColor.brighter(0.2f);

    // Background with gradient for dimensional appearance
    auto buttonGradient = createVerticalGradient(area, buttonColor.brighter(0.1f), buttonColor.darker(0.2f));
    g.setGradientFill(buttonGradient);
    g.fillRoundedRectangle(area, cornerSize);

    // Glow effect for highlighted or pressed states
    if (shouldDrawButtonAsHighlighted || shouldDrawButtonAsDown)
    {
        drawGlowEffect(g, area, glowColor.withAlpha(0.4f), 8.0f);
    }

    // Outline with adaptive brightness
    g.setColour(glowColor.withAlpha(shouldDrawButtonAsHighlighted ? 1.0f : 0.6f));
    g.drawRoundedRectangle(area.reduced(1.0f), cornerSize, 1.5f);
}

/**
 * @brief Custom button text rendering with glow effects for highlighted states
 * @param g Graphics context for drawing operations
 * @param button Reference to the text button component
 * @param shouldDrawButtonAsHighlighted Whether button should show highlighted state
 * @param shouldDrawButtonAsDown Whether button should show pressed state
 *
 * Renders button text with sophisticated visual effects:
 * - Standard text rendering for normal states
 * - Glow effect around text when highlighted
 * - Multi-directional text shadows for glow illusion
 * - Adaptive text color based on button state
 * - Centered text alignment with proper font sizing
 *
 * The text rendering ensures excellent readability while adding
 * mystical visual flair during user interactions.
 */
void MysticalLookAndFeel::drawButtonText(juce::Graphics& g, juce::TextButton& button,
                                        bool shouldDrawButtonAsHighlighted,
                                        bool shouldDrawButtonAsDown)
{
    auto font = getTextButtonFont(button, button.getHeight());
    g.setFont(font);

    auto textColor = button.findColour(shouldDrawButtonAsDown ? juce::TextButton::textColourOnId
                                                             : juce::TextButton::textColourOffId);

    // Text glow effect for highlighted state
    if (shouldDrawButtonAsHighlighted)
    {
        g.setColour(glowColor.withAlpha(0.3f));
        // Create glow by drawing text in multiple directions
        for (int i = 1; i <= 3; ++i)
        {
            g.drawText(button.getButtonText(), button.getLocalBounds().translated(i, 0),
                      juce::Justification::centred, true);
            g.drawText(button.getButtonText(), button.getLocalBounds().translated(-i, 0),
                      juce::Justification::centred, true);
            g.drawText(button.getButtonText(), button.getLocalBounds().translated(0, i),
                      juce::Justification::centred, true);
            g.drawText(button.getButtonText(), button.getLocalBounds().translated(0, -i),
                      juce::Justification::centred, true);
        }
    }

    // Main text rendering
    g.setColour(textColor);
    g.drawText(button.getButtonText(), button.getLocalBounds(),
              juce::Justification::centred, true);
}

/**
 * @brief Custom label rendering with subtle glow effects
 * @param g Graphics context for drawing operations
 * @param label Reference to the label component
 *
 * Renders labels with enhanced visual appeal:
 * - Standard background fill from label properties
 * - Subtle glow effect behind text for mystical ambiance
 * - Proper alpha handling for disabled states
 * - Consistent text color matching theme
 * - Proper text area calculation respecting label borders
 *
 * The label rendering maintains excellent readability while
 * adding subtle mystical touches that enhance the overall theme.
 */
void MysticalLookAndFeel::drawLabel(juce::Graphics& g, juce::Label& label)
{
    g.fillAll(label.findColour(juce::Label::backgroundColourId));

    if (!label.isBeingEdited())
    {
        auto alpha = label.isEnabled() ? 1.0f : 0.5f;
        const auto textColour = label.findColour(juce::Label::textColourId).withMultipliedAlpha(alpha);

        // Subtle glow effect for mystical ambiance
        g.setColour(glowColor.withAlpha(0.2f));
        auto textArea = getLabelBorderSize(label).subtractedFrom(label.getLocalBounds());
        g.drawText(label.getText(), textArea.translated(1, 1),
                  label.getJustificationType(), true);

        // Main text rendering
        g.setColour(textColour);
        g.drawText(label.getText(), textArea,
                  label.getJustificationType(), true);
    }
}

/**
 * @brief Custom resizable frame rendering with mystical background gradients
 * @param g Graphics context for drawing operations
 * @param w Width of the resizable frame
 * @param h Height of the resizable frame
 * @param border Border size specification (not used in this implementation)
 *
 * Creates an atmospheric background for resizable windows:
 * - Multi-layer gradient background with mystical color transitions
 * - Strategic color stops for visual depth and interest
 * - Subtle border glow for frame definition
 * - Adaptive sizing based on frame dimensions
 *
 * This method is typically called for the main plugin window
 * background, establishing the foundational visual atmosphere
 * for the entire interface.
 */
void MysticalLookAndFeel::drawResizableFrame(juce::Graphics& g, int w, int h, const juce::BorderSize<int>& border)
{
    auto area = juce::Rectangle<float>(0, 0, w, h);

    // Multi-layer mystical gradient background
    auto mainGradient = juce::ColourGradient(darkBlue.darker(0.3f), 0, 0,
                                            mediumBlue.darker(0.1f), w, h, false);
    mainGradient.addColour(0.3, mysticalPurple.withAlpha(0.1f));
    mainGradient.addColour(0.7, darkBlue.brighter(0.05f));

    g.setGradientFill(mainGradient);
    g.fillAll();

    // Subtle border glow for frame definition
    g.setColour(glowColor.withAlpha(0.1f));
    g.drawRect(0, 0, w, h, 2);
}

// Helper Methods Implementation

/**
 * @brief Creates a vertical gradient between two colors
 * @param area Rectangle area for the gradient
 * @param topColor Color at the top of the gradient
 * @param bottomColor Color at the bottom of the gradient
 * @return juce::ColourGradient Configured vertical gradient
 *
 * Utility method for creating consistent vertical gradients throughout
 * the interface. Used extensively for component backgrounds to create
 * visual depth and dimensional appearance. The gradient flows from
 * top to bottom within the specified rectangular area.
 */
juce::ColourGradient MysticalLookAndFeel::createVerticalGradient(const juce::Rectangle<float>& area,
                                                                const juce::Colour& topColor,
                                                                const juce::Colour& bottomColor) const
{
    return juce::ColourGradient(topColor, area.getTopLeft(),
                               bottomColor, area.getBottomLeft(), false);
}

/**
 * @brief Creates a radial gradient from center to edges
 * @param area Rectangle area for the gradient (determines center point)
 * @param centerColor Color at the center of the gradient
 * @param edgeColor Color at the edges of the gradient
 * @return juce::ColourGradient Configured radial gradient
 *
 * Utility method for creating radial gradients, primarily used for
 * circular elements like slider thumbs and buttons. The gradient
 * radiates from the center of the specified area outward, creating
 * a dimensional, illuminated appearance.
 */
juce::ColourGradient MysticalLookAndFeel::createRadialGradient(const juce::Rectangle<float>& area,
                                                              const juce::Colour& centerColor,
                                                              const juce::Colour& edgeColor) const
{
    return juce::ColourGradient(centerColor, area.getCentre(),
                               edgeColor, area.getTopLeft(), true);
}

/**
 * @brief Draws a subtle glow effect around a rectangular area
 * @param g Graphics context for drawing operations
 * @param area Rectangle area to glow around
 * @param glowColor Color of the glow effect
 * @param glowRadius Radius of the glow effect in pixels
 *
 * Creates mystical glow effects by drawing multiple concentric rectangles
 * with decreasing alpha values. The effect starts at the specified radius
 * and fades inward, creating a soft, atmospheric glow that enhances
 * the mystical theme without overwhelming the interface.
 *
 * The glow effect is used for:
 * - Interactive element highlighting
 * - Focus indicators
 * - Active state visualization
 * - General mystical ambiance
 */
void MysticalLookAndFeel::drawGlowEffect(juce::Graphics& g, const juce::Rectangle<float>& area,
                                        const juce::Colour& glowColor, float glowRadius) const
{
    for (float i = glowRadius; i > 0; i -= 1.0f)
    {
        auto alpha = (glowRadius - i) / glowRadius * 0.1f;
        g.setColour(glowColor.withAlpha(alpha));
        g.drawRoundedRectangle(area.expanded(i), area.getHeight() * 0.1f, 1.0f);
    }
}