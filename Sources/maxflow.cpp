#include "maxflow.h"
#include <queue>
#include <algorithm>
#define str(n) QString::number(n)

MaxFlow::MaxFlow(QString name, Graph *graph, QObject *parent)
    : AbstractAlgorithm(name, graph, parent)
{
    prev.resize(maxNode + 1, 0);
    if (handleUnweightedEdges()) emit updateHintLabel("Select starting vertex");
}

void MaxFlow::setUp() {
    emit updateHintLabel("Computing max flow");
    if (run()) emit updateHintLabel(result);
}


bool MaxFlow::BFS() {
    for (int i=1; i<=maxNode; i++) prev[i] = -1;
    prev[startVertex->id()] = 0;

    while (Q.size()) Q.pop();
    Q.push(startVertex->id());
    while (Q.size()) {
        int x = Q.front(); Q.pop();
        for (auto e: V[x]) {
            int y = e.to;
            if (prev[y] == -1 && RG[x][y] > 0) {
                prev[y] = x;
                Q.push(y);
                if (y == endVertex->id()) return 1;
            }
        }
    }
    return 0;
}

bool MaxFlow::run() {
    V = graph->toVectorList();
    RG.resize(maxNode + 1);

    for (int i=0; i<=maxNode; i++) {
        RG[i].resize(maxNode + 1, 0);
    }

    for (unsigned int i = 0; i<V.size(); i++) {
        for (auto edge : V[i]) {
            int w = edge.weight;
            int to = edge.to;

            RG[i][to] += w;
        }
    }

    while (BFS()) {
        int pathFlow = 1e9;
        for (int i = endVertex->id(); i != startVertex->id(); i = prev[i]) {
            pathFlow = std::min(pathFlow, RG[prev[i]][i]);
        }

        for (int i = endVertex->id(); i != startVertex->id(); i = prev[i]) {
            RG[prev[i]][i] -= pathFlow;
            RG[i][prev[i]] += pathFlow;
        }

        maxFlow += pathFlow;
    }

    for (unsigned int i=0; i<V.size(); i++) {
        for (auto e : V[i]) {
            Edge* edge = e.edge;
            int flow = e.weight - RG[i][e.to];
            bool dir = edge->getIsDirected();

            if (flow<0) {
                if (!dir) continue;
                else flow = 0;
            } else if (flow && !dir) edge->setDrawMiddleArrow(graph->findNode(e.to), 1);
            edge->setFlaw(flow);
        }
    }

    result = tr("Maximal flow from vertex %1 to %2 equals %3").arg(str(startVertex->id()), str(endVertex->id()), str(maxFlow));
    return 1;
}


void MaxFlow::onVertexSelected(Node *node)
{
    if (sleeper->isStopped()) return;
    if (!startVertex) {
        startVertex = node;
        startVertex->setColor(Node::Highlighted2);
        emit updateHintLabel("Select end vertex");

    } else if (!endVertex && startVertex != node) {
        endVertex = node;
        endVertex->setColor(Node::Highlighted1);
        setUp();
    }
}
