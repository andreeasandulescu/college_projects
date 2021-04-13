import java.util.ArrayList;

public class ElementOne extends TM {			//elementary Turing Machines which can move the head at most once

	char moveHead;
	char symbol;

	public ElementOne(char moveHead, char symbol, String name) {
		this.moveHead = moveHead;
		this.symbol = symbol;
		this.name = name;
	}

	public void setSymbolName(char symbol, String name) {
		this.symbol = symbol;
		this.name = name;
	}

	@Override
	public void run(ArrayList<Character> tape) {
		int headInd = tape.indexOf('>');
		if(moveHead == symbol)
		{
			switch(moveHead)
			{
				case'L':
						if(headInd == 0)
							tape.add(1,'#');
						else
						{
							char c = tape.get(headInd - 1);
							tape.set(headInd, c);
							tape.set(headInd - 1, '>');
						}
						break;
				case'R':
						if(headInd + 1 == tape.size())
						{
							tape.add(tape.size()-1,'#');
							tape.add('#');
						}
						else
						{
							char c = tape.get(headInd + 1);
							tape.set(headInd, c);
							tape.set(headInd + 1, '>');
							if(tape.indexOf('>') + 1 == tape.size())
							tape.add('#');
						}
						break;
			}
		}
		else
		{
			if(headInd + 1 < tape.size())
				tape.set(headInd + 1,symbol);
			else
			{
				tape.add(symbol);
			}
		}
	}

	@Override
	public String toString() {
		if(moveHead == symbol)
			return "[" + moveHead + "]";
		else
			return "[" +  symbol + "]";
	}
}
