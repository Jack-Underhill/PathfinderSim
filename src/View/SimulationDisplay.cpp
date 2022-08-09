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
        int xOfSimDisplay = WALL_WIDTH + ((currentPos.x - 1) * cellAndWallSize);
        int yOfSimDisplay = WALL_WIDTH + ((currentPos.y - 1) * cellAndWallSize);

        // fill cell
        m_Window->fillRect(DISPLAY_LEFT_BUFFER + xOfSimDisplay, DISPLAY_TOP_BUFFER + yOfSimDisplay,
                        cellSize, cellSize);

        if(isMazeGenerated) 
        {
            updateMazeNodeFiller(node, cellSize);
        }
    }
    
    void SimulationDisplay::updatePathNode(MazeNode*& node, int cellSize)
    {
        NodePosition currentPos = node->getPosition();

        int pathSize = cellSize * .45;
        int centeringInsideCell = ((cellSize / 2) - (pathSize / 2));

        int cellAndWallSize = cellSize + WALL_WIDTH;
        int xOfSimDisplay = WALL_WIDTH + ((currentPos.x - 1) * cellAndWallSize) + centeringInsideCell;
        int yOfSimDisplay = WALL_WIDTH + ((currentPos.y - 1) * cellAndWallSize) + centeringInsideCell;

        m_Window->setColor(node->getColor());

        // connect between curr node and past node
        updatePathNodeFiller(node, cellSize);

        // fill cell
        m_Window->fillOval(DISPLAY_LEFT_BUFFER + xOfSimDisplay, 
                           DISPLAY_TOP_BUFFER + yOfSimDisplay,
                           pathSize, pathSize); 
    }
    
    void SimulationDisplay::updateMazeNodeFiller(MazeNode*& node, int cellSize)
    {
        NodePosition currentPos = node->getPosition();
    
        int cellAndWallSize = cellSize + WALL_WIDTH;
        int xOfSimDisplay = (currentPos.x - 1) * cellAndWallSize;
        int yOfSimDisplay = (currentPos.y - 1) * cellAndWallSize;

        // filler space (where a wall was) between cells. If movedIn = CENTER, this is skipped
        if(node->getDirectionMovedIn() == NORTH) 
        {    
            //south wall fill
            m_Window->fillRect(DISPLAY_LEFT_BUFFER + xOfSimDisplay + WALL_WIDTH,
                                DISPLAY_TOP_BUFFER + yOfSimDisplay + cellAndWallSize,
                                cellSize, WALL_WIDTH);
        }
        else if(node->getDirectionMovedIn() == WEST) 
        {    
            //east wall fill
            m_Window->fillRect(DISPLAY_LEFT_BUFFER + xOfSimDisplay + cellAndWallSize,
                                DISPLAY_TOP_BUFFER + yOfSimDisplay + WALL_WIDTH, 
                                WALL_WIDTH, cellSize);
        }
        else if(node->getDirectionMovedIn() == SOUTH) 
        {    
            //north wall fill
            m_Window->fillRect(DISPLAY_LEFT_BUFFER + xOfSimDisplay + WALL_WIDTH, 
                                DISPLAY_TOP_BUFFER + yOfSimDisplay,
                                cellSize, WALL_WIDTH);
        }
        else if(node->getDirectionMovedIn() == EAST) 
        {    
            //west wall fill
            m_Window->fillRect(DISPLAY_LEFT_BUFFER + xOfSimDisplay, 
                                DISPLAY_TOP_BUFFER + yOfSimDisplay + WALL_WIDTH, 
                                WALL_WIDTH, cellSize);
        }
    }

    void SimulationDisplay::updatePathNodeFiller(MazeNode*& node, int cellSize) 
    {
        NodePosition currentPos = node->getPosition();

        int pathSize = (cellSize * .45);
        int centeringInsideCell = ((cellSize / 2) - (pathSize / 2));

        int cellAndWallSize = cellSize + WALL_WIDTH;
        int xOfSimDisplay = WALL_WIDTH + ((currentPos.x - 1) * cellAndWallSize) + centeringInsideCell;
        int yOfSimDisplay = WALL_WIDTH + ((currentPos.y - 1) * cellAndWallSize) + centeringInsideCell;

        pathSize += 2;

        if(node != nullptr) 
        {
            // filler space (where a wall was) between cells. If movedIn = CENTER, this is skipped
            if(node->getDirectionMovedIn() == NORTH) 
            {
                //south wall fill
                m_Window->fillRect(DISPLAY_LEFT_BUFFER + xOfSimDisplay - 1,
                                   DISPLAY_TOP_BUFFER + yOfSimDisplay + (pathSize / 2) - 1, 
                                   pathSize, cellAndWallSize);
            }
            else if(node->getDirectionMovedIn() == WEST) 
            {    
                //east wall fill
                m_Window->fillRect(DISPLAY_LEFT_BUFFER + xOfSimDisplay + (pathSize / 2) - 1,
                                   DISPLAY_TOP_BUFFER + yOfSimDisplay - 1, 
                                   cellAndWallSize, pathSize);
            }
            else if(node->getDirectionMovedIn() == SOUTH) 
            {    
                //north wall fill
                m_Window->fillRect(DISPLAY_LEFT_BUFFER + xOfSimDisplay - 1,
                                   DISPLAY_TOP_BUFFER + yOfSimDisplay - (cellAndWallSize - (pathSize / 2)) - 1, 
                                   pathSize, cellAndWallSize);
            }
            else if(node->getDirectionMovedIn() == EAST) 
            {    
                //west wall fill
                m_Window->fillRect(DISPLAY_LEFT_BUFFER + xOfSimDisplay - (cellAndWallSize - (pathSize / 2)) - 1,
                                   DISPLAY_TOP_BUFFER + yOfSimDisplay - 1, 
                                   cellAndWallSize, pathSize);
            }   
        }
    }

} // namespace PFSim