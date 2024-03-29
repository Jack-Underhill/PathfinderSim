/* Jack Underhill & Jack Sanger
 * CS 133, Spring 2022
 * Final Project - Pathfinding
 */
#include "GeneratorTemplate.h"

namespace PFSim {

    GeneratorTemplate::GeneratorTemplate(MazeGraph*& graph)
    {
        m_MappedNodes = graph->getNodeMap();
        m_MazeLength = graph->getMazeLength();
    }

    // bool GeneratorTemplate::isInsideMaze(const NodePosition& p) const
    // {
    //     bool isWithinXBounds = (1 <= p.x && p.x <= m_MazeLength);
    //     bool isWithinYBounds = (1 <= p.y && p.y <= m_MazeLength);

    //     return isWithinXBounds && isWithinYBounds;
    // }
    
    bool GeneratorTemplate::isInsideMaze(int positionKey) const
    {
        return (1 <= positionKey && positionKey <= pow(m_MazeLength, 2));
    }
    
    MazeNode* GeneratorTemplate::connectNodes(MazeNode*& curr, const DirectionMoved& chosenMove) const
    {
        NodePosition newPos = copyPosition(curr->getPosition());
        MazeNode* cellMovedTo;

        // find chosenMove
        if(chosenMove == NORTH) 
        {
            // move to chosen position
            newPos.y--;
            newPos.updatePositionKey(m_MazeLength);
            // find node at position
            cellMovedTo = m_MappedNodes->at(newPos.positionKey);
            // connect nodes
            curr->N = cellMovedTo;
            cellMovedTo->S = curr;
        }
        else if(chosenMove == WEST) 
        {
            // move to chosen position
            newPos.x--;
            newPos.updatePositionKey(m_MazeLength);
            // find node at position
            cellMovedTo = m_MappedNodes->at(newPos.positionKey);
            // connect nodes
            curr->W = cellMovedTo;
            cellMovedTo->E = curr;
        }
        else if(chosenMove == SOUTH) 
        {
            // move to chosen position
            newPos.y++;
            newPos.updatePositionKey(m_MazeLength);
            // find node at position
            cellMovedTo = m_MappedNodes->at(newPos.positionKey);
            // connect nodes
            curr->S = cellMovedTo;
            cellMovedTo->N = curr;
        }
        else 
        {
            // move to chosen position
            newPos.x++;
            newPos.updatePositionKey(m_MazeLength);
            // find node at position
            cellMovedTo = m_MappedNodes->at(newPos.positionKey);
            // connect nodes
            curr->E = cellMovedTo;
            cellMovedTo->W = curr;
        }

        return cellMovedTo;
    }
    
    NodePosition GeneratorTemplate::copyPosition(const NodePosition& pos) const
    {
        NodePosition p(pos.x, pos.y, m_MazeLength);

        return p;
    }
    
    DirectionMoved GeneratorTemplate::inverseDirection(const DirectionMoved& dir) const
    {
        if(dir < 2)
        {
            return (DirectionMoved)(dir + 2);
        }
        else
        {
            return (DirectionMoved)(dir - 2);
        }
    }
    
    bool GeneratorTemplate::isOnTheSameRow(int positionKey, const MazeNode* node) const
    {
        return GeneratorTemplate::m_MappedNodes->at(positionKey)->getPosition().y == node->getPosition().y;
    }
    
} // namespace PFSim