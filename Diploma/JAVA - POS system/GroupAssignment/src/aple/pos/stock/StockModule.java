package aple.pos.stock;
import aple.pos.product.SmartPhone;
import aple.pos.product.Tablet;
import aple.pos.product.Laptop;
import aple.pos.product.PC;
import aple.pos.any.Product;
import aple.pos.any.ProductGroup;
import aple.pos.any.StockTransaction;
import java.io.IOException;
import static java.lang.System.console;
import java.util.ArrayList;
import java.util.Scanner;

public final class StockModule { 
    private final ProductGroup[] productGroups;
    private int addedQuantity;
    private String addedProduct;
    private int qtyChg;
    private final ArrayList<StockTransaction> stockTransactions;
    Scanner sc = new Scanner(System.in);
    
    public StockModule(final ProductGroup[] productGroups, final ArrayList<StockTransaction> stockTransactions) {
        this.productGroups = productGroups;
        this.stockTransactions = stockTransactions;
    }

    /**
     * This method is to print Stock Module menu and get user selection.
     * @param clsProcessBuilder is to print Screen
     * @throws IOException
     * @throws InterruptedException 
     */
    public void processMainMenu(ProcessBuilder clsProcessBuilder) throws IOException, InterruptedException{
        boolean isContinue = true;
        do {
            printStockMenu(clsProcessBuilder);
            int option = InputValidation.getIntegerInput("Enter option:");
            switch (option) {
                case 1:
                    addProduct();
                    break;
                case 2:
                    editProduct();
                    break;
                case 3:
                    deleteProduct();
                    break;
                case 4:
                    displayProduct();
                    break;
                case 5:
                    System.out.println("Returning to main menu ... ");
                    isContinue = false;
                    break;
                default:
                    System.out.println("Invalid option,Please try again");
                    isContinue = true;
            }
        } while (isContinue);
    }
    private void addProduct(){
        boolean repeat = true;
        do{
            try{
                String name = InputValidation.getStringInput("Enter product name :");
                int quantity = InputValidation.getIntegerInput("Enter quantity :");
                addedQuantity = quantity;
                addedProduct = name;
                float cost = InputValidation.getFloatInput("Enter cost :");
                float price = InputValidation.getFloatInput("Enter price :");

                for (final ProductGroup productGroup : productGroups) {
                    if ( productGroup.isProductNameExist(name) ) {
                        throw new AssertionError();
                    }
                    repeat = true;
                }
                printProductGroupAddMenu();
                int type = InputValidation.getIntegerInput("Option :");
                //assign product to category of product
                switch (type) {
                    case 1:
                        productGroups[0].add(new SmartPhone(name,quantity,cost,price));
                        System.out.println("Product added to SmartPhone Successfully !");
                        console().readPassword(); // to receive input without echo
                        break;
                    case 2:
                        productGroups[1].add(new Tablet(name,quantity,cost,price));
                        System.out.println("Product added to Tablet Successfully !");
                        console().readPassword(); // to receive input without echo
                        break;
                    case 3:
                        productGroups[2].add(new Laptop(name,quantity,cost,price));
                        System.out.println("Product added to Laptop Successfully !");
                        console().readPassword(); // to receive input without echo
                        break;
                    case 4:
                        productGroups[3].add(new PC(name,quantity,cost,price));
                        System.out.println("Product added to PC Successfully !");
                        console().readPassword(); // to receive input without echo
                        break;
                    case 5:
                        System.out.println("The product is not created");
                        System.out.println("Returning to Stock Menu, Press a key to continue ...");
                        console().readPassword(); // to receive input without echo                
                        break;
                    default:
                        System.out.println("Invalid input, Please enter again");
                }
                repeat = false;
            }catch(AssertionError e){
                System.out.println("The product is already exist, Please try again");
                repeat = true;
            }catch(Exception e){
                System.out.println("An unexpected Error has occur, Please try again");
                repeat = true;
            }
        }while(repeat);
    }
    private void editProduct()  {
        while (true) {
            printProductGroupEditMenu();
            final int type = InputValidation.getIntegerInput("Option :");
            if (type > 0 && type <= productGroups.length) editSpecificProduct(type - 1);
            else if (type == productGroups.length + 1) return;
            else System.out.println("Invalid input, Please enter again");    
        }
    }
    /**
     * This method is to show product according product group and let user
     * modify details of product.
     * @param index Group of product
     */

    private void editSpecificProduct(final int index ){
        boolean isContinue = true;
        do {
            System.out.println("Index | Product description | Quantity | Cost   | Price ");
            System.out.println("----------------------------------------------");
            for (int i = 0; i < productGroups[index].getProducts().size(); ++i) {
                System.out.printf(
                    "%-5d | %-19s |%-10d|%-8.2f|%-8.2f\n",
                    i + 1, productGroups[index].getProducts().get(i).getName(),
                    productGroups[index].getProducts().get(i).getQuantity(),
                    productGroups[index].getProducts().get(i).getCost(),
                    productGroups[index].getProducts().get(i).getPrice()
                );
            }
            System.out.println("----------------------------------------------");
            System.out.println("Please select which product you want to modify ");
            System.out.printf("Enter %d to return to menu\n", productGroups[index].getProducts().size() + 1);
            System.out.println("----------------------------------------------");

            int option = InputValidation.getIntegerInput("Enter your option :");

            //make sure option is in range 
            if (option > 0 && option <= productGroups[index].getProducts().size()) {
                Product ptr = productGroups[index].getProducts().get(option - 1);
                boolean innerLoop = true;

                do {
                    try {
                        ptr.print();
                        printEditMenu();
                        int choice = InputValidation.getIntegerInput("Enter your option :");

                        //modify => 1. name,2.quantity,3.cost,4.price,5.exit
                        switch (choice) {
                            case 1:
                                String name = InputValidation.getStringInput("Enter new name :");

                                for (Product p : productGroups[index].getProducts())
                                    if (name.equals(p.getName()))
                                        throw new AssertionError();

                                ptr.setName(name);
                                break;
                            case 2:
                                int quantity = InputValidation.getIntegerInput("Enter new quantity :");
                                if (quantity == (ptr.getQuantity())) throw new AssertionError();
                                
                                if (quantity < ptr.getQuantity())
                                    stockTransactions.add(new StockTransaction(ptr.getName(), "OUT", ptr.getQuantity() - quantity));
                                else
                                    stockTransactions.add(new StockTransaction(ptr.getName(), "IN", quantity - ptr.getQuantity()));
                                
                                ptr.setQuantity(quantity);
                                break;
                            case 3:
                                float cost = InputValidation.getFloatInput("Enter new cost :");

                                if (cost == (ptr.getCost()))
                                    throw new AssertionError();

                                ptr.setCost(cost);
                                break;
                            case 4:
                                float price = InputValidation.getFloatInput("Enter new price :");

                                if (price == (ptr.getPrice()))
                                    throw new AssertionError();

                                ptr.setPrice(price);
                                break;
                            case 5:
                                System.out.println("Returning to Product Selection ... \n");
                                innerLoop = false;
                                break;
                            default:
                                System.out.println("Invalid input,Please try again");
                                innerLoop = true;
                        }
                    } catch (final AssertionError err) {
                        System.out.println("Input cannot be the same as previous one, Please try again");
                        innerLoop = true;
                    }
                } while (innerLoop);
            } else if (option == productGroups[index].getProducts().size() + 1) {
                System.out.println("Returning to Category Menu ...");
                isContinue = false;
            } else {
                System.out.println("Invalid input ,Please try again");
            }
        } while (isContinue);
    }
    private void deleteProduct() {
        //display category menu and ask user option, pass array position (category product)
        while (true) {
            printProductGroupDeleteMenu();
            final int type = InputValidation.getIntegerInput("Option :");
            if (type > 0 && type <= productGroups.length) deleteSpecificProduct(type - 1);
            else if (type == productGroups.length + 1) return;
            else System.out.println("Invalid input, Please enter again");
        }
    }
    /**
     * This method is to show product according group and let user choose which to delete
     * @param index is group of product
     */
    private void deleteSpecificProduct(final int index) {
        boolean isContinue = true;

        do {
            System.out.println("Index | Product description | Quantity | Cost   | Price ");
            System.out.println("----------------------------------------------");
            for (int i = 0; i < productGroups[index].getProducts().size(); ++i) {
                System.out.printf(
                    "%-5d | %-19s |%-10d|%-8.2f|%-8.2f\n",
                    i + 1, productGroups[index].getProducts().get(i).getName(),
                    productGroups[index].getProducts().get(i).getQuantity(),
                    productGroups[index].getProducts().get(i).getCost(),
                    productGroups[index].getProducts().get(i).getPrice()
                );
            }
            System.out.println("----------------------------------------------");
            System.out.printf("Enter %d to return to menu\n", productGroups[index].getProducts().size() + 1);
            int option = InputValidation.getIntegerInput("Please enter the index of Product to delete:");

            if (option > 0 && option <= productGroups[index].getProducts().size()) {
                Product ptr = productGroups[index].getProducts().get(option - 1);
                ptr.print();
                System.out.println("Are you sure you want to delete the Product ?[Y/N]");
                char answer = InputValidation.getCharOption();

                //confirmation delete
                switch (answer) {
                    case 'y': case 'Y':
                        productGroups[index].getProducts().remove(option - 1);
                        System.out.println("The product has been deleted");
                        break;
                    case 'n': case 'N': System.out.println("The Product is reserved");
                        break;
                    default: System.out.println("Invalid input ! The Product is reserved");
                }
                
            } else if (option == productGroups[index].getProducts().size() + 1) {
                System.out.println("Returning to Category Menu ...");
                isContinue = false;
            } else {
                System.out.println("Invalid input , Please try again");
            }
        } while (isContinue);
    }
    private void displayProduct () {
        //display category menu and ask user option, pass array position (category product) 
        while (true) {
            printProductGroupDisplayMenu();
            final int type = InputValidation.getIntegerInput("Option :");
            if (type > 0 && type <= productGroups.length) displaySpecificProduct(type - 1);
            else if (type == productGroups.length + 1) return;  
            else System.out.println("Invalid input, Please enter again");
        }
    }
    //show item according category product and user option show details product
    private void displaySpecificProduct(final int index) {
        boolean isContinue = true;
        do {
            System.out.println("Index | Product description");
            System.out.println("----------------------------------------------");
            
            for (int i = 0; i < productGroups[index].getProducts().size(); ++i)
            System.out.printf("%-5d | %-30s\n", i + 1, productGroups[index].getProducts().get(i).getName());
            System.out.println("----------------------------------------------");
            System.out.printf("Enter %d to return to menu\n", productGroups[index].getProducts().size() + 1);
            System.out.println("Enter the index to show details product");
            System.out.println("----------------------------------------------");

            int option = InputValidation.getIntegerInput("Enter option:");

            if (option > 0 && option <= productGroups[index].getProducts().size()) {
                Product ptr = productGroups[index].getProducts().get(option - 1);
                ptr.print();
                System.out.print("Entery any key to continue ");
                console().readPassword(); // to receive input without echo
            } else if (option == productGroups[index].getProducts().size() + 1) {
                System.out.println("Returning to Category Menu ...");
                isContinue = false;
            } else {
                System.out.println("Invalid input , Please try again");
            }
        } while (isContinue);
    }

    private static void printProductGroupAddMenu(){
        System.out.println("-----------------------------------------------");        
        System.out.println("Select Category Of Products you want to Create:");
        System.out.println("-----------------------------------------------");
        System.out.println("1.SmartPhone Group");
        System.out.println("2.Tablet Group");
        System.out.println("3.Laptop Group");
        System.out.println("4.PC Group");
        System.out.println("5.Return Stock Module");
        System.out.println("-----------------------------------------------");
    }
    private static void printProductGroupEditMenu(){
        System.out.println("---------------------------------------------");        
        System.out.println("Select Category Of Products you want to Edit:");
        System.out.println("---------------------------------------------");
        System.out.println("1.Smartphone Group");
        System.out.println("2.Tablet Group");
        System.out.println("3.Laptop Group");
        System.out.println("4.PC Group");
        System.out.println("5.Return Stock Module");
        System.out.println("---------------------------------------------");
    }
    private static void printEditMenu () {
        System.out.println("-------------------------------------------");            
        System.out.println("Select details Of Product you want to Edit:");
        System.out.println("-------------------------------------------");
        System.out.println("1. Name");
        System.out.println("2. Quantity");
        System.out.println("3. Cost");
        System.out.println("4. Price");
        System.out.println("5. Back");
        System.out.println("-------------------------------------------");
    }
    private static void printProductGroupDeleteMenu(){
        System.out.println("-----------------------------------------------");     
        System.out.println("Select Category Of Products you want to Delete:");
        System.out.println("-----------------------------------------------");
        System.out.println("1.Smartphone Group");
        System.out.println("2.Tablet Group");
        System.out.println("3.Laptop Group");
        System.out.println("4.PC Group");
        System.out.println("5.Return Stock Module");
        System.out.println("-------------------------------------------");
    }
    private static void printProductGroupDisplayMenu(){
        System.out.println("------------------------------------------------");
        System.out.println("Select Category Of Products you want to Display:");
        System.out.println("------------------------------------------------");
        System.out.println("1.Smartphone Group");
        System.out.println("2.Tablet Group");
        System.out.println("3.Laptop Group");
        System.out.println("4.PC Group");
        System.out.println("5.Return Stock Module");
        System.out.println("------------------------------------------------");
    }
    private static void printStockMenu(ProcessBuilder clsProcessBuilder) throws IOException, InterruptedException {
        clsProcessBuilder.start().waitFor();
        System.out.println("------------------------------------------");
        System.out.println("Stock Maintenance Module");
        System.out.println("------------------------------------------");
        System.out.println("1. Create New Products");
        System.out.println("2. Modify Products");
        System.out.println("3. Delete Products");
        System.out.println("4. Display Products");
        System.out.println("5. Exit The Module");
        System.out.println("------------------------------------------");
    }
    public int getAddedQuantity() {
        return addedQuantity;
    }
    public String getAddedProduct() {
        return addedProduct;
    }
    public int getQtyChg() {
        return qtyChg;
    }
}