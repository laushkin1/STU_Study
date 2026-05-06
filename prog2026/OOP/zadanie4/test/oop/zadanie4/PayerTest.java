package oop.zadanie4;

import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.*;

class PayerTest {
    @Test
    void paymentSuccessful() {
        Wallet w = new Wallet(120);
        int result = Payer.payByWallet(w, 100);
        assertEquals(20, result);
    }

    @Test
    void negativePaymentsException() {
        Wallet w = new Wallet(100);
        int result = Payer.payByWallet(w, -10);
        assertEquals(-1, result);
    }

    @Test
    void notEnoughMoneyException() {
        Wallet w = new Wallet(100);
        int result = Payer.payByWallet(w, 200);
        assertEquals(-2, result);
    }
}
