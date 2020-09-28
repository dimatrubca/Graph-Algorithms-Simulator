#include "eulercycle.h"
#include <stack>
#include <algorithm>
#define asleep() if(!sleeper->sleep(800)) return false;


EulerCycle::EulerCycle(QString name, Graph* graph, QObject *parent):
    AbstractAlgorithm(name, graph, parent)
{
    if (graph->isMixed()) {
        emit triggerStopButton();
        emit updateHintLabel("Current implementation of Euler cycle finder cannot be run on a mixed graph. Remove all of the undirected or directed edges.");
    } else setUp();
}


void EulerCycle::setUp() {
    emit updateHintLabel("Computing Euler cycle...");
    if (run()) {
        if (result.size() && result[result.size() - 1] == '-') result = result.left(result.size() - 1);
        emit updateHintLabel(result);
    }
}

bool EulerCycle::run() {
    std::vector<std::vector<VLGraphEdge>> V = graph->toVectorListWithIndex();
    std::stack<std::pair<Node*, Edge*> > S;
    std::vector<std::pair<Node*, Edge*> > Path;
    std::vector<bool> u(1000, 0); //!

    //!check if not empty graph
    startVertex = graph->getFirstNode();
    if (!startVertex) {
        emit triggerStopButton();
        emit updateHintLabel("Graph is null");
        return 0;
    }

    Node* lastVertex = 0;
    S.push({startVertex, 0});
    result = "Euler cycle: ";
    while (S.size()) {
        Node* x = S.top().first, *x2 = x;
        Edge* e = S.top().second;
        if (!e) x2=startVertex;
        else if (x2->id() != e->v2()->id()) x2 = e->v1();

        int v = x->id();
        if (V[v].size()) {
            Node* v2 = V[v].back().v2;
            Edge* e = V[v].back().edge;
            int idx = V[v].back().weight;
            qDebug() <<" v2, idx, " <<v2<<" "<<idx<<'\n';
            qDebug() <<"idx = "<<idx<<" "<<u[idx];
            if (!u[idx]) {
                u[idx] = 1;
                S.push({v2, e});
            }

            V[v].pop_back();
        } else {
            S.pop();
            Path.push_back({x2, e});
            if (!lastVertex) lastVertex = x;
        }
    }

    std::reverse(Path.begin(), Path.end());
    if (!validCycle(Path)) {
        result = "There is no Euler Cycle";
        emit triggerStopButton();
        return 1;
    }

    startVertex->setColor(Node::Highlighted2);asleep();
    result.append(QString::number(startVertex->id()));

    for (unsigned int i=1; i<Path.size(); i++) {
        Node* node = Path[i].first;
        Edge* edge = Path[i].second;
        edge->setDrawMiddleArrow(node, 1);
        edge->setColor(Edge::Highlighted);

        node->setColor(Node::Highlighted1);
        result.append(tr(" - %1").arg(QString::number(node->id())));
        asleep();
    }

    return true;
}

bool EulerCycle::validCycle(std::vector<std::pair<Node*, Edge*> > path) {
    std::vector<int> vis(graph->getNumberEdges());
    if (path.front().first != path.back().first) return 0;
    if (path.size() != graph->getNumberEdges() + 1) return 0;

    for (unsigned int i=1; i<path.size(); i++) {
        Node* v2 = getSecondVertex(path[i].second, path[i].first);
        if (path[i-1].first != v2) return 0;
        if (!graph->findAnyEdge(path[i-1].first, path[i].first)) return 0;
    }
    return 1;
}

Node* EulerCycle::getSecondVertex(Edge* edge, Node* node) {
    if (!edge) return node;
    if (edge->v1() != node) return edge->v1();
    else return edge->v2();
}

void EulerCycle::onVertexSelected(Node *node) { }
