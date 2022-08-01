/* Jack Underhill & Jack Sanger
 * CS 133, Spring 2022
 * Final Project - Pathfinding
 */
#include "GeneratorTemplate.h"

namespace PFSim {

    GeneratorTemplate::GeneratorTemplate(std::unordered_map<int, MazeNode*>*& mappedNodes, int mazeLength)
    {
        m_MappedNodes = mappedNodes;
        m_MazeLength = mazeLength;
    }

    bool GeneratorTemplate::isInsideMaze(const NodePosition& p, int mazeLength) const
    {
        bool isWithinXBounds = (1 <= p.x && p.x <= mazeLength);
        bool isWithinYBounds = (1 <= p.y && p.y <= mazeLength);

        return isWithinXBounds && isWithinYBounds;
    }
    
} // namespace PFSim