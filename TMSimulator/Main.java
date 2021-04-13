import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;

public class Main {

	public static void main(String[] args) {
		BufferedReader br;
		try {
			br = new BufferedReader(new FileReader(args[0]));
			Flexer scanner = new Flexer(br);
			scanner.yylex();
			scanner.tm.runMT(args[1], args[2]);

		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

}

