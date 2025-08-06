# Require minimum CMake version 3.30
cmake_minimum_required(VERSION 3.30)

# Define the project name
project(PanTronic VERSION 0.1.0)

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
        GIT_TAG 8.0.8
)
FetchContent_MakeAvailable(JUCE)

# Create the plugin target
juce_add_plugin(PanTronic
        COMPANY_NAME "HSBI"
        IS_SYNTH TRUE
        NEEDS_MIDI_INPUT TRUE
        NEEDS_MIDI_OUTPUT FALSE
        IS_MIDI_EFFECT FALSE
        EDITOR_WANTS_KEYBOARD_FOCUS FALSE
        COPY_PLUGIN_AFTER_BUILD TRUE
        PLUGIN_MANUFACTURER_CODE Hsbi
        PLUGIN_CODE PnTr  # Unverwechselbarer 4-Zeichen-Code mit genau einem Großbuchstaben
        FORMATS AU VST3 Standalone
        PRODUCT_NAME "PanTronic"
)

juce_add_binary_data(PanTronic_BinaryData
        SOURCES
        Resources/Pan.jpg
)

target_include_directories(PanTronic PRIVATE ${magic_enum_SOURCE_DIR}/include)

# Generate JUCE header
juce_generate_juce_header(PanTronic)

# Add source files
target_sources(PanTronic
        PRIVATE
        src/PluginEditor.cpp
        src/PluginProcessor.cpp
        src/WaveformComponent.cpp
        src/Utils.cpp
        src/ADSRComponent.cpp
        src/ReverbComponent.cpp
        src/SpectrumComponent.cpp
        src/ChorusComponent.cpp
        src/ChorusEffect.cpp
        src/MysticalLookAndFeel.cpp
)

# Set compile definitions
target_compile_definitions(PanTronic
        PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
        DONT_SET_USING_JUCE_NAMESPACE
)

# Link libraries
target_link_libraries(PanTronic
        PRIVATE
        juce::juce_audio_utils
        juce::juce_dsp
        magic_enum::magic_enum
        PanTronic_BinaryData
        PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)
