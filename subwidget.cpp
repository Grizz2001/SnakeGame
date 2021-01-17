// --------------------------------------------------------------
// Copyright (c) 2020 Wang Haotian in SJTU. All rights reserved.
// --------------------------------------------------------------
#include "subwidget.h"

SubWidget::SubWidget(int num, QWidget *)
{
    food_obj = nullptr;
    obstacle  = nullptr;
    Timer  = nullptr;
    buttonStart = nullptr;
    buttonExit = nullptr;
    buttonSave = nullptr;
    buttonAuto = nullptr;
    buttonRestart = nullptr;
    for(int i = 0;i < 2;++i){
        Snake_obj[i] = nullptr;
        ScoreTitle[i] = nullptr;
        Score[i] = nullptr;
        HPTitle[i] = nullptr;
        HP[i] = nullptr;
    }

    this->resize(1200, 810);
    this->setWindowTitle("Snake Game");

    /* set background */
    QPalette pal = this->palette();
    pal.setBrush(QPalette::Background, QBrush(QImage(":/Images/background2.png")));
    this->setPalette(pal);

    /* set property */
    player = num;

    for(int i = 0;i < player;++i){
        Snake_obj[i] = new Snake(i);

        ScoreTitle[i] = new QLabel(this);
        ScoreTitle[i]->setText("Score:");
        ScoreTitle[i]->setGeometry(950+i*100, 200, 100, 60);

        Score[i] = new QLabel(this);
        Score[i]->setText(QString::number(Snake_obj[i]->score));
        Score[i]->setGeometry(950+i*100, 300, 100, 60);

        HPTitle[i] = new QLabel(this);
        HPTitle[i]->setText("HP:");
        HPTitle[i]->setGeometry(950+i*100, 400, 100, 60);

        HP[i] = new QLabel(this);
        HP[i]->setText(QString::number(Snake_obj[i]->HP));
        HP[i]->setGeometry(950+i*100, 500, 100, 60);
    }

    Timer = new QTimer(this);
    connect(Timer, SIGNAL(timeout()), this, SLOT(runSnake()));

    obstacle = new Obstacle;

    food_obj = new Food(Snake_obj, *obstacle, num);

    buttonRestart = new QPushButton(this);
    buttonRestart->setIconSize(QSize(50, 50));
    buttonRestart->setGeometry(880, 600, 50, 50);
    buttonRestart->setIcon(QIcon(":/Images/widget_restart.png"));
    connect(buttonRestart, SIGNAL(clicked(bool)), this, SLOT(restartSnake()));

    buttonSave = new QPushButton(this);
    buttonSave->setIconSize(QSize(50, 50));
    buttonSave->setGeometry(940, 600, 50, 50);
    buttonSave->setIcon(QIcon(":/Images/widget_save.png"));
    connect(buttonSave, SIGNAL(clicked(bool)), this, SLOT(saveSnake()));

    buttonStart = new QPushButton(this);
    buttonStart->setIconSize(QSize(50, 50));
    buttonStart->setGeometry(1000,600, 50, 50);
    buttonStart->setIcon(QIcon(":/Images/widget_start.png"));
    connect(buttonStart, SIGNAL(clicked(bool)), this, SLOT(runSnake()));

    buttonExit = new QPushButton(this);
    buttonExit->setIconSize(QSize(50, 50));
    buttonExit->setGeometry(1060, 600, 50, 50);
    buttonExit->setIcon(QIcon(":/Images/widget_exit.png"));
    connect(buttonExit, SIGNAL(clicked(bool)), this, SLOT(exitSnake()));

    if(player == 1){
        buttonAuto = new QPushButton(this);
        buttonAuto->setIconSize(QSize(50, 50));
        buttonAuto->setGeometry(1120, 600, 50, 50);
        buttonAuto->setIcon(QIcon(":/Images/widget_auto.png"));
        connect(buttonAuto, SIGNAL(clicked(bool)), this, SLOT(autoSnake()));
    }

    imageWall.load(":/Images/wall.png");
    imageWallLight.load(":/Images/wallLight.png");
    pauseConnected = false;
    autoRunning = false;

//    exitBox = new QMessageBox;
}

SubWidget::SubWidget(std::string &filename, QWidget *)
{
    food_obj = nullptr;
    obstacle  = nullptr;
    Timer  = nullptr;
    buttonStart = nullptr;
    buttonExit = nullptr;
    buttonSave = nullptr;
    buttonAuto = nullptr;
    buttonRestart = nullptr;
    for(int i = 0;i < 2;++i){
        Snake_obj[i] = nullptr;
        ScoreTitle[i] = nullptr;
        Score[i] = nullptr;
        HPTitle[i] = nullptr;
        HP[i] = nullptr;
    }

    this->resize(1200, 810);
    this->setWindowTitle("Snake Game");

    std::fstream file;
    std::vector<int> snake_data;
    file.open(filename);
    int number;
    file >> number;
    player = number;
    // errors will occur when there is no such file
    // to be changed here

    for(int i = 0;i < number;++i){
        int length;
        file >> length;
        int x, y, px, py, DIR, health, SCORE, SPEED;
        for(int i = 0;i < length;++i){
            file >> x >> y;
            snake_data.push_back(x);
            snake_data.push_back(y);
        }
        file >> px >> py >> DIR >> health >> SCORE >> SPEED;

        Snake_obj[i] = new Snake(i, length, snake_data, px, py,
                              DIR, health, SCORE, SPEED);
        snake_data.clear(); // Attention! It is important!
    }

    // obstacle data
    int arrX[8];
    int arrY[8];
    for(int i = 0;i < 8;++i){
        file >> arrX[i];
    }

    for(int i = 0;i < 8;++i){
        file >> arrY[i];
    }
    obstacle = new Obstacle(arrX, arrY);

    // food data
    int type, x, y;
    file >> x >> y >> type;
    food_obj = new Food(x, y, type);

    file.close();

    /* set background */
    QPalette pal = this->palette();
    pal.setBrush(QPalette::Background, QBrush(QImage(":/Images/background2.png")));
    this->setPalette(pal);

    // codes below are the same as the constructor with no parameter
    for(int i = 0;i < player;++i){
        ScoreTitle[i] = new QLabel(this);
        ScoreTitle[i]->setText("Score:");
        ScoreTitle[i]->setGeometry(950+i*100, 200, 100, 60);

        Score[i] = new QLabel(this);
        Score[i]->setText(QString::number(Snake_obj[i]->score));
        Score[i]->setGeometry(950+i*100, 300, 100, 60);

        HPTitle[i] = new QLabel(this);
        HPTitle[i]->setText("HP:");
        HPTitle[i]->setGeometry(950+i*100, 400, 100, 60);

        HP[i] = new QLabel(this);
        HP[i]->setText(QString::number(Snake_obj[i]->HP));
        HP[i]->setGeometry(950+i*100, 500, 100, 60);
    }

    Timer = new QTimer(this);
    connect(Timer, SIGNAL(timeout()), this, SLOT(runSnake()));

    buttonRestart = new QPushButton(this);
    buttonRestart->setIconSize(QSize(50, 50));
    buttonRestart->setGeometry(880, 600, 50, 50);
    buttonRestart->setIcon(QIcon(":/Images/widget_restart.png"));
    connect(buttonRestart, SIGNAL(clicked(bool)), this, SLOT(restartSnake()));

    buttonSave = new QPushButton(this);
    buttonSave->setIconSize(QSize(50, 50));
    buttonSave->setGeometry(940, 600, 50, 50);
    buttonSave->setIcon(QIcon(":/Images/widget_save.png"));
    connect(buttonSave, SIGNAL(clicked(bool)), this, SLOT(saveSnake()));

    buttonStart = new QPushButton(this);
    buttonStart->setIconSize(QSize(50, 50));
    buttonStart->setGeometry(1000,600, 50, 50);
    buttonStart->setIcon(QIcon(":/Images/widget_start.png"));
    connect(buttonStart, SIGNAL(clicked(bool)), this, SLOT(runSnake()));

    buttonExit = new QPushButton(this);
    buttonExit->setIconSize(QSize(50, 50));
    buttonExit->setGeometry(1060, 600, 50, 50);
    buttonExit->setIcon(QIcon(":/Images/widget_exit.png"));
    connect(buttonExit, SIGNAL(clicked(bool)), this, SLOT(exitSnake()));

    if(player == 1){
        buttonAuto = new QPushButton(this);
        buttonAuto->setIconSize(QSize(50, 50));
        buttonAuto->setGeometry(1120, 600, 50, 50);
        buttonAuto->setIcon(QIcon(":/Images/widget_auto.png"));
        connect(buttonAuto, SIGNAL(clicked(bool)), this, SLOT(autoSnake()));
    }

    imageWall.load(":/Images/wall.png");
    imageWallLight.load(":/Images/wallLight.png");
    pauseConnected = false;
    autoRunning = false;

//    exitBox = new QMessageBox;
}

void SubWidget::keyPressEvent(QKeyEvent *k)
{
    // forbide key press event during auto-running
    if(autoRunning) return;

    if(player == 1){
        switch(k->key())
        {
        case Qt::Key_W:
            Snake_obj[0]->toUp();
            update();
            break;
        case Qt::Key_A:
            Snake_obj[0]->toLeft();
            update();
            break;
        case Qt::Key_S:
            Snake_obj[0]->toDown();
            update();
            break;
        case Qt::Key_D:
            Snake_obj[0]->toRight();
            update();
            break;
        default:
            break;
        }
    }
    else{
        switch(k->key())
        {
        case Qt::Key_W:
            Snake_obj[0]->toUp();
            update();
            break;
        case Qt::Key_A:
            Snake_obj[0]->toLeft();
            update();
            break;
        case Qt::Key_S:
            Snake_obj[0]->toDown();
            update();
            break;
        case Qt::Key_D:
            Snake_obj[0]->toRight();
            update();
            break;
        case Qt::Key_I:
            Snake_obj[1]->toUp();
            update();
            break;
        case Qt::Key_J:
            Snake_obj[1]->toLeft();
            update();
            break;
        case Qt::Key_K:
            Snake_obj[1]->toDown();
            update();
            break;
        case Qt::Key_L:
            Snake_obj[1]->toRight();
            update();
            break;
        default:
            break;
        }
    }
}

void SubWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    /* draw Wall */
    int x, y;
    for(x = 0 , y = 0 ; x <= 26; ++x)
        painter.drawImage(QRect(30*x, 30*y, 30, 30), imageWall);
    for(x = 0 , y = 26; x <= 26; ++x)
        painter.drawImage(QRect(30*x, 30*y, 30, 30), imageWall);
    for(x = 0 , y = 1 ; y <= 25; ++y)
        painter.drawImage(QRect(30*x, 30*y, 30, 30), imageWall);
    for(x = 26, y = 1 ; y <= 25; ++y)
        painter.drawImage(QRect(30*x, 30*y, 30, 30), imageWall);

    /* draw obstacles */
    for(int i = 0;i < 8;++i){
        x = obstacle->arrayX[i];
        y = obstacle->arrayY[i];
        if(i != obstacle->chosen)
            painter.drawImage(QRect(30*x, 30*y, 30, 30), imageWall);
        else
            painter.drawImage(QRect(30*x, 30*y, 30, 30), imageWallLight);
    }

    /* draw Snake */
    for(int i = 0;i < player;++i){
        node *tmp = Snake_obj[i]->head;
        while(tmp != nullptr){
            painter.drawImage(QRect(30*tmp->x, 30*tmp->y, 30, 30), tmp->image);
            tmp = tmp->next;
        }
    }

    /* draw Food */
    painter.drawImage(QRect(30*food_obj->x, 30*food_obj->y, 30, 30), food_obj->image);
}

void SubWidget::mousePressEvent(QMouseEvent *m)
{
    if(m->button() != Qt::LeftButton) return;
    if(pauseConnected) return;

    int mx, my;
    mx = m->x();
    my = m->y();
    mx /= 30;
    my /= 30;

    if(obstacle->chosen == 8){// obstacle is unchosen
        for(int i = 0;i < 8;++i){
            if(mx == obstacle->arrayX[i] && my == obstacle->arrayY[i]){
                obstacle->chosen = i;// choose an obstacle
                update();
                return; // success
            }
        }
    }else{// obstacle is chosen, change its position
        if(mx == food_obj->x && my == food_obj->y) return;
        if(mx < 1 || mx > 25 || my < 1 || my > 25) return;
        if(!SnakeFree(mx, my)) return;
        for(int i = 0;i < 8;++i){
            if(mx == obstacle->arrayX[i] && my == obstacle->arrayY[i] && i != obstacle->chosen) return;
        }

        obstacle->arrayX[obstacle->chosen] = mx;
        obstacle->arrayY[obstacle->chosen] = my;
        obstacle->chosen = 8;
        update();
        return;
    }

    if(mx == food_obj->x && my == food_obj->y){
        food_obj->changeFood();
        update();
        return;
    }
}

bool SubWidget::SnakeFree(int x, int y)
{
    node *tmp;
    for(int i = 0;i < player;++i){
        tmp = Snake_obj[i]->head;
        while(tmp != nullptr){
            if(x == tmp->x && y == tmp->y) return false;
            tmp = tmp->next;
        }
    }

    return true;
}

bool SubWidget::ObstacleFree(int x, int y)
{
    for(int i = 0;i < 8;++i){
        if(obstacle->arrayX[i] == x && obstacle->arrayY[i] == y)
            return false;
    }
    return true;
}

bool SubWidget::FoodFree(int x, int y)
{
    if(x == food_obj->x && y == food_obj->y) return false;
    return true;
}

void SubWidget::SnakeReborn(int num)
{
    int x = 10 + 2*num, y = 10 + 2*num;
    bool flag = 1;
    node *tmp = Snake_obj[num]->head;
    while(tmp != nullptr){
        tmp->x = -1;
        tmp->y = -1;
        tmp = tmp->next;
    }

    std::stack<int> xstack, ystack;

    while(!recursion(Snake_obj[num]->head, x, y, xstack, ystack)){
        if(flag == 1) x++;
        else y++;
        flag = !flag;
    }
    tmp = Snake_obj[num]->head;
    while(tmp != nullptr){
        tmp->x = xstack.top();
        tmp->y = ystack.top();
        xstack.pop();
        ystack.pop();
        tmp = tmp->next;
    }

    Snake_obj[num]->HP--;
    Snake_obj[num]->dir = up;
    if(num == 0)
        Snake_obj[num]->head->image.load(":/Images/green_head_up.png");
    else
        Snake_obj[num]->head->image.load(":/Images/red_head_up.png");
    Snake_obj[num]->updateTail();
}

bool SubWidget::recursion(node *&tmp, int x, int y,
                          std::stack<int> &xstack, std::stack<int> &ystack)
{
    if(tmp->next == nullptr){
        if(ObstacleFree(x,y) && FoodFree(x,y) && SnakeFree(x,y)){
            xstack.push(x);
            ystack.push(y);
            return true;
        }else return false;
    }

    if(!(valid(x, y) && FoodFree(x, y)))
        return false;

    if(recursion(tmp->next, x, y+1, xstack, ystack)){
        xstack.push(x);
        ystack.push(y);
        return true;
    }else if(recursion(tmp->next, x+1, y, xstack, ystack)){
        xstack.push(x);
        ystack.push(y);
        return true;
    }else if(recursion(tmp->next, x-1, y, xstack, ystack)){
        xstack.push(x);
        ystack.push(y);
        return true;
    }else if(recursion(tmp->next, x, y-1, xstack, ystack)){
        xstack.push(x);
        ystack.push(y);
        return true;
    }

    return false;
}

int Manhattan(int x, int y, int tarX, int tarY)
{
    int deltaX = ((x - tarX) >= 0) ? (x - tarX) : (tarX - x);
    int deltaY = ((y - tarY) >= 0) ? (y - tarY) : (tarY - y);
    return (deltaX + deltaY);
}

int F(int x, int y, int tarX, int tarY, int G)
{
    return (G + Manhattan(x, y, tarX, tarY));
}

bool SubWidget::AStar()
{
    open.clear();
    closed.clear();
    while(!path.empty()) path.pop();

    int iniX = Snake_obj[0]->head->x;
    int iniY = Snake_obj[0]->head->y;
    int tarX = food_obj->x;
    int tarY = food_obj->y;

    coordinate initial(iniX, iniY, iniX, iniY, 0);
    initial.F = F(initial.x, initial.y, tarX, tarY, initial.G);
    open.push_back(initial);

    while(true){
        if(open.empty()){
//            QMessageBox::information(this, "ExitGame", "Found No Possible Way", QMessageBox::Yes);
//            exitSnake();
            return false;
        }

        std::vector<coordinate>::iterator search = closed.begin();
        for(;search != closed.end();++search){
            if((*search).x == tarX && (*search).y == tarY) break;
        }
        if(search != closed.end()) break;

        std::vector<coordinate>::iterator cur = open.begin();
        int curF = (*cur).F;
        for(std::vector<coordinate>::iterator itr = open.begin();
            itr != open.end();++itr){
            if((*itr).F < curF){
                curF = (*itr).F;
                cur = itr;
            }
        }

        coordinate curC = *cur; // current coordinate
        closed.push_back(curC);
        open.erase(cur);

        // 1
        if(valid(curC.x + 1, curC.y)){
            // in Closed or not
            std::vector<coordinate>::iterator itr = closed.begin();
            for(;itr != closed.end();++itr){
                if((*itr).x == curC.x + 1 && (*itr).y == curC.y) break;
            }

            // Do the following operation when current
            // coordinate's neighbor is not in Closed
            if(itr == closed.end()){ // not in Closed
                itr = open.begin();
                for(;itr != open.end();++itr){
                    if((*itr).x == curC.x + 1 && (*itr).y == curC.y) break;
                }

                // not in Open
                if(itr == open.end()){
                    coordinate tmp(curC.x + 1, curC.y, curC.x, curC.y, curC.G + 1);
                    tmp.F = F(tmp.x, tmp.y, tarX, tarY, tmp.G);
                    open.push_back(tmp);
                }else{ // already in open
                    if(curC.G + 1 < (*itr).G){
                        (*itr).parentx = curC.x;
                        (*itr).parenty = curC.y;
                        (*itr).G = curC.G + 1;
                        (*itr).F = F((*itr).x, (*itr).y, (*itr).parentx, (*itr).parenty, (*itr).G);
                    }
                }
            }
            // if the current coordinate's beighbor is in Closed, skip it
        }

        // 2
        if(valid(curC.x, curC.y + 1)){
            std::vector<coordinate>::iterator itr = closed.begin();
            for(;itr != closed.end();++itr){
                if((*itr).x == curC.x && (*itr).y == curC.y + 1) break;
            }

            if(itr == closed.end()){
                itr = open.begin();
                for(;itr != open.end();++itr){
                    if((*itr).x == curC.x && (*itr).y == curC.y + 1) break;
                }

                if(itr == open.end()){
                    coordinate tmp(curC.x, curC.y + 1, curC.x, curC.y, curC.G + 1);
                    tmp.F = F(tmp.x, tmp.y, tarX, tarY, tmp.G);
                    open.push_back(tmp);
                }else{
                    if(curC.G + 1 < (*itr).G){
                        (*itr).parentx = curC.x;
                        (*itr).parenty = curC.y;
                        (*itr).G = curC.G + 1;
                        (*itr).F = F((*itr).x, (*itr).y, (*itr).parentx, (*itr).parenty, (*itr).G);
                    }
                }
            }
        }

        // 3
        if(valid(curC.x - 1, curC.y)){
            std::vector<coordinate>::iterator itr = closed.begin();
            for(;itr != closed.end();++itr){
                if((*itr).x == curC.x - 1 && (*itr).y == curC.y) break;
            }

            if(itr == closed.end()){
                itr = open.begin();
                for(;itr != open.end();++itr){
                    if((*itr).x == curC.x - 1 && (*itr).y == curC.y) break;
                }

                if(itr == open.end()){
                    coordinate tmp(curC.x - 1, curC.y, curC.x, curC.y, curC.G + 1);
                    tmp.F = F(tmp.x, tmp.y, tarX, tarY, tmp.G);
                    open.push_back(tmp);
                }else{
                    if(curC.G + 1 < (*itr).G){
                        (*itr).parentx = curC.x;
                        (*itr).parenty = curC.y;
                        (*itr).G = curC.G + 1;
                        (*itr).F = F((*itr).x, (*itr).y, (*itr).parentx, (*itr).parenty, (*itr).G);
                    }
                }
            }
        }

        // 4
        if(valid(curC.x, curC.y - 1)){
            std::vector<coordinate>::iterator itr = closed.begin();
            for(;itr != closed.end();++itr){
                if((*itr).x == curC.x && (*itr).y == curC.y - 1) break;
            }

            if(itr == closed.end()){
                itr = open.begin();
                for(;itr != open.end();++itr){
                    if((*itr).x == curC.x && (*itr).y == curC.y - 1) break;
                }

                if(itr == open.end()){
                    coordinate tmp(curC.x, curC.y - 1, curC.x, curC.y, curC.G + 1);
                    tmp.F = F(tmp.x, tmp.y, tarX, tarY, tmp.G);
                    open.push_back(tmp);
                }else{
                    if(curC.G + 1 < (*itr).G){
                        (*itr).parentx = curC.x;
                        (*itr).parenty = curC.y;
                        (*itr).G = curC.G + 1;
                        (*itr).F = F((*itr).x, (*itr).y, (*itr).parentx, (*itr).parenty, (*itr).G);
                    }
                }
            }
        }
    }

    // construct path from Closed
    while(!(tarX == Snake_obj[0]->head->x && tarY == Snake_obj[0]->head->y)){
        for(std::vector<coordinate>::iterator tmp = closed.begin();
            tmp != closed.end();++tmp){
            if((*tmp).x == tarX && (*tmp).y == tarY){
                path.push(pathNode(tarX, tarY));
                tarX = (*tmp).parentx;
                tarY = (*tmp).parenty;
                break;
            }
        }
    }
    return true;
}

bool SubWidget::valid(int x, int y)
{
    if(x > 25 || x < 1 || y > 25 || y < 1) return false;
    return (SnakeFree(x,y) && ObstacleFree(x,y));
}

void SubWidget::runSnake()
{
    obstacle->chosen = 8;

    if(player == 1){
        if(Snake_obj[0]->interval < Snake_obj[0]->speed) Snake_obj[0]->interval += 100;
        else{
            if(autoRunning){
                if(path.empty()){
                    if(!AStar()){
                        QMessageBox::information(this, "ExitGame", "Found No Possible Way", QMessageBox::Yes);
                        exitSnake();
                    }
                }

                int tmpx = path.top().x;
                int tmpy = path.top().y;

                path.pop();

                if(tmpx > Snake_obj[0]->head->x) Snake_obj[0]->toRight();
                else if(tmpx < Snake_obj[0]->head->x) Snake_obj[0]->toLeft();
                else if(tmpy > Snake_obj[0]->head->y) Snake_obj[0]->toDown();
                else if(tmpy < Snake_obj[0]->head->y) Snake_obj[0]->toUp();
            }

            Snake_obj[0]->move();

            if(!Snake_obj[0]->isAlive(Snake_obj[0], *obstacle, 0)){
                if(!Snake_obj[0]->reborn()){
                    exitSnake(); // exit button
                    return;
                }else{
                    SnakeReborn(0);
                    update();
                }
            }
            else{
                if(Snake_obj[0]->ifGrow(food_obj))
                    food_obj->reset(Snake_obj, *obstacle, player);
                Score[0]->setText(QString::number(Snake_obj[0]->score)); // set Score
                HP[0]->setText(QString::number(Snake_obj[0]->HP)); // set HP
                update();
            }
            Snake_obj[0]->interval = 0;
        }
    }

    if(player == 2){
        if(Snake_obj[0]->interval < Snake_obj[0]->speed) Snake_obj[0]->interval += 100;
        else{
            Snake_obj[0]->move();

            if((!Snake_obj[0]->isAlive(Snake_obj[0], *obstacle, 0))
                    || (!Snake_obj[0]->isAlive(Snake_obj[1], *obstacle, 1))){
                if(!Snake_obj[0]->reborn())
                    exitSnake(); // exit button
                else{
                    SnakeReborn(0);
                    update();
                }
            }
            else{
                if(Snake_obj[0]->ifGrow(food_obj))
                    food_obj->reset(Snake_obj, *obstacle, player);
                Score[0]->setText(QString::number(Snake_obj[0]->score)); // set Score
                HP[0]->setText(QString::number(Snake_obj[0]->HP)); // set HP
                update();
            }
            Snake_obj[0]->interval = 0;
        }

        if(Snake_obj[1]->interval < Snake_obj[1]->speed) Snake_obj[1]->interval += 100;
        else{
            Snake_obj[1]->move();

            if((!Snake_obj[1]->isAlive(Snake_obj[0], *obstacle, 1))
                    || (!Snake_obj[1]->isAlive(Snake_obj[1], *obstacle, 0))){
                if(!Snake_obj[1]->reborn())
                    exitSnake(); // exit button
                else{
                    SnakeReborn(1);
                    update();
                }
            } else {
                if(Snake_obj[1]->ifGrow(food_obj)){
                    food_obj->reset(Snake_obj, *obstacle, player);
                    Score[1]->setText(QString::number(Snake_obj[1]->score)); // set Score
                    HP[1]->setText(QString::number(Snake_obj[1]->HP)); // set HP
                    update();
                }
            }
            Snake_obj[1]->interval = 0;
        }
    }

    if(!pauseConnected)
    {
        buttonStart->setIcon(QIcon(":/Images/widget_pause.png"));
        buttonStart->disconnect(SIGNAL(clicked(bool)));
        connect(buttonStart, SIGNAL(clicked(bool)), this, SLOT(pauseSnake()));
        pauseConnected = true;
    }

    Timer->start(65);

}

void SubWidget::exitSnake()
{
    QString info1 = "Player 1 Score: ", info2 = " ;Player 2 Score: ";
    QString tmp = QString::number(Snake_obj[0]->score);
    info1 += tmp;
    if(player == 2){
        tmp = QString::number(Snake_obj[1]->score);
        info2 += tmp;
        info1 += info2;
    }
    pauseSnake();
    QMessageBox::information(this, "ExitGame", info1, QMessageBox::Yes);
    this->close();
//    something unexpected will here and end the process
//    forcefully if I delete the "this" pointer

//    delete this;
}

void SubWidget::pauseSnake()
{
    if(autoRunning){
        autoRunning = !autoRunning;
        buttonAuto->setIcon(QIcon(":/Images/widget_auto.png"));
    }
    Timer->stop();
    buttonStart->setIcon(QIcon(":/Images/widget_start.png"));
    buttonStart->disconnect(SIGNAL(clicked(bool)));
    connect(buttonStart, SIGNAL(clicked(bool)), this, SLOT(runSnake()));
    pauseConnected = false;
}

void SubWidget::saveSnake()
{
    std::fstream file;
    if(player == 1) file.open("snakedata1.txt", std::ios::out);
    else file.open("snakedata2.txt", std::ios::out);

    file << player << ' ';

    /* save snake data */
    for(int i = 0;i < player;++i){
        int length = 0;
        node *tmp = Snake_obj[i]->head;
        while(tmp != nullptr){
            tmp = tmp->next;
            ++length;
        }
        file << length << ' ';
        tmp = Snake_obj[i]->head;
        while(tmp != nullptr){
            file << tmp->x << ' ' << tmp->y << ' ';
            tmp = tmp->next;
        }

        file << Snake_obj[i]->pretail->x << ' '
             << Snake_obj[i]->pretail->y << ' ';

        if(Snake_obj[i]->dir == up) file << 1 << ' ';
        else if(Snake_obj[i]->dir == right) file << 2 << ' ';
        else if(Snake_obj[i]->dir == down) file << 3 << ' ';
        else if(Snake_obj[i]->dir == left) file << 4 << ' ';

        file << Snake_obj[i]->HP << ' ';
        file << Snake_obj[i]->score << ' ';
        file << Snake_obj[i]->speed << ' ';
    }
    QMessageBox::information(this, "Save the Snake",
                             "Data has been saved, and you can fetch it in your working directory.", QMessageBox::Yes);

    /* save obstacle data */
    for(int i = 0;i < 8;++i){
        file << obstacle->arrayX[i] << ' ';
    }
    for(int i = 0;i < 8;++i){
        file << obstacle->arrayY[i] << ' ';
    }

    /* save food data */
    file << food_obj->x << ' ' << food_obj->y << ' ';

    if(food_obj->ftp == ordinary) file << 1 << ' ';
    else if(food_obj->ftp == speedup) file << 2 << ' ';
    else if(food_obj->ftp == slowdown) file << 3 << ' ';
    else if(food_obj->ftp == addHP) file << 4 << ' ';
    else if(food_obj->ftp == doublescore) file << 5 << ' ';

    file.close();
}

void SubWidget::autoSnake()
{
    if(autoRunning){
        buttonAuto->setIcon(QIcon(":/Images/widget_auto.png"));
        autoRunning = !autoRunning;
        pauseSnake();
    }else{
        if(pauseConnected) return;
        else
        {
            buttonAuto->setIcon(QIcon(":/Images/widget_auto2.png"));
            autoRunning = !autoRunning;
            if(AStar()) runSnake();
            else{
                QMessageBox::information(this, "ExitGame", "Found No Possible Way", QMessageBox::Yes);
                exitSnake();
            }
        }
    }
}

void SubWidget::restartSnake()
{
    if(pauseConnected) return;
    else
    {
        if(player == 1){
            Snake_obj[0]->reset(0);
            obstacle->reset();
            food_obj->reset(Snake_obj, *obstacle, 1);
            Score[0]->setText(QString::number(Snake_obj[0]->score)); // set Score
            HP[0]->setText(QString::number(Snake_obj[0]->HP)); // set HP
        }
        else{
            Snake_obj[0]->reset(0);
            Snake_obj[1]->reset(1);
            obstacle->reset();
            food_obj->reset(Snake_obj, *obstacle, 1);
            Score[0]->setText(QString::number(Snake_obj[0]->score)); // set Score
            HP[0]->setText(QString::number(Snake_obj[0]->HP)); // set HP
            Score[1]->setText(QString::number(Snake_obj[1]->score)); // set Score
            HP[1]->setText(QString::number(Snake_obj[1]->HP)); // set HP
        }
    }
    update();
}

SubWidget::~SubWidget()
{
    delete food_obj;

    for(int i = 0;i < player;++i){
        delete Snake_obj[i];
        delete ScoreTitle[i];
        delete Score[i];
        delete HPTitle[i];
        delete HP[i];
    }

    if(player == 1) delete buttonAuto;

    delete obstacle;
    delete Timer;
    delete buttonStart;
    delete buttonExit;
    delete buttonSave;
}
