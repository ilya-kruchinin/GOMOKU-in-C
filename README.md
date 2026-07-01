# GOMOKU-in-C
Traditional japanese board game in C
GOMOKU 15x15 (五目並べ)
A classic 15x15 Gomoku (Five in a Row) game built for MS-DOS, featuring authentic retro EGA/VGA graphics, a custom bitmap Japanese title screen, and a highly progressive AI system.
Originally developed for Turbo C 2.0/3.0, this game takes you on a journey from local school beginners to mythical world legends, and eventually into an infinite elite AI gauntlet.
✨ Features
Authentic DOS Graphics: Runs in 640x480 VGA mode with a custom-drawn wooden board, star points, and smooth cursor tracking.
Custom Bitmap Typography: Features a hand-crafted 12x12 pixel bitmap font for the Japanese title (五目並べ / ごもくならべ).
Multiple Game Modes:
1. Player vs Player (PvP): Local hot-seat multiplayer.
2. Player vs Computer (PvC): Face off against a progressively scaling AI.
Progressive AI Difficulty:
Levels 1-5: Distinct rival groups ranging from School Beginners to World Legends.
Level 6+ (Infinite Elite Mode): Unlocks after defeating the Level 5 "Gods of Go", featuring hyper-aggressive AI with extreme center-control prioritization.
Advanced AI Logic:
5-cell window pattern recognition (detects threats and gaps).
Center-board control weighting (scales with difficulty).
1-Ply Lookahead (Level 3+): AI predicts and penalizes moves that give the human player an easy winning response.
Rival System & Rematches: Each level features unique randomized rival names. After a match, you can choose to Rematch the same rival or Continue to the next tier.
🕹️ Controls
Main Menu
1 : Start Player vs Player
2 : Start Player vs Computer
ESC : Quit Game
In-Game
Arrow Keys : Move the selection cursor
Enter / Space : Place a stone
ESC : Return to Main Menu
Post-Match Screen
R : Rematch (Play the exact same rival again)
C : Continue (Advance to the next AI level and face a new rival. Note: You must win to advance!)
M : Return to Main Menu
🧠 AI Rival Groups
Level
Group Name
Example Rivals
1
School Beginners
Novice Ken, Rookie Aoi, Beginner Jin...
2
Club Amateurs
Student Haruto, Amateur Mei, Scholar Naoki...
3
Regional Experts
Expert Hiroshi, Master Yuki, Tactician Emi...
4
National Masters
Pro Takumi, Grandmaster Ryu, Sensei Hideo...
5
World Legends
Legend Akira, Mythical Yuto, God of Go Shota...
6+
Infinite Elite
Cyber Alpha, Neural Deep, Quantum Ren... (Endless Scaling)
🛠️ Compilation & Requirements
Requirements
OS: MS-DOS, FreeDOS, or a DOS emulator (like DOSBox).
Compiler: Borland Turbo C 2.0 or 3.0.
Hardware/Emulation: EGA/VGA graphics support.
How to Compile (Turbo C)
Open GOMOKU.C in the Turbo C IDE.
Ensure your BGI path is configured. The code attempts to register the embedded EGAVGA driver via registerbgidriver(EGAVGA_driver);.
Note: If you get a linker error regarding EGAVGA_driver, you may need to run the BGIOBJ utility on your EGAVGA.BGI file to generate EGAVGA.OBJ, and add EGAVGA.OBJ to your project/linker list.
Compile and build the executable (ALT+F9 to compile, F9 to make).
Run GOMOKU.EXE.
Running in DOSBox
If you are playing on a modern system via DOSBox:
Mount your compiled game directory: mount c C:\GOMOKU
Switch to the C drive: C:
Run the game: GOMOKU.EXE
📜 Credits & Acknowledgments
Game Design & Programming: I. Kruchinin
Publisher / Group: KIPSoft
Engine: Custom C / BGI Graphics Library
