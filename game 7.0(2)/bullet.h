#ifndef BULLET_H
#define BULLET_H

#include <QPoint>
#include <QSize>
#include <QPixmap>
#include <QObject>

class QPainter;
class Enemy;
class MainWindow;


class Bullet:QObject
{
    Q_OBJECT
    Q_PROPERTY(QPoint b_currentPos READ currentPos WRITE setCurrentPos)
public:
    Bullet(QPoint startPos, QPoint targetPoint, int damage, Enemy *target,
           MainWindow *game, const QPixmap &sprite = QPixmap(":/pictures/bullet.png"));
    void move();
    void drawBullet(QPainter * painter) const;
    void setCurrentPos(QPoint pos);
    QPoint currentPos() const;

private slots:
    void hitTarget();

private:
    const QPoint b_towerPos,b_targetPos;
    const QPixmap	b_sprite;
    QPoint b_currentPos;
    int b_attackdamage;

    Enemy * b_target;
    MainWindow *	b_game;

    static const QSize fixedSize;

};

#endif // BULLET_H
