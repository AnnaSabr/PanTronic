#pragma once

#include <JuceHeader.h>

class ReverbComponent : public juce::Component
{
public:
    ReverbComponent();
    ~ReverbComponent() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

    // Setter methods for reverb parameters
    void setRoomSize(float roomSize);
    void setDamping(float damping);
    void setWetLevel(float wetLevel);
    void setDryLevel(float dryLevel);
    void setWidth(float width);

    // Callback for parameter changes
    std::function<void(float roomSize, float damping, float wetLevel, float dryLevel, float width)> onParameterChanged;

private:
    void parameterChanged();

    juce::Slider roomSizeSlider;
    juce::Slider dampingSlider;
    juce::Slider wetLevelSlider;
    juce::Slider dryLevelSlider;
    juce::Slider widthSlider;

    juce::Label roomSizeLabel;
    juce::Label dampingLabel;
    juce::Label wetLevelLabel;
    juce::Label dryLevelLabel;
    juce::Label widthLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ReverbComponent)
};