package oop.zadanie6;

public class PerishableProduct extends Product {
    private int expirationDays;

    public PerishableProduct(String id, String name, double price, int expirationDays) {
        super(id, name, price);
        this.expirationDays = expirationDays;
    }

    public int getExpirationDays() { return expirationDays; }
    public void setExpirationDays(int expirationDays) { this.expirationDays = expirationDays; }

    @Override
    public String toString() {
        return "This is a perishable product " + getId() + " with name " + getName() + " and price " + getPrice() + " and expiration in " + expirationDays + " days";
    }
}
