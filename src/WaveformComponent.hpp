/**
 * @file WaveformComponent.hpp
 * @brief Header file for the WaveformComponent class
 * @author [Author Name]
 * @date [Date]
 * @version 1.0
 */

#pragma once

#include "JuceHeader.h"

/**
 * @brief A JUCE component that displays real-time audio waveform visualization
 *
 * This class inherits from both juce::Component and juce::Timer to provide
 * a continuously updating visual representation of an audio buffer. The waveform
 * is displayed as a lime-colored line on a black background, updating at approximately
 * 60 frames per second.
 *
 * The component visualizes the audio data starting from the current write position
 * and wrapping around the circular buffer, providing a continuous scrolling effect
 * that shows the most recent audio data.
 */
class WaveformComponent : public juce::Component, public juce::Timer {
  public:
    /**
     * @brief Constructs the WaveformComponent with references to audio data
     *
     * Initializes the component with references to an audio buffer and its current
     * write position. Starts an internal timer at 60Hz for display updates.
     *
     * @param bufferRef Reference to the audio buffer containing the data to visualize
     * @param writePosRef Reference to the current write position in the buffer
     *
     * @note The references must remain valid for the lifetime of this component
     */
    WaveformComponent(juce::AudioSampleBuffer &bufferRef, int &writePosRef);

    /**
     * @brief Renders the waveform visualization
     *
     * Overrides the Component::paint method to draw the waveform.
     * Sets a black background and draws the waveform in lime color.
     *
     * @param g The graphics context used for rendering
     */
    void paint(juce::Graphics &g) override;

  private:
    /**
     * @brief Timer callback for display updates
     *
     * Overrides Timer::timerCallback to trigger repainting of the component.
     * Called at the frequency set by startTimerHz() (60Hz by default).
     */
    void timerCallback() override;

    /**
     * @brief Renders the actual waveform path
     *
     * Creates and draws a continuous line representing the audio waveform.
     * The visualization starts from the current write position and wraps around
     * the circular buffer, mapping audio samples (-1 to 1) to screen coordinates.
     *
     * @param g The graphics context used for drawing the waveform
     *
     * @note This method handles buffer wraparound and sample interpolation
     *       to create a smooth visual representation
     */
    void drawWaveform(juce::Graphics &g) const;

    juce::AudioSampleBuffer &buffer; ///< Reference to the audio buffer to visualize
    int &writePos;                   ///< Reference to the current write position in the buffer
};