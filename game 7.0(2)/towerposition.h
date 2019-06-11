#ifndef TOWERPOSITION_H
#define TOWERPOSITION_H

#include <QPoint>
#include <QSize>
#include <QPixmap>
#include "tower.h"

class Gun;


class towerposition
{
public:
    towerposition(QPoint pos,const QPixmap&sprite = QPixmap(":/pictures/posi.png") );
    bool canhaveTower();
    bool containTower(const QPoint &pos);
    const QPoint centerPos();
    void sethaveTower(Tower *tower);
    void sethaveTower(Gun * gun);
    Tower* getTower();
    Gun* getGun();


    void drawTower(QPainter *painter) const;

private:
    bool t_haveTower;
    QPoint t_pos;
    QPixmap t_sprite;

    Tower* tower;
    Gun* gun;

    static const QSize fixedSize;
};

#endif // TOWERPOSITION_H
