#include "MysticalLookAndFeel.hpp"

// Color definitions
const juce::Colour MysticalLookAndFeel::darkBlue = juce::Colour(0xff0a0f1c);      // Sehr dunkles Blau
const juce::Colour MysticalLookAndFeel::mediumBlue = juce::Colour(0xff1a2332);    // Mittleres Blau
const juce::Colour MysticalLookAndFeel::lightBlue = juce::Colour(0xff2d3e54);     // Helleres Blau
const juce::Colour MysticalLookAndFeel::mysticalPurple = juce::Colour(0xff4a3472); // Mystisches Lila
const juce::Colour MysticalLookAndFeel::glowColor = juce::Colour(0xff64b5f6);     // Heller Glow
const juce::Colour MysticalLookAndFeel::textColor = juce::Colour(0xffc5d1de);     // Heller Text

MysticalLookAndFeel::MysticalLookAndFeel()
{
    // Grundfarben setzen
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

void MysticalLookAndFeel::drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
                                          float sliderPos, float minSliderPos, float maxSliderPos,
                                          const juce::Slider::SliderStyle style, juce::Slider& slider)
{
    auto trackWidth = juce::jmin(6.0f, (float) height * 0.25f);
    auto trackArea = juce::Rectangle<float>(x, y + height * 0.5f - trackWidth * 0.5f, width, trackWidth);

    // Hintergrund-Track mit Verlauf
    auto backgroundGradient = createVerticalGradient(trackArea, darkBlue.brighter(0.1f), darkBlue.darker(0.2f));
    g.setGradientFill(backgroundGradient);
    g.fillRoundedRectangle(trackArea, trackWidth * 0.5f);

    // Aktiver Track-Bereich mit mystischem Verlauf
    auto activeTrackWidth = sliderPos - x;
    if (activeTrackWidth > 0)
    {
        auto activeTrackArea = trackArea.withWidth(activeTrackWidth);
        auto activeGradient = createVerticalGradient(activeTrackArea, glowColor.withAlpha(0.8f), mysticalPurple);
        g.setGradientFill(activeGradient);
        g.fillRoundedRectangle(activeTrackArea, trackWidth * 0.5f);

        // Glow-Effekt für aktiven Track
        drawGlowEffect(g, activeTrackArea, glowColor.withAlpha(0.3f), 4.0f);
    }

    // Umrandung
    g.setColour(glowColor.withAlpha(0.5f));
    g.drawRoundedRectangle(trackArea, trackWidth * 0.5f, 1.0f);
}

void MysticalLookAndFeel::drawLinearSliderThumb(juce::Graphics& g, int x, int y, int width, int height,
                                               float sliderPos, float minSliderPos, float maxSliderPos,
                                               const juce::Slider::SliderStyle style, juce::Slider& slider)
{
    auto thumbSize = juce::jmin(height, 20);
    auto thumbArea = juce::Rectangle<float>(sliderPos - thumbSize * 0.5f, y + height * 0.5f - thumbSize * 0.5f,
                                           thumbSize, thumbSize);

    // Glow-Effekt um den Thumb
    drawGlowEffect(g, thumbArea.expanded(4.0f), glowColor.withAlpha(0.4f), 8.0f);

    // Thumb mit radialem Verlauf
    auto thumbGradient = createRadialGradient(thumbArea, glowColor, mysticalPurple.darker());
    g.setGradientFill(thumbGradient);
    g.fillEllipse(thumbArea);

    // Äußere Umrandung
    g.setColour(glowColor);
    g.drawEllipse(thumbArea, 2.0f);

    // Innerer Ring
    g.setColour(juce::Colours::white.withAlpha(0.6f));
    g.drawEllipse(thumbArea.reduced(3.0f), 1.0f);
}

void MysticalLookAndFeel::drawComboBox(juce::Graphics& g, int width, int height, bool isButtonDown,
                                      int buttonX, int buttonY, int buttonW, int buttonH,
                                      juce::ComboBox& box)
{
    auto cornerSize = 4.0f;
    auto area = juce::Rectangle<float>(0, 0, width, height);

    // Hintergrund mit Verlauf
    auto backgroundGradient = createVerticalGradient(area, mediumBlue.brighter(0.1f), mediumBlue.darker(0.2f));
    g.setGradientFill(backgroundGradient);
    g.fillRoundedRectangle(area, cornerSize);

    // Glow-Effekt wenn hervorgehoben
    if (box.hasKeyboardFocus(true) || isButtonDown)
    {
        drawGlowEffect(g, area, glowColor.withAlpha(0.5f), 6.0f);
    }

    // Umrandung
    g.setColour(glowColor.withAlpha(0.7f));
    g.drawRoundedRectangle(area.reduced(1.0f), cornerSize, 1.5f);

    // Dropdown-Pfeil
    auto arrowArea = juce::Rectangle<float>(width - 20, 0, 20, height);
    auto arrowPath = juce::Path();
    arrowPath.addTriangle(arrowArea.getCentreX() - 4, arrowArea.getCentreY() - 2,
                         arrowArea.getCentreX() + 4, arrowArea.getCentreY() - 2,
                         arrowArea.getCentreX(), arrowArea.getCentreY() + 3);

    g.setColour(textColor);
    g.fillPath(arrowPath);
}

void MysticalLookAndFeel::drawButtonBackground(juce::Graphics& g, juce::Button& button,
                                              const juce::Colour& backgroundColour,
                                              bool shouldDrawButtonAsHighlighted,
                                              bool shouldDrawButtonAsDown)
{
    auto area = button.getLocalBounds().toFloat();
    auto cornerSize = 6.0f;

    // Basis-Hintergrundfarbe bestimmen
    auto buttonColor = mysticalPurple;
    if (shouldDrawButtonAsDown)
        buttonColor = buttonColor.darker(0.3f);
    else if (shouldDrawButtonAsHighlighted)
        buttonColor = buttonColor.brighter(0.2f);

    // Hintergrund mit Verlauf
    auto buttonGradient = createVerticalGradient(area, buttonColor.brighter(0.1f), buttonColor.darker(0.2f));
    g.setGradientFill(buttonGradient);
    g.fillRoundedRectangle(area, cornerSize);

    // Glow-Effekt bei Highlight oder Press
    if (shouldDrawButtonAsHighlighted || shouldDrawButtonAsDown)
    {
        drawGlowEffect(g, area, glowColor.withAlpha(0.4f), 8.0f);
    }

    // Umrandung
    g.setColour(glowColor.withAlpha(shouldDrawButtonAsHighlighted ? 1.0f : 0.6f));
    g.drawRoundedRectangle(area.reduced(1.0f), cornerSize, 1.5f);
}

void MysticalLookAndFeel::drawButtonText(juce::Graphics& g, juce::TextButton& button,
                                        bool shouldDrawButtonAsHighlighted,
                                        bool shouldDrawButtonAsDown)
{
    auto font = getTextButtonFont(button, button.getHeight());
    g.setFont(font);

    auto textColor = button.findColour(shouldDrawButtonAsDown ? juce::TextButton::textColourOnId
                                                             : juce::TextButton::textColourOffId);

    // Text-Glow-Effekt
    if (shouldDrawButtonAsHighlighted)
    {
        g.setColour(glowColor.withAlpha(0.3f));
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

    // Haupttext
    g.setColour(textColor);
    g.drawText(button.getButtonText(), button.getLocalBounds(),
              juce::Justification::centred, true);
}

void MysticalLookAndFeel::drawLabel(juce::Graphics& g, juce::Label& label)
{
    g.fillAll(label.findColour(juce::Label::backgroundColourId));

    if (!label.isBeingEdited())
    {
        auto alpha = label.isEnabled() ? 1.0f : 0.5f;
        const auto textColour = label.findColour(juce::Label::textColourId).withMultipliedAlpha(alpha);

        // Leichter Glow-Effekt für Labels
        g.setColour(glowColor.withAlpha(0.2f));
        auto textArea = getLabelBorderSize(label).subtractedFrom(label.getLocalBounds());
        g.drawText(label.getText(), textArea.translated(1, 1),
                  label.getJustificationType(), true);

        // Haupttext
        g.setColour(textColour);
        g.drawText(label.getText(), textArea,
                  label.getJustificationType(), true);
    }
}

void MysticalLookAndFeel::drawResizableFrame(juce::Graphics& g, int w, int h, const juce::BorderSize<int>& border)
{
    // Mystischer Hintergrund mit mehreren Verläufen
    auto area = juce::Rectangle<float>(0, 0, w, h);

    // Hauptverlauf
    auto mainGradient = juce::ColourGradient(darkBlue.darker(0.3f), 0, 0,
                                            mediumBlue.darker(0.1f), w, h, false);
    mainGradient.addColour(0.3, mysticalPurple.withAlpha(0.1f));
    mainGradient.addColour(0.7, darkBlue.brighter(0.05f));

    g.setGradientFill(mainGradient);
    g.fillAll();

    // Subtiler Rand-Glow
    g.setColour(glowColor.withAlpha(0.1f));
    g.drawRect(0, 0, w, h, 2);
}

// Note: MidiKeyboardComponent background wird direkt über Komponentenfarben gesetzt
// Diese Methode existiert nicht in JUCE LookAndFeel

// Helper Methods

juce::ColourGradient MysticalLookAndFeel::createVerticalGradient(const juce::Rectangle<float>& area,
                                                                const juce::Colour& topColor,
                                                                const juce::Colour& bottomColor) const
{
    return juce::ColourGradient(topColor, area.getTopLeft(),
                               bottomColor, area.getBottomLeft(), false);
}

juce::ColourGradient MysticalLookAndFeel::createRadialGradient(const juce::Rectangle<float>& area,
                                                              const juce::Colour& centerColor,
                                                              const juce::Colour& edgeColor) const
{
    return juce::ColourGradient(centerColor, area.getCentre(),
                               edgeColor, area.getTopLeft(), true);
}

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