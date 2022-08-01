/* Jack Underhill & Jack Sanger
 * CS 133, Spring 2022
 * Final Project - Pathfinding
 */
#include "BFS.h"

namespace PFSim {

namespace Pathfinder {

    // if you can figure out how to pass in checkpoints by reference correctly and prevent the need of a return function for it, please do.
    BFS::BFS(MazeNode* startNode, std::unordered_set<NodePosition>* checkpointsToFind) : PathfinderTemplate(checkpointsToFind) 
    {
        nodeQueue.push(startNode);
    }

    int BFS::currStep() 
    {
        //load curr node from queue
        MazeNode* currNode = nodeQueue.front();
        nodeQueue.pop();

        CellType type = currNode->getType();
        // NodePosition pos = currNode->getPosition();
        // set<NodePosition>* checkpoints = getCheckpoints();
        // if(checkpoints->size() == 0 && type == EndCell)
        if(type == EndCell) 
        { //
            setIsComplete(true);
            setEndNode(currNode);
        }
        // else if(type == CheckpointCell && checkpoints->count(pos)) 
        // {
        //     setIsComplete(true);
        //     setEndNode(currNode);
        //     checkpoints->erase(pos);
        //     setCheckpoints(checkpoints);
        // }
        else 
        {
            //set as current visited cell and not next.
            currNode->setIsVisited(true);
            currNode->setIsNext(false);

            //load available moves into queue
            queueAvailableMoves(nodeQueue, currNode);
        }

        // if(isComplete())                                 // Im keeping this because it was from the old version. But I do think it is redundant and bad style.
        // {
        //     MazeNode* nullNode = nullptr;
        //     return nullNode;
        // }
        // else 
        // {
            return currNode->getPosition().positionKey;
        // }
    }

    void BFS::queueAvailableMoves(std::queue<MazeNode*>& nodeQueue, MazeNode*& curr) 
    {
        // Check to add NORTH neighboring cell.
        if(curr->N != nullptr && curr->N->isAvailableToMoveInto()) 
        {
            nodeQueue.push(curr->N);
            curr->N->setDirectionMovedIn(NORTH);
            setNext(curr->N);
            curr->N->parent = curr;
        }

        // Check to add WEST neighboring cell.
        if(curr->W != nullptr && curr->W->isAvailableToMoveInto()) 
        {
            nodeQueue.push(curr->W);
            curr->W->setDirectionMovedIn(WEST);
            setNext(curr->W);
            curr->W->parent = curr;
        }

        // Check to add SOUTH neighboring cell.
        if(curr->S != nullptr && curr->S->isAvailableToMoveInto()) 
        {
            nodeQueue.push(curr->S);
            curr->S->setDirectionMovedIn(SOUTH);
            setNext(curr->S);
            curr->S->parent = curr;
        }

        // Check to add EAST neighboring cell.
        if(curr->E != nullptr && curr->E->isAvailableToMoveInto()) 
        {
            nodeQueue.push(curr->E);
            curr->E->setDirectionMovedIn(EAST);
            setNext(curr->E);
            curr->E->parent = curr;
        }
    }

    std::string BFS::getTitle() const 
    {
        return "BFS Pathfinding";
    }
    
} // namespace Pathfinder

} // namespace PFSim