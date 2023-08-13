package aple.pos.any;

public final class StockTransaction {
    private final String productName;
    private final String status;
    private final int quantity;

    public StockTransaction(final String productName, final String status, final int quantity) {
        this.productName = productName;
        this.status = status;
        this.quantity = quantity;
    }
    
    String getProductName() {
        return productName;
    }
    
    String getStatus() {
        return status;
    }
    
    int getQuantity() {
        return quantity;
    }
}