#ifndef BFS_H
#define BFS_H

#include <QObject>
#include "abstractalgorithm.h"

class BFS : public AbstractAlgorithm
{
    Q_OBJECT
public:
    explicit BFS(QString name, Graph* graph, QObject *parent = nullptr);

    void setUp();
    bool run();
    void stop();

public slots:
    virtual void onVertexSelected(Node* node);

private:
    Node* startVertex;
};

#endif // BFS_H
