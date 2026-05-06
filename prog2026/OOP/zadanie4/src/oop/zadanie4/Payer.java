package oop.zadanie4;

public class Payer {
    private static int NOT_POSITIVE_PAYMENTS = -1;
    private static int NOT_ENOUGH_MONEY = -2;

    public static int payByWallet(Wallet wallet, int price) {
        try {
            wallet.pay(price);
        } catch (NotEnoughtMoneyException e) {
            return NOT_ENOUGH_MONEY;
        }  catch (NotPositivePaymentException e) {
            return NOT_POSITIVE_PAYMENTS;
        }
        
        return wallet.getCache();
    }
}
