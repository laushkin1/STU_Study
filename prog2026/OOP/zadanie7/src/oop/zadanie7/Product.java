package oop.zadanie7;

import java.util.Objects;

public abstract class Product implements Comparable<Product> {
    private String id;
    private String name;
    private double price;

    public Product(String id, String name, double price) throws IllegalArgumentException {
        this.id = id;
        this.name = name;
        if (price < 0){
            throw new IllegalArgumentException("Price cannot be negative");
        }
        this.price = price;
    }

    public String getId() {
        return id;
    }

    public String getName() {
        return name;
    }

    public double getPrice() {
        return price;
    }

    public void setPrice(double price) {
        this.price = price;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Product product = (Product) o;
        return Objects.equals(id, product.id);
    }

    @Override
    public int hashCode() {
        return Objects.hash(id);
    }

    @Override
    public int compareTo(Product o) {
        return this.id.compareTo(o.id);
    }

    @Override
    public String toString() {
        return "Product{" +
                "id='" + id + '\'' +
                ", name='" + name + '\'' +
                ", price=" + price +
                '}';
    }
}
