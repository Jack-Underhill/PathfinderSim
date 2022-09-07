/* Jack Underhill & Jack Sanger
 * CS 133, Spring 2022
 * Final Project - Pathfinding
 */

#include "DFS.h"

namespace PFSim {

namespace Pathfinder {

    DFS::DFS(MazeGraph*& graph) : PathfinderTemplate(graph) 
    {
        m_NodeStack.push( getStartingPlace() );
    }

    int DFS::currStep() 
    {
        MazeNode* currNode = m_NodeStack.top();
        m_NodeStack.pop();

        if(m_TargetListSize == 0 && currNode->getType() == EndCell) 
        {
            setIsComplete(true);
            m_TargetNodeFound = currNode;
            m_IsStillSearching = false;
        }
        else if((currNode->getType() == CheckpointCell && !removeTargetIfContained(currNode)) || 
                 currNode->getType() != CheckpointCell)
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