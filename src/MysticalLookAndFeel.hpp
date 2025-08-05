#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class MysticalLookAndFeel : public juce::LookAndFeel_V4
{
public:
    MysticalLookAndFeel();
    ~MysticalLookAndFeel() override = default;

    // Slider customization
    void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
                         float sliderPos, float minSliderPos, float maxSliderPos,
                         const juce::Slider::SliderStyle style, juce::Slider& slider) override;

    void drawLinearSliderThumb(juce::Graphics& g, int x, int y, int width, int height,
                              float sliderPos, float minSliderPos, float maxSliderPos,
                              const juce::Slider::SliderStyle style, juce::Slider& slider) override;

    // ComboBox customization
    void drawComboBox(juce::Graphics& g, int width, int height, bool isButtonDown,
                     int buttonX, int buttonY, int buttonW, int buttonH,
                     juce::ComboBox& box) override;

    // Button customization
    void drawButtonBackground(juce::Graphics& g, juce::Button& button,
                            const juce::Colour& backgroundColour,
                            bool shouldDrawButtonAsHighlighted,
                            bool shouldDrawButtonAsDown) override;

    void drawButtonText(juce::Graphics& g, juce::TextButton& button,
                       bool shouldDrawButtonAsHighlighted,
                       bool shouldDrawButtonAsDown) override;

    // Label customization
    void drawLabel(juce::Graphics& g, juce::Label& label) override;

    // Window background
    void drawResizableFrame(juce::Graphics& g, int w, int h, const juce::BorderSize<int>& border) override;

    // Die Keyboard-Farben werden direkt auf der Komponente gesetzt

private:
    // Color definitions
    static const juce::Colour darkBlue;
    static const juce::Colour mediumBlue;
    static const juce::Colour lightBlue;
    static const juce::Colour mysticalPurple;
    static const juce::Colour glowColor;
    static const juce::Colour textColor;

    // Helper methods for gradients
    juce::ColourGradient createVerticalGradient(const juce::Rectangle<float>& area,
                                               const juce::Colour& topColor,
                                               const juce::Colour& bottomColor) const;

    juce::ColourGradient createRadialGradient(const juce::Rectangle<float>& area,
                                             const juce::Colour& centerColor,
                                             const juce::Colour& edgeColor) const;

    void drawGlowEffect(juce::Graphics& g, const juce::Rectangle<float>& area,
                       const juce::Colour& glowColor, float glowRadius = 8.0f) const;
};