package aple.pos.enums;

public enum Position {
    NONE("None"), MANAGER("Manager"), CLERK("Clerk"), ANY("Any");
    
    private final String value;
    
    private Position(final String value) {
        this.value = value;
    }
    
    @Override public String toString() {
        return value;
    }
}