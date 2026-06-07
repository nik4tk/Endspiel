# ♞ Endspiel 01

**Endspiel** (German for *"Endgame"*) is a moderatly fast (for now), custom-built, UCI-compatible chess engine written from scratch in modern C++.

Designed with a focus on bitwise optimization and aggressive tactical search, Endspiel utilizes 64-bit integers (Bitboards) to represent the board state, allowing for lightning-fast move generation and evaluation.

## Architecture

Endspiel's "brain" is built on the classic architecture of elite chess engines, split into three main components: **The Scout (Move Generation)**, **The Intuition (Evaluation)**, and **The Memory (Transposition Tables)**.

### Features
* **Board Representation:** 64-bit Bitboards for instantaneous board-state queries and piece masking.
* **Search Algorithm:** Fail-Soft Alpha-Beta Pruning.
* **Move Ordering:** * MVV-LVA (Most Valuable Victim - Least Valuable Attacker) to instantly find tactical captures.
    * Promotion prioritization.
* **Memory & Caching:** * Incremental Zobrist Hashing (64-bit unique board fingerprinting).
    * 16MB Transposition Table (TT) storing Exact, Upper Bound (Alpha), and Lower Bound (Beta) scores to prevent redundant calculations.
* **Evaluation:** Material counting paired with mirrored Piece-Square Tables (PSTs) to encourage central control, rapid development, and king safety.
* **Communication:** Fully supports the Universal Chess Interface (UCI) protocol.

## Building the Engine

Endspiel uses CMake for easy cross-platform compilation. To build the executable yourself:

1. Clone the repository:
   ```bash
   git clone https://github.com/nik4tk/Endspiel.git
   cd Endspiel
   
2. Generate the build files and compile:
   ```bash mkdir build
   cd build
   cmake ..
   cmake --build . --config Release
(Note: Always build in Release mode! Chess engines run significantly slower in Debug mode due to unoptimized search trees).

## How to Play Against Endspiel
Endspiel is a command-line program that communicates via the UCI protocol. To play against it, you need a Chess GUI.
1. Download a free UCI-compatible GUI like Arena Chess GUI or CuteChess
2. Go to engines > install engine (or equivalent in your GUI)
3. Select the compiled Endspiel.exe
4. Start a new game (good luck ;D)

## For the Future

Endspiel 01 is just the beginning. While the current version relies on classic heuristics and "Expert Human-Tuning" for its strategic intuition, the ultimate goal is to transition Endspiel into a modern, data-driven engine.

**Actively in Development:**
* **Quiescence Search & Null Move Pruning:** Advanced search techniques to eliminate the "horizon effect" and drastically increase calculation depth.
* **Automated Texel Tuning (ML):** Implementing a machine-learning script to autonomously evaluate millions of positions from the **Lichess Open Database (`database.lichess.org`)**. Instead of relying on human guesses, Endspiel will use gradient descent optimization to mathematically "learn" the true value of pieces and positional bonuses by minimizing its evaluation error against real-world data.

*Stay tuned as Endspiel evolves from a hard-coded calculator into a self-tuning machine!*

## Fun Facts :D
Endspiel originally started its life under the name "Zwischenzug".
During development, chess GUIs kept confusing the engine with an early 2000s program also named "Zwischenzug" by Manuel Diaz, forcibly assigning it a Spanish flag. 
After a quick rebranding, Endspiel 01 finally broke free from Manuel's ghost and now proudly flies the German flag!
