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
#include<QMediaPlayer>
using namespace std;


const QSize Tower::fixedSize(140,140);

Tower::Tower(QPoint pos,MainWindow *game,int _level):
     t_attack(false)
    , t_rotationSprite(0.0)
    , t_chooseEnemy(NULL)
    , t_game(game)
    , t_pos(pos)
    , level(_level)
    , t_sprite1(QPixmap(":/pictures/tower1.png"))
    , t_sprite2(QPixmap(":/pictures/tower2.png"))
    , t_sprite3(QPixmap(":/pictures/tower3.png"))
    {
    if (level==1) {
        t_attackRange=200;
        t_attackDamage=5;
        t_attackBreak=(1500);
    }
    else if (level==2) {
        t_attackRange=250;
        t_attackDamage=10;
        t_attackBreak=(1200);
    }
    else if (level==3) {
        t_attackRange=300;
        t_attackDamage=15;
        t_attackBreak=(1000);
    }
    QMediaPlayer * player = new QMediaPlayer;
    player->setMedia(QUrl("qrc:/sound/loadtower.mp3"));
    player->setVolume(30);
    player->play();
    t_attackBreakTimer=new QTimer(this);
    connect(t_attackBreakTimer, SIGNAL(timeout()), this, SLOT(shootWeapon()));
}

Tower::~Tower()
{
    delete t_attackBreakTimer;
    t_attackBreakTimer=NULL;
}

void Tower::attackEnemy() {
    t_attackBreakTimer->start(t_attackBreak);
}



void Tower::drawTower(QPainter *painter) {
    painter->save();
    //painter->setPen(Qt::white);
    //painter->drawLine(t_pos.x(),t_pos.y(),t_pos.x()+t_attackRange,t_pos.y());
    //攻击范围的绘制


    static const QPoint offsetPoint (-fixedSize.width()/2,-fixedSize.height()/2);
    painter->translate(t_pos);
    if (level ==1 ) painter->drawPixmap(offsetPoint,t_sprite1);
    else if (level==2) painter->drawPixmap(offsetPoint,t_sprite2);
    else if (level==3) painter->drawPixmap(offsetPoint,t_sprite3);
    painter->restore();
}

void Tower::chooseEnemyforAttack(Enemy *enemy) {
    t_chooseEnemy = enemy;
    attackEnemy();
    t_chooseEnemy->getAttacked(this); //敌人类-确定被攻击的函数；
}

void Tower::enemyKilled() {
    if (t_chooseEnemy) t_chooseEnemy = NULL;
    t_attackBreakTimer->stop();
    t_rotationSprite=0.0;
}

void Tower::loseSightofEnmey() {
    t_chooseEnemy->getLostSight(this);//敌人类-离开攻击范围的函数；
    if (t_chooseEnemy) t_chooseEnemy = NULL;
    t_attackBreakTimer->stop();
    t_rotationSprite=0.0;
}

void Tower::shootWeapon()
{
    Bullet *bullet = new Bullet(t_pos, t_chooseEnemy->pos(),t_attackDamage, t_chooseEnemy, t_game);
    bullet->move();
    t_game->addBullet(bullet);
}

void Tower::checkEnemyInRange() {
    if (t_chooseEnemy)
    {
        // 这种情况下,需要旋转炮台对准敌人
        // 向量标准化
        QVector2D normalized(t_chooseEnemy->pos() - t_pos);
        normalized.normalize();
        //t_rotationSprite = qRadiansToDegrees(qAtan2(normalized.y(), normalized.x())) - 90;

        // 如果敌人脱离攻击范围
        if (!collisionWithCircle(t_pos,t_attackRange,t_chooseEnemy->pos()))
            loseSightofEnmey();
    }
    else
    {
        // 遍历敌人,看是否有敌人在攻击范围内
        QList<Enemy *> enemyList = t_game->enemyList1();
        foreach (Enemy *enemy, enemyList)
        {
            if (collisionWithCircle(t_pos, t_attackRange, enemy->pos()))
            {
                chooseEnemyforAttack(enemy);
                break;
            }
        }
    }
}

Tower* Tower::upgrade() {
    level++;
    Tower *tower=new Tower(t_pos,t_game,level);
    t_game->addTower(tower);

    QMediaPlayer * player = new QMediaPlayer;
    player->setMedia(QUrl("qrc:/sound/updatetower.mp3"));
    player->setVolume(30);
    player->play();
    return tower;
}
