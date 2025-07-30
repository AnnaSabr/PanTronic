#include "ChorusEffect.hpp"

ChorusEffect::ChorusEffect()
{}

ChorusEffect::~ChorusEffect()
{}

void ChorusEffect::prepare(const juce::dsp::ProcessSpec& spec)
{
    sampleRate = static_cast<float>(spec.sampleRate);

    // Calculate delay buffer size
    int delayBufferSize = static_cast<int>(sampleRate * maxDelayTime) + 1;

    leftDelayLine.setSize(delayBufferSize);
    rightDelayLine.setSize(delayBufferSize);

    updateLFO();
}

void ChorusEffect::processBlock(juce::AudioBuffer<float>& buffer)
{
    auto numSamples = buffer.getNumSamples();
    auto numChannels = buffer.getNumChannels();

    for (int sample = 0; sample < numSamples; ++sample)
    {
        // Update LFO
        float lfoValue = std::sin(lfoPhase * juce::MathConstants<float>::twoPi);
        lfoPhase += lfoPhaseIncrement;
        if (lfoPhase >= 1.0f)
            lfoPhase -= 1.0f;

        // Calculate modulated delay time
        float modulatedDelay = baseDelayTime + (depth * maxDelayTime * 0.5f * (lfoValue + 1.0f));
        float delayInSamples = modulatedDelay * sampleRate;

        for (int channel = 0; channel < numChannels; ++channel)
        {
            auto& delayLine = (channel == 0) ? leftDelayLine : rightDelayLine;

            float inputSample = buffer.getSample(channel, sample);
            float delayedSample = delayLine.read(delayInSamples);

            // Add feedback
            float feedbackSample = delayedSample * feedback;
            delayLine.write(inputSample + feedbackSample);

            // Mix dry and wet signals
            float outputSample = inputSample * (1.0f - mix) + delayedSample * mix;
            buffer.setSample(channel, sample, outputSample);
        }
    }
}

void ChorusEffect::setRate(float newRate)
{
    rate = juce::jlimit(0.1f, 10.0f, newRate);
    updateLFO();
}

void ChorusEffect::setDepth(float newDepth)
{
    depth = juce::jlimit(0.0f, 1.0f, newDepth);
}

void ChorusEffect::setFeedback(float newFeedback)
{
    feedback = juce::jlimit(0.0f, 0.95f, newFeedback);
}

void ChorusEffect::setMix(float newMix)
{
    mix = juce::jlimit(0.0f, 1.0f, newMix);
}

void ChorusEffect::updateLFO()
{
    lfoPhaseIncrement = rate / sampleRate;
}