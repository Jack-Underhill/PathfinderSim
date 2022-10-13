
#ifndef _MinHeap_h_
#define _MinHeap_h_

#include <vector>

#include "MazeNode.h"
#include "PathSolution.h"


namespace PFSim {

    class HeapProps
    {
    public:
        virtual int getValue() const = 0;
    };


    class MinHeap 
    {
    public:
        MinHeap() = default;
        virtual ~MinHeap();
        bool isEmpty() const { return (m_PropVector.size() == 0); }
        HeapProps* top() const;
        void pop();
        virtual bool push(HeapProps* props) = 0;

        // void print(int index);
    protected:
        std::vector<HeapProps*> m_PropVector; // change to array (initialize the capacity with mazeLength^2 or graph size)

        int getLeftChildIndex(int parentIndex) const { return 2 * parentIndex + 1; }
        int getRightChildIndex(int parentIndex) const { return 2 * parentIndex + 2; }
        int getParentIndex(int childIndex) const { return (childIndex - 1) / 2; }

        bool hasLeftChild(int index) const { return getLeftChildIndex(index) < m_PropVector.size(); }
        bool hasRightChild(int index) const { return getRightChildIndex(index) < m_PropVector.size(); }
        bool hasParent(int index) const { return getParentIndex(index) >= 0; }

        HeapProps* leftChild(int index) const { return m_PropVector[getLeftChildIndex(index)]; }
        HeapProps* rightChild(int index) const { return m_PropVector[getRightChildIndex(index)]; }
        HeapProps* parent(int index) const { return m_PropVector[getParentIndex(index)]; }

        void swap(int index1, int index2);
        
        //bubble it into position  (default to bubble up from the end but give the option to bubble up from a given index)
        void bubbleUp(int index = -1);
        
        void bubbleDown();
    };

} // namespace PFSim

#endif