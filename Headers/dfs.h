#ifndef DFS_H
#define DFS_H

#include <QObject>
#include "sleeper.h"
#include "graph.h"
#include "abstractalgorithm.h"

class DFS : public AbstractAlgorithm
{
    Q_OBJECT
public:
    explicit DFS(QString name, Graph* graph, QObject *parent = nullptr);

    void setUp();
    bool run();
    void stop();

public slots:
    virtual void onVertexSelected(Node* node);

private:
    Node* startVertex;

};

#endif // DFS_H
