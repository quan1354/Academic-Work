import java.io.*;
import java.util.ArrayList;
//this class is used as a management for read write file
public class FileManagement{
    //write to file
    public static void writeFile(ArrayList<Member> list){
        try{//serializing object to memberData file
            FileOutputStream writeData = new FileOutputStream("memberData.ser");
            ObjectOutputStream writeStream = new ObjectOutputStream(writeData);
            writeStream.writeObject(list);
            //push the contents of the buffer to the underlying Stream.
            writeStream.flush();
            writeStream.close();
        }catch (IOException e) {
            //print the throwable along with other details where the exception occurred.
            e.printStackTrace();
        }
    }
    //read to file
    public static ArrayList<Member> readFile(){
        File file = new File("memberData.ser");
        if (file.length() == 0 || !file.canRead() || !file.exists())
            return null;
        ArrayList<Member> memberList = null;
        try{
            FileInputStream readData = new FileInputStream("memberData.ser");
            ObjectInputStream readStream = new ObjectInputStream(readData);
            memberList = (ArrayList<Member>) readStream.readObject();
            readStream.close();
            System.out.println(memberList.toString());
            return memberList;
        }catch (IOException | ClassNotFoundException e) {
            //print the throwable along with other details where the exception occurred.
            e.printStackTrace();
        }
        return memberList;
    }
}
