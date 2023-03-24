/* 
 * Jack Underhill
 * Summer 2022
 * Maze Generating and Pathfinding Simulator
 * 
 * Pathfinder--Breadth First Search(BFS)
 * 
 * This pathfinding algorithm radially searches out, from a starting point, in every direction.
 * BFS continues to search out radially through the graph (layerwise) until it finds its target.
 * 
 * This implementation of DFS uses a queue. 
 * Before each step, the current available moves to be made are pushed onto the queue. The node 
 * from the front of the queue is always the chosen next step. This allows only the first pushed
 * visited (available moves) to be prioritized first. The algorithm ends when the queue is empty.
 */

#ifndef _BFS_h_
#define _BFS_h_

#include <string>
#include <queue>
#include <unordered_set>
#include <stack>

#include "PathfinderTemplate.h"
#include "MazeNode.h"

namespace PFSim {
namespace Pathfinder {

    class BFS : public PathfinderTemplate 
    {
    public:
        //Takes the given graph to store the starting node for this pathfinding algorithm.
        BFS(MazeGraph*& graph);
        
        //Defualt destructor.
        ~BFS() = default;

        //Returns the animation's title.
        std::string getTitle() const { return "Breadth First Search"; }

        //Returns the pathfinder type.
        PathfinderType getPathfinderType() const { return PathfinderType::BFS; }

    protected:
        //Gets and pops the front node from the m_NodeQueue, and returns the node.
        MazeNode* getCurrentNodeStep();

        //addIfAvailable() for each (N,W,S,E) neighboring nodes into the m_NodeQueue.
        //If the m_NodeQueue is still empty after, then set the algorithm to be no longer searching.
        void updatePathfinderStep(MazeNode*& currNode);

    private:
        std::queue<MazeNode*> m_NodeQueue;

        //Takes and pops the front node from the m_NodeQueue to be current node step.
        //PathfinderTemplate::currStepTemplate() is called and passed the current node.
        //Returns the popped node.
        int currStep();

        //If it is available then: 
        //set the given moved direction to the given curr node,
        //set the curr node to visited and set next,
        //set the curr node's parent node to be the given prev node.
        void addIfAvailable(MazeNode*& curr, MazeNode*& prev, DirectionMoved dir);
    };
    
} // namespace Pathfinder
} // namespace PFSim

#endif