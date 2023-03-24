/* 
 * Jack Underhill
 * Summer 2022
 * Maze Generating and Pathfinding Simulator
 */
#include "DFS.h"

namespace PFSim {
namespace Pathfinder {

    DFS::DFS(MazeGraph*& graph) : PathfinderTemplate(graph) 
    {
        m_NodeStack.push( PathfinderTemplate::getStartingPlace() );
    }

    int DFS::currStep() 
    {
        MazeNode* currNode = m_NodeStack.top();
        m_NodeStack.pop();

        PathfinderTemplate::currStepTemplate(currNode);

        return currNode->getPosition().positionKey;
    }
    
    void DFS::updatePathfinderStep(MazeNode*& currNode)
    {
        currNode->setIsNext(false);
        currNode->setIsVisited(true);

        PathfinderTemplate::addAvailableMoves(currNode);

        if(m_NodeStack.empty())
        {
            m_IsStillSearching = false;
        }
    }
    
    // am i pushing the same available nodes? through the course of the algorithm?
    // meaning: a specific node can be available to multiple nodes (up to 4 = NWSE), can a node be found available multiple times before being a chosen move?
    // if so then this is duplicating nodes into the m_NodeStack.
    void DFS::addIfAvailable(MazeNode*& curr, MazeNode*& prev, DirectionMoved dir) 
    {
        if(PathfinderTemplate::isAvailableMove(curr)) 
        {
            m_NodeStack.push(curr);

            curr->setDirectionMovedIn(dir);
            PathfinderTemplate::setNext(curr);
            
            curr->parent = prev;
        }
    }
    
} // namespace Pathfinder
} // namespace PFSim