/* Jack Underhill & Jack Sanger
 * CS 133, Spring 2022
 * Final Project - Pathfinding
 */
#include "PathfinderTemplate.h"

namespace PFSim {

    PathfinderTemplate::PathfinderTemplate(std::unordered_set<int>* targetList) : AnimationObject() 
    {
        m_TargetList = targetList;
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
        if(curr->getType() == BlankCell)
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

} // namespace PFSim