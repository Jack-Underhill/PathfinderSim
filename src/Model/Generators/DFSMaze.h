/* 
 * Jack Underhill
 * Summer 2022
 * Maze Generating and Pathfinding Simulator
 * 
 * Generator--Depth First Search Maze(DFSMaze)
 * 
 * This generating algorithm links every node in the given graph in a randomized depth first search.
 * The links are undirected.
 * 
 * This implementation randomizes each step for a depth; instead of sticking to 1 direction for a
 * depth in a normal DFS. For each step to be decided, a list of unvisited NWSE neighboring nodes
 * is created and then 1 node from the list is chosen at random. This continues until there is an 
 * empty list of unvisited neighboring nodes. Then backtrack the steps until the list is no longer
 * empty and repeat from the top. The algorithm finishes once it has backtracked to the startnode.
 */

#ifndef _DFSMaze_h_
#define _DFSMaze_h_

#include <cmath>
#include <iostream>
#include <vector>
#include <stack>

#include "GeneratorTemplate.h"
#include "MazeNode.h"

namespace PFSim {
namespace Generator {

    //Moves through the disconnected graph, connecting nodes in its path, until it reaches a
    //deadend (no current-neighboring unvisited-cells). Then it backtrack its steps 
    //until it can move into a neighboring unvisited cell (NextCell).
    //This DFS is unique to normal DFS; it randomly chooses from its avaible moves to move into.
    //Returns the cell it came from or nullptr if the graph is complete.
    class DFSMaze : public GeneratorTemplate 
    {
    public:
        //Takes the given graph to store the starting node for this generating algorithm.
        DFSMaze(MazeGraph*& graph);

        //Returns the animation's title.
        std::string getTitle() const { return "Random DFS Maze"; }

        //Returns the generator type.
        GeneratorType getGeneratorType() const { return GeneratorType::DFSMaze; }
        
        //Saves the current position to return at the end of the function.
        //
        //If there are an available unvisited node to move into, then call stepNew().
        //Else if the current node is not the startcell (it hasnt backtracked back to the start), 
        //   ---then call stepBacktrack().
        //Else its the end of the algorithm and is marked for completion.
        int step();

    private:
        std::stack<MazeNode*> m_NodeStack;

        //Check the N,S,W,E neighboring nodes, into isAvailableMove(), to return true. 
        //Each neighbor that does return true: their direction from the curr node is added into a 
        //vector to be returned.
        std::vector<DirectionMoved> getAvailableMoves(const MazeNode* curr) const;

        //Returns true if a given position contains a cell which is valid to connect to.
        //
        //Valid if:
        //--The given position matches a node's position inside of the maze.
        //--The matched node hasn't been visited/backtracked.
        bool isAvailableMove(int positionKey) const;

        //Open the given currNode by setting it to being visited.
        //
        //Out of the availableMoves, a direction to move in is randomly selected. The node moved 
        //into is then linked with currNode.
        //
        //The node moved into is then added into the m_NodeStack.
        void stepNew(std::vector<DirectionMoved> availableMoves, MazeNode* currNode);

        //Display top node of the m_NodeStack as closed: set to unvisited and a blankcell.
        //Pop the top node of the m_NodeStack.
        void stepBacktrack(MazeNode* currNode);
    };

} // namespace Generator
} // namespace PFSim

#endif