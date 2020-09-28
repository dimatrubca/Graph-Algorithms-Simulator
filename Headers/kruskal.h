#ifndef KRUSKAL_H
#define KRUSKAL_H

#include <QObject>
#include <abstractalgorithm.h>
#include <graph.h>

class Kruskal : public AbstractAlgorithm
{
    Q_OBJECT
public:
    explicit Kruskal(QString name, Graph* graph, QObject *parent = nullptr);

    void setUp();
    bool run();
    void stop();

public slots:
    virtual void onVertexSelected(Node* node);

protected:
    int mstCost;
private:
    int find(int x);
    bool isConnected();

    Node* startVertex;
    std::vector<VLGraphEdge> Edges;
    std::vector<int> p;

};


#endif // KRUSKAL_H
