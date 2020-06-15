#ifndef HAMILTONIANCYCLE_H
#define HAMILTONIANCYCLE_H

#include <abstractalgorithm.h>

class HamiltonianCycle : public AbstractAlgorithm
{
    Q_OBJECT
public:
    explicit HamiltonianCycle(QString name, Graph* graph, QObject *parent = nullptr);

    void setUp();
    bool run();
    void stop();

public slots:
    virtual void onVertexSelected(Node* node);

private:
};
#endif // HAMILTONIANCYCLE_H
