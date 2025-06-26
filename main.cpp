#include <iostream>
#include <vector>
#include <string>
#include <fstream>

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
            file << "Intestatario: " << utente << "\n";
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

    virtual ~ContoCorrente() {}
};

class ContoRisparmio : public ContoCorrente {
public:
    ContoRisparmio(string nome) : ContoCorrente(nome, "Conto Risparmio") {}
};

class ContoDeposito : public ContoCorrente {
private:
    double interessePercentuale;

public:
    ContoDeposito(string nome, double interesse)
        : ContoCorrente(nome, "Conto Deposito"), interessePercentuale(interesse) {}

    void applicaInteresse() {
        double saldoAttuale = calcolaSaldo();
        double interesse = saldoAttuale * interessePercentuale;
        aggiungiTransazione(Transazione("entrata", interesse, "oggi"));
    }
};

int main(int argc, char* argv[]) {
    bool testMode = argc > 1 && string(argv[1]) == "e";

    string nome;
    int scelta = 0;

    if (testMode) {
        nome = "Le Beretti";
        scelta = 1;
    } else {
        cout << "Benvenuto nel sistema bancario!\n";
        cout << "Inserisci il tuo nome completo: ";
        getline(cin, nome);

        cout << "Che tipo di conto vuoi aprire?\n";
        cout << "[1] Conto Risparmio\n";
        cout << "[2] Conto Deposito\n";
        cout << "[5] Annulla\n";
        cin >> scelta;

        if (scelta == 5) {
            cout << "Operazione annullata. Grazie e arrivederci!\n";
            return 0;
        }
    }

    ContoCorrente* conto = nullptr;

    if (scelta == 1) {
        conto = new ContoRisparmio(nome);
    } else if (scelta == 2) {
        conto = new ContoDeposito(nome, 0.05);
    } else {
        cout << "Scelta non valida.\n";
        return 1;
    }

    if (testMode) {
        conto->aggiungiTransazione(Transazione("entrata", 500.0, "20-06-2025"));
        conto->aggiungiTransazione(Transazione("uscita", 200.0, "21-06-2025"));
        conto->stampaEstrattoConto();
        conto->salvaSuFile("estratto_conto_test.txt");
    } else {
        int opzione = 0;
        do {
            cout << "\nScegli un'operazione:\n";
            cout << "[1] Aggiungi transazione\n";
            cout << "[2] Stampa estratto conto\n";
            cout << "[3] Salva su file\n";
            cout << "[0] Esci\n";
            cin >> opzione;

            if (opzione == 1) {
                int tipoTrans;
                double importo;
                string data;

                cout << "Tipo di transazione:\n";
                cout << "[1] Entrata\n";
                cout << "[2] Uscita\n";
                cout << "Scelta: ";
                cin >> tipoTrans;

                if (tipoTrans != 1 && tipoTrans != 2) {
                    cout << "Scelta non valida!\n";
                    continue;
                }

                cout << "Importo: ";
                cin >> importo;

                if (importo <= 0) {
                    cout << "L'importo deve essere maggiore di zero!\n";
                    continue;
                }

                cout << "Data (formato gg-mm-aaaa, es. 20-06-2025): ";
                cin >> data;

                string tipo = (tipoTrans == 1) ? "entrata" : "uscita";
                conto->aggiungiTransazione(Transazione(tipo, importo, data));
            } else if (opzione == 2) {
                conto->stampaEstrattoConto();
            } else if (opzione == 3) {
                conto->salvaSuFile("estratto_conto.txt");
                cout << "Dati salvati nel file 'estratto_conto.txt'\n";
            }
        } while (opzione != 0);
    }

    delete conto;
    cout << "Grazie per aver usato il nostro sistema!\n";
    return 0;
}
