package aple.pos.emailmodification;

import aple.pos.modification.EmailModification;
import static java.lang.System.console;

public final class ManagerEmailModification extends EmailModification {
    private String selectedUsername;
    
    @Override protected void init() {
        System.out.println();
        System.out.print("Selected Username: ");
        selectedUsername = console().readLine();
        System.out.print("New Email: ");
        newEmail = console().readLine();
    }
    
    public String getSelectedUsername() {
        return selectedUsername;
    }
}