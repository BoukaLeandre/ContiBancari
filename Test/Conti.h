#ifndef CONTI_H
#define CONTI_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Transazione {
public:
    string tipo;
    double importo;
    string data;

    Transazione(string t, double i, string d) : tipo(t), importo(i), data(d) {}
};

class ContoCorrente {
protected:
    string utente;
    string tipoConto;
    vector<Transazione> transazioni;

public:
    ContoCorrente(string nome, string tipo) : utente(nome), tipoConto(tipo) {}

    void aggiungiTransazione(const Transazione& t) {
        if (t.importo <= 0) return;
        transazioni.push_back(t);
    }

    virtual double calcolaSaldo() const {
        double saldo = 0.0;
        for (const auto& t : transazioni) {
            if (t.tipo == "entrata")
                saldo += t.importo;
            else if (t.tipo == "uscita")
                saldo -= t.importo;
        }
        return saldo;
    }

    virtual void applicaInteresse() = 0;

    virtual ~ContoCorrente() {}
};

class ContoRisparmio : public ContoCorrente {
public:
    ContoRisparmio(string nome) : ContoCorrente(nome, "Conto Risparmio") {}

    void applicaInteresse() override {

    }
};

class ContoDeposito : public ContoCorrente {
private:
    double interessePercentuale;

public:
    ContoDeposito(string nome, double interesse)
        : ContoCorrente(nome, "Conto Deposito"), interessePercentuale(interesse) {}

    void applicaInteresse() override {
        double saldoAttuale = calcolaSaldo();
        double interesse = saldoAttuale * interessePercentuale;
        if (interesse > 0)
            aggiungiTransazione(Transazione("entrata", interesse, "oggi"));
    }
};

#endif // CONTI_H
