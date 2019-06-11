#ifndef WAYPOINT_H
#define WAYPOINT_H

#include <QPoint>

class QPainter;

class waypoint
{
public:
    waypoint(QPoint pos);
    void setNextWayPoint(waypoint *nextwaypoint);
    const QPoint pos();
    //void draw(QPainter *painter) const;
    waypoint * nextWayPoint() const;

private:
    QPoint w_pos;
    waypoint * w_nextwaypoint;
};

#endif // WAYPOINT_H
