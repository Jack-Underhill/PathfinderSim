#include "ClearObstacles.h"

namespace PFSim {

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