﻿#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <ctime>

using namespace sf;

const int cellSize = 32; // ðàçìåð ÿ÷ååê
const int size = 10; // ðàçìåð ïîëÿ
const int numBombs = 10;
int win = 0;

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

int Grid[size + 2][size + 2]; // èãðîâîå ïîëå êîìïüþòåðà
bool RevealGrid[size + 2][size + 2];

void drawGrid(RenderWindow& window, int grid[size + 2][size + 2], bool revealgrid[size + 2][size + 2]) {
    Sprite cell;
    cell.setPosition(sf::Vector2f(cellSize, cellSize));

    for (int x = 0; x < size + 2; x++) {
        for (int y = 0; y < size + 2; y++) {
            cell.setPosition(x * cellSize, y * cellSize);
            if (Grid[x][y] == -3) { // ïîïàäàíèå
                cell.setTexture(hidden);
            }
            else if ((Grid[x][y] == -2 || Grid[x][y] == 0) && revealgrid[x][y] == 1) { // ïîïàäàíèå
                cell.setTexture(revealed);
            }
            else if ((Grid[x][y] == -1) && revealgrid[x][y] == 1) { // ïîïàäàíèå
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
                                            || Grid[x][y] == -2)) { // ïîïàäàíèå
                cell.setTexture(hidden);
            }
            else if (Grid[x][y] == 1 && revealgrid[x][y] == 1) { // ïàëóáà êîðàáëÿ
                cell.setTexture(one);
            }
            else if (Grid[x][y] == 2 && revealgrid[x][y] == 1) { // ïðîìàõ
                cell.setTexture(two);
            }
            else if (Grid[x][y] == 3 && revealgrid[x][y] == 1) { // ïðîìàõ
                cell.setTexture(three);
            }
            else if (Grid[x][y] == 4 && revealgrid[x][y] == 1) { // ïðîìàõ
                cell.setTexture(four);
            }
            else if (Grid[x][y] == 5 && revealgrid[x][y] == 1) { // ïðîìàõ
                cell.setTexture(five);
            }
            else if (Grid[x][y] == 6 && revealgrid[x][y] == 1) { // ïðîìàõ
                cell.setTexture(six);
            }
            else if (Grid[x][y] == 7 && revealgrid[x][y] == 1) { // ïðîìàõ
                cell.setTexture(seven);
            }
            else if (Grid[x][y] == 8 && revealgrid[x][y] == 1) { // ïðîìàõ
                cell.setTexture(eight);
            }
            window.draw(cell);
        }
    }
}

void Bomb_placement(int Grid[size + 2][size + 2],int size, int numBombs) {
    // Устанавливаем numBombs бомб на доске
    int bombsPlaced = 0;
    while (bombsPlaced < numBombs) {
        int row = rand() % size + 1;
        int col = rand() % size + 1;

        // Если в ячейке уже есть бомба, то продолжаем генерировать случайные координаты
        if (Grid[row][col] == -1) {
            continue;
        }

        // Устанавливаем бомбу в ячейке
        Grid[row][col] = -1;
        bombsPlaced++;
    }

    // Заполняем оставшиеся ячейки значениями, указывающими на количество бомб в соседних клетках
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

//bool isClear(int x, int y, int Grid[size + 2][size + 2], int size) {
//    if (Grid[x + 1][y] != 1) {
//        if (Grid[x + 1][y] == 3 || Grid[x + 1][y] == 4) {
//            if (Grid[x + 2][y] != 1) {
//                if (Grid[x + 2][y] == 3 || Grid[x + 2][y] == 4) {
//                    if (Grid[x + 3][y] != 1) {
//                        if (Grid[x + 3][y] == 3 || Grid[x + 3][y] == 4) {
//                            return true;
//                        }
//                    }
//                    else if (Grid[x + 3][y] == 1) {
//                        return false;
//                    }
//                }
//            }
//            else if (Grid[x + 2][y] == 1) {
//                return false;
//            }
//        }
//    }
//    else if (Grid[x + 1][y] == 1) {
//        return false;
//    }
//    if (Grid[x - 1][y] != 1) {
//        if (Grid[x - 1][y] == 3 || Grid[x - 1][y] == 4) {
//            if (Grid[x - 2][y] != 1) {
//                if (Grid[x - 2][y] == 3 || Grid[x - 2][y] == 4) {
//                    if (Grid[x - 3][y] != 1) {
//                        if (Grid[x - 3][y] == 3 || Grid[x - 3][y] == 4) {
//                            return true;
//                        }
//                    }
//                    else if (Grid[x - 3][y] == 1) {
//                        return false;
//                    }
//                }
//            }
//            else if (Grid[x - 2][y] == 1) {
//                return false;
//            }
//        }
//    }
//    else if (Grid[x - 1][y] == 1) {
//        return false;
//    }
//    if (Grid[x][y + 1] != 1) {
//        if (Grid[x][y + 1] == 3 || Grid[x][y + 1] == 4) {
//            if (Grid[x][y + 2] != 1) {
//                if (Grid[x][y + 2] == 3 || Grid[x][y + 2] == 4) {
//                    if (Grid[x][y + 3] != 1) {
//                        if (Grid[x][y + 3] == 3 || Grid[x][y + 3] == 4) {
//                            return true;
//                        }
//                    }
//                    else if (Grid[x][y + 3] == 1) {
//                        return false;
//                    }
//                }
//            }
//            else if (Grid[x][y + 2] == 1) {
//                return false;
//            }
//        }
//    }
//    else if (Grid[x][y + 1] == 1) {
//        return false;
//    }
//    if (Grid[x][y - 1] != 1) {
//        if (Grid[x][y - 1] == 3 || Grid[x][y - 1] == 4) {
//            if (Grid[x][y - 2] != 1) {
//                if (Grid[x][y - 2] == 3 || Grid[x][y - 2] == 4) {
//                    if (Grid[x][y - 3] != 1) {
//                        if (Grid[x][y - 3] == 3 || Grid[x][y - 3] == 4) {
//                            return true;
//                        }
//                    }
//                    else if (Grid[x][y - 3] == 1) {
//                        return false;
//                    }
//                }
//            }
//            else if (Grid[x][y - 2] == 1) {
//                return false;
//            }
//        }
//    }
//    else if (Grid[x][y - 1] == 1) {
//        return false;
//    }
//    return true;
//}

int main() {
    RenderWindow window(VideoMode((size + 2) * cellSize, (size + 2) * cellSize), "Minesweeper", Style::Titlebar | Style::Close);
    window.setFramerateLimit(60);


    //Texture one;
    one.loadFromFile("../images/number_1.png");

    //Texture two;
    two.loadFromFile("../images/number_2.png");

    //Texture three;
    three.loadFromFile("../images/number_3.png");

    //Texture four;
    four.loadFromFile("../images/number_4.png");

    //Texture five;
    five.loadFromFile("../images/number_5.png");

    //Texture six;
    six.loadFromFile("../images/number_6.png");

    //Texture seven;
    seven.loadFromFile("../images/number_7.png");

    //Texture eight;
    eight.loadFromFile("../images/number_8.png");

    //Texture hidden;
    hidden.loadFromFile("../images/tile_hidden.png");

    //Texture revealed;
    revealed.loadFromFile("../images/tile_revealed.png");

    //Texture mine;
    mine.loadFromFile("../images/mine.png");

    //Texture flag;
    flag.loadFromFile("../images/flag.png");



    // èíèöèàëèçèðóåì ïîëå èãðîêà è êîìïüþòåðà
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

    // ðàíäîìíî ðàçìåùàåì êîðàáëè êîìïüþòåðà
    srand(time(NULL));

    Bomb_placement(Grid, size, numBombs);

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
            if (event.type == Event::MouseButtonPressed) { // åñëè ñåé÷àñ õîä èãðîêà è îí íàæàë íà ÿ÷åéêó
                int x = event.mouseButton.x / cellSize;
                int y = event.mouseButton.y / cellSize;

                if ((Grid[x][y] == 0
                    || Grid[x][y] == -2
                    || Grid[x][y] == 1
                    || Grid[x][y] == 2
                    || Grid[x][y] == 3
                    || Grid[x][y] == 4
                    || Grid[x][y] == 5
                    || Grid[x][y] == 6
                    || Grid[x][y] == 7
                    || Grid[x][y] == 8
                    || Grid[x][y] == -1
                    ) && RevealGrid[x][y] == 0) { // óáèë
                    RevealGrid[x][y] = 1;
                }
                else if (Grid[x][y] == -3) {
                    Grid[x][y] = -3;
                }
                else if (Grid[x][y] == 1 && RevealGrid[x][y] == 1) {
                    Grid[x][y] = 1;
                }
                else if (Grid[x][y] == 2 && RevealGrid[x][y] == 1){
                    Grid[x][y] = 2;
                }
                else if (Grid[x][y] == 3 && RevealGrid[x][y] == 1){
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
                else if (Grid[x][y] == -1 && RevealGrid[x][y] == -1) {
                    Grid[x][y] = -1;
                }
            }
        }
        window.clear(Color::White);

        /*for (int x = 1; x < size + 1; x++) {
            for (int y = 1; y < size + 1; y++) {
                if (Grid[x][y] == 0) {
                    if (isClear(x, y, Grid, size) == true) {
                        Grid[x][y] = -2;
                        if (Grid[x + 1][y] == 0 && (x + 1) >= 1 && (x + 1) < size + 1 && y >= 1 && y < size + 1) {
                            Grid[x + 1][y] = -2;
                        }
                        if (Grid[x - 1][y] == 0 && (x - 1) >= 1 && (x - 1) < size + 1 && y >= 1 && y < size + 1) {
                            Grid[x - 1][y] = -2;
                        }
                        if (Grid[x][y + 1] == 0 && x >= 1 && x < size + 1 && (y + 1) >= 1 && (y + 1) < size + 1) {
                            Grid[x][y + 1] = -2;
                        }
                        if (Grid[x][y - 1] == 0 && x >= 1 && x < size + 1 && (y - 1) >= 1 && (y - 1) < size + 1) {
                            Grid[x][y - 1] = -2;
                        }
                        if (Grid[x + 1][y + 1] == 0 && (x + 1) >= 1 && (x + 1) < size + 1 && (y + 1) >= 1 && (y + 1) < size + 1) {
                            Grid[x + 1][y + 1] = -2;
                        }
                        if (Grid[x - 1][y - 1] == 0 && (x - 1) >= 1 && (x - 1) < size + 1 && (y - 1) >= 1 && (y - 1) < size + 1) {
                            Grid[x - 1][y - 1] = -2;
                        }
                        if (Grid[x - 1][y + 1] == 0 && (x - 1) >= 1 && (x - 1) < size + 1 && (y + 1) >= 1 && (y + 1) < size + 1) {
                            Grid[x - 1][y + 1] = -2;
                        }
                        if (Grid[x + 1][y - 1] == 0 && (x + 1) >= 1 && (x + 1) < size + 1 && (y - 1) >= 1 && (y - 1) < size + 1) {
                            Grid[x + 1][y - 1] = -2;
                        }
                    }
                }
            }
        }*/

        drawGrid(window, Grid, RevealGrid); // ðèñóåì ïîëå èãðîêà

        window.display();

    }

    return 0;
}