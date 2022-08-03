#include "MazeNode.h"

namespace PFSim  {

    // bool NodePosition::operator < (const NodePosition& pos) const 
    // {
    //     return x < pos.x || (x == pos.x && y < pos.y);
    // }

    
    MazeNode::MazeNode(const NodePosition& pos) 
    {
        m_Position = pos;

        N = nullptr;
        W = nullptr;
        S = nullptr;
        E = nullptr;
        parent = nullptr;
        
        m_CellType = DefaultCell;
        m_IsVisited = false;
        m_IsNext = false;
        m_IsPath = false;
    }

    std::string MazeNode::getTypeString() const 
    {
        switch(m_CellType) 
        {
            case(StartCell):
                return "Maze Start";
            case(EndCell):
                return "Maze End";
            case(CheckpointCell):
                return "Maze Checkpoint";
            case(DefaultCell):
                return "Generation Cell";
            case(PathCell):
                // if(this->isNext()) {
                //     return "Path Head";
                // } 
                // else {
                    // return "Path Cell";
                // }
            default:
                if(isPath()) 
                {
                    return "Path Cell";
                } 
                else if(isNext()) 
                {
                    return "Next Cell";
                } 
                else if(isVisited()) 
                {
                    return "Visited Cell";
                } 
                else 
                {
                    return "Empty Cell";
                }
        }
    }

    std::string MazeNode::getColor() const
    {
        if(m_IsPath)
        {
            if(m_IsNext)
            {
                return "cyan";
            }
            else
            {
                return "#123D73";// dark blue
            }
        }

        switch(m_CellType) 
        {
            case(StartCell):
                return "#77F032";// green
            case(EndCell):
                return "#9c2022";// red
            case(CheckpointCell):
                return "orange";
            case(DefaultCell):
                return "#A7A7A7";//gray
            case(PathCell):
                if(isNext()) 
                {
                    // return "#84B6C5";//glacier blue
                    // return "#207d9c";// gray blue
                    return "cyan";
                } 
                else 
                {
                    return "#123D73";// dark blue
                }
            default:
                // if(isPath() && isNext())
                // {
                //     return "cyan";
                // }
                // else if(isPath())
                // {
                //     return "#123D73";// dark blue
                // }
                 if(isNext()) 
                {
                    return "#207d9c";// gray blue
                } 
                else if(isVisited()) 
                {
                    return "cyan";
                } 
                else 
                {
                    return "#FFFFFF";// white
                }
        }
    }

    bool MazeNode::isAvailableToMoveInto() 
    {
        return !isVisited() && getType() != StartCell;
    }

}