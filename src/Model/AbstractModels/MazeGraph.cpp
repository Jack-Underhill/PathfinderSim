#include "MazeGraph.h"

namespace PFSim {
    
    MazeGraph::MazeGraph(int length) 
    {
        m_MazeLength = length;
        m_TargetListSize = 0;

        m_MappedNodes = nullptr;
        m_StartNode = nullptr;
        m_LastTargetFound = nullptr;

        m_IsReadyForSimulation = true;
        
        buildDisconnectedGraph();
    }
    
    MazeGraph::~MazeGraph() 
    { 
        int size = m_MappedNodes->size();
        for(int key = 1; key <= size; key++) 
        {
            delete m_MappedNodes->at(key);
        } 

        delete m_MappedNodes;
    }

    MazeNode*& MazeGraph::updateCheckpoint(bool isAdding)
    {
        m_IsReadyForSimulation = true;
        
        if(isAdding)//Add a checkpoint
        {
            return spawnInCellType(CheckpointCell);
        }
        else//Remove a checkpoint
        {
            MazeNode*& node = m_MappedNodes->at(m_TargetList[m_TargetListSize - 1]);
            node->setType(BlankCell);

            m_TargetList[m_TargetListSize - 1] = 0;
            m_TargetListSize--;

            return node;
        }
    }
    
    MazeNode*& MazeGraph::updateEndNode() 
    { 
        setEndNode( spawnInCellType(EndCell) ); 

        return m_EndNode;
    }

    /******************************************Private********************************************/

    void MazeGraph::buildDisconnectedGraph() 
    {
        m_MappedNodes = new std::unordered_map<int, MazeNode*>();

        for(int row = 1; row <= m_MazeLength; row++) 
        {
            for(int col = 1; col <= m_MazeLength; col++) 
            {
                NodePosition pos = NodePosition(col, row, m_MazeLength);
                MazeNode* node = new MazeNode(pos);
                (*m_MappedNodes)[pos.positionKey] = node;
            }
        }

        spawnInCellType(StartCell);        
        
        double removedWallSizeFromTotalSize = DISPLAY_SIZE - ((m_MazeLength + 1) * WALL_WIDTH);
        m_CellSize = removedWallSizeFromTotalSize / m_MazeLength;
    }

    MazeNode*& MazeGraph::findAvailableNode() 
    {
        NodePosition pos = NodePosition(rand() % m_MazeLength + 1, rand() % m_MazeLength + 1, m_MazeLength);
        MazeNode*& node = m_MappedNodes->at(pos.positionKey);
        
        // If curr node is already taken, then "reroll" position until curr node is not taken.
        while(node->getType() == StartCell || node->getType() == EndCell || node->getType() == CheckpointCell || node->getType() == WallCell) 
        {
            pos = NodePosition(rand() % m_MazeLength + 1, rand() % m_MazeLength + 1, m_MazeLength);
            node = m_MappedNodes->at(pos.positionKey);
        }

        return node;
    }

    MazeNode*& MazeGraph::spawnInCellType(CellType type) 
    {
        MazeNode*& node = findAvailableNode();

        node->setType(type);

        if(type == StartCell) 
        {
            m_StartNode = node;
        }
        else if(type == CheckpointCell) 
        {
            m_TargetList[m_TargetListSize] = node->getPosition().positionKey;
            m_TargetListSize++;
        }

        return node;
    }

}