package aple.pos.any;

import aple.pos.stock.Printable;
import java.util.Objects;

public class Product implements Printable {
    protected final int id;
    private String name;
    private int quantity;
    private float price;
    private float cost;
    private int salesQuantity = 0;
    
    public Product() {
        this("", 0, 0.0f, 0.0f);
    }
    
    public Product(final String name, final int quantity, final float cost, final float price) {
        this.name = name;
        this.quantity = quantity;
        this.price = price;
        this.cost = cost;
        id = hashCode();
    }
    
    public Product(final Product stockItem, final int quantity) {
        this(stockItem.name, quantity, stockItem.price, stockItem.cost);
    }
    
    public String getName() {
        return name;
    }
    
    public void setName(final String name) {
        this.name = name;
    }
    
    public int getQuantity() {
        return quantity;
    }
    
    public void setQuantity(final int quantity) {
        this.quantity = quantity;
    }
    
    public float getCost() {
        return cost;
    }
    public void setCost(final float cost) {
        this.cost = cost;
    }
    public float getPrice() {
        return price;
    }
    public void setPrice(final float price) {
        this.price = price;
    }
    int getSalesQuantity() {
        return salesQuantity;
    }
    void addSalesQuantity(final int salesQuantity) {
        this.salesQuantity += salesQuantity;
    }
    public void increaseQuantity(final int quantity) {
        this.quantity += quantity;
    }
    
    /**
     * @param quantity it must be positive
     * @return true if success, false otherwise
     */
    public boolean decreaseQuantity(final int quantity) {
        if (quantity > this.quantity) return false;
        this.quantity -= quantity;
        return true;
    }
    
    public void showMenuItem(final int index) {
        System.out.printf("%d. %s\n", index, name);
    }

    @Override public int hashCode() {
        int hash = 7;
        hash = 97 * hash + Objects.hashCode(this.name);
        hash = 97 * hash + Float.floatToIntBits(this.price);
        hash = 97 * hash + Float.floatToIntBits(this.cost);
        return hash;
    }
    
    @Override public boolean equals(final Object obj) {
        if(!(obj instanceof Product)) return false;
        Product temp = (Product)obj;
        if (id != temp.id) return false;
        return name.equals(temp.name) && cost == temp.cost && price == temp.price;
    }
    
    @Override public String toString() {
        return "Product{" +
                "name='" + name + '\'' +
                ", quantity=" + quantity +
                ", cost=" + cost +
                ", price=" + price +
                '}';
    }

    @Override public void print() {
        System.out.printf("Name: %s\n",name);
        System.out.printf("quantity: %s\n",quantity);
        System.out.printf("cost:%.2f \n",cost);
        System.out.printf("price: %s\n",price);
        System.out.println("-----------------------------");
    }
}