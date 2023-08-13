package aple.pos.any;

import aple.pos.enums.Action;
import static aple.pos.enums.Action.BACK;
import static aple.pos.enums.Action.GO_TO_PAYMENT;
import static aple.pos.enums.Action.NONE;
import aple.pos.productgroup.TabletGroup;
import aple.pos.productgroup.SmartphoneGroup;
import aple.pos.productgroup.PCGroup;
import aple.pos.productgroup.LaptopGroup;
import java.util.Scanner;
import java.io.IOException;
import static aple.pos.enums.Position.MANAGER;
import aple.pos.payment.Card;
import aple.pos.payment.Payment;
import aple.pos.stock.StockModule;
import static java.lang.Runtime.getRuntime;
import java.util.InputMismatchException;
import static java.lang.System.console;
import java.util.ArrayList;

public final class POS {
    private final ProcessBuilder clsProcessBuilder = new ProcessBuilder("cmd", "/c", "cls").inheritIO();
    private final Scanner input = new Scanner(System.in);
    private final StaffGroup staffGroup = new StaffGroup(this);
    private final ProductGroup[] productGroups = {
        new SmartphoneGroup(clsProcessBuilder, input),
        new TabletGroup(clsProcessBuilder, input),
        new LaptopGroup(clsProcessBuilder, input),
        new PCGroup(clsProcessBuilder, input)
    };
    private final ArrayList<Order> orders = new ArrayList<>(){{
        add(new Order(clsProcessBuilder, input));
    }};
    private Staff currentStaff;
    private final ArrayList<StockTransaction> stockTransactions = new ArrayList<>();
    private final StockModule stockModule = new StockModule(productGroups, stockTransactions);
    Card cards[] = {
        new Card("Hong Leong Bank","Debit Card", "2347889965471254",5000.0f),
        new Card("Public Bank","Debit Card",     "2356789901235567",3000.0f),
        new Card("AmBank","Debit Card",          "2356785132035569",4000.0f),
        new Card("UOB Bank","Credit Card",       "4335699878892330",7000.0f),
        new Card("Hong Leong Bank","Credit Card","4335694685219232",2000.0f),
        new Card("Public Bank","Credit Card",    "4335690234992333",5000.0f),
    };
    Payment payment = new Payment(orders, productGroups, cards, stockTransactions);
    Report report = new Report(payment,productGroups, stockTransactions);
    
    public static void main(final String[] args) throws IOException, InterruptedException {
        if (args.length != 1 || !args[0].equals("--run"))
            getRuntime().exec(String.format("cmd /c start cmd /k java -jar %s\\dist\\POS.jar --run", System.getProperty("user.dir")));
        else {
            final POS system = new POS();
            while (true) {
                system.goToLoginPage();
                system.goToMainMenu();
            }
        }
    }
    
    ProcessBuilder getClsProcessBuilder() {
        return clsProcessBuilder;
    }
    
    Scanner getInputScanner() {
        return input;
    }
    
    private void goToLoginPage() throws IOException, InterruptedException {
        do printLogo();
        while ((currentStaff = staffGroup.loginStaff()) == null);
    }
    
    private void printLogo() throws IOException, InterruptedException {
        clsProcessBuilder.start().waitFor();
        System.out.println(
            "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n" +
            "|                 _        _____   ____   _____ |\n" +
            "|     /\\         | |      |  __ \\ / __ \\ / ____||\n" +
            "|    /  \\   _ __ | | ___  | |__) | |  | | (___  |\n" +
            "|   / /\\ \\ | '_ \\| |/ _ \\ |  ___/| |  | |\\___ \\ |\n" +
            "|  / ____ \\| |_) | |  __/ | |    | |__| |____) ||\n" +
            "| /_/    \\_\\ .__/|_|\\___| |_|     \\____/|_____/ |\n" +
            "|          | |                                  |\n" +
            "|          |_|                                  |\n" +
            "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
        );
    }
    
    private void goToMainMenu() throws IOException, InterruptedException {
        while (true) {
            printMainMenu();
            try {
                if (reactMainMenuChoice(input.nextInt())) return;
            } catch (final InputMismatchException err) {
                System.out.println();
                System.out.println("Invalid choice! Please try again. Press enter key to continue.");
                input.nextLine();
                console().readPassword(); // to receive input without echo
            }
        }
    }
    
    private void printMainMenu() throws IOException, InterruptedException {
        clsProcessBuilder.start().waitFor();
        System.out.println("Main Menu");
        System.out.println("1. Staff");
        System.out.println("2. Gallery");
        System.out.println("3. Payment");
        System.out.println("4. Stock Management");
        System.out.println("5. Reporting");
        System.out.println("6. Sign Out");
        System.out.print("Enter a choice > ");
    }
    
    /**
     * @return true to back the menu, false otherwise
     */
    private boolean reactMainMenuChoice(final int choice) throws IOException, InterruptedException {
        switch (choice) {
            case 1: goToStaffMenu();
                return false;
            case 2:
                if (goToGalleryMenu() != GO_TO_PAYMENT) return false;
            case 3:
                clsProcessBuilder.start().waitFor();
                payment.createPayment(clsProcessBuilder, currentStaff);
                return false;
            case 4:
                clsProcessBuilder.start().waitFor();
                stockModule.processMainMenu(clsProcessBuilder);
                return false;
            case 5:
                if (currentStaff.getPosition() != MANAGER) {
                    System.out.println();
                    System.out.println("Sorry, you are not allowed to view the reports! Press enter key to continue.");
                    console().readPassword(); // to receive input without echo
                } else {
                    clsProcessBuilder.start().waitFor();
                    report.createReport();    
                }
                return false;
            case 6: return true;
            default: throw new InputMismatchException();
        }
    }
    
    private void goToStaffMenu() throws IOException, InterruptedException {
        while (true) {
            printStaffMenu();
            try {
                if (reactStaffMenuChoice(input.nextInt())) return;
            } catch (final InputMismatchException err) {
                System.out.println();
                System.out.println("Invalid choice! Please try again. Press enter key to continue.");
                input.nextLine();
                console().readPassword(); // to receive input without echo
            }
        }
    }
    
    private Action goToGalleryMenu() throws IOException, InterruptedException {
        while (true) {
            printGalleryMenu();
            try {
                switch (reactGalleryMenuChoice(input.nextInt())) {
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
    
    private void printStaffMenu() throws IOException, InterruptedException {
        clsProcessBuilder.start().waitFor();
        System.out.println("Staff Menu");
        System.out.println("1. Add Staff");
        System.out.println("2. Modify Staff");
        System.out.println("3. Delete Staff");
        System.out.println("4. Display Staff" + (currentStaff.getPosition() == MANAGER ? "s" : ""));
        System.out.println("5. Back");
        System.out.print("Enter a choice > ");
    }
    
    private void printGalleryMenu() throws IOException, InterruptedException {
        clsProcessBuilder.start().waitFor();
        System.out.println("Gallery Menu");
        System.out.println("1. Smartphone");
        System.out.println("2. Tablet");
        System.out.println("3. Laptop");
        System.out.println("4. PC");
        System.out.println("5. Back");
        System.out.print("Enter a choice > ");
    }
    
    /**
     * @return true to back the menu, false otherwise
     */
    private boolean reactStaffMenuChoice(final int choice) throws IOException, InterruptedException {
        switch (choice) {
            case 1:
                if (currentStaff.getPosition() == MANAGER) staffGroup.registerStaff();
                else {
                    System.out.println();
                    System.out.println("Sorry, you are not allowed to register new staff! Press enter key to continue.");
                    console().readPassword(); // to receive input without echo
                }
                return false;
            case 2: staffGroup.modifyStaff();
                return false;
            case 3:
                if (currentStaff.getPosition() == MANAGER) staffGroup.deleteStaff();
                else {
                    System.out.println();
                    System.out.println("Sorry, you are not allowed to delete staff! Press enter key to continue.");
                    console().readPassword(); // to receive input without echo
                }
                return false;
            case 4: staffGroup.show();
                return false;
            case 5: return true;
            default: throw new InputMismatchException();
        }
    }
    
    /**
     * @return true to back the menu, false otherwise
     */
    private Action reactGalleryMenuChoice(final int choice) throws IOException, InterruptedException {
        if (choice < 1 || choice > productGroups.length + 1) throw new InputMismatchException();
        else if (choice == productGroups.length + 1) return BACK;
        if (orders.get(orders.size() - 1).isPaid()) orders.add(new Order(clsProcessBuilder, input));
        if (productGroups[choice - 1].goToGalleryMenu(choice - 1, orders.get(orders.size() - 1), input) == GO_TO_PAYMENT)
            return GO_TO_PAYMENT;
        return NONE;
    }
}