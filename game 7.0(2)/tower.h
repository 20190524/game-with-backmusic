#ifndef TOWER_H
#define TOWER_H

#include <QPoint>
#include <QSize>
#include <QPixmap>
#include <QObject>

class QPainter;
class MainWindow;
class QTimer;
class Enemy;
class Bullet;


class Tower: public QObject
{
    Q_OBJECT
public:
    Tower(QPoint pos, MainWindow *game, int _level);
    ~Tower();

    void drawTower(QPainter *painter);
    void chooseEnemyforAttack(Enemy *enemy);
    void attackEnemy();
    void loseSightofEnmey();
    void enemyKilled();
    void checkEnemyInRange();
    void removeBullet();

    Tower* upgrade();

    int level;


private slots:
    void shootWeapon();

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

#endif // TOWER_H
