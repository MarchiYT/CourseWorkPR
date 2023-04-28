#pragma warning(disable:4244)
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <cmath>
#include <ctime>
#include <windows.h>

using namespace sf;
//Объявление переменных, текстур, музыки и массивов
float cellSize;
const int size = 10;
const int numBombs = 10;
int win = 0;
bool islost = 0;
int face = 1;
bool firstHit = 1;
bool ispaused = 0;

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
Texture arrow;
Texture pause;
Texture resume;
Music music;

int Grid[size + 2][size + 2];
int RevealGrid[size + 2][size + 2];
int Musicqueue[15];
//Функция drawGrid отвечает за отрисовку поля по одному селлу
void drawGrid(RenderWindow& window, int grid[size + 2][size + 2], int revealgrid[size + 2][size + 2], int openCells, int Vertres) {
    Sprite cell; //Объявление селла, который мы будем отрисовывать
    cell.setPosition(sf::Vector2f(cellSize, cellSize));
    cell.setScale(cellSize/32, cellSize/32);
    //Проверка элемента массива, чтобы отрисовать соответствующие селл с нужной текстурой
    for (int x = 0; x < size + 2; x++) {
        for (int y = 0; y < size + 2; y++) {
            cell.setPosition(x * cellSize, y * cellSize);
            if (Grid[x][y] == -3) {
                cell.setTexture(border);
            }
            else if (Grid[x][y] == -5) {
                cell.setTexture(arrow);
            }
            else if (Grid[x][y] == -6) {
                cell.setTexture(pause);
            }
            else if (Grid[x][y] == -7) {
                cell.setTexture(resume);
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
            window.draw(cell);//отрисовка селла
        }
    }
}
//Функция Bomb_placement отвечает за расстановку бомб и внесение значения об окружающих бомбах а массив
void Bomb_placement(int Grid[size + 2][size + 2],int size, int numBombs) {
    //Расстановка бомб
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

    //Внесение в клетку значения о рядом стоящих бомбах
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
//Функция PlayingMusic отвечает за проверку работает ли музыка, и если нет, включает её
void PlayingMusic() {
    srand(time(NULL));
    if (music.getStatus() != 0) {
        return;
    }
    else {//тут у меня такая система, что индекс музыки идёт в массив, номера которого не повторяются и рандомно выбираются
        bool newsong = 0;
        while (newsong == 0) {
            int m = rand() % 15 + 1;
            if (m == 1 && Musicqueue[m - 1] == 0) {
                music.openFromFile("./music/music1.ogg");
                Musicqueue[m - 1] = 1;
                newsong = 1;
            }
            else if (m == 2 && Musicqueue[m - 1] == 0) {
                music.openFromFile("./music/music2.ogg");
                Musicqueue[m - 1] = 1;
                newsong = 1;
            }
            else if (m == 3 && Musicqueue[m - 1] == 0) {
                music.openFromFile("./music/music3.ogg");
                Musicqueue[m - 1] = 1;
                newsong = 1;
            }
            else if (m == 4 && Musicqueue[m - 1] == 0) {
                music.openFromFile("./music/music4.ogg");
                Musicqueue[m - 1] = 1;
                newsong = 1;
            }
            else if (m == 5 && Musicqueue[m - 1] == 0) {
                music.openFromFile("./music/music5.ogg");
                Musicqueue[m - 1] = 1;
                newsong = 1;
            }
            else if (m == 6 && Musicqueue[m - 1] == 0) {
                music.openFromFile("./music/music6.ogg");
                Musicqueue[m - 1] = 1;
                newsong = 1;
            }
            else if (m == 7 && Musicqueue[m - 1] == 0) {
                music.openFromFile("./music/music7.ogg");
                Musicqueue[m - 1] = 1;
                newsong = 1;
            }
            else if (m == 8 && Musicqueue[m - 1] == 0) {
                music.openFromFile("./music/music8.ogg");
                Musicqueue[m - 1] = 1;
                newsong = 1;
            }
            else if (m == 9 && Musicqueue[m - 1] == 0) {
                music.openFromFile("./music/music9.ogg");
                Musicqueue[m - 1] = 1;
                newsong = 1;
            }
            else if (m == 10 && Musicqueue[m - 1] == 0) {
                music.openFromFile("./music/music10.ogg");
                Musicqueue[m - 1] = 1;
                newsong = 1;
            }
            else if (m == 11 && Musicqueue[m - 1] == 0) {
                music.openFromFile("./music/music11.ogg");
                Musicqueue[m - 1] = 1;
                newsong = 1;
            }
            else if (m == 12 && Musicqueue[m - 1] == 0) {
                music.openFromFile("./music/music12.ogg");
                Musicqueue[m - 1] = 1;
                newsong = 1;
            }
            else if (m == 13 && Musicqueue[m - 1] == 0) {
                music.openFromFile("./music/music13.ogg");
                Musicqueue[m - 1] = 1;
                newsong = 1;
            }
            else if (m == 14 && Musicqueue[m - 1] == 0) {
                music.openFromFile("./music/music14.ogg");
                Musicqueue[m - 1] = 1;
                newsong = 1;
            }
            else if (m == 15 && Musicqueue[m - 1] == 0) {
                music.openFromFile("./music/music15.ogg");
                Musicqueue[m - 1] = 1;
                newsong = 1;
            }
        }
        music.play();//лютый навал музыки
    }
    music.setVolume(10);
    int count = 0;//если все песенки отыграли, очередь заново выстраевается
    for (int i = 0; i < 15; i++) {
        if (Musicqueue[i] == 1) {
            count++;
        }
    }
    if (count == 15) {
        for (int i = 0; i < 15; i++) {
            Musicqueue[i] = 0;
        }
    }
}
//Тут просто иницаилизация всех текстур
void init() {

    //Texture one;
    one.loadFromFile("./images/number_1.png");
    one.setSmooth(true);

    //Texture two;
    two.loadFromFile("./images/number_2.png");
    two.setSmooth(true);

    //Texture three;
    three.loadFromFile("./images/number_3.png");
    three.setSmooth(true);

    //Texture four;
    four.loadFromFile("./images/number_4.png");
    four.setSmooth(true);

    //Texture five;
    five.loadFromFile("./images/number_5.png");
    five.setSmooth(true);

    //Texture six;
    six.loadFromFile("./images/number_6.png");
    six.setSmooth(true);

    //Texture seven;
    seven.loadFromFile("./images/number_7.png");
    seven.setSmooth(true);

    //Texture eight;
    eight.loadFromFile("./images/number_8.png");
    eight.setSmooth(true);

    //Texture hidden;
    hidden.loadFromFile("./images/tile_hidden.png");
    hidden.setSmooth(true);

    //Texture revealed;
    revealed.loadFromFile("./images/tile_revealed.png");
    revealed.setSmooth(true);

    //Texture mine;
    mine.loadFromFile("./images/mine.png");
    mine.setSmooth(true);

    //Texture flag;
    flag.loadFromFile("./images/flag.png");
    flag.setSmooth(true);

    //Texture border;
    border.loadFromFile("./images/border.png");
    border.setSmooth(true);

    //Texture face_happy;
    face_happy.loadFromFile("./images/face_happy.png");
    face_happy.setSmooth(true);

    //Texture face_lose;
    face_lose.loadFromFile("./images/face_lose.png");
    face_lose.setSmooth(true);

    //Texture face_win;
    face_win.loadFromFile("./images/face_win.png");
    face_win.setSmooth(true);

    //Texture arrow;
    arrow.loadFromFile("./images/arrow.png");
    arrow.setSmooth(true);

    //Texture arrow;
    pause.loadFromFile("./images/pause.png");
    pause.setSmooth(true);

    //Texture arrow;
    resume.loadFromFile("./images/resume.png");
    resume.setSmooth(true);
}
//Эта функция очищает наш массив
void newGrid() {
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
    Grid[size + 1][size + 1] = -5;
    if (ispaused == 0) {
        Grid[size][size + 1] = -6;
    }
    else if (ispaused == 1) {
        Grid[size][size + 1] = -7;
    }
    srand(time(NULL));
}
//Эта функция нужна для того, чтобы менять значения кнопки паузы
void Buttonswl(Event event) {
    if (event.mouseButton.button == Mouse::Left && event.mouseButton.x >= (size + 1) * cellSize && event.mouseButton.x < (size + 2) * cellSize && event.mouseButton.y >= (size + 1) * cellSize && event.mouseButton.y < (size + 2) * cellSize) {
        music.stop();
        if (ispaused == 1) {
            Grid[size][size + 1] = -6;
            ispaused = 0;
        }
                        }
    else if (event.mouseButton.button == Mouse::Left && event.mouseButton.x >= size * cellSize && event.mouseButton.x < (size + 1) * cellSize && event.mouseButton.y >= (size + 1) * cellSize && event.mouseButton.y < (size + 2) * cellSize) {
        if (ispaused == 0) {
            Grid[size][size + 1] = -7;
            music.pause();
            ispaused = 1;
        }
        else if (ispaused == 1) {
            Grid[size][size + 1] = -6;
            music.play();
            ispaused = 0;
        }
    }
}

int main() {
    //Тут мы получаем высоту окна
    HDC hDCScreen = GetDC(NULL);
    int Vertres = GetDeviceCaps(hDCScreen, VERTRES);
    ReleaseDC(NULL, hDCScreen);
    //Тут размер селла задается в зависимости от высоты окна(на 4к телеке все ок будет:))
    cellSize = Vertres / 33.75;
    //Тут мы создаем окошечко
    RenderWindow window(VideoMode((size + 2) * (int)cellSize, (size + 2) * (int)cellSize), "Minesweeper", Style::Titlebar | Style::Close);
    ShowWindow(GetConsoleWindow(), SW_HIDE);
    window.setFramerateLimit(60);
    //Чистил массив музыки
    for (int i = 0; i < 15; i++) {
        Musicqueue[i] = 0;
    }
    //Включаем навал музла
    PlayingMusic();
    //Инициализируем текстуры
    init();
    //Очищаем поле для игры
    newGrid();
    //Тут начинается игра
    while (window.isOpen()) {
        PlayingMusic();//Проверяем музло
        Event event;//Объявляем ивент
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {//Закрытие игры
                window.close();
            }
            if (event.type == Event::MouseButtonPressed) {//Нажали на мышку(норушку)
                if (islost == 0) {//Тут начинается стрельба
                    if (event.mouseButton.button == Mouse::Left && event.mouseButton.x >= cellSize && event.mouseButton.x < (size + 1) * cellSize && event.mouseButton.y >= cellSize && event.mouseButton.y < (size + 1) * cellSize) {
                        int x = event.mouseButton.x / cellSize;//Тут мы берём координаты нашего выстрела
                        int y = event.mouseButton.y / cellSize;

                        if (firstHit == 1) {//Если мы ещё не стреляли генерим новый массив, чтобы мо попали точно в 0
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
                        }//Ну а тут у нас изменение массисов, в зависимости от того, куда мы стреляли
                        if ((Grid[x][y] == 1 || Grid[x][y] == 2 || Grid[x][y] == 3 || Grid[x][y] == 4 || Grid[x][y] == 5 || Grid[x][y] == 6 || Grid[x][y] == 7 || Grid[x][y] == 8) && RevealGrid[x][y] == 0) {
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
                    }//Тут у нас расстановка флагов
                    else if (event.mouseButton.button == Mouse::Right && firstHit == 0) {
                        int openCells = 0;
                        for (int x = 1; x < size + 1; x++) {
                            for (int y = 1; y < size + 1; y++) {
                                if (RevealGrid[x][y] == 1) {
                                    openCells++;
                                }
                            }
                        }//Тут микро проверочка, что мы ещё не выйграли
                        if ((win != numBombs || (openCells != (size * size - numBombs)))) {
                            int x = event.mouseButton.x / cellSize;
                            int y = event.mouseButton.y / cellSize;
                            if (Grid[x][y] == -1 && RevealGrid[x][y] == 0 && Grid[x][y] != -3 && Grid[x][y] != -4 && Grid[x][y] != -5 && Grid[x][y] != -6 && Grid[x][y] != -7) {
                                win++;
                            }
                            else if (Grid[x][y] == -1 && RevealGrid[x][y] == 2 && Grid[x][y] != -3 && Grid[x][y] != -4 && Grid[x][y] != -5 && Grid[x][y] != -6 && Grid[x][y] != -7) {
                                win--;
                            }
                            else if (Grid[x][y] != -1 && RevealGrid[x][y] == 2 && Grid[x][y] != -3 && Grid[x][y] != -4 && Grid[x][y] != -5 && Grid[x][y] != -6 && Grid[x][y] != -7) {
                                win++;
                            }
                            else if (Grid[x][y] != -1 && RevealGrid[x][y] == 0 && Grid[x][y] != -3 && Grid[x][y] != -4 && Grid[x][y] != -5 && Grid[x][y] != -6 && Grid[x][y] != -7) {
                                win--;
                            }
                            if (RevealGrid[x][y] == 0 && Grid[x][y] != -3 && Grid[x][y] != -4 && Grid[x][y] != -5 && Grid[x][y] != -6 && Grid[x][y] != -7) {
                                RevealGrid[x][y] = 2;
                            }
                            else if (RevealGrid[x][y] == 2 && Grid[x][y] != -3 && Grid[x][y] != -4 && Grid[x][y] != -5 && Grid[x][y] != -6 && Grid[x][y] != -7) {
                                RevealGrid[x][y] = 0;
                            }
                        }
                    }//Обновляем смайлик в зависимости от исхода стрельбы
                    Buttonswl(event);
                }//Тут проверка, что если проиграли, при нажатии на смайлик, игра обновлятся
                else if (islost == 1) {
                    if (event.type == Event::MouseButtonPressed) {
                        if (event.mouseButton.x >= 0 && event.mouseButton.x <= cellSize && event.mouseButton.y >= 0 && event.mouseButton.y <= cellSize) {
                            newGrid();
                            islost = 0;
                            win = 0;
                            firstHit = 1;
                        }//Обновляем смайлик в зависимости от исхода стрельбы
                        Buttonswl(event);
                    }
                }
            }
        }
        //Тут мы вскрываем все клетки, рядом с пустой открытой
        for (int i = 0; i < size * size; i++) {
            for (int x = 1; x < size + 1; x++) {
                for (int y = 1; y < size + 1; y++) {
                    if ((Grid[x][y] == 0 || Grid[x][y] == -2) && RevealGrid[x][y] == 1) {

                        if (Grid[x + 1][y] != -3 && Grid[x + 1][y] != -4 && Grid[x + 1][y] != -1 && (x + 1) >= 1 && (x + 1) < size + 1 && y >= 1 && y < size + 1 && RevealGrid[x + 1][y] != 1 && RevealGrid[x + 1][y] != 2) {
                            RevealGrid[x + 1][y] = 1;
                        }
                        if (Grid[x - 1][y] != -3 && Grid[x - 1][y] != -4 && Grid[x - 1][y] != -1 && (x - 1) >= 1 && (x - 1) < size + 1 && y >= 1 && y < size + 1 && RevealGrid[x - 1][y] != 1 && RevealGrid[x - 1][y] != 2) {
                            RevealGrid[x - 1][y] = 1;
                        }
                        if (Grid[x][y + 1] != -3 && Grid[x][y + 1] != -4 && Grid[x][y + 1] != -1 && x >= 1 && x < size + 1 && (y + 1) >= 1 && (y + 1) < size + 1 && RevealGrid[x][y + 1] != 1 && RevealGrid[x][y + 1] != 2) {
                            RevealGrid[x][y + 1] = 1;
                        }
                        if (Grid[x][y - 1] != -3 && Grid[x][y - 1] != -4 && Grid[x][y - 1] != -1 && x >= 1 && x < size + 1 && (y - 1) >= 1 && (y - 1) < size + 1 && RevealGrid[x][y - 1] != 1 && RevealGrid[x][y - 1] != 2) {
                            RevealGrid[x][y - 1] = 1;
                        }
                    }
                }
            }
        }
        //Это нам надо, чтобы на победу проверить
        int openCells = 0;
        for (int x = 1; x < size + 1; x++) {
            for (int y = 1; y < size + 1; y++) {
                if (RevealGrid[x][y] == 1) {
                    openCells++;
                }
            }
        }
        //Отчистка окна
        window.clear(Color::White);
        //Отрисовка поля
        drawGrid(window, Grid, RevealGrid, openCells, Vertres);
        //Лютый показ окна
        window.display();
        //Тут проверочка на победу
        if (win == numBombs && (openCells == (size*size-numBombs))) {
            if (event.type == Event::MouseButtonPressed) {
                if (event.mouseButton.x >= 0 && event.mouseButton.x <= cellSize && event.mouseButton.y >= 0 && event.mouseButton.y <= cellSize) {
                    newGrid();
                    win = 0;
                    firstHit = 1;
                }
            }
        }

    }
    return 0;
}