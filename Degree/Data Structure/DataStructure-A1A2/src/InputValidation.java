import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.InputMismatchException;
import java.util.Scanner;
import java.util.Date;
/*this class is for scanner purpose and has been distributed into several functions
for different data types usage. Besides, try catch statement also has been used  
in case of any java exceptions occur*/
public class InputValidation {
    public static String getStringInput(String prompt){
        String placer = "";
        boolean isValid = false;
        Scanner input = new Scanner(System.in);
        do {
            try {
                System.out.print(prompt);
                placer = input.nextLine().trim();//eliminates the trailing spaces
                if (placer.equals(""))
                    throw new InputMismatchException();
                isValid = true;
            }catch (InputMismatchException e){
                System.out.println("empty field detected, please try again");
                isValid = false;
            }catch (Exception e){
                System.out.println("Unexpected Error Occur");
                isValid = false;
            }
        }while(!isValid);
        return placer;
    }
    public static double getDoubleInput(String prompt){
        double placer = 0.0;
        boolean isValid = false;
        Scanner input = new Scanner(System.in);
        while(!isValid){
            try {
                System.out.println(prompt);
                placer = Double.parseDouble(input.nextLine().trim());
                isValid = true;
            }catch (InputMismatchException|NumberFormatException e){
                System.out.println("Invalid Input, Please Try Again");
                isValid = false;
            }catch (Exception e){
                System.out.println("Unexpected Error Has Occur");
                isValid = false;
            }
        }
        placer = Double.parseDouble(String.format("%.2f",placer));
        return placer;
    }
    public static int getIntegerInput(String prompt){
        int placer = 0;
        boolean isValid=true;
        Scanner input = new Scanner(System.in);
        do {
            try {
                System.out.print(prompt);
                placer = Integer.parseInt(input.nextLine().trim());
                isValid = true;
            }catch (InputMismatchException | NumberFormatException e){
                System.out.println("Invalid Input, Please Try Again");
                isValid = false;
            }catch (Exception e){
                System.out.println("An Unexpected Error Has Occur");
                isValid = false;
            }
        }while(!isValid);
        return placer;
    }
    public static char getCharInput(String prompt){
        Scanner input = new Scanner(System.in);
        char placer = '\u0000';
        boolean isValid = false;
        while (!isValid){
            try {
                System.out.print(prompt);
                placer = input.nextLine().trim().charAt(0);
                isValid = true;
            }catch (InputMismatchException | StringIndexOutOfBoundsException e){
                System.out.println("Invalid Input,Please Try Again");
                isValid = false;
            }catch (Exception e){
                System.out.println("Unexpected Error Has Occur");
                isValid = false;
            }
        }
        return placer;
    }
    public static Date getDateInput(String prompt){
        Scanner input = new Scanner(System.in);
        String date="";
        Date javaDate=null;
        boolean isValid;
        SimpleDateFormat format = new SimpleDateFormat("dd-MM-yyyy");
        format.setLenient(false);
        do{
            try {
                System.out.print(prompt);
                date = input.nextLine().trim();
                javaDate = format.parse(date);
                isValid = true;
            }catch (ParseException e){
                System.out.println("Invalid date or format,Please enter valid date");
                isValid = false;
            }catch (Exception e){
                System.out.println("An unexpected error has occur");
                isValid = false;
            }
        }while (!isValid);
        return javaDate;
    }
}
