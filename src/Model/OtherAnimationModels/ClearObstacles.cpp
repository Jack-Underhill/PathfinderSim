#include "ClearObstacles.h"

namespace PFSim {

    ClearObstacles::ClearObstacles(std::unordered_map<int, MazeNode*>*& mappedNodes, int mazeLength) : ResetNodes(mappedNodes, mazeLength)
    {
    }

    int ClearObstacles::step() 
    {
        MazeNode*& node = m_MappedNodes->at(m_CurrPos.positionKey);
        if(node->getType() == WallCell)
        {
            node->setType(BlankCell);
        }

        return ResetNodes::step();
    }

} // namespace PFSim