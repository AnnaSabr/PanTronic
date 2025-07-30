#pragma once



#include "JuceHeader.h"

#include "juce_dsp/juce_dsp.h"



class ChorusEffect {

public:

    ChorusEffect();

    ~ChorusEffect();



    void prepare(const juce::dsp::ProcessSpec& spec);

    void processBlock(juce::AudioBuffer<float>& buffer);



    void setRate(float rate);

    void setDepth(float depth);

    void setFeedback(float feedback);

    void setMix(float mix);



private:

    struct DelayLine {

        std::vector<float> buffer;

        int writeIndex = 0;

        int bufferSize = 0;



        void setSize(int size) {

            bufferSize = size;

            buffer.resize(size);

            std::fill(buffer.begin(), buffer.end(), 0.0f);

            writeIndex = 0;

        }



        void write(float sample) {

            buffer[writeIndex] = sample;

            writeIndex = (writeIndex + 1) % bufferSize;

        }



        float read(float delayInSamples) {

            float readPosition = writeIndex - delayInSamples;

            if (readPosition < 0)

                readPosition += bufferSize;



            // Linear interpolation

            int index1 = static_cast<int>(readPosition) % bufferSize;

            int index2 = (index1 + 1) % bufferSize;

            float fraction = readPosition - std::floor(readPosition);



            return buffer[index1] * (1.0f - fraction) + buffer[index2] * fraction;

        }

    };



    DelayLine leftDelayLine, rightDelayLine;



    float sampleRate = 44100.0f;

    float rate = 0.5f;

    float depth = 0.5f;

    float feedback = 0.3f;

    float mix = 0.5f;



    float lfoPhase = 0.0f;

    float lfoPhaseIncrement = 0.0f;



    static constexpr float maxDelayTime = 0.05f; // 50ms max delay

    static constexpr float baseDelayTime = 0.005f; // 5ms base delay



    void updateLFO();

};
