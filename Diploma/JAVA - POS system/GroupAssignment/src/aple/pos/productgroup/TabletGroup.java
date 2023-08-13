package aple.pos.productgroup;

import aple.pos.any.Product;
import aple.pos.any.ProductGroup;
import aple.pos.enums.Empty;
import aple.pos.product.Tablet;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Scanner;

public final class TabletGroup extends ProductGroup {
    public TabletGroup(final ProcessBuilder clsProcessBuilder, final Scanner input) {
        super(new ArrayList<Product>() {{
            add(new Tablet("Aple Pyro 1T", 10, 250.00f, 490.00f));
            add(new Tablet("Aple Pyro 1", 10, 300.00f, 599.00f));
            add(new Tablet("Aple Pyro 2T", 10, 450.00f, 789.00f));
            add(new Tablet("Aple Pyro 2", 10, 600.00f, 968.00f));
            add(new Tablet("Aple Pyro 3", 10, 700.00f, 1298.00f));
            add(new Tablet("Aple Pyro 3 Plus", 10, 950.00f, 1599.00f));
            add(new Tablet("Aple Pyro 4", 10, 1200.00f, 1899.00f));
            add(new Tablet("Aple Pyro 4 Plus", 10, 1500.00f, 2349.00f));
        }}, clsProcessBuilder, input);
    }
    
    public TabletGroup(final Empty empty, final ProcessBuilder clsProcessBuilder, final Scanner input) {
        super(clsProcessBuilder, input);
    }
    
    @Override protected void showGalleryMenu() throws IOException, InterruptedException {
        clsProcessBuilder.start().waitFor();
        System.out.println("Tablet Gallery Menu");
        for (int i = 0; i != products.size(); ++i) products.get(i).showMenuItem(i + 1);
        System.out.printf("%d. Back\n", products.size() + 1);
        System.out.print("Enter a choice > ");
    }
}