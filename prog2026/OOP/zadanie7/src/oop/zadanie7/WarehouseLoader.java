package oop.zadanie7;

import java.io.*;
import java.time.LocalDate;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.List;

public class WarehouseLoader {

    public List<Product> loadProducts(String filename) throws InvalidProductFormatException{
        List<Product> products = new ArrayList<>();
        try (BufferedReader reader = new BufferedReader(new FileReader(filename))) {
            String line;

            while ((line = reader.readLine()) != null) {
                String[] parts = line.split(",");

                if (parts.length != 5) {
                    throw new InvalidProductFormatException("Nesprávny formát riadku (očakáva sa 5 stĺpcov): " + line);
                }

                String type = parts[0].trim();
                String id = parts[1].trim();
                String name = parts[2].trim();
                double price;

                try {
                    price = Double.parseDouble(parts[3].trim());
                } catch (NumberFormatException e) {
                    throw new InvalidProductFormatException("Nesprávny formát ceny v riadku: " + line);
                }

                if (type.equals("E")) {
                    try {
                        int warrantyMonths = Integer.parseInt(parts[4].trim());
                        products.add(new ElectronicProduct(id, name, price, warrantyMonths));
                    } catch (NumberFormatException e) {
                        throw new InvalidProductFormatException("Nesprávny formát mesiacov záruky: " + line);
                    }
                } else if (type.equals("P")) {
                    try {
                        LocalDate expirationDate = LocalDate.parse(parts[4].trim());
                        products.add(new PerishableProduct(id, name, price, expirationDate));
                    } catch (Exception e) {
                        throw new InvalidProductFormatException("Nesprávny formát dátumu (očakáva sa YYYY-MM-DD): " + line);
                    }
                } else {
                    throw new InvalidProductFormatException("Neznámy typ produktu: " + type);
                }
            }
        } catch (IOException e) {
            System.err.println("Chyba pri čítaní súboru: " + e.getMessage());
        }


        products.sort(Comparator.comparing(Product::getId));

        return products;
    }

    public void saveProducts(List<Product> products, String filename) throws InvalidProductFormatException{
        try (PrintWriter writer = new PrintWriter(new FileWriter(filename))) {

            for (Product p : products) {
                if (p instanceof ElectronicProduct) {
                    ElectronicProduct ep = (ElectronicProduct) p;
                    writer.println("E," + ep.getId() + "," + ep.getName() + "," + ep.getPrice() + "," + ep.getWarrantyMonths());
                } else if (p instanceof PerishableProduct) {
                    PerishableProduct pp = (PerishableProduct) p;
                    writer.println("P," + pp.getId() + "," + pp.getName() + "," + pp.getPrice() + "," + pp.getExpirationDate());
                }
            }

        } catch (IOException e) {
            System.err.println("Chyba pri čítaní súboru: " + e.getMessage());
        }
    }
}
