#ifndef TOWERPLUS_H
#define TOWERPLUS_H

#include <QMainWindow>
#include <QImage>
#include "bullet.h"
#include "utility.h"
#include "tower.h"
#include <QPainter>
#include <QRect>
#include <QBrush>
#include <QFont>
#include <QKeyEvent>
#include <QTimer>
#include <iostream>
#include <QObject>
using namespace std;

class MainWindow;
class Enemy;
class towerplus:public Tower
{
public:
    towerplus();
};

#endif // TOWERPLUS_H
