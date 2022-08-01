#include "Open.h"

namespace PFSim {
    
namespace Generator {
    
    Open::Open(std::unordered_map<int, MazeNode*>*& mappedNodes, int mazeLength) 
    : GeneratorTemplate(mappedNodes, mazeLength) 
    {
        m_CurrPos = NodePosition(1, 1, m_MazeLength);
    }

    MazeNode*& Open::step() {
        MazeNode*& node = m_MappedNodes->at(m_CurrPos.positionKey);

        node->setType(Blank);
        node->setDirectionMovedIn(CENTER);

        connectUnlinkedNeighbors(node);
        
        if(m_CurrPos.x < m_MazeLength) {
            m_CurrPos.x++;
        }
        else if(m_CurrPos.x == m_MazeLength && m_CurrPos.y < m_MazeLength) {
            m_CurrPos.x = 1;
            m_CurrPos.y++;
        }
        else {
            setIsComplete(true);
        }

        m_CurrPos.updatePositionKey(m_MazeLength);

        return node;
    }

    std::string Open::getTitle() const {
        return "Generating without Maze";
    }

    /*********************************************Private*********************************************/

    void Open::connectUnlinkedNeighbors(MazeNode*& node) {
        // Check NORTH neighboring cell if has been connected. Connect if not yet.
        NodePosition checkNorth(m_CurrPos.x, m_CurrPos.y - 1, m_MazeLength);
        if(isInsideMaze(checkNorth, m_MazeLength)) {
            node->N = m_MappedNodes->at(checkNorth.positionKey);
        }

        NodePosition checkWest(m_CurrPos.x - 1, m_CurrPos.y, m_MazeLength);
        if(isInsideMaze(checkWest, m_MazeLength)) {
            node->W = m_MappedNodes->at(checkWest.positionKey);
        }

        NodePosition checkSouth(m_CurrPos.x, m_CurrPos.y + 1, m_MazeLength);
        if(isInsideMaze(checkSouth, m_MazeLength)) {
            node->S = m_MappedNodes->at(checkSouth.positionKey);
        }

        NodePosition checkEast(m_CurrPos.x + 1, m_CurrPos.y, m_MazeLength);
        if(isInsideMaze(checkEast, m_MazeLength)) {
            node->E = m_MappedNodes->at(checkEast.positionKey);
        }
    }

} // namespace Generator

} // namespace PFSim