# Word Ladder Game 🪜

A modern, interactive C++ implementation of the classic Word Ladder puzzle, featuring a full Graphical User Interface (GUI) built with Qt. 

## 📸 Gameplay Preview
<img width="596" height="475" alt="Screenshot 2026-05-26 at 21 23 35" src="https://github.com/user-attachments/assets/80f87635-ca9e-4478-bb55-4e2bf89d5190" />
<img width="598" height="478" alt="Screenshot 2026-05-26 at 21 23 13" src="https://github.com/user-attachments/assets/3b0a09de-f2aa-4d2c-9df8-1abd8de9b563" />
<img width="598" height="487" alt="Screenshot 2026-05-26 at 21 22 54" src="https://github.com/user-attachments/assets/f56195ca-a603-4bd6-a834-75bbcd8ffd4b" />
<img width="600" height="485" alt="Screenshot 2026-05-26 at 21 22 35" src="https://github.com/user-attachments/assets/e9a9445c-5d1f-42d5-9a59-42888c7e599b" />
<img width="599" height="484" alt="Screenshot 2026-05-26 at 21 22 16" src="https://github.com/user-attachments/assets/1ed42966-54cb-44b5-a127-7b0f3f62459a" />

## 🎮 Game Modes
* **Automatic Mode:** Instantly finds the optimal, shortest path between a starting word and a target word using the Breadth-First Search (BFS) algorithm.
* **Playing Mode:** Challenges the user to solve the ladder step-by-step. It includes strict real-time move validation and an intelligent, color-coded hint system.
* **Analytics Mode:** Tracks player performance across multiple sessions. It reads and parses CSV files to display personalized statistics, such as total games played and the number of unique words used.

## 🛠️ Technologies & Data Structures
* **Language:** C++
* **GUI Framework:** Qt 
* **Core Algorithm:** Breadth-First Search (BFS) for optimal pathfinding and graph traversal.
* **Custom Structures:** A generic Graph template implemented from scratch using `std::map` and `std::set` for $O(1)$/$O(\log n)$ neighbor lookups.
* **Data Management:** File I/O operations and CSV parsing for persistent gameplay history.

## 📚 Code Documentation
The entire source code is fully documented following professional Doxygen standards. 

👉 **[Click Here to view the full HTML Doxygen Documentation](https://amaliatanase.github.io/WordLadder/html/index.html)** 
