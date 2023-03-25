/* 
 * Jack Underhill
 * Summer 2022
 * Maze Generating and Pathfinding Simulator
 * 
 * Generator--Open
 * 
 * This generating algorithm links every node in the given graph to its unlinked adjacent 
 * neighbors. Doing so changes the unlinked graph into an undirected graph oriented like a grid.
 * 
 * This implementation only stores the integer value of the current node position key.
 * This way with each step of this algorithm, the current key is incremented until it has 
 * iterated through every node in the graph. At each iteration the current node is linked to each 
 * of its N,S,W,E neighbors.
 */

#ifndef _Open_h_
#define _Open_h_

#include <unordered_map>

#include "GeneratorTemplate.h"

namespace PFSim {
namespace Generator {
    
    class Open : public GeneratorTemplate
    {
    public:
        //Passes the given graph into the GeneratorTemplate constructor. 
        //Sets the current position key to 1.
        Open(MazeGraph*& graph);

        //Returns the animation's title.
        std::string getTitle() const { return "Open"; }

        //Returns the generator type.
        GeneratorType getGeneratorType() const { return GeneratorType::Open; }

        //Saves the current position to return at the end of the function.
        //
        //Connects the unlinked neighbors of the node at the current position.
        //
        //Update current position to the next position if there is one, else marks the algorithm as 
        //complete.
        int step();

    private:
        int m_CurrPos;

        //Connects/links the given node to every valid N,S,W,E neighbors.
        void connectUnlinkedNeighbors(MazeNode*& node);
    };

} // namespace Generator
} // namespace PFSim


#endif