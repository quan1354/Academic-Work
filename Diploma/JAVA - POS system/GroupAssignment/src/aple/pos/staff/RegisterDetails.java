package aple.pos.staff;

import aple.pos.any.Staff;
import static aple.pos.enums.Position.CLERK;

public final class RegisterDetails extends Staff {
    private final char[] retypedPassword;
    
    public RegisterDetails(final String name, final char[] password, final char[] retypedPassword, final String email, final String phone) {
        super(name, password, CLERK, email, phone);
        this.retypedPassword = retypedPassword;
    }
    
    public char[] getRetypedPassword() {
        return retypedPassword;
    }
}