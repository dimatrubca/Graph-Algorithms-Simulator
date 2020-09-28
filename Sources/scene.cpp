#include "scene.h"
#include "node.h"
#include "edgeform.h"
#include "mainwindow.h"
#include "graph.h"

Scene::Scene(QObject* parent) :  QGraphicsScene(parent)
{
    sceneMode = DrawNode;
    graph = new Graph(this, this);
    connect(this->graph, &Graph::actionPerformed, (MainWindow*) parent, &MainWindow::updateInfoLabel);
    connect(this->graph, &Graph::graphStateChanged, (MainWindow*) parent, &MainWindow::graphStateChanged);
}

void Scene::setMode(Scene::Mode mode)
{
    this->sceneMode = mode;
    if (origNode) {
        origNode->setColor(Node::Default);
        origNode = 0;
    }
}


void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (sceneMode != DrawEdge && origNode) {
        origNode->setColor(Node::Default);
        origNode = 0;
    }

    switch (sceneMode) {
        case AlgRunning: {
            QGraphicsItem *item = itemAt(event->scenePos(), QTransform());
            Node *node = dynamic_cast<Node *>(item);

            if (node) emit vertexSelected(node);
            break;
        }

        case SelectObject: {
            QGraphicsScene::mousePressEvent(event);
            break;
        }
        case DrawNode: {
            graph->addNode(event->scenePos());
            break;
        }

        case DrawEdge: { // check qgraphics cas?/dunamic cast
            QGraphicsItem *item = itemAt(event->scenePos(), QTransform());
            Node *node = dynamic_cast<Node *>(item);

            if (node) {
                if (origNode == nullptr) {
                    origNode = node;
                    emit sendHintMessage(tr("Select the second vertex"));
                    node->setColor(Node::Highlighted1);

                } else if (node != origNode) {
                    EdgeForm edgeForm;
                    edgeForm.setModal(true);
                    int result = edgeForm.exec();
                    int weight=0, isDirected=0, isWeighted=0;

                    if (result == QDialog::Accepted) {
                        isDirected = edgeForm.getIsDirected();
                        isWeighted = edgeForm.getIsWeighted();
                        if (isWeighted) weight = edgeForm.getWeight();

                        graph->addEdge(origNode, node, isDirected, isWeighted, weight);
                    }

                    origNode->setColor(Node::Default);
                    origNode = 0;
                    emit sendHintMessage(tr("Select starting vertex"));
                }
            }

            break;
        }

        case RemoveItem: {
            QGraphicsItem *item = itemAt(event->scenePos(), QTransform());
            Node *node = dynamic_cast<Node *>(item);
            Edge *edge = dynamic_cast<Edge *>(item);
            if (node) {
                graph->removeNode(node);
            } else if (edge) {
                graph->removeEdge(edge, 1);
            }
        }
    }
}

Graph *Scene::getGraph()
{
    return graph;
}
