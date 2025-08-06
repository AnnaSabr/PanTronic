#pragma once

#include "JuceHeader.h"
#include "juce_dsp/juce_dsp.h"

class SpectrumComponent : public juce::Component, public juce::Timer {
public:
    SpectrumComponent(juce::AudioBuffer<float>& buffer, int& writePos);
    ~SpectrumComponent() override;

    void paint(juce::Graphics& g) override;
    void resized() override;
    void timerCallback() override;

private:
    void processFFT();
    void drawFrequencyScale(juce::Graphics& g);
    void drawMagnitudeScale(juce::Graphics& g);

    static constexpr int fftOrder = 11;  // 2^11 = 2048 samples
    static constexpr int fftSize = 1 << fftOrder;
    static constexpr int scopeSize = 512;

    juce::dsp::FFT forwardFFT;
    juce::dsp::WindowingFunction<float> window;

    float fifo[fftSize];
    float fftData[2 * fftSize];
    int fifoIndex = 0;
    bool nextFFTBlockReady = false;
    float scopeData[scopeSize];

    // Referenzen zum Audio-Buffer
    juce::AudioBuffer<float>& audioBuffer;
    int& bufferWritePos;
    int lastReadPos = 0;

    // Smoothing für das Spektrum
    float smoothingFactor = 0.8f;
    bool firstFrame = true;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SpectrumComponent)
};


