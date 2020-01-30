#include "motor.h"

#include<iostream>
#include<fstream>
using namespace std;

motor::motor()
{
    ka = 0;
    tempo = 0;
    qtdElementos = 0;
}

void motor::setKa(double value)
{
    ka = value;
}

void motor::setTempo(int value)
{
    tempo = value;
}

void motor::setLeituras(QString input, QString Ka)
{
    tensao.clear();
    corrente.clear();
    velocidade.clear();
    ka = Ka.toDouble();
    QStringList list = input.split("|");

    qtdElementos = int((list.size()-1)/3);
    std::ofstream out( "saida.txt" , std::ofstream::out);
    cout << "qtdElementos: " << qtdElementos << endl;
    cout << "list.size(): " << list.size() << endl;

    for(int i = 0; i < list.size()-1; i= i+3)
    {
        //cout << i << endl;
        tensao.push_back( list[i].toDouble() );
        corrente.push_back( list[i+1].toDouble() );
        velocidade.push_back( 60*list[i+2].toDouble() );

        out << list[i].toDouble() << "|" << list[i+1].toDouble() << "|" << list[i+2].toDouble() << "|"<< endl;

    }
    std::cout << "Ka: " << ka << std::endl;
}

int motor::getTamanho()
{
    return qtdElementos;
}

std::list<double> motor::getTensao()
{
    return tensao;
}

std::list<double> motor::getCorrente()
{
    return corrente;
}

std::list<double> motor::getVelocidade()
{
    return velocidade;
}
