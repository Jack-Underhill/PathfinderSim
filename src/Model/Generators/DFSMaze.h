/* Jack Underhill & Jack Sanger
 * CS 133, Spring 2022
 * Final Project - Pathfinding
 * The purpose of this class is to generate a randomized DFS maze for the pathfinder to navigate.
 */
#ifndef _DFSMaze_h_
#define _DFSMaze_h_

#include <cmath>
#include <iostream>
#include <vector>
#include <stack>
#include <unordered_map>

#include "GeneratorTemplate.h"
#include "MazeNode.h"

namespace PFSim {
    
namespace Generator {

    class DFSMaze : public GeneratorTemplate {
    public:
        //Constructor takes a map of all the existing nodes to their location and a starting position
        //to generate the graph from.
        DFSMaze(std::unordered_map<int, MazeNode*>* mappedNodes, int mazeLength, MazeNode* startNode);

        //Moves through the disconnected graph, connecting nodes in its path, until it reaches a
        //deadend (no current-neighboring unvisited-cells). Then it backtrack its steps 
        //until it can move into a neighboring unvisited cell (NextCell).
        //This DFS is unique to normal DFS; it randomly chooses from its avaible moves to move into.
        //Returns the cell it came from or nullptr if the graph is complete.
        int step();

        //Returns the animation's title to be displayed on the GUI while it runs.
        std::string getTitle() const { return "Random DFS Maze"; }

        GeneratorType getGeneratorType() const { return GeneratorType::DFSMaze; }

    private:
        std::stack<MazeNode*> nodeStack;
        MazeNode* currNode; 
        DirectionMoved currBacktrackFrom;

        //Connects the given current cell to the one in the given direction
        //then returns the cell in that direction.
        MazeNode* connectNodes(MazeNode*& curr, DirectionMoved& chosenMove) const;

        // Check N,S,W,E and see if they are nullptr (available) and make a vector of available directions.
        std::vector<DirectionMoved> getAvailableMoves(MazeNode*& curr) const;

        //Returns true if a given position contains a cell which is valid to connect to.
        bool isAvailableMove(NodePosition checkPos) const;

        int stepNew(std::vector<DirectionMoved> availableMoves);
        int stepBacktrack();
    };

} // namespace Generator

} // namespace PFSim

#endif