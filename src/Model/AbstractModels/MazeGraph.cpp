#include "MazeGraph.h"

namespace PFSim {
    
    MazeGraph::MazeGraph() 
    {
        m_MazeLength = DEFAULT_MAZE_LENGTH;

        m_StartNode = nullptr;
        m_LastTargetFound = nullptr;

        m_TargetList = nullptr;
        m_CheckpointStack = nullptr;
        m_MappedNodes = nullptr;

        m_Animation = nullptr;
        m_PathSolution = nullptr;

        m_IsReadyForSimulation = true;
        m_IsMousePressed = false;
        m_IsMazeGenerated = true;
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
    
    CellType MazeGraph::getTargetFoundType() 
    { 
        if(m_LastTargetFound != nullptr) 
        {
            return m_LastTargetFound->getType(); 
        }
        else
        {
            return CellType::GenerationCell;
        }
    }
    
    MazeNode*& MazeGraph::updateAnimation() 
    {
        return m_MappedNodes->at(m_Animation->step());
    }

    void MazeGraph::updatePathfinderStart() 
    { 
        m_LastTargetFound = ((PathfinderTemplate*)m_Animation)->getTargetNodeFound(); 

        m_PathSolution->addCurrentSolution(m_LastTargetFound);
    }
    
    void MazeGraph::updateSimulationSetup()
    {
        m_LastTargetFound = nullptr;

        initTargetList();

        m_PathSolution = new PathSolution();
    }

    void MazeGraph::setGenerator(GeneratorType type) 
    {
        generatorSetup();
        m_Generated = type;

        m_IsMazeGenerated = true;

        switch(type)
        {
        case(Open):
            m_IsMazeGenerated = false;
            initGeneratorOpen();
            break;
        case(DFSMaze):
            initGeneratorDFS();
            break;
        }
    }

    void MazeGraph::setPathfinder(PathfinderType type) 
    {
        pathfinderSetup();

        switch(type)
        {
        case(BFS):
            initPathfinderBFS();
            break;
        case(DFS):
            initPathfinderDFS();
            break;
        }
    }
    
    void MazeGraph::setGraphReset()
    {
        initResetNodes();
    }

    void MazeGraph::setPathSolution()
    {
        initPathSolution();
    }
    
    void MazeGraph::setMousePressed(int x, int y)
    {
        m_MousePositionKey = getKeyConversion(x, y);
        m_IsMousePressed = true;

        // animate node clicked
        // std::cout << "Animate MousePressed: (" << x << ", " << y << ")" << std::endl;
    }

    void MazeGraph::setMouseMoved(int x, int y)
    {
        if(m_NodesToSwap.size() > 0)
        {
            m_NodesToSwap.pop();
            m_NodesToSwap.pop();
        }

        if(isNewPosition(x, y))
        {
            //swap cell types
            CellType temp = m_MappedNodes->at(m_LastMousePositionKey)->getType();
            m_MappedNodes->at(m_LastMousePositionKey)->setType( m_MappedNodes->at(m_MousePositionKey)->getType() );
            m_MappedNodes->at(m_MousePositionKey)->setType( temp );

            //update start
            if(m_MappedNodes->at(m_LastMousePositionKey)->getType() == StartCell)
            {
                m_StartNode = m_MappedNodes->at(m_LastMousePositionKey);
            }
            else if(m_MappedNodes->at(m_MousePositionKey)->getType() == StartCell)
            {
                m_StartNode = m_MappedNodes->at(m_MousePositionKey);
            }

            //update checkpoints
            if((m_MappedNodes->at(m_LastMousePositionKey)->getType() == CheckpointCell) &&
               (m_MappedNodes->at(m_MousePositionKey)->getType() == CheckpointCell)) 
            {
                int temp = m_LastMousePositionKey;
                m_LastMousePositionKey = m_MousePositionKey;
                m_MousePositionKey = temp;
            }
            else if(m_MappedNodes->at(m_LastMousePositionKey)->getType() == CheckpointCell)
            {
                updateSwappedCheckpoints(m_LastMousePositionKey, m_MousePositionKey);
            }
            else if(m_MappedNodes->at(m_MousePositionKey)->getType() == CheckpointCell)
            {
                updateSwappedCheckpoints(m_MousePositionKey, m_LastMousePositionKey);
            }

            // add to stack to be animated
            m_NodesToSwap.push(m_MappedNodes->at(m_LastMousePositionKey));
            m_NodesToSwap.push(m_MappedNodes->at(m_MousePositionKey));            
        }
    }

    int MazeGraph::setAvailableNodeCellType(CellType type) 
    {
        NodePosition pos = NodePosition(rand() % m_MazeLength + 1, rand() % m_MazeLength + 1, m_MazeLength);
        MazeNode*& node = m_MappedNodes->at(pos.positionKey);
        
        // If curr node is already taken, then "reroll" position until curr node is not taken.
        while(node->getType() == StartCell || node->getType() == EndCell || node->getType() == CheckpointCell) 
        {
            pos = NodePosition(rand() % m_MazeLength + 1, rand() % m_MazeLength + 1, m_MazeLength);
            node = m_MappedNodes->at(pos.positionKey);
        }

        // Node found to host new CellType, set node.
        setNodeCellType(node, type);

        return pos.positionKey;
    }

    MazeNode*& MazeGraph::addCheckpoint() 
    { 
        m_IsReadyForSimulation = true;

        setAvailableNodeCellType(CheckpointCell); 

        return m_MappedNodes->at(m_CheckpointStack->top());
    }
    
    MazeNode*& MazeGraph::removeTopCheckpoint() 
    {
        m_IsReadyForSimulation = true;

        MazeNode*& node = m_MappedNodes->at(m_CheckpointStack->top());

        m_TargetList->erase(m_CheckpointStack->top());
        m_CheckpointStack->pop();

        node->setType(BlankCell);

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
        int mazeLength = std::sqrt(m_MappedNodes->size());
        for(int row = 1; row <= mazeLength; row++) 
        {
            for(int col = 1; col <= mazeLength; col++) 
            {
                NodePosition pos(col, row, mazeLength);
                delete m_MappedNodes->at(pos.positionKey);
            }
        }   

        // delete the map
        delete m_MappedNodes;
        
        delete m_TargetList;
        delete m_CheckpointStack;
    }

    bool MazeGraph::isNewPosition(int x, int y)
    {
        int checkPositionKey = getKeyConversion(x, y);

        bool isNew = m_MousePositionKey != checkPositionKey;

        if(isNew)
        {
            m_LastMousePositionKey = m_MousePositionKey;
            m_MousePositionKey = checkPositionKey;
        }

        // compare
        return isNew;
    }

    int MazeGraph::getKeyConversion(int x, int y)
    {
        //convert pixel coords to node position.
        int xPos = (x - DISPLAY_LEFT_BUFFER - WALL_WIDTH) / (getCellSize() + WALL_WIDTH);
        xPos += (x - DISPLAY_LEFT_BUFFER - WALL_WIDTH) % (getCellSize() + WALL_WIDTH) > 0;
        
        int yPos = (y - DISPLAY_TOP_BUFFER - WALL_WIDTH) / (getCellSize() + WALL_WIDTH);
        yPos += (y - DISPLAY_TOP_BUFFER - WALL_WIDTH) % (getCellSize() + WALL_WIDTH) > 0;

        int key = NodePosition(xPos, yPos, m_MazeLength).positionKey;
        
        // std::cout << m_MousePositionKey << ": (" << xPos << ", " << yPos << ") = " << key << std::endl;

        return key;
    }

    void MazeGraph::setNodeCellType(MazeNode* node, CellType type) 
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

    void MazeGraph::generatorSetup()
    {
        freeAllocatedAnimation();

        if(m_MappedNodes != nullptr && m_MappedNodes->size() > 0) 
        {
            disposeGraph();
        }
        buildDisconnectedGraph();

        setAvailableNodeCellType(StartCell);        
        
        m_IsReadyForSimulation = true;
    }
    
    void MazeGraph::pathfinderSetup() 
    {
        freeAllocatedAnimation();
    }
    
    void MazeGraph::initGeneratorOpen()
    {
        m_Animation = new Generator::Open(m_MappedNodes, m_MazeLength);
    }
    
    void MazeGraph::initGeneratorDFS()
    {
        m_Animation = new Generator::DFSMaze(m_MappedNodes, m_MazeLength, m_StartNode);
    }

    void MazeGraph::initPathfinderBFS()
    {
        if(m_LastTargetFound == nullptr) // isn't re-pathfinding from last target found. Is starting from the start node.
        {
            m_Animation = new Pathfinder::BFS(m_StartNode, m_TargetList);
        }
        else
        {
            m_Animation = new Pathfinder::BFS(m_LastTargetFound, m_TargetList);
        }
    }
    
    void MazeGraph::initPathfinderDFS()
    {
        if(m_LastTargetFound == nullptr) // isn't re-pathfinding from last target found. Is starting from the start node.
        {
            m_Animation = new Pathfinder::DFS(m_StartNode, m_TargetList);
        }
        else
        {
            m_Animation = new Pathfinder::DFS(m_LastTargetFound, m_TargetList);
        }
    }

    void MazeGraph::initResetNodes() 
    {
        freeAllocatedAnimation();

        m_Animation = new ResetNodes(m_MappedNodes, m_MazeLength);
    }

    void MazeGraph::initPathSolution()
    {
        m_PathSolution->reversePath();

        freeAllocatedAnimation();

        m_Animation = m_PathSolution;
        m_PathSolution = nullptr;
        
        m_IsReadyForSimulation = false;
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
    
    void MazeGraph::updateSwappedCheckpoints(int newKey, int oldKey)
    {
        std::stack<int> temp;
        while(m_CheckpointStack->top() != oldKey)
        {
            temp.push(m_CheckpointStack->top());
            m_CheckpointStack->pop();
        }

        //found old key to remove
        m_CheckpointStack->pop();
        m_CheckpointStack->push(newKey);

        //put all keys back into place
        while(temp.size() > 0)
        {
            m_CheckpointStack->push(temp.top());
            temp.pop();
        }
    }

}