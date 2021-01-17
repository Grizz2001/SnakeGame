// --------------------------------------------------------------
// Copyright (c) 2020 Wang Haotian in SJTU. All rights reserved.
// --------------------------------------------------------------
#ifndef FOOD_H
#define FOOD_H

#include <QImage>
#include "snake.h"
#include "obstacle.h"
#include "subwidget.h"

enum foodType {ordinary, speedup, slowdown, addHP, doublescore};
struct node;
class Obstacle;

class Food
{
    friend class Snake;
    friend class SubWidget;
public:
    Food(Snake **mySnake, Obstacle &obs, int num);
    Food(int x, int y, int type);
    void reset(Snake **mySnake, Obstacle &obs, int num);
    void changeFood();
    QImage image;
private:
    int x;
    int y;
    foodType ftp;

    bool SnakeFree(int x, int y, Snake **p, int num) const;
    bool ObstacleFree(int x, int y, Obstacle &obs) const;
};

#endif // FOOD_H
