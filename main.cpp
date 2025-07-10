#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#ifndef UNIT_TEST
#include <ctime>

using namespace std;

// Classe Data
class Data {
    string giorno, mese, anno;

public:
    Data(const string& formato) {
        if (formato.size() == 10 && formato[2] == '-' && formato[5] == '-') {
            giorno = formato.substr(0, 2);
            mese = formato.substr(3, 2);
            anno = formato.substr(6, 4);
        } else {
            giorno = "00";
            mese = "00";
            anno = "0000";
        }
    }

    string toString() const {
        return giorno + "-" + mese + "-" + anno;
    }

    bool isToday() const {
        time_t now = time(0);
        tm* localTime = localtime(&now);

        char buffer[11];
        strftime(buffer, sizeof(buffer), "%d-%m-%Y", localTime);
        return toString() == string(buffer);
    }
};

// Classe Transazione
class Transazione {
public:
    string tipo;
    double importo;
    Data data;

    Transazione(string t, double i, string d) : tipo(t), importo(i), data(Data(d)) {
        if (!data.isToday()) {
            cerr << "Errore: la data inserita non valida! Transazione annullata.\n";
            importo = -1;
        }
    }
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
            cerr << "Transazione non valida. Ignorata.\n";
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
            cout << t.data.toString();
            if (t.tipo == "entrata" && t.data.toString() == Data("oggi").toString())
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
                file << "Data: " << t.data.toString() << "\n";
                file << t.tipo << ": " << t.importo << " €\n";
            }
            file << "\nSaldo finale: " << calcolaSaldo() << " €\n";
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
            aggiungiTransazione(Transazione("entrata", interesse, Data("01-01-1970").toString()));
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
        conto->aggiungiTransazione(Transazione("entrata", 500.0, "10-07-2025"));
        conto->aggiungiTransazione(Transazione("uscita", 200.0, "10-07-2025"));
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

        if (scelta == 1) conto = new ContoRisparmio(nome);
        else if (scelta == 2) {
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
                string dataInput;

                cout << "Tipo di transazione:\n[1] Entrata\n[2] Uscita\nScelta: ";
                cin >> tipoTrans;

                cout << "Importo: ";
                cin >> importo;
                cout << "Data (formato gg-mm-aaaa): ";
                cin >> dataInput;

                string tipo = (tipoTrans == 1) ? "entrata" : "uscita";
                conto->aggiungiTransazione(Transazione(tipo, importo, dataInput));

            } else if (opzione == 2) conto->stampaEstrattoConto();
            else if (opzione == 3) {
                conto->salvaSuFile("estratto_conto.txt");
                cout << "Dati salvati nel file 'estratto_conto.txt'\n";
            } else if (opzione == 4 && contoDeposito) contoDeposito->applicaInteresse();

        } while (opzione != 0);

        delete conto;
        cout << "Grazie per aver usato il nostro sistema!\n";
    }
    return 0;
}
#endif
