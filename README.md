# OrionSort

A C++ sorting algorithm visualizer built with SDL3 and ImGui. The application animates different sorting techniques with optional audio feedback and flexible display modes.

## Features

- Multiple algorithms: Bubble, Quick, Comb, Merge, Insertion, Selection, Radix LSD, Pigeon Hole, Gravity and Bogo sort.
- Several visual styles including bars, dots, circles and spirals.
- Adjustable array length and sorting speed.
- Cross-platform build via CMake.
- Debug Window for tracking variables

## Prerequisites

This project requires a **C++20** capable compiler and **CMake 3.28** or newer.
All third‑party libraries (SDL3, SDL_ttf and OpenAL) are included as git
submodules, so ensure they are cloned with:

```bash
git clone --recursive <repo-url>
# or, if already cloned
git submodule update --init --recursive
```


### **Windows**: 
Install **MSYS2** with either the `mingw64-toolchain` or `ucrt64-toolchain` environment (or use Visual Studio/MSVC).
  
### **Linux**
Install `build-essential` and `cmake`:

* **Ubuntu/Debian-based** — `sudo apt update && sudo apt install build-essential cmake`
* **Fedora/Red Hat-based** — `sudo dnf install gcc gcc-c++ make cmake`
* **Arch/Manjaro-based** — `sudo pacman -S base-devel cmake ninja`
* **openSUSE** — `sudo zypper install gcc gcc-c++ make cmake`

### **MacOS**
Use **Brew**: `brew install cmake`.


## Building
```bash
cmake -S . -B build
cmake --build build --target install
```

The resulting binaries are placed in the `dist/` directory.

## Running

Run the generated `Sorting` executable. The GUI lets you select algorithms, tweak parameters and view real-time statistics.

> **Note for macOS users:**  
> If you download the app without an Apple-signed build, macOS may quarantine it and show a **"Damaged"** warning.  
> To allow it to run, open **Terminal** and run:
> ```bash
> sudo xattr -dr com.apple.quarantine /Applications/OrionSort.app
> ```
> This removes the quarantine flag so the app can launch normally.


## Tests (_Temporarily Unavailable_)

Unit tests are provided with GoogleTest. From the build directory, execute:

```bash
ctest --output-on-failure
```

## License

This project is licensed under the terms of the GNU GPLv3. See the [LICENSE](LICENSE) file for details.

## Todo

- [x] Full multithreading with thread safety
- [x] FPS stabilization
- [x] Finishing sort animation
- [x] Real-time display along with apparent time.
- [x] Fix obscure big involving freezing when starting a sort
- [x] Streamline counting accesses, comparisions, swaps, etc...
- [x] Improve options and selection screen
- [x] Sort registry for easier addition of new algorithms
- [ ] Shorten GCC compile times (Linux & Windows) by atleast 25%
- [ ] At least 25 sorting algorithms
- [ ] Better audio output - less crackly, more satisfying
- [ ] Fully high DPI compliant
- [x] CPack Integration
