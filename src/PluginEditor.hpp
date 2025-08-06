#pragma once

#include "ADSRComponent.hpp"
#include "ChorusComponent.hpp"
#include "PluginProcessor.hpp"
#include "ReverbComponent.hpp"
#include "SpectrumComponent.hpp"
#include "WaveformComponent.hpp"
#include "MysticalLookAndFeel.hpp"

/**
 * @file PluginEditor.hpp
 * @brief Main audio processor editor for the AvSynth synthesizer plugin
 * @author Your Name
 * @date 2024
 */

/**
 * @class AvSynthAudioProcessorEditor
 * @brief The main GUI editor class for the AvSynth audio synthesizer
 *
 * This class provides the complete user interface for the AvSynth synthesizer,
 * including controls for oscillators, ADSR envelope, effects (chorus, reverb),
 * filters, and real-time audio visualization components.
 *
 * The editor features a mystical-themed design with custom graphics and
 * a specialized look-and-feel implementation.
 *
 * @inherits juce::AudioProcessorEditor
 * @inherits juce::AudioProcessorValueTreeState::Listener
 */
class AvSynthAudioProcessorEditor final : public juce::AudioProcessorEditor,
                                          public juce::AudioProcessorValueTreeState::Listener {
public:
    /**
     * @brief Constructor for the audio processor editor
     * @param p Reference to the AvSynthAudioProcessor instance
     *
     * Initializes all GUI components, sets up parameter attachments,
     * loads the mystical background image, and configures the custom look-and-feel.
     */
    explicit AvSynthAudioProcessorEditor(AvSynthAudioProcessor &p);

    /**
     * @brief Destructor
     *
     * Cleans up parameter listeners and resets the look-and-feel to prevent
     * memory leaks and dangling references.
     */
    ~AvSynthAudioProcessorEditor() override;

    //==============================================================================

    /**
     * @brief Paints the editor's background and visual elements
     * @param g Graphics context for drawing operations
     *
     * Renders the mystical background gradient, draws the Pan image,
     * and adds subtle glow effects in the corners for atmospheric lighting.
     */
    void paint(juce::Graphics &g) override;

    /**
     * @brief Handles component layout and resizing
     *
     * Arranges all GUI components (sliders, labels, effects panels, keyboard,
     * and visualization components) in a responsive layout that adapts to
     * window size changes.
     */
    void resized() override;

    /**
     * @brief Parameter change listener callback
     * @param parameterID String identifier of the changed parameter
     * @param newValue New normalized value (0.0-1.0) of the parameter
     *
     * Updates the corresponding GUI components when parameters change,
     * ensuring visual synchronization with audio parameter states.
     * Handles ADSR, Reverb, and Chorus parameter updates with proper scaling.
     */
    void parameterChanged(const juce::String& parameterID, float newValue) override;

    /**
     * @brief Loads a complete flute preset
     *
     * Sets up a realistic flute sound by configuring:
     * - Oscillator type to flute waveform
     * - ADSR envelope with flute-appropriate timing
     * - Filter settings for natural flute frequency response
     * - Reverb parameters for concert hall acoustics
     */
    void loadFlutePreset();

    /**
     * @brief Sets ADSR envelope parameters for flute sound
     *
     * Configures attack, decay, sustain, and release values
     * that mimic the natural envelope characteristics of a flute:
     * - Moderate attack for breath control simulation
     * - Medium decay and high sustain for wind instrument behavior
     * - Controlled release for realistic note endings
     */
    void setFlutePreset();

    /**
     * @brief Configures filter settings for flute timbre
     *
     * Sets high-pass and low-pass filter frequencies to match
     * the natural frequency response of a concert flute:
     * - Low-pass at 8kHz to remove harsh high frequencies
     * - High-pass at 80Hz to reduce unnecessary low-end rumble
     */
    void setFluteFilterPreset();

    /**
     * @brief Sets reverb parameters for flute acoustics
     *
     * Configures reverb settings to simulate a concert hall environment
     * suitable for flute performance:
     * - Medium room size for natural acoustics
     * - Moderate damping for clarity
     * - Balanced wet/dry mix for spatial presence
     */
    void setFluteReverbPreset();

    /**
     * @brief Loads the mystical background image
     *
     * Attempts to load the Pan.jpg image from multiple sources:
     * 1. Binary data embedded in the plugin
     * 2. External file in Resources directory
     * 3. Fallback procedural gradient if image loading fails
     *
     * Ensures the editor always has a mystical background regardless
     * of file availability.
     */
    void loadMysticalImage();

    /**
     * @brief Draws the mystical background image
     * @param g Graphics context for drawing operations
     *
     * Renders the loaded mystical image (Pan) with proper scaling
     * and positioning in the upper-right area of the interface.
     * Handles cases where the image is invalid gracefully.
     */
    void drawMysticalImage(juce::Graphics& g);

private:
    /**
     * @brief Returns a vector of all GUI components for batch operations
     * @return std::vector<juce::Component*> Collection of all UI components
     *
     * Used internally for adding all components to the editor and
     * managing their visibility states efficiently.
     */
    std::vector<juce::Component *> GetComps();

    /**
     * @brief Initializes the ADSR envelope component
     *
     * Sets up parameter bindings, initial values, and change callbacks
     * for the interactive ADSR envelope visualizer. Handles bidirectional
     * communication between GUI and audio parameters with proper scaling.
     */
    void setupADSRComponent();

    /**
     * @brief Initializes the reverb effect component
     *
     * Configures the reverb GUI panel with parameter attachments,
     * initial values from the processor, and change callbacks for
     * real-time reverb parameter control.
     */
    void setupReverbComponent();

    /**
     * @brief Initializes the chorus effect component
     *
     * Sets up the chorus effect GUI with proper parameter bindings,
     * initial values, and real-time update callbacks for rate, depth,
     * feedback, and mix controls.
     */
    void setupChorusComponent();

    //==============================================================================
    // Core Components

    /**
     * @brief Reference to the audio processor
     *
     * Provides access to the processor's parameters, state, and audio processing
     * components for GUI-to-audio communication.
     */
    AvSynthAudioProcessor &processorRef;

    //==============================================================================
    // UI Labels

    juce::Label gainLabel;          ///< Label for the gain/volume control
    juce::Label frequencyLabel;     ///< Label for the frequency control
    juce::Label oscTypeLabel;       ///< Label for the oscillator type selector
    juce::Label lowCutFreqLabel;    ///< Label for the low-pass filter frequency
    juce::Label highCutFreqLabel;   ///< Label for the high-pass filter frequency
    juce::Label adsrLabel;          ///< Label for the ADSR envelope section
    juce::Label reverbLabel;        ///< Label for the reverb effect section
    juce::Label chorusLabel;        ///< Label for the chorus effect section
    juce::Label spectrumLabel;      ///< Label for the spectrum analyzer

    //==============================================================================
    // Main Controls

    juce::Slider gainSlider;        ///< Master volume/gain control slider
    juce::AudioProcessorValueTreeState::SliderAttachment gainAttachment;  ///< Parameter attachment for gain

    juce::Slider frequencySlider;   ///< Base frequency control slider
    juce::AudioProcessorValueTreeState::SliderAttachment frequencyAttachment;  ///< Parameter attachment for frequency

    juce::ComboBox oscTypeComboBox; ///< Oscillator waveform type selector
    juce::AudioProcessorValueTreeState::ComboBoxAttachment oscTypeAttachment;  ///< Parameter attachment for oscillator type

    //==============================================================================
    // Filter Controls

    juce::Slider lowCutFreqSlider;  ///< Low-pass filter frequency control
    juce::AudioProcessorValueTreeState::SliderAttachment lowCutFreqAttachment;  ///< Parameter attachment for low-pass filter

    juce::Slider highCutFreqSlider; ///< High-pass filter frequency control
    juce::AudioProcessorValueTreeState::SliderAttachment highCutFreqAttachment;  ///< Parameter attachment for high-pass filter

    //==============================================================================
    // Visual and Interactive Components

    /**
     * @brief Mystical background image (Pan deity)
     *
     * Stores the loaded background image that provides the mystical
     * aesthetic theme for the synthesizer interface.
     */
    juce::Image mysticalImage;

    /**
     * @brief Custom look-and-feel implementation
     *
     * Provides the mystical visual styling for all GUI components
     * with custom colors, gradients, and glow effects.
     */
    MysticalLookAndFeel mysticalLookAndFeel;

    //==============================================================================
    // Effect and Envelope Components

    /**
     * @brief Interactive ADSR envelope visualizer and editor
     *
     * Provides visual representation and interactive control of the
     * attack, decay, sustain, and release parameters with real-time feedback.
     */
    ADSRComponent adsrComponent;

    /**
     * @brief Chorus effect control panel
     *
     * Interactive GUI component for controlling chorus effect parameters
     * including rate, depth, feedback, and wet/dry mix.
     */
    ChorusComponent chorusComponent;

    /**
     * @brief Reverb effect control panel
     *
     * GUI component for reverb effect control with parameters for
     * room size, damping, wet level, dry level, and stereo width.
     */
    ReverbComponent reverbComponent;

    //==============================================================================
    // Preset and Input Components

    /**
     * @brief Flute preset activation button
     *
     * One-click button that loads a complete flute sound preset,
     * configuring all relevant parameters for realistic flute emulation.
     */
    juce::TextButton flutePresetButton;

    /**
     * @brief Virtual MIDI keyboard component
     *
     * Interactive on-screen keyboard for note input and testing,
     * with custom mystical color scheme matching the overall design.
     */
    juce::MidiKeyboardComponent keyboardComponent;

    //==============================================================================
    // Audio Visualization Components

    /**
     * @brief Real-time waveform display
     *
     * Shows the current audio waveform in real-time, providing
     * visual feedback of the generated audio signal.
     */
    WaveformComponent waveformComponent;

    /**
     * @brief Real-time spectrum analyzer
     *
     * Displays the frequency spectrum of the generated audio,
     * useful for understanding the harmonic content and filter effects.
     */
    SpectrumComponent spectrumComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AvSynthAudioProcessorEditor)
};