package aple.pos.product;

import aple.pos.any.Product;

public class Laptop extends Product {
    public Laptop() {
        this("",0,0.0f,0.0f);
    }

    public Laptop(String name, int qty, float cost, float price) {
        super(name, qty, cost, price);
    }

    @Override
    public int hashCode() {
        int hash = super.hashCode();
        return hash;
    }
    
    @Override
    public boolean equals(Object o) {
        if (!(o instanceof Laptop))
            return false;
        Laptop temp = (Laptop) o;
        if (id != temp.id)
            return false;
        return getName().equals(temp.getName()) && getCost() == temp.getCost() && getPrice() == temp.getPrice();
    }

    @Override
    public String toString() {
        return "Laptop{" +
                "name='" + this.getName() + '\'' +
                ", quantity=" + this.getQuantity() +
                ", cost=" + this.getCost() +
                ", price=" + this.getPrice() +
                '}';
    }

    @Override
    public void print() {
        System.out.println("Laptop Group");
        System.out.printf("Name: %s\n",this.getName());
        System.out.printf("quantity: %s\n",this.getQuantity());
        System.out.printf("cost:%.2f \n",this.getCost());
        System.out.printf("price: %s\n",this.getPrice());
        System.out.println("-----------------------------");
    }
}