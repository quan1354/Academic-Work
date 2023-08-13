package aple.pos.any;

import aple.pos.payment.Payment;
import aple.pos.stock.StockModule;
import static java.lang.System.console;
import static java.time.temporal.ChronoUnit.SECONDS;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Date;
import java.util.Scanner;

/**
 *
 * @author user
 */
public class Report{
    private final Payment payment;
    private final ProductGroup[] productGroups;
    private StockModule stock;
    private final ArrayList<StockTransaction> stockTransactions;
    Product product;
    private int reportType;
    Scanner input = new Scanner (System.in);

    public Report(Payment payment, ProductGroup[] productGroups, ArrayList<StockTransaction> stockTransactions) {
        this.payment = payment;
        this.productGroups = productGroups;
        this.stockTransactions = stockTransactions;
    }
    
    public void createReport() {
        do{
            System.out.println("Report Type");
            System.out.println("===========");
            System.out.println("1. Daily Sales Report");
            System.out.println("2. Daily Stock Orders Report");
            System.out.println("3. Most popular Product of the day");
            System.out.println("4. Return");
            System.out.print("Enter the report wish to generate: ");
            reportType = input.nextInt();
            System.out.println();
            switch (reportType){
                case 1: dailySalesReport();
                    break;
                case 2: dailyStockOrdersReport();
                    break;
                case 3: mostPopularProduct();
                    break;
                case 4: return;
                default:
                    System.out.println("Invalid selection.");
                    System.out.println();
            }
        } while (reportType < 1 || reportType > 4);
    }
    
    private void dailySalesReport(){
        System.out.println("                            Aple Store");
        System.out.println("                        Daily Sales Report");
        System.out.println("                        ==================");
        System.out.println("Date: " + new Date());
        System.out.println();
        System.out.println("Time      Product                      Price      Qty  Subtotal");
        System.out.println("===============================================================");
        float total = 0.f;
        for(int i=0;i<payment.getPaymentCount();i++){
            for(int j=0;j<4;j++){
                for(int l=0;l<payment.getOrders().get(i).getProductGroups()[j].getProducts().size();l++){
                    float subtotal;
                    System.out.printf(
                        "%s  %-25s %8.2f %8d %9.2f\n",
                        payment.getPaymentTime().get(i).truncatedTo(SECONDS),
                        payment.getOrders().get(i).getProductGroups()[j].getProducts().get(l).getName(),
                        payment.getOrders().get(i).getProductGroups()[j].getProducts().get(l).getPrice(),
                        payment.getOrders().get(i).getProductGroups()[j].getProducts().get(l).getQuantity(),
                        (subtotal = payment.getOrders().get(i).getProductGroups()[j].getProducts().get(l).getPrice() *
                        payment.getOrders().get(i).getProductGroups()[j].getProducts().get(l).getQuantity())
                    );
                    total += subtotal;
                }
            }
        }
        System.out.println("---------------------------------------------------------------");
        System.out.printf("Total Daily Sales: RM%.2f\n", total);
        System.out.println("---------------------------------------------------------------");
        System.out.println("===============================================================");
        console().readPassword(); // to receive input without echo
    }
    
    private void dailyStockOrdersReport() {
        System.out.println("                Aple Store");
        System.out.println("            Daily Stock Report");
        System.out.println("            ==================");
        System.out.println("Date: " + new Date());
        System.out.println();
        System.out.println("Product                   Status    Quantity");
        System.out.println("============================================");
        
        for (final StockTransaction stockTransaction : stockTransactions)
            System.out.printf(
                "%-25s %6s %8d\n", stockTransaction.getProductName(), stockTransaction.getStatus(), stockTransaction.getQuantity()
            );
        
        console().readPassword(); // to receive input without echo
    }
    
    private void mostPopularProduct(){
        System.out.println("                Aple Store");
        System.out.println("      Top 20 Daily Hot Selling Report");
        System.out.println("      ===============================");
        System.out.println("Date: " + new Date());
        System.out.println();
        System.out.println("Product                   Sales Qty  Revenue");
        System.out.println("============================================");
        
        final Product[] products = Arrays.stream(productGroups).flatMap(productGroup -> productGroup.getProducts().stream()).sorted(
            (l, r) -> r.getSalesQuantity() - l.getSalesQuantity()
        ).limit(20).toArray(Product[]::new);
        
        for (final Product product : products) {
            System.out.printf(
                "%-25s %9d %8.2f\n",
                product.getName(), product.getSalesQuantity(), (product.getPrice() - product.getCost()) * product.getSalesQuantity()
            );
        }
        
        console().readPassword(); // to receive input without echo
    }
}