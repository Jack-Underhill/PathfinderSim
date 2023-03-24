/* 
 * Jack Underhill
 * Summer 2022
 * Maze Generating and Pathfinding Simulator
 */
#include "BFS.h"

namespace PFSim {
namespace Pathfinder {

    BFS::BFS(MazeGraph*& graph) : PathfinderTemplate(graph) 
    {
        m_NodeQueue.push( PathfinderTemplate::getStartingPlace() );
    }

    int BFS::currStep() 
    {  
        MazeNode* currNode = m_NodeQueue.front();
        m_NodeQueue.pop();
        
        PathfinderTemplate::currStepTemplate(currNode);

        return currNode->getPosition().positionKey;
    }
    
    MazeNode* BFS::getCurrentNodeStep()
    {
        MazeNode* currNode = m_NodeQueue.front();
        m_NodeQueue.pop();

        return currNode;
    }
    
    void BFS::updatePathfinderStep(MazeNode*& currNode)
    {
        currNode->setIsNext(false);
        currNode->setIsVisited(true);

        PathfinderTemplate::addAvailableMoves(currNode);

        if(m_NodeQueue.empty())
        {
            m_IsStillSearching = false;
        }
    }

    void BFS::addIfAvailable(MazeNode*& curr, MazeNode*& prev, DirectionMoved dir)
    {
        if(PathfinderTemplate::isAvailableMove(curr)) 
        {
            m_NodeQueue.push(curr);

            curr->setDirectionMovedIn(dir);
            curr->setIsVisited(true);
            PathfinderTemplate::setNext(curr);

            curr->parent = prev;
        }
    }
    
} // namespace Pathfinder
} // namespace PFSim