package aple.pos.any;

import aple.pos.enums.Action;
import static aple.pos.enums.Action.BACK;
import static aple.pos.enums.Action.GO_TO_PAYMENT;
import static aple.pos.enums.Action.NONE;
import java.io.IOException;
import static java.lang.System.console;
import java.util.ArrayList;
import java.util.InputMismatchException;
import java.util.Scanner;

public abstract class ProductGroup {
    protected final ArrayList<Product> products;
    private int productCount;
    protected final ProcessBuilder clsProcessBuilder;
    private final Scanner input;
    private final ArrayList<Integer> oldQuantityInputs;
    
    private ProductGroup(final ArrayList<Product> products, final ProcessBuilder clsProcessBuilder, final Scanner input,
        final ArrayList<Integer> oldQuantityInputs) {
        this.products = products;
        productCount = products.size();
        this.clsProcessBuilder = clsProcessBuilder;
        this.input = input;
        this.oldQuantityInputs = oldQuantityInputs;
    }
    
    protected ProductGroup(final ArrayList<Product> products, final ProcessBuilder clsProcessBuilder, final Scanner input) {
        this(products, clsProcessBuilder, input, new ArrayList<>(){{
            for (int i = 0; i != products.size(); ++i) add(0);
        }});
    }
    
    protected ProductGroup(final ProcessBuilder clsProcessBuilder, final Scanner input) {
        this(new ArrayList<>(), clsProcessBuilder, input, null);
        productCount = 0;
    }
    
    public final ArrayList<Product> getProducts() {
        return products;
    }
    
    public final void add(final Product product) {
        for (int i = 0; i != productCount; ++i) {
            if (!products.get(i).getName().equals(product.getName())) continue;
            products.set(i, product);
            return;
        }
        products.add(product);
        ++productCount;
    }
    
    final Action goToGalleryMenu(final int index, final Order order, final Scanner input) throws IOException, InterruptedException {
        while (true) {
            showGalleryMenu();
            try {
                switch (reactGalleryMenuChoice(index, order, input.nextInt())) {
                    case BACK: return NONE;
                    case GO_TO_PAYMENT: return GO_TO_PAYMENT;
                }
            } catch (final InputMismatchException err) {
                System.out.println();
                System.out.println("Invalid choice! Please try again. Press enter key to continue.");
                input.nextLine();
                console().readPassword(); // to receive input without echo
            }
        }
    }
    
    protected abstract void showGalleryMenu() throws IOException, InterruptedException;
    
    final Action reactGalleryMenuChoice(final int index, final Order order, final int choice) throws IOException, InterruptedException {
        if (choice < 1 || choice > products.size() + 1) throw new InputMismatchException();
        if (choice == products.size() + 1) return BACK;
        System.out.println();
        System.out.print("Enter new quantity to be bought > ");
        try {
            final int quantityInput = input.nextInt();
            if (quantityInput < 0) {
                System.out.println();
                System.out.println("Quantity must be positive! Please try again. Press enter key to continue.");
                console().readPassword(); // to receive input without echo
            } else {
                products.get(choice - 1).increaseQuantity(oldQuantityInputs.get(choice - 1));
                if (!products.get(choice - 1).decreaseQuantity(quantityInput)) {
                    products.get(choice - 1).decreaseQuantity(oldQuantityInputs.get(choice - 1));
                    System.out.println();
                    System.out.println("There are not enough stock available! Please try again. Press enter key to continue.");
                    console().readPassword(); // to receive input without echo
                } else {
                    oldQuantityInputs.set(choice - 1, quantityInput);
                    order.update(index, new Product(products.get(choice - 1), quantityInput));
                    System.out.println();
                    System.out.println("Successfully update the product(s) to the order!");
                    input.nextLine(); // to clear garbage input
                    while (true) {
                        System.out.print("Proceed to Payment? (Y/N) ");
                        switch (input.nextLine().toUpperCase()) {
                            case "Y":
                                return GO_TO_PAYMENT;
                            case "N":
                                return NONE;
                            default:
                                System.out.println("Invalid choice! Please try again.");
                                System.out.println();
                        }
                    }
                }
            }
        } catch (final InputMismatchException err) {
            System.out.println();
            System.out.println("Invalid quantity! Please try again. Press enter key to continue.");
            input.nextLine();
            console().readPassword(); // to receive input without echo
        }
        return NONE;
    }
    
    public final boolean isProductNameExist(final String productName) {
        for (final Product product : products) if (product.getName().equalsIgnoreCase(productName)) return true;
        return false;
    }
    
    public final void undoOrderPart() {
        for (int i = 0; i != products.size(); ++i) {
            products.get(i).increaseQuantity(oldQuantityInputs.get(i));
            oldQuantityInputs.set(i, 0);
        }
    }
    
    public final void commitOrderPart() {
        for (int i = 0; i != products.size(); ++i) {
            products.get(i).addSalesQuantity(oldQuantityInputs.get(i));
            oldQuantityInputs.set(i, 0);
        }
    }
    
    @Override public final String toString() {
        final StringBuilder result = new StringBuilder();
        for (final Product product : products) result.append(product);
        return result.toString();
    }
}