#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

// Classe che rappresenta una transazione
class Transazione {
public:
    string tipo; // "entrata" o "uscita"
    double importo;
    string data;

    Transazione(string t, double i, string d) : tipo(t), importo(i), data(d) {}
};

// Classe base astratta per Conto Corrente
class ContoCorrente {
protected:
    string intestatario;
    vector<Transazione> transazioni;

public:
    ContoCorrente(string nome) : intestatario(nome) {}

    void aggiungiTransazione(const Transazione& t) {
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
        cout << "Estratto conto di " << intestatario << ":\n";
        for (const auto& t : transazioni) {
            cout << t.data << " - " << t.tipo << ": " << t.importo << " euro\n";
        }
        cout << "Saldo finale: " << calcolaSaldo() << " euro\n";
    }

    // 🔽 Sauvegarde sur fichier
    virtual void salvaSuFile(ofstream& out) const {
        out << intestatario << endl;
        for (const auto& t : transazioni) {
            out << t.tipo << " " << t.importo << " " << t.data << endl;
        }
        out << "#" << endl; // Fin du compte
    }

    // 🔽 Chargement depuis fichier
    virtual void caricaDaFile(ifstream& in) {
        transazioni.clear();
        in >> intestatario;
        string tipo;
        double importo;
        string data;

        while (in >> tipo && tipo != "#") {
            in >> importo >> data;
            transazioni.emplace_back(tipo, importo, data);
        }
    }

    virtual ~ContoCorrente() {}
};

// Conto Risparmio
class ContoRisparmio : public ContoCorrente {
public:
    ContoRisparmio(string nome) : ContoCorrente(nome) {}

    void stampaTipo() const {
        cout << "Tipo di conto: Risparmio\n";
    }
};

// Conto Deposito con interessi
class ContoDeposito : public ContoCorrente {
private:
    double interessePercentuale;

public:
    ContoDeposito(string nome, double interesse) : ContoCorrente(nome), interessePercentuale(interesse) {}

    void applicaInteresse() {
        double saldoAttuale = calcolaSaldo();
        double interesse = saldoAttuale * interessePercentuale;
        aggiungiTransazione(Transazione("entrata", interesse, "oggi"));
    }

    void stampaTipo() const {
        cout << "Tipo di conto: Deposito con interesse " << interessePercentuale * 100 << "%\n";
    }
};

// -------------------- TEST UNITARI --------------------

void test_calcolaSaldo_ContoRisparmio() {
    ContoRisparmio test("Test");
    test.aggiungiTransazione(Transazione("entrata", 100, "2025-01-01"));
    test.aggiungiTransazione(Transazione("uscita", 30, "2025-01-02"));
    double saldo = test.calcolaSaldo();

    if (saldo == 70) {
        cout << "[OK] test_calcolaSaldo_ContoRisparmio" << endl;
    } else {
        cout << "[ERRORE] test_calcolaSaldo_ContoRisparmio: atteso 70, ottenuto " << saldo << endl;
    }
}

void test_applicaInteresse_ContoDeposito() {
    ContoDeposito test("Test", 0.10); // 10% interesse
    test.aggiungiTransazione(Transazione("entrata", 1000, "2025-01-01"));
    test.applicaInteresse(); // aggiunge 100 euro di interesse

    double saldo = test.calcolaSaldo();

    if (saldo == 1100) {
        cout << "[OK] test_applicaInteresse_ContoDeposito" << endl;
    } else {
        cout << "[ERRORE] test_applicaInteresse_ContoDeposito: atteso 1100, ottenuto " << saldo << endl;
    }
}

// -------------------- MAIN --------------------

int main() {
    // Creazione conto risparmio
    ContoRisparmio conto1("Leandre Siddhata");
    conto1.aggiungiTransazione(Transazione("entrata", 500, "2025-06-01"));
    conto1.aggiungiTransazione(Transazione("uscita", 150, "2025-06-02"));
    conto1.stampaTipo();
    conto1.stampaEstrattoConto();

    cout << endl;

    // Creazione conto deposito
    ContoDeposito conto2("Bouka", 0.05); // 5% interesse
    conto2.aggiungiTransazione(Transazione("entrata", 1000, "2025-06-01"));
    conto2.applicaInteresse();
    conto2.stampaTipo();
    conto2.stampaEstrattoConto();

    // ✅ Sauvegarde su file
    ofstream outFile("conti_salvati.txt");
    conto1.salvaSuFile(outFile);
    conto2.salvaSuFile(outFile);
    outFile.close();
    cout << "\n[OK] Conti salvati su file conti_salvati.txt\n";

    // ✅ Lettura da file
    ifstream inFile("conti_salvati.txt");
    ContoRisparmio conto3("Vuoto");
    ContoDeposito conto4("Vuoto", 0.05);
    conto3.caricaDaFile(inFile);
    conto4.caricaDaFile(inFile);
    inFile.close();

    cout << "\n[OK] Conti letti da file\n";
    conto3.stampaEstrattoConto();
    conto4.stampaEstrattoConto();

    cout << "\n--- Esecuzione Test Unitari ---\n";
    test_calcolaSaldo_ContoRisparmio();
    test_applicaInteresse_ContoDeposito();

    return 0;
}
