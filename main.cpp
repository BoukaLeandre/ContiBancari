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

    virtual void salvaSuFile(const string& nomeFile) const {
        ofstream file(nomeFile);
        if (file.is_open()) {
            file << "Intestatario: " << intestatario << "\n";
            for (const auto& t : transazioni) {
                file << t.data << "," << t.tipo << "," << t.importo << "\n";
            }
            file << "Saldo finale: " << calcolaSaldo() << "\n";
            file.close();
        } else {
            cerr << "Errore nell'apertura del file.\n";
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

// -------------------- MAIN INTERATTIVO --------------------

int main() {
    cout << "Benvenuto nel sistema bancario!\n";
    cout << "Inserisci il tuo nome completo: ";
    string nome;
    getline(cin, nome);

    cout << "Che tipo di conto vuoi aprire?\n";
    cout << "[1] Conto Risparmio\n";
    cout << "[2] Conto Deposito\n";
    cout << "[5] Annulla\n";
    int scelta;
    cin >> scelta;

    if (scelta == 5) {
        cout << "Operazione annullata. Grazie e arrivederci!\n";
        return 0;
    }

    ContoCorrente* conto = nullptr;

    if (scelta == 1) {
        conto = new ContoRisparmio(nome);
    } else if (scelta == 2) {
        conto = new ContoDeposito(nome, 0.05); // 5% di interesse
    } else {
        cout << "Scelta non valida.\n";
        return 1;
    }

    int opzione = 0;
    do {
        cout << "\nScegli un'operazione:\n";
        cout << "[1] Aggiungi transazione\n";
        cout << "[2] Stampa estratto conto\n";
        cout << "[3] Salva su file\n";
        cout << "[0] Esci\n";
        cin >> opzione;

        if (opzione == 1) {
            string tipo, data;
            double importo;
            cout << "Tipo di transazione (entrata/uscita): ";
            cin >> tipo;
            cout << "Importo: ";
            cin >> importo;
            cout << "Data (es. 2025-06-20): ";
            cin >> data;

            conto->aggiungiTransazione(Transazione(tipo, importo, data));
        } else if (opzione == 2) {
            conto->stampaEstrattoConto();
        } else if (opzione == 3) {
            conto->salvaSuFile("estratto_conto.txt");
            cout << "Dati salvati nel file 'estratto_conto.txt'\n";
        }
    } while (opzione != 0);

    delete conto;

    cout << "Grazie per aver usato il nostro sistema!\n";
    return 0;
}
