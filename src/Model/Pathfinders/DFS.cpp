/* Jack Underhill & Jack Sanger
 * CS 133, Spring 2022
 * Final Project - Pathfinding
 */

#include "DFS.h"

namespace PFSim {

namespace Pathfinder {

    DFS::DFS(MazeNode* startNode, std::unordered_set<int>* targetList) : PathfinderTemplate(targetList) 
    {
        m_NodeStack.push(startNode);
    }

    int DFS::currStep() 
    {
        MazeNode* currNode = m_NodeStack.top();
        m_NodeStack.pop();

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
            currNode->setIsVisited(true);
            currNode->setIsNext(false);

            addAvailableMoves(currNode);

            if(m_NodeStack.empty())
            {
                m_IsStillSearching = false;
            }
        }

        return currNode->getPosition().positionKey;
    }
    
    void DFS::addIfAvailable(MazeNode*& curr, MazeNode*& prev, DirectionMoved dir)
    {
        if(PathfinderTemplate::isAvailableMove(curr)) 
        {
            m_NodeStack.push(curr);

            curr->setDirectionMovedIn(dir);
            setNext(curr);
            
            curr->parent = prev;
        }
    }
    
} // namespace Pathfinder

} // namespace PFSim