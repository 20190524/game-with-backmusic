#include "enemy.h"
#include "waypoint.h"
#include "tower.h"
#include "utility.h"
#include "mainwindow.h"
#include "gun.h"
#include <QPainter>
#include <QColor>
#include <QDebug>
#include <QMatrix>
#include <QVector2D>
#include <QtMath>
#include<QMediaPlayer>
static const int Health_Bar_Width = 40;

const QSize Enemy::fixedSize(140,140);

Enemy::Enemy(waypoint *startWayPoint, MainWindow *game, const QPixmap &sprite):QObject(0)
      , e_active(false)
      , e_maxHP(40)
      , e_currentHP(40)
      , e_walkspeed(1.0)
      , e_rotationSprite(0.0)
      , e_pos(startWayPoint->pos())
      , e_desWayPoint(startWayPoint->nextWayPoint())
      , e_game(game)
  , e_sprite(sprite) {

}



void Enemy::draw(QPainter *painter){
    if (!e_active)
        return;

    painter->save();

    QPoint healthBarPoint = e_pos + QPoint(-Health_Bar_Width / 2 - 5, -fixedSize.height() / 3);
    // 绘制血条
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::red);
    QRect healthBarBackRect(healthBarPoint, QSize(Health_Bar_Width, 2));
    painter->drawRect(healthBarBackRect);

    painter->setBrush(Qt::red);
    QRect healthBarRect(healthBarPoint, QSize((double)e_currentHP / e_maxHP * Health_Bar_Width, 2));
    painter->drawRect(healthBarRect);

    // 绘制偏转坐标,由中心+偏移=左上
    static const QPoint offsetPoint(-fixedSize.width() / 2, -fixedSize.height() / 2);
    painter->translate(e_pos);
    painter->rotate(e_rotationSprite);
    // 绘制敌人
    painter->drawPixmap(offsetPoint, e_sprite);

    painter->restore();
}


void Enemy::move(){
    if (!e_active)
        return;

    if (collisionWithCircle(e_pos, 1, e_desWayPoint->pos()))
    {
        // 敌人抵达了一个航点
        if (e_desWayPoint->nextWayPoint())
        {
            // 还有下一个航点
            e_pos = e_desWayPoint->pos();
            e_desWayPoint = e_desWayPoint->nextWayPoint();
        }
        else
        {
            // 表示进入基地
            e_game->getHpDamage();
            e_game->removeEnemy(this);
            return;
        }
    }

    // 还在前往航点的路上
    // 目标航点的坐标
    QPoint targetPoint = e_desWayPoint->pos();
    // 未来修改这个可以添加移动状态,加快,减慢,m_walkingSpeed是基准值

    // 向量标准化
    qreal movementSpeed = e_walkspeed;
    QVector2D normalized(targetPoint - e_pos);
    normalized.normalize();
    e_pos = e_pos + normalized.toPoint() * movementSpeed;

    // 确定敌人选择方向
    // 默认图片向左,需要修正180度转右
    e_rotationSprite = qRadiansToDegrees(qAtan2(normalized.y(), normalized.x())) + 180;
}

void Enemy::do_Activate(){
    e_active=true;
}

void Enemy::getRemoved(Tower *a) {
    if (attackedTowerslist.empty()) return;
    foreach (Tower * attacker, attackedTowerslist) {
        attacker->enemyKilled();
    }
    QMediaPlayer * player = new QMediaPlayer;
    player->setMedia(QUrl("qrc:/sound/dead.mp3"));
    player->setVolume(30);
    player->play();
    e_game->removeEnemy(this);
}

void Enemy::getRemoved(Gun *a) {
    if (attackedGunlist.empty()) return;
    foreach (Gun *gun, attackedGunlist) {
        gun->enemyKilled();
    }
    QMediaPlayer * player = new QMediaPlayer;
    player->setMedia(QUrl("qrc:/sound/dead.mp3"));
    player->setVolume(30);
    player->play();
    e_game->removeEnemy(this);
}

void Enemy::getDamage(int damage) {
    e_currentHP -= damage;
    if (e_currentHP<=0) {
        getRemoved(attackedTowerslist.back());
        e_game->awardGold(200);
    }
}

void Enemy::getAttacked(Tower *attacker) {
    attackedTowerslist.push_back(attacker);
}

void Enemy::getLostSight(Tower *attacker) {
    attackedTowerslist.removeOne(attacker);
}

const QPoint Enemy::pos() {
    return e_pos;
}
