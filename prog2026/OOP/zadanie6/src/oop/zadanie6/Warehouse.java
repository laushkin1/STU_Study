package oop.zadanie6;

import java.util.HashSet;
import java.util.Set;

public class Warehouse {
    private Set<Product> products = new HashSet<>();

    public Warehouse() {}

    public void addProduct(Product product) throws ProductAlreadyExistsException {
        if(!products.add(product)){
            throw new ProductAlreadyExistsException("Product with '" + product.getId() + "' ID already exists");
        }
    }
    public void removeProduct(String id) throws ProductNotFoundException {
        if(!products.removeIf(product -> product.getId().equals(id))){
            throw new ProductNotFoundException("Product with '" + id + "' ID not found");
        }
    }
    public int getProductCount() { return products.size(); }


}
