#ifndef _InteractiveCells_h_
#define _InteractiveCells_h_

#include <cmath>
#include <stack>
#include <unordered_map>

#include "Constants.h"

#include "MazeNode.h"
#include "MazeGraph.h"

namespace PFSim {

    class InteractiveCells
    {
    public:
        InteractiveCells(int x, int y, MazeGraph*& graph);

        std::stack<MazeNode*>& getMouseUpdatedNodes() { return m_MouseUpdatedNodes; }
        int getMousePositionKey() { return m_MousePositionKey; }
        MazeNode*& getNewStartNode() { return m_StartNode; }

        bool isNewPosition(int x, int y) const;
        bool isStartNodeMoved() { return m_IsStartNodeMoved; }

        bool setMouseMoved(int x, int y);   //returns false if conditions give no update.
        void setDrawWallsMode(bool val);
        void setEraseWallsMode(bool val);

    private:
        int m_MazeLength;
        std::unordered_map<int, MazeNode*>* m_MappedNodes;

        std::stack<MazeNode*> m_MouseUpdatedNodes;

        int m_MousePositionKey;
        int m_LastMousePositionKey;
        int m_CellSize;

        bool m_IsDrawWallsMode;
        bool m_IsEraseWallsMode;

        bool m_IsStartNodeMoved;

        MazeNode* m_StartNode;
        
        int* m_TargetList;
        int m_TargetListSize;
        
        int getKeyConversion(int x, int y) const;
        
        void handlePaintOver(int x, int y);
        void handleDraggedNode(int x, int y);
        void updateTrackedNodesDragged(MazeNode*& currNode, MazeNode*& prevNode);
        void updateSwappedCheckpoints(int newKey, int oldKey);
    };
    
} // namespace PFSim


#endif