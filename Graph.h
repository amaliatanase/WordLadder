#pragma once
#include <iostream>
#include <map>
#include <set>

/**
 * @class Graph
 * @brief A generic template class representing an undirected graph.
 * * @tparam T The data type of the nodes stored in the graph (e.g., std::string).
 * Uses an adjacency list implemented with a std::map and std::set for efficient neighbor lookups.
 */
template <typename T>
class Graph {
private:
    /**
     * @brief Adjacency list storing nodes and their neighbors.
     */
    //each key in the map is a node of type T
    // the values is a set with its neighbors
    std::map<T, std::set<T>> adjList;
public:
    /**
     * @brief Default constructor.
     */
    Graph();

    /**
     * @brief Adds an isolated node to the graph.
     * @param node The node to be added.
     */
    void addNode(const T& node);
    /**
     * @brief Creates an undirected edge between two nodes.
     * If the nodes do not exist, they are automatically added to the graph.
     * @param u The first node.
     * @param v The second node.
     */
    void addEdge(const T& u, const T& v);

    /**
     * @brief Retrieves all neighbors of a specific node.
     * @param node The node whose neighbors are to be found.
     * @return A set containing all neighbor nodes. Returns an empty set if the node has no neighbors or doesn't exist.
     */
    std::set<T> getNeighbors(const T& node) const;

    /**
     * @brief Prints the graph's adjacency list to the standard output.
     */
    void printGraph() const;
};

//the constructor
template <typename T>
Graph<T>::Graph() {}

//add an isolated node
template <typename T>
void Graph<T>::addNode(const T& node) {
    //if the node does not already exist in the map
    if (adjList.find(node) == adjList.end())
        //create an empty set for its neighbors
        adjList[node] = std::set<T>();
}

//create an edge between 2 nodes
template <typename T>
void Graph<T>::addEdge(const T& v1, const T& v2) {
    addNode(v1);
    addNode(v2);

    //add v2 as a neighbor of v1 and viceversa
    adjList[v1].insert(v2);
    adjList[v2].insert(v1);
}

//returns the neighbors of a current node
template <typename T>
std::set<T> Graph<T>::getNeighbors(const T& node) const {
    //if it exists in the map
    //use .at because of const, [] might create a new node
    if (adjList.find(node) != adjList.end())
        return adjList.at(node);
    //else return am empty set
    return std::set<T>();
}

template <typename T>
void Graph<T>::printGraph() const {
    //iterate through the pairs of the map
    for (const auto& pair : adjList) {
        //print the node
        std::cout << pair.first << " -> ";
        //and all of its neighbors
        for (const auto& neighbor : pair.second)
            std::cout << neighbor << " ";
        std::cout << std::endl;
    }
}