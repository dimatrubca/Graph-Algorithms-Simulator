#ifndef SCENE_H
#define SCENE_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include "node.h"
#include "graph.h"

class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    enum Mode {SelectObject, DrawEdge, DrawNode, RemoveItem, AlgRunning};
    Scene(QObject* parent = 0);
    void setMode(Mode mode);
    Graph* getGraph();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

signals:
    void sendHintMessage(QString mess);
    void vertexSelected(Node* node);

private:
    Mode sceneMode;
    Node *origNode = 0;
    Graph* graph;
};

#endif // SCENE_H
