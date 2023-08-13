package aple.pos.payment;

import aple.pos.any.Order;
import aple.pos.any.Product;
import aple.pos.any.ProductGroup;
import aple.pos.any.Staff;
import aple.pos.any.StockTransaction;
import static java.lang.System.console;
import java.time.LocalTime;
import java.util.ArrayList;
import java.util.Date;
import java.util.InputMismatchException;
import java.util.Scanner;

/**
 *
 * @author user
 */
public class Payment{
    private String paymentType;
    private String cardNum;
    private int chooseType;
    private final ArrayList<Order> orders;
    private final ArrayList<Float> subtotals = new ArrayList<>();
    private float total = 0;
    private float amountReceived;
    private float change;
    private final Card[] cards;
    private final ProductGroup[] productGroups;
    private int[] salesQty;
    private int paymentCount = 0;
    private int currentCardI;
    protected ArrayList<LocalTime> paymentTime = new ArrayList<>();
    private final ArrayList<StockTransaction> stockTransactions;
    Scanner input = new Scanner(System.in);

    public Payment(ArrayList<Order> orders, ProductGroup[] productGroups, Card[] cards, ArrayList<StockTransaction> stockTransactions) {
        this.orders = orders;
        this.productGroups = productGroups;
        this.cards = cards;
        this.stockTransactions = stockTransactions;
    }
    
    public void createPayment(final ProcessBuilder clsProcessBuilder, final Staff currentStaff){
        do {
            try {
                chooseType = paymentType();
                switch (chooseType){
                    case 1:
                        paymentType = "Debit Card";
                        currentCard("Debit Card");
                        break;
                    case 2:
                        paymentType = "Credit Card";
                        currentCard("Credit Card");
                        break;
                    case 3: paymentType = "Cash";
                        break;
                    case 4: return;
                    default: throw new InputMismatchException();
                }
            } catch (final InputMismatchException err) {
                System.out.println();
                System.out.println("Invalid option.");
                input.nextLine();
                System.out.println();
            }
        } while (chooseType < 1 || chooseType > 3);
        
        if (orders.get(orders.size() - 1).isPaid()) orders.add(new Order(clsProcessBuilder, input));
        
        for(int i=0;i<4;i++){
            for(int j=0;j<orders.get(orders.size() - 1).getProductGroups()[i].getProducts().size();j++){
                final float subtotal = orders.get(orders.size() - 1).getProductGroups()[i].getProducts().get(j).getPrice() *
                    orders.get(orders.size() - 1).getProductGroups()[i].getProducts().get(j).getQuantity();
                subtotals.add(subtotal);
                total += subtotal;
            }
        }
        System.out.println("Order Summary");
        System.out.println("===============================================");
        System.out.println("Menu                Price   Qty.    Total Price");
        System.out.println("===============================================");
        
        int k = 0;
        for(int i=0;i<4;i++){
            for(int j=0;j<orders.get(orders.size() - 1).getProductGroups()[i].getProducts().size();j++){
                System.out.printf("%-19s%.2f%7d%15.2f\n",orders.get(orders.size() - 1).getProductGroups()[i].getProducts().get(j).getName(),
                    orders.get(orders.size() - 1).getProductGroups()[i].getProducts().get(j).getPrice(),
                    orders.get(orders.size() - 1).getProductGroups()[i].getProducts().get(j).getQuantity(), subtotals.get(k));
                // salesQty[k]++;
                ++k;
            }
        }
        System.out.println("===============================================");        
        System.out.println();
        System.out.printf("Total Amount : RM%.2f\n", total);
        
        if (paymentType.equals("Cash")) {
            System.out.printf("Enter payment amount : ");
            amountReceived = input.nextFloat();
            System.out.println();
            if (amountReceived < total) {
                System.out.println("Unsuccessful order.");
                console().readPassword(); // to receive input without echo
                for (final ProductGroup productGroup : productGroups) productGroup.undoOrderPart();
                orders.get(orders.size() - 1).init(clsProcessBuilder, input);
            } else {
                change = amountReceived - total;
                receipt(currentStaff);
                paymentCount++;
                
                for (final ProductGroup productGroup : orders.get(orders.size() - 1).getProductGroups())
                    for (final Product product : productGroup.getProducts())
                        stockTransactions.add(new StockTransaction(product.getName(), "OUT", product.getQuantity()));
                for (final ProductGroup productGroup : productGroups) productGroup.commitOrderPart();
                orders.get(orders.size() - 1).setPaid(true);
            }
        } else if (cards[currentCardI].getBalance() < total) {
            System.out.println();
            System.out.println("Unsuccessful order. Not enough balance.");
            console().readPassword(); // to receive input without echo
            for (final ProductGroup productGroup : productGroups) productGroup.undoOrderPart();
            orders.get(orders.size() - 1).init(clsProcessBuilder, input);
        } else {
            System.out.println();
            amountReceived = total;
            change = 0.f;
            cards[currentCardI].setBalance(cards[currentCardI].getBalance() - total);
            receipt(currentStaff);
            paymentCount++;
            
            for (final ProductGroup productGroup : orders.get(orders.size() - 1).getProductGroups())
                for (final Product product : productGroup.getProducts())
                    stockTransactions.add(new StockTransaction(product.getName(), "OUT", product.getQuantity()));
            
            for (final ProductGroup productGroup : productGroups) productGroup.commitOrderPart();
            orders.get(orders.size() - 1).setPaid(true);
        }
        reset();
    }
    
    private void receipt(final Staff currentStaff) {
        System.out.println("Receipt");
        System.out.println("=======");
        System.out.println("Store: Aple Store");
        System.out.println("Address: 4 Kompleks Pertama Jln Tuanku Abdul Rahman, 50100 Kuala Lumpur, Malaysia");
        System.out.println(currentStaff.getPosition() + ": " + currentStaff.getName());
        System.out.println();
        System.out.println(new Date());
        System.out.println();
        paymentTime.add(LocalTime.now());
        System.out.println("===============================================");
        System.out.println("Menu                Price   Qty.    Total Price");
        System.out.println("===============================================");
        
        int k = 0;
        for(int i=0;i<4;i++){
            for(int j=0;j<orders.get(orders.size() - 1).getProductGroups()[i].getProducts().size();j++){
                System.out.printf("%-19s%.2f%7d%15.2f\n",orders.get(orders.size() - 1).getProductGroups()[i].getProducts().get(j).getName(),
                    orders.get(orders.size() - 1).getProductGroups()[i].getProducts().get(j).getPrice(),
                    orders.get(orders.size() - 1).getProductGroups()[i].getProducts().get(j).getQuantity(), subtotals.get(k));
                // salesQty[k]++;
                ++k;
            }
        }
        
        System.out.println();
        System.out.printf("Total price   : RM%.2f\n", total);
        System.out.printf("Amount of pay : RM%.2f\n", amountReceived);
        System.out.printf("Changes       : RM%.2f\n", change);
        System.out.println("Payment type  : "+ paymentType);
        if (!"Cash".equals(paymentType)) System.out.println("Card(************"+cardNum.substring(cardNum.length() - 4)+")");
        System.out.println("===============================================");
        System.out.println("                Return Policy");
        System.out.println("               All Sales Final");
        System.out.println("        Thank you for purchasing with us!");
        console().readPassword(); // to receive input without echo
    }
    
    private void currentCard(final String cardType) {
        input.nextLine(); // to clear garbage input
        while (true) {
            System.out.println();
            System.out.print("Enter card number: ");
            cardNum = input.nextLine();
            if (cardNum.length() != 16) {
                System.out.println("Invalid card number. Please enter again.");
                continue;
            }
            for (int i = 0; i != cards.length; ++i) {
                if (!(cardNum.equals(cards[i].getCardNo()) && cards[i].getType().equals(cardType))) continue;
                currentCardI = i;
                return;
            }
            System.out.println("Invalid card number. Please enter again.");
        }
    }
    
    private int paymentType(){
        System.out.println("Payment type");
        System.out.println("------------");
        System.out.println("1.Debit Card");
        System.out.println("2.Credit Card");
        System.out.println("3.Cash");
        System.out.println("4.Back");
        System.out.print("Select payment type: ");
        chooseType = input.nextInt();
        return chooseType;
    }

    public ArrayList<Order> getOrders() {
        return orders;
    }

    public float getTotal() {
        return total;
    }

    public ArrayList<Float> getSubtotal() {
        return subtotals;
    }

    public int[] getSalesQty() {
        return salesQty;
    }
    
    public int getPaymentCount() {
        return paymentCount;
    }
    
    public ArrayList<LocalTime> getPaymentTime() {
        return paymentTime;
    }
    
    private void reset() {
        subtotals.clear();
        total = 0;
    }
}