#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <ctime>
#include <windows.h>

using namespace sf;

float cellSize;
const int size = 10;
const int numBombs = 10;
int win = 0;
bool islost = 0;
int face = 1;
bool firstHit = 1;

Texture border;
Texture revealed;
Texture hidden;
Texture mine;
Texture flag;
Texture one;
Texture two;
Texture three;
Texture four;
Texture five;
Texture six;
Texture seven;
Texture eight;
Texture face_happy;
Texture face_lose;
Texture face_win;

int Grid[size + 2][size + 2];
int RevealGrid[size + 2][size + 2];

void drawGrid(RenderWindow& window, int grid[size + 2][size + 2], int revealgrid[size + 2][size + 2], int openCells, int Vertres) {
    Sprite cell;
    cell.setPosition(sf::Vector2f(cellSize, cellSize));
    cell.setScale(cellSize/32, cellSize/32);

    for (int x = 0; x < size + 2; x++) {
        for (int y = 0; y < size + 2; y++) {
            cell.setPosition(x * cellSize, y * cellSize);
            if (Grid[x][y] == -3) {
                cell.setTexture(border);
            }
            else if (Grid[x][y] == -4 && islost != 1 && (win != numBombs || openCells != (size * size - numBombs))) {
                cell.setTexture(face_happy);
            }
            else if (Grid[x][y] == -4 && win == numBombs && islost != 1 && openCells == (size * size - numBombs)) {
                cell.setTexture(face_win);
            }
            else if (Grid[x][y] == -4 && win != numBombs && islost == 1) {
                cell.setTexture(face_lose);
            }
            else if ((Grid[x][y] == -2 || Grid[x][y] == 0) && revealgrid[x][y] == 1) {
                cell.setTexture(revealed);
            }
            else if ((Grid[x][y] == -1) && revealgrid[x][y] == 1) {
                cell.setTexture(mine);
            }
            else if (revealgrid[x][y] == 0 && (Grid[x][y] == -1 
                                            || Grid[x][y] == 1 
                                            || Grid[x][y] == 2 
                                            || Grid[x][y] == 3 
                                            || Grid[x][y] == 4 
                                            || Grid[x][y] == 5 
                                            || Grid[x][y] == 6 
                                            || Grid[x][y] == 7 
                                            || Grid[x][y] == 8
                                            || Grid[x][y] == 0 
                                            || Grid[x][y] == -2)) {
                cell.setTexture(hidden);
            }
            else if (Grid[x][y] == 1 && revealgrid[x][y] == 1) {
                cell.setTexture(one);
            }
            else if (Grid[x][y] == 2 && revealgrid[x][y] == 1) {
                cell.setTexture(two);
            }
            else if (Grid[x][y] == 3 && revealgrid[x][y] == 1) {
                cell.setTexture(three);
            }
            else if (Grid[x][y] == 4 && revealgrid[x][y] == 1) {
                cell.setTexture(four);
            }
            else if (Grid[x][y] == 5 && revealgrid[x][y] == 1) {
                cell.setTexture(five);
            }
            else if (Grid[x][y] == 6 && revealgrid[x][y] == 1) {
                cell.setTexture(six);
            }
            else if (Grid[x][y] == 7 && revealgrid[x][y] == 1) {
                cell.setTexture(seven);
            }
            else if (Grid[x][y] == 8 && revealgrid[x][y] == 1) {
                cell.setTexture(eight);
            }
            else if (revealgrid[x][y] == 2) {
                cell.setTexture(flag);
            }
            window.draw(cell);
        }
    }
}

void Bomb_placement(int Grid[size + 2][size + 2],int size, int numBombs) {
    
    int bombsPlaced = 0;
    while (bombsPlaced < numBombs) {
        int row = rand() % size + 1;
        int col = rand() % size + 1;

        
        if (Grid[row][col] == -1) {
            continue;
        }

        
        Grid[row][col] = -1;
        bombsPlaced++;
    }

    
    for (int row = 1; row < size + 1; row++) {
        for (int col = 1; col < size + 1; col++) {
            if (Grid[row][col] == -1 && Grid[row][col] != -3) {
                continue;
            }
            int numAdjacentBombs = 0;
            if (row > 1 && col > 1 && Grid[row - 1][col - 1] == -1) {
                numAdjacentBombs++;
            }
            if (row > 1 && Grid[row - 1][col] == -1) {
                numAdjacentBombs++;
            }
            if (row > 1 && col < size + 1 && Grid[row - 1][col + 1] == -1) {
                numAdjacentBombs++;
            }
            if (col > 1 && Grid[row][col - 1] == -1) {
                numAdjacentBombs++;
            }
            if (col < size + 1 && Grid[row][col + 1] == -1) {
                numAdjacentBombs++;
            }
            if (row < size + 1 && col > 0 && Grid[row + 1][col - 1] == -1) {
                numAdjacentBombs++;
            }
            if (row < size + 1 && Grid[row + 1][col] == -1) {
                numAdjacentBombs++;
            }
            if (row < size + 1 && col < size + 1 && Grid[row + 1][col + 1] == -1) {
                numAdjacentBombs++;
            }
            Grid[row][col] = numAdjacentBombs;
        }
    }
}

int main() {

    HDC hDCScreen = GetDC(NULL);
    int Vertres = GetDeviceCaps(hDCScreen, VERTRES);
    ReleaseDC(NULL, hDCScreen);

    cellSize = Vertres / 33.75;

    RenderWindow window(VideoMode((size + 2) * cellSize, (size + 2) * cellSize), "Minesweeper", Style::Titlebar | Style::Close);
    window.setFramerateLimit(60);


    //Texture one;
    one.loadFromFile("../images/number_1.png");
    one.setSmooth(true);

    //Texture two;
    two.loadFromFile("../images/number_2.png");
    two.setSmooth(true);

    //Texture three;
    three.loadFromFile("../images/number_3.png");
    three.setSmooth(true);

    //Texture four;
    four.loadFromFile("../images/number_4.png");
    four.setSmooth(true);

    //Texture five;
    five.loadFromFile("../images/number_5.png");
    five.setSmooth(true);

    //Texture six;
    six.loadFromFile("../images/number_6.png");
    six.setSmooth(true);

    //Texture seven;
    seven.loadFromFile("../images/number_7.png");
    seven.setSmooth(true);

    //Texture eight;
    eight.loadFromFile("../images/number_8.png");
    eight.setSmooth(true);

    //Texture hidden;
    hidden.loadFromFile("../images/tile_hidden.png");
    hidden.setSmooth(true);

    //Texture revealed;
    revealed.loadFromFile("../images/tile_revealed.png");
    revealed.setSmooth(true);

    //Texture mine;
    mine.loadFromFile("../images/mine.png");
    mine.setSmooth(true);

    //Texture flag;
    flag.loadFromFile("../images/flag.png");
    flag.setSmooth(true);

    //Texture border;
    border.loadFromFile("../images/border.png");
    border.setSmooth(true);

    //Texture face_happy;
    face_happy.loadFromFile("../images/face_happy.png");
    face_happy.setSmooth(true);

    //Texture face_lose;
    face_lose.loadFromFile("../images/face_lose.png");
    face_lose.setSmooth(true);

    //Texture face_win;
    face_win.loadFromFile("../images/face_win.png");
    face_win.setSmooth(true);


    for (int x = 1; x < size + 1; x++) {
        for (int y = 1; y < size + 1; y++) {
            Grid[x][y] = 0;
        }
    }
    for (int x = 0; x < size + 2; x++) {
        Grid[x][0] = -3;
        Grid[x][size + 1] = -3;
    }
    for (int y = 0; y < size + 2; y++) {
        Grid[0][y] = -3;
        Grid[size + 1][y] = -3;
    }
    for (int x = 1; x < size + 1; x++) {
        for (int y = 1; y < size + 1; y++) {
            RevealGrid[x][y] = 0;
        }
    }

    Grid[0][0] = -4;

    
    srand(time(NULL));

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
            if (event.type == Event::MouseButtonPressed) {
                if (islost == 0) {
                    if (event.mouseButton.button == Mouse::Left && event.mouseButton.x >= cellSize && event.mouseButton.x < (size + 1) * cellSize && event.mouseButton.y >= cellSize && event.mouseButton.y < (size + 1) * cellSize) {
                        int x = event.mouseButton.x / cellSize;
                        int y = event.mouseButton.y / cellSize;

                        if (firstHit == 1) {
                            Bomb_placement(Grid, size, numBombs);
                            while (Grid[x][y] == -1 || Grid[x][y] == 1 || Grid[x][y] == 2 || Grid[x][y] == 3 || Grid[x][y] == 4 || Grid[x][y] == 5 || Grid[x][y] == 6 || Grid[x][y] == 7 || Grid[x][y] == 8) {
                                for (int x = 1; x < size + 1; x++) {
                                    for (int y = 1; y < size + 1; y++) {
                                        Grid[x][y] = -2;
                                    }
                                }
                                Bomb_placement(Grid, size, numBombs);
                            }
                            firstHit = 0;
                        }

                        if ((Grid[x][y] == 1
                            || Grid[x][y] == 2
                            || Grid[x][y] == 3
                            || Grid[x][y] == 4
                            || Grid[x][y] == 5
                            || Grid[x][y] == 6
                            || Grid[x][y] == 7
                            || Grid[x][y] == 8) 
                            && RevealGrid[x][y] == 0) {
                            RevealGrid[x][y] = 1;
                        }
                        else if ((Grid[x][y] == 0
                            || Grid[x][y] == -2) 
                            && RevealGrid[x][y] == 0) {
                            RevealGrid[x][y] = 1;

                        }
                        else if (Grid[x][y] == -1 && RevealGrid[x][y] == 0) {
                            RevealGrid[x][y] = 1;
                            islost = 1;
                        }

                        else if (Grid[x][y] == -3) {
                            Grid[x][y] = -3;
                        }
                        else if (Grid[x][y] == 1 && RevealGrid[x][y] == 1) {
                            Grid[x][y] = 1;
                        }
                        else if (Grid[x][y] == 2 && RevealGrid[x][y] == 1) {
                            Grid[x][y] = 2;
                        }
                        else if (Grid[x][y] == 3 && RevealGrid[x][y] == 1) {
                            Grid[x][y] = 3;
                        }
                        else if (Grid[x][y] == 4 && RevealGrid[x][y] == 1) {
                            Grid[x][y] = 4;
                        }
                        else if (Grid[x][y] == 5 && RevealGrid[x][y] == 1) {
                            Grid[x][y] = 5;
                        }
                        else if (Grid[x][y] == 6 && RevealGrid[x][y] == 1) {
                            Grid[x][y] = 6;
                        }
                        else if (Grid[x][y] == 7 && RevealGrid[x][y] == 1) {
                            Grid[x][y] = 7;
                        }
                        else if (Grid[x][y] == 8 && RevealGrid[x][y] == 1) {
                            Grid[x][y] = 8;
                        }
                        else if (Grid[x][y] == -1 && RevealGrid[x][y] == 1) {
                            Grid[x][y] = -1;
                        }
                        else if (RevealGrid[x][y] == 2) {
                            Grid[x][y] = -1;
                        }
                    }
                    else if (event.mouseButton.button == Mouse::Right) {
                        int x = event.mouseButton.x / cellSize;
                        int y = event.mouseButton.y / cellSize;
                        if (Grid[x][y] == -1 && RevealGrid[x][y] == 0) {
                            win++;
                        }
                        else if (Grid[x][y] == -1 && RevealGrid[x][y] == 2) {
                            win--;
                        }
                        else if (Grid[x][y] != -1 && RevealGrid[x][y] == 2) {
                            win++;
                        }
                        else if (Grid[x][y] != -1 && RevealGrid[x][y] == 0) {
                            win--;
                        }
                        if (RevealGrid[x][y] == 0) {
                            RevealGrid[x][y] = 2;
                        }
                        else if (RevealGrid[x][y] == 2) {
                            RevealGrid[x][y] = 0;
                        }
                    }
                }

                else if (islost == 1) {
                    if (event.type == Event::MouseButtonPressed) {
                        if (event.mouseButton.x >= 0 && event.mouseButton.x <= cellSize && event.mouseButton.y >= 0 && event.mouseButton.y <= cellSize) {
                            for (int x = 1; x < size + 1; x++) {
                                for (int y = 1; y < size + 1; y++) {
                                    Grid[x][y] = 0;
                                }
                            }
                            for (int x = 0; x < size + 2; x++) {
                                Grid[x][0] = -3;
                                Grid[x][size + 1] = -3;
                            }
                            for (int y = 0; y < size + 2; y++) {
                                Grid[0][y] = -3;
                                Grid[size + 1][y] = -3;
                            }
                            for (int x = 1; x < size + 1; x++) {
                                for (int y = 1; y < size + 1; y++) {
                                    RevealGrid[x][y] = 0;
                                }
                            }

                            Grid[0][0] = -4;
                            srand(time(NULL));
                            islost = 0;
                            win = 0;
                            firstHit = 1;
                        }
                    }
                }
            }
        }

        for (int i = 0; i < size * size; i++) {
            for (int x = 1; x < size + 1; x++) {
                for (int y = 1; y < size + 1; y++) {
                    if ((Grid[x][y] == 0 || Grid[x][y] == -2) && RevealGrid[x][y] == 1) {

                        if (Grid[x + 1][y] != -3 && Grid[x + 1][y] != -4 && Grid[x + 1][y] != -1 && (x + 1) >= 1 && (x + 1) < size + 1 && y >= 1 && y < size + 1 && RevealGrid[x + 1][y] != 1) {
                            RevealGrid[x + 1][y] = 1;
                        }
                        if (Grid[x - 1][y] != -3 && Grid[x - 1][y] != -4 && Grid[x - 1][y] != -1 && (x - 1) >= 1 && (x - 1) < size + 1 && y >= 1 && y < size + 1 && RevealGrid[x - 1][y] != 1) {
                            RevealGrid[x - 1][y] = 1;
                        }
                        if (Grid[x][y + 1] != -3 && Grid[x][y + 1] != -4 && Grid[x][y + 1] != -1 && x >= 1 && x < size + 1 && (y + 1) >= 1 && (y + 1) < size + 1 && RevealGrid[x][y + 1] != 1) {
                            RevealGrid[x][y + 1] = 1;
                        }
                        if (Grid[x][y - 1] != -3 && Grid[x][y - 1] != -4 && Grid[x][y - 1] != -1 && x >= 1 && x < size + 1 && (y - 1) >= 1 && (y - 1) < size + 1 && RevealGrid[x][y - 1] != 1) {
                            RevealGrid[x][y - 1] = 1;
                        }
                    }
                }
            }
        }

        int openCells = 0;
        for (int x = 1; x < size + 1; x++) {
            for (int y = 1; y < size + 1; y++) {
                if (RevealGrid[x][y] == 1) {
                    openCells++;
                }
            }
        }

        window.clear(Color::White);

        drawGrid(window, Grid, RevealGrid, openCells, Vertres);

        window.display();

        if (win == size && (openCells == (size*size-numBombs))) {
            if (event.type == Event::MouseButtonPressed) {
                if (event.mouseButton.x >= 0 && event.mouseButton.x <= cellSize && event.mouseButton.y >= 0 && event.mouseButton.y <= cellSize) {
                    for (int x = 1; x < size + 1; x++) {
                        for (int y = 1; y < size + 1; y++) {
                            Grid[x][y] = 0;
                        }
                    }
                    for (int x = 0; x < size + 2; x++) {
                        Grid[x][0] = -3;
                        Grid[x][size + 1] = -3;
                    }
                    for (int y = 0; y < size + 2; y++) {
                        Grid[0][y] = -3;
                        Grid[size + 1][y] = -3;
                    }
                    for (int x = 1; x < size + 1; x++) {
                        for (int y = 1; y < size + 1; y++) {
                            RevealGrid[x][y] = 0;
                        }
                    }

                    Grid[0][0] = -4;
                    srand(time(NULL));
                    win = 0;
                    firstHit = 1;
                }
            }
        }

    }

    return 0;
}