#include "MazeGraph.h"

namespace PFSim {
    
    MazeGraph::MazeGraph() 
    {
        m_MazeLength = DEFAULT_MAZE_LENGTH;
        m_MappedNodes = nullptr;

        m_StartNode = nullptr;
        m_EndNode = nullptr;

        m_Animation = nullptr;
        
        // foundTargetNode = nullptr;
        // m_IsComplete = false;

        // checkpointsToFind = new std::unordered_set<NodePosition>();
        // checkpointStack = new std::stack<NodePosition>();
        // currCheckpointsToFind = new std::unordered_set<NodePosition>();
    }
    
    MazeGraph::~MazeGraph() 
    { 
        disposeGraph(); 

        delete m_Animation;
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


            int key = m_Animation->step();
            MazeNode*& n = m_MappedNodes->at(key);

            return n;
        // }
    }
    
    void MazeGraph::setGeneratorOpen(int mazeLength) 
    {
        if(m_Animation != nullptr) 
        {
            delete m_Animation;
            m_Animation = nullptr;
        }

        if(m_MappedNodes != nullptr && m_MappedNodes->size() > 0) 
        {
            disposeGraph();
        }
        m_MazeLength = mazeLength;
        buildDisconnectedGraph();

        findNodeToSetType(StartCell);
        findNodeToSetType(EndCell);

        m_Animation = new Generator::Open(m_MappedNodes, mazeLength);
    }

    void MazeGraph::setPathfinderBFS()
    {
        if(m_Animation != nullptr) 
        {
            delete m_Animation;
            m_Animation = nullptr;
        }

        m_Animation = new Pathfinder::BFS(m_StartNode, nullptr);    // nullptr is temp until checkpoints are implemented
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

        m_StartNode = nullptr;
        m_EndNode = nullptr;
    }

    void MazeGraph::findNodeToSetType(CellType type) 
    {
        NodePosition pos = NodePosition(rand() % m_MazeLength + 1, rand() % m_MazeLength + 1, m_MazeLength);
        MazeNode*& node = m_MappedNodes->at(pos.positionKey);
        
        // If curr node is already taken, then "reroll" position until curr node is not taken.
        while(node->getType() != DefaultCell && node->getType() != Blank) 
        {
            pos = NodePosition(rand() % m_MazeLength + 1, rand() % m_MazeLength + 1, m_MazeLength);
            node = m_MappedNodes->at(pos.positionKey);
        }

        // Node found to host new CellType, set node.
        setNode(node, type);
    }

    void MazeGraph::setNode(MazeNode* node, CellType type) 
    {
        node->setDirectionMovedIn(CENTER);
        if(type == StartCell) 
        {
            node->setType(StartCell);
            m_StartNode = node;
        }
        else if(type == EndCell) 
        {
            node->setType(EndCell);
            m_EndNode = node;
        }
        // else if(type == CheckpointCell) 
        // {
        //     node->setType(CheckpointCell);
        //     NodePosition pos = node->getPosition();
        //     checkpointsToFind->insert(pos);
        //     checkpointStack->push(pos);
        // } 
        else 
        {
            node->setType(type);
        }
    }

}

