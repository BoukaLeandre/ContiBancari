#include <gtest/gtest.h>
#include "../Test/Conti.h"

TEST(ContoTest, SaldoCorrettoContoDeposito) {
    ContoDeposito conto("Mario Rossi", 0.05);
    conto.aggiungiTransazione(Transazione("entrata", 500.0, "20-06-2025"));
    conto.aggiungiTransazione(Transazione("uscita", 200.0, "21-06-2025"));
    conto.applicaInteresse();

    EXPECT_DOUBLE_EQ(conto.calcolaSaldo(), 315.0);
}

TEST(ContoTest, SaldoContoRisparmio) {
    ContoRisparmio conto("Giulia Bianchi");
    conto.aggiungiTransazione(Transazione("entrata", 300.0, "10-07-2025"));
    conto.aggiungiTransazione(Transazione("uscita", 100.0, "11-07-2025"));

    EXPECT_DOUBLE_EQ(conto.calcolaSaldo(), 200.0);
}

TEST(ContoTest, TransazioneImportoNonValido) {
    ContoRisparmio conto("Invalido");


    conto.aggiungiTransazione(Transazione("entrata", -50.0, "01-01-2025"));


    EXPECT_DOUBLE_EQ(conto.calcolaSaldo(), 0.0);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
