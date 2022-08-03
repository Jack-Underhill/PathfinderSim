#ifndef _MazeNode_h_
#define _MazeNode_h_

#include <string>

namespace PFSim {
    
    // Representation of different cell types on the maze.
    enum CellType 
    {
        GenerationCell, // DefaultCell,
        BlankCell,
        EndCell,
        CheckpointCell,
        StartCell
    };

    // Directions to move in.
    enum DirectionMoved 
    {
        NORTH,
        WEST,
        SOUTH,
        EAST, 
        CENTER
    };

    struct NodePosition 
    {
        int x;
        int y;
        int positionKey;

        // sets (x, y) coordinates.
        NodePosition(int x = 0, int y = 0, int mazeLength = -1) : x(x), y(y) 
        { 
            if(mazeLength > -1)     // current flagger for if constructed with mazeLength or not.
                updatePositionKey(mazeLength); 
        }

        void updatePositionKey(int mazeLength) 
        {
            positionKey = x + ((y - 1) * (mazeLength));
        }

        // // overload `<` operator to use a `Node` object as a key in a `std::map`
        // // It returns true if the current object appears before the specified object
        // bool operator < (const NodePosition& pos) const { return x < pos.x || (x == pos.x && y < pos.y); } 
    };

    class MazeNode 
    {
    public:
        MazeNode* N;
        MazeNode* W;
        MazeNode* S;
        MazeNode* E;
        MazeNode* parent;

        MazeNode(const NodePosition& p);

        //Returns the CellType's string name.
        std::string getTypeString() const;

        //Returns the color the cell should be in the GUI
        std::string getColor() const;

        CellType getType() const { return m_CellType; }
        const NodePosition& getPosition() const { return m_Position; }
        DirectionMoved getDirectionMovedIn() const { return m_MovedIn; }

        bool isVisited() const { return m_IsVisited; }
        bool isNext() const { return m_IsNext; }
        bool isPath() const { return m_IsPath; }

        // Returns true if the cell has not been visited and is not a StartCell.
        bool isAvailableToMoveInto();

        void setType(CellType c) { m_CellType = c; }
        void setDirectionMovedIn(DirectionMoved Dir) { m_MovedIn = Dir; }
        void setIsVisited(bool isVisited) { m_IsVisited = isVisited; }
        void setIsNext(bool isNext) { m_IsNext = isNext; }
        void setIsPath(bool isPath) { m_IsPath = isPath; }

    private:
        CellType m_CellType;
        NodePosition m_Position;
        DirectionMoved m_MovedIn;

        bool m_IsVisited;
        bool m_IsNext;
        bool m_IsPath;
    };

}

#endif