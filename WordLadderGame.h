#pragma once
#include <string>
#include <vector>
#include "Graph.h"

/**
 * @class WordLadderGame
 * @brief Handles the core logic and backend operations for the Word Ladder game.
 * * This class is responsible for loading the dictionary, building the graph of words,
 * generating valid game states, and running the Breadth-First Search (BFS) algorithm
 * to find optimal paths.
 */
class WordLadderGame {
private:
    /**
     * @brief Graph data structure storing the words as nodes.
     */
    Graph<std::string> wordGraph;

    /**
     * @brief Vector storing all loaded valid dictionary words.
     */
    std::vector<std::string> dictionary;
public:
    /**
     * @brief Default constructor for WordLadderGame.
     */
    WordLadderGame() = default;

    /**
     * @brief Loads words from a specified text file into the dictionary.
     * @param filename The absolute or relative path to the dictionary file.
     */
    void loadDictionary(const std::string& filename);

    /**
     * @brief Builds the word graph using a wildcard pattern matching algorithm.
     * Connects words that differ by exactly one letter.
     */
    void buildGraph();

    /**
     * @brief Prints the current state of the word graph to the standard output.
     * Mainly used for debugging purposes.
     */
    void printGameGraph() const;

    /**
     * @brief Finds the shortest path between two words using the Breadth-First Search (BFS) algorithm.
     * @param start The starting word.
     * @param target The target word.
     * @return A vector of strings representing the path from start to target. Returns an empty vector if no path exists.
     */
    std::vector<std::string> findShortestPath(const std::string& start, const std::string& target);

    /**
     * @brief Randomly generates a valid game configuration with a guaranteed solution.
     * @param length The required length of the words.
     * @param outStart Reference to store the generated starting word.
     * @param outTarget Reference to store the generated target word.
     * @return True if a valid pair was found, False otherwise.
     */
    bool generateValidGame(int length, std::string& outStart, std::string& outTarget);

    /**
     * @brief Checks if a given word exists in the loaded dictionary.
     * @param word The word to validate.
     * @return True if the word is found, False otherwise.
     */
    bool isValidWord(const std::string& word) const;
};
