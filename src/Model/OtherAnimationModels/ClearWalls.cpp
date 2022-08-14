#include "ClearWalls.h"

namespace PFSim {

    ClearWalls::ClearWalls(std::unordered_map<int, MazeNode*>*& mappedNodes, int mazeLength) : ResetNodes(mappedNodes, mazeLength)
    {
    }

    int ClearWalls::step() 
    {
        MazeNode*& node = m_MappedNodes->at(m_CurrPos.positionKey);
        if(node->getType() == WallCell)
        {
            node->setType(BlankCell);
        }

        return ResetNodes::step();
    }

} // namespace PFSim