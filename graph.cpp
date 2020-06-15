#include "graph.h"
#include "scene.h"
#include "mainwindow.h"
#include<iterator>
#include<QFileDialog>

Graph::Graph(Scene* scene, QObject *parent)
    : QObject(parent), scene(scene)
{

}

void Graph::addNode(QPointF pos, int id)
{
    if (!id) id = ++maxNode;
    Node *node = new Node(id, pos);
    Nodes.push_back(node);
    scene->addItem(node);

    emit actionPerformed(tr("Added node: %1").arg(QString::number(node->id())));
    emit graphStateChanged(Nodes.size(), Edges.size());
}

void Graph::addEdge(Node *v1, Node *v2, bool isDirected, bool isWeighted, int weight)
{
    if (isDirected) {
        removeEdge(findEdgeAB(v1, v2), 0);
        removeEdge(findUndirectEdgeAB(v1, v2), 0);
    } else {
        removeEdge(findEdgeAB(v1,v2), 0);
        removeEdge(findEdgeAB(v2, v1), 0);
    }

    Edge* edge = new Edge(v1, v2, isDirected, isWeighted, weight);
    v1->addEdge(edge, 1);
    v2->addEdge(edge, 0);
    scene->addItem(edge);
    Edges.push_back(edge);

    updateEdgesAB(v1, v2);

    emit actionPerformed(tr("Added edge %1-%3%2").arg(QString::number(v1->id()), QString::number(v2->id()), isDirected?">":""));
    emit graphStateChanged(Nodes.size(), Edges.size());

}

//edge may be null
//returns a pointer to the next edge
std::vector<Edge*>::iterator Graph::removeEdge(Edge* edge, bool updateEdges) {
    if (!edge) return Edges.end();

    qDebug() <<"Removing edge " << edge->v1()->id()<<"-" <<edge->v2()->id();
    auto next = Edges.erase(find(Edges.begin(), Edges.end(), edge));
    edge->v1()->removeEdge(edge);
    edge->v2()->removeEdge(edge);
    scene->removeItem(edge);
    if (updateEdges) updateEdgesAB(edge->v1(), edge->v2());

    emit actionPerformed(tr("Removed edge %1-%2").arg(QString::number(edge->v1()->id()), QString::number(edge->v2()->id())));
    emit graphStateChanged(Nodes.size(), Edges.size());
    delete edge;

    return next;
}

//sets isSingle
void Graph::updateEdgesAB(Node *v1, Node *v2) {
    int cnt = 0;
    for (auto edge : Edges) {
        if ((edge->v1() == v1 && edge->v2() == v2) || (edge->v1() == v2 && edge->v2() == v1)) {
            cnt++;
        }
    }
    qDebug() << "Updating edge: " << v1->id() <<"-" << v2->id();
    if (!cnt) return;
    bool isSingle = (cnt==1);
    for (auto edge : Edges) {
        if ((edge->v1() == v1 && edge->v2() == v2) || (edge->v1() == v2 && edge->v2() == v1)) {
            edge->setIsSingle(isSingle);
        }

    }
}

std::vector<Node*>::iterator Graph::removeNode(Node *v) {
    qDebug() << "Removing vertex: " << v->id();

    for (auto it = Edges.begin(); it!=Edges.end(); ) {
        if ((*it)->v1() == v || (*it)->v2() == v) {
            it = removeEdge((*it), 0);
        } else ++it;
    }
    auto next = Nodes.erase(find(Nodes.begin(), Nodes.end(), v));
    //Nodes.erase(std:: remove(Nodes.begin(), Nodes.end(), v), Nodes.end()); // check
    scene->removeItem(v);

    emit actionPerformed(tr("Removed node: %1").arg(QString::number(v->id())));
    emit graphStateChanged(Nodes.size(), Edges.size());
    delete v;

    return next;
}

void Graph::clearGraph()
{
    qDebug() << "clear";
    for (auto edge = Edges.begin(); edge != Edges.end(); ) {
        edge = removeEdge(*edge, 1);
    }

    for (auto node = Nodes.begin(); node != Nodes.end(); ) {
        node = removeNode(*node);
    }
    maxNode = 0;

    emit actionPerformed("Graph deleted");
}

std::vector<std::vector<VLGraphEdge> > Graph::toVectorList()
{
    std::vector<std::vector<VLGraphEdge> > V(maxNode + 1);
    for (auto edge : Edges) {
        int from = edge->v1()->id();
        int to = edge->v2()->id();
        int weight = edge->getWeight();
        Node* v1 = edge->v1();
        Node* v2 = edge->v2();

        if (edge->getIsDirected()) {
            V[from].push_back({to, weight, edge, v1, v2});
        } else {
            V[from].push_back({to, weight, edge, v1, v2});
            V[to].push_back({from, weight, edge, v2, v1});
        }
    }

    return V;
}

std::vector<std::vector<VLGraphEdge> > Graph::toVectorListWithIndex()
{
    std::vector<std::vector<VLGraphEdge> > V(maxNode + 1);
    int idx = 0;
    for (auto edge : Edges) {
        int from = edge->v1()->id();
        int to = edge->v2()->id();
        int weight = idx;
        Node* v1 = edge->v1();
        Node* v2 = edge->v2();

        if (edge->getIsDirected()) {
            V[from].push_back({to, weight, edge, v1, v2});
        } else {
            V[from].push_back({to, weight, edge, v1, v2});
            V[to].push_back({from, weight, edge, v2, v1});
        }
        idx++;
    }

    return V;
}

Edge *Graph::findEdgeAB(Node *v1, Node *v2)
{
    for (auto edge : Edges) {
        if (edge->v1() == v1 && edge->v2() == v2) return edge;
    }
    return 0;
}

Edge *Graph::findUndirectEdgeAB(Node *v1, Node *v2) {
    for (auto edge : Edges) {
        if (edge->getIsDirected() == 0 && ((edge->v1() == v1 && edge->v2() == v2) || (edge->v1() == v2 && edge->v2() == v1))) return edge;
    }
    return 0;
}

Edge *Graph::findAnyEdge(Node *v1, Node *v2)
{
    Edge* edge = findEdgeAB(v1,v2);
    if (!edge) edge = findUndirectEdgeAB(v1,v2);
    return edge;
}

Node *Graph::findNode(int id)
{
    for (auto node: Nodes) {
        if (node->id() == id) return node;
    }
    qDebug("Node not found");
    return nullptr;
}

void Graph::setDefaultColor() {
    for (auto edge : Edges) {
        edge->setColor(Edge::Default);
    }

    for (auto node : Nodes) {
        node->setColor(Node::Default);
    }
}

bool Graph::hasDirectedEdges()
{
    for (auto edge: Edges) {
        if (edge->getIsDirected()) return 1;
    }
    return 0;
}

bool Graph::isMixed()
{
    bool flag1 = 0, flag2 = 0;
    for (auto edge: Edges) {
        if (edge->getIsDirected()) flag1 = 1;
        else flag2 = 1;
    }
    return (flag1 && flag2);
}

bool Graph::existUnweightedEdges()
{
    for (auto edge : Edges) {
        if (!edge->getIsWeighted()) return true;
    }
    return 0;
}

void Graph::updateUnweightedEdges(int weight)
{
    for (auto edge : Edges) {
        if (!edge->getIsWeighted()) {
            edge->setWeight(weight);
        }
    }
}

//restores the state of the graph after executing an algorithm
void Graph::restoreDefaultState()
{
    for (auto node: Nodes) {
        node->setColor(Node::Default);
    }
    for (auto edge: Edges) {
        qDebug("Update edge");
        edge->restoreDefaultState();
        qDebug("Updating next:");
    }
}

void Graph::saveToFile()
{
    QString fileName = QFileDialog::getSaveFileName(MainWindow::getInstance(), tr("Save File"),
                                                    "/", tr("Binary files (*.graph)"));
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Could not open file";
        return;
    }

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_5_14);

    out << maxNode << Nodes.size() << Edges.size();
    for (auto node: Nodes) {
        out << node->id() << node->pos();
    }

    for (auto edge: Edges) {
        out << edge->v1()->id() << edge->v2()->id() << edge->getIsDirected() << edge->getIsWeighted() << edge->getWeight();
    }

    file.flush();
    file.close();
    return;
}

void Graph::loadFromFile()
{
    QString fileName = QFileDialog::getOpenFileName(MainWindow::getInstance(), tr("Open File"),
                                                    "/", tr("Binary files (*.graph)"));
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Could not open file for reading";
        return;
    }

    clearGraph();
    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_5_14);
    int  maxNode, nrNodes, nrEdges;

    in >> maxNode >> nrNodes >> nrEdges;
    if (in.status() == QDataStream::ReadCorruptData) {
        emit actionPerformed("Corrupted file");
        return;
    }
    for (int i=0; i< nrNodes; i++) {
        int id;
        QPointF pos;
        in >> id >> pos;
        addNode(pos, id);
    }

    for (int i=0; i<nrEdges; i++) {
        int v1, v2, weight;
        bool isDirected, isWeighted;
        in >> v1 >> v2 >> isDirected >> isWeighted >> weight;
        addEdge(findNode(v1), findNode(v2), isDirected, isWeighted, weight);
    }

    this->maxNode = maxNode;
    file.close();

    return;
}
