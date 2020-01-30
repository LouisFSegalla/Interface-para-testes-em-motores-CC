#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "motor.h"
#include "qcustomplot.h"

#include <QMainWindow>
#include <QFile>
#include <QString>
#include <chrono>

//Bibliotecas para comunicação serial
#include <QSerialPort>
#include <QSerialPortInfo>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();//mostra a corrente
    void on_pushButton_2_clicked();//abre um arquivo prévio
    void on_pushButton_3_clicked();//mostra a potência
    void on_pushButton_6_clicked();//mostra a tensão
    void on_pushButton_4_clicked();//mostra a velocidade angular em rps
    void on_pushButton_5_clicked();//ativa o arduino
    void preencheArduinoRead();//função usada para escrever o que é lido pela saída serial
private:
    Ui::MainWindow *ui;
    motor m;

    //informações sobre o arduino
    QSerialPort *arduino;
    static const quint16 arduino_uno_vendor_id = 9025;
    static const quint16 arduino_uno_product_id = 67;
    QString arduino_port_name;
    bool arduino_is_available;

    //string usada para ler os valores da porta serial
    QString serialBuffer;//se pah que não precisa desse static

    std::chrono::time_point<std::chrono::system_clock> start;
    int runtime;
};
#endif // MAINWINDOW_H
