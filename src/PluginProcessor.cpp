/**
 * @file PluginProcessor.cpp
 * @brief Implementation of the main audio processor for the AvSynth synthesizer plugin
 *
 * This file contains the core audio processing logic for the AvSynth synthesizer,
 * including oscillator generation, filtering, ADSR envelope, reverb, and chorus effects.
 *
 * @author AvSynth Development Team
 * @version 1.0
 * @date 2024
 */

#include "PluginProcessor.hpp"
#include "PluginEditor.hpp"
#include "Utils.hpp"
#include <magic_enum/magic_enum.hpp>
#include "ChorusEffect.hpp"

/**
 * @brief Retrieves the current parameter values from the ValueTreeState
 *
 * This method creates a new ChainSettings object and populates it with current parameter values
 * from the audio processor's parameter tree. Uses magic_enum for type-safe parameter access.
 *
 * @param parameters Reference to the AudioProcessorValueTreeState containing all plugin parameters
 * @return ChainSettings A struct containing all current parameter values
 */
AvSynthAudioProcessor::ChainSettings
AvSynthAudioProcessor::ChainSettings::Get(const juce::AudioProcessorValueTreeState &parameters) {
    ChainSettings settings{};

    // Load the gain parameter value using magic_enum to convert enum to string
    settings.gain = parameters.getRawParameterValue(magic_enum::enum_name<Parameters::Gain>().data())->load();
    // Load the frequency parameter value using magic_enum to convert enum to string
    settings.frequency = parameters.getRawParameterValue(magic_enum::enum_name<Parameters::Frequency>().data())->load();
    settings.oscType = static_cast<OscType>(
        static_cast<int>(parameters.getRawParameterValue(magic_enum::enum_name<Parameters::OscType>().data())->load()));
    settings.LowPassFreq = parameters.getRawParameterValue(magic_enum::enum_name<Parameters::LowPassFreq>().data())->load();
    settings.HighPassFreq = parameters.getRawParameterValue(magic_enum::enum_name<Parameters::HighPassFreq>().data())->load();

    // Load ADSR parameters
    settings.attack = parameters.getRawParameterValue(magic_enum::enum_name<Parameters::Attack>().data())->load();
    settings.decay = parameters.getRawParameterValue(magic_enum::enum_name<Parameters::Decay>().data())->load();
    settings.sustain = parameters.getRawParameterValue(magic_enum::enum_name<Parameters::Sustain>().data())->load();
    settings.release = parameters.getRawParameterValue(magic_enum::enum_name<Parameters::Release>().data())->load();

    // Load Reverb parameters
    settings.reverbRoomSize = parameters.getRawParameterValue(magic_enum::enum_name<Parameters::ReverbRoomSize>().data())->load();
    settings.reverbDamping = parameters.getRawParameterValue(magic_enum::enum_name<Parameters::ReverbDamping>().data())->load();
    settings.reverbWetLevel = parameters.getRawParameterValue(magic_enum::enum_name<Parameters::ReverbWetLevel>().data())->load();
    settings.reverbDryLevel = parameters.getRawParameterValue(magic_enum::enum_name<Parameters::ReverbDryLevel>().data())->load();
    settings.reverbWidth = parameters.getRawParameterValue(magic_enum::enum_name<Parameters::ReverbWidth>().data())->load();

    // Load Chorus parameters
    settings.chorusRate = parameters.getRawParameterValue(magic_enum::enum_name<Parameters::ChorusRate>().data())->load();
    settings.chorusDepth = parameters.getRawParameterValue(magic_enum::enum_name<Parameters::ChorusDepth>().data())->load();
    settings.chorusFeedback = parameters.getRawParameterValue(magic_enum::enum_name<Parameters::ChorusFeedback>().data())->load();
    settings.chorusMix = parameters.getRawParameterValue(magic_enum::enum_name<Parameters::ChorusMix>().data())->load();

    return settings;
}

/**
 * @brief Constructor for the AvSynthAudioProcessor
 *
 * Initializes the audio processor with appropriate bus configuration for a synthesizer plugin.
 * Sets up stereo output and MIDI input capabilities.
 */
AvSynthAudioProcessor::AvSynthAudioProcessor()
    : AudioProcessor(BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
                         .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
                         .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
      ) {
}

/**
 * @brief Destructor for the AvSynthAudioProcessor
 */
AvSynthAudioProcessor::~AvSynthAudioProcessor() {}

//==============================================================================

/**
 * @brief Returns the name of the plugin
 * @return String containing the plugin name as defined in JucePlugin_Name
 */
const juce::String AvSynthAudioProcessor::getName() const { return JucePlugin_Name; }

/**
 * @brief Indicates whether the plugin accepts MIDI input
 * @return true if MIDI input is enabled, false otherwise
 */
bool AvSynthAudioProcessor::acceptsMidi() const {
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

/**
 * @brief Indicates whether the plugin produces MIDI output
 * @return true if MIDI output is enabled, false otherwise
 */
bool AvSynthAudioProcessor::producesMidi() const {
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

/**
 * @brief Indicates whether this is a MIDI effect plugin
 * @return true if this is a MIDI effect, false otherwise
 */
bool AvSynthAudioProcessor::isMidiEffect() const {
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

/**
 * @brief Returns the tail length in seconds for reverb and other time-based effects
 * @return double The tail length in seconds (currently 0.0 as not implemented)
 */
double AvSynthAudioProcessor::getTailLengthSeconds() const { return 0.0; }

/**
 * @brief Returns the number of available programs/presets
 * @return int Number of programs (always returns 1 for compatibility)
 */
int AvSynthAudioProcessor::getNumPrograms() {
    return 1; // NB: some hosts don't cope very well if you tell them there are 0 programs,
              // so this should be at least 1, even if you're not really implementing programs.
}

/**
 * @brief Returns the currently selected program index
 * @return int The current program index (always 0)
 */
int AvSynthAudioProcessor::getCurrentProgram() { return 0; }

/**
 * @brief Sets the current program by index
 * @param index The program index to set (ignored in this implementation)
 */
void AvSynthAudioProcessor::setCurrentProgram(int index) { juce::ignoreUnused(index); }

/**
 * @brief Returns the name of a program by index
 * @param index The program index (ignored in this implementation)
 * @return String The program name (empty in this implementation)
 */
const juce::String AvSynthAudioProcessor::getProgramName(int index) {
    juce::ignoreUnused(index);
    return {};
}

/**
 * @brief Changes the name of a program
 * @param index The program index (ignored)
 * @param newName The new program name (ignored)
 */
void AvSynthAudioProcessor::changeProgramName(int index, const juce::String &newName) {
    juce::ignoreUnused(index, newName);
}

//==============================================================================

/**
 * @brief Prepares the processor for audio playback
 *
 * This method is called before audio processing begins. It initializes all audio processing
 * components including filters, reverb, chorus, ADSR envelope, and circular buffer for visualization.
 *
 * @param sampleRate The sample rate at which audio will be processed
 * @param samplesPerBlock Maximum number of samples that will be processed in each block
 */
void AvSynthAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    juce::ignoreUnused(sampleRate);

    previousChainSettings = ChainSettings::Get(parameters);
    circularBuffer.setSize(1, samplesPerBlock * 4);

    updateAngleDelta(previousChainSettings.frequency);

    juce::dsp::ProcessSpec spec{};
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = 1;

    leftChain.prepare(spec);
    rightChain.prepare(spec);

    // Prepare reverb
    reverb.prepare(spec);
    updateReverbParameters(previousChainSettings);

    updateLowPassCoefficients(previousChainSettings.LowPassFreq);
    updateHighPassCoefficients(previousChainSettings.HighPassFreq);

    // Initialize ADSR
    adsr.setSampleRate(sampleRate);

    // Initialize Chorus
    chorus.prepare(spec);
    updateChorusParameters(previousChainSettings);
}

/**
 * @brief Called when audio playback stops
 *
 * This method can be used to free up any spare memory or resources when playback stops.
 */
void AvSynthAudioProcessor::releaseResources() {
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

/**
 * @brief Checks if a specific bus layout is supported
 *
 * Validates whether the plugin can handle the requested input/output channel configuration.
 * This implementation supports mono and stereo layouts.
 *
 * @param layouts The bus layout configuration to check
 * @return true if the layout is supported, false otherwise
 */
bool AvSynthAudioProcessor::isBusesLayoutSupported(const BusesLayout &layouts) const {
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() &&
        layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
#if !JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}

/**
 * @brief Main audio processing function called by the host to process a block of audio data
 *
 * This is the core method where all audio synthesis and processing occurs. It handles:
 * - MIDI message processing for note on/off events
 * - Oscillator synthesis with frequency smoothing
 * - ADSR envelope application
 * - Filter processing (high-pass and low-pass)
 * - Chorus and reverb effects
 * - Output gain application
 * - Circular buffer updates for visualization
 *
 * @param buffer Audio buffer containing the input/output audio data
 * @param midiMessages MIDI messages to be processed for this audio block
 */
void AvSynthAudioProcessor::processBlock(juce::AudioBuffer<float> &buffer, juce::MidiBuffer &midiMessages) {
    // Ignore MIDI messages as they're not used in this processor
    juce::ignoreUnused(midiMessages);
    // Prevent denormalized numbers in audio calculations for better performance
    juce::ScopedNoDenormals noDenormals;
    const auto totalNumOutputChannels = getTotalNumOutputChannels();

    // Merge keyboardComponent MIDI events into midiMessages
    keyboardState.processNextMidiBuffer(midiMessages, 0, buffer.getNumSamples(), true);

    // Get current parameter values
    const auto chainSettings = ChainSettings::Get(parameters);

    // Update ADSR parameters if they have changed
    adsrParams.attack = chainSettings.attack;
    adsrParams.decay = chainSettings.decay;
    adsrParams.sustain = chainSettings.sustain;
    adsrParams.release = chainSettings.release;
    adsr.setParameters(adsrParams);

    // Update reverb parameters
    updateReverbParameters(chainSettings);

    // Process MIDI messages
    if (!midiMessages.isEmpty()) {
        for (const auto &message : midiMessages) {
            if (message.getMessage().isNoteOn()) {
                float frequency =
                    static_cast<float>(juce::MidiMessage::getMidiNoteInHertz(message.getMessage().getNoteNumber()));
                // Set the frequency based on the MIDI note number
                auto *freqParam = parameters.getParameter(magic_enum::enum_name<Parameters::Frequency>().data());
                if (auto *floatParam = dynamic_cast<juce::AudioParameterFloat *>(freqParam)) {
                    // The value needs to be normalized to the range of 0 to 1 for the parameter
                    float normValue = floatParam->convertTo0to1(frequency);
                    floatParam->setValueNotifyingHost(normValue);
                }

                // Trigger ADSR note on
                adsr.noteOn();
                noteIsOn = true;
                break;
            }
            else if (message.getMessage().isNoteOff()) {
                // Trigger ADSR note off
                adsr.noteOff();
                noteIsOn = false;
            }
        }
    }

    // Check if the frequency has changed since the last process call
    if (!juce::approximatelyEqual(previousChainSettings.frequency, chainSettings.frequency)) {
        // Create a linear ramp to smoothly transition the frequency
        LinearRamp<float> frequencyRamp;
        frequencyRamp.reset(previousChainSettings.frequency, chainSettings.frequency, buffer.getNumSamples());

        for (auto sample = 0; sample < buffer.getNumSamples(); ++sample) {
            auto currentSample = getOscSample(chainSettings.oscType, currentAngle);
            currentAngle += angleDelta;
            updateAngleDelta(frequencyRamp.getNext());

            // Apply ADSR envelope
            float envelopeValue = adsr.getNextSample();
            currentSample *= envelopeValue;

            // Write the current sample to all output channels
            for (int channel = 0; channel < totalNumOutputChannels; ++channel) {
                buffer.getWritePointer(channel)[sample] = currentSample;
            }
        }
    } else {
        // Sample the sine wave at the current sample rate and write the samples to the buffers
        for (auto sample = 0; sample < buffer.getNumSamples(); ++sample) {
            auto currentSample = getOscSample(chainSettings.oscType, currentAngle);
            currentAngle += angleDelta;

            // Apply ADSR envelope
            float envelopeValue = adsr.getNextSample();
            currentSample *= envelopeValue;

            // Write the current sample to all output channels
            for (int channel = 0; channel < totalNumOutputChannels; ++channel) {
                buffer.getWritePointer(channel)[sample] = currentSample;
            }
        }
    }

    updateLowPassCoefficients(chainSettings.LowPassFreq);
    updateHighPassCoefficients(chainSettings.HighPassFreq);

    // Apply the filters to the audio buffer
    juce::dsp::AudioBlock<float> block(buffer);

    auto leftBlock = block.getSingleChannelBlock(0);
    auto rightBlock = block.getSingleChannelBlock(1);

    juce::dsp::ProcessContextReplacing<float> leftContext(leftBlock);
    juce::dsp::ProcessContextReplacing<float> rightContext(rightBlock);

    leftChain.process(leftContext);
    rightChain.process(rightContext);

    // Update Chorus parameters if they have changed
    updateChorusParameters(chainSettings);
    // Apply Chorus effect
    chorus.processBlock(buffer);

    // Apply reverb effect
    juce::dsp::ProcessContextReplacing<float> reverbContext(block);
    reverb.process(reverbContext);

    if (juce::approximatelyEqual(chainSettings.gain, previousChainSettings.gain)) {
        for (int channel = 0; channel < totalNumOutputChannels; ++channel) {
            buffer.applyGain(channel, 0, buffer.getNumSamples(), previousChainSettings.gain);
        }
    } else {
        for (int channel = 0; channel < totalNumOutputChannels; ++channel) {
            buffer.applyGainRamp(channel, 0, buffer.getNumSamples(), previousChainSettings.gain, chainSettings.gain);
        }
    }

    previousChainSettings = chainSettings;

    // Get pointer to the first channel's data (mono processing for simplicity)
    const float *channelData = buffer.getReadPointer(0);

    // Copy input samples to circular buffer for visualization
    for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
        // Write current sample to circular buffer
        circularBuffer.setSample(0, bufferWritePos, channelData[sample]);
        // Increment write position and wrap around if necessary
        bufferWritePos = (bufferWritePos + 1) % circularBuffer.getNumSamples();
    }
}

//==============================================================================

/**
 * @brief Indicates whether the processor has a custom editor
 * @return true if a custom editor is available, false to use generic editor
 */
bool AvSynthAudioProcessor::hasEditor() const {
    return true; // (change this to false if you choose to not supply an editor)
}

/**
 * @brief Creates and returns the custom editor component
 * @return AudioProcessorEditor* Pointer to the newly created editor
 */
juce::AudioProcessorEditor *AvSynthAudioProcessor::createEditor() {
    return new AvSynthAudioProcessorEditor(*this);
    // return new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================

/**
 * @brief Save plugin state - called by the host when saving project or plugin preset
 *
 * This method stores all parameter values in a memory block that can be saved by the host.
 * The ValueTree system is used as an intermediate format for complex parameter data.
 *
 * @param destData Memory block where the plugin state will be stored
 */
void AvSynthAudioProcessor::getStateInformation(juce::MemoryBlock &destData) {
    // This method stores parameters in the memory block.
    // ValueTree is used as an intermediate to save complex parameter data
    // Parameters are written to a memory stream which is then stored in the destData block

    // Create output stream and write parameter state to it
    juce::MemoryOutputStream stream(destData, true);
    parameters.state.writeToStream(stream);
}

/**
 * @brief Restore plugin state from saved data
 *
 * This method restores all parameter values from a memory block that was previously
 * saved using getStateInformation().
 *
 * @param data Pointer to the saved state data
 * @param sizeInBytes Size of the data in bytes
 */
void AvSynthAudioProcessor::setStateInformation(const void *data, int sizeInBytes) {
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    auto tree = juce::ValueTree::readFromData(data, sizeInBytes);
    if (tree.isValid()) {
        parameters.replaceState(tree);
    }
}

/**
 * @brief Updates the angular delta for oscillator frequency
 *
 * Calculates the phase increment per sample based on the desired frequency
 * and current sample rate.
 *
 * @param frequency The desired oscillator frequency in Hz
 */
void AvSynthAudioProcessor::updateAngleDelta(float frequency) {
    auto cyclesPerSample = frequency / getSampleRate();
    angleDelta = cyclesPerSample * juce::MathConstants<double>::twoPi;
}

/**
 * @brief Generates a flute-like waveform using harmonic synthesis
 *
 * Creates a more complex waveform that simulates the harmonic content of a flute
 * by combining a fundamental frequency with characteristic overtones and breath modulation.
 *
 * @param angle The current phase angle for oscillation
 * @return float The computed flute waveform sample
 */
float AvSynthAudioProcessor::getFluteWaveform(double angle) {
    // Fundamental tone
    float fundamental = static_cast<float>(std::sin(angle));

    // Characteristic flute overtones (mainly odd harmonics)
    float harmonic2 = 0.3f * static_cast<float>(std::sin(2.0 * angle));  // Octave (weak)
    float harmonic3 = 0.15f * static_cast<float>(std::sin(3.0 * angle)); // Fifth
    float harmonic4 = 0.05f * static_cast<float>(std::sin(4.0 * angle)); // Double octave (very weak)
    float harmonic5 = 0.08f * static_cast<float>(std::sin(5.0 * angle)); // Major third above double octave

    // Light modulation for "breath" effect
    float breathModulation = 1.0f + 0.02f * static_cast<float>(std::sin(angle * 0.1));

    return (fundamental + harmonic2 + harmonic3 + harmonic4 + harmonic5) * breathModulation * 0.8f;
}

/**
 * @brief Generates oscillator samples based on the specified waveform type
 *
 * This function generates different types of periodic waveforms including sine, square,
 * sawtooth, triangle, and a custom flute-like waveform.
 *
 * @param type The type of oscillator waveform to generate
 * @param angle The current phase angle for the oscillation
 * @return float The computed sample value
 */
float AvSynthAudioProcessor::getOscSample(OscType type, double angle) {
    switch (type) {
    case OscType::Sine:
        return static_cast<float>(std::sin(angle));
    case OscType::Square:
        return (std::sin(angle) >= 0.0 ? 1.0f : -1.0f);
    case OscType::Saw:
        return static_cast<float>(
            2.0f * (angle / juce::MathConstants<double>::twoPi - std::floor(0.5 + angle / juce::MathConstants<double>::twoPi)));
    case OscType::Triangle:
        return 2.0f * static_cast<float>(std::abs(2.0f * (angle / juce::MathConstants<double>::twoPi -
                                                          std::floor(0.5 + angle / juce::MathConstants<double>::twoPi)))) -
               1.0f;
    case OscType::Flute:
        // Flute-like waveform with characteristic overtones
        return getFluteWaveform(angle);
    default:
        return 0.0f;
    }
}

/**
 * @brief Updates the high-pass filter coefficients
 *
 * Recalculates and applies new high-pass filter coefficients to both left and right
 * channel filter chains using a 4th-order Butterworth design.
 *
 * @param frequency The cutoff frequency for the high-pass filter in Hz
 */
void AvSynthAudioProcessor::updateHighPassCoefficients(float frequency) {
    auto highPassCoefficients =
        juce::dsp::FilterDesign<float>::designIIRHighpassHighOrderButterworthMethod(frequency, getSampleRate(), 4);

    auto &leftHighPass = leftChain.get<0>();
    *leftHighPass.get<0>().coefficients = *highPassCoefficients[0];
    *leftHighPass.get<1>().coefficients = *highPassCoefficients[1];

    auto &rightHighPass = rightChain.get<0>();
    *rightHighPass.get<0>().coefficients = *highPassCoefficients[0];
    *rightHighPass.get<1>().coefficients = *highPassCoefficients[1];
}

/**
 * @brief Updates the low-pass filter coefficients
 *
 * Recalculates and applies new low-pass filter coefficients to both left and right
 * channel filter chains using a 4th-order Butterworth design.
 *
 * @param frequency The cutoff frequency for the low-pass filter in Hz
 */
void AvSynthAudioProcessor::updateLowPassCoefficients(float frequency) {
    auto lowPassCoefficients =
        juce::dsp::FilterDesign<float>::designIIRLowpassHighOrderButterworthMethod(frequency, getSampleRate(), 4);

    auto &leftLowPass = leftChain.get<1>();
    *leftLowPass.get<0>().coefficients = *lowPassCoefficients[0];
    *leftLowPass.get<1>().coefficients = *lowPassCoefficients[1];

    auto &rightLowPass = rightChain.get<1>();
    *rightLowPass.get<0>().coefficients = *lowPassCoefficients[0];
    *rightLowPass.get<1>().coefficients = *lowPassCoefficients[1];
}

/**
 * @brief Updates the chorus effect parameters
 *
 * Applies new chorus effect parameters from the current chain settings to the
 * chorus effect processor.
 *
 * @param settings The current chain settings containing chorus parameters
 */
void AvSynthAudioProcessor::updateChorusParameters(const ChainSettings& settings) {
    chorus.setRate(settings.chorusRate);
    chorus.setDepth(settings.chorusDepth);
    chorus.setFeedback(settings.chorusFeedback);
    chorus.setMix(settings.chorusMix);
}

/**
 * @brief Updates the reverb effect parameters
 *
 * Applies new reverb parameters from the current chain settings to the reverb processor.
 * The freeze mode is kept at 0 for normal operation.
 *
 * @param settings The current chain settings containing reverb parameters
 */
void AvSynthAudioProcessor::updateReverbParameters(const ChainSettings& settings) {
    reverbParams.roomSize = settings.reverbRoomSize;
    reverbParams.damping = settings.reverbDamping;
    reverbParams.wetLevel = settings.reverbWetLevel;
    reverbParams.dryLevel = settings.reverbDryLevel;
    reverbParams.width = settings.reverbWidth;
    reverbParams.freezeMode = 0.0f; // Keep this at 0 for normal operation

    reverb.setParameters(reverbParams);
}

/**
 * @brief Template helper function to create typed parameters with magic_enum
 *
 * This template function creates audio parameters using magic_enum for type-safe
 * parameter identification, eliminating the need for string constants.
 *
 * @tparam ParamT The type of parameter to create (e.g., AudioParameterFloat)
 * @tparam Param The parameter enum value for identification
 * @tparam Args Variadic template for constructor arguments
 * @param args Arguments to forward to the parameter constructor
 * @return std::unique_ptr<ParamT> Unique pointer to the created parameter
 */
template <typename ParamT, AvSynthAudioProcessor::Parameters Param, typename... Args>
static std::unique_ptr<ParamT> makeParameter(Args &&...args) {
    return std::make_unique<ParamT>(magic_enum::enum_name<Param>().data(), magic_enum::enum_name<Param>().data(),
                                    std::forward<Args>(args)...);
}

/**
 * @brief Creates the parameter layout for the audio processor
 *
 * This function defines all the parameters that the plugin exposes to the host,
 * including their types, ranges, and default values. Parameters include:
 * - Basic synthesis: gain, frequency, oscillator type
 * - Filtering: high-pass and low-pass cutoff frequencies
 * - ADSR envelope parameters
 * - Reverb parameters
 * - Chorus parameters
 *
 * @return ParameterLayout The complete parameter layout for the processor
 */
juce::AudioProcessorValueTreeState::ParameterLayout AvSynthAudioProcessor::createParameterLayout() {
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add(makeParameter<juce::AudioParameterFloat, Parameters::Gain>(0.0f, 1.0f, 0.25f));

    layout.add(makeParameter<juce::AudioParameterFloat, Parameters::Frequency>(
        juce::NormalisableRange(20.0f, 20000.0f, 1.0f, 0.3f), 440.0f));

    layout.add(makeParameter<juce::AudioParameterFloat, Parameters::HighPassFreq>(
        juce::NormalisableRange(20.0f, 20000.0f, 1.0f, 0.3f), 20.0f));

    layout.add(makeParameter<juce::AudioParameterFloat, Parameters::LowPassFreq>(
        juce::NormalisableRange(20.0f, 20000.0f, 1.0f, 0.3f), 20000.0f));

    layout.add(makeParameter<juce::AudioParameterChoice, Parameters::OscType>(
        juce::StringArray{magic_enum::enum_name<OscType::Sine>().data(), magic_enum::enum_name<OscType::Saw>().data(),
                          magic_enum::enum_name<OscType::Square>().data(), magic_enum::enum_name<OscType::Triangle>().data()},
        0));

    // ADSR Parameters
    layout.add(makeParameter<juce::AudioParameterFloat, Parameters::Attack>(
        juce::NormalisableRange(0.001f, 5.0f, 0.001f, 0.3f), 0.1f));

    layout.add(makeParameter<juce::AudioParameterFloat, Parameters::Decay>(
        juce::NormalisableRange(0.001f, 5.0f, 0.001f, 0.3f), 0.1f));

    layout.add(makeParameter<juce::AudioParameterFloat, Parameters::Sustain>(
        juce::NormalisableRange(0.0f, 1.0f, 0.01f), 0.7f));

    layout.add(makeParameter<juce::AudioParameterFloat, Parameters::Release>(
        juce::NormalisableRange(0.001f, 5.0f, 0.001f, 0.3f), 0.3f));

    // Reverb Parameters
    layout.add(makeParameter<juce::AudioParameterFloat, Parameters::ReverbRoomSize>(
        juce::NormalisableRange(0.0f, 1.0f, 0.01f), 0.5f));

    layout.add(makeParameter<juce::AudioParameterFloat, Parameters::ReverbDamping>(
        juce::NormalisableRange(0.0f, 1.0f, 0.01f), 0.5f));

    layout.add(makeParameter<juce::AudioParameterFloat, Parameters::ReverbWetLevel>(
        juce::NormalisableRange(0.0f, 1.0f, 0.01f), 0.33f));

    layout.add(makeParameter<juce::AudioParameterFloat, Parameters::ReverbDryLevel>(
        juce::NormalisableRange(0.0f, 1.0f, 0.01f), 0.4f));

    layout.add(makeParameter<juce::AudioParameterFloat, Parameters::ReverbWidth>(
        juce::NormalisableRange(0.0f, 1.0f, 0.01f), 1.0f));

    // Chorus Parameters
    layout.add(makeParameter<juce::AudioParameterFloat, Parameters::ChorusRate>(
        juce::NormalisableRange(0.1f, 10.0f, 0.1f), 0.5f));

    layout.add(makeParameter<juce::AudioParameterFloat, Parameters::ChorusDepth>(
        juce::NormalisableRange(0.0f, 1.0f, 0.01f), 0.5f));

    layout.add(makeParameter<juce::AudioParameterFloat, Parameters::ChorusFeedback>(
        juce::NormalisableRange(0.0f, 0.95f, 0.01f), 0.3f));

    layout.add(makeParameter<juce::AudioParameterFloat, Parameters::ChorusMix>(
        juce::NormalisableRange(0.0f, 1.0f, 0.01f), 0.5f));

    return layout;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter() { return new AvSynthAudioProcessor(); }