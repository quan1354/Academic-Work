package aple.pos.productgroup;

import aple.pos.any.ProductGroup;
import aple.pos.enums.Empty;
import aple.pos.product.Laptop;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Scanner;

public final class LaptopGroup extends ProductGroup {
    public LaptopGroup(final ProcessBuilder clsProcessBuilder, final Scanner input) {
        super(new ArrayList<>() {{
            add(new Laptop("Aple Renobook T3", 10, 1000.00f, 1899.00f));
            add(new Laptop("Aple Renobook S7", 10, 1500.00f, 2499.00f));
            add(new Laptop("Aple Renobook X11", 10, 2000.00f, 3690.00f));
            add(new Laptop("Aple Renobook SX150", 10, 3000.00f, 5290.00f));
            add(new Laptop("Aple Thunderbook S17", 10, 2500.00f, 4188.00f));
            add(new Laptop("Aple Blizzardstorm X99", 10, 4000.00f, 6280.00f));
            add(new Laptop("Aple Lightforce SX750", 10, 6000.00f, 9500.00f));
            add(new Laptop("Aple Darkforce SX850 Plus", 10, 8000.00f, 12500.00f));
        }}, clsProcessBuilder, input);
    }
    
    public LaptopGroup(final Empty empty, final ProcessBuilder clsProcessBuilder, final Scanner input) {
        super(clsProcessBuilder, input);
    }
    
    @Override protected void showGalleryMenu() throws IOException, InterruptedException {
        clsProcessBuilder.start().waitFor();
        System.out.println("Laptop Gallery Menu");
        for (int i = 0; i != products.size(); ++i) products.get(i).showMenuItem(i + 1);
        System.out.printf("%d. Back\n", products.size() + 1);
        System.out.print("Enter a choice > ");
    }
}