#include "dijkstra.h"
#include <set>
#include <stack>
#include <QDialog>
#include <QInputDialog>
#include "mainwindow.h"
#define asleep() if(!sleeper->sleep(800)) return false;

Dijkstra::Dijkstra(QString name, Graph *graph, QObject *parent)
    : AbstractAlgorithm(name, graph, parent), startVertex(0), endVertex(0)
{
    if (handleUnweightedEdges()) emit updateHintLabel("Select starting vertex");
}

void Dijkstra::setUp() {
    emit updateHintLabel(tr("Computing the shortest path from %1 to %2").arg(QString::number(startVertex->id()), QString::number(endVertex->id())));
    if (run()) emit updateHintLabel(result);
}

bool Dijkstra::run() {
    std::vector<std::vector<VLGraphEdge>> V = graph->toVectorList();
    std::set<std::pair<int, int> > S;
    std::stack<Edge*> path;
    int length = 0, startId = startVertex->id(), endId = endVertex->id();
    prev.resize(maxNode + 1, nullptr);

    for (int i=0; i<=maxNode; i++) dist.push_back(1e9);
    dist[startVertex->id()] = 0;
    S.insert({0, startId});

    while (S.size()) {
        int vertex = S.begin()->second;
        int weight = S.begin()->first;
        S.erase(S.begin());

        if (dist[vertex] < weight) continue;

        for (auto it: V[vertex]) {
            int v2 = it.to;
            int w2 = it.weight;

            if (weight + w2 < dist[v2]) {
                dist[v2] = weight + w2;
                S.insert({weight + w2, v2});
                prev[v2] = it.edge;
            }
        }
    }

    for (auto v = endId; prev[v]; ) {
        length += prev[v]->getWeight();

        path.push(prev[v]);
        int v1 = prev[v]->v1()->id();
        int v2 = prev[v]->v2()->id();
        if (v==v1) v = v2;
        else v = v1;
    }

    Node* prev = startVertex;
    while (path.size()) {
        asleep();
        Node* v1 = path.top()->v1();
        Node* v2 = path.top()->v2();
        path.top()->setColor(Edge::Highlighted);
        path.pop();

        if (v1 == prev) prev = v2;
        else prev = v1;
        if (prev != endVertex) prev->setColor(Node::Highlighted1);
    }

    if (dist[endId] != 1e9) result = tr("Length of the shortest path from %1 to %2 = %3")
                                        .arg(QString::number(startId), QString::number(endId), QString::number(length));
    else result = tr("There is no path from vertex %1 to %2")
                                        .arg(QString::number(startId), QString::number(endId));

    return 1;
}

void Dijkstra::onVertexSelected(Node *node)
{
    if (sleeper->isStopped()) return;
    if (!startVertex) {
        startVertex = node;
        startVertex->setColor(Node::Highlighted2);
        emit updateHintLabel("Select the end vertex of the shortest path");

    } else if (!endVertex && startVertex != node) {
        endVertex = node;
        endVertex->setColor(Node::Highlighted2);
        setUp();
    }
}
