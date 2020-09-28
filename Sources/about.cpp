#include "about.h"
#include "ui_about.h"

About::About(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);
    ui->textBrowser->viewport()->setAutoFillBackground(false);
    ui->textBrowser->setFrameStyle(QFrame::NoFrame);
}

About::~About()
{
    delete ui;
}
