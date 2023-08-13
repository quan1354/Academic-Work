package aple.pos.productgroup;

import aple.pos.any.ProductGroup;
import aple.pos.enums.Empty;
import aple.pos.product.SmartPhone;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Scanner;

public final class SmartphoneGroup extends ProductGroup {
    public SmartphoneGroup(final ProcessBuilder clsProcessBuilder, final Scanner input) {
        super(new ArrayList<>() {{
            add(new SmartPhone("Aple Luno 1S", 10, 200.00f, 299.00f));
            add(new SmartPhone("Aple Luno 1", 10, 250.00f, 349.00f));
            add(new SmartPhone("Aple Luno 2S", 10, 300.00f, 398.00f));
            add(new SmartPhone("Aple Luno 2", 10, 350.00f, 499.00f));
            add(new SmartPhone("Aple Luno 3", 10, 400.00f, 698.00f));
            add(new SmartPhone("Aple Luno 3X", 10, 500.00f, 890.00f));
            add(new SmartPhone("Aple Luno 4", 10, 750.00f, 1049.00f));
            add(new SmartPhone("Aple Luno 4X", 10, 1000.00f, 1698.00f));
        }}, clsProcessBuilder, input);
    }
    
    public SmartphoneGroup(final Empty empty, final ProcessBuilder clsProcessBuilder, final Scanner input) {
        super(clsProcessBuilder, input);
    }
    
    @Override protected void showGalleryMenu() throws IOException, InterruptedException {
        clsProcessBuilder.start().waitFor();
        System.out.println("Smartphone Gallery Menu");
        for (int i = 0; i != products.size(); ++i) products.get(i).showMenuItem(i + 1);
        System.out.printf("%d. Back\n", products.size() + 1);
        System.out.print("Enter a choice > ");
    }
}