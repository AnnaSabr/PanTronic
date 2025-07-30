#pragma once

#include "JuceHeader.h"
#include "juce_dsp/juce_dsp.h"
#include "ChorusEffect.hpp"

//==============================================================================
class AvSynthAudioProcessor final : public juce::AudioProcessor {
    friend class AvSynthAudioProcessorEditor;

  public:
    enum class Parameters {
        Gain, Frequency, OscType, LowPassFreq, HighPassFreq,
        Attack, Decay, Sustain, Release,
        ReverbRoomSize, ReverbDamping, ReverbWetLevel, ReverbDryLevel, ReverbWidth,
        ChorusRate, ChorusDepth, ChorusFeedback, ChorusMix,
        NumParameters
    };
    enum class OscType { Sine, Square, Saw, Triangle, Flute, NumTypes};

    struct ChainSettings {
        float gain = 0.25f;
        float frequency = 440.0f;
        OscType oscType = OscType::Sine;
        float LowPassFreq = 20000.0f;
        float HighPassFreq = 20.0f;
        float attack = 0.1f;
        float decay = 0.1f;
        float sustain = 0.7f;
        float release = 0.3f;
        //Reverb parameters
        float reverbRoomSize = 0.5f;
        float reverbDamping = 0.5f;
        float reverbWetLevel = 0.33f;
        float reverbDryLevel = 0.4f;
        float reverbWidth = 1.0f;

        // Chorus parameters
        float chorusRate = 0.5f;
        float chorusDepth = 0.5f;
        float chorusFeedback = 0.3f;
        float chorusMix = 0.5f;

        static forcedinline ChainSettings Get(const juce::AudioProcessorValueTreeState &parameters);
    };

  public:
    //==============================================================================
    AvSynthAudioProcessor();
    ~AvSynthAudioProcessor() override;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported(const BusesLayout &layouts) const override;

    void processBlock(juce::AudioBuffer<float> &, juce::MidiBuffer &) override;
    using AudioProcessor::processBlock;

    //==============================================================================
    juce::AudioProcessorEditor *createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String &newName) override;

    //==============================================================================
    void getStateInformation(juce::MemoryBlock &destData) override;
    void setStateInformation(const void *data, int sizeInBytes) override;

    void updateAngleDelta(float frequency);

    static float getOscSample(OscType type, double angle);
    static float getFluteWaveform(double angle);

    void updateHighPassCoefficients(float frequency);
    void updateLowPassCoefficients(float frequency);
    void updateReverbParameters(const ChainSettings& settings);
    void updateChorusParameters(const ChainSettings& settings);  // Add this declaration

  private:
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

  public:
    juce::AudioProcessorValueTreeState parameters{*this, nullptr, "Parameters", createParameterLayout()};
    juce::MidiKeyboardState keyboardState;

  private:
    juce::Random random;
    ChainSettings previousChainSettings;
    juce::AudioSampleBuffer circularBuffer;
    int bufferWritePos = 0;

  private:
    using Filter = juce::dsp::IIR::Filter<float>;

    using CutFilter = juce::dsp::ProcessorChain<Filter, Filter>;
    using MonoChain = juce::dsp::ProcessorChain<CutFilter, CutFilter>;

    MonoChain leftChain, rightChain;

    double currentAngle = 0.0, angleDelta = 0.0;

    // ADSR Envelope
    juce::ADSR adsr;
    juce::ADSR::Parameters adsrParams;
    bool noteIsOn = false;

    // Reverb
    juce::dsp::Reverb reverb;
    juce::dsp::Reverb::Parameters reverbParams;

    // Chorus Effect - Add this member variable
    ChorusEffect chorus;

  private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AvSynthAudioProcessor)
};