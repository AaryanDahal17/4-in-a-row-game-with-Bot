# 🧠 Connect Four with AI Bot (MCTS) — C++ & SDL2

This is a **Connect-four** game built using **C++ and SDL2**, featuring an AI bot opponent powered by **Monte Carlo Tree Search (MCTS)**.

---

## 🎮 Features

- Classic Connect Four gameplay
- AI opponent using Monte Carlo Tree Search
- Built with SDL2 for graphics and input handling
- Cross-platform (Windows, Linux, macOS)

---

## 📁 Project Structure

```
.
├── src/
│   ├── Main.cpp         # Entry point
│   └── bot.cpp          # MCTS bot logic
├── run.bat              # (Optional) Windows run script
├── .gitignore
└── README.md
```

---

## 🛠️ Requirements

- C++17 or later
- SDL2
- SDL2\_image

---

## 🧰 Installation Instructions

### ✅ Windows (with [vcpkg](https://github.com/microsoft/vcpkg))

1. **Install vcpkg**:

```bash
git clone https://github.com/microsoft/vcpkg
cd vcpkg
bootstrap-vcpkg.bat
```

2. **Install dependencies**:

```bash
vcpkg install sdl2 sdl2-image
```

3. **Compile using g++ or CMake**. Example with g++:

```bash
g++ src/Main.cpp src/bot.cpp -I[path_to_vcpkg]/installed/x64-windows/include `
 -L[path_to_vcpkg]/installed/x64-windows/lib `
 -lSDL2 -lSDL2_image -o connect4
```

> Replace `[path_to_vcpkg]` with your actual vcpkg directory.

---

### ✅ Linux (Debian/Ubuntu)

```bash
sudo apt update
sudo apt install build-essential libsdl2-dev libsdl2-image-dev
g++ src/Main.cpp src/bot.cpp -lSDL2 -lSDL2_image -o connect4
```

---

### ✅ macOS (with Homebrew)

```bash
brew install sdl2 sdl2_image
g++ src/Main.cpp src/bot.cpp -lSDL2 -lSDL2_image -o connect4
```

## 🚀 Running the Game

After compilation:

```bash
./connect4
```

Or on Windows:

```cmd
connect4.exe
```

---

Enjoy!
