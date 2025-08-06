#include "SpectrumComponent.hpp"

SpectrumComponent::SpectrumComponent(juce::AudioBuffer<float>& buffer, int& writePos)
    : audioBuffer(buffer), bufferWritePos(writePos), forwardFFT(fftOrder), window(fftSize, juce::dsp::WindowingFunction<float>::hann)
{
    // Initialize arrays
    juce::zeromem(fifo, sizeof(fifo));
    juce::zeromem(fftData, sizeof(fftData));
    juce::zeromem(scopeData, sizeof(scopeData));

    // Start timer for regular updates (60 FPS)
    startTimer(1000 / 60);
}

SpectrumComponent::~SpectrumComponent()
{
    stopTimer();
}

void SpectrumComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);

    // Draw frequency and magnitude scales
    drawFrequencyScale(g);
    drawMagnitudeScale(g);

    // Draw spectrum
    g.setColour(juce::Colours::lightblue);

    auto area = getLocalBounds().reduced(40, 20);
    auto width = area.getWidth();
    auto height = area.getHeight();

    juce::Path spectrumPath;
    bool pathStarted = false;

    for (int i = 1; i < scopeSize; ++i)
    {
        auto x = juce::jmap(float(i), 0.0f, float(scopeSize), float(area.getX()), float(area.getRight()));

        // Convert magnitude to dB and map to pixel height
        auto magnitude = scopeData[i];
        auto dB = magnitude > 0.0f ? juce::Decibels::gainToDecibels(magnitude) : -100.0f;
        auto y = juce::jmap(juce::jlimit(-100.0f, 0.0f, dB), -100.0f, 0.0f, float(area.getBottom()), float(area.getY()));

        if (!pathStarted)
        {
            spectrumPath.startNewSubPath(x, y);
            pathStarted = true;
        }
        else
        {
            spectrumPath.lineTo(x, y);
        }
    }

    g.strokePath(spectrumPath, juce::PathStrokeType(1.5f));

    // Fill area under curve with gradient
    g.setGradientFill(juce::ColourGradient(juce::Colours::lightblue.withAlpha(0.3f),
                                          area.getCentreX(), area.getY(),
                                          juce::Colours::lightblue.withAlpha(0.05f),
                                          area.getCentreX(), area.getBottom(), false));

    juce::Path fillPath = spectrumPath;
    fillPath.lineTo(area.getRight(), area.getBottom());
    fillPath.lineTo(area.getX(), area.getBottom());
    fillPath.closeSubPath();

    g.fillPath(fillPath);
}

void SpectrumComponent::resized()
{
    // Component resized - nothing specific to do here
}

void SpectrumComponent::timerCallback()
{
    // Read new audio data from the circular buffer
    if (audioBuffer.getNumSamples() > 0)
    {
        auto numSamples = audioBuffer.getNumSamples();
        auto currentWritePos = bufferWritePos;

        // Debug: Check if we have any audio data
        static int debugCounter = 0;
        if (++debugCounter % 60 == 0) // Print every second
        {
            float maxSample = 0.0f;
            for (int i = 0; i < numSamples; ++i)
            {
                maxSample = juce::jmax(maxSample, std::abs(audioBuffer.getSample(0, i)));
            }
            DBG("Audio max level: " << maxSample << ", Write pos: " << currentWritePos);
        }

        // Calculate how many samples to read
        int samplesToRead = 0;
        if (currentWritePos >= lastReadPos)
        {
            samplesToRead = currentWritePos - lastReadPos;
        }
        else
        {
            samplesToRead = (numSamples - lastReadPos) + currentWritePos;
        }

        // Limit samples to read to avoid buffer overflow
        samplesToRead = juce::jmin(samplesToRead, fftSize - fifoIndex);

        for (int i = 0; i < samplesToRead; ++i)
        {
            auto readPos = (lastReadPos + i) % numSamples;
            fifo[fifoIndex] = audioBuffer.getSample(0, readPos);

            if (++fifoIndex >= fftSize)
            {
                nextFFTBlockReady = true;
                fifoIndex = 0;
                DBG("FFT block ready!"); // Debug
                break;
            }
        }

        lastReadPos = (lastReadPos + samplesToRead) % numSamples;

        if (nextFFTBlockReady)
        {
            processFFT();
            nextFFTBlockReady = false;
            repaint();
        }
    }
}

void SpectrumComponent::processFFT()
{
    // Debug: Check input data
    float maxInput = 0.0f;
    for (int i = 0; i < fftSize; ++i)
    {
        maxInput = juce::jmax(maxInput, std::abs(fifo[i]));
    }
    DBG("FFT input max: " << maxInput);

    // Copy fifo to fftData
    for (int i = 0; i < fftSize; ++i)
        fftData[i] = fifo[i];

    // Zero out the imaginary parts
    for (int i = fftSize; i < 2 * fftSize; ++i)
        fftData[i] = 0.0f;

    // Apply windowing function
    window.multiplyWithWindowingTable(fftData, fftSize);

    // Perform FFT
    forwardFFT.performFrequencyOnlyForwardTransform(fftData);

    // Debug: Check FFT output
    float maxFFT = 0.0f;
    for (int i = 0; i < fftSize / 2; ++i)
    {
        maxFFT = juce::jmax(maxFFT, fftData[i]);
    }
    DBG("FFT output max: " << maxFFT);

    // Convert to scope data with smoothing
    for (int i = 0; i < scopeSize; ++i)
    {
        // Map scope index to FFT bin (logarithmic scaling for better frequency resolution)
        auto skewedProportionX = 1.0f - std::exp(std::log(1.0f - float(i) / float(scopeSize)) * 0.2f);
        auto fftDataIndex = juce::jlimit(0, fftSize / 2, int(skewedProportionX * (fftSize / 2)));

        // Get magnitude and normalize
        auto magnitude = fftData[fftDataIndex];
        auto normalizedMagnitude = magnitude / float(fftSize / 4); // Normalize

        // Apply smoothing
        if (firstFrame)
        {
            scopeData[i] = normalizedMagnitude;
        }
        else
        {
            scopeData[i] = smoothingFactor * scopeData[i] + (1.0f - smoothingFactor) * normalizedMagnitude;
        }
    }

    firstFrame = false;
}

void SpectrumComponent::drawFrequencyScale(juce::Graphics& g)
{
    g.setColour(juce::Colours::white.withAlpha(0.7f));
    g.setFont(10.0f);

    auto area = getLocalBounds().reduced(40, 20);

    // Draw frequency labels (logarithmic scale)
    std::vector<float> frequencies = {20, 50, 100, 200, 500, 1000, 2000, 5000, 10000, 20000};

    for (auto freq : frequencies)
    {
        // Map frequency to x position (logarithmic)
        auto proportion = (std::log10(freq) - std::log10(20.0f)) / (std::log10(20000.0f) - std::log10(20.0f));
        auto x = area.getX() + proportion * area.getWidth();

        if (x >= area.getX() && x <= area.getRight())
        {
            g.drawVerticalLine(int(x), area.getY(), area.getBottom());

            juce::String label;
            if (freq >= 1000)
                label = juce::String(freq / 1000, 1) + "k";
            else
                label = juce::String(int(freq));

            g.drawText(label, int(x - 15), area.getBottom() + 2, 30, 15,
                      juce::Justification::centred);
        }
    }
}

void SpectrumComponent::drawMagnitudeScale(juce::Graphics& g)
{
    g.setColour(juce::Colours::white.withAlpha(0.7f));
    g.setFont(10.0f);

    auto area = getLocalBounds().reduced(40, 20);

    // Draw dB scale
    for (int dB = -100; dB <= 0; dB += 20)
    {
        auto y = juce::jmap(float(dB), -100.0f, 0.0f, float(area.getBottom()), float(area.getY()));

        g.drawHorizontalLine(int(y), area.getX(), area.getRight());
        g.drawText(juce::String(dB) + " dB", 2, int(y - 6), 35, 12,
                  juce::Justification::centredLeft);
    }
}