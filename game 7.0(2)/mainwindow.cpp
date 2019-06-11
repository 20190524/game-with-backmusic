#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "gun.h"
#include<QMediaPlayer>
using namespace std;

static const int Towercost[]={100,200,200,300,300};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
  , ui(new Ui::MainWindow)
  , waves(1)
  , playerHp(4)
  , playerGold(1000)
  , checkpoint(2)
  , win(false)
  , lose(false)
  ,_startgame(false)
  ,_mainpage(true)
  ,_statement(false)
  ,_close(false)
{
    ui->setupUi(this);
    loadTowerPositions();
    addwaypoint();
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateMap()));
    timer->start(30);
    QTimer::singleShot(3000, this, SLOT(gameStart()));//和时间有关


    QMediaPlayer * player = new QMediaPlayer;
    player->setMedia(QUrl("qrc:/sound/musicback.mp3"));
    player->setVolume(30);
    player->play();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *){
    QPainter painter(this);
    if(_mainpage)
    {
        painter.drawPixmap(0,0,this->width(),this->height(),QPixmap(":/pictures/mainpage.jpg"));
    }
    if(_statement){
        painter.drawPixmap(0,0,this->width(),this->height(),QPixmap(":/pictures/statement.jpg"));
    }
    if(_startgame)
    {
        painter.drawPixmap(0,0,this->width(),this->height(),QPixmap(":/pictures/grass.jpg"));
        foreach (const towerposition &towerPos, t_towerpositionlist) {
            towerPos.drawTower(&painter);
        }
        foreach (Tower *tower,t_towerlist) {
            tower->drawTower(&painter);
        }
        foreach (Gun *gun,gunlist) {
            gun->drawTower(&painter);
        }

        foreach (Enemy*enemy1, enemylist1) enemy1->draw(&painter);
        foreach (Enemy*enemy2, enemylist2) enemy2->draw(&painter);
        foreach (Enemy*enemy3, enemylist3) enemy3->draw(&painter);
        foreach (Enemy*enemy4, enemylist4) enemy4->draw(&painter);

        while (!Bonuses.empty())
        {
            delete Bonuses[0];
            Bonuses.removeAt(0);
        }

        //foreach (const waypoint *wayPoint, waypointlist)
        //wayPoint->draw(&painter);

        foreach (const Bullet *bullet, bulletlist)
            bullet->drawBullet(&painter);

        /*if (first_lose || first_win)
    {
        QString text = first_lose ? "YOU LOST!!!" : "YOU WIN!!!";
        QPainter painter(this);
        painter.setPen(QPen(Qt::red));
        painter.drawText(rect(), Qt::AlignCenter, text);
        return;
    }*/

        drawWave(&painter);
        drawHP(&painter);
        drawPlayerGold(&painter);
    }

}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    if(_mainpage)
    {
        if(event->button()==Qt::LeftButton)
        {
            QPoint pressPos = event->pos();
            if(pressPos.rx()<(this->width()/2+100)&&pressPos.rx()>(this->width()/2-100)&&pressPos.ry()<(this->height()/2+70)&&pressPos.ry()>(this->height()/2+20)){
                _mainpage=false;
                _statement=true;
            }
            if(pressPos.rx()<(this->width()/2+100)&&pressPos.rx()>(this->width()/2-100)&&pressPos.ry()<(this->height()/2+150)&&pressPos.ry()>(this->height()/2+100)){
                _mainpage=false;
                _close=true;
//                exit(1);
                return;//这个没法直接退出了！
            }
            if(pressPos.rx()<(this->width()/2+100)&&pressPos.rx()>(this->width()/2-100)&&pressPos.ry()<(this->height()/2+230)&&pressPos.ry()>(this->height()/2+180)){
                _mainpage=false;
                _startgame=true;
            }
        }
    }
    if(_statement){
        if(event->button()==Qt::LeftButton)
        {
            QPoint pressPos = event->pos();
            if(pressPos.rx()<this->width()-280&&pressPos.ry()<360&&pressPos.rx()>this->width()-370&&pressPos.ry()>280){
                _statement=false;
                _mainpage=true;
            }
        }
    }
    if(_startgame)
    {
        if(event->button()==Qt::LeftButton)
        {
            QPoint pressPos = event->pos();
            if (checkpoint==1) {
                auto it = t_towerpositionlist.begin();
                while (it != t_towerpositionlist.end())
                {
                    if (canbuyTower(1) && it->containTower(pressPos) && it->canhaveTower())
                    {
                        //m_audioPlayer->playSound(TowerPlaceSound);

                        playerGold -= Towercost[0];
                        Tower *tower = new Tower(it->centerPos(), this, 1);
                        it->sethaveTower(tower);
                        t_towerlist.push_back(tower);
                        update();
                        break;
                    }
                    else if (it->containTower(pressPos) && !it->canhaveTower()) {
                        QMessageBox box1(QMessageBox::NoIcon, "提示","是否升级炮塔?", QMessageBox::Yes | QMessageBox::No, NULL);
                        box1.setIconPixmap(QPixmap(":/pictures/gold.png"));
                        if(box1.exec()==QMessageBox::Yes) {
                            if ( it->getTower()->level==1 ) {
                                if (canbuyTower(2)) {
                                    Tower *tower=it->getTower()->upgrade();
                                    removeTower(it->getTower());
                                    it->sethaveTower(tower);
                                    playerGold-=Towercost[1];
                                }
                                else {
                                    QMessageBox box2(QMessageBox::NoIcon, "提示", "金币不足！");
                                    box2.setIconPixmap(QPixmap(":/pictures/remind.png"));
                                    box2.exec();
                                }
                            }
                            else if (it->getTower()->level==2) {
                                if (canbuyTower(3)) {
                                    Tower *tower=it->getTower()->upgrade();
                                    removeTower(it->getTower());
                                    it->sethaveTower(tower);
                                    playerGold-=Towercost[2];
                                }
                                else {
                                    QMessageBox box2(QMessageBox::NoIcon, "提示", "金币不足！");
                                    box2.setIconPixmap(QPixmap(":/pictures/remind.png"));
                                    box2.exec();
                                }
                            }
                            else if (it->getTower()->level==3) {
                                QMessageBox box2(QMessageBox::NoIcon, "提示", "已升至当前最高级炮塔！");
                                box2.setIconPixmap(QPixmap(":/pictures/remind.png"));
                                box2.exec();
                            }
                        }
                    }

                    ++it;
                }
            }
            else if (checkpoint==2) {
                auto it = t_towerpositionlist.begin();
                while (it != t_towerpositionlist.end())
                {
                    if (canbuyTower(1) && it->containTower(pressPos) && it->canhaveTower())
                    {
                        //m_audioPlayer->playSound(TowerPlaceSound);
                        playerGold -= Towercost[0];
                        Tower *tower = new Tower(it->centerPos(), this, 1);
                        it->sethaveTower(tower);
                        t_towerlist.push_back(tower);
                        update();
                        break;
                    }
                    else if (it->containTower(pressPos) && !it->canhaveTower()) {
                        QMessageBox box1(QMessageBox::NoIcon, "提示","是否升级炮塔?", QMessageBox::Yes | QMessageBox::No, NULL);
                        box1.setIconPixmap(QPixmap(":/pictures/gold.png"));
                        if(box1.exec()==QMessageBox::Yes) {
                            if ( it->getTower()->level==1 ) {
                                if (canbuyTower(2)) {
                                    Tower *tower=it->getTower()->upgrade();
                                    removeTower(it->getTower());
                                    it->sethaveTower(tower);
                                    playerGold-=Towercost[1];
                                }
                                else {
                                    QMessageBox box2(QMessageBox::NoIcon, "提示", "金币不足！");
                                    box2.setIconPixmap(QPixmap(":/pictures/remind.png"));
                                    box2.exec();
                                }
                            }
                            else if (it->getTower()->level==2) {
                                if (canbuyTower(3)) {
                                    Tower *tower=it->getTower()->upgrade();
                                    removeTower(it->getTower());
                                    it->sethaveTower(tower);
                                    playerGold-=Towercost[2];
                                }
                                else {
                                    QMessageBox box2(QMessageBox::NoIcon, "提示", "金币不足！");
                                    box2.setIconPixmap(QPixmap(":/pictures/remind.png"));
                                    box2.exec();
                                }
                            }
                            else if (it->getTower()->level==3) {
                                if (canbuyTower(4)) {
                                    removeTower(it->getTower());
                                    Gun *gun=new Gun(pressPos,this,4);
                                    addGun(gun);
                                    gun->level++;
                                    it->sethaveTower(gun);
                                    playerGold-=Towercost[3];
                                }
                                else {
                                    QMessageBox box2(QMessageBox::NoIcon, "提示", "金币不足！");
                                    box2.setIconPixmap(QPixmap(":/pictures/gold.png"));
                                    box2.exec();
                                }
                            }
                        }
                    }

                    ++it;
                }
            }
        }
    }
}

void MainWindow::loadTowerPositions() {
    QPoint pos[]={
        QPoint(0, 35),
        QPoint(200, 35),
        QPoint(400, 35),
        QPoint(600, 35),
        QPoint(800, 35),
        QPoint(1000, 35),

        QPoint(0, 210),
        QPoint(200, 210),
        QPoint(400, 210),
        QPoint(600, 210),
        QPoint(800, 210),
        QPoint(1000, 210),//

        QPoint(0, 385),
        QPoint(200, 385),
        QPoint(400, 385),
        QPoint(600, 385),
        QPoint(800, 385),
        QPoint(1000, 385),

        QPoint(0, 560),
        QPoint(200, 560),
        QPoint(400, 560),
        QPoint(600, 560),
        QPoint(800, 560),
        QPoint(1000, 560)
            };
    int len	= sizeof(pos) / sizeof(pos[0]);
    for (int i = 0; i < len; ++i)
        t_towerpositionlist.push_back(pos[i]);

}

void MainWindow::removeEnemy(Enemy *enemy) {
    Q_ASSERT(enemy);
    enemylist1.removeOne(enemy);
    delete enemy;

    if (enemylist1.empty()) {
        waves++;
        if (!loadWaves()) {
            if (checkpoint==2) win=true;
            else if (checkpoint==1) checkpoint++;
        }
    }
}

QList<Enemy *> MainWindow::enemyList1() const {
    return enemylist1;
}

QList<Enemy *> MainWindow::enemyList2() const {
    return enemylist2;
}

QList<Enemy *> MainWindow::enemyList3() const {
    return enemylist3;
}

QList<Enemy *> MainWindow::enemyList4() const {
    return enemylist4;
}

void MainWindow::removeBullet(Bullet *bullet) {
    Q_ASSERT(bullet);

    bulletlist.removeOne(bullet);
    delete bullet;
}

void MainWindow::addwaypoint() {
    waypoint *wayPoint11 = new waypoint(QPoint(10,100));
    waypointlist1.push_back(wayPoint11);

    waypoint *wayPoint12 = new waypoint(QPoint(1280, 100));
    waypointlist1.push_back(wayPoint12);
    wayPoint12->setNextWayPoint(wayPoint11);

    waypoint *wayPoint21 = new waypoint(QPoint(10, 280));
    waypointlist2.push_back(wayPoint21);

    waypoint *wayPoint22 = new waypoint(QPoint(1280, 280));
    waypointlist2.push_back(wayPoint22);
    wayPoint22->setNextWayPoint(wayPoint21);

    waypoint *wayPoint31 = new waypoint(QPoint(10,460));
    waypointlist3.push_back(wayPoint31);

    waypoint *wayPoint32 = new waypoint(QPoint(1280,460));
    waypointlist3.push_back(wayPoint32);
    wayPoint32->setNextWayPoint(wayPoint31);

    waypoint *wayPoint41 = new waypoint(QPoint(10, 640));
    waypointlist4.push_back(wayPoint41);

    waypoint *wayPoint42 = new waypoint(QPoint(1280, 640));
    waypointlist4.push_back(wayPoint42);
    wayPoint42->setNextWayPoint(wayPoint41);

}

bool MainWindow::loadWaves(){
    if (checkpoint==1||checkpoint==2){
        if(waves>=20){
        return false;
    }//大于波数则结束游戏
   QList<waypoint *> startpoint;
   waypoint * startpoint1=waypointlist1.back();
   waypoint * startpoint2=waypointlist2.back();
   waypoint * startpoint3=waypointlist3.back();
   waypoint * startpoint4=waypointlist4.back();
   startpoint.push_back(startpoint1);
   startpoint.push_back(startpoint2);
   startpoint.push_back(startpoint3);
   startpoint.push_back(startpoint4);
   int enemyInterval[]={100,500,600,1000,3000,6000};
   for(int i=0;i<5;i++){
       int q;
        if (waves < 10) q = qrand() % ((waves+1)/2) + 1;
        else q = qrand() % ((waves+1)/4) + 1;
        Enemy *enemy=new Enemy(startpoint[q-1],this);
        enemylist1.push_back(enemy);
        QTimer::singleShot(enemyInterval[i],enemy,SLOT(do_Activate()));
        }
    return true;
    }
}

void MainWindow::getHpDamage(int damage/* = 1*/)
{
    //audioPlayer->playSound(LifeLoseSound);
    playerHp -= damage;
    if (playerHp <= 0)
        doGameOver();
}

void MainWindow::doGameOver()
{
    if (!lose)
    {
        lose = true;
        // 此处应该切换场景到结束场景
        // 暂时以打印替代,见paintEvent处理
    }
}

void MainWindow::updateMap() {
    foreach (Enemy *enemy1, enemylist1)
        enemy1->move();
    foreach (Enemy *enemy2, enemylist2)
        enemy2->move();
    foreach (Enemy *enemy3, enemylist3)
        enemy3->move();
    foreach (Enemy *enemy4, enemylist4)
        enemy4->move();
    foreach (Tower *tower, t_towerlist)
        tower->checkEnemyInRange();

    this->removeDeath();
    this->act();
    if (qrand() % 200 < 1)
    {
        Bonus* sun = new zSunFall(this);
        sun->setgame(this);
        Bonuses.append(sun);
    }
    this->exit->raise();


    update();
}

void MainWindow::addBullet(Bullet *bullet) {
    Q_ASSERT(bullet);

    bulletlist.push_back(bullet);
}


bool MainWindow::canbuyTower(int level) const {
    if (Towercost[level-1]<=playerGold) return true;
    else return false;
}

void MainWindow::awardGold(int gold) {
    playerGold+=gold;
    update();
}

void MainWindow::drawWave(QPainter *painter)
{
    painter->setPen(QPen(Qt::white));
    painter->setFont(QFont("黑体", 20, QFont::Bold, false));
        painter->drawPixmap(370,750,350,150,QPixmap(":/pictures/wave_1.png"));
    painter->drawText(QRect(520,770,200,100), QString("%1").arg(waves));
}

void MainWindow::drawHP(QPainter *painter)
{
    painter->setPen(QPen(Qt::white));
    painter->setFont(QFont("黑体", 20, QFont::Bold, false));
    painter->drawPixmap(0,750,350,150,QPixmap(":/pictures/blood_1.png"));
    painter->drawText(QRect(150,770,200,100), QString("%1").arg(playerHp));
    //painter->drawText(hdc,QString("HP : %1").arg(waves),-1,QRect(0,750,350,150), DT_CALCRECT | DT_CENTER | DT_SINGLELINE);
}

void MainWindow::drawPlayerGold(QPainter *painter)
{
    painter->setPen(QPen(Qt::white));
    painter->setFont(QFont("黑体", 20, QFont::Bold, false));
        painter->drawPixmap(740,750,350,150,QPixmap(":/pictures/gold_1.png"));
    painter->drawText(QRect(890,770,200,100), QString("%1").arg(playerGold));
}
void MainWindow::gameStart()
{
    loadWaves();
}

void MainWindow::removeDeath() {
    int p = 0;
    while (p < Bonuses.count())
    {
        if (!(Bonuses[p]->alive))
        {
            delete (Bonuses[p]);
            Bonuses.removeAt(p);
        }
        else
        {
            p++;
        }
    }
}

void MainWindow::act() {
    Bonus* i;
    foreach(i, Bonuses)
    {
        i->act();
    }
}

void MainWindow::removeTower(Tower *tower) {
    Q_ASSERT(tower);
    t_towerlist.removeOne(tower);
    delete tower;
}

void MainWindow::addTower(Tower *tower) {
    Q_ASSERT(tower);
    t_towerlist.push_back(tower);
    update();
}

void MainWindow::removeGun(Gun *gun) {
    Q_ASSERT(gun);
    gunlist.removeOne(gun);
    delete gun;
}

void MainWindow::addGun(Gun *gun) {
    Q_ASSERT(gun);
    gunlist.push_back(gun);
    update();
}
