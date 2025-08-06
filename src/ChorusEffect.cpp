/**
 * @file ChorusEffect.cpp
 * @brief Implementation of the ChorusEffect class
 */

#include "ChorusEffect.hpp"

ChorusEffect::ChorusEffect()
{
}

ChorusEffect::~ChorusEffect()
{
}

void ChorusEffect::prepare(const juce::dsp::ProcessSpec& spec)
{
    // Store sample rate for calculations
    sampleRate = static_cast<float>(spec.sampleRate);

    // Calculate delay buffer size - needs to accommodate maximum delay time
    // Add 1 sample for safety margin
    int delayBufferSize = static_cast<int>(sampleRate * maxDelayTime) + 1;

    // Initialize delay lines for stereo processing
    leftDelayLine.setSize(delayBufferSize);
    rightDelayLine.setSize(delayBufferSize);

    // Calculate initial LFO parameters
    updateLFO();
}

void ChorusEffect::processBlock(juce::AudioBuffer<float>& buffer)
{
    auto numSamples = buffer.getNumSamples();
    auto numChannels = buffer.getNumChannels();

    // Process each sample individually for smooth modulation
    for (int sample = 0; sample < numSamples; ++sample)
    {
        // Generate LFO value using sine wave (-1.0 to +1.0)
        float lfoValue = std::sin(lfoPhase * juce::MathConstants<float>::twoPi);

        // Advance LFO phase and wrap around at 1.0
        lfoPhase += lfoPhaseIncrement;
        if (lfoPhase >= 1.0f)
            lfoPhase -= 1.0f;

        // Calculate modulated delay time
        // Base delay + modulation range scaled by LFO and depth
        float modulatedDelay = baseDelayTime + (depth * maxDelayTime * 0.5f * (lfoValue + 1.0f));
        float delayInSamples = modulatedDelay * sampleRate;

        // Process each channel
        for (int channel = 0; channel < numChannels; ++channel)
        {
            // Select appropriate delay line for this channel
            auto& delayLine = (channel == 0) ? leftDelayLine : rightDelayLine;

            // Get input sample for this channel
            float inputSample = buffer.getSample(channel, sample);

            // Read delayed sample with interpolation
            float delayedSample = delayLine.read(delayInSamples);

            // Apply feedback - delayed signal fed back into delay line
            float feedbackSample = delayedSample * feedback;
            delayLine.write(inputSample + feedbackSample);

            // Mix dry (original) and wet (delayed) signals
            float outputSample = inputSample * (1.0f - mix) + delayedSample * mix;
            buffer.setSample(channel, sample, outputSample);
        }
    }
}

void ChorusEffect::setRate(float newRate)
{
    // Clamp rate to valid range and update LFO
    rate = juce::jlimit(0.1f, 10.0f, newRate);
    updateLFO();
}

void ChorusEffect::setDepth(float newDepth)
{
    // Clamp depth to valid range (0.0 = no modulation, 1.0 = full modulation)
    depth = juce::jlimit(0.0f, 1.0f, newDepth);
}

void ChorusEffect::setFeedback(float newFeedback)
{
    // Clamp feedback to safe range (0.95 max to prevent runaway feedback)
    feedback = juce::jlimit(0.0f, 0.95f, newFeedback);
}

void ChorusEffect::setMix(float newMix)
{
    // Clamp mix ratio to valid range
    mix = juce::jlimit(0.0f, 1.0f, newMix);
}

void ChorusEffect::updateLFO()
{
    // Calculate how much the LFO phase should advance per sample
    // Rate (Hz) / Sample Rate (Hz) = Phase increment per sample
    lfoPhaseIncrement = rate / sampleRate;
}