#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cassert>

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
        if (t.importo <= 0) {
            cerr << "L'importo deve essere maggiore di zero. Transazione annullata.\n";
            return;
        }
        transazioni.push_back(t);
    }

    virtual double calcolaSaldo() const {
        double saldo = 0.0;
        for (const auto& t : transazioni) {
            if (t.tipo == "entrata") {
                saldo += t.importo;
            } else if (t.tipo == "uscita") {
                saldo -= t.importo;
            }
        }
        return saldo;
    }

    virtual void stampaEstrattoConto() const {
        cout << "\n--- Estratto conto ---\n";
        cout << "utente: " << utente << "\n";
        cout << "Tipo di conto: " << tipoConto << "\n";
        for (const auto& t : transazioni) {
            cout << t.data << " - " << t.tipo << ": " << t.importo << " €\n";
        }
        cout << "Saldo finale: " << calcolaSaldo() << " €\n";
        cout << "----------------------\n";
    }

    virtual void salvaSuFile(const string& nomeFile) const {
        ofstream file(nomeFile);
        if (file.is_open()) {
            file << "utente: " << utente << "\n";
            file << "Tipo di conto: " << tipoConto << "\n";
            for (const auto& t : transazioni) {
                file << "tipo: " << t.tipo << "\n";
                file << "importo: " << t.importo << " €\n";
                file << "data: " << t.data << "\n";
            }
            file << "Saldo finale: " << calcolaSaldo() << " €\n";
            file.close();
        } else {
            cerr << "Errore nell'apertura del file.\n";
        }
    }

    virtual void applicaInteresse() = 0; // Méthode virtuelle pure

    virtual ~ContoCorrente() {}
};

class ContoRisparmio : public ContoCorrente {
public:
    ContoRisparmio(string nome) : ContoCorrente(nome, "Conto Risparmio") {}
    void applicaInteresse() override {
        // Aucun intérêt appliqué
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
        if (interesse > 0) {
            aggiungiTransazione(Transazione("entrata", interesse, "oggi"));
            cout << " Interesse applicato: +" << interesse << " €\n";
        }
    }
};


void testContoRisparmio() {
    ContoRisparmio conto("Leandre");
    conto.aggiungiTransazione(Transazione("entrata", 100.0, "01-06-2025"));
    conto.aggiungiTransazione(Transazione("uscita", 40.0, "02-06-2025"));
    assert(conto.calcolaSaldo() == 60.0);
}

void testContoDeposito() {
    ContoDeposito conto("Roosvelt", 0.10);
    conto.aggiungiTransazione(Transazione("entrata", 200.0, "01-06-2025"));
    conto.applicaInteresse();
    double saldo = conto.calcolaSaldo();
    assert(saldo > 200.0);
}

int main() {
    testContoRisparmio();
    testContoDeposito();
    cout << "\n Tutti i testi hanno riusciti con successo.\n";
    return 0;
}
