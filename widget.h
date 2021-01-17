// --------------------------------------------------------------
// Copyright (c) 2020 Wang Haotian in SJTU. All rights reserved.
// --------------------------------------------------------------
#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QIcon>
#include <QPalette>
#include <QBrush>
#include <QImage>
#include <QPushButton>
#include <QMessageBox>
#include <string>

#include "subwidget.h"

class Widget : public QWidget
{
    Q_OBJECT

public:
    QPushButton *OneStartButton;
    QPushButton *TwoStartButton;
    QPushButton *OneContinueButton;
    QPushButton *TwoContinueButton;
    QPushButton *exitButton;
    //QMessageBox *exitBox;

    Widget(QWidget *parent = 0);
    ~Widget();

private:
    SubWidget *gamewidget;

private slots:
    void OnePlayerStart();
    void TwoPlayerStart();
    void OnePlayerContinue();
    void TwoPlayerContinue();
    void mainExit();
};

#endif // WIDGET_H
