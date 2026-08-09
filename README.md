# Cels Viewer

A lightweight terminal-based IDE for FLECS ECS.

## 📂 Project Structure

```text
cels-viewer/
├── README.md             # Project overview and structure guide
├── CMakeLists.txt        # Build configuration
├── include/              # Public headers
│   ├── app.h             # App lifecycle and state
│   └── tui.h             # TUI primitives
├── src/                  # Implementation
│   ├── main.c            # Entry point
│   ├── app.c             # App logic implementation
│   └── tui/              # TUI specific implementation
│       └── tui.c
├── tests/                # Testing Hierarchy
│   ├── cli/              # Hardness testing and quick-add tests
│   ├── benchmarks/       # Standalone performance benchmarks
│   └── features/         # Feature-specific tests, benchmarks, and CLI tools
├── build/                # Binary Outputs
│   ├── debug/            # Debug binaries
│   └── release/          # Release binaries
```

## 🛠 Build Instructions

The project uses CMake. Binaries are output to `build/debug` and `build/release`.

```bash
mkdir build && cd build
cmake ..
make
```
