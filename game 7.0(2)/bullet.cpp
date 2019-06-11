#include "bullet.h"
#include "enemy.h"
#include "mainwindow.h"
#include <QPainter>
#include <QPropertyAnimation>
#include<QMediaPlayer>
Bullet::Bullet(QPoint startPos, QPoint targetPoint, int damage, Enemy *target,
               MainWindow *game, const QPixmap &sprite ):
    b_towerPos(startPos),
    b_targetPos(targetPoint),
    b_attackdamage(damage),
    b_target(target),
    b_game(game),
    b_sprite(sprite),
    b_currentPos(startPos)
{

}


void Bullet::move()
{
    // 100毫秒内击中敌人
    static const int duration = 100;

    QPropertyAnimation *animation = new QPropertyAnimation(this,"b_currentPos");//动画
    animation->setDuration(duration);
    animation->setStartValue(b_towerPos);
    animation->setEndValue(b_targetPos);
    connect(animation, SIGNAL(finished()), this, SLOT(hitTarget()));

    animation->start();
}

void Bullet::hitTarget() {
    if (b_game->enemyList1().indexOf(b_target)!=-1) {
        b_target->getDamage(b_attackdamage);
    }
    QMediaPlayer * player = new QMediaPlayer;
    player->setMedia(QUrl("qrc:/sound/shoot.mp3"));
    player->setVolume(30);
    player->play();
    b_game->removeBullet(this);
}

void Bullet::drawBullet(QPainter *painter) const
{
    painter->drawPixmap(b_currentPos, b_sprite);
}

void Bullet::setCurrentPos(QPoint pos)
{
    b_currentPos = pos;
}

QPoint Bullet::currentPos() const
{
    return b_currentPos;
}
