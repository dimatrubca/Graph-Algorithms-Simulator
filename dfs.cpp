#include <stack>
#include "dfs.h"
#include "mainwindow.h"
#define asleep() if(!sleeper->sleep(800)) return false;

DFS::DFS(QString name, Graph* graph, QObject *parent) :
    AbstractAlgorithm(name, graph, parent), startVertex(0)
{
    emit updateHintLabel("Select starting vertex");
}

void DFS::setUp() {
    emit updateHintLabel("DFS running");
    if (run()) {
        if (result.size() && result[result.size() - 1] == ',') result = result.left(result.size() - 1);
        emit updateHintLabel(result);
    }
}

bool DFS::run() {
    std::vector<std::vector<VLGraphEdge>> V = graph->toVectorList();
    std::stack<std::pair<Node*, Edge*> > S;
    std::vector<int> viz(maxNode + 1, 0);

    S.push({startVertex, 0});
    result = "DFS traversal order:";
    while (S.size()) {
        Node* x = S.top().first;
        Edge* e = S.top().second;
        S.pop();

        if (viz[x->id()]) continue;
        viz[x->id()] = 1;
        result.append(tr(" %1,").arg(QString::number(x->id())));

        if (e) e->setColor(Edge::Highlighted);
        x->setColor(Node::Highlighted2);
        asleep();

        for (auto it: V[x->id()]) {
            if (viz[it.to]) continue;
            Edge* edge = it.edge;
            S.push({it.v2, edge});
        }
    }

    return true;
}

void DFS::onVertexSelected(Node *node)
{
    if (sleeper->isStopped()) return;
    if (!startVertex) {
        startVertex = node;
        setUp();
    }
}
