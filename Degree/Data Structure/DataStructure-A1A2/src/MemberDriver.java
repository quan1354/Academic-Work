import java.io.File;
import java.text.ParseException;
import java.util.ArrayList;
//this class is a driver class, used for running the main program
public class MemberDriver {
    public static void main(String[] args) throws ParseException {
        MemberModule memberModule;
        if(FileManagement.readFile() == null){
            memberModule = new MemberModule();
        }else {
            ArrayList <Member> list = FileManagement.readFile();
            memberModule = new MemberModule(list);
        }
        memberModule.processMainMenu();
    }
}



