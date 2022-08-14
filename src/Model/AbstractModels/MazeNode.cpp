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
        
        m_CellType = GenerationCell;
        m_IsVisited = false;
        m_IsNext = false;
        m_IsPath = false;
    }

    std::string MazeNode::getTypeString() const 
    {
        switch(m_CellType) 
        {
        case(WallCell):
            return "Wall Cell";
        case(StartCell):
            return "Maze Start";
        case(EndCell):
            return "Maze End";
        case(CheckpointCell):
            return "Maze Checkpoint";
        case(GenerationCell):
            return "Generation Cell";
        default: //(BlankCell)
            if(m_IsPath) 
            {
                if(m_IsNext)
                    return "Path Head";
                else
                    return "Path Cell";
            } 
            else if(m_IsNext) 
                return "Next Cell";
            else if(m_IsVisited) 
                return "Visited Cell";
            else 
                return "Empty Cell";
        }
    }

    std::string MazeNode::getColor() const
    {
        // Disregards Celltype
        if(m_IsPath)
        {
            if(m_IsNext)
                // return "#84B6C5";//glacier blue
                // return "#207d9c";// gray blue
                return "cyan";
            else
                return "#123D73";// dark blue
        }

        switch(m_CellType) 
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
            if(m_IsNext) 
                return "#207d9c";// gray blue
            else if(m_IsVisited) 
                return "cyan";
            else 
                return "#FFFFFF";// white
        }
    }

    bool MazeNode::isAvailableToMoveInto() 
    {
        return !m_IsVisited && m_CellType != StartCell;
    }

}