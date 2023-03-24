/* 
 * Jack Underhill
 * Summer 2022
 * Maze Generating and Pathfinding Simulator
 * 
 * Pathfinder--Depth First Search(DFS)
 * 
 * This pathfinding algorithm walks the depth of a chosen direction until it can no longer.
 * It then continues on walking the depth of a new randomly-chosen direction if available;
 * if there are no available moves, then the algorithm recursively backtracks its steps until 
 * there is available moves and then repeats the steps above. The algorithm ends once it has 
 * found its target(s) or it has recursively backtracked to the start.
 * 
 * This implementation of DFS uses a stack. 
 * Before each step, the current available moves to be made are pushed onto the stack. The node 
 * from the top of the stack is always the next chosen step. This allows only the most recently 
 * visited (available moves) to be prioritized first. The algorithm ends when the stack is empty.
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
        //Takes the given graph to store the starting node for this pathfinding algorithm.
        DFS(MazeGraph*& graph);

        //Default destructor.
        ~DFS() = default;

        //Returns the animation's title.
        std::string getTitle() const { return "Depth First Search"; }

        //Returns the pathfinder type.
        PathfinderType getPathfinderType() const { return PathfinderType::DFS; }
        
    private:
        std::stack<MazeNode*> m_NodeStack;

        //Takes and pops the top node from the m_NodeStack to be current node step.
        //PathfinderTemplate::currStepTemplate() is called and passed the current node.
        //Returns the popped node.
        int currStep();
        
        //addIfAvailable() for each (N,W,S,E) neighboring nodes into the m_NodeStack.
        //If the m_NodeStack is still empty after, then set the algorithm to be no longer searching.
        void updatePathfinderStep(MazeNode*& currNode);

        //If it is available then: 
        //set the given moved direction to the given curr node,
        //set the curr node to next,
        //set the curr node's parent node to be the given prev node.
        void addIfAvailable(MazeNode*& curr, MazeNode*& prev, DirectionMoved dir);
    };
    
} // namespace Pathfinder
} // namespace PFSim

#endif