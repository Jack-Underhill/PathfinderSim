#include "Open.h"

namespace PFSim {
    
namespace Generator {
    
    Open::Open(MazeGraph*& graph) : GeneratorTemplate(graph) 
    {
        m_CurrPos = NodePosition(1, 1, m_MazeLength);
    }

    int Open::step() 
    {        
        MazeNode*& node = m_MappedNodes->at(m_CurrPos.positionKey);

        if(node->getType() == GenerationCell)
        {
            node->setType(BlankCell);
        }
        node->setDirectionMovedIn(CENTER);

        connectUnlinkedNeighbors(node);
        
        if(m_CurrPos.x < m_MazeLength)
        {
            m_CurrPos.x++;
        }
        else if(m_CurrPos.y < m_MazeLength) 
        {
            m_CurrPos.x = 1;
            m_CurrPos.y++;
        }
        else 
        {
            setIsComplete(true);
        }

        m_CurrPos.updatePositionKey(m_MazeLength);

        m_stepCount++;

        return node->getPosition().positionKey;
    }

    /*********************************************Private*********************************************/

    void Open::connectUnlinkedNeighbors(MazeNode*& node) 
    {
        // Check NORTH neighboring cell if has been connected. Connect if not yet.
        NodePosition checkNorth(m_CurrPos.x, m_CurrPos.y - 1, m_MazeLength);
        if(isInsideMaze(checkNorth, m_MazeLength)) 
        {
            node->N = m_MappedNodes->at(checkNorth.positionKey);
        }

        NodePosition checkWest(m_CurrPos.x - 1, m_CurrPos.y, m_MazeLength);
        if(isInsideMaze(checkWest, m_MazeLength)) 
        {
            node->W = m_MappedNodes->at(checkWest.positionKey);
        }

        NodePosition checkSouth(m_CurrPos.x, m_CurrPos.y + 1, m_MazeLength);
        if(isInsideMaze(checkSouth, m_MazeLength)) 
        {
            node->S = m_MappedNodes->at(checkSouth.positionKey);
        }

        NodePosition checkEast(m_CurrPos.x + 1, m_CurrPos.y, m_MazeLength);
        if(isInsideMaze(checkEast, m_MazeLength)) 
        {
            node->E = m_MappedNodes->at(checkEast.positionKey);
        }
    }

} // namespace Generator

} // namespace PFSim