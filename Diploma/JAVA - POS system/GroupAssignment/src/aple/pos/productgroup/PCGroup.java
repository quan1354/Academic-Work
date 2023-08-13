package aple.pos.productgroup;

import aple.pos.any.ProductGroup;
import aple.pos.enums.Empty;
import aple.pos.product.PC;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Scanner;

public final class PCGroup extends ProductGroup {
    public PCGroup(final ProcessBuilder clsProcessBuilder, final Scanner input) {
        super(new ArrayList<>() {{
            add(new PC("Aple Galaxy S2 Mini", 10, 900.00f, 1598.00f));
            add(new PC("Aple Galaxy S3", 10, 1200.00f, 2059.00f));
            add(new PC("Aple Galaxy S4", 10, 1800.00f, 3099.00f));
            add(new PC("Aple Galaxy S5 Plus", 10, 2300.00f, 4149.00f));
            add(new PC("Aple Heatadder Z79", 10, 2000.00f, 4099.00f));
            add(new PC("Aple Windadder Z89", 10, 3000.00f, 5390.00f));
            add(new PC("Aple Southwind Z99", 10, 5000.00f, 7890.00f));
            add(new PC("Aple Northwind ZX999", 10, 7500.00f, 9999.00f));
        }}, clsProcessBuilder, input);
    }
    
    public PCGroup(final Empty empty, final ProcessBuilder clsProcessBuilder, final Scanner input) {
        super(clsProcessBuilder, input);
    }
    
    @Override protected void showGalleryMenu() throws IOException, InterruptedException {
        clsProcessBuilder.start().waitFor();
        System.out.println("PC Gallery Menu");
        for (int i = 0; i != products.size(); ++i) products.get(i).showMenuItem(i + 1);
        System.out.printf("%d. Back\n", products.size() + 1);
        System.out.print("Enter a choice > ");
    }
}