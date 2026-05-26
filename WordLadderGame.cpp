/**
 * @file WordLadderGame.cpp
 * @brief Implementation of the WordLadderGame class.
 * * This file contains the implementations of the methods responsible for
 * game logic, including loading the dictionary, building the word graph,
 * pathfinding using BFS, and validating user moves.
 */
#include "WordLadderGame.h"
#include "Graph.h"
#include <queue>
#include <set>
#include <map>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>

/**
 * @brief Reads words from a text file and stores them in the dictionary vector.
 * @param filename The path to the text file containing the words.
 */
//read words from file and keep them in a vector
void WordLadderGame::loadDictionary(const std::string &filename) {
    std::ifstream file(filename);
    std::string word;
    if (!file.is_open()) {
        std::cerr << "Error: Invalid File " << filename << std::endl;
        return;
    }
    //read word by word until the end of the file
    while (file >> word) {
        dictionary.push_back(word);
    }
    file.close();
}

/**
 * @brief Constructs the graph of words.
 * * Words are connected if they differ by exactly one letter. This is achieved
 * efficiently by generating wildcard patterns (e.g., "C*T" for "CAT") and grouping
 * matching words together.
 */
//build the graph using wildcards
void WordLadderGame::buildGraph() {
    std::map<std::string, std::vector<std::string>> patternMap;
    //generate patterns for each word in the map
    for (const std::string& word : dictionary) {
        for (size_t i = 0; i < word.length(); ++i) {
            std::string pattern = word;
            pattern[i] = '*';
            //add the word in the patterns map
            patternMap[pattern].push_back(word);
        }
    }
    //connect words with the same pattern
    for (const auto& pair : patternMap) {
        //creat a vector of strings with the values in the initial map
        const std::vector<std::string>& bucket = pair.second;
        //iterate through the bucket and add edges between all pairs of words
        for (size_t i = 0; i < bucket.size(); ++i)
            for (size_t j = i + 1; j < bucket.size(); ++j)
                wordGraph.addEdge(bucket[i], bucket[j]);
    }
}

/**
 * @brief Outputs the adjacency list of the game's graph to the console.
 */
void WordLadderGame::printGameGraph() const {
    wordGraph.printGraph();
}

/**
 * @brief Finds the shortest path between two words using the Breadth-First Search (BFS) algorithm.
 * @param start The starting word.
 * @param target The target word to reach.
 * @return A vector of strings representing the sequence of words in the shortest path.
 * Returns an empty vector if no path exists or if the words are invalid.
 */
std::vector<std::string> WordLadderGame::findShortestPath(const std::string &start, const std::string &target) {
    if (wordGraph.getNeighbors(start).empty()) {
        std::cout << "Starting node does not exist" << std::endl;
        return {};
    }
    if (wordGraph.getNeighbors(target).empty()) {
        std::cout << "Target node does not exist" << std::endl;
        return {};
    }
    std::queue<std::string> queue; //for BFS
    std::map<std::string, std::string> parent; //previous
    std::set<std::string> visited;
    queue.push(start);
    visited.insert(start);
    parent[start] = "";
    bool found = false;
    while (!queue.empty()) {
        std::string current = queue.front();
        queue.pop();
        if (current == target) {
            found = true;
            break;
        }
        std::set<std::string> neighbors = wordGraph.getNeighbors(current);
        //iterate through the neighbors
        for (const std::string& neighbor : neighbors) {
            // if the neighbors has not been visisted yet
            if (visited.find(neighbor) == visited.end()) {
                visited.insert(neighbor);
                parent[neighbor] = current; // set the current node to be the parent of the neighbors
                queue.push(neighbor); // add it in the queue to explore its neighbors later
            }
        }
    }
    //reconstructing the path
    std::vector<std::string> path;
    if (!found) return path;
    std::string current = target;
    while (current != "") {
        path.push_back(current);
        current = parent[current];
    }
    std::reverse(path.begin(), path.end());
    return path;
}

/**
 * @brief Generates a valid starting and target word of the specified length that are guaranteed to have a solution path.
 * @param length The required string length for the game words.
 * @param outStart Reference variable where the chosen starting word will be stored.
 * @param outTarget Reference variable where the chosen target word will be stored.
 * @return True if a valid pair was successfully generated, false otherwise.
 */
bool WordLadderGame::generateValidGame(int length, std::string& outStart, std::string& outTarget) {
    std::vector<std::string> validWords;
    // filter only the words of needed length in the dictionary
    for (const std::string& w : dictionary)
        if (w.length() == length)
            validWords.push_back(w);

    if (validWords.size() < 2) return false;

    srand(time(0));

    // pick a random starting word
    outStart = validWords[rand() % validWords.size()];

    // search for a target that has a path from start
    // search this word for at most 100 times
    for (int i = 0; i < 100; ++i) {
        std::string tempTarget = validWords[rand() % validWords.size()];
        // if it is not the start node and it can find a path, then the game is valid
        if (tempTarget != outStart && !findShortestPath(outStart, tempTarget).empty()) {
            outTarget = tempTarget;
            return true;
        }
    }
    return false; // no valid pair has been found
}

/**
 * @brief Verifies if the provided word exists within the loaded dictionary.
 * @param word The word to check.
 * @return True if the word is in the dictionary, false otherwise.
 */
bool WordLadderGame::isValidWord(const std::string& word) const {
    // check if the word given by the user exists in the dictionary
    return std::find(dictionary.begin(), dictionary.end(), word) != dictionary.end();
}
