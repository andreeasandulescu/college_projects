import java.util.ArrayList;

public class ElementMore extends TM {			//elementary Turing Machines which can move the head at least once

	char moveHead;
	char symbol;

	public ElementMore(char moveHead, char symbol, String name) {
		this.moveHead = moveHead;
		this.symbol = symbol;
		this.name = name;
	}

	@Override
	public void run(ArrayList<Character> tape) {
		int headInd = tape.indexOf('>');
		char c;
		switch(moveHead)
		{
			case'R':
			if(headInd + 1 == tape.size())
				tape.add('#');
			do
			{
				c = tape.get(headInd + 1);
				tape.set(headInd, c);
				tape.set(headInd + 1, '>');
				headInd++;
			}while( (headInd + 1 < tape.size()) && (tape.get(headInd + 1) != symbol) );
			break;
			case'L':
			if(headInd == 0)
			{
				tape.add(0,'#');
				headInd = 1;				
			}
			do
			{
				c = tape.get(headInd - 1);
				tape.set(headInd, c);
				tape.set(headInd - 1, '>');
				headInd--;
			}while((headInd > 0 ) && (tape.get(headInd + 1) != symbol) );
			break;
		}
	}

	@Override
	public String toString() {
		return "[" + moveHead + "(" + symbol + ")]";
	}
}