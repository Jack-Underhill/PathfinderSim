/* Jack Underhill
 * CS 133, Spring 2022
 * Final Project - Pathfinding
 */
#include "PathfinderTemplate.h"

namespace PFSim {

    PathfinderTemplate::PathfinderTemplate(MazeGraph*& graph) : AnimationObject() 
    {
        m_Graph = graph;

        m_IsStillSearching = true;
    }

    PathfinderTemplate::~PathfinderTemplate() {}

    int PathfinderTemplate::step() 
    {
        if(stackOfNextNodes.size() > 0) 
        {
            return nextStep();
        }
        else 
        {
            m_stepCount++;
            return currStep();
        }
    }

    AnimationType PathfinderTemplate::getType() const 
    {
        return Pathfind;
    }

    int PathfinderTemplate::nextStep() 
    {
        MazeNode* nextNode = stackOfNextNodes.top();
        stackOfNextNodes.pop();
        
        return nextNode->getPosition().positionKey;
    }

    void PathfinderTemplate::setNext(MazeNode* curr) 
    {
        if(m_IsAnimating && curr->getType() == BlankCell)
        {
            curr->setIsNext(true);
            stackOfNextNodes.push(curr);
        }
    }

    void PathfinderTemplate::addAvailableMoves(MazeNode*& curr) 
    {
        // NORTH neighboring cell checked to add.
        addIfAvailable(curr->N, curr, NORTH);

        // WEST neighbor
        addIfAvailable(curr->W, curr, WEST);

        // SOUTH neighbor
        addIfAvailable(curr->S, curr, SOUTH);

        // EAST neighbor
        addIfAvailable(curr->E, curr, EAST);
    }
    
    bool PathfinderTemplate::isAvailableMove(MazeNode*& curr) const
    {
        return (curr != nullptr && curr->getType() != WallCell && !curr->isVisited());
    }
    
    bool PathfinderTemplate::removeTargetIfContained(MazeNode*& curr)
    {
        for(int i = 0; i < m_TargetListSize; i++) 
        {
            if(m_TargetList[i] == curr->getPosition().positionKey)
            {
                setIsComplete(true);
                m_IsStillSearching = false;
                m_TargetNodeFound = curr;

                m_TargetList[i] = m_TargetList[m_TargetListSize - 1];
                m_TargetListSize--;

                return true;
            }
        }

        return false;
    }
    
    void PathfinderTemplate::currStepTemplate(MazeNode*& currNode)
    {
        if(m_TargetListSize == 0 && currNode->getType() == EndCell)
        {
            finalizePathfinder(currNode);
        }
        else if(currNode->getType() == CheckpointCell)
        {
            int index = PathfinderTemplate::getIndexOfTargetInList(currNode->getPosition().positionKey);

            //target is in list of remaining targets
            if(index != -1)
            {
                setIsComplete(true);
                m_IsStillSearching = false;
                m_TargetNodeFound = currNode;

                m_TargetList[index] = m_TargetList[m_TargetListSize - 1];
                m_TargetListSize--;
            }
            //else treat as basic step
            else
            {
                updatePathfinderStep(currNode);
            }
        }
        else
        {
            updatePathfinderStep(currNode);
        }
    }
    
    void PathfinderTemplate::finalizePathfinder(MazeNode*& currNode)
    {
        AnimationObject::setIsComplete(true);
        m_TargetNodeFound = currNode;
        m_IsStillSearching = false;
    }
    
    int PathfinderTemplate::getIndexOfTargetInList(int target)
    {
        for(int i = 0; i < m_TargetListSize; i++) 
        {
            if(m_TargetList[i] == target)
            {
                return i;
            }
        }

        return -1;
    }

    MazeNode* PathfinderTemplate::getStartingPlace()
    {
        if(m_Graph->getLastFoundTarget() == nullptr)
        {
            return m_Graph->getStartNode();
        }
        else
        {
            return m_Graph->getLastFoundTarget();
        }
    }
    
    void PathfinderTemplate::setTargetList(int size, int* targets)
    {
        m_TargetListSize = size;

        for(int i = 0; i < m_TargetListSize; i++)
        {
            m_TargetList[i] = *(targets + i);
        }
    }

} // namespace PFSim