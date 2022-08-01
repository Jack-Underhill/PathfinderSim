#include "MazeGraph.h"

namespace PFSim {
    
    MazeGraph::MazeGraph() 
    {
        m_MazeLength = DEFAULT_MAZE_LENGTH;
        m_MappedNodes = nullptr;

        m_StartNode = nullptr;
        m_EndNode = nullptr;

        m_Animation = new Generator::Open(m_MappedNodes, m_MazeLength);
        
        // foundTargetNode = nullptr;
        // m_IsComplete = false;

        // checkpointsToFind = new std::unordered_set<NodePosition>();
        // checkpointStack = new std::stack<NodePosition>();
        // currCheckpointsToFind = new std::unordered_set<NodePosition>();

        // buildDisconnectedGraph();
    }
    
    int MazeGraph::getCellSize() const 
    {
        double removedWallSizeFromTotalSize = DISPLAY_SIZE - ((m_MazeLength + 1) * WALL_WIDTH);

        return (removedWallSizeFromTotalSize / m_MazeLength);
    }
    
    MazeNode*& MazeGraph::updateAnimation() 
    {
        // if(m_Animation->getType() == Pathfind) {
        //     // PathfinderTemplate* finder = (PathfinderTemplate*)m_Animation;
        //     // return animatingPathfinder(finder);
        // } else { 
            return m_Animation->step();
        // }
    }
    
    void MazeGraph::setGeneratorOpen(int mazeLength) 
    {
        if(m_MappedNodes != nullptr && m_MappedNodes->size() > 0) 
        {
            disposeGraph();
        }
        m_MazeLength = mazeLength;
        buildDisconnectedGraph();

        m_Animation = new Generator::Open(m_MappedNodes, mazeLength);
    }

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
    }

    void MazeGraph::disposeGraph() 
    {
        // delete all mapped MazeNodes
        for(int row = 1; row <= m_MazeLength; row++) 
        {
            for(int col = 1; col <= m_MazeLength; col++) 
            {
                NodePosition pos(col, row, m_MazeLength);
                delete m_MappedNodes->at(pos.positionKey);
            }
        }   

        // delete the map
        delete m_MappedNodes;
    }

}
