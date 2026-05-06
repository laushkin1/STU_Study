package oop.zadanie6;

public class ElectronicProduct extends Product implements Discountable{
    private int warrantyMonths;

    public ElectronicProduct(String id, String name, double price, int warrantyMonths) {
        super(id, name, price);
        this.warrantyMonths = warrantyMonths;
    }

    public int getWarrantyMonths() { return warrantyMonths; }
    public void setWarrantyMonths(int warrantyMonths) { this.warrantyMonths = warrantyMonths; }

    @Override
    public void applyDiscount(double percentage) {
        if (0.0 < percentage && percentage <= 100.0) {
            double new_price = getPrice() * (1.0 - percentage/100.0);
            setPrice(new_price);
        }
    }

    @Override
    public String toString() {
        return "This is a electronic product " + getId() + " with name " + getName() + " and price " + getPrice() + " and " + warrantyMonths + " month warranty";
    }
}
