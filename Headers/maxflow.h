#ifndef MAXFLOW_H
#define MAXFLOW_H
#include "abstractalgorithm.h"
#include <queue>

class MaxFlow : public AbstractAlgorithm
{
    Q_OBJECT
public:
    explicit MaxFlow(QString name, Graph* graph, QObject *parent = nullptr);

    void setUp();
    bool run();
    void stop();

public slots:
    virtual void onVertexSelected(Node* node);

private:
    bool BFS();

    Node* startVertex = 0;
    Node* endVertex = 0;
    int maxFlow = 0;

    std::vector<std::vector<int> > RG; //residual graph
    std::vector<std::vector<VLGraphEdge>> V;
    std::vector<int> prev;
    std::queue<int> Q;
};
#endif // MAXFLOW_H
