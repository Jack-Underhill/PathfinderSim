#include "ResetNodes.h"

namespace PFSim {

    ResetNodes::ResetNodes(MazeGraph*& graph)
    {
        m_MazeLength = graph->getMazeLength();
        m_MappedNodes = graph->getNodeMap();
        m_CurrPos = NodePosition(1, 1, m_MazeLength);
    }

    int ResetNodes::step() 
    {
        m_lastPositionKey = m_CurrPos.positionKey;
        MazeNode*& node = m_MappedNodes->at(m_CurrPos.positionKey);

        node->setIsVisited(false);
        node->setIsNext(false);
        node->setIsPath(false);
        node->parent = nullptr;
        
        if(m_CurrPos.y < m_MazeLength) 
        {
            m_CurrPos.y++;
        }
        else if(m_CurrPos.x < m_MazeLength) 
        {
            m_CurrPos.y = 1;
            m_CurrPos.x++;
        }
        else 
        {
            setIsComplete(true);
        }

        m_CurrPos.updatePositionKey(m_MazeLength);

        m_stepCount++;

        return node->getPosition().positionKey;
    }
    
    void ResetNodes::resetDirection() 
    { 
        MazeNode* node = m_MappedNodes->at(m_lastPositionKey);

        if(node->getType() != CheckpointCell)
        {
            node->setDirectionMovedIn(CENTER);
        }
    }

} // namespace PFSim