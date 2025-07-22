# Sorting

A C++ sorting algorithm visualizer built with SDL3 and ImGui. The application animates different sorting techniques with optional audio feedback and flexible display modes.

## Features

- Multiple algorithms: Bubble, Quick, Comb, Merge, Insertion, Selection, Radix LSD, Pigeon Hole, Gravity and Bogo sort.
- Several visual styles including bars, dots, circles and spirals.
- Adjustable array length and sorting speed.
- Cross-platform build via CMake.

## Building

### Prerequisites

This project requires a **C++20** capable compiler and **CMake 3.28** or newer.
All third‑party libraries (SDL3, SDL_ttf and OpenAL) are included as git
submodules, so ensure they are cloned with:

```bash
git clone --recursive <repo-url>
# or, if already cloned
git submodule update --init --recursive
```

Platform specific setup:

- **Windows**: Install **MSYS2** with either the `mingw64-toolchain` or `ucrt64-toolchain` environment (or use Visual Studio/MSVC). The required dependencies are already provided by the submodules.
  
- **Linux**: Install packages such as `build-essential`, `cmake`, and `ninja-build`.
  
- **macOS**: Use Xcode-Build-Tools or install the tools with Homebrew: `brew install cmake ninja`.

```bash
cmake -S . -B build
cmake --build build --target install
```

The resulting binaries are placed in the `dist/` directory.

## Running

Run the generated `Sorting` executable. The GUI lets you select algorithms, tweak parameters and view real-time statistics.

**Note**: On **MacOS**, you might need to provide _permissions_ in the **_System Preferences_** in order for the app to run.

## Tests

Unit tests are provided with GoogleTest. From the build directory, execute:

```bash
ctest --output-on-failure
```

## License

This project is licensed under the terms of the GNU GPLv3. See the [LICENSE](LICENSE) file for details.

## Todo

- [x] Full multithreading with thread safety
- [x] FPS Stabilization
- [ ] Finishing Animation
- [ ] Fix obscure big involving freezing when starting a sort
- [ ] Improve options and selection screen
- [ ] Sort registry for easier addition of new algorithms
- [ ] Shorten GCC compile times (Linux & Windows) by atleast 25%
- [ ] At least 25 sorting algorithms
- [ ] Better audio output - less crackly, more satisfying
- [ ] Other minor fixes
