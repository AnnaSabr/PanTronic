/**
 * @file WaveformComponent.cpp
 * @brief Implementation of the WaveformComponent class for real-time audio visualization
 * @author [Author Name]
 * @date [Date]
 * @version 1.0
 */

#include "WaveformComponent.hpp"

/**
 * @brief Constructs the WaveformComponent with audio buffer references
 *
 * Initializes the component to visualize the provided audio buffer data.
 * Starts a timer at 60Hz to continuously update the display, providing
 * smooth real-time visualization of the audio waveform.
 *
 * @param bufferRef Reference to the audio buffer containing the data to visualize
 * @param writePosRef Reference to the current write position in the circular buffer
 *
 * @note The component assumes the buffer is used as a circular buffer where
 *       writePos indicates the most recently written sample position
 */
WaveformComponent::WaveformComponent(juce::AudioSampleBuffer &bufferRef, int &writePosRef)
    : buffer(bufferRef), writePos(writePosRef) {
    startTimerHz(60); // Starts timer to refresh display at ~60 frames per second
}

/**
 * @brief Handles the rendering of the component
 *
 * Overrides Component::paint to provide custom rendering of the waveform visualization.
 * Sets up the visual styling (black background, lime waveform) and delegates
 * the actual waveform drawing to drawWaveform().
 *
 * @param g The graphics context used for drawing operations
 */
void WaveformComponent::paint(juce::Graphics &g) {
    g.fillAll(juce::Colours::black);  // Set background to black for contrast
    g.setColour(juce::Colours::lime); // Set waveform color to lime green
    drawWaveform(g);                  // Draw the actual waveform visualization
}

/**
 * @brief Timer callback that triggers display updates
 *
 * Overrides Timer::timerCallback to provide continuous display updates.
 * Called at the frequency set by startTimerHz() (60Hz) to maintain smooth
 * real-time visualization of the changing audio buffer contents.
 */
void WaveformComponent::timerCallback() {
    repaint(); // Request a repaint to update the visual display
}

/**
 * @brief Renders the audio waveform as a continuous line
 *
 * Creates a visual representation of the audio buffer contents by:
 * 1. Starting visualization from the sample after the current write position
 * 2. Sampling across the buffer width to create a continuous waveform
 * 3. Handling circular buffer wraparound for seamless visualization
 * 4. Mapping audio sample values (-1.0 to 1.0) to screen coordinates
 *
 * The resulting visualization shows the most recent audio data on the right
 * side of the display, with older data scrolling to the left.
 *
 * @param g The graphics context used for drawing the waveform path
 *
 * @note Uses juce::Path for smooth line rendering and handles fractional
 *       sample indices through modulo arithmetic for circular buffer access
 */
void WaveformComponent::drawWaveform(juce::Graphics &g) const {
    auto width = getWidth();   // Get component width in pixels
    auto height = getHeight(); // Get component height in pixels

    juce::Path waveformPath;
    waveformPath.startNewSubPath(0.f, height / 2.f); // Start path at vertical center (zero amplitude)

    const int numSamples = buffer.getNumSamples();
    const float step = static_cast<float>(numSamples) / width; // Calculate samples per pixel
    const int start = (writePos + 1) % numSamples;             // Get starting point after current write position

    // Draw the waveform point by point across the component width
    for (int i = 0; i < width; ++i) {
        // Calculate the actual sample index with wraparound for circular buffer
        const float index = std::fmod(start + i * step, static_cast<float>(numSamples));
        // Get the audio sample value at this index (assumes mono channel 0)
        const float sample = buffer.getSample(0, static_cast<int>(index));
        // Map the sample value (-1 to 1) to screen coordinates (height to 0)
        const float y = juce::jmap(sample, -1.0f, 1.0f, static_cast<float>(height), 0.0f);
        waveformPath.lineTo(static_cast<float>(i), y);
    }

    g.strokePath(waveformPath, juce::PathStrokeType(1.0f)); // Render the final path with 1px stroke
}