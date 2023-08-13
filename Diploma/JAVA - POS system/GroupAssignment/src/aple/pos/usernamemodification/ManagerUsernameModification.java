package aple.pos.usernamemodification;

import aple.pos.modification.UsernameModification;
import static java.lang.System.console;

public final class ManagerUsernameModification extends UsernameModification {
    private String oldUsername;
    
    @Override protected void init() {
        System.out.println();
        System.out.print("Old Username: ");
        oldUsername = console().readLine();
        System.out.print("New Username: ");
        newUsername = console().readLine();
    }
    
    public String getOldUsername() {
        return oldUsername;
    }
}