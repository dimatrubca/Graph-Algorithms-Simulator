#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include <QtCore>
#include <QtGui>
#include <QGraphicsScene>
#include "abstractalgorithm.h"
#include "scene.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    static MainWindow* getInstance() {
         if(!instance) {
           instance = new MainWindow();
         }
         return instance;
     }

    inline Scene* getScene() { return scene; }

protected:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setUIDisabled(bool isDisabled);

public slots:
    void on_stopAlgorithmButton_clicked();

private slots:
    void on_addNodeButton_clicked();

    void on_addEdgeButton_clicked();

    void on_dragModeButton_clicked();

    void on_removeButton_clicked();

    void on_clearButton_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void on_about_clicked();

signals:
    void sendMessage(QString mess);
    void stopAlgorithm();

public slots:
    void updateInfoLabel(QString mess);
    void updateHintLabel(QString mess);
    void graphStateChanged(int nodes, int edges);

private:
    static MainWindow* instance;
    AbstractAlgorithm* algorithmInstance;
    Ui::MainWindow *ui;
    Scene* scene;
};
#endif // MAINWINDOW_H
