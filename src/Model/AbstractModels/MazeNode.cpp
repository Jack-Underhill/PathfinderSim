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
                    return "Path Cell";
                // }
            default:
                if(this->isNext()) 
                {
                    return "Next Cell";
                } 
                if(this->isVisited()) 
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
                if(this->isNext()) 
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
                if(this->isNext()) 
                {
                    return "#207d9c";// gray blue
                } 
                else if(this->isVisited()) 
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