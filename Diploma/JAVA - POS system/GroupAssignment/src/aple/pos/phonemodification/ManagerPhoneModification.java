package aple.pos.phonemodification;

import aple.pos.modification.PhoneModification;
import static java.lang.System.console;

public final class ManagerPhoneModification extends PhoneModification {
    private String selectedUsername;
    
    @Override protected void init() {
        System.out.println();
        System.out.print("Selected Username: ");
        selectedUsername = console().readLine();
        System.out.print("New Phone: ");
        newPhone = console().readLine();
    }
    
    public String getSelectedUsername() {
        return selectedUsername;
    }
}