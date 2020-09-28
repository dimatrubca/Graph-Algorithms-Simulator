#ifndef CUSTOMELLIPSE_H
#define CUSTOMELLIPSE_H

#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>


class CustomEllipse: QGraphicsItem
{
public:
    CustomEllipse();
    void addLine(QGraphicsLineItem *line, bool isPoint1) {
        this->line = line;
        isP1 = isPoint1;
    }

    QVariant itemChange(GraphicsItemChange change, const QVariant &value)
    {
        if (change == ItemPositionChange && scene()) {
            // value is the new position.
            QPointF newPos = value.toPointF();

            moveLineToCenter(newPos);
        }
        return QGraphicsItem::itemChange(change, value);
    }

    void moveLineToCenter(QPointF newPos) {
        // Converts the elipse position (top-left)
        // to its center position
        int xOffset = rect().x() + rect().width()/2;
        int yOffset = rect().y() + rect().height()/2;

        QPointF newCenterPos = QPointF(newPos.x() + xOffset, newPos.y() + yOffset);

        // Move the required point of the line to the center of the elipse
        QPointF p1 = isP1 ? newCenterPos : line->line().p1();
        QPointF p2 = isP1 ? line->line().p2() : newCenterPos;

        line->setLine(QLineF(p1, p2));
    }


private:
    QGraphicsItem *line;
    bool isP1;
};

#endif // CUSTOMELLIPSE_H
