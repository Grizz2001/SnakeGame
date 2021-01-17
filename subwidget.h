// --------------------------------------------------------------
// Copyright (c) 2020 Wang Haotian in SJTU. All rights reserved.
// --------------------------------------------------------------
#ifndef SUBWIDGET_H
#define SUBWIDGET_H

#include <QWidget>
#include <QIcon>
#include <QPalette>
#include <QBrush>
#include <QImage>
#include <QPushButton>
#include <QMessageBox>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QLabel>
#include <QTimer>
#include <QTime>
#include <QKeyEvent>

#include <ctime>
#include <string>
#include <stack>
#include <fstream>
#include <vector>

#include "food.h"
#include "snake.h"
#include "obstacle.h"

class Snake;
class Food;
class Obstacle;
struct node;

struct coordinate{
    int x;
    int y;
    int parentx;
    int parenty;
    int G;
    int F;

    coordinate(int q, int w, int e, int r, int g){
        x = q;
        y = w;
        parentx = e;
        parenty = r;
        G = g;
    }
};

struct pathNode{
    int x;
    int y;

    pathNode(int m, int n):x(m), y(n){};
};

int Manhattan(int x, int y, int tarX, int tarY);
int F(int x, int y, int tarX, int tarY, int G);

class SubWidget : public QWidget
{
    Q_OBJECT

    friend int Manhattan (int iniX, int iniY, int tarX, int tarY);
    friend int F(int x, int y, int tarX, int tarY, int G);

private:
    int player;
    Food *food_obj;
    Snake *Snake_obj[2];
    Obstacle *obstacle;
    QImage imageWall;
    QImage imageWallLight;

    bool pauseConnected;
    bool autoRunning;

public:
    std::vector<coordinate> open;
    std::vector<coordinate> closed;
    std::stack<pathNode> path;

    explicit SubWidget(int num, QWidget *parent = 0);
    explicit SubWidget(std::string &filename, QWidget *parent = 0);
    ~SubWidget();

    void keyPressEvent(QKeyEvent *k);
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *m);

    bool SnakeFree(int x, int y);
    bool ObstacleFree(int x, int y);
    bool FoodFree(int x, int y);
    void SnakeReborn(int num);
    bool recursion(node *&tmp, int x, int y,
                   std::stack<int> &xstack, std::stack<int> &ystack); // called in function SnakeReborn

    // auto-play
    // this is a AStar algorithm of static version
    // so when the snake is too long, it cannot find any path
    bool AStar();
    bool valid(int x, int y);

    QLabel *ScoreTitle[2];
    QLabel *Score[2];
    QLabel *HPTitle[2];
    QLabel *HP[2];
    QTimer *Timer;
    QPushButton *buttonStart;
    QPushButton *buttonExit;
    QPushButton *buttonSave;
    QPushButton *buttonAuto;
    QPushButton *buttonRestart;
private slots:
    void runSnake();
    void exitSnake();
    void pauseSnake();
    void saveSnake();
    void autoSnake();
    void restartSnake();
};

#endif // SUBWIDGET_H
