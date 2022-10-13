
#include "MinHeap.h"

namespace PFSim {
    
    // void MinHeap::print(int index) 
    // {
    //     if(-1 < index && index < m_PropVector.size())
    //     {
    //         // node
    //         std::cout << m_PropVector[index]->getValue() << std::endl;
    //         if(!hasLeftChild(index) && !hasRightChild(index)) //isleaf
    //         {
    //             std::cout << "---------" << std::endl;
    //         }

    //         if(hasLeftChild(index))
    //         {
    //             print(getLeftChildIndex(index));
    //         }
    //         if(hasRightChild(index))
    //         {
    //             print(getRightChildIndex(index));
    //         }
    //     }
    // }

    MinHeap::~MinHeap()
    {
        for(HeapProps* prop : m_PropVector)
        {
            delete prop;
        }
    }

    HeapProps* MinHeap::top() const
    {
        if(m_PropVector.size() > 0)
        {
            return m_PropVector[0];
        }
        else
        {
            return nullptr;
        }
    }

    void MinHeap::pop()
    {
        if(m_PropVector.size() > 0)
        {
            delete m_PropVector.front();
            m_PropVector.front() = m_PropVector.back();
            m_PropVector.pop_back();

            bubbleDown();
        }
    }
    
    void MinHeap::swap(int index1, int index2)
    {
        HeapProps* temp = m_PropVector[index1];
        m_PropVector[index1] = m_PropVector[index2];
        m_PropVector[index2] = temp;
    }

    void MinHeap::bubbleUp(int index)
    {
        if(index == -1)
        {
            index = m_PropVector.size() - 1;
        }

        while(hasParent(index) && (parent(index)->getValue() > m_PropVector[index]->getValue()))
        {
            swap(getParentIndex(index), index);
            index = getParentIndex(index);
        }
    }

    void MinHeap::bubbleDown()
    {
        int index = 0;
        bool isStillBubbling = true;

        while((isStillBubbling) && hasLeftChild(index))
        {
            //get index of child with lower FScore
            int smallerChildIndex = getLeftChildIndex(index);
            if(hasRightChild(index) && (rightChild(index)->getValue() < leftChild(index)->getValue()))
            {
                smallerChildIndex = getRightChildIndex(index);
            }

            //bubble down if child is less than or equal to parent, else end while loop.
            if(m_PropVector[index]->getValue() >= m_PropVector[smallerChildIndex]->getValue())
            {
                swap(index, smallerChildIndex); 
            }
            else
            {
                isStillBubbling = false;
            }

            //update new parent
            index = smallerChildIndex;
        }
    }

} // namespace PFSim