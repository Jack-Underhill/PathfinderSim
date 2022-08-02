#include "MazeGraph.h"

namespace PFSim {
    
    MazeGraph::MazeGraph() 
    {
        m_MazeLength = DEFAULT_MAZE_LENGTH;
        m_MappedNodes = nullptr;

        m_StartNode = nullptr;
        m_EndNode = nullptr;
        
        m_TargetList = nullptr;
        m_CheckpointStack = nullptr;

        m_Animation = nullptr;

        m_Pathfinder = PathfinderType::NoPathfinder;
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
        int key = m_Animation->step();
        MazeNode*& node = m_MappedNodes->at(key);

        if(m_Animation->isComplete() && m_Animation->getType() == Reset)
        {
            findPathfinderToInit();
        }
        else if(m_Animation->isComplete() && m_Animation->getType() == Pathfind && 
                ((PathfinderTemplate*)m_Animation)->getTargetNodeFound()->getType() == CheckpointCell)
        {
            m_LastTargetFound = ((PathfinderTemplate*)m_Animation)->getTargetNodeFound();

            initResetNodes();
        }

        return node;
    }
    
    void MazeGraph::setGeneratorOpen(int mazeLength) 
    {
        initGeneratorOpen(mazeLength);
    }

    void MazeGraph::setPathfinderBFS()
    {
        initTargetList();
        m_LastTargetFound = nullptr;

        if(m_Pathfinder != PathfinderType::NoPathfinder) // not newly generated
        {
            initResetNodes();
        
            m_Pathfinder = PathfinderType::BFS;
        }
        else
        {            
            initPathfinderBFS();
        }
    }

    // void MazeGraph::setPathfinderDFS()
    // {
    //     initTargetList();
    //     m_LastTargetFound = nullptr;

    //     if(m_Pathfinder != PathfinderType::NoPathfinder) // not newly generated
    //     {
    //         initResetNodes();
        
    //         m_Pathfinder = PathfinderType::DFS;
    //     }
    //     else
    //     {
    //         initPathfinderDFS();
    //     }
    // }

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
    
    MazeNode*& MazeGraph::removeTopCheckpoint() 
    {
        MazeNode*& node = m_MappedNodes->at(m_CheckpointStack->top());

        m_TargetList->erase(m_CheckpointStack->top());
        m_CheckpointStack->pop();

        node->setType(Blank);

        return node;
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

        m_TargetList = new std::unordered_set<int>();
        m_CheckpointStack = new std::stack<int>();
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
        
        delete m_TargetList;
        delete m_CheckpointStack;
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
        else if(type == CheckpointCell) 
        {
            node->setType(CheckpointCell);
            m_CheckpointStack->push(node->getPosition().positionKey);
        } 
        else 
        {
            node->setType(type);
        }
    }

    void MazeGraph::freeAllocatedAnimation()
    {
        if(m_Animation != nullptr) 
        {
            delete m_Animation;
            m_Animation = nullptr;
        }
    }

    void MazeGraph::initGenerator(int mazeLength)
    {
        if(m_MappedNodes != nullptr && m_MappedNodes->size() > 0) 
        {
            disposeGraph();
        }
        m_MazeLength = mazeLength;
        buildDisconnectedGraph();

        findNodeToSetType(StartCell);
        findNodeToSetType(EndCell);

        m_Pathfinder = PathfinderType::NoPathfinder;
    }
    
    void MazeGraph::initGeneratorOpen(int mazeLength)
    {
        freeAllocatedAnimation();

        initGenerator(mazeLength);

        m_Animation = new Generator::Open(m_MappedNodes, mazeLength);
    }

    void MazeGraph::findPathfinderToInit() 
    {
        switch (m_Pathfinder)
        {
        case(BFS):
            initPathfinderBFS();
            break;
        case(DFS):
            // initPathfinderDFS();
            break;
        case(NoPathfinder):
            break;
        }
    }

    void MazeGraph::initPathfinderBFS()
    {
        freeAllocatedAnimation();
        
        m_Pathfinder = PathfinderType::BFS;

        if(m_LastTargetFound == nullptr) // isn't re-pathfinding from last target found. Is starting from the start node.
        {
            m_Animation = new Pathfinder::BFS(m_StartNode, m_TargetList);
        }
        else
        {
            m_Animation = new Pathfinder::BFS(m_LastTargetFound, m_TargetList);
        }
    }

    void MazeGraph::initResetNodes() 
    {
        freeAllocatedAnimation();

        m_Animation = new ResetNodes(m_MappedNodes, m_MazeLength);
    }

    void MazeGraph::initTargetList() 
    {
        std::stack<int> temp;
        int stackSize = m_CheckpointStack->size();

        for(int i = 0; i < stackSize; i++)
        {
            m_TargetList->insert(m_CheckpointStack->top());

            temp.push(m_CheckpointStack->top());
            m_CheckpointStack->pop();
        }

        for(int i = 0; i < stackSize; i++)
        {
            m_CheckpointStack->push(temp.top());
            temp.pop();
        }
    }

}

