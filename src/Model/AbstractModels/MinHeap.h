
#ifndef _MinHeap_h_
#define _MinHeap_h_

#include <vector>

#include "MazeNode.h"

namespace PFSim {
    
    struct AStarNodeProps
    {
        MazeNode* node;    // Node's posisiton Key
        int distanceG;  // Step count from start
        int distanceH;  // Heuristic (Manhatten) distance

        int getFScore() { return distanceG + distanceH; } 

        AStarNodeProps(MazeNode* node, int distanceG, int distanceH)
        {
            this->node = node;
            this->distanceG = distanceG;
            this->distanceH = distanceH;
        }
        ~AStarNodeProps() = default;
    };

    class MinHeap 
    {
    public:
        MinHeap();
        ~MinHeap();
        bool isEmpty() const { return (m_PropVector.size() == 0); }
        AStarNodeProps* top() const;
        void pop();
        bool push(AStarNodeProps* props);

        // void print(int index);
    private:
        std::vector<AStarNodeProps*> m_PropVector; // change to array (initialize the capacity with mazeLength^2 or graph size)

        int getLeftChildIndex(int parentIndex) const { return 2 * parentIndex + 1; }
        int getRightChildIndex(int parentIndex) const { return 2 * parentIndex + 2; }
        int getParentIndex(int childIndex) const { return (childIndex - 1) / 2; }

        bool hasLeftChild(int index) const { return getLeftChildIndex(index) < m_PropVector.size(); }
        bool hasRightChild(int index) const { return getRightChildIndex(index) < m_PropVector.size(); }
        bool hasParent(int index) const { return getParentIndex(index) >= 0; }

        AStarNodeProps* leftChild(int index) const { return m_PropVector[getLeftChildIndex(index)]; }
        AStarNodeProps* rightChild(int index) const { return m_PropVector[getRightChildIndex(index)]; }
        AStarNodeProps* parent(int index) const { return m_PropVector[getParentIndex(index)]; }

        int find(int key) const;

        void swap(int index1, int index2);
        
        //bubble it into position  (default to bubble up from the end but give the option to bubble up from a given index)
        void bubbleUp(int index = -1);
        
        void bubbleDown();
    };

} // namespace PFSim

#endif