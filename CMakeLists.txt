# Require minimum CMake version 3.30
cmake_minimum_required(VERSION 3.30)

# Define the project name
project(AVSYNTH VERSION 0.1.0)

# Set C++23 as the required standard for all targets
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Use fetch content to download dependencies
include(FetchContent)

# Download magic_enum
FetchContent_Declare(
        magic_enum
        GIT_REPOSITORY https://github.com/Neargye/magic_enum.git
        GIT_TAG v0.9.7
)
FetchContent_MakeAvailable(magic_enum)

# Download JUCE
FetchContent_Declare(
        JUCE
        GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
        GIT_TAG        8.0.8
)
FetchContent_MakeAvailable(JUCE)

# Create the plugin target FIRST
juce_add_plugin(AvSynth
        # VERSION ...                               # Set this if the plugin version is different to the project version
        # ICON_BIG ...                              # ICON_* arguments specify a path to an image file to use as an icon for the Standalone
        # ICON_SMALL ...
        COMPANY_NAME "HSBI"                         # Specify the name of the plugin's author
        IS_SYNTH TRUE                               # Is this a synth or an effect?
        NEEDS_MIDI_INPUT TRUE                       # Does the plugin need midi input?
        NEEDS_MIDI_OUTPUT FALSE                     # Does the plugin need midi output?
        IS_MIDI_EFFECT FALSE                        # Is this plugin a MIDI effect?
        EDITOR_WANTS_KEYBOARD_FOCUS FALSE           # Does the editor need keyboard focus?
        COPY_PLUGIN_AFTER_BUILD TRUE                # Should the plugin be installed to a default location after building?
        PLUGIN_MANUFACTURER_CODE Hsbi               # A four-character manufacturer id with at least one upper-case character
        PLUGIN_CODE Avsy                            # A unique four-character plugin id with exactly one upper-case character
        # GarageBand 10.3 requires the first letter to be upper-case, and the remaining letters to be lower-case
        FORMATS AU VST3 Standalone                  # The formats to build. Other valid formats are: AAX Unity VST AU AUv3
        PRODUCT_NAME "Audio Visual Synthesizer")    # The name of the final executable, which can differ from the target name


target_include_directories(AvSynth PRIVATE ${magic_enum_SOURCE_DIR}/include)

# Generate JUCE header
juce_generate_juce_header(AvSynth)

# Add source files
target_sources(AvSynth
        PRIVATE
        src/PluginEditor.cpp
        src/PluginProcessor.cpp
        src/WaveformComponent.cpp
        src/Utils.cpp
        src/ADSRComponent.cpp
        src/ReverbComponent.cpp
        src/SpectrumComponent.cpp)

# Set compile definitions
target_compile_definitions(AvSynth
        PUBLIC
        # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
        JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_plugin` call
        JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_plugin` call
        JUCE_VST3_CAN_REPLACE_VST2=0
        DONT_SET_USING_JUCE_NAMESPACE
)

# Link libraries (including magic_enum)
target_link_libraries(AvSynth
        PRIVATE
        # AudioPluginData           # If we'd created a binary data target, we'd link to it here
        juce::juce_audio_utils
        juce::juce_dsp
        magic_enum::magic_enum
        PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)