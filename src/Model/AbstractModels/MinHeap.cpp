
#include "MinHeap.h"

namespace PFSim {
    
    // void MinHeap::print(int index) 
    // {
    //     if(-1 < index && index < m_PropVector.size())
    //     {
    //         // node
    //         std::cout << m_PropVector[index]->node->getPosition().positionKey << ": " << m_PropVector[index]->getFScore() << std::endl;
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

    MinHeap::MinHeap()
    {
    }

    MinHeap::~MinHeap()
    {
        for(AStarNodeProps* prop : m_PropVector)
        {
            delete prop;
        }
    }

    AStarNodeProps* MinHeap::top() const
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

    bool MinHeap::push(AStarNodeProps* props)
    {
        int index = find(props->node->getPosition().positionKey);

        if(index == -1)
        {
            m_PropVector.push_back(props);
            bubbleUp();
        }
        else
        {
            AStarNodeProps* curr = m_PropVector[index];

            if(curr->distanceG > props->distanceG)
            {
                // update optimized distanceG
                curr->distanceG = props->distanceG;

                bubbleUp(index);
            }
            else
            {
                return false;
            }
        }

        return true;
    }

    int MinHeap::find(int key) const 
    {
        for(int i = 0; i < m_PropVector.size(); i++)
        {
            if(m_PropVector[i]->node->getPosition().positionKey == key)
            {
                return i;
            }
        }

        return -1;
    }
    
    void MinHeap::swap(int index1, int index2)
    {
        AStarNodeProps* temp = m_PropVector[index1];
        m_PropVector[index1] = m_PropVector[index2];
        m_PropVector[index2] = temp;
    }

    void MinHeap::bubbleUp(int index)
    {
        if(index == -1)
        {
            index = m_PropVector.size() - 1;
        }

        while(hasParent(index) && (parent(index)->getFScore() > m_PropVector[index]->getFScore()))
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
            if(hasRightChild(index) && (rightChild(index)->getFScore() < leftChild(index)->getFScore()))
            {
                smallerChildIndex = getRightChildIndex(index);
            }

            //bubble down if child is less than or equal to parent, else end while loop.
            if(m_PropVector[index]->getFScore() >= m_PropVector[smallerChildIndex]->getFScore())
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