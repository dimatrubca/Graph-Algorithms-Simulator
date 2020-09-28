#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "node.h"
#include "edge.h"
#include "dfs.h"
#include "bfs.h"
#include "kruskal.h"
#include "dijkstra.h"
#include "eulercycle.h"
#include "maxflow.h"
#include "hamiltoniancycle.h"
#include "about.h"
 #include <QShortcut>

MainWindow* MainWindow:: instance;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), algorithmInstance(nullptr)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stopAlgorithmButton->setVisible(0);

    Scene* scene = new Scene(this);
    this->scene = scene;
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);

    QShortcut *shortcut = new QShortcut(QKeySequence("Ctrl+E"), this);
    QObject::connect(shortcut, SIGNAL(activated()), this, SLOT(on_addEdgeButton_clicked()));

    QShortcut *shortcut2 = new QShortcut(QKeySequence("Ctrl+N"), this);
    QObject::connect(shortcut2, SIGNAL(activated()), this, SLOT(on_addNodeButton_clicked()));

    QShortcut *shortcut3 = new QShortcut(QKeySequence("Ctrl+r"), this);
    QObject::connect(shortcut3, SIGNAL(activated()), this, SLOT(on_removeButton_clicked()));

    QShortcut *shortcut4 = new QShortcut(QKeySequence("Ctrl+m"), this);
    QObject::connect(shortcut4, SIGNAL(activated()), this, SLOT(on_dragModeButton_clicked()));

    connect(this, SIGNAL(sendMessage(QString)), this, SLOT(updateHintLabel(QString)));
    connect(scene, SIGNAL(sendHintMessage(QString)), this, SLOT(updateHintLabel(QString)));

    QAction *openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcut(QKeySequence::Open);
    openAct->setStatusTip("Import an existing graph");
    connect(openAct, &QAction::triggered, scene->getGraph(), &Graph::loadFromFile);
    ui->menubar->addAction(openAct);

    QAction *saveAct = new QAction(tr("&Save..."), this);
    saveAct->setShortcut(QKeySequence::Open);
    saveAct->setStatusTip("Save current graph");
    connect(saveAct, &QAction::triggered, scene->getGraph(), &Graph::saveToFile);
    ui->menubar->addAction(saveAct);

    QAction *aboutAct = new QAction(tr("&About..."), this);
    connect(aboutAct, &QAction::triggered, this, &MainWindow::on_about_clicked);

    ui->menubar->addAction(aboutAct);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_addNodeButton_clicked()
{
    scene->setMode(Scene::DrawNode);
    emit sendMessage(tr("Click on the workspace to add a vertex"));
}

void MainWindow::on_addEdgeButton_clicked()
{
    scene->setMode(Scene::DrawEdge);
    emit sendMessage(tr("Select starting vertex"));
}


void MainWindow::on_dragModeButton_clicked()
{
    scene->setMode(Scene::SelectObject);
    emit sendMessage(tr("Move the vertices by mouse"));
}

void MainWindow::on_removeButton_clicked()
{
    scene->setMode(Scene::RemoveItem);
    emit sendMessage(tr("Click on an edge/vertex to delete it"));
}

void MainWindow::updateInfoLabel(QString mess)
{
    ui->lastUpdatesLabel->setText(mess);
}

void MainWindow::updateHintLabel(QString mess) {
    ui->hintLabel->setText(mess);
}

void MainWindow::graphStateChanged(int nodes, int edges) {
    QString mess = QString("Nodes: %1, Edges: %2").arg(QString::number(nodes), QString::number(edges));
    ui->graphInfoLabel->setText(mess);
}

void MainWindow::setUIDisabled(bool isDisabled)
{
    if (isDisabled) scene->setMode(Scene::AlgRunning);
    ui->stopAlgorithmButton->setVisible(isDisabled);
    ui->clearButton->setDisabled(isDisabled);
    ui->addEdgeButton->setDisabled(isDisabled);
    ui->addNodeButton->setDisabled(isDisabled);
    ui->removeButton->setDisabled(isDisabled);
    ui->dragModeButton->setDisabled(isDisabled);
    ui->comboBox->setDisabled(isDisabled);

    QMenuBar* menuBar = ui->menubar;
    menuBar->setDisabled(isDisabled);
}

void MainWindow::on_clearButton_clicked()
{
    Graph* graph = scene->getGraph();
    graph->clearGraph();
}

void MainWindow::on_stopAlgorithmButton_clicked()
{
    emit stopAlgorithm();
    scene->setMode(Scene::SelectObject);
    ui->comboBox->setCurrentIndex(0);
}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    if (index == 0) {
        setUIDisabled(0);
        return;
    } else if (algorithmInstance) {
        algorithmInstance->destroy();
        algorithmInstance = 0;
    }

    setUIDisabled(1);
    Graph* graph = scene->getGraph();
    if (index == AbstractAlgorithm::DFS) {
        algorithmInstance = new DFS("DFS", graph, this);
    } else if (index == AbstractAlgorithm::BFS) {
        algorithmInstance = new BFS("BFS", graph, this);
    } else if (index == AbstractAlgorithm::Kruskal) {
        algorithmInstance = new Kruskal("Kruskal", graph, this);
    } else if (index == AbstractAlgorithm::Dijkstra) {
        algorithmInstance = new Dijkstra("Dijkstra", graph, this);
    } else if (index == AbstractAlgorithm::EulerCycle) {
        algorithmInstance = new EulerCycle("Euler", graph, this);
    } else if (index == AbstractAlgorithm::MaxFlow) {
        algorithmInstance = new MaxFlow("Max Flow", graph, this);
    } else if (index == AbstractAlgorithm::HamiltonianCycle) {
        algorithmInstance = new HamiltonianCycle("Hamiltonian Cycle", graph, this);
    }
}

void MainWindow::on_about_clicked()
{
    About aboutForm;
    aboutForm.setModal(true);
    aboutForm.exec();
}
