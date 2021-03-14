import java.util.ArrayList;
public abstract class TM {

	String name;
	public abstract void run(ArrayList<Character> tape);
	public void add(TM m){ };
}
