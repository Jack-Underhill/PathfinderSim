/* 
 * Jack Underhill
 * Summer 2022
 * Maze Generating and Pathfinding Simulator
 * 
 * Generator--Prims
 * 
 * This generating algorithm links every node in the given graph into a minimum spanning tree.
 * The links are undirected.
 * 
 * This implementation doesn't have a weighted graph, so instead of expanding to the minimum
 * weighted node, randomness is used for expanding the tree. A random unopened-node from the
 * fringe(nodes that are adjacent to the perimeter of the tree outwardly) is chosen to link to its
 * adjacent closed node. If there are multiple neighboring/adjacent closed nodes then one of them
 * is chosen at random. This process continues until every node in the graph has been linked and 
 * closed.
 */

#ifndef _Prims_h_
#define _Prims_h_

#include <vector>
#include <stack>

#include "GeneratorTemplate.h"
#include "MazeNode.h"

namespace PFSim {
namespace Generator {
    
    class Prims : public GeneratorTemplate
    {
    public:
        //Takes the given graph to store the starting node into currNodeList for this generating 
        //algorithm.
        Prims(MazeGraph*& graph);

        //Returns the animation's title.
        std::string getTitle() const { return "Prim's Maze"; }

        //Returns the generator type.
        GeneratorType getGeneratorType() const { return GeneratorType::Prims; }

        //Handles and returns a node from nextNodeList if nextNodeList isn't empty, else handles 
        //and returns a node from currNodeList. If both lists are empty then Prims is marked 
        //complete.
        int step();

    private:
        std::vector<int> currNodeList;
        std::stack<int> nextNodeList;

        //Returns the top node's position key from the nextNodeList and removes it from the list.
        int handleNextNode();

        //Returns a randomly chosen node from the currNodeList after handling it and removing it 
        //from the list.
        int handleCurrNode();

        //The given node's NWSE neighboring nodes are opened if they are valid and in bounds.
        void openNeighbors(MazeNode*& node, int nodePositionKey);

        //The node of the given key is opened if it has not been processed already. 
        //Opening a node consists of: setting it to visited, and adding it to both currNodeList 
        //and nextNodeList.
        void openNode(int positionKey);

        //Returns a list of the NWSE neighboring nodes, of the given node/positionKey, that're
        //closed, valid and in bounds.
        std::vector<DirectionMoved> getClosedNeighbors(MazeNode*& node, int nodePositionKey) const;

        //Only if the given neighbors list isn't empty:
        //The given node is linked to a random node from the given neighbors list. The given node's
        //DirectionMovedIn is set towards its freshly linked neighbor, and then the given node is
        //set to a BlankCell.
        //
        //The given node is then always set to closed.
        void updateValues(std::vector<DirectionMoved>& neighbors, MazeNode*& node) const;

        //Returns true and is valid if the given node position key is inside the bounds of the 
        //graph.
        bool isVerticallyValid(int key) const;

        //Returns true and is valid if the given node position key is inside the bounds of the 
        //graph and is on the same row of the given prevNode.
        bool isHorizontallyValid(int newKey, MazeNode*& prevNode) const;
        
        //The node of the given key is considered closed if it's cell type is no longer a 
        //GenerationCell; if so then this function returns true.
        bool isClosed(int key) const;
    };

} // namespace Generator
} // namespace PFSim

#endif