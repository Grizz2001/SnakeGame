// --------------------------------------------------------------
// Copyright (c) 2020 Wang Haotian in SJTU. All rights reserved.
// --------------------------------------------------------------
#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    gamewidget = nullptr;

    this->resize(1000, 800);
    this->setWindowTitle("Grizz's Snake");
    this->setWindowIcon(QIcon(":/Images/green_head_up.png"));

    /* set background image */
    QPalette pal = this->palette();
    pal.setBrush(QPalette::Background, QBrush(QImage(":/Images/background.png")));
    this->setPalette(pal);

    /* set button */
    OneStartButton = new QPushButton(this);
    TwoStartButton = new QPushButton(this);
    OneContinueButton = new QPushButton(this);
    TwoContinueButton = new QPushButton(this);
    exitButton = new QPushButton(this);

    OneStartButton->setIconSize(QSize(100, 100));    // icon size
    OneStartButton->setGeometry(150, 500, 100, 100); // set location
    OneStartButton->setIcon(QIcon(":/Images/widget_start.png")); // set icon

    TwoStartButton->setIconSize(QSize(100, 100));    // icon size
    TwoStartButton->setGeometry(300, 500, 100, 100); // set location
    TwoStartButton->setIcon(QIcon(":/Images/widget_start.png")); // set icon

    OneContinueButton->setIconSize(QSize(100, 100));    // icon size
    OneContinueButton->setGeometry(450, 500, 100, 100); // set location
    OneContinueButton->setIcon(QIcon(":/Images/widget_continue.png")); // set icon

    TwoContinueButton->setIconSize(QSize(100, 100));    // icon size
    TwoContinueButton->setGeometry(600, 500, 100, 100); // set location
    TwoContinueButton->setIcon(QIcon(":/Images/widget_continue.png")); // set icon

    exitButton->setIconSize(QSize(100, 100));    // icon size
    exitButton->setGeometry(750, 500, 100, 100); // set location
    exitButton->setIcon(QIcon(":/Images/widget_exit.png")); // set icon

    connect(OneStartButton, SIGNAL(clicked(bool)), this, SLOT(OnePlayerStart()));
    connect(TwoStartButton, SIGNAL(clicked(bool)), this, SLOT(TwoPlayerStart()));
    connect(OneContinueButton, SIGNAL(clicked(bool)), this, SLOT(OnePlayerContinue()));
    connect(TwoContinueButton, SIGNAL(clicked(bool)), this, SLOT(TwoPlayerContinue()));
    connect(exitButton, SIGNAL(clicked(bool)), this, SLOT(mainExit()));
}

Widget::~Widget()
{
    delete OneStartButton;
    delete TwoStartButton;
    delete OneContinueButton;
    delete TwoContinueButton;
    delete exitButton;

    if(gamewidget != nullptr) delete gamewidget;
}

void Widget::OnePlayerStart()
{
    gamewidget = new SubWidget(1, this);
    gamewidget->show();
    gamewidget->setAttribute(Qt::WA_DeleteOnClose);
}

void Widget::TwoPlayerStart()
{
    gamewidget = new SubWidget(2, this);
    gamewidget->show();
    gamewidget->setAttribute(Qt::WA_DeleteOnClose);
}

void Widget::OnePlayerContinue()
{
    std::string filename = "snakedata1.txt";
    gamewidget = new SubWidget(filename);
    gamewidget->show();
    gamewidget->setAttribute(Qt::WA_DeleteOnClose);
}

void Widget::TwoPlayerContinue()
{
    std::string filename = "snakedata2.txt";
    gamewidget = new SubWidget(filename);
    gamewidget->show();
    gamewidget->setAttribute(Qt::WA_DeleteOnClose);
}

void Widget::mainExit()
{
    this->close();
    delete this;
}

