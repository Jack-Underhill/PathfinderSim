/* Jack Underhill & Jack Sanger
 * CS 133, Spring 2022
 * Final Project - Pathfinding
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
        //Constructor takes the start, end, and a list of checkpoints the pathfinder must hit.
        BFS(MazeGraph*& graph);

        //Returns the animation's title to be displayed on the GUI while it runs.
        std::string getTitle() const { return "BFS"; }

        PathfinderType getPathfinderType() const { return PathfinderType::BFS; }

    private:
        std::queue<MazeNode*> m_NodeQueue;

        //Searches through the maze graph by 1 (current-neighboring unvisited-cells) at a time.
        //All neighboring unvisited cells (NextCell) of visited cells are kept track of in a queue.
        //1 NextCell is dequeued at a time to be moved into and changed to visited. Doing this updates
        //queue with adding the NextCells of the newly visited cell.
        //Search ends when it has found all checkpoints (if any), then finds the end node.
        int currStep();

        void addIfAvailable(MazeNode*& curr, MazeNode*& prev, DirectionMoved dir);
    };
    
} // namespace Pathfinder

} // namespace PFSim

#endif