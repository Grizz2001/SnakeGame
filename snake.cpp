// --------------------------------------------------------------
// Copyright (c) 2020 Wang Haotian in SJTU. All rights reserved.
// --------------------------------------------------------------
#include <QImage>
#include <QPaintEvent>
#include "snake.h"
#include "food.h"
#include "obstacle.h"

enum foodType;

Snake::Snake(int num)
{
    // initial location need to be set carefully
    number = num;
    head = new node(12+2*num, 12+2*num);
    tail = head;
    pretail = new node(-1, -1);
    HP = 1;
    score = 0;
    dir = up;
    speed = 600;
    interval = 0;

    if(number == 0){
        head->image.load(":/Images/green_head_up.png");
    } else{
        head->image.load(":/Images/red_head_up.png");
    }
}

Snake::Snake(int health, int num)
{   // initial location need to be set carefully
    number = num;
    head = new node(12+2*num, 12+2*num);
    tail = head;
    pretail = new node(-1, -1);
    HP = health;
    score = 0;
    dir = up;
    speed = 600;
    interval = 0;

    if(number == 0){
        head->image.load(":/Images/green_head_up.png");
    } else{
        head->image.load(":/Images/red_head_up.png");
    }
}

Snake::Snake(int num, int length, std::vector<int> snake_data,int px, int py,
             int DIR, int health, int SCORE, int SPEED)
{
    number = (num == 0) ? 0 : 1;
    node *tmp1;
    head = new node(snake_data[0], snake_data[1]);
    tmp1 = head;
    if(number == 0){
        for(int i = 1;i < length;++i){
            tmp1->next = new node(snake_data[2*i], snake_data[2*i+1]);
            tmp1 = tmp1->next;
            tmp1->image.load(":/Images/green_body.png");
        }
    } else {
        for(int i = 1;i < length;++i){
            tmp1->next = new node(snake_data[2*i], snake_data[2*i+1]);
            tmp1 = tmp1->next;
            tmp1->image.load(":/Images/red_body.png");
        }
    }

    tail = head;
    for(int i = 0;i < length - 1;++i){
        tail = tail->next;
    }

    updateTail();

    pretail = new node(px, py);

    if(number == 0){
        switch(DIR){
        case 1:
            dir = up;
            head->image.load(":/Images/green_head_up.png");
            break;
        case 2:
            dir = right;
            head->image.load(":/Images/green_head_right.png");
            break;
        case 3:
            dir = down;
            head->image.load(":/Images/green_head_down.png");
            break;
        case 4:
            dir = left;
            head->image.load(":/Images/green_head_left.png");
            break;
        }
    } else{
        switch(DIR){
        case 1:
            dir = up;
            head->image.load(":/Images/red_head_up.png");
            break;
        case 2:
            dir = right;
            head->image.load(":/Images/red_head_right.png");
            break;
        case 3:
            dir = down;
            head->image.load(":/Images/red_head_down.png");
            break;
        case 4:
            dir = left;
            head->image.load(":/Images/red_head_left.png");
            break;
        }
    }

    HP = health;
    score = SCORE;
    speed = SPEED;
    interval = 0;
}

Snake::~Snake()
{
    node *tmp = head;
    while(tmp != nullptr)
    {
        head = tmp->next;
        delete tmp;
        tmp = head;
    }
    delete pretail;
}

void Snake::slowDown()
{
    // lower bound
    if(speed >= 900) return;
    else speed += 100;
}

void Snake::speedUp()
{
    // upper bound
    if(speed <= 200) return;
    else speed -= 100;
}

void Snake::toDown()
{
    node *tmp = head->next;
    if(tmp != nullptr){
        int x = tmp->x, y = tmp->y, headx = head->x, heady = head->y;
        if(headx == x && heady < y) return;
    }

    dir = down;
    if(number == 0) head->image.load(":/Images/green_head_down.png");
    else head->image.load(":/Images/red_head_down.png");
}

void Snake::toUp()
{
    node *tmp = head->next;
    if(tmp != nullptr){
        int x = tmp->x, y = tmp->y, headx = head->x, heady = head->y;
        if(headx == x && heady > y) return;
    }

    dir = up;
    if(number == 0) head->image.load(":/Images/green_head_up.png");
    else head->image.load(":/Images/red_head_up.png");
}

void Snake::toLeft()
{
    node *tmp = head->next;
    if(tmp != nullptr){
        int x = tmp->x, y = tmp->y, headx = head->x, heady = head->y;
        if(headx > x && heady == y) return;
    }

    dir = left;
    if(number == 0) head->image.load(":/Images/green_head_left.png");
    else head->image.load(":/Images/red_head_left.png");
}

void Snake::toRight()
{
    node *tmp = head->next;
    if(tmp != nullptr){
        int x = tmp->x, y = tmp->y, headx = head->x, heady = head->y;
        if(headx < x && heady == y) return;
    }

    dir = right;
    if(number == 0) head->image.load(":/Images/green_head_right.png");
    else head->image.load(":/Images/red_head_right.png");
}

bool Snake::ifGrow(Food *&food)
{
    if(!(head->x == food->x && head->y == food->y)) return false;

    // Snake grows when food and head overlap
    tail->next = new node(pretail->x, pretail->y);
    tail = tail->next;

    // update body and tail
    for(node *tmp = head->next;tmp != nullptr && tmp != tail; tmp = tmp->next){
        if(number == 0) tmp->image.load(":/Images/green_body.png");
        else tmp->image.load(":/Images/red_body.png");
    }

    updateTail();

    // food's effect
    if(food->ftp == doublescore) score *= 2;
    else score += 1;

    if(food->ftp == addHP) HP++;
    if(food->ftp == speedup) speedUp();
    if(food->ftp == slowdown) slowDown();

    return true;
}

bool Snake::isAlive(Snake *&snake, Obstacle &obs, int flag)
//flag = 0 means the paremeter Snake is the caller Snake
{
    if(head->x == 0 || head->y == 0 || head->x == 26 || head->y == 26)
        return false;

    for(int i = 0;i < 8;++i){
        if(head->x == obs.arrayX[i] && head->y == obs.arrayY[i])
            return false;
    }


    node *tmp = snake->head;
    if(flag == 0) tmp = tmp->next;

    int headx = this->head->x, heady = this->head->y; // caller Snake
    while(tmp != nullptr){
        if(tmp->x == headx && tmp->y == heady)
            return false;
        tmp = tmp->next;
    }

    return true;
}

bool Snake::reborn()
{
    if(HP == 1) return false;

    return true;
}

void Snake::move()
{
    pretail->x = tail->x;
    pretail->y = tail->y;
    node *tmp;
    int x1 = head->x, y1 = head->y;
    int x2, y2;

    if(dir == right) x1++;
    if(dir == down) y1++;
    if(dir == left) x1--;
    if(dir == up) y1--;

    tmp = head;

    //x1, y1 store this node's future location
    //x2 ,y2 store this node's current location (its post node's future location)
    while(tmp != nullptr){
        x2 = tmp->x;
        y2 = tmp->y;
        tmp->x = x1;
        tmp->y = y1;
        x1 = x2;
        y1 = y2;
        tmp = tmp->next;
    }
    updateTail();
}

void Snake::updateTail()
{
    if(head == tail) return;

    node *tmp1 = head, *tmp2 = head->next;

    // parallel pointer tmp1 and tmp2
    while(tmp2 != tail){
        tmp1 = tmp1->next;
        tmp2 = tmp2->next;
    }
    // tmp1 : the node before tail node
    // tmp2 : the tail node

    int x1 = tmp1->x, y1 = tmp1->y;
    int x2 = tmp2->x, y2 = tmp2->y;
    if(number == 0){
        if(x1 > x2) tail->image.load(":/Images/green_tail_left.png");
        if(x1 < x2) tail->image.load(":/Images/green_tail_right.png");
        if(y1 > y2) tail->image.load(":/Images/green_tail_up.png");
        if(y1 < y2) tail->image.load(":/Images/green_tail_down.png");
    } else {
        if(x1 > x2) tail->image.load(":/Images/red_tail_left.png");
        if(x1 < x2) tail->image.load(":/Images/red_tail_right.png");
        if(y1 > y2) tail->image.load(":/Images/red_tail_up.png");
        if(y1 < y2) tail->image.load(":/Images/red_tail_down.png");
    }
}

void Snake::reset(int num)
{
    node *tmp = head;
    while(tmp != nullptr)
    {
        head = tmp->next;
        delete tmp;
        tmp = head;
    }
    delete pretail;

    head = nullptr;
    pretail = nullptr;

    number = num;
    head = new node(12+2*num, 12+2*num);
    tail = head;
    pretail = new node(-1, -1);
    HP = 1;
    score = 0;
    dir = up;
    speed = 600;
    interval = 0;

    if(number == 0){
        head->image.load(":/Images/green_head_up.png");
    } else{
        head->image.load(":/Images/red_head_up.png");
    }
}
