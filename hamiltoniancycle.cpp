#include "hamiltoniancycle.h"
#include <algorithm>
#define asleep() if(!sleeper->sleep(800)) return false;
#define str(n) QString::number(n)

HamiltonianCycle::HamiltonianCycle(QString name, Graph *graph, QObject *parent)
    : AbstractAlgorithm(name, graph, parent)
{
    if (graph->getNumberNode() >=11) {
        emit triggerStopButton();
        emit updateHintLabel("Too many verices for detecting Hamiltonian cycle (max. alowed: 10)");
    } else setUp();
}

void HamiltonianCycle::setUp() {
    emit updateHintLabel("Hamiltonian cycle running");
    if (run()) emit updateHintLabel(result);
}

bool HamiltonianCycle::run() {
    std::vector<std::vector<VLGraphEdge>> V = graph->toVectorListWithIndex();
    std::vector<Node*> Nodes = graph->getNodes();
    std::vector<std::vector<bool>> G(maxNode + 1);
    std::vector<int> b;

    if (Nodes.empty()) {
        emit triggerStopButton();
        emit updateHintLabel("Graph is null");
        return 0;
    }

    for (int i=1; i <= maxNode; i++) G[i].resize(maxNode+1, 0);
    for (auto it: Nodes) b.push_back(it->id());
    for (unsigned int i=0; i<V.size(); i++) {
        for (auto edge: V[i]) {
            G[i][edge.to] = 1;
        }
    }

    bool flag = 1;
    do {
        flag = 1;
        for (unsigned int i=1; i < b.size() && flag; i++) {
            int x2 = b[i];
            int x = b[i-1];
            if (!G[x][x2]) flag = 0;
        }
        if (!G[b.back()][b[0]]) flag = 0;
        if (flag) break;
    } while (std::next_permutation(b.begin(), b.end()));
    if (flag) {
        result = tr("Hamiltonian Cycle: %1").arg(str(b[0]));
        b.push_back(b[0]);
        for (unsigned int i=1; i < b.size(); i++) {
            Node* v1 = graph->findNode(b[i-1]);
            Node* v2 = graph->findNode(b[i]);
            Edge* edge = graph->findAnyEdge(v1, v2);

            if (i==1) {
                v1->setColor(Node::Highlighted2);
                asleep();
            }
            result.append(tr(" - %1").arg(str(v2->id())));
            v2->setColor(Node::Highlighted2);
            edge->setColor(Edge::Highlighted);
            edge->setDrawMiddleArrow(v2, 1);
            asleep();
        }
    } else {
        result = "The graph doesn't have a Hamiltonian Cycle";
        emit triggerStopButton();
    }
    return 1;
}


void HamiltonianCycle::onVertexSelected(Node *node) {}
