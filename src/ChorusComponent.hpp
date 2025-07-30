#pragma once



#include "JuceHeader.h"



class ChorusComponent : public juce::Component {

public:

    ChorusComponent();

    ~ChorusComponent() override;



    void paint(juce::Graphics& g) override;

    void resized() override;



    // Setter methods for parameters

    void setRate(float rate);

    void setDepth(float depth);

    void setFeedback(float feedback);

    void setMix(float mix);



    // Callback for parameter changes

    std::function<void(float rate, float depth, float feedback, float mix)> onParameterChanged;



private:

    void updateParameters();



    // Sliders

    juce::Slider rateSlider;

    juce::Slider depthSlider;

    juce::Slider feedbackSlider;

    juce::Slider mixSlider;



    // Labels

    juce::Label rateLabel;

    juce::Label depthLabel;

    juce::Label feedbackLabel;

    juce::Label mixLabel;



    // Current values

    float currentRate = 0.5f;

    float currentDepth = 0.5f;

    float currentFeedback = 0.3f;

    float currentMix = 0.5f;



    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ChorusComponent)

};