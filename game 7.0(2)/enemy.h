#ifndef ENEMY_H
#define ENEMY_H

#include <QObject>
#include <QPoint>
#include <QSize>
#include <QPixmap>

class waypoint;
class QPainter;
class MainWindow;
class Tower;
class Gun;

class Enemy: public QObject
{
    Q_OBJECT
public:
    Enemy(waypoint *startWayPoint, MainWindow *game, const QPixmap &sprite = QPixmap(":/pictures/enemy01.png"));
    ~Enemy() { }
    //Enemy(MainWindow *game, int road);

    void draw(QPainter *painter);
    void move();
    void getRemoved(Tower *tower);
    void getRemoved(Gun *gun);
    void getDamage(int damage);
    void getAttacked(Tower* attacker);
    void getLostSight(Tower* attacker);
    const QPoint pos();

    int difficulty;
    int road;


protected slots:
    void do_Activate();

private:
    double e_walkspeed;
    int e_currentHP;
    int e_maxHP;
    bool e_active;
    double e_rotationSprite;
    waypoint * e_desWayPoint;//终点

    QPoint e_pos;
    const QPixmap e_sprite;
    MainWindow * e_game;
    QList<Tower *>	attackedTowerslist;//被攻击的塔列表
    QList<Gun *> attackedGunlist;

    static const QSize fixedSize;
};

#endif // ENEMY_H
