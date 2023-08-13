
import java.text.DecimalFormat;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.*;

public class MemberModule{
    private final ArrayList<Member> list;//initialize arraylist object
    //initialize Dictionary of for gender and status
    Dictionary<Character, String> genderDic = new Hashtable<>();
    Dictionary <String,Boolean> statusDic = new Hashtable<>();
    //initialize DecimalFormat with 000
    DecimalFormat df = new DecimalFormat("000");
    Scanner input = new Scanner(System.in);
    //get the date format in dd-MM-yyyy
    SimpleDateFormat format = new SimpleDateFormat("dd-MM-yyyy");
    MemberModule() throws ParseException {
        list = new ArrayList<>();
        Date dob1 = format.parse("12-12-2012");//parse string into date
        Date dob2 = format.parse("13-1-2013");
        //add data into the arraylist 
        list.add(new Member("Chuah Jing Quan", dob1, 18, 'M', "019-123456", "Pulau Pinang", "Gold",  dob1, true, expireDateControl(dob1)));
        list.add(new Member("Chee Kexin", dob2, 12, 'F', "012-123456", "Pulau Pinang", "Diamond", dob2, false, expireDateControl(dob2)));
        list.add(new Member("Doraemon", dob1, 31, 'M', "017-1645545","Sarawak","Platinum", dob1, true, expireDateControl(dob1)));
        list.add(new Member("Mr.spider man", dob2, 20, 'M', "012-4654654", "Perak", "Diamond", dob2, false, expireDateControl(dob2)));
        list.add(new Member("Ms.Thursday", dob1, 21, 'M', "017-44444411", "Terangganu", "Gold", dob1, true, expireDateControl(dob1)));
    }
    public Date expireDateControl(Date date){
        Calendar c = Calendar.getInstance();//get instance of calendar
        c.setTime(date);
        c.add(Calendar.YEAR,1);//add a year after date to get expDate
        Date expDate = c.getTime();
        return expDate;
    }
    public void processMainMenu(){
        //adds key-value pair into both gender and status dictionaries
        genderDic.put('M',"male");
        genderDic.put('F',"female");
        statusDic.put("active",true);
        statusDic.put("inactive",false);
        boolean isContinue = true;
        do {//continue the looping if the isContinue is true
            printMemberMenu();
            //shift to InputValidation class for the input Scanner
            int option = InputValidation.getIntegerInput("Enter option:");
            switch (option) {//prompt user to choose an option
                case 1 -> addMember();
                case 2 -> modifyMember();
                case 3 -> deleteMember();
                case 4 -> displayMember();
                case 5 -> showViewCriteria();
                case 6 -> showDesireRecord();
                case 7 -> {
                    System.out.println("Latest Information saved successfully, See you again");
                    //save to the file shown inside FileManagement class
                    FileManagement.writeFile(this.list);
                    isContinue = false;//stop the program
                }
                default -> {
                    System.out.println("Invalid option,Please try again");
                    isContinue = true;
                }
            }
        } while (isContinue);
    }
    /*this section is to shows the user member record according to different criteria. 
    stack has been used to push the member list and shows the corresponding members*/
    public void showDesireRecord(){
        boolean isRepeat = true;
        String response = "";
        boolean isValid = true;
        do {
            try {
                System.out.println("1. Sorted by member name.");
                System.out.println("2. Sorted by date of registration as member.");
                System.out.println("3. Search member’s identity card number.");
                System.out.println("4. Search member’s name.");
                System.out.println("5. Based on status of member (active or inactive).");
                System.out.println("6. Based on member level.");
                System.out.println("7. Back to main menu");
                int option = InputValidation.getIntegerInput("Enter your option >");
                switch (option){
                    case 1:
                        insertionSort("memberName");
                        break;
                    case 2:
                        bubbleSort("registerDate");
                        break;
                    case 3:
                        String memberID;
                        int length;
                        do {
                            memberID = InputValidation.getStringInput("Enter member ID >");
                            length = memberID.length();
                            if(length<2 ){
                                System.out.println("Member ID must at least 2 character");
                            }else if(memberID.toUpperCase().charAt(0) != 'M'){
                                System.out.println("Member ID must start with M");
                            }
                        }while (length<2 || memberID.toUpperCase().charAt(0) != 'M');
                        int convertID = Integer.parseInt(memberID.substring(1));
                        int indexID = binarySearchID(list, convertID,0,list.size()-1);
                        if(indexID != -1){
                            System.out.println(list.get(indexID).toString());
                            input.nextLine();
                        }else {
                            System.out.println("Record not found :(");
                            input.nextLine();
                            isRepeat = true;
                        }
                        break;
                    case 4:
                        final int MAX = list.size();
                        Member current;
                        int i,j;
                        String memberName;
                        memberName = InputValidation.getStringInput("Enter member name >");
                        for(i=1; i<MAX; i++){
                            current = list.get(i);
                            j=i;
                            while((j>0)&&(list.get(j-1).getName().compareTo(current.getName()))>0){
                                list.set(j,list.get(j-1));
                                j--;
                                list.set(j,current);
                            }
                        }
                        double indexName = binarySearchName(list, memberName.toLowerCase());
                        if(indexName != -1){
                            System.out.println(list.get((int) indexName).toString());
                            input.nextLine();
                        }else {
                            System.out.println("Record not found :(");
                            input.nextLine();
                            isRepeat = true;
                        }
                        break;
                    case 5:
                        do {
                            response = InputValidation.getStringInput("Enter Member status to show (active/inactive) >");
                            if(response.equalsIgnoreCase("active") || response.equalsIgnoreCase("inactive"))
                                isValid = false;
                        }while(isValid);
                        System.out.println("--------------------------------------------------");
                        System.out.println("Index |  Member ID |    Name    ");
                        boolean status = response.equalsIgnoreCase("active");
                        for (Member member : list) {
                            if (member.getStatus() == status) {
                                int holder = Integer.parseInt(member.getId().substring(1));
                                if (holder < 100) {//if member id<100 shows the character with 'M', else shows the plain member id
                                    System.out.printf("%-5d | %7s    | %-10s  \n", list.indexOf(member) + 1, "M" + df.format(holder), member.getName());
                                }
                                else {
                                    System.out.printf("%-5d | %7s    | %-10s  \n", list.indexOf(member) + 1, member.getId(), member.getName());
                                }
                            }
                        }
                        System.out.println("--------------------------------------------------");
                        input.nextLine();
                        break;
                    case 6:
                        do {
                            response = InputValidation.getStringInput("Enter Member level >");
                            if(response.equalsIgnoreCase("diamond") || response.equalsIgnoreCase("platinum") || response.equalsIgnoreCase("gold"))
                                isValid = false;
                        }while(isValid);
                        System.out.println("--------------------------------------------------");
                        System.out.println("Index |  Member ID |    Name    ");
                        for (Member member : list) {
                            if (member.getLevel().equalsIgnoreCase(response)) {
                                int holder = Integer.parseInt(member.getId().substring(1));
                                if (holder < 100) {
                                    System.out.printf("%-5d | %7s    | %-10s  \n", list.indexOf(member) + 1, "M" + df.format(holder), member.getName());
                                } else {
                                    System.out.printf("%-5d | %7s    | %-10s  \n", list.indexOf(member) + 1, member.getId(), member.getName());
                                }
                            }
                        }
                        System.out.println("--------------------------------------------------");
                        input.nextLine();
                        break;
                    case 7 :
                        isRepeat = false;
                        break;
                    default:
                        System.out.println("Invalid Input, Please Try again.");
                        input.nextLine();
                }
            }catch (Exception e){
                //System.out.println("An unexpected error has occur, please try again");
                System.err.println(e.getMessage());
                input.nextLine();
                isRepeat = true;
            }
        }while (isRepeat);
    }
    public int binarySearchID(ArrayList<Member> list, int target,int low,int high){
        int middle;
        while (low<=high){
            middle = (low + high)/2;
            int current =Integer.parseInt(list.get(middle).getId().substring(1));
            if(target == current){
                return middle;
            }else if(target<current){
                high = middle -1 ;
            }else if(target>current){
                low = middle + 1 ;
            }
        }
        return -1;
    }
    public double binarySearchName(ArrayList<Member> list, String key){
        double middle;
        int low=0, high=list.size()-1;
        while(low<=high){
            middle = Math.floor((low+high)/2.0);
            System.out.println(middle);
            input.nextLine();
            int res = key.compareToIgnoreCase(list.get((int) Math.abs(middle)).getName());
            if(res == 0){
                return middle;
            } else if(res>0){
                low = (int) middle+1;
            } else{
                high =(int) middle-1;
            }
        }
        return -1;
    }
    public void bubbleSort(String type) {
        ArrayList <Member> sortList = (ArrayList <Member>)list.clone();
        final int MAX = sortList.size();
        Member current;
        int i,j;
        if(type.equals("registerDate")){
            for(i=0; i<MAX;i++) {
                for (j=0; j<MAX-1; j++) {
                    if(sortList.get(j).getRegister().after(sortList.get(j+1).getRegister())) {
                        current = sortList.get(j);
                        sortList.set(j,sortList.get(j+1));
                        sortList.set(j+1,current);
                    }
                }
            }
            System.out.println("-------------------------------------------------------------");
            System.out.println("Index |  Member ID |      Name      |    Register Date");
            for (Member member : sortList) {
                int holder = Integer.parseInt(member.getId().substring(1));
                if (holder < 100) {//if member id<100 shows the character with 'M', else shows the plain member id
                    System.out.printf("%-5d | %7s    | %-10s  | %6td-%<tm-%<tY\n", sortList.indexOf(member) + 1, "M" + df.format(holder), member.getName(),member.getRegister());
                }
                else {
                    System.out.printf("%-5d | %7s    | %-10s  | %6td-%<tm-%<tY\n", sortList.indexOf(member) + 1, member.getId(), member.getName(),member.getRegister());
                }
            }
            System.out.println("-------------------------------------------------------------");
        }
    }
    public void insertionSort(String type) {
        ArrayList <Member> sortList = (ArrayList <Member>)list.clone();
        final int MAX = sortList.size();
        Member current;
        int i,j;
        if(type.equals("memberName")){
            for(i=1; i<MAX; i++){
                current = sortList.get(i);
                j=i;
                while((j>0)&&(sortList.get(j-1).getName().compareTo(current.getName()))>0){
                    sortList.set(j,sortList.get(j-1));
                    j--;
                    sortList.set(j,current);
                }
            }
            System.out.println("-------------------------------------------------------------");
            System.out.println("Index |  Member ID |      Name");
            for (Member member : sortList) {
                int holder = Integer.parseInt(member.getId().substring(1));
                if (holder < 100) {//if member id<100 shows the character with 'M', else shows the plain member id
                    System.out.printf("%-5d | %7s    | %-10s\n", sortList.indexOf(member) + 1, "M" + df.format(holder), member.getName());
                }
                else {
                    System.out.printf("%-5d | %7s    | %-10s\n", sortList.indexOf(member) + 1, member.getId(), member.getName());
                }
            }
        }
    }

    public void showViewCriteria(){
        boolean isValid = false;
        Stack<Integer> stack = new Stack<>();//initialize Stack
        while (!isValid) {
            System.out.println("1. show active member record");
            System.out.println("2. show member age over 18 year old of record");
            System.out.println("3. show record sorted by Member ID");
            System.out.println("4. return main menu");
            //shift to InputValidation class for the input Scanner
            int option = InputValidation.getIntegerInput("Enter option >");
            switch (option) {
                case 1:
                    int placer;
                    do {
                        stack.clear();
                        System.out.println("--------------------------------------------------");
                        System.out.println("Index |  Member ID |    Name    ");
                        for (Member member : list) {
                            if (member.getStatus()) {
                                //change the id from string to integer and get the numbers after 1 character
                                int holder = Integer.parseInt(member.getId().substring(1));
                                if (holder < 100) {//if member id<100 shows the character with 'M', else shows the plain member id
                                    System.out.printf("%-5d | %7s    | %-10s  \n", list.indexOf(member) + 1, "M" + df.format(holder), member.getName());
                                }
                                else {
                                    System.out.printf("%-5d | %7s    | %-10s  \n", list.indexOf(member) + 1, member.getId(), member.getName());
                                }
                                //push the list of the member that are active
                                stack.push(list.indexOf(member) + 1);
                            }
                        }
                        if (stack.isEmpty()){
                            System.out.println("sry no record found :(");
                            input.nextLine();
                            placer = -1;
                        }else {
                            placer = InputValidation.getIntegerInput("Enter an index number to show further record detail (-1 to back menu) >");
                            if(!stack.contains(placer) && !stack.isEmpty() && placer != -1){
                                System.out.println("Invalid index number, Please try again");
                                input.nextLine();
                            }
                            else{
                                while(!stack.isEmpty()){
                                    int compare = stack.pop();
                                    if(compare == placer){
                                        System.out.println(list.get(placer-1).toString());
                                        input.nextLine();
                                    }
                                }
                            }
                        }
                    }while(placer!=-1);
                    break;
                case 2:
                    int answer;
                    do {
                        stack.clear();
                        System.out.println("--------------------------------------------------");
                        System.out.println("Index |  Member ID |    Name    ");
                        for (Member member : list) {
                            if (member.getAge()>18) {
                                //parse id from string to integer and get the numbers after 1 character
                                int holder = Integer.parseInt(member.getId().substring(1));
                                if (holder < 100) {
                                    System.out.printf("%-5d | %7s    | %-10s  \n", list.indexOf(member) + 1, "M" + df.format(holder), member.getName());
                                }
                                else {
                                    System.out.printf("%-5d | %7s    | %-10s  \n", list.indexOf(member) + 1, member.getId(), member.getName());
                                }
                                //push the list of the member that are over 18 years old
                                stack.push(list.indexOf(member) + 1);
                            }
                        }
                        if (stack.isEmpty()){
                            System.out.println("sry no record found :(");
                            input.nextLine();
                            answer = -1;
                        }else {
                            answer = InputValidation.getIntegerInput("Enter an index number to show further record detail (-1 to back menu) >");
                            if(!stack.contains(answer) && !stack.empty() && answer!=-1){
                                System.out.println("Invalid index number, Please try again");
                                input.nextLine();
                            }else{
                                while (!stack.isEmpty()){
                                    int compare = stack.pop();
                                    if (compare == answer){
                                        System.out.println(list.get(answer-1).toString());
                                        input.nextLine();
                                    }
                                }
                            }
                        }
                    }while(answer!=-1);
                    break;
                case 3:
                    ArrayList <Member> sortedList;
                    //create sorted list and copy another linked list called list
                    sortedList = (ArrayList <Member>)list.clone();
                    sortedList.sort(new Comparator<>() {
                        @Override
                        public int compare(Member o1, Member o2) {
                            int holder = (int) o1.getId().charAt(0) * 100000;
                            int holder2 = (int) o2.getId().charAt(0) * 100000;
                            int placer = Integer.parseInt(o1.getId().substring(1));
                            int placer2 = Integer.parseInt(o2.getId().substring(1));
                            return (holder + placer) - (holder2 + placer2);
                        }
                    });
                    System.out.println("--------------------------------------------------");
                    System.out.println("Index |  Member ID |    Name    ");
                    for (Member member : sortedList) {
                        //parse id from string to integer and get the numbers after 1 character
                        int holder = Integer.parseInt(member.getId().substring(1));
                        if (holder < 100) {
                            System.out.printf("%-5d | %7s    | %-10s  \n", sortedList.indexOf(member) + 1, "M" + df.format(holder), member.getName());
                        }
                        else {
                            System.out.printf("%-5d | %7s    | %-10s  \n", sortedList.indexOf(member) + 1, member.getId(), member.getName());
                        }
                    }
                    break;
                case 4:
                    isValid = true;
                    break;
                default:
                    isValid = false;
                    break;
            }
        }
    }
    public MemberModule(ArrayList<Member> list){this.list = list;}
    /*this section is to add member to the member list. User has to enter the details of the member before adding
    to the list for registration*/
    public void addMember(){
        boolean isContinue = false;
        do {
            try {
                //shift to InputValidation class for the input Scanner to add in object with different data type
                String name = InputValidation.getStringInput("Enter Name >");
                Date dob = InputValidation.getDateInput("Enter Date Of Birth >");
                int age = InputValidation.getIntegerInput("Enter age >");
                char gender = Character.toUpperCase(InputValidation.getCharInput("Enter gender (M/F) >"));
                String phone = InputValidation.getStringInput("Enter phone number >");
                String address = InputValidation.getStringInput("Enter address >");
                String level = InputValidation.getStringInput("Enter level >");
                Date register = InputValidation.getDateInput("Enter Register Date >");
                /*repeats if age is negative value or gender cannot be found in dictionary 
                or membership level is not correct*/
                while (age < 0 || genderDic.get(gender) == null || !Arrays.asList(Member.memberLevel).contains(level)){
                    System.out.println("--------------------------------------------------------------");
                    System.out.println("Invalid information detected :( Please correct below information");
                    if(age < 0){
                        System.out.println("age cannot less than 0");
                        age = InputValidation.getIntegerInput("Enter age >");
                    /*if the memberLevel of Member in list view of array does not contains the input of level 
                    which is in small lettercase, prompt the user key in again*/
                    }else if(!Arrays.asList(Member.memberLevel).contains(level.toLowerCase())){
                        System.out.println("member level must be fill in (gold/platinum/diamond)");
                        level = InputValidation.getStringInput("Enter member level >");
                    }else{
                        System.out.println("gender invalid, please try again");
                        gender = InputValidation.getCharInput("Enter gender >");
                    }
                }//ignore the big or small letter if meets equalsIgnoreCase
                if(level.equalsIgnoreCase("gold")){
                    System.out.println("Register Fee is 120");
                }else if(level.equalsIgnoreCase("platinum")){
                    System.out.println("Register Fee is 180");
                }else {
                    System.out.println("Register Fee is 250");
                }
                char confirm = InputValidation.getCharInput("Confirm register? (Y/N)>");
                if(Character.toUpperCase(confirm) == 'Y'){
                    char option = InputValidation.getCharInput("Record add at the end? (Y/N) >");
                    if(option == 'N' || option == 'n'){
                        boolean isValid = false;
                        while (!isValid){//loop if isValid is true
                            for(int i = 0; i < list.size(); i++) {
                                int holder = Integer.parseInt(list.get(i).getId().substring(1));
                                //print the member in list with condition holder 
                                if (holder <100){
                                    System.out.printf("%-5d | %7s    | %-10s  \n", i + 1, "M" + df.format(holder), list.get(i).getName());
                                }else{
                                    System.out.printf("%-5d | %7s    | %-10s  \n", i + 1, list.get(i).getId(), list.get(i).getName());
                                }
                            }
                            int placer = InputValidation.getIntegerInput("Enter Index number to place this record >");
                            if(placer-1 > list.size() || placer-1 < 0){//if the input number is bigger than the list size or is negative value
                                System.out.printf("Invalid Index number,Please in range 1-%d\n",list.size()+1);
                                isValid = false;
                            }else{//add the new member into list after enter an index number
                                list.add(placer-1,new Member(name, dob, age, gender, phone, address, level, register, true, expireDateControl(register)));
                                isValid = true;
                            }
                        }
                    }else {//option is not N, so the new member will be added to behind in the list according to the index number
                        list.add(new Member(name, dob, age, gender, phone, address, level, register, true, expireDateControl(register)));
                    }
                    System.out.println("Record add Successfully");
                }else {
                    System.out.println("Record has been denial");
                    isContinue = false;
                }
            }catch (AssertionError e){
                System.out.println(e.getMessage());
                isContinue = true;
            }catch (Exception e){
                System.out.println("An Unexpected Error has Occur");
                isContinue = true;
            }
        }while(isContinue);
    }
    public int printRecordList(){
        int size = list.size();
        System.out.println("--------------------------------------------------");
        System.out.println("Index |  Member ID |    Name    ");
        for(int i = 0; i < size; i++) {
            //parse id in list from string to integer and get the numbers after 1 character
            int holder = Integer.parseInt(list.get(i).getId().substring(1));
            if (holder <100){
                System.out.printf("%-5d | %7s    | %-10s  \n", i + 1,list.get(i).getId().charAt(0)+ df.format(holder), list.get(i).getName());
            }else{
                System.out.printf("%-5d | %7s    | %-10s  \n", i + 1, list.get(i).getId(), list.get(i).getName());
            }
        }
        System.out.println("--------------------------------------------------");
        System.out.printf("Press %d to return menu\n",size+1);
        return size;
    }
    /*this section is about modify the member list. User can try to either modify the details of the members or changing the 
    position of the members*/
    public void modifyMember(){
        boolean isContinue = true;
        boolean innerLoop = true;
        do {
            final int SIZE = printRecordList();
            //ask user to choose an option
            System.out.printf("Press %d to exchange record position\n",SIZE + 2);
            System.out.println("Enter an Index number proceed");
            int option = InputValidation.getIntegerInput("Index number >");
            //if option is between the number showing the index number of member
            if(option > 0 && option < SIZE + 1){
                Member ptr = list.get(option-1);
                do {
                    try {
                        //print the relevant member according to the option user choose
                        System.out.println(ptr.toString());
                        System.out.println("Press any key to continue");
                        input.nextLine();
                        printModifyMenu();
                        int response = InputValidation.getIntegerInput("Enter your option >");
                        switch (response) {//ask the user to enter an option to modify the record they want
                            case 1 -> {
                                String id = InputValidation.getStringInput("Enter new ID >");
                                for (Member member : list)
                                    if (id.equals(member.getId()))//if the new id is same with the old or others id, throws error
                                        throw new AssertionError("having record with same id or value same with older record, please try again ");
                                ptr.setId(id);//set the id 
                                System.out.println("Record modify Successfully");
                                input.nextLine();
                            }
                            case 2 -> {
                                String name = InputValidation.getStringInput("Enter new name >");
                                if (name.equals(ptr.getName()))//throws error if meets the same name
                                    throw new AssertionError("name same with older record, Please try again");
                                ptr.setName(name);//set the name
                                System.out.println("Record modify Successfully");
                                input.nextLine();
                            }
                            case 3 -> {
                                Date dob = InputValidation.getDateInput("Enter new date of birth >");
                                if (dob.equals(ptr.getDate()))//if same date throws error
                                    throw new AssertionError("date of birth same with older record, Please try again");
                                ptr.setDate(dob);
                                System.out.println("Record modify Successfully");
                                input.nextLine();
                            }
                            case 4 -> {
                                int age = InputValidation.getIntegerInput("Enter new age >");
                                if (age == ptr.getAge() || age < 0)//if negative value or same with other record 
                                    throw new AssertionError("age same with older record or age entry less than 0, Please try again");
                                ptr.setAge(age);
                                System.out.println("Record modify Successfully");
                                input.nextLine();
                            }
                            case 5 -> {
                                char gender = Character.toUpperCase(InputValidation.getCharInput("Enter new gender (M/F) >"));
                                //if gender is same with older record or cannot be found in dictionary
                                if (gender == ptr.getGender() || genderDic.get(gender) == null)
                                    throw new AssertionError("gender same with older record or gender entry not M and F, Please try again");
                                ptr.setGender(gender);
                                System.out.println("Record modify Successfully");
                                input.nextLine();
                            }
                            case 6 -> {
                                String phone = InputValidation.getStringInput("Enter new contact >");
                                if (phone.equals(ptr.getPhone()))//throws error if same with older record
                                    throw new AssertionError("phone number same with older record, Please try again");
                                ptr.setPhone(phone);
                                System.out.println("Record modify Successfully");
                                input.nextLine();
                            }
                            case 7 -> {
                                String address = InputValidation.getStringInput("Enter new address >");
                                if (address.equals(ptr.getAddress()))//throws error if same with older record
                                    throw new AssertionError("Address same with previous record, Please try again");
                                ptr.setAddress(address);
                                System.out.println("Record modify Successfully");
                                input.nextLine();
                            }
                            case 8 -> {
                                String level = InputValidation.getStringInput("Enter new membership level (gold,platinum,diamond) >");
                                if (level.equals(ptr.getLevel()) || !Arrays.asList(Member.memberLevel).contains(level))//same or cant be found in array
                                    throw new AssertionError("level same with older record or level input not (gold,platinum,diamond), Please try again)");
                                ptr.setLevel(level);
                                System.out.println("Record modify Successfully >");
                                input.nextLine();
                            }
                            case 9 -> {
                                Date register = InputValidation.getDateInput("Enter new date of joining >");
                                if (register.equals(ptr.getRegister()))//throws error if same with older record
                                    throw new AssertionError("register date same with older record, Please try again");
                                ptr.setRegister(register);
                                System.out.println("Record modify Successfully");
                                input.nextLine();
                            }
                            case 10 -> {
                                boolean result = true;
                                String status = InputValidation.getStringInput("Enter new status (activate/inactive) >").toLowerCase();
                                if (status.equals("inactive")) {
                                    result = false;
                                }
                                //throws error if same with older record or wrong input
                                if (result == ptr.getStatus() || statusDic.get(status.toLowerCase()) == null)
                                    throw new AssertionError("status same with older record or input not (active/inactive), Please try again");
                                ptr.setStatus(result);
                                System.out.println("Record modify Successfully");
                                input.nextLine();
                            }
                            case 11 -> {//modify membership level will need to renew level by paying the new fees
                                System.out.printf("Your current member level is %s", ptr.getLevel());
                                if (ptr.getLevel().equalsIgnoreCase("gold")) {
                                    System.out.println("Must paid 80, in order to renew membership");
                                } else if (ptr.getLevel().equalsIgnoreCase("platinum")) {
                                    System.out.println("Must paid 150, in order to renew membership");
                                } else {
                                    System.out.println("Must paid 200, in order to renew membership");
                                }
                                char proceed = InputValidation.getCharInput("Confirm renew (Y/N) ?");
                                switch (proceed) {
                                    case 'Y', 'y' -> {//if confirm to renew, register and will set the expire date 
                                        ptr.setExpireDate(expireDateControl(ptr.getRegister()));
                                        System.out.println("Record modify Successfully");
                                        input.nextLine();
                                    }
                                    case 'N', 'n' -> System.out.println("Renew member has been denial");
                                }
                            }
                            case 12 -> innerLoop = false;
                            default -> {
                                System.out.println("Please enter number 1 to 12 to proceed next step");
                                input.nextLine();
                            }
                        }
                    }catch (AssertionError e){
                        System.out.println("-----------------   incorrect information detected   ---------------------");
                        System.out.println(e.getMessage());
                        System.out.println("Press any key to continue");//press key and back to inner loop
                        input.nextLine();
                        innerLoop = true;
                    }catch (Exception e){
                        System.out.println("An Unexpected Error Has Occur");
                        innerLoop = true;
                    }
                }while (innerLoop);//continue looping when innerLoop is true
            }else if (option == SIZE + 1){
                isContinue = false;
            }else if(option == SIZE + 2){
                boolean isValid = false;
                while (!isValid){
                    try {
                        System.out.println("--------------------------------------------------");
                        System.out.println("Index |  Member ID |    Name    ");
                        for(int i = 0; i < SIZE; i++) {
                            int holder = Integer.parseInt(list.get(i).getId().substring(1));
                            if (holder <100){
                                System.out.printf("%-5d | %7s    | %-10s  \n", i + 1, "M" + df.format(holder), list.get(i).getName());
                            }else{System.out.printf("%-5d | %7s    | %-10s  \n", i + 1, list.get(i).getId(), list.get(i).getName());
                            }
                        }
                        System.out.println("--------------------------------------------------");
                        System.out.println("Enter 2 record of index number to do the exchange");
                        int first = InputValidation.getIntegerInput("first record >");
                        int second = InputValidation.getIntegerInput("second record >");
                        Collections.swap(list,first-1,second-1);//swap the elements which is in the list
                        isValid = true;
                    }catch (IndexOutOfBoundsException e){
                        System.out.println("Invalid index number, Please try again");
                        isValid = false;
                    }catch (Exception e){
                        System.out.println("An Unexpected has error Occur");
                        isValid = false;
                    }
                }
            }else {
                System.out.println("Invalid Input, Please Try Again");
            }
        }while (isContinue);
    }
    //this section is to shows the current member list
    public void displayMember(){
        boolean isContinue = true;
        do {
            final int SIZE = printRecordList();
            System.out.println("Enter Index number to show further information");
            int option = InputValidation.getIntegerInput("Enter your option >");
            if(option > 0 && option < SIZE + 1){
                Member member = list.get(option-1);//shows the list of the option user entered
                System.out.println(member.toString());
                System.out.println("Press any key to continue");
                input.nextLine();
            }else if (option == SIZE + 1){
                isContinue = false;
            }else {
                System.out.println("Invalid Input, Please Try Again");
            }
        }while(isContinue);
    }
    //this section is to delete members. User will be ask to enter an index number to delete the corresponding member.
    public void deleteMember(){
        boolean isContinue = true;
        do {
            int size = printRecordList();
            System.out.println("Enter Index number to delete member record");
            int option = InputValidation.getIntegerInput("Enter your option >");
            if(option > 0 && option < size + 1){
                Member member = list.get(option-1);//shows the list of the option user entered
                System.out.println(member.toString());
                char answer = InputValidation.getCharInput("Sure delete member (Y/N) ?>");
                if(answer == 'y' || answer == 'Y'){
                    list.remove(option-1);//delete the member chosen
                }else if(answer == 'n' || answer == 'N'){
                    isContinue = false;
                }else {
                    System.out.println("Invalid Input, record is reserve, Please try again");
                    isContinue = true;
                }
            }else if (option == size + 1){
                isContinue = false;
            }else {
                System.out.println("Invalid Input, Please Try Again");
            }
        }while(isContinue);
    }
    public void printMemberMenu(){
        System.out.println("Welcome to Member Management");
        System.out.println("1.Add member");
        System.out.println("2.Modify member");
        System.out.println("3.Delete member");
        System.out.println("4.Display member");
        System.out.println("5.Show record by criteria");
        System.out.println("6.Show desire record");
        System.out.println("7.Exit");
    }
    private static void printModifyMenu() {
        System.out.println("Select Member detail that you want to modify:");
        System.out.println("----------------------------------------");
        System.out.println("1.Member ID");
        System.out.println("2.Name");
        System.out.println("3.Date of birth");
        System.out.println("4.Age");
        System.out.println("5.Gender");
        System.out.println("6.Contact");
        System.out.println("7.Address");
        System.out.println("8.Membership level");
        System.out.println("9.Date of joining");
        System.out.println("10.Status");
        System.out.println("11.Expiry date/Renew member");
        System.out.println("12.Back Record Menu");
        System.out.println("----------------------------------------");
    }
}