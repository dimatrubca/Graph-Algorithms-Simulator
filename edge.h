#ifndef EDGE_H
#define EDGE_H

#include <QPainter>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsLineItem>
#include <QDebug>

#include <math.h>

class Node;

class Edge : public QGraphicsLineItem
{
public:
    enum ColorMode {Default, Highlighted};

    Edge(Node* vertex1, Node* vertex2, bool isDirected=0, bool isWeighted = 0, int weight = 0, QGraphicsItem *parent=0);
    ~Edge();
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    Node* v1();
    Node* v2();
    void setPoints(QPointF p1, QPointF p2);
    void setColor(ColorMode color);
    void setWeight(int weight);
    void setFlaw(int flow);
    void setDrawMiddleArrow(Node* to, bool draw);
    void restoreDefaultState();

    inline void setIsSingle(bool isSingle) { this->isSingle = isSingle; }
    inline bool getIsDirected() { return isDirected; }
    inline bool getIsWeighted() { return isWeighted; }
    inline int getWeight() { return weight; }

private:
    QPointF p1();
    QPointF p2();
    QPointF getWeightItemPos(QRectF rect);
    std::pair<QLineF, QLineF> getArrowAtPoint(int x, int y, int sameDir);
    double angleLineOX(const QLineF& line);
    void addWeightItem();

    QGraphicsTextItem* weightGraphicsItem = nullptr;
    Node *vertex1 = nullptr, *vertex2 = nullptr;
    bool isDirected;
    bool isWeighted;
    int weight;
    bool isSingle;
    bool drawMiddleArrow;
    bool middleArrowCodirectional;

    ColorMode colorState;
};

#endif // EDGE_H
