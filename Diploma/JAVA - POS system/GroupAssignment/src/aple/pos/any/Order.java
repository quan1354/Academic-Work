package aple.pos.any;

import aple.pos.enums.Empty;
import aple.pos.productgroup.LaptopGroup;
import aple.pos.productgroup.PCGroup;
import aple.pos.productgroup.SmartphoneGroup;
import aple.pos.productgroup.TabletGroup;
import java.util.Scanner;

public final class Order {
    private static int makeCount = 0;
    private final int id = makeCount++;
    private ProductGroup[] productGroups;
    private boolean isPaid = false;
    
    public Order(final ProcessBuilder clsProcessBuilder, final Scanner input) {
        init(clsProcessBuilder, input);
    }
    
    void update(final int index, final Product product) {
        productGroups[index].add(product);
    }
    
    public ProductGroup[] getProductGroups() {
        return productGroups;
    }
    
    public void init(final ProcessBuilder clsProcessBuilder, final Scanner input) {
        productGroups = new ProductGroup[]{
            new SmartphoneGroup(Empty.V, clsProcessBuilder, input),
            new TabletGroup(Empty.V, clsProcessBuilder, input),
            new LaptopGroup(Empty.V, clsProcessBuilder, input),
            new PCGroup(Empty.V, clsProcessBuilder, input)
        };
        isPaid = false;
    }
    
    public boolean isPaid() {
        return isPaid;
    }
    
    public void setPaid(final boolean isPaid) {
        this.isPaid = isPaid;
    }
}