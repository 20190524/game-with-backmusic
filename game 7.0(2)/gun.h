#ifndef GUN_H
#define GUN_H

#include "tower.h"
#include "mainwindow.h"
#include <QPoint>
#include <QSize>
#include <QPixmap>
#include <QObject>

class QPainter;
class MainWindow;
class QTimer;
class Enemy;
class Bullet;

class Gun: public Tower
{
    Q_OBJECT
public:
    Gun(QPoint pos, MainWindow* game,int _level);
    ~Gun();
    Gun* upgrade();
    void drawTower(QPainter *painter);

    int level;

private:
    bool t_attack;
    int t_attackRange;
    int t_attackDamage;
    int t_attackBreak;
    qreal t_rotationSprite;

    const QPoint t_pos;
    const QPixmap t_sprite1;
    const QPixmap t_sprite2;
    const QPixmap t_sprite3;
    QTimer * t_attackBreakTimer;
    MainWindow * t_game;
    Enemy * t_chooseEnemy;

    static const QSize fixedSize;

};

#endif // GUN_H
