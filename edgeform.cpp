#include "edgeform.h"
#include "ui_edgeform.h"
#include <QIntValidator>

EdgeForm::EdgeForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EdgeForm)
{
    ui->setupUi(this);
    ui->weightLineEdit->setValidator(new QIntValidator(0, 999999, this) );
    ui->weightLineEdit->setVisible(0);
}

EdgeForm::~EdgeForm()
{
    delete ui;
}

bool EdgeForm::getIsWeighted()
{
    return isWeighted;
}

bool EdgeForm::getIsDirected()
{
    return isDirected;
}

int EdgeForm::getWeight()
{
    return weight;
}

void EdgeForm::on_directedCheckBox_stateChanged(int arg1)
{
    isDirected = arg1;
}

void EdgeForm::on_okButton_clicked()
{
    weight = ui->weightLineEdit->text().toInt();
    accept();
}

void EdgeForm::on_weightedCheckBox_stateChanged(int arg1)
{
    isWeighted = arg1;
    ui->weightLineEdit->setVisible(isWeighted);
}
