#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <ctime>

using namespace sf;

const int cellSize = 50; // ðàçìåð ÿ÷ååê
const int size = 10; // ðàçìåð ïîëÿ
const int numBombs = 10;
int win = 0;
int lose = 0;

int Grid[size + 2][size + 2]; // èãðîâîå ïîëå êîìïüþòåðà

bool isPlayerTurn = true; // õîä èãðîêà

void drawGrid(RenderWindow& window, int grid[size + 2][size + 2]) {
    RectangleShape cell(Vector2f(cellSize, cellSize));
    cell.setOutlineThickness(1.f);

    for (int x = 0; x < size + 2; x++) {
        for (int y = 0; y < size + 2; y++) {
            cell.setPosition(x * cellSize, y * cellSize);
            if (Grid[x][y] == -3) { // ïîïàäàíèå
                cell.setFillColor(Color::Magenta);
                cell.setOutlineColor(Color::Magenta);
            }
            else if (Grid[x][y] == -2) { // ïîïàäàíèå
                cell.setFillColor(Color::White);
                cell.setOutlineColor(Color::White);
            }
            else if (Grid[x][y] == -1) { // ïîïàäàíèå
                cell.setFillColor(Color::Black);
                cell.setOutlineColor(Color::Black);
            }
            else if (Grid[x][y] == 0) { // íåîòêðûòàÿ ÿ÷åéêà
                cell.setFillColor(Color::Yellow);
                cell.setOutlineColor(Color::Black);
            }
            else if (Grid[x][y] == 1) { // ïàëóáà êîðàáëÿ
                cell.setFillColor(Color::Yellow);
                cell.setOutlineColor(Color::Black);
            }
            else if (Grid[x][y] == 2) { // ïðîìàõ
                cell.setFillColor(Color::Yellow);
                cell.setOutlineColor(Color::Black);
            }
            else if (Grid[x][y] == 3) { // ïðîìàõ
                cell.setFillColor(Color::Yellow);
                cell.setOutlineColor(Color::Black);
            }
            else if (Grid[x][y] == 4) { // ïðîìàõ
                cell.setFillColor(Color::Yellow);
                cell.setOutlineColor(Color::Black);
            }
            else if (Grid[x][y] == 5) { // ïðîìàõ
                cell.setFillColor(Color::Yellow);
                cell.setOutlineColor(Color::Black);
            }
            else if (Grid[x][y] == 6) { // ïðîìàõ
                cell.setFillColor(Color::Yellow);
                cell.setOutlineColor(Color::Black);
            }
            else if (Grid[x][y] == 7) { // ïðîìàõ
                cell.setFillColor(Color::Yellow);
                cell.setOutlineColor(Color::Black);
            }
            else if (Grid[x][y] == 8) { // ïðîìàõ
                cell.setFillColor(Color::Yellow);
                cell.setOutlineColor(Color::Black);
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
            if (Grid[row][col] == -1 && Grid[row][col] != -2 && Grid[row][col] != -3) {
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

bool isClear(int x, int y, int Grid[size + 2][size + 2], int size) {
    if (Grid[x + 1][y] != 1) {
        if (Grid[x + 1][y] == 3 || Grid[x + 1][y] == 4) {
            if (Grid[x + 2][y] != 1) {
                if (Grid[x + 2][y] == 3 || Grid[x + 2][y] == 4) {
                    if (Grid[x + 3][y] != 1) {
                        if (Grid[x + 3][y] == 3 || Grid[x + 3][y] == 4) {
                            return true;
                        }
                    }
                    else if (Grid[x + 3][y] == 1) {
                        return false;
                    }
                }
            }
            else if (Grid[x + 2][y] == 1) {
                return false;
            }
        }
    }
    else if (Grid[x + 1][y] == 1) {
        return false;
    }
    if (Grid[x - 1][y] != 1) {
        if (Grid[x - 1][y] == 3 || Grid[x - 1][y] == 4) {
            if (Grid[x - 2][y] != 1) {
                if (Grid[x - 2][y] == 3 || Grid[x - 2][y] == 4) {
                    if (Grid[x - 3][y] != 1) {
                        if (Grid[x - 3][y] == 3 || Grid[x - 3][y] == 4) {
                            return true;
                        }
                    }
                    else if (Grid[x - 3][y] == 1) {
                        return false;
                    }
                }
            }
            else if (Grid[x - 2][y] == 1) {
                return false;
            }
        }
    }
    else if (Grid[x - 1][y] == 1) {
        return false;
    }
    if (Grid[x][y + 1] != 1) {
        if (Grid[x][y + 1] == 3 || Grid[x][y + 1] == 4) {
            if (Grid[x][y + 2] != 1) {
                if (Grid[x][y + 2] == 3 || Grid[x][y + 2] == 4) {
                    if (Grid[x][y + 3] != 1) {
                        if (Grid[x][y + 3] == 3 || Grid[x][y + 3] == 4) {
                            return true;
                        }
                    }
                    else if (Grid[x][y + 3] == 1) {
                        return false;
                    }
                }
            }
            else if (Grid[x][y + 2] == 1) {
                return false;
            }
        }
    }
    else if (Grid[x][y + 1] == 1) {
        return false;
    }
    if (Grid[x][y - 1] != 1) {
        if (Grid[x][y - 1] == 3 || Grid[x][y - 1] == 4) {
            if (Grid[x][y - 2] != 1) {
                if (Grid[x][y - 2] == 3 || Grid[x][y - 2] == 4) {
                    if (Grid[x][y - 3] != 1) {
                        if (Grid[x][y - 3] == 3 || Grid[x][y - 3] == 4) {
                            return true;
                        }
                    }
                    else if (Grid[x][y - 3] == 1) {
                        return false;
                    }
                }
            }
            else if (Grid[x][y - 2] == 1) {
                return false;
            }
        }
    }
    else if (Grid[x][y - 1] == 1) {
        return false;
    }
    return true;
}

void timer() {
    clock_t start_time = clock(); // Ïîëó÷åíèå òåêóùåãî âðåìåíè
    while (clock() < start_time + 0.5 * CLOCKS_PER_SEC) {} // Îæèäàíèå 2 ñåêóíä
}

int main() {
    RenderWindow window(VideoMode((size + 2) * cellSize, (size + 2) * cellSize), "Minesweeper", Style::Titlebar | Style::Close);
    window.setFramerateLimit(60);
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

                if (Grid[x][y] == 0) { // óáèë
                    Grid[x][y] = -2;
                }
                else if (Grid[x][y] == 4) {
                    Grid[x][y] = 4;
                }
                else if (Grid[x][y] == 3) {
                    Grid[x][y] = 3;
                }
                else if (Grid[x][y] == 2) {
                    Grid[x][y] = 2;
                }
                else if (Grid[x][y] == -1) {
                    Grid[x][y] = -1;
                }
            }
        }
        window.clear(Color::White);

        for (int x = 1; x < size + 1; x++) {
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
        }

        drawGrid(window, Grid); // ðèñóåì ïîëå èãðîêà

        window.display();

    }

    return 0;
}