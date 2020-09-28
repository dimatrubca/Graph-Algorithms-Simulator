#ifndef NODE_H
#define NODE_H

#include <QPainter>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <vector>
#include "edge.h"

class Node : public QAbstractGraphicsShapeItem
{
public:
    enum ColorMode {Default, Highlighted1, Highlighted2};

    Node(int _id, QPointF centerPos);
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

    inline int id() { return _id; };
    inline QPointF getCenterPos() { return centerPos; }
    void addEdge(Edge *line, bool isPoint1);
    void removeEdge(Edge* edge);
    void moveEdgesToCenter(QPointF newPos);
    QPointF adjustCenterToTopLeft(QPointF);
    QPointF adjustTopLeftToCenter(QPointF);
    QPointF getMiddlePos();

    void setColor(ColorMode state);

private:

    QColor getPenColor();
    QColor getBrushColor();

    int _id;
    QPointF centerPos;
    ColorMode colorState;
    std::vector<QGraphicsLineItem*> inEdges;
    std::vector<QGraphicsLineItem*> outEdges;
};

#endif // NODE_H
