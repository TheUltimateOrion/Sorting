# Sorting

A C++ sorting algorithm visualizer built with SDL3 and ImGui. The application animates different sorting techniques with optional audio feedback and flexible display modes.

## Features

- Multiple algorithms: Bubble, Quick, Comb, Merge, Insertion, Selection, Radix LSD, Pigeon Hole, Gravity and Bogo sort.
- Several visual styles including bars, dots, circles and spirals.
- Adjustable array length and sorting speed.
- Cross-platform build via CMake.

## Building

```bash
cmake -S . -B build
cmake --build build --target install
```

The resulting binaries are placed in the `dist/` directory.

## Running

Run the generated `Sorting` executable. The GUI lets you select algorithms, tweak parameters and view real-time statistics.

## Tests

Unit tests are provided with GoogleTest. From the build directory, execute:

```bash
ctest --output-on-failure
```

## License

This project is licensed under the terms of the GNU GPLv3. See the [LICENSE](LICENSE) file for details.
