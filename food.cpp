// --------------------------------------------------------------
// Copyright (c) 2020 Wang Haotian in SJTU. All rights reserved.
// --------------------------------------------------------------
#include "food.h"
#include "snake.h"
#include <ctime>
#include <cstdlib>

Food::Food(Snake **mySnake, Obstacle &obs, int num)
{
    int type_rand;
    srand(time(NULL));

    type_rand = rand() % 25;
    switch (type_rand){
    case 0: case 1:
        ftp = doublescore;
        image.load(":/Images/food_doubleScore.png");
        break;
    case 2:
        ftp = addHP;
        image.load(":/Images/food_addHP.png");
        break;
    case 3:
        ftp = slowdown;
        image.load(":/Images/food_slowDown.png");
        break;
    case 4: case 5:
        ftp = speedup;
        image.load(":/Images/food_speedUp.png");
        break;
    default:
        ftp = ordinary;
        image.load(":/Images/food_ordinary.png");
    }

    do{
        x = rand() % 25 + 1;
        y = rand() % 25 + 1;  // to be changed here
    }while(!SnakeFree(x, y, mySnake, num)
           && !ObstacleFree(x, y, obs));
}

Food::Food(int m, int n, int type)
{
    x = m;
    y = n;
    switch(type)
    {
    case 1:
        ftp = ordinary;
        image.load(":/Images/food_ordinary.png");
        break;
    case 2:
        ftp = speedup;
        image.load(":/Images/food_speedUp.png");
        break;
    case 3:
        ftp = slowdown;
        image.load(":/Images/food_slowDown.png");
        break;
    case 4:
        ftp = addHP;
        image.load(":/Images/food_addHP.png");
        break;
    case 5:
        ftp = doublescore;
        image.load(":/Images/food_doubleScore.png");
        break;
    }
}

void Food::reset(Snake **mySnake, Obstacle &obs, int num)
{
    int type_rand;
    srand(time(NULL));

    type_rand = rand() % 20;
    switch (type_rand){
    case 0: case 1:
        ftp = doublescore;
        image.load(":/Images/food_doubleScore.png");
        break;
    case 2:
        ftp = addHP;
        image.load(":/Images/food_addHP.png");
        break;
    case 3: case 4:
        ftp = slowdown;
        image.load(":/Images/food_slowDown.png");
        break;
    case 5: case 6:
        ftp = speedup;
        image.load(":/Images/food_speedUp.png");
        break;
    default:
        ftp = ordinary;
        image.load(":/Images/food_ordinary.png");
    }

    x = rand() % 25 + 1;
    y = rand() % 25 + 1;

    for(;;++x){
        for(;;++y){
            if(y == 26) y = 1;
            if(SnakeFree(x, y, mySnake, num) && ObstacleFree(x, y, obs))
                return;
        }
       if(x == 26) x = 1;
    }
}

void Food::changeFood()
{
    if(ftp == ordinary){
        ftp = speedup;
        image.load(":/Images/food_speedUp.png");
    }
    else if(ftp == speedup){
        ftp = slowdown;
        image.load(":/Images/food_slowDown.png");
    }
    else if(ftp == slowdown){
        ftp = addHP;
        image.load(":/Images/food_addHP.png");
    }
    else if(ftp == addHP){
        ftp = doublescore;
        image.load(":/Images/food_doubleScore.png");
    }
    else if(ftp == doublescore){
        ftp = ordinary;
        image.load(":/Images/food_ordinary.png");
    }
}

bool Food::SnakeFree(int x, int y, Snake **p, int num) const
{
    node *tmp;
    for(int i = 0;i < num;++i){
        tmp = p[i]->head;
        while(tmp != nullptr){
            if(x == tmp->x && y == tmp->y) return false;
            tmp = tmp->next;
        }
    }

    return true;
}

bool Food::ObstacleFree(int x, int y, Obstacle &obs) const
{
    for(int i = 0;i < 8;++i){
        if(x == obs.arrayX[i] && y == obs.arrayY[i])
            return false;
    }

    return true;
}
