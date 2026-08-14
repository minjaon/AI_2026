#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->label->setStyleSheet("background-color : red;");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnPush_clicked()
{
    static int cnt = 0;
    static QStringList color = {"green", "blue", "red"};
    // QString s = QString("background-color:") + color[cnt];
    QString s = QString("background-color:").arg(color[cnt%3]);
    ui->lblColor->setStyleSheet(s);
    cnt++;
}

void MainWindow::on_btnCheckable_toggled(bool checked)
{
}