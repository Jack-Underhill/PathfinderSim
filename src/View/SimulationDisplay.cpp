#include "SimulationDisplay.h"

namespace PFSim {

    void SimulationDisplay::clearDisplay() 
    {
        m_Window->setColor(BACKGROUND_WINDOW_COLOR);
        m_Window->fillRect(DISPLAY_LEFT_BUFFER, DISPLAY_TOP_BUFFER, DISPLAY_SIZE, DISPLAY_SIZE);
    }
    
    void SimulationDisplay::updateResetMarkers(int x, int cellSize, int mazeLength)
    {
        int markerHeight = 13;

        // clear past markers
        m_Window->setColor(BACKGROUND_WINDOW_COLOR);
        //top
        m_Window->fillRect(DISPLAY_LEFT_BUFFER + (x - 2) * (cellSize + WALL_WIDTH) + WALL_WIDTH,
                        DISPLAY_TOP_BUFFER - markerHeight, cellSize, markerHeight);
        //bottom
        m_Window->fillRect(DISPLAY_LEFT_BUFFER + (x - 2) * (cellSize + WALL_WIDTH) + WALL_WIDTH,
                        DISPLAY_TOP_BUFFER + ((cellSize + WALL_WIDTH) * mazeLength) + WALL_WIDTH,
                        cellSize, markerHeight);

        // paint new markers
        m_Window->setColor("red");
        if(x <= mazeLength) {
            //top
            m_Window->fillRect(DISPLAY_LEFT_BUFFER + (x - 1) * (cellSize + WALL_WIDTH) + WALL_WIDTH,
                            DISPLAY_TOP_BUFFER - markerHeight, cellSize, markerHeight);
            //bottom
            m_Window->fillRect(DISPLAY_LEFT_BUFFER + (x - 1) * (cellSize + WALL_WIDTH) + WALL_WIDTH,
                            DISPLAY_TOP_BUFFER + ((cellSize + WALL_WIDTH) * mazeLength) + WALL_WIDTH,
                            cellSize, markerHeight);
        }
    }

    void SimulationDisplay::updateMazeNode(MazeNode*& node, int cellSize, bool isMazeGenerated) {
        m_Window->setColor(node->getColor());

        NodePosition currentPos = node->getPosition();

        int cellAndWallSize = cellSize + WALL_WIDTH;
        int xOfGraphRegion = WALL_WIDTH + ((currentPos.x - 1) * cellAndWallSize);
        int yOfGraphRegion = WALL_WIDTH + ((currentPos.y - 1) * cellAndWallSize);

        // fill cell
        m_Window->fillRect(DISPLAY_LEFT_BUFFER + xOfGraphRegion, DISPLAY_TOP_BUFFER + yOfGraphRegion,
                        cellSize, cellSize);

        if(isMazeGenerated) 
        {
            updateMazeNodeFiller(node, cellSize);
        }

        // Update Algorithm tick by 1 if the curr celltype is a default cell or a blank visited cell.
        // CellType type = node->getType();
        // if(type == DefaultCell || (type == Blank && !node->isNext())) 
        // {
        //     algorithmTick++;
        //     setCount(algorithmTick, true);
        // }
    }
    
    void SimulationDisplay::updatePathNode(MazeNode*& node, int cellSize)
    {
        NodePosition currentPos = node->getPosition();
        double reduceSizeBy = cellSize * .65;

        int cellAndWallSize = cellSize + WALL_WIDTH;
        int xOfGraphRegion = (currentPos.x - 1) * cellAndWallSize;
        int yOfGraphRegion = (currentPos.y - 1) * cellAndWallSize;

        m_Window->setColor(node->getColor());

        // connect between curr node and past node
        updatePathNodeFiller(node, cellSize);

        // fill cell
        m_Window->fillOval(DISPLAY_LEFT_BUFFER + xOfGraphRegion + WALL_WIDTH + (reduceSizeBy / 2), 
                           DISPLAY_TOP_BUFFER + yOfGraphRegion + WALL_WIDTH + (reduceSizeBy / 2),
                           cellSize - reduceSizeBy, cellSize - reduceSizeBy); 

        // // Update Path tick by 1 as long as the curr celltype is not a Start or an End cell.
        // CellType type = node->getType();
        // if(type == PathCell && !node->isNext()) {
        //     pathTick++;
        //     setCount(pathTick, false); 
        // }
    }
    
    void SimulationDisplay::updateMazeNodeFiller(MazeNode*& node, int cellSize)
    {
        NodePosition currentPos = node->getPosition();
    
        int cellAndWallSize = cellSize + WALL_WIDTH;
        int xOfGraphRegion = (currentPos.x - 1) * cellAndWallSize;
        int yOfGraphRegion = (currentPos.y - 1) * cellAndWallSize;

        // fill filler space (where a wall was) between cells. If movedIn = CENTER, this is skipped
        if(node->getDirectionMovedIn() == NORTH) 
        {    
            //south wall
            m_Window->fillRect(DISPLAY_LEFT_BUFFER + xOfGraphRegion + WALL_WIDTH,
                                DISPLAY_TOP_BUFFER + yOfGraphRegion + cellAndWallSize,
                                cellSize, WALL_WIDTH);
        }
        else if(node->getDirectionMovedIn() == WEST) 
        {    
            //east wall
            m_Window->fillRect(DISPLAY_LEFT_BUFFER + xOfGraphRegion + cellAndWallSize,
                                DISPLAY_TOP_BUFFER + yOfGraphRegion + WALL_WIDTH, 
                                WALL_WIDTH, cellSize);
        }
        else if(node->getDirectionMovedIn() == SOUTH) 
        {    
            //north wall
            m_Window->fillRect(DISPLAY_LEFT_BUFFER + xOfGraphRegion + WALL_WIDTH, 
                                DISPLAY_TOP_BUFFER + yOfGraphRegion,
                                cellSize, WALL_WIDTH);
        }
        else if(node->getDirectionMovedIn() == EAST) 
        {    
            //west wall
            m_Window->fillRect(DISPLAY_LEFT_BUFFER + xOfGraphRegion, 
                                DISPLAY_TOP_BUFFER + yOfGraphRegion + WALL_WIDTH, 
                                WALL_WIDTH, cellSize);
        }
    }

    void SimulationDisplay::updatePathNodeFiller(MazeNode*& node, int cellSize) 
    {
        NodePosition currentPos = node->getPosition();
        double reduceSizeBy = cellSize * .65;

        int cellAndWallSize = cellSize + WALL_WIDTH;
        int xOfGraphRegion = (currentPos.x - 1) * cellAndWallSize;
        int yOfGraphRegion = (currentPos.y - 1) * cellAndWallSize;

        if(node != nullptr && node->getType() != StartCell) 
        {
            // fill filler space (where a wall was) between cells. If movedIn = CENTER, this is skipped
            if(node->getDirectionMovedIn() == NORTH) 
            {
                //south wall
                m_Window->fillRect(DISPLAY_LEFT_BUFFER + xOfGraphRegion + (WALL_WIDTH / 2) + (reduceSizeBy / 2),
                                DISPLAY_TOP_BUFFER + yOfGraphRegion + (WALL_WIDTH / 2) + (cellSize / 2),
                                cellSize - reduceSizeBy + (WALL_WIDTH / 2), cellAndWallSize);
            }
            else if(node->getDirectionMovedIn() == WEST) 
            {    
                //east wall
                m_Window->fillRect(DISPLAY_LEFT_BUFFER + xOfGraphRegion + (WALL_WIDTH / 2) + (cellSize / 2),
                                DISPLAY_TOP_BUFFER + yOfGraphRegion + (WALL_WIDTH / 2) + (reduceSizeBy / 2),
                                cellAndWallSize, cellSize - reduceSizeBy + (WALL_WIDTH / 2));
            }
            else if(node->getDirectionMovedIn() == SOUTH) 
            {    
                //north wall
                m_Window->fillRect(DISPLAY_LEFT_BUFFER + xOfGraphRegion + (WALL_WIDTH / 2) + (reduceSizeBy / 2), 
                                DISPLAY_TOP_BUFFER + yOfGraphRegion - (cellSize / 2),
                                cellSize - reduceSizeBy + (WALL_WIDTH / 2), cellAndWallSize);
            }
            else if(node->getDirectionMovedIn() == EAST) 
            {    
                //west wall
                m_Window->fillRect(DISPLAY_LEFT_BUFFER + xOfGraphRegion - (cellSize / 2),
                                DISPLAY_TOP_BUFFER + yOfGraphRegion + (WALL_WIDTH / 2) + (reduceSizeBy / 2),
                                cellAndWallSize, cellSize - reduceSizeBy + (WALL_WIDTH / 2));
            }   
        }
    }

}