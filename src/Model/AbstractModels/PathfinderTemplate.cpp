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
            return currStep();
        }
    }

    int PathfinderTemplate::nextStep() 
    {
        MazeNode* nextNode = stackOfNextNodes.top();
        stackOfNextNodes.pop();
        return nextNode->getPosition().positionKey;
    }

    void PathfinderTemplate::setNext(MazeNode* curr) 
    {
        if(curr->getType() != EndCell) 
        {
            curr->setIsNext(true);
            curr->setIsVisited(true);
            stackOfNextNodes.push(curr);
        }
    }

    AnimationType PathfinderTemplate::getType() const 
    {
        return Pathfind;
    }

    // void PathfinderTemplate::setTargetFound(MazeNode* node)
    // {
    //     this->m_NodeTargetFound = node;
    // }

    // MazeNode* PathfinderTemplate::getEndNode() const
    // {
    //     return m_NodeTargetFound;
    // }

    // void PathfinderTemplate::setCheckpoints(std::unordered_set<NodePosition>* checkpointsToFind) 
    // {
    //     this->checkpointsToFind = checkpointsToFind;
    // }

    // std::unordered_set<NodePosition>* PathfinderTemplate::getCheckpoints() const 
    // {
    //     return checkpointsToFind;
    // }

} // namespace PFSim