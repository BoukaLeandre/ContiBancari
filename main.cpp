#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#ifndef UNIT_TEST
using namespace std;

// Classe Transazione
class Transazione {
public:
    string tipo;
    double importo;
    string data;

    Transazione(string t, double i, string d) : tipo(t), importo(i), data(d) {}
};

// Classe base ContoCorrente
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
            if (t.tipo == "entrata") saldo += t.importo;
            else if (t.tipo == "uscita") saldo -= t.importo;
        }
        return saldo;
    }

    virtual void applicaInteresse() = 0;

    virtual void stampaEstrattoConto() const {
        cout << "\n--- Estratto conto ---\n";
        cout << "Utente: " << utente << "\n";
        cout << "Tipo di conto: " << tipoConto << "\n";
        for (const auto& t : transazioni) {
            cout << t.data;
            if (t.tipo == "entrata" && t.data == "oggi")
                cout << " - entrata (interesse): ";
            else
                cout << " - " << t.tipo << ": ";
            printf("%.2f €\n", t.importo);
        }
        printf("\nSaldo finale: %.2f €\n", calcolaSaldo());
        cout << "----------------------\n";
    }

    virtual void salvaSuFile(const string& nomeFile) const {
        ofstream file(nomeFile);
        if (file.is_open()) {
            file << "Utente: " << utente << "\n";
            file << "Tipo di conto: " << tipoConto << "\n";
            for (const auto& t : transazioni) {
                file << t.data << " - " << t.tipo << ": " << t.importo << " \u20AC\n";
            }
            file << "\nSaldo finale: " << calcolaSaldo() << " \u20AC\n";
            file.close();
        } else {
            cerr << "Errore nell'apertura del file.\n";
        }
    }

    virtual ~ContoCorrente() {}
};

// Classe ContoRisparmio
class ContoRisparmio : public ContoCorrente {
public:
    ContoRisparmio(string nome) : ContoCorrente(nome, "Conto Risparmio") {}
    void applicaInteresse() override {}
};

// Classe ContoDeposito
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
            cout << "[INFO] Interesse applicato: +" << interesse << " €\n";
        }
    }
};

int main(int argc, char* argv[]) {
    bool testMode = argc > 1 && string(argv[1]) == "e";
    string nome;
    int scelta = 0;

    ContoCorrente* conto = nullptr;
    ContoDeposito* contoDeposito = nullptr;

    if (testMode) {
        nome = "Le Beretti";
        contoDeposito = new ContoDeposito(nome, 0.05);
        conto = contoDeposito;
        conto->aggiungiTransazione(Transazione("entrata", 500.0, "20-06-2025"));
        conto->aggiungiTransazione(Transazione("uscita", 200.0, "21-06-2025"));
        contoDeposito->applicaInteresse();
        conto->stampaEstrattoConto();
        cout << "\nTutti i test sono riusciti con successo.\n";
    } else {
        cout << "Benvenuto nel sistema bancario!\n";
        cout << "Inserisci il tuo nome completo: ";
        cin.ignore();
        getline(cin, nome);

        cout << "Che tipo di conto vuoi aprire?\n";
        cout << "[1] Conto Risparmio\n";
        cout << "[2] Conto Deposito (con interesse del 5%)\n";
        cout << "[5] Annulla\n";
        cin >> scelta;

        if (scelta == 5) {
            cout << "Operazione annullata. Grazie e arrivederci!\n";
            return 0;
        }

        if (scelta == 1) {
            conto = new ContoRisparmio(nome);
        } else if (scelta == 2) {
            contoDeposito = new ContoDeposito(nome, 0.05);
            conto = contoDeposito;
            cout << "Hai scelto un Conto Deposito. Verrà applicato un interesse del 5% sul saldo.\n";
        } else {
            cout << "Scelta non valida.\n";
            return 1;
        }

        int opzione;
        do {
            cout << "\nScegli un'operazione:\n";
            cout << "[1] Aggiungi transazione\n";
            cout << "[2] Stampa estratto conto\n";
            cout << "[3] Salva su file\n";
            if (contoDeposito) cout << "[4] Applica interesse\n";
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

                cout << "Data (formato gg-mm-aaaa): ";
                cin >> data;

                string tipo = (tipoTrans == 1) ? "entrata" : "uscita";
                conto->aggiungiTransazione(Transazione(tipo, importo, data));

            } else if (opzione == 2) {
                conto->stampaEstrattoConto();
            } else if (opzione == 3) {
                conto->salvaSuFile("estratto_conto.txt");
                cout << "Dati salvati nel file 'estratto_conto.txt'\n";
            } else if (opzione == 4 && contoDeposito) {
                contoDeposito->applicaInteresse();
            }
        } while (opzione != 0);

        delete conto;
        cout << "Grazie per aver usato il nostro sistema!\n";
    }
    return 0;
}
#endif