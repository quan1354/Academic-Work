package aple.pos.any;

import aple.pos.emailmodification.ManagerEmailModification;
import aple.pos.modification.PasswordModification;
import aple.pos.modification.UsernameModification;
import aple.pos.staff.RegisterDetails;
import aple.pos.usernamemodification.ManagerUsernameModification;
import aple.pos.passwordmodification.ManagerPasswordModification;
import aple.pos.staff.LoginDetails;
import static aple.pos.enums.Position.CLERK;
import static aple.pos.enums.Position.MANAGER;
import aple.pos.modification.EmailModification;
import aple.pos.modification.PhoneModification;
import aple.pos.phonemodification.ManagerPhoneModification;
import java.io.IOException;
import static java.lang.System.console;
import java.util.Arrays;
import java.util.InputMismatchException;

final class StaffGroup {
    private final POS system;
    private final Staff[] staffs = new Staff[8];
    private final Stack<Integer> nullIndices = new Stack<>(new Integer[staffs.length]);
    private static final int MIN_PASSWORD_LENGTH = 8;
    private int currentStaffI;
    
    StaffGroup(final POS system) {
        this.system = system;
        staffs[0] = new Staff(
            "staff1", new char[]{ 'p', 'a', 's', 's', 'w', 'o', 'r', 'd', '1' }, MANAGER, "staff1@aple.com", "+60 12-295 6282"
        );
        staffs[1] = new Staff(
            "staff2", new char[]{ 'p', 'a', 's', 's', 'w', 'o', 'r', 'd', '2' }, CLERK, "staff2@aple.com", "+60 11-5103 1937"
        );
        staffs[2] = new Staff(
            "staff3", new char[]{ 'p', 'a', 's', 's', 'w', 'o', 'r', 'd', '3' }, CLERK, "staff3@aple.com", "+60 16-386 7029"
        );
        staffs[3] = new Staff(
            "staff4", new char[]{ 'p', 'a', 's', 's', 'w', 'o', 'r', 'd', '4' }, CLERK, "staff4@aple.com", "+60 17-249 4450"
        );
        for (int i = 0; i != 4; ++i) nullIndices.push(4 + i);
    }
    
    /**
     * @return the staff who login
     */
    Staff loginStaff() {
        final Staff staff = getStaffFromLoginDetails(acceptLoginDetails());
        if (staff != null) return staff;
        System.out.println();
        System.out.println("Invalid username or password! Please try again. Press enter key to continue.");
        console().readPassword(); // to receive input without echo
        return null;
    }
    
    private static LoginDetails acceptLoginDetails() {
        System.out.println();
        System.out.print("Username: ");
        final String username = console().readLine();
        System.out.print("Password: ");
        final char[] password = console().readPassword();
        return new LoginDetails(username, password);
    }
    
    private Staff getStaffFromLoginDetails(final Staff loginDetails) {
        for (int i = 0; i != staffs.length; ++i) {
            if (staffs[i] == null || !staffs[i].equals(loginDetails)) continue;
            currentStaffI = i;
            return staffs[i];
        }
        return null;
    }
    
    void registerStaff() {
        if (nullIndices.isEmpty()) {
            System.out.println();
            System.out.println("Too many staffs registered for this system! Please delete some. Press enter key to continue.");
            console().readPassword(); // to receive input without echo
            return;
        }
        final RegisterDetails registerDetails = acceptRegisterDetails();
        if (registerDetails.getName().isEmpty()) {
            System.out.println();
            System.out.println("Staff username cannot be empty! Please try again. Press enter key to continue.");
            console().readPassword(); // to receive input without echo
            return;
        }
        if (registerDetails.getPassword().length < MIN_PASSWORD_LENGTH) {
            System.out.println();
            System.out.println(
                "Staff password must have at least " + MIN_PASSWORD_LENGTH + " characters! Please try again. Press enter key to continue."
            );
            console().readPassword(); // to receive input without echo
            return;
        }
        if (!Arrays.equals(registerDetails.getPassword(), registerDetails.getRetypedPassword())) {
            System.out.println();
            System.out.println("Password and retyped password must be the same! Please try again. Press enter key to continue.");
            console().readPassword(); // to receive input without echo
            return;
        }
        if (!registerDetails.getEmail().matches("[a-zA-Z]\\w*@aple.com")) {
            System.out.println();
            System.out.println(
                "Invalid staff email! It must starts with letter followed by any number of letters or digits and @aple.com . " +
                "Press enter key to continue."
            );
            console().readPassword(); // to receive input without echo
            return;
        }
        if (!registerDetails.getPhone().matches("\\+\\d{2} \\d{2}-\\d{3,4} \\d{4}")) {
            System.out.println();
            System.out.println("Invalid staff phone number! Correct Example: +60 11-5103 1937 . Press enter key to continue.");
            console().readPassword(); // to receive input without echo
            return;
        }
        if (isEmailExist(registerDetails.getEmail())) {
            System.out.println();
            System.out.println("Staff email already exists! Please try again. Press enter key to continue.");
            console().readPassword(); // to receive input without echo
            return;
        }
        if (isPhoneExist(registerDetails.getPhone())) {
            System.out.println();
            System.out.println("Staff phone number already exists! Please try again. Press enter key to continue.");
            console().readPassword(); // to receive input without echo
            return;
        }
        if (getStaffI(registerDetails.getName()) != -1) {
            System.out.println();
            System.out.println("Staff username already exists! Please try again. Press enter key to continue.");
            console().readPassword(); // to receive input without echo
            return;
        }
        staffs[nullIndices.pop()] = registerDetails;
        System.out.println();
        System.out.println("Successfully registered a new staff.");
        console().readPassword(); // to receive input without echo
    }
    
    private static RegisterDetails acceptRegisterDetails() {
        System.out.println();
        System.out.print("New Username: ");
        final String username = console().readLine();
        System.out.print("New Password: ");
        final char[] password = console().readPassword();
        System.out.print("Retype Password: ");
        final char[] retypedPassword = console().readPassword();
        System.out.print("New Email: ");
        final String email = console().readLine();
        System.out.print("New Phone No.: ");
        final String phone = console().readLine();
        return new RegisterDetails(username, password, retypedPassword, email, phone);
    }
    
    /**
     * @return the index of the staff, -1 if not found
     */
    private int getStaffI(final String username) {
        for (int i = 0; i != staffs.length; ++i) {
            if (staffs[i] == null || !staffs[i].getName().equals(username)) continue;
            return i;
        }
        return -1;
    }
    
    private boolean isEmailExist(final String email) {
        for (final Staff staff : staffs) {
            if (staff == null || !staff.getEmail().equals(email)) continue;
            return true;
        }
        return false;
    }
    
    private boolean isPhoneExist(final String phone) {
        for (final Staff staff : staffs) {
            if (staff == null || !staff.getPhone().equals(phone)) continue;
            return true;
        }
        return false;
    }
    
    void modifyStaff() throws IOException, InterruptedException {
        while (true) {
            printModifyStaffMenu();
            try {
                if (reactModifyStaffMenuChoice(system.getInputScanner().nextInt())) return;
            } catch (final InputMismatchException err) {
                System.out.println();
                System.out.println("Invalid choice! Please try again. Press enter key to continue.");
                system.getInputScanner().nextLine();
                console().readPassword(); // to receive input without echo
            }
        }
    }
    
    private void printModifyStaffMenu() throws IOException, InterruptedException {
        system.getClsProcessBuilder().start().waitFor();
        System.out.println("Modify Staff Menu");
        System.out.println("1. Change Username");
        System.out.println("2. Change Password");
        System.out.println("3. Change Email");
        System.out.println("4. Change Phone No.");
        System.out.println("5. Back");
        System.out.print("Enter a choice > ");
    }
    
    /**
     * @return true to back the menu, false otherwise
     */
    private boolean reactModifyStaffMenuChoice(final int choice) {
        switch (choice) {
            case 1: modifyStaffUsername();
                return false;
            case 2: modifyStaffPassword();
                return false;
            case 3: modifyStaffEmail();
                return false;
            case 4: modifyStaffPhone();
                return false;
            case 5: return true;
            default: throw new InputMismatchException();
        }
    }
    
    private void modifyStaffUsername() {
        final UsernameModification usernameModification;
        final int selectedStaffI;
        switch (staffs[currentStaffI].getPosition()) {
            case MANAGER:
                usernameModification = new ManagerUsernameModification();
                selectedStaffI = getStaffI(((ManagerUsernameModification)usernameModification).getOldUsername());
                if (selectedStaffI == -1) {
                    System.out.println();
                    System.out.println("The staff to modify does not exist! Please try again. Press enter key to continue.");
                    console().readPassword(); // to receive input without echo
                    return;
                }
                break;
            case CLERK:
            default:
                usernameModification = new UsernameModification();
                selectedStaffI = currentStaffI;
        }
        if (usernameModification.getNewUsername().isEmpty()) {
            System.out.println();
            System.out.println("The new username must not be empty! Please try again. Press enter key to continue.");
            console().readPassword(); // to receive input without echo
            return;
        }
        if (getStaffI(usernameModification.getNewUsername()) != -1) {
            System.out.println();
            System.out.println("The new username already being used! Please try again. Press enter key to continue.");
            console().readPassword(); // to receive input without echo
            return;
        }
        staffs[selectedStaffI].setName(usernameModification.getNewUsername());
        System.out.println();
        System.out.println("Successfully modifying the staff username. Press enter key to continue.");
        console().readPassword(); // to receive input without echo
    }
    
    private void modifyStaffPassword() {
        final PasswordModification passwordModification;
        final int selectedStaffI;
        switch (staffs[currentStaffI].getPosition()) {
            case MANAGER:
                passwordModification = new ManagerPasswordModification();
                selectedStaffI = getStaffI(((ManagerPasswordModification)passwordModification).getSelectedUsername());
                if (selectedStaffI == -1) {
                    System.out.println();
                    System.out.println("The staff to modify does not exist! Please try again. Press enter key to continue.");
                    console().readPassword(); // to receive input without echo
                    return;
                }
                break;
            case CLERK:
            default:
                passwordModification = new PasswordModification();
                selectedStaffI = currentStaffI;
        }
        if (passwordModification.getNewPassword().length < MIN_PASSWORD_LENGTH) {
            System.out.println();
            System.out.println(
                "The new password must be at least " + MIN_PASSWORD_LENGTH + " characters! Please try again. Press enter key to continue."
            );
            console().readPassword(); // to receive input without echo
            return;
        }
        if (!Arrays.equals(passwordModification.getNewPassword(), passwordModification.getRetypedPassword())) {
            System.out.println();
            System.out.println("The new password and retyped password must be the same! Please try again. Press enter key to continue.");
            console().readPassword(); // to receive input without echo
            return;
        }
        staffs[selectedStaffI].setPassword(passwordModification.getNewPassword());
        System.out.println();
        System.out.println("Successfully modifying the staff password. Press enter key to continue.");
        console().readPassword(); // to receive input without echo
    }
    
    private void modifyStaffEmail() {
        final EmailModification emailModification;
        final int selectedStaffI;
        switch (staffs[currentStaffI].getPosition()) {
            case MANAGER:
                emailModification = new ManagerEmailModification();
                selectedStaffI = getStaffI(((ManagerEmailModification)emailModification).getSelectedUsername());
                if (selectedStaffI == -1) {
                    System.out.println();
                    System.out.println("The staff to modify does not exist! Please try again. Press enter key to continue.");
                    console().readPassword(); // to receive input without echo
                    return;
                }
                break;
            case CLERK:
            default:
                emailModification = new EmailModification();
                selectedStaffI = currentStaffI;
        }
        if (!emailModification.getNewEmail().matches("[a-zA-Z]\\w*@aple.com")) {
            System.out.println();
            System.out.println(
                "Invalid staff email! It must starts with letter followed by any number of letters or digits and @aple.com . " +
                "Press enter key to continue."
            );
            console().readPassword(); // to receive input without echo
            return;
        }
        if (isEmailExist(emailModification.getNewEmail())) {
            System.out.println();
            System.out.println("Staff email already exists! Please try again. Press enter key to continue.");
            console().readPassword(); // to receive input without echo
            return;
        }
        staffs[selectedStaffI].setEmail(emailModification.getNewEmail());
        System.out.println();
        System.out.println("Successfully modifying the staff email. Please enter key to continue.");
        console().readPassword(); // to receive input without echo
    }
    
    private void modifyStaffPhone() {
        final PhoneModification phoneModification;
        final int selectedStaffI;
        switch (staffs[currentStaffI].getPosition()) {
            case MANAGER:
                phoneModification = new ManagerPhoneModification();
                selectedStaffI = getStaffI(((ManagerPhoneModification)phoneModification).getSelectedUsername());
                if (selectedStaffI == -1) {
                    System.out.println();
                    System.out.println("The staff to modify does not exist! Please try again. Press enter key to continue.");
                    console().readPassword(); // to receive input without echo
                    return;
                }
                break;
            case CLERK:
            default:
                phoneModification = new PhoneModification();
                selectedStaffI = currentStaffI;
        }
        if (!phoneModification.getNewPhone().matches("\\+\\d{2} \\d{2}-\\d{3,4} \\d{4}")) {
            System.out.println();
            System.out.println("Invalid staff phone number! Correct Example: +60 11-5103 1937 . Press enter key to continue.");
            console().readPassword(); // to receive input without echo
            return;
        }
        if (isPhoneExist(phoneModification.getNewPhone())) {
            System.out.println();
            System.out.println("Staff phone number already exists! Please try again. Press enter key to continue.");
            console().readPassword(); // to receive input without echo
            return;
        }
        staffs[selectedStaffI].setPhone(phoneModification.getNewPhone());
        System.out.println();
        System.out.println("Successfully modifying the staff phone number. Press enter key to continue.");
        console().readPassword(); // to receive input without echo
    }
    
    void deleteStaff() {
        final String username = acceptDeletionDetails();
        if (staffs[currentStaffI].getName().equals(username)) {
            System.out.println();
            System.out.println("Cannot delete yourself! Please try again. Press enter key to continue.");
            console().readPassword(); // to receive input without echo
            return;
        }
        final int staffI = getStaffI(username);
        if (staffI == -1) {
            System.out.println();
            System.out.println("The staff to be deleted does not exist! Please try again. Press enter key to continue.");
            console().readPassword(); // to receive input without echo
            return;
        }
        if (!acceptDeletionConfirmation().equals(staffs[staffI].getName())) {
            System.out.println();
            System.out.println("You cancelled the deletion. Press enter key to continue.");
            console().readPassword(); // to receive input without echo
            return;
        }
        staffs[staffI] = null;
        nullIndices.push(staffI);
        System.out.println();
        System.out.println("Successfully delete the staff. Press enter key to continue.");
        console().readPassword(); // to receive input without echo
    }
    
    /**
     * @return username whose staff is to be deleted
     */
    private static String acceptDeletionDetails() {
        System.out.println();
        System.out.print("Username: ");
        return console().readLine();
    }
    
    private static String acceptDeletionConfirmation() {
        System.out.println();
        System.out.print("Are you sure you really want to delete it? Retype staff username to confirm or else cancel > ");
        return console().readLine();
    }
    
    void show() {
        System.out.println();
        switch (staffs[currentStaffI].getPosition()) {
            case MANAGER: showAll();
                break;
            case CLERK: showCurrent();
        }
        System.out.println("Press enter key to continue.");
        console().readPassword(); // to receive input without echo
    }
    
    private void showAll() {
        for (final Staff staff : staffs) {
            if (staff == null) continue;
            staff.show();
            System.out.println();
        }
    }
    
    private void showCurrent() {
        staffs[currentStaffI].show();
        System.out.println();
    }
}