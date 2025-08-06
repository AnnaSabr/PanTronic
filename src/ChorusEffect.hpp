/**
 * @file ChorusEffect.hpp
 * @brief Chorus audio effect implementation
 */

#pragma once

#include "JuceHeader.h"

/**
 * @class DelayLine
 * @brief Simple delay line for audio delay effects
 *
 * Implements a circular buffer delay line with interpolation
 * for variable delay times.
 */
class DelayLine {
public:
    /**
     * @brief Sets the buffer size of the delay line
     * @param size Buffer size in samples
     */
    void setSize(int size) {
        buffer.resize(size, 0.0f);
        writeIndex = 0;
    }

    /**
     * @brief Writes a sample to the delay line
     * @param sample Audio sample to write
     */
    void write(float sample) {
        buffer[writeIndex] = sample;
        writeIndex = (writeIndex + 1) % buffer.size();
    }

    /**
     * @brief Reads a sample from the delay line with variable delay
     *
     * Uses linear interpolation for non-integer delay values.
     *
     * @param delayInSamples Delay in samples (can have decimal places)
     * @return float Interpolated sample from the delay line
     */
    float read(float delayInSamples) const {
        float readIndex = writeIndex - delayInSamples;
        while (readIndex < 0) readIndex += buffer.size();
        while (readIndex >= buffer.size()) readIndex -= buffer.size();

        int index1 = static_cast<int>(readIndex);
        int index2 = (index1 + 1) % buffer.size();
        float fraction = readIndex - index1;

        return buffer[index1] * (1.0f - fraction) + buffer[index2] * fraction;
    }

private:
    std::vector<float> buffer;  ///< Circular audio buffer
    int writeIndex = 0;         ///< Current write index
};

/**
 * @class ChorusEffect
 * @brief Implementation of a chorus audio effect
 *
 * A chorus effect creates a spatial, shimmering sound by
 * modulating delay times with an LFO (Low Frequency Oscillator).
 * The effect mixes the original signal with modulated, delayed
 * versions to create the characteristic chorus sound.
 */
class ChorusEffect {
public:
    /**
     * @brief Constructor
     */
    ChorusEffect();

    /**
     * @brief Destructor
     */
    ~ChorusEffect();

    /**
     * @brief Prepares the effect for audio processing
     *
     * Initializes delay lines based on sample rate and calculates LFO parameters.
     *
     * @param spec ProcessSpec with sample rate, block size and channel count
     */
    void prepare(const juce::dsp::ProcessSpec& spec);

    /**
     * @brief Processes an audio block with the chorus effect
     *
     * Applies chorus effect to all channels of an audio buffer.
     * For each sample:
     * 1. Calculate LFO value
     * 2. Determine modulated delay time
     * 3. Process delayed signal with feedback
     * 4. Mix dry and wet signals
     *
     * @param buffer AudioBuffer to be processed
     */
    void processBlock(juce::AudioBuffer<float>& buffer);

    /**
     * @brief Sets the LFO rate
     *
     * @param newRate LFO frequency in Hz (0.1 to 10.0)
     */
    void setRate(float newRate);

    /**
     * @brief Sets the modulation depth
     *
     * @param newDepth Depth of delay modulation (0.0 to 1.0)
     */
    void setDepth(float newDepth);

    /**
     * @brief Sets the feedback amount
     *
     * @param newFeedback Feedback of the delayed signal (0.0 to 0.95)
     */
    void setFeedback(float newFeedback);

    /**
     * @brief Sets the dry/wet mix ratio
     *
     * @param newMix Amount of effect signal (0.0 = dry only, 1.0 = wet only)
     */
    void setMix(float newMix);

private:
    /**
     * @brief Updates LFO parameters
     *
     * Calculates phase increment based on rate and sample rate.
     */
    void updateLFO();

    // Audio parameters
    float sampleRate = 44100.0f;           ///< Current sample rate in Hz

    // Chorus parameters
    float rate = 0.5f;                     ///< LFO rate in Hz
    float depth = 0.5f;                    ///< Modulation depth (0.0-1.0)
    float feedback = 0.3f;                 ///< Feedback level (0.0-0.95)
    float mix = 0.5f;                      ///< Dry/wet mix (0.0-1.0)

    // Delay parameters
    static constexpr float maxDelayTime = 0.05f;  ///< Maximum delay time in seconds (50ms)
    static constexpr float baseDelayTime = 0.01f; ///< Base delay time in seconds (10ms)

    // LFO state
    float lfoPhase = 0.0f;                 ///< Current LFO phase (0.0-1.0)
    float lfoPhaseIncrement = 0.0f;        ///< Phase increment per sample

    // Delay lines
    DelayLine leftDelayLine;               ///< Delay line for left channel
    DelayLine rightDelayLine;              ///< Delay line for right channel
};