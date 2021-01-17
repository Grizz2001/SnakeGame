// --------------------------------------------------------------
// Copyright (c) 2020 Wang Haotian in SJTU. All rights reserved.
// --------------------------------------------------------------
#include "obstacle.h"
#include <ctime>
#include <cstdlib>

Obstacle::Obstacle()
{
    srand(time(NULL));
    int x, y;
    for(int i = 0;i < 8;++i){
        do{
            x = rand() % 22 + 2;
            y = rand() % 22 + 2;
        }while(x == y && (x == 12 || x == 14));
        arrayX[i] = x;
        arrayY[i] = y;
    }
    chosen = 8;
}

Obstacle::Obstacle(int *x, int *y)
{
    for(int i = 0;i < 8;++i){
        arrayX[i] = *(x+i);
        arrayY[i] = *(y+i);
    }
    chosen = 8;
}

void Obstacle::reset()
{
    srand(time(NULL));
    int x, y;
    for(int i = 0;i < 8;++i){
        do{
            x = rand() % 22 + 2;
            y = rand() % 22 + 2;
        }while(x == y && (x == 12 || x == 14));
        arrayX[i] = x;
        arrayY[i] = y;
    }
    chosen = 8;
}
