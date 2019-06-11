#include "towerposition.h"
#include "mainwindow.h"
#include <QPainter>

const  QSize towerposition:: fixedSize(140,140);

towerposition::towerposition(QPoint pos,const QPixmap & sprite ):
    t_pos(pos),
    t_haveTower(true),
    t_sprite(sprite)
{

}

const QPoint towerposition::centerPos() {
    QPoint offsetpoint(fixedSize.width()/2,fixedSize.height()/2);
    return t_pos+offsetpoint;
}

bool towerposition::containTower(const QPoint &pos) {
    bool X=(t_pos.x()<pos.x())&&(pos.x()<(t_pos.x()+fixedSize.width()-80));
    bool Y=(t_pos.y()<pos.y())&&(pos.y()<((t_pos.y())+fixedSize.height()-80));
    return X&&Y;
}

bool towerposition::canhaveTower(){
    return t_haveTower;
}

void towerposition::drawTower(QPainter *painter) const {
    painter->drawPixmap(t_pos.x(),t_pos.y(),t_sprite);
}

void towerposition::sethaveTower(Tower *a) {
    t_haveTower=false;
    tower=a;
}

Tower* towerposition::getTower() {
    return tower;
}

Gun* towerposition::getGun() {
    return gun;
}

void towerposition::sethaveTower(Gun *a) {
    t_haveTower=false;
    gun=a;
}
