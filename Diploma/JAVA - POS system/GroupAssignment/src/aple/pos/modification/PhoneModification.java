package aple.pos.modification;

import static java.lang.System.console;

public class PhoneModification {
    protected String newPhone;
    
    {
        init();
    }
    
    protected void init() {
        System.out.println();
        System.out.print("New Phone No.: ");
        newPhone = console().readLine();
    }
    
    public final String getNewPhone() {
        return newPhone;
    }
}