#ifndef GRAPH_H
#define GRAPH_H

#include <QObject>
#include "node.h"
#include "edge.h"
#include <vector>

class Scene;

struct VLGraphEdge {
    int to, weight;
    Edge* edge;
    Node *v1, *v2;
};

class Graph : public QObject
{
    Q_OBJECT
public:
    explicit Graph(Scene *scene, QObject *parent = nullptr);
    void addNode(QPointF pos, int id = 0);
    void addEdge(Node* v1, Node* v2, bool isDirected=0, bool isWeighted=0, int weight = 1);
    Node* findNode(int id);
    Edge* findEdgeAB(Node* v1, Node* v2);
    Edge* findUndirectEdgeAB(Node *v1, Node *v2);
    Edge* findAnyEdge(Node*v1, Node* v2);


    void updateEdgesAB(Node *v1, Node *v2);
    void clearGraph();
    void setDefaultColor();
    bool hasDirectedEdges();
    bool isMixed();
    bool existUnweightedEdges();
    void updateUnweightedEdges(int weight);
    void restoreDefaultState();

    inline int getMaxNode() { return maxNode; }
    inline int getNumberNode() { return Nodes.size(); }
    inline int getNumberEdges() { return Edges.size(); }
    inline Node* getFirstNode() { return Nodes.size()?Nodes.front():0; }
    inline std::vector<Node*> getNodes() { return Nodes; }

    //returns next
    std::vector<Edge*>::iterator removeEdge(Edge* edge, bool updateEdges);
    std::vector<Node*>::iterator removeNode(Node *v1);
    std::vector<std::vector<VLGraphEdge>> toVectorList();
    std::vector<std::vector<VLGraphEdge>> toVectorListWithIndex();

signals:
    void actionPerformed(QString mess);
    void graphStateChanged(int nodes, int edges);

public slots:
    void saveToFile();
    void loadFromFile();

private:
    int maxNode = 0;
    Scene* scene;

    std::vector <Edge*> Edges;
    std::vector <Node*> Nodes;
};

#endif // GRAPH_H
