package aple.pos.stock;

import java.util.InputMismatchException;
import java.util.Scanner;

public class InputValidation {
    public static String getStringInput(String prompt){
        boolean isValid = false;
        String input = "";
        Scanner sc = new Scanner(System.in);
        do{
            try{
                System.out.print(prompt);
                input = sc.nextLine().trim();
                System.out.println();
                if(input.equals(""))
                    throw new InputMismatchException();
                else
                    isValid = true;
            }catch(InputMismatchException e ){
                System.out.println("Invalid input, Please try again");
                isValid = false;
            }catch(Exception e){
                System.out.println("An Unexpected Error occur,Please try again");
                isValid = false;
            }

        }while(!isValid);

        return input;
    }

    public static float getFloatInput(String prompt) {
        boolean isValid = false;
        float input;
        float stringInput = 0.0f;
        Scanner sc = new Scanner(System.in);

        while(!isValid){
            try{
                System.out.print(prompt);
                stringInput = Float.parseFloat(sc.nextLine().trim());
                System.out.println();
                isValid = true;
            }catch (InputMismatchException | NumberFormatException e){
                System.out.println("Invalid input,Please try again");
                isValid = false;
            }catch(Exception e){
                System.out.println("Unexpected Error is occur,Please try again");
                isValid = false;
            }
        }
        input = Float.parseFloat(String.format("%.2f", stringInput));
        return input;
    }

    public static int getIntegerInput(String prompt){
        boolean isValid= false;
        int input = 0;
        Scanner sc = new Scanner(System.in);

        while(!isValid){
            try{
                System.out.print(prompt);
                input = Integer.parseInt(sc.nextLine().trim());
                System.out.println();
                isValid = true;
            }catch(InputMismatchException | NumberFormatException e){
                System.out.println("Invalid input,Please try again");
                isValid = false;
            }catch(Exception e){
                System.out.println("Unexpected is occur,Please try again");
                isValid = false;
            }
        }
        return input;
    }

    public static char getCharOption(){
        Scanner sc = new Scanner(System.in);
        boolean isValid = false;
        char input = '\u0000';

        while(!isValid){
            try {
                System.out.print("Option:");
                input = sc.nextLine().trim().charAt(0);
                isValid = true;
            }
            catch(InputMismatchException | StringIndexOutOfBoundsException e){
                System.out.println("Invalid input, please try again!");
                isValid = false;
            }
            catch(Exception e){
                System.out.println("Oops! An unexpected error occur during the process. "+e.getMessage());
                isValid = false;
            }
        }
        return input;
    }
}
