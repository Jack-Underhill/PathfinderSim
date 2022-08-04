/* Jack Underhill & Jack Sanger
 * CS 133, Spring 2022
 * Final Project - Pathfinding
 */

#include "DFS.h"

namespace PFSim {

namespace Pathfinder {

    // if you can figure out how to pass in checkpoints by reference correctly and prevent the need of a return function for it, please do.
    DFS::DFS(MazeNode* startNode, std::unordered_set<int>* targetList) : PathfinderTemplate(targetList) 
    {
        nodeStack.push(startNode);
    }

    int DFS::currStep() 
    {
        //load curr node from stack
        MazeNode* currNode = nodeStack.top();
        nodeStack.pop();

        if(m_TargetList->size() == 0 && currNode->getType() == EndCell) 
        {
            setIsComplete(true);
            m_TargetNodeFound = currNode;
        }
        else if(currNode->getType() == CheckpointCell && m_TargetList->count(currNode->getPosition().positionKey)) 
        {
            setIsComplete(true);
            m_TargetNodeFound = currNode;
            m_TargetList->erase(currNode->getPosition().positionKey);
        }
        else 
        {
            //set as current visited cell and not next.
            currNode->setIsVisited(true);
            currNode->setIsNext(false);

            //load available moves into stack
            stackAvailableMoves(nodeStack, currNode);
        }

        return currNode->getPosition().positionKey;
    }

    void DFS::stackAvailableMoves(std::stack<MazeNode*>& nodeStack, MazeNode*& curr) 
    {
        // Check to add NORTH neighboring cell.
        if(curr->N != nullptr && curr->N->isAvailableToMoveInto()) 
        {
            nodeStack.push(curr->N);
            curr->N->setDirectionMovedIn(NORTH);
            setNext(curr->N);
            curr->N->parent = curr;
        }

        // Check to add WEST neighboring cell.
        if(curr->W != nullptr && curr->W->isAvailableToMoveInto()) 
        {
            nodeStack.push(curr->W);
            curr->W->setDirectionMovedIn(WEST);
            setNext(curr->W);
            curr->W->parent = curr;
        }

        // Check to add SOUTH neighboring cell.
        if(curr->S != nullptr && curr->S->isAvailableToMoveInto()) 
        {
            nodeStack.push(curr->S);
            curr->S->setDirectionMovedIn(SOUTH);
            setNext(curr->S);
            curr->S->parent = curr;
        }

        // Check to add EAST neighboring cell.
        if(curr->E != nullptr && curr->E->isAvailableToMoveInto()) 
        {
            nodeStack.push(curr->E);
            curr->E->setDirectionMovedIn(EAST);
            setNext(curr->E);
            curr->E->parent = curr;
        }
    }

    std::string DFS::getTitle() const 
    {
        return "DFS Pathfinding";
    }
    
} // namespace Pathfinder

} // namespace PFSim