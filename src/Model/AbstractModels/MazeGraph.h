#ifndef _MazeGraph_h_
#define _MazeGraph_h_

#include <unordered_map>

#include "Constants.h"

#include "MazeNode.h"
#include "AnimationObject.h"


namespace PFSim {

    class MazeGraph 
    {
    public:
        MazeGraph(int length);
        ~MazeGraph();

        int getMazeLength() const { return m_MazeLength; }
        int getCellSize() const { return m_CellSize; }
        GeneratorType getGeneratorType() { return m_Generated; }
        std::unordered_map<int, MazeNode*>* getNodeMap() { return m_MappedNodes; }
        MazeNode* getStartNode() { return m_StartNode; }
        MazeNode* getEndNode() { return m_EndNode; }
        int getTargetCount() { return m_TargetListSize; }
        int* getTargets() { return m_TargetList; }
        MazeNode* getLastFoundTarget() { return m_LastTargetFound; }

        bool isReadyForSimulation() const { return m_IsReadyForSimulation; }
        bool isMazeGenerated() const { return (m_Generated != GeneratorType::Open); }
 
        MazeNode*& updateCheckpoint(bool isAdding);
        // MazeNode*& updateEndNode() { return spawnInCellType(EndCell); }
        MazeNode*& updateEndNode();

        void clearTargetFound() { m_LastTargetFound = nullptr; }

        void setIsReadyForSimulation(bool val) { m_IsReadyForSimulation = val; }
        void setGeneratorType(GeneratorType type) { m_Generated = type; }
        void setStartNode(MazeNode*& node) { m_StartNode = node; }
        void setEndNode(MazeNode*& node) { m_EndNode = node; }
        void setTargetFound(MazeNode* lastTargetFound) { m_LastTargetFound = lastTargetFound; }

    private:
        int m_MazeLength;
        int m_CellSize;
        std::unordered_map<int, MazeNode*>* m_MappedNodes;

        int m_TargetList[CHECKPOINT_LIMIT] = {0};
        int m_TargetListSize;

        MazeNode* m_StartNode;
        MazeNode* m_EndNode;
        MazeNode* m_LastTargetFound;

        GeneratorType m_Generated;

        bool m_IsReadyForSimulation;

        // Creates maze node's in a grid formation each with their own position.
        // The grid formation is a square of the gui's given mazeLength of nodes.
        // Each node is mapped to its position since this function is building a disconnected graph
        // (no nodes connected).
        void buildDisconnectedGraph();

        MazeNode*& spawnInCellType(CellType type);
        MazeNode*& findAvailableNode();
    };

} // namespace PFSim

#endif