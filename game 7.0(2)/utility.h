#ifndef UTILITY_H
#define UTILITY_H

#include <QtMath>
#include <QPoint>

inline bool collisionWithCircle(QPoint point1, int range, QPoint point2)
{
    if (point1.x()-point2.x() <= 0 && fabs(point1.x()-point2.x()) <= range && fabs(point1.y()-point2.y())<=100)
        return true;
    else return false;
}

#endif // UTILITY_H
