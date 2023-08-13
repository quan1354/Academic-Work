package aple.pos.modification;

import static java.lang.System.console;

public class EmailModification {
    protected String newEmail;
    
    {
        init();
    }
    
    protected void init() {
        System.out.println();
        System.out.print("New Email: ");
        newEmail = console().readLine();
    }
    
    public final String getNewEmail() {
        return newEmail;
    }
}