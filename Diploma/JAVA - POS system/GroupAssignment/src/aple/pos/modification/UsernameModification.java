package aple.pos.modification;

import static java.lang.System.console;

public class UsernameModification {
    protected String newUsername;
    
    {
        init();
    }
    
    protected void init() {
        System.out.println();
        System.out.print("New Username: ");
        newUsername = console().readLine();
    }
    
    public final String getNewUsername() {
        return newUsername;
    }
}