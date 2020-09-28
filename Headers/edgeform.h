#ifndef EDGEFORM_H
#define EDGEFORM_H

#include <QDialog>

namespace Ui {
class EdgeForm;
}

class EdgeForm : public QDialog
{
    Q_OBJECT

public:
    explicit EdgeForm(QWidget *parent = nullptr);
    ~EdgeForm();
    bool getIsWeighted();
    bool getIsDirected();
    int getWeight();

private slots:
    void on_directedCheckBox_stateChanged(int arg1);
    void on_okButton_clicked();
    void on_weightedCheckBox_stateChanged(int arg1);

private:
    Ui::EdgeForm *ui;
    bool isWeighted =0;
    bool isDirected =0;
    int  weight = 0;
};

#endif // EDGEFORM_H
