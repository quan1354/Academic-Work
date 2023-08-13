import java.io.Serializable;
import java.text.SimpleDateFormat;
import java.util.Arrays;
import java.util.Date;
import java.util.Random;
//ensure that state of instances of class Member can be saved in a file.
public class Member implements Serializable {
    //initialize all the attributes
    SimpleDateFormat dateFormat = new SimpleDateFormat("dd-MM-yyyy");
    public static String[] memberLevel = {"gold","platinum","diamond"};
    private final char[] genderContainer = {'m', 'f'};
    final static String IDFORMAT = "M";
    private String id;
    private String name;
    private Date dob;
    private int age;
    private char gender;
    private String phoneNum;
    private String address;
    private String level;
    private Date register;
    private boolean status;
    private Date expireDate;
    private static int totalRecord;
    public Member(String name,Date date,int age,char gender,String phone,
        String address,String level,Date register,boolean status,Date expireDate){
        this.name = name;
        this.dob = date;
        this.age = Math.max(age, 0);
        this.gender = gender;
        this.phoneNum = phone;
        this.address = address;
        this.level = level;
        this.register = register;
        this.status = status;
        this.expireDate = expireDate;
        totalRecord++;
        this.id = IDFORMAT + totalRecord;
    }
    //accessor methods
    public String getId() {return id;}
    public String getName(){return name;}
    public Date getDate(){return dob;}
    public int getAge(){return age;}
    public char getGender(){return gender;}
    public String getPhone(){return phoneNum;}
    public String getAddress(){return address;}
    public String getLevel(){return level;}
    public Date getRegister(){return register;}
    public boolean getStatus(){return status;}
    public Date getExpireDate(){return expireDate;}
    public void setId(String id){this.id = id;}
    //mutator methods
    public void setName(String name){this.name = name;}
    public void setDate(Date date){this.dob = date;}
    public void setAge(int age){Math.max(age, 0);}
    public void setGender(char gender){
        int random = new Random().nextInt(genderContainer.length);
        if (Character.toUpperCase(gender) != 'M' && Character.toUpperCase(gender) != 'F') {
            this.gender = genderContainer[random];
        }else {
            this.gender = gender;
        }
    }
    public void setPhone(String phone){this.phoneNum = phone;}
    public void setAddress(String address){this.address = address;}
    public void setLevel(String level){
        int random = new Random().nextInt(memberLevel.length);
        //set the level when the memberLevel contains the level to lowercase
        if (Arrays.asList(memberLevel).contains(level.toLowerCase())){/**/
            this.level = level;
        }else {
            this.level = memberLevel[random];
        }
    }
    public void setRegister(Date register){this.register = register;}
    public void setStatus(boolean status){this.status = status;}
    public void setExpireDate(Date expireDate){
        if(expireDate.after(register)){//if the expire date is date after register
            this.expireDate = expireDate;
        }else {
            this.expireDate = null;
        }
    }
    public boolean equals(Object o){
        //return true if objects are same
        if (o == this) {
            return true;
        }
        if(!(o instanceof Member)){
            return false;
        }
        Member temp = (Member) o;
        return id.equals(temp.getId());
    }

    @Override
    public String toString() {
        //check the member status whether is active or not
        String checkStatus = (status) ? "Active" : "Inactive";
        return "===================================\n" +
                "******** Member Detail ***********\n" +
                "===================================\n" +
                "  Member ID: " + id + "\n" +
                ", Name: " + name + "\n"+
                ", Date of birth: " + dateFormat.format(dob) + "\n"+
                ", Age: " + age + "\n"+
                ", Gender: " + gender + "\n"+
                ", Contact: " + phoneNum + "\n"+
                ", Address: " + address + "\n"+
                ", Membership level: " + level + "\n"+
                ", Date of joining: " + dateFormat.format(register) + "\n"+
                ", Status: " + checkStatus + "\n"+
                ", Expiry date: " + dateFormat.format(expireDate) + "\n";
    }
}

