#ifndef BONUS_H
#define BONUS_H

#include <QWidget>
#include <QMovie>
#include <QMouseEvent>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QDebug>
#include <QKeyEvent>

class MainWindow;

class Bonus : public QLabel
{
public:
    Bonus(QWidget* parent = 0);
    virtual void act() = 0;
    bool alive = true;
    int strength = 1;
    virtual void setgame(MainWindow* game);
    MainWindow *_game;

};

class zSun : virtual public Bonus
{
public:
    zSun(QWidget* parent = 0);
    ~zSun();
    void act();
    int frame;
    float level, speed, accelerate, x_speed = 0;
    void setgame(MainWindow* game);
    MainWindow *_game;


protected:
    void mousePressEvent(QMouseEvent *event);
private:
    QMovie* anim = new QMovie(":/pictures/gold.png");
};

class zSunFall : virtual public Bonus
{
public:
    zSunFall(QWidget* parent = 0);
    ~zSunFall();
    void act();
    int frame;
    float level, speed;
    void setgame(MainWindow* game);
    MainWindow *_game;

protected:
    void mousePressEvent(QMouseEvent *event);
private:
    QMovie* anim = new QMovie(":/pictures/gold.png");
};

#endif // BONUS_H
