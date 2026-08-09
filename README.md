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

The project uses CMake presets. Application binaries (`cels_viewer`, `test_app`)
are output to `build/debug` or `build/release`; all other build artifacts
(CMake files, object files, vendored libraries) go to `out/debug` or `out/release`.

```bash
cmake --preset debug          # or: release
cmake --build --preset debug  # or: release
./build/debug/cels_viewer
```
