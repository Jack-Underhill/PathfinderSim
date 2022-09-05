/* Jack Underhill & Jack Sanger
 * CS 133, Spring 2022
 * Final Project - Pathfinding
 */
#include "BFS.h"

namespace PFSim {

namespace Pathfinder {

    BFS::BFS(MazeGraph*& graph) : PathfinderTemplate(graph) 
    {
        m_NodeQueue.push( getStartingPlace() );
    }

    int BFS::currStep() 
    {  
        MazeNode* currNode = m_NodeQueue.front();
        m_NodeQueue.pop();

        if(m_TargetListSize == 0 && currNode->getType() == EndCell)
        {
            setIsComplete(true);
            m_TargetNodeFound = currNode;
            m_IsStillSearching = false;//
        }
        // else if(currNode->getType() == CheckpointCell && m_TargetList->count(currNode->getPosition().positionKey)) 
        // {
        //     setIsComplete(true);
        //     m_TargetNodeFound = currNode;

        //     m_TargetList->erase(currNode->getPosition().positionKey);
        // }
        else if(currNode->getType() == CheckpointCell && removeTargetIfContained(currNode)) 
        {
        }
        else 
        {
            currNode->setIsVisited(true);
            currNode->setIsNext(false);

            addAvailableMoves(currNode);

            if(m_NodeQueue.empty())
            {
                m_IsStillSearching = false;
            }
        }

        return currNode->getPosition().positionKey;
    }

    void BFS::addIfAvailable(MazeNode*& curr, MazeNode*& prev, DirectionMoved dir)
    {
        if(PathfinderTemplate::isAvailableMove(curr)) 
        {
            m_NodeQueue.push(curr);

            curr->setDirectionMovedIn(dir);
            curr->setIsVisited(true);
            setNext(curr);

            curr->parent = prev;
        }
    }
    
} // namespace Pathfinder

} // namespace PFSim