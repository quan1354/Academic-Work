package aple.pos.any;

import aple.pos.enums.Position;
import static aple.pos.enums.Position.ANY;
import static aple.pos.enums.Position.MANAGER;
import static aple.pos.enums.Position.CLERK;
import java.util.Arrays;
import java.util.Objects;

public class Staff {
    private final int id;
    private final Position position;
    private String name;
    private char[] password;
    private String email;
    private String phone;
    
    protected Staff(final String name, final char[] password) {
        this(name, password, null, null);
    }
    
    protected Staff(final String name, final char[] password, final Position position) {
        this(name, password, position, null, null);
    }
    
    protected Staff(final String name, final char[] password, final String email, final String phone) {
        this(name, password, ANY, email, phone);
    }
    
    protected Staff(final String name, final char[] password, final Position position, final String email, final String phone) {
        this.name = name;
        this.password = password;
        this.position = position;
        this.email = email;
        this.phone = phone;
        id = hashCode();
    }
    
    public final Position getPosition() {
        return position;
    }
    
    public final String getName() {
        return name;
    }
    
    final void setName(final String name) {
        this.name = name;
    }
    
    public final char[] getPassword() {
        return password;
    }
    
    final void setPassword(final char[] password) {
        this.password = password;
    }
    
    public final String getEmail() {
        return email;
    }
    
    final void setEmail(final String email) {
        this.email = email;
    }
    
    public final String getPhone() {
        return phone;
    }
    
    final void setPhone(final String phone) {
        this.phone = phone;
    }
    
    final void show() {
        switch (position) {
            case MANAGER: System.out.println("Position: Manager");
                break;
            case CLERK: System.out.println("Position: Clerk");
        }
        System.out.println("Username: " + name);
        System.out.println("Password: " + String.valueOf(password));
        System.out.println("Email   : " + email);
        System.out.println("Phone   : " + phone);
    }
    
    @Override public int hashCode() {
        int hash = 7;
        hash = 29 * hash + Objects.hashCode(this.name);
        hash = 29 * hash + Arrays.hashCode(this.password);
        return hash;
    }
    
    @Override public final boolean equals(final Object thatObj) {
        final Staff that = (Staff)thatObj;
        if (id != that.id) return false;
        return (position == ANY || that.position == ANY || position == that.position) && name.equals(that.name) &&
            Arrays.equals(password, that.password) && (that.email == null || email.equals(that.email)) &&
            (that.phone == null || phone.equals(that.phone));
    }
}