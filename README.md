# 💼 Programma di Gestione dei Conti Bancari

Questo progetto, scritto in C++, simula un sistema bancario per la gestione di conti correnti e conti di risparmio. È stato realizzato come esercitazione pratica universitaria per comprendere meglio l'uso delle strutture, dei file e dell'interazione con l'utente.

---

## 🧠 Funzionalità principali

- 👤 Creazione di un nuovo conto (Corrente o Risparmio)
- 💾 Salvataggio delle informazioni in un file di testo (`conti_salvati.txt`)
- 🧑‍💻 Interazione dinamica con l’utente tramite terminale
- ❌ Possibilità di annullare l’operazione
- 🔎 Visualizzazione dei conti salvati (opzionale)
- ✅ Gestione della memoria e pulizia dei dati

---

## 📂 Struttura del progetto

- `main.cpp` → codice sorgente principale
- `conti_salvati.txt` → file dove vengono salvati i conti creati
- `CMakeLists.txt` → configurazione per compilare con CMake
- `.github/workflows/test.yml` → workflow per test automatici con GitHub Actions
- `.gitignore` → file per escludere i file inutili dal repository

---
🔍 Esempio di utilizzo

1- Il programma chiede all’utente di scegliere il tipo di conto da creare (1 = Risparmio, 2 = Deposito, 5 = Annulla)

2- L’utente inserisce nome, cognome, saldo iniziale

3- Il conto viene salvato nel file conti_salvati.txt

4- È possibile successivamente aggiungere nuove funzionalità (modifiche, ricerche, ecc.)

## 🔧 Come compilare

### Con G++:


`g++ main.cpp -o conti
./conti`


## 👥 Autori
`Léandre Siddhata Bouka` – leandre.bouka@edu.unifi.it
`Roosvelt Fowada`- roosvelt.fowada@edu.unifi.it

Progetto realizzato per l'Università di Firenze – Corso di Laurea in Informatica.
Corso di Laurea in Informatica – A.A. 2024/2025
