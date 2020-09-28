#include <QInputDialog>

#include "abstractalgorithm.h"
#include "mainwindow.h"

AbstractAlgorithm::AbstractAlgorithm(QString name, Graph* graph, QObject *parent) :
    QObject(parent), graph(graph), name(name)
{
    sleeper = new Sleeper(this);
    maxNode = graph->getMaxNode();

    MainWindow *mainWindow = MainWindow::getInstance();
    Scene* scene = mainWindow ->getScene();
    connect(scene, SIGNAL(vertexSelected(Node*)), this, SLOT(onVertexSelected(Node*)));
    connect(this, SIGNAL(updateHintLabel(QString)), mainWindow, SLOT(updateHintLabel(QString)));
    connect(mainWindow, SIGNAL(stopAlgorithm()), this, SLOT(stop()));
    connect(this, SIGNAL(triggerStopButton()), mainWindow, SLOT(on_stopAlgorithmButton_clicked()));

}

bool AbstractAlgorithm::handleUnweightedEdges()
{
    if (graph->existUnweightedEdges()) {
        bool ok;
        int weight = QInputDialog::getInt(MainWindow::getInstance(), tr("Unweighted edges detected\n"),
                                     tr("Select the default weight for unweighted edges:             "), 1, 0, 999, 1, &ok);
        if (ok) graph->updateUnweightedEdges(weight);
        else {
            emit triggerStopButton();
            return 0;
        }
    }
    return 1;
}


void AbstractAlgorithm::stop() {
    graph->restoreDefaultState();
    sleeper->stop();
    emit updateHintLabel("Algorithm stopped");
}

void AbstractAlgorithm::destroy() {
    delete sleeper;
    delete this; //suicide
}

