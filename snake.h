// --------------------------------------------------------------
// Copyright (c) 2020 Wang Haotian in SJTU. All rights reserved.
// --------------------------------------------------------------
#ifndef SNAKE_H
#define SNAKE_H

#include <QImage>
#include <QPaintEvent>
#include <vector>
#include "food.h"
#include "subwidget.h"

enum direction {up, right, down, left};

class Snake;
class Food;
class Obstacle;

struct node
{
    int x;         // x coordinate
    int y;         // y coordinate
    node *next;    // next node's pointer
    QImage image;

    node(int x_tmp, int y_tmp, node *&p):x(x_tmp), y(y_tmp), next(p){}; // for body and head
    node(int x_tmp, int y_tmp):x(x_tmp), y(y_tmp){next = nullptr;} // for tail
};

class Snake
{
    friend class Food;
    friend class SubWidget;
    friend class Obstacle;
private:
    int number;         // this number determines whether it's green or red
    node *head;         // snake's head
    node *tail;         // snake's tail
    node *pretail;      // snake's pre-tail (record tail's previous location to make it grow)
    direction dir;      // moving direction
    int HP;             // health point
    int score;          // your score
    int speed;          // moving speed: the greater the value of speed is, the slower the snake moves
    int interval;       // to control two snakes move;

    /* Special Food Attribute */
    void speedUp();     // speed Up
    void slowDown();    // slow down

    /* Judgement */
    // judge if the snake grows longer and do corresponding operation
    bool ifGrow(Food *&food);
    // judge if the snake is alive
    bool isAlive(Snake *&mysnake, Obstacle &obs, int flag);
public:
    Snake(int number = 0);
    Snake(int health, int numbner);
    Snake(int number, int length, std::vector<int> snake_data,int px, int py,
          int DIR, int health, int SCORE, int SPEED); // for file input
    ~Snake();

    /* Operation */
    void toUp();        // shift to up
    void toRight();     // shift to right
    void toDown();      // shift to down
    void toLeft();      // shift to left

    void move();        // move
    void updateTail();  // update tail's image
    bool reborn();      // reborn after death if HP>1

    void reset(int num);       // to restart
};

#endif // SNAKE_H
