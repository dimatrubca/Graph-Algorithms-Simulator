#include <queue>
#include "bfs.h"
#include "mainwindow.h"

#define asleep() if(!sleeper->sleep(700)) return false;

BFS::BFS(QString name, Graph* graph, QObject *parent) :
    AbstractAlgorithm(name, graph, parent), startVertex(0)
{
    emit updateHintLabel("Select starting vertex");
}

void BFS::setUp() {
    emit updateHintLabel("BFS running");
    if (run()) {
        if (result.size() && result[result.size() - 1] == ',') result = result.left(result.size() - 1);
        emit updateHintLabel(result);
    }
}

bool BFS::run() {
    std::vector<std::vector<VLGraphEdge>> V = graph->toVectorList();
    std::queue<std::pair<Node*, Edge*> > Q;
    std::vector<int> viz(maxNode + 1, 0);

    viz[startVertex->id()] = 1;
    Q.push({startVertex, 0});
    result = "BFS traversal order:";
    while (Q.size()) {
        Node* x = Q.front().first;
        Edge* e = Q.front().second;
        Q.pop();
        result.append(tr(" %1,").arg(QString::number(x->id())));

        if (e) e->setColor(Edge::Highlighted);
        x->setColor(Node::Highlighted2);
        asleep();

        for (auto it: V[x->id()]) {
            if (viz[it.to]) continue;
            viz[it.to] = 1;
            Q.push({it.v2, it.edge});
            it.edge->setColor(Edge::Highlighted);
            it.v2->setColor(Node::Highlighted1);
            asleep();
        }
    }

    return true;
}

void BFS::onVertexSelected(Node *node)
{
    if (sleeper->isStopped()) return;

    if (!startVertex) {
        startVertex = node;
        setUp();
    }
}
