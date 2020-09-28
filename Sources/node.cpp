#include "node.h"
#include "edge.h"
#include "scene.h"
#include <QDebug>
Node::Node(int id, QPointF centerPos) : _id(id), centerPos(centerPos)
{
    this->setPos(adjustCenterToTopLeft(centerPos));
    setColor(Default);
    setFlags(QGraphicsItem::ItemSendsScenePositionChanges | QGraphicsItem::ItemIsMovable);
    //this->pos()
}


QRectF Node::boundingRect() const
{
    return QRectF(0, 0, 60, 60);
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF rec = boundingRect();
    painter->setBrush(this->brush());
    painter->setPen(this->pen());
    painter->drawEllipse(rec);

    QFont font = painter->font() ;
    font.setPointSize(13);
    font.setBold(1);
    font.setStyleHint(QFont::Decorative);
    painter->setFont(font);
    painter->setPen(Qt::black);
    painter->drawText(rec,  Qt::AlignCenter, QString("%1").arg(_id));
}


void Node:: addEdge(Edge *currEdge, bool isOutEdge) {
       if (isOutEdge) {
           outEdges.push_back(currEdge);
       } else {
           inEdges.push_back(currEdge);
       }
   }

QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value)
{
   if (change == ItemPositionChange && scene()) {
       // value is the new position.
       QPointF newPos = value.toPointF();
       moveEdgesToCenter(newPos);
   }
   return QGraphicsItem::itemChange(change, value);
}

void Node::moveEdgesToCenter(QPointF newPos) {
    // Converts the elipse position (top-left)
    // to its center position
    int xOffset = boundingRect().x() + boundingRect().width()/2;
    int yOffset = boundingRect().y() + boundingRect().height()/2;
    QPointF newCenterPos = QPointF(newPos.x() + xOffset, newPos.y() + yOffset);

    // Move the required point of the line to the center of the elipse
    for (auto edge: inEdges) {
        QPointF p1 = edge->line().p1();
        QPointF p2 = newCenterPos;
        edge->setLine(QLineF(p1, p2));
    }

    for (auto edge: outEdges) {
        QPointF p1 = newCenterPos;
        QPointF p2 = edge->line().p2();
        edge->setLine(QLineF(p1, p2));
    }

}

QPointF Node::getMiddlePos() {
    int xOffset = boundingRect().x() + boundingRect().width()/2;
    int yOffset = boundingRect().y() + boundingRect().height()/2;

    return QPointF(pos().x() + xOffset, pos().y() + yOffset);
}

void Node::setColor(ColorMode colorMode)
{
    colorState = colorMode;
    this->setPen(QPen(getPenColor(), 4));
    this->setBrush(QBrush(getBrushColor()));
    update();
}

QColor Node::getPenColor() {
    switch (colorState) {
        case Default:
            return QColor(1, 179, 179, 255);
        case Highlighted1:
            return QColor(251, 182, 165, 255);
        case Highlighted2:
            return QColor(230, 130, 105, 255);
    }
}

QColor Node::getBrushColor()
{
    switch (colorState) {
        case Default:
            return QColor(1, 191, 191, 255);
        case Highlighted1:
            return QColor(255, 200, 185, 255);
        case Highlighted2:
            return QColor(244, 152, 128, 255);
    }
}

QPointF Node::adjustCenterToTopLeft(QPointF pos) {
    int xOffset = boundingRect().width()/2;
    int yOffset = boundingRect().height()/2;

    QPointF newCenterPos = QPointF(pos.x() - xOffset, pos.y() - yOffset);
    return newCenterPos;
}

void Node::removeEdge(Edge* edge) {
    for (auto it = inEdges.begin(); it != inEdges.end(); ) {
        if (*it == edge) {
            it = inEdges.erase(it);
        } else ++it;
    }

    for (auto it = outEdges.begin(); it != outEdges.end(); ) {
        if (*it == edge) {
            it = outEdges.erase(it);
        }
        else ++it;
    }
}

