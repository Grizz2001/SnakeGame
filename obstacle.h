// --------------------------------------------------------------
// Copyright (c) 2020 Wang Haotian in SJTU. All rights reserved.
// --------------------------------------------------------------
#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "subwidget.h"
#include "snake.h"


class Obstacle
{
    friend class Snake;
    friend class SubWidget;
    friend class Food;
private:
    int arrayX[8];
    int arrayY[8];
    int chosen;
public:
    Obstacle();
    Obstacle(int *x, int *y);

    void reset();
};

#endif // OBSTACLE_H
