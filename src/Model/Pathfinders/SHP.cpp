#include "SHP.h"

namespace PFSim {
namespace Pathfinder {    

    Permutations::Permutations(int* targets, int size)
    {
        std::vector<int> v;
        for(int i = 0; i < size; i++)
        {
            v.push_back(*(targets + i));
        }
        
        m_Sequence = new std::stack<Sequence*>();
        permutate({}, v, m_Sequence);
    }

    Permutations::~Permutations()
    {
        while(!m_Sequence->empty())
        {
            delete m_Sequence->top();
            m_Sequence->pop();
        }
    }
    
    void Permutations::permutate(std::vector<int> chosen, std::vector<int> remaining, std::stack<Sequence*>* result)
    {
        if(remaining.size() == 0 && chosen.size() > 0)
        {
            std::stack<int>* s = new std::stack<int>();
            for(int i = 0; i < chosen.size(); i++)
            {
                s->push(chosen[i]);
            }

            result->push(new Sequence(s));
        }
        else
        {
            for(int i = 0; i < remaining.size(); i++)
            {
                //choose
                chosen.push_back(remaining[i]);
                remaining.erase(remaining.begin() + i);
                //explore
                permutate(chosen, remaining, result);
                //unchoose
                remaining.insert(remaining.begin() + i, chosen[chosen.size() - 1]);
                chosen.pop_back();
            }
        }
    }
    
    /*------------------------------------------SHP Heap-----------------------------------------*/
    
    bool SHPMinHeap::push(HeapProp* props)
    {
        m_PropVector.push_back(props);
        bubbleUp();

        return true;
    }

    /*--------------------------------------------SHP_BFS----------------------------------------*/
    
    SHP_BFS::SHP_BFS(MazeGraph*& graph, int target) : BFS(graph) 
    {
        m_Target = target;
    }

    int SHP_BFS::currStep()
    {
        MazeNode* currNode = BFS::getCurrentNodeStep();

        if(currNode->getPosition().positionKey == m_Target) //found
        {
            finalizePathfinder(currNode);
        }
        else //keep searching
        {
            BFS::updatePathfinderStep(currNode);
        }

        return currNode->getPosition().positionKey;
    }
    
    /*-------------------------------------------SHP_AStar---------------------------------------*/

    SHP_AStar::SHP_AStar(MazeGraph*& graph, int target) : AStar(graph) 
    {
        m_Target = target;
    }

    int SHP_AStar::currStep()
    {
        MazeNode* currNode = AStar::getCurrentNodeStep();

        if(currNode->getPosition().positionKey == m_Target) //found
        {
            finalizePathfinder(currNode);
        }
        else //keep searching
        {
            AStar::updatePathfinderStep(currNode);
        }

        return currNode->getPosition().positionKey;
    }
    
} // namespace Pathfinder
} // namespace PFSim