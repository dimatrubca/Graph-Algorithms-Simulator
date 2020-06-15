#include "edge.h"
#include "mainwindow.h"

Edge::Edge(Node* vertex1, Node* vertex2, bool isDirected, bool isWeighted, int weight, QGraphicsItem *parent) :
    QGraphicsLineItem(QLineF(vertex1->getCenterPos(), vertex2->getCenterPos()), parent),
    vertex1(vertex1), vertex2(vertex2), isDirected(isDirected), isWeighted(isWeighted), weight(weight),
    isSingle(1), drawMiddleArrow(0)
{
    setColor(Default);
    setZValue(-1);

    qDebug() << isWeighted << " weighted";
    if (isWeighted) addWeightItem();
}

Edge::~Edge()
{
    Scene* scene =  MainWindow::getInstance()->getScene();
    if (isWeighted) {
        scene->removeItem(weightGraphicsItem);
        delete weightGraphicsItem;
    }
}


void Edge::addWeightItem() {
    qDebug() << "Adding weight: " << vertex1->id() <<" -> "<<vertex2->id();
    Scene* scene = MainWindow::getInstance()->getScene();
    weightGraphicsItem = new QGraphicsTextItem(QString::number(weight));
    weightGraphicsItem->setZValue(-0.5);
    weightGraphicsItem->setPos(getWeightItemPos(weightGraphicsItem->boundingRect()));

    QFont font = weightGraphicsItem->font();
    font.setPointSize(12);
    font.setStyleHint(QFont::Decorative);

    weightGraphicsItem->setFont(font);
    weightGraphicsItem->setVisible(1);
    scene->addItem(weightGraphicsItem);
}

double Edge::angleLineOX(const QLineF& line) {
    return atan2(-(line.y2()-line.y1()), line.x2() - line.x1());
}

QRectF Edge::boundingRect() const
{
    QRectF boundingRect = QGraphicsLineItem::boundingRect();
    return QRectF(boundingRect.x() - 30, boundingRect.y() - 30, boundingRect.width() + 60, boundingRect.height() + 60);
}

void Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    painter->setPen(this->pen());

    double alpha = angleLineOX(line());
    double d = 6;
    double xoff = 0, yoff = 0;
    if (!isSingle) {
        xoff = -d * sin(alpha);
        yoff = -d * cos(alpha);
    }

    double x1 = p1().x() + xoff;
    double y1 = p1().y() + yoff;
    double x2 = p2().x() + xoff;
    double y2 = p2().y() + yoff;

    if (isDirected) {
        double x3 = x2 - 31 * cos(alpha);
        double y3 = y2 + 31 * sin(alpha);

        std::pair<QLineF, QLineF> arrow = getArrowAtPoint(x3, y3, 1);
        painter->drawLine(arrow.first);
        painter->drawLine(arrow.second);
    }

    if (drawMiddleArrow) {
        std::pair<QLineF, QLineF> arrow = getArrowAtPoint((x1+x2)/2, (y1+y2)/2, middleArrowCodirectional);
        painter->drawLine(arrow.first);
        painter->drawLine(arrow.second);
    }

    if (isWeighted) {
        QFont font = painter->font() ;
        font.setPointSize(11);
        font.setStyleHint(QFont::Decorative);
        weightGraphicsItem->setFont(font);
        weightGraphicsItem->setPos(getWeightItemPos(weightGraphicsItem->boundingRect()));
        weightGraphicsItem->setRotation(0);
    }

    QGraphicsLineItem::setLine(x1, y1, x2, y2);
    QGraphicsLineItem::paint(painter, option, widget);
}


std::pair<QLineF, QLineF> Edge::getArrowAtPoint(int x, int y, int sameDir) {
    double alpha = angleLineOX(line());
    if (!sameDir) alpha = (M_PI + alpha);
    double beta = M_PI/7, gama = M_PI/2 - alpha - beta;
    double a = 15;
    double c = a/cos(beta);
    double x1 = x - sin(gama) * c;
    double y1 = y + cos(gama) * c;

    gama = alpha - beta;
    double x2 = x - cos(gama) * c;
    double y2 = y + sin(gama) * c;

    return {QLineF(x,y,x1,y1), QLineF(x, y, x2, y2)};
}

QPointF Edge::getWeightItemPos(QRectF rect) {
    double alpha = angleLineOX(line());
    double xmid = (p1().x() + p2().x())/2;
    double ymid = (p1().y() + p2().y())/2;

    double w = rect.width()/2, h = rect.height()/2;
    double x = xmid - (w + 6)*sin(alpha) - w;
    double y = ymid - (h + 6)*cos(alpha) - h;

    return QPointF(x, y);
}

void Edge::setColor(Edge::ColorMode colorState)
{
    this->colorState = colorState;
    switch (colorState) {
        case Default:
            this->setPen(QPen(QColor(136, 217, 217, 100), 4.5));
            break;
        case Highlighted:
            this->setPen(QPen(QColor(255, 128, 95, 255), 4.5));
        break;
    }
    update();
}

void Edge::setWeight(int weight)
{
    this->weight = weight;
    if (!isWeighted) addWeightItem();
    else weightGraphicsItem->setPlainText(QString::number(weight));
    isWeighted = 1;
    update();
}

void Edge::setDrawMiddleArrow(Node* to, bool draw)
{
    if (draw) {
        drawMiddleArrow = 1;
        middleArrowCodirectional = (to == v2());
    } else drawMiddleArrow = 0;
    update();
}

void Edge::restoreDefaultState()
{
    setColor(ColorMode::Default);
    drawMiddleArrow = 0;
    if (isWeighted) weightGraphicsItem->setPlainText(QString::number(weight));
}

void Edge::setFlaw(int flow)
{
    weightGraphicsItem->setPlainText(QString("%2/%1").arg(QString::number(weight), QString::number(flow)));
    update();
}


QPointF Edge::p1()
{
    return vertex1->getMiddlePos();
}

QPointF Edge::p2()
{
    return vertex2->getMiddlePos();
}

Node *Edge::v1()
{
    return vertex1;
}


Node *Edge::v2()
{
    return vertex2;
}
