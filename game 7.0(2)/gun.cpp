#include "gun.h"
#include "tower.h"
#include "enemy.h"
#include "bullet.h"
#include "mainwindow.h"
#include "utility.h"

#include <QMainWindow>
#include <QImage>
#include <QPainter>
#include <QRect>
#include <QBrush>
#include <QWidget>
#include <QMovie>
#include <QMouseEvent>
#include <QLabel>
#include <QPushButton>
#include <QKeyEvent>
#include <QFont>
#include <QTimer>
#include <iostream>

using namespace std;


const QSize Gun::fixedSize(120,120);

Gun::Gun (QPoint pos, MainWindow* game,int _level):
    t_pos(pos)
  , t_game(game)
  , t_sprite1(QPixmap(":/pictures/power1.png"))
  , t_sprite2(QPixmap(":/pictures/power2.png"))
  , level(_level)
  , t_attack(false)
  , t_rotationSprite(0.0)
  , t_chooseEnemy(NULL)
  , Tower (pos,game,_level)
{
    if (level==4) {
        t_attackRange=400;
        t_attackDamage=20;
        t_attackBreak=(800);
    }
    else if (level==5) {
        t_attackRange=500;
        t_attackDamage=25;
        t_attackBreak=(800);
    }
    t_attackBreakTimer=new QTimer(this);
    connect(t_attackBreakTimer, SIGNAL(timeout()), this, SLOT(shootWeapon()));
}

Gun::~Gun() {
    delete t_attackBreakTimer;
    t_attackBreakTimer=NULL;
}

Gun* Gun::upgrade() {
    level++;
    Gun *gun=new Gun(t_pos,t_game,level);
    t_game->addGun(gun);
    return gun;
}

void Gun::drawTower(QPainter *painter) {
    static const QPoint offsetPoint (-fixedSize.width()/2,-fixedSize.height()/2);
    painter->translate(t_pos);
    if (level ==4 ) painter->drawPixmap(offsetPoint,t_sprite1);
    else if (level==5) painter->drawPixmap(offsetPoint,t_sprite2);
    //else if (level==3) painter->drawPixmap(offsetPoint,t_sprite3);
    painter->restore();
}

