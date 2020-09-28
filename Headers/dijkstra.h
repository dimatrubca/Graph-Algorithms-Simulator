#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <QObject>
#include "abstractalgorithm.h"

class Dijkstra : public AbstractAlgorithm
{
    Q_OBJECT
public:
    explicit Dijkstra(QString name, Graph* graph, QObject *parent = nullptr);

    void setUp();
    bool run();
    void stop();

public slots:
    virtual void onVertexSelected(Node* node);

private:
    Node* startVertex;
    Node* endVertex;

    std::vector<int> dist;
    std::vector<Edge*> prev;
};


#endif // DIJKSTRA_H
