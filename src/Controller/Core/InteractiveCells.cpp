#include "InteractiveCells.h"

namespace PFSim {
    
    InteractiveCells::InteractiveCells(int x, int y, MazeGraph*& graph)
    {
        m_MazeLength = graph->getMazeLength();
        m_MappedNodes = graph->getNodeMap();

        m_TargetList = graph->getTargets();
        m_TargetListSize = graph->getTargetCount();
        m_CellSize = graph->getCellSize();

        m_MousePositionKey = getKeyConversion(x, y);
        
        m_IsDrawWallsMode = false;
        m_IsEraseWallsMode = false;
    }

    bool InteractiveCells::isNewPosition(int x, int y) const
    {
        int checkPositionKey = getKeyConversion(x, y);
        
        return (m_MousePositionKey != checkPositionKey);
    }

    bool InteractiveCells::setMouseMoved(int x, int y)
    {
        while(m_MouseUpdatedNodes.size() > 0)
        {
            m_MouseUpdatedNodes.pop();
        }

        m_IsStartNodeMoved = false;

        if(m_IsDrawWallsMode || m_IsEraseWallsMode)
        {
            handlePaintOver(x, y);
        }
        else if(m_MappedNodes->at( getKeyConversion(x, y) )->getType() != WallCell)
        {
            handleDraggedNode(x, y);
        }
        else
        {   
            return false;
        }

        return true;
    }
    
    void InteractiveCells::setDrawWallsMode(bool val) 
    {
        m_IsDrawWallsMode = val; 

        if(m_IsDrawWallsMode) 
        {
            MazeNode*& node = m_MappedNodes->at(m_MousePositionKey);
            node->setType(WallCell);

            m_MouseUpdatedNodes.push(node);
        }
    }

    void InteractiveCells::setEraseWallsMode(bool val) 
    {
        m_IsEraseWallsMode = val; 

        if(m_IsEraseWallsMode) 
        {
            MazeNode*& node = m_MappedNodes->at(m_MousePositionKey);
            node->setType(BlankCell);

            m_MouseUpdatedNodes.push(node);
        }
    }

    int InteractiveCells::getKeyConversion(int x, int y) const
    {
        //convert pixel coords to node position.
        int xPos = (x - DISPLAY_LEFT_BUFFER - WALL_WIDTH) / (m_CellSize + WALL_WIDTH);
        xPos += (x - DISPLAY_LEFT_BUFFER - WALL_WIDTH) % (m_CellSize + WALL_WIDTH) > 0;
        
        int yPos = (y - DISPLAY_TOP_BUFFER - WALL_WIDTH) / (m_CellSize + WALL_WIDTH);
        yPos += (y - DISPLAY_TOP_BUFFER - WALL_WIDTH) % (m_CellSize + WALL_WIDTH) > 0;

        int key = NodePosition(xPos, yPos, m_MazeLength).positionKey;

        return key;
    }
    
    void InteractiveCells::updateSwappedCheckpoints(int newKey, int oldKey)
    {
        int i = 0;
        while(m_TargetList[i] != oldKey) 
        {
            i++;
        }

        m_TargetList[i] = newKey;
    }
    
    void InteractiveCells::handlePaintOver(int x, int y)
    {
        int checkPositionKey = getKeyConversion(x, y);
        MazeNode*& node = m_MappedNodes->at(checkPositionKey);

        CellType type = node->getType();
        if(type != CheckpointCell && type != StartCell && type != EndCell) 
        {
            if(m_IsDrawWallsMode)
            {
                node->setType(WallCell);
            }
            else if(m_IsEraseWallsMode)
            {
                node->setType(BlankCell);
            }

            m_MouseUpdatedNodes.push(node);
        }
    }

    void InteractiveCells::handleDraggedNode(int x, int y)
    {
        //update keys
        m_LastMousePositionKey = m_MousePositionKey;
        m_MousePositionKey = getKeyConversion(x, y);

        MazeNode*& currNode = m_MappedNodes->at(m_MousePositionKey);
        MazeNode*& prevNode = m_MappedNodes->at(m_LastMousePositionKey);

        //swap cell types
        CellType temp = currNode->getType();
        currNode->setType( prevNode->getType() );
        prevNode->setType(temp);

        m_MouseUpdatedNodes.push(currNode);   
        m_MouseUpdatedNodes.push(prevNode);
        updateTrackedNodesDragged(currNode, prevNode);
    }
    
    void InteractiveCells::updateTrackedNodesDragged(MazeNode*& currNode, MazeNode*& prevNode)
    {
        //update start
        if(currNode->getType() == StartCell)
        {
            m_StartNode = currNode;
            m_IsStartNodeMoved = true;
        }
        else if(prevNode->getType() == StartCell)
        {
            m_StartNode = prevNode;
            m_IsStartNodeMoved = true;
        }

        //update end
        if(currNode->getType() == EndCell)
        {
            m_EndNode = currNode;
            m_IsEndNodeMoved = true;
        }
        else if(prevNode->getType() == EndCell)
        {
            m_EndNode = prevNode;
            m_IsEndNodeMoved = true;
        }

        //update checkpoints
        if(currNode->getType() == CheckpointCell && prevNode->getType() == CheckpointCell) 
        {
            int temp = m_LastMousePositionKey;
            m_LastMousePositionKey = m_MousePositionKey;
            m_MousePositionKey = temp;
        }
        else if(currNode->getType() == CheckpointCell)
        {
            updateSwappedCheckpoints(m_MousePositionKey, m_LastMousePositionKey);
        }
        else if(prevNode->getType() == CheckpointCell)
        {
            updateSwappedCheckpoints(m_LastMousePositionKey, m_MousePositionKey);
        }
    }

} // namespace PFSim
