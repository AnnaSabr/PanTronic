#pragma once

#include "JuceHeader.h"

class ADSRComponent : public juce::Component
{
public:
    ADSRComponent();
    ~ADSRComponent() override;

    void paint(juce::Graphics& g) override;
    void resized() override;
    
    void mouseDown(const juce::MouseEvent& event) override;
    void mouseDrag(const juce::MouseEvent& event) override;
    void mouseUp(const juce::MouseEvent& event) override;
    void mouseMove(const juce::MouseEvent& event) override;

    // ADSR-Parameter setzen/abrufen (0.0 bis 1.0)
    void setAttack(float attack);
    void setDecay(float decay);
    void setSustain(float sustain);
    void setRelease(float release);
    
    float getAttack() const { return attackValue; }
    float getDecay() const { return decayValue; }
    float getSustain() const { return sustainValue; }
    float getRelease() const { return releaseValue; }
    
    // Callback für Parameteränderungen
    std::function<void(float attack, float decay, float sustain, float release)> onParameterChanged;
    
    // Farbschema updaten
    void updateColors(juce::Colour primary, juce::Colour secondary);

private:
    // ADSR-Werte (0.0 bis 1.0)
    float attackValue = 0.1f;
    float decayValue = 0.3f;
    float sustainValue = 0.7f;
    float releaseValue = 0.5f;
    

    enum class DragMode {
        None,
        Attack,
        Decay, 
        Sustain,
        Release
    };
    
    DragMode currentDragMode = DragMode::None;
    juce::Point<float> lastMousePos;
    

    juce::Colour primaryColor = juce::Colours::darkblue;
    juce::Colour secondaryColor = juce::Colours::darkblue;
    

    juce::Path createADSRPath() const;
    DragMode getHitTest(juce::Point<float> position) const;
    juce::Point<float> getAttackPoint() const;
    juce::Point<float> getDecayPoint() const;
    juce::Point<float> getSustainPoint() const;
    juce::Point<float> getReleasePoint() const;
    
    float mapToADSRValue(float screenValue, bool isTime) const;
    float mapFromADSRValue(float adsrValue, bool isTime) const;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ADSRComponent)
};