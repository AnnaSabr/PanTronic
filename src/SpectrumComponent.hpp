/**
 * @file SpectrumComponent.hpp
 * @brief Header file for real-time spectrum analyzer component
 *
 * This file contains the declaration of the SpectrumComponent class, which provides
 * real-time frequency spectrum visualization of audio signals using FFT analysis.
 *
 * @author AvSynth Development Team
 * @version 1.0
 * @date 2024
 */

#pragma once

#include "JuceHeader.h"
#include "juce_dsp/juce_dsp.h"

/**
 * @class SpectrumComponent
 * @brief Real-time spectrum analyzer visualization component
 *
 * This class provides a real-time frequency spectrum display using Fast Fourier Transform (FFT).
 * It continuously analyzes incoming audio data and displays the frequency content with
 * logarithmic frequency scaling and dB magnitude scaling. Features include:
 * - Real-time FFT analysis with windowing
 * - Logarithmic frequency axis (20Hz to 20kHz)
 * - dB magnitude scaling (-100dB to 0dB)
 * - Smoothed spectrum display to reduce flickering
 * - Professional frequency and magnitude grid lines
 */
class SpectrumComponent : public juce::Component, public juce::Timer {
public:
    /**
     * @brief Constructor for SpectrumComponent
     *
     * Initializes the FFT analyzer, windowing function, and starts the update timer.
     * Sets up references to the audio buffer for continuous spectrum analysis.
     *
     * @param buffer Reference to the audio buffer to analyze
     * @param writePos Reference to the current write position in the buffer
     */
    SpectrumComponent(juce::AudioBuffer<float>& buffer, int& writePos);

    /**
     * @brief Destructor for SpectrumComponent
     *
     * Stops the update timer and cleans up resources.
     */
    ~SpectrumComponent() override;

    /**
     * @brief Paint method for rendering the spectrum display
     *
     * Draws the frequency spectrum with:
     * - Black background
     * - Light blue spectrum curve with gradient fill
     * - Frequency scale (logarithmic, 20Hz to 20kHz)
     * - Magnitude scale (linear in dB, -100dB to 0dB)
     * - Grid lines for reference
     *
     * @param g Graphics context for drawing operations
     */
    void paint(juce::Graphics& g) override;

    /**
     * @brief Resized method called when component bounds change
     *
     * Currently no specific resize handling is needed as the spectrum
     * adapts automatically to the component size during painting.
     */
    void resized() override;

    /**
     * @brief Timer callback for regular spectrum updates
     *
     * Called at 60 FPS to update the spectrum display. Reads new audio data
     * from the circular buffer, processes it through FFT when enough samples
     * are available, and triggers a repaint when the spectrum data is updated.
     */
    void timerCallback() override;

private:
    /**
     * @brief Processes audio data through FFT to generate spectrum
     *
     * Performs the following steps:
     * - Applies Hann windowing to reduce spectral leakage
     * - Executes forward FFT transformation
     * - Converts FFT bins to spectrum display data with logarithmic frequency mapping
     * - Applies smoothing to reduce visual flickering
     * - Normalizes magnitude values for display
     */
    void processFFT();

    /**
     * @brief Draws the frequency scale with labels and grid lines
     *
     * Creates a logarithmic frequency scale from 20Hz to 20kHz with
     * appropriate labels (20, 50, 100, 200, 500, 1k, 2k, 5k, 10k, 20k)
     * and vertical grid lines for visual reference.
     *
     * @param g Graphics context for drawing the frequency scale
     */
    void drawFrequencyScale(juce::Graphics& g);

    /**
     * @brief Draws the magnitude scale with dB labels and grid lines
     *
     * Creates a linear magnitude scale in decibels from -100dB to 0dB
     * with horizontal grid lines every 20dB for visual reference.
     *
     * @param g Graphics context for drawing the magnitude scale
     */
    void drawMagnitudeScale(juce::Graphics& g);

    // FFT Configuration Constants
    static constexpr int fftOrder = 11;              ///< FFT order (2^11 = 2048 samples)
    static constexpr int fftSize = 1 << fftOrder;    ///< FFT size in samples
    static constexpr int scopeSize = 512;            ///< Number of spectrum display bins

    // FFT Processing Components
    juce::dsp::FFT forwardFFT;                       ///< Forward FFT processor
    juce::dsp::WindowingFunction<float> window;      ///< Hann windowing function

    // Data Buffers
    float fifo[fftSize];                             ///< Input sample FIFO buffer
    float fftData[2 * fftSize];                      ///< FFT input/output buffer (real + imaginary)
    int fifoIndex = 0;                               ///< Current index in FIFO buffer
    bool nextFFTBlockReady = false;                  ///< Flag indicating when FFT block is ready
    float scopeData[scopeSize];                      ///< Processed spectrum data for display

    // Audio Buffer References
    juce::AudioBuffer<float>& audioBuffer;           ///< Reference to the main audio buffer
    int& bufferWritePos;                             ///< Reference to current write position
    int lastReadPos = 0;                             ///< Last read position for continuous processing

    // Display Smoothing
    float smoothingFactor = 0.8f;                    ///< Smoothing factor for spectrum display (0.0 = no smoothing, 1.0 = maximum smoothing)
    bool firstFrame = true;                          ///< Flag for first frame processing

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SpectrumComponent)
};