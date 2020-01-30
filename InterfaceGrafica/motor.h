#ifndef MOTOR_H
#define MOTOR_H
#include <list>
#include <QFile>
#include <QString>

class motor
{
public:
    motor();
    void setKa(double value);
    void setTempo(int value);
    void setLeituras(QString input, QString Ka);
    int  getTamanho();
    std::list<double> getTensao();
    std::list<double> getCorrente();
    std::list<double> getVelocidade();
private:
    double ka;
    int tempo;
    int qtdElementos;
    std::list<double> tensao;
    std::list<double> corrente;
    std::list<double> velocidade;
};

#endif // MOTOR_H
