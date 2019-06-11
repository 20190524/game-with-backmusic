#include "waypoint.h"
#include <QPainter>
#include <QColor>

waypoint::waypoint(QPoint pos):
    w_pos(pos),
    w_nextwaypoint(NULL)
{

}

/*void waypoint::draw(QPainter *painter) const {
    painter->save();
    painter->setBrush(Qt::NoBrush);
    painter->drawEllipse(w_pos, 6, 6);
    painter->drawEllipse(w_pos, 2, 2);//调换方向

    if (w_nextwaypoint)
        painter->drawLine(w_pos, w_nextwaypoint->w_pos);
    painter->restore();
}*/

void waypoint::setNextWayPoint(waypoint *nextwaypoint) {
    w_nextwaypoint = nextwaypoint;
}

waypoint* waypoint::nextWayPoint() const {
    return w_nextwaypoint;
}

const QPoint waypoint::pos() {
    return w_pos;
}


