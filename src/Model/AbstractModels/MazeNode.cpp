#include "MazeNode.h"

namespace PFSim  {
    
    MazeNode::MazeNode(const NodePosition& pos) 
    {
        m_Position = pos;

        N = nullptr;
        W = nullptr;
        S = nullptr;
        E = nullptr;
        parent = nullptr;
        
        m_MovedIn = CENTER;
        m_CellType = GenerationCell;
        m_IsVisited = false;
        m_IsNext = false;
        m_IsPath = false;
    }

    std::string MazeNode::getString() const 
    {
        return PFSim::getTypeString(m_CellType, m_IsNext, m_IsVisited, m_IsPath);
    }

    std::string MazeNode::getColor() const
    {
        return PFSim::getNodeColor(m_CellType, m_IsNext, m_IsVisited, m_IsPath);
    }

    bool MazeNode::isAvailableToMoveInto() 
    {
        return !m_IsVisited && m_CellType != StartCell;
    }

    std::string getTypeString(CellType type, bool isNext, bool isVisited, bool isPath)
    {
        if(isPath) 
        {
            if(isNext)
                return "Path Head";
            else
                return "Path Cell";
        } 

        switch(type) 
        {
        case(WallCell):
            return "Wall Cell";
        case(StartCell):
            return "Start Cell";
        case(EndCell):
            return "End Cell";
        case(CheckpointCell):
            return "Checkpoint Cell";
        case(GenerationCell):
            return "Generation Cell";
        default: //(BlankCell)
            if(isNext) 
                return "Next Cell";
            else if(isVisited) 
                return "Visited Cell";
            else 
                return "Empty Cell";
        }
    }
    
    std::string getNodeColor(CellType type, bool isNext, bool isVisited, bool isPath)
    {
        // Prioritize before Celltype
        if(isPath)
        {
            if(isNext)
                // return "#84B6C5";//glacier blue
                // return "#207d9c";// gray blue
                return "cyan";
            else
                return "#123D73";// dark blue
        }

        switch(type) 
        {
        case(WallCell):
            return "dark gray";
        case(StartCell):
            return "#77F032";// green
        case(EndCell):
            return "#9c2022";// red
        case(CheckpointCell):
            return "orange";
        case(GenerationCell):
            return "#A7A7A7";//gray
        default: //(BlankCell)
            if(isNext) 
                return "#207d9c";// gray blue
            else if(isVisited) 
                return "cyan";
            else 
                return "#FFFFFF";// white
        }
    }

}