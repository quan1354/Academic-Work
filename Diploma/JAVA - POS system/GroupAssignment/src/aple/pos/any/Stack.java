package aple.pos.any;

final class Stack<T> {
    private final T[] ts;
    private int top = 0;
    
    /**
     * @param ts it must be created without initializing
     */
    Stack(final T[] ts) {
        this.ts = ts;
    }
    
    boolean isEmpty() {
        return top == 0;
    }
    
    void push(final T t) {
        ts[top++] = t;
    }
    
    T pop() {
        return ts[--top];
    }
}