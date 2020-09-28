#ifndef ABSTRACTALGORITHM_H
#define ABSTRACTALGORITHM_H

#include <QObject>
#include "sleeper.h"
#include "graph.h"

class AbstractAlgorithm : public QObject
{
    Q_OBJECT
public:
    enum Algorithms {NotSelected, DFS, BFS, MaxFlow, Dijkstra, Kruskal, EulerCycle, HamiltonianCycle};

    explicit AbstractAlgorithm(QString name, Graph* graph, QObject *parent = nullptr);
    virtual void setUp() = 0;
    virtual bool run() = 0;


public slots:
    bool handleUnweightedEdges();
    virtual void onVertexSelected(Node* node) = 0;
    void stop();
    void destroy();

protected:
    Sleeper *sleeper;
    Graph* graph;
    QString result;
    QString name;
    int maxNode;

signals:
    void updateHintLabel(QString result);
    void triggerStopButton();
};

#endif // ABSTRACTALGORITHM_H
