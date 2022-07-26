/* Jack Underhill & Jack Sanger
 * CS 133, Spring 2022
 * Final Project - Pathfinding
 * The purpose of this class is to interatively find the end of a mazing using a DFS algorithm.
 * To do this the class has a step() method which will progress the pathfinder one step as well
 * as a findEnd() method which will step through the pathfinder until the end is found.
 * When a path has been found the class can then return that class via the getPath() method.
 */

#ifndef _DFS_h_
#define _DFS_h_

#include <string>
#include <unordered_set>
#include <stack>

#include "PathfinderTemplate.h"
#include "MazeNode.h"

namespace PFSim {

namespace Pathfinder {

    class DFS: public PathfinderTemplate 
    {
    public:
        //Constructor takes the start, end, and a list of checkpoints the pathfinder must hit
        DFS(MazeGraph*& graph);

        //Returns the animation's title to be displayed on the GUI while it runs
        std::string getTitle() const { return "Depth First Search"; }

        PathfinderType getPathfinderType() const { return PathfinderType::DFS; }
        
    private:
        std::stack<MazeNode*> m_NodeStack;

        //Searches through the maze graph until it reaches a deadend (no current-neighboring 
        //unvisited-cells). Then it backtrack its steps until it can move into a unvisited cell.
        //Search ends when it has found all checkpoints (if any), then finds the end node.
        int currStep();
        
        void addIfAvailable(MazeNode*& curr, MazeNode*& prev, DirectionMoved dir);
    };
    
} // namespace Pathfinder

} // namespace PFSim

#endif