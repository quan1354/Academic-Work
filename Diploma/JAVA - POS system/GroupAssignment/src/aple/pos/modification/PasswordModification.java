package aple.pos.modification;

import static java.lang.System.console;

public class PasswordModification {
    protected char[] newPassword;
    protected char[] retypedPassword;
    
    {
        init();
    }
    
    protected void init() {
        System.out.println();
        System.out.print("New Password: ");
        newPassword = console().readPassword();
        System.out.print("Retype Password: ");
        retypedPassword = console().readPassword();
    }
    
    public final char[] getNewPassword() {
        return newPassword;
    }
    
    public final char[] getRetypedPassword() {
        return retypedPassword;
    }
}