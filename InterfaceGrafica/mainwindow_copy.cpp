#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>

#include <QFile>
#include <QString>
#include <QTextStream>
#include <QMessageBox>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void  MainWindow::on_pushButton_2_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,"Arquivo de Entrada","/home");
    QFile file(fileName);

    if(!file.open((QFile::ReadOnly | QFile::Text)))
    {
        QMessageBox::warning(this,"ERRO","Arquivo não existente.");
    }
    QTextStream in(&file);
    QString s,s2;
    s = in.readAll();
    QStringList list = s.split("|");
    double dTotal = 0;
    for(int i = 0; i < list.size(); i++)
    {
        double d = list[i].toDouble();
        dTotal += d;
        std::cout << d << std::endl;
    }
    std::cout << "dTotal: " << dTotal << std::endl;
}

void MainWindow::EscreveTela(QString text)
{
    ui->CaixaDeTexto->setPlainText(text);
}
