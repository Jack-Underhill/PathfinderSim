#include "ResetNodes.h"

namespace PFSim {

    ResetNodes::ResetNodes(std::unordered_map<int, MazeNode*>*& mappedNodes, int mazeLength)
    {
        m_MazeLength = mazeLength;
        m_MappedNodes = mappedNodes;
        m_CurrPos = NodePosition(1, 1, mazeLength);
    }

    int ResetNodes::step() 
    {
        MazeNode*& node = m_MappedNodes->at(m_CurrPos.positionKey);

        node->setIsVisited(false);
        node->setIsNext(false);
        node->setIsPath(false);
        node->parent = nullptr;

        // // Reset cell to a blank cell in case of it being a path cell or was a next cell.
        // CellType type = node->getType();
        // if(type == PathCell) {
        //     node->setType(Blank);
        // }
        
        if(m_CurrPos.y < m_MazeLength) {
            m_CurrPos.y++;
        }
        else if(m_CurrPos.x < m_MazeLength) {
            m_CurrPos.y = 1;
            m_CurrPos.x++;
        }
        else {
            setIsComplete(true);
        }

        m_CurrPos.updatePositionKey(m_MazeLength);

        return node->getPosition().positionKey;
    }

} // namespace PFSim