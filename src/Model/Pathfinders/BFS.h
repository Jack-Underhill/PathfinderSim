/* Jack Underhill & Jack Sanger
 * CS 133, Spring 2022
 * Final Project - Pathfinding
 */
#ifndef _BFS_h_
#define _BFS_h_

#include <string>
#include <vector>
#include <queue>
#include <set>
#include <stack>

#include "PathfinderTemplate.h"
#include "MazeNode.h"

namespace Pathfinder
{

    // class BFS: public Pathfinder {
    // public:
    //     // //Constructor takes the start, end, and a list of checkpoints the pathfinder must hit.
    //     // PathfindBFS(MazeNode* startNode, set<Position>* checkpointsToFind);

    //     // //Returns the animation's title to be displayed on the GUI while it runs.
    //     // string getTitle() const;

    // private:
    //     // queue<MazeNode*> nodeQueue;

    //     // //Searches through the maze graph by 1 (current-neighboring unvisited-cells) at a time.
    //     // //All neighboring unvisited cells (NextCell) of visited cells are kept track of in a queue.
    //     // //1 NextCell is dequeued at a time to be moved into and changed to visited. Doing this updates
    //     // //queue with adding the NextCells of the newly visited cell.
    //     // //Search ends when it has found all checkpoints (if any), then finds the end node.
    //     // MazeNode* currStep();
        
    //     // //Helper function which modifies the given queue to be
    //     // //a collection of unvisited neighbors of the given node
    //     // void queueAvailableMoves(queue<MazeNode*>& nodeQueue, MazeNode*& curr);
    // };
    
} // namespace Pathfinder

#endif