#include "kruskal.h"
#include "mainwindow.h"
#include <QInputDialog>
#define asleep() if(!sleeper->sleep(800)) return false;

bool cmp(VLGraphEdge l, VLGraphEdge r) {
    int weight1 = l.edge->getWeight();
    int weight2 = r.edge->getWeight();

    return weight1 < weight2;
}

int Kruskal::find(int x) {
    if (x==p[x]) return x;
    else return p[x]=find(p[x]);
}


Kruskal::Kruskal(QString name, Graph* graph, QObject *parent)
    : AbstractAlgorithm(name, graph, parent), mstCost(0)
{
    if (graph->hasDirectedEdges()) {
        emit triggerStopButton();
        emit updateHintLabel("Kruskal algorithm cannot be run on a directed graph. Remove directed edges");
    } else {
        if (graph->existUnweightedEdges()) {
            bool ok;
            int weight = QInputDialog::getInt(MainWindow::getInstance(), tr("Unweighted edges detected\n"),
                                         tr("Select the default weight for unweighted edges:             "), 1, 0, 999, 1, &ok);
            if (ok) {
                 graph->updateUnweightedEdges(weight);
                 setUp();
            } else {
                emit triggerStopButton();
                return;
            }
        } else setUp();
    }
}

void Kruskal::setUp() {
    emit updateHintLabel("Kruskal's algorithm running...");
    if (run()) {
        emit updateHintLabel(result);
    }
}

bool Kruskal::run() {
    std::vector<std::vector<VLGraphEdge>> V = graph->toVectorList();
    for (unsigned int i=0; i<V.size(); i++) {
        for (auto it : V[i]) {
           Edges.push_back(it);
        }
    }

    int maxNode = graph->getMaxNode();
    p.resize(maxNode + 1);
    for (int i = 1; i <= maxNode; i++) p[i]=i;

    sort(Edges.begin(), Edges.end(), cmp);
    int k = 0;
    for (auto it : Edges) {
        int v1 = it.v1->id();
        int v2 = it.v2->id();
        int weight = it.edge->getWeight();

        int a = find(v1), b = find(v2);
        if (a!=b) {
            p[a] = b;
            mstCost += weight;
            Edges[k++] = it;
        }
    }

    if (!isConnected()) {
        result = "The graph is not connected";
        return 1;
    } else  result = tr("Minimum spanning tree cost = %1").arg(mstCost);

    asleep();
    for (int i=0; i<k; i++) {
        Edges[i].edge->setColor(Edge::Highlighted);
        Edges[i].edge->v1()->setColor(Node::Highlighted2);
        Edges[i].edge->v2()->setColor(Node::Highlighted2);
        asleep();

    }

    return 1;
}

bool Kruskal::isConnected()
{
    std::vector<Node*> V = graph->getNodes();
    for (unsigned int i=1 ; i < V.size(); i++) {
        int a = find(V[i]->id());
        int b = find(V[i-1]->id());
        if (a != b) return 0;
    }

    return 1;
}

void Kruskal::onVertexSelected(Node* node) { }
