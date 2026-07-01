# GOMOKU 15x15 (MS-DOS)

A classic 15x15 Gomoku (Five in a Row) game written in C for MS-DOS. This project features a retro BGI graphics interface, a custom Japanese bitmap font title screen, and a highly scalable, progressive AI system that adapts to your skill level.

![DOS](https://img.shields.io/badge/Platform-MS--DOS-blue)
![Language](https://img.shields.io/badge/Language-C-blue)
![Graphics](https://img.shields.io/badge/Graphics-BGI%20(EGA/VGA)-green)

## 🌟 Features

- **Classic Gameplay**: Traditional 15x15 board with star points (hoshi).
- **Game Modes**: 
  - 👥 **Player vs Player (PvP)**
  - 🤖 **Player vs Computer (PvC)**
- **Progressive AI System**: 
  - Face off against uniquely named rivals across 6 distinct difficulty tiers (from *School Beginners* to *World Legends*).
  - **Infinite Elite Mode**: Unlocks after Level 5, featuring cybernetic/AI-themed grandmasters with extreme center-control and aggression.
- **Advanced AI Logic**: 
  - 5-cell window scan for pattern recognition (handles gaps and open/closed ends).
  - Center-board prioritization.
  - 1-ply lookahead (predicts opponent's best response) at higher levels.
- **Retro DOS Graphics**: Authentic EGA/VGA 640x480 graphics using the Borland Graphics Interface (BGI).
- **Custom Bitmap Fonts**: Hand-crafted 12x12 pixel Japanese Kanji (`五目並べ`) and Hiragana (`ごもくならべ`) title screens.

## 🧠 AI Difficulty & Rivals

The AI scales progressively. Defeat a rival to advance to the next tier:

| Level | Tier | Example Rivals | AI Behavior |
| :--- | :--- | :--- | :--- |
| **1** | School Beginners | Novice Ken, Rookie Aoi | Basic pattern matching. |
| **2** | Club Amateurs | Student Haruto, Amateur Mei | Adds center-board control. |
| **3** | Regional Experts | Expert Hiroshi, Master Yuki | 1-ply lookahead (blocks traps). |
| **4** | National Masters | Pro Takumi, Grandmaster Ryu | Aggressive offense + defense. |
| **5** | World Legends | Legend Akira, Mythical Yuto | Near-perfect tactical play. |
| **6+** | **Infinite Elite** | Cyber Alpha, Neural Deep | Maximum aggression, extreme center priority. |

## 🎹 Controls

### In-Game
- **Arrow Keys**: Move the cursor.
- **Enter** or **Space**: Place a stone.
- **ESC**: Return to Main Menu.

### Post-Match
- **R**: **Rematch** (Play the exact same rival again).
- **C**: **Continue** (Advance to the next level/rival if you won).
- **M**: **Main Menu**.

## 🛠️ Building & Running

Because this is an MS-DOS application utilizing the BGI graphics library, it requires a DOS environment to compile and run.

### Prerequisites
- **Turbo C 2.0 / 3.0** (or Borland C++).
- **DOSBox** (Recommended for running on modern Windows/Linux/macOS).
- **EGAVGA.BGI** driver (Required for graphics initialization).

### Compilation Steps (Turbo C)
1. Open `GOMOKU.C` in the Turbo C IDE.
2. Ensure the BGI directory is configured in `Options -> Directories`.
3. The code attempts to register the embedded `EGAVGA_driver`. If you encounter graphics initialization errors, ensure `EGAVGA.BGI` is in the same directory as your compiled `.EXE`, or use the `BGIOBJ` utility to link the driver directly into the executable.
4. Compile and run.

### Running on Modern Systems
1. Install [DOSBox](https://www.dosbox.com/).
2. Mount the directory containing your compiled `GOMOKU.EXE` and `EGAVGA.BGI` in DOSBox.
3. Run the executable:
   ```dos
   mount c C:\path\to\your\folder
   c:
   GOMOKU.EXE

### Credits
- Game Design & Programming: I. Kruchinin
- Group: KIPSoft
- Engine: Turbo C / Borland BGI
