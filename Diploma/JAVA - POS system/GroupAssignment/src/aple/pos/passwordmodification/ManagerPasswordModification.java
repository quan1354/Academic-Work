package aple.pos.passwordmodification;

import aple.pos.modification.PasswordModification;
import static java.lang.System.console;

public final class ManagerPasswordModification extends PasswordModification {
    private String selectedUsername;
    
    @Override protected void init() {
        System.out.println();
        System.out.print("Selected Username: ");
        selectedUsername = console().readLine();
        System.out.print("New Password: ");
        newPassword = console().readPassword();
        System.out.print("Retype Password: ");
        retypedPassword = console().readPassword();
    }
    
    public String getSelectedUsername() {
        return selectedUsername;
    }
}