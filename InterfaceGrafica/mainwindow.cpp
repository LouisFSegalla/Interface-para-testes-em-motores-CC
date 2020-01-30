#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <list>

#include <QFile>
#include <QString>
#include <QTextStream>
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>
#include <cmath>


//biblioteca para comunicação serial
#include <QSerialPort>
#include <QSerialPortInfo>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    //fecha o arduino caso esteja aberto
    if(arduino->isOpen())
    {
        arduino->close();
    }
}

//Abre arquivo prévio
void  MainWindow::on_pushButton_2_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,"Arquivo de Entrada","/home");
    QFile file(fileName);

    if(!file.open((QFile::ReadOnly | QFile::Text)))
    {
        QMessageBox::warning(this,"ERRO","Arquivo não existente.");
    }
    QTextStream in(&file);
    QString s;
    s = in.readAll();
    m.setLeituras(s, ui->plainTextEdit->toPlainText());
}

//Mostra corrente
void MainWindow::on_pushButton_clicked()
{
    std::list<double> auxTensao, auxCorrente, auxVelocidade;
    auxCorrente = m.getCorrente();
    double maxY = 0;
    std::list<double>::iterator it = auxCorrente.begin();

    QVector<double> x(m.getTamanho()), y(m.getTamanho());
    for (int i=0; i<m.getTamanho(); ++i)
    {
      x[i] = i;
      y[i] = *it;
      if(y[i] > maxY){maxY = y[i];}
      it++;
    }
    // create graph and assign data to it:
    ui->customPlot->addGraph();
    ui->customPlot->graph(0)->setData(x, y);
    // give the axes some labels:
    ui->customPlot->xAxis->setLabel("t");
    ui->customPlot->yAxis->setLabel("A");
    // set axes ranges, so we see all data:
    ui->customPlot->xAxis->setRange(0, m.getTamanho());
    ui->customPlot->yAxis->setRange(0, maxY);
    ui->customPlot->replot();

}

//Mostra potência
void MainWindow::on_pushButton_3_clicked()
{
    std::list<double> auxTensao, auxCorrente;

    auxTensao = m.getTensao();
    auxCorrente = m.getCorrente();

    std::list<double>::iterator it1 = auxCorrente.begin();
    std::list<double>::iterator it2 = auxTensao.begin();

    QVector<double> x(m.getTamanho()), y(m.getTamanho());
    double maxY = 0;

    for (int i=0; i<m.getTamanho(); ++i)
    {
      x[i] = i;
      y[i] = (*it1)*(*it2);
      if(y[i] > maxY){maxY = y[i];}
      it1++;
      it2++;
    }

    // create graph and assign data to it:
    ui->customPlot->addGraph();
    ui->customPlot->graph(0)->setData(x, y);
    // give the axes some labels:
    ui->customPlot->xAxis->setLabel("t");
    ui->customPlot->yAxis->setLabel("W");
    // set axes ranges, so we see all data:
    ui->customPlot->xAxis->setRange(0, m.getTamanho());
    ui->customPlot->yAxis->setRange(0, maxY);
    ui->customPlot->replot();
}

//Mostra tensão
void MainWindow::on_pushButton_6_clicked()
{
    std::list<double> auxTensao;
    auxTensao = m.getTensao();
    double maxY = 0;
    std::list<double>::iterator it = auxTensao.begin();

    QVector<double> x(m.getTamanho()), y(m.getTamanho());
    for (int i=0; i<m.getTamanho(); ++i)
    {
      x[i] = i;
      y[i] = *it;
      if(y[i] > maxY){maxY = y[i];}
      it++;
    }
    // create graph and assign data to it:
    ui->customPlot->addGraph();
    ui->customPlot->graph(0)->setData(x, y);
    // give the axes some labels:
    ui->customPlot->xAxis->setLabel("t");
    ui->customPlot->yAxis->setLabel("V");
    // set axes ranges, so we see all data:
    ui->customPlot->xAxis->setRange(0, m.getTamanho());
    ui->customPlot->yAxis->setRange(0, maxY);
    ui->customPlot->replot();
}

//Mostra velocidade
void MainWindow::on_pushButton_4_clicked()
{
    std::list<double> auxVelocidade;
    auxVelocidade = m.getVelocidade();
    double maxY = 0;
    std::list<double>::iterator it = auxVelocidade.begin();

    QVector<double> x(m.getTamanho()), y(m.getTamanho());
    for (int i=0; i<m.getTamanho(); ++i)
    {
      x[i] = i;
      y[i] = *it;
      if(y[i] > maxY){maxY = y[i];}
      it++;
    }
    // create graph and assign data to it:
    ui->customPlot->addGraph();
    ui->customPlot->graph(0)->setData(x, y);
    // give the axes some labels:
    ui->customPlot->xAxis->setLabel("t");
    ui->customPlot->yAxis->setLabel("RPM");
    // set axes ranges, so we see all data:
    ui->customPlot->xAxis->setRange(0, m.getTamanho());
    ui->customPlot->yAxis->setRange(0, maxY);
    ui->customPlot->replot();
}

//Ativa o arduino
void MainWindow::on_pushButton_5_clicked()
{
    arduino_is_available = false;
    arduino_port_name = "";
    arduino = new QSerialPort;

    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
        if(serialPortInfo.hasVendorIdentifier() && serialPortInfo.hasProductIdentifier()){
            if(serialPortInfo.vendorIdentifier() == arduino_uno_vendor_id){
                if(serialPortInfo.productIdentifier() == arduino_uno_product_id){
                    arduino_port_name = serialPortInfo.portName();
                    arduino_is_available = true;
                }
            }
        }
    }

    if(arduino_is_available){
        //Abre a configura as portas seriais
        arduino->setPortName(arduino_port_name);
        arduino->open(QSerialPort::ReadOnly);//configura o arduino apenas para leitura
        arduino->setBaudRate(QSerialPort::Baud9600);
        arduino->setDataBits(QSerialPort::Data8);
        arduino->setParity(QSerialPort::NoParity);
        arduino->setStopBits(QSerialPort::OneStop);
        arduino->setFlowControl(QSerialPort::NoFlowControl);
        runtime = abs(ui->plainTextEdit_2->toPlainText().toInt());//Tempo que deve ser usado para coleta de dados
        if(runtime > 600)
        {
            //mensagem que aparece caso o teste exceda o tempo limite
            QMessageBox::warning(this, "Time error", "Tempo excede o máximo possível. Teste reduzido há 10 minutos.");//conferir português
            runtime = 600;
        }
        start = std::chrono::system_clock::now();
        QObject::connect(arduino, SIGNAL(readyRead()), this, SLOT(preencheArduinoRead()) );
    }else{
        //Mostra uma mensagem de erro caso não seja possível acessar a porta
        QMessageBox::warning(this, "Port error", "Não foi possível encontrar o Arduino!");
    }
}

void MainWindow::preencheArduinoRead()
{
    std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
    if(std::chrono::duration_cast<std::chrono::seconds>(end-start).count() > runtime)//compara se o tempo delimitado foi atingido | ficar de olho
    {
        arduino->blockSignals(true);
        m.setLeituras(serialBuffer,ui->plainTextEdit->toPlainText());
    }
    QByteArray daraSerial = arduino->readAll();
    serialBuffer += QString::fromStdString(daraSerial.toStdString());
    //std::cout << serialBuffer.toStdString();
    std::cout << "tamanho do vetor: " << serialBuffer.size() << std::endl;


}
