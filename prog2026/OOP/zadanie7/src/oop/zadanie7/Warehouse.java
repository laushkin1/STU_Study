package oop.zadanie7;

import java.util.*;

public class Warehouse {
    private Map<String, Product> products;

    public Warehouse() {
        this.products = new HashMap<>();
    }

    public void addProduct(Product p) throws ProductAlreadyExistsException {
        if (products.containsKey(p.getId())){
            throw new ProductAlreadyExistsException("Product s ID '" + p.getId() + "' sa v sklade už nachádza.");
        } else {
            products.put(p.getId(), p);
        }
    }

    public void removeProduct(String id) throws ProductNotFoundException {
        if (products.containsKey(id)){
            products.remove(id);
        }
        else {
            throw new ProductNotFoundException("Product s ID '" + id + "' sa v sklade nenachádza.");
        }
    }

    public Product getProduct(String id) throws ProductNotFoundException {
        return products.get(id);
    }

    public int getProductCount() {
        return products.size();
    }

    public List<Product> findProductsByPriceRange(double minPrice, double maxPrice){
        List<Product> result = new ArrayList<>();
        for (Map.Entry<String, Product> entry : products.entrySet()){
            if (entry.getValue().getPrice() >= minPrice && entry.getValue().getPrice() <= maxPrice){
                result.add(entry.getValue());
            }
        }
        return result;
    }

    public List<Product> getProductsSortedByPrice(){
        List<Product> result = new ArrayList<>(products.values());
        result.sort(Comparator.comparingDouble(Product::getPrice));
        return result;
    }

    public List<Product> getProductsSortedById(){
        List<Product> result = new ArrayList<>(products.values());
        result.sort(Comparator.comparing(Product::getId));
        return result;
    }

    public <T> List<T> getProductsByType(Class<T> type){
        List<T> result = new ArrayList<>();
        for (Product product : products.values()) {
            if (type.isInstance(product)) {
                result.add(type.cast(product));
            }
        }
        return result;
    }

    public Product findProductWithSmallestId() {
        if (products.isEmpty()) {
            return null;
        }

        Product smallestProduct = null;

        for (Product currentProduct : products.values()) {
            if (smallestProduct == null) {
                smallestProduct = currentProduct;
            }
            else if (currentProduct.compareTo(smallestProduct) < 0) {
                smallestProduct = currentProduct;
            }
        }

        return smallestProduct;
    }

    public double calculateTotalValue() {
        double totalValue = 0.0;
        for (Product product : products.values()) {
            totalValue += product.getPrice();
        }
        return totalValue;
    }
}
