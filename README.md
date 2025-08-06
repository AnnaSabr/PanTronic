# PanTronic – Synthesizer Plugin & Standalone

## Project Overview

**PanTronic** is our very first attempt at creating a digital synthesizer. It is built using modern C++ and the JUCE framework.  
PanTronic is available both as a **VST3 plugin** and a **standalone application**, making it suitable for use in DAWs or as a separate audio tool.

---

## Build System

This project uses **CMake** as the build system. Follow the steps below to build PanTronic:

### 🛠️ Build Instructions

1. Clone the repository:
   ```bash
   git clone https://github.com/AnnaSabr/PanTronic.git
   ```

2. Navigate to the project root directory:
   ```bash
   cd PanTronic
   ```

3. Create a build directory and enter it:
   ```bash
   mkdir build && cd build
   ```

4. Configure the project using CMake:
   ```bash
   cmake ..
   ```

5. Build the project:
   ```bash
   cmake --build .
   ```

> 💡 Make sure CMake and a supported compiler are installed on your system.

---

## Dependencies

PanTronic depends on the following:

- **JUCE Framework** – For audio processing, GUI, and plugin handling.
- **magic_enum** – For modern C++ enum reflection.
- Other common libraries handled via CMake or system dependencies.

All dependencies are managed via CMake. You do not need to install JUCE globally – the project expects it to be accessible during configuration (e.g., via submodules or FetchContent).

---

## License

Please refer to the `LICENSE` file in the project root for licensing information.

---

## Authors

PanTronic is developed by the PanTronic Team as a learning and creative exploration of synthesizer design.
