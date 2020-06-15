#ifndef EULERCYCLE_H
#define EULERCYCLE_H

#include "abstractalgorithm.h"

class EulerCycle : public AbstractAlgorithm
{
    Q_OBJECT
public:
    explicit EulerCycle(QString name, Graph* graph, QObject *parent = nullptr);

    void setUp();
    bool run();
    void stop();


public slots:
    virtual void onVertexSelected(Node* node);

private:
    bool validCycle(std::vector<std::pair<Node*, Edge*> > path);
    Node* getSecondVertex(Edge* e, Node* v);
    Node* startVertex;

};


#endif // EULERCYCLE_H
