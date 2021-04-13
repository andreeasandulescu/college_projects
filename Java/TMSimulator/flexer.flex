import java.util.ArrayList;
import java.util.HashMap;

%%

%class Flexer
%unicode
/*%debug*/
%int
%line
%column

%{
    Alphabet alphabet = new Alphabet();
    TuringMachines tm = new TuringMachines();
    ArrayList <Character> transSet = new ArrayList <Character>();     //set of symbols for which the Turing machine will execute the same transition
    ArrayList <String> aliases = new ArrayList <String>();            //the name given to a symbol inside a transition declaration
    HashMap<String, TM> tags = new HashMap<String, TM>();             //the name of a Turing machine declared earlier, in the file
    String name = " ";
    String symbol = " ";
    char moveHead;
    int i = 0;

%}

lineTerminator = \r|\n|\r\n
ws = {lineTerminator} | [ \t\f]
letter = [a-z]|[A-Z]
other = "#"|"$"|"*"|"@"
symbol = {letter} | [:digit:] | {other}
name = ({letter} | [:digit:] | "_")+

%state ALPHASEPA ALPHASYMB CODE SYMBDECL SETDECL MT MTCALL ELEMENT SETTRANS TRANS
/* States:

   **ALPHASEPA: reads the separator between the "alphabet" keyword and the character list (::)
   **ALPHASYMB: reads a symbol(character) of the alphabet
   **CODE: the code from a source file, which can contain symbol declaration, set declaration, or Turing machine .
   **SYMBDECL: reads a symbol declaration
   **SETDECL: reads a set declaration
   **MT: reads a Turing Machine declaration
   **MTCALL: reads a Turing Machine call
   **ELEMENT: reads what is between the brackets of an elementary Turing machine ( R(element), L(element) )
   **SETTRANS: reads the set of symbols for which the Turing machine will execute a transition
   **TRANS: reads the separator between the set of symbols and the transition ("->")
*/

%%

{ws}  {/*Skip whitespace in any state*/}

<YYINITIAL>  "alphabet" {//Read the "alphabet" keyword
                          yybegin(ALPHASEPA);
}

<ALPHASEPA> "::"  {//Read the separator between the "alphabet" keyword and character list
                      yybegin(ALPHASYMB);
}

<ALPHASYMB> {
    {symbol}  {//Read one symbol at a time or read the separator placed at the end of the character list (;)
                  String text = yytext();
                  alphabet.addSymbol(text.charAt(0));
              }
    ";"       {  yybegin(CODE);}
}

<CODE> {
    {name}  {
                name = yytext();
            }
    "="     {//Read a symbol declaration
                      yybegin(SYMBDECL);
            }
    ":="     {//Read a set declaration
                      yybegin(SETDECL);
            }
    "::="     {//Read a Turing Machine declaration
                      yybegin(MT);
                      tm.push(new Complex(name));
                      name = "*****";
            }
    ";"[^\r\n"\r\n"]+{lineTerminator} { }             //reads comments
}

<SYMBDECL> {symbol}{ws}*";" {
                                    yybegin(CODE);
                                    String symbText = yytext();
                                    String[] symbParts = symbText.split(" ");
                                    symbol = symbParts[0];
                                    alphabet.addSymbol(symbol.charAt(0), name);
                                    name = "*****";
                            }

<SETDECL> {
    ","         {}
    "{"         {
                  alphabet.addSet(name);
                }
    {symbol}    {
                  String text = yytext();
                  alphabet.addSymbolToSet(name, text.charAt(0));
                }
    "}"{ws}*";" {  yybegin(CODE);
                   name = "*****";
                }
}

<MT> {
    "["          {   //Turing machine call 
                    yybegin(MTCALL);
                 }
     "("         { //Turing Machine defined as a list of transitions
                    yybegin(SETTRANS); 
                    tm.push(new Trans(name));
                    name = "*****";
                }
    ";"         {   //end of a transition
                    yybegin(SETTRANS);
                    TM t = tm.pop(); 
                }
    ";;"        {   //end of a Turing machine declaration
                    yybegin(CODE);  
                    tm.addFinalTM(); 
                }
    {name}"@"   {
                    String text = yytext();
                    name = text.substring(0, text.length() - 1);
                    Complex namedTM = new Complex(name);
                    tags.put(name,namedTM);
                    tm.push(namedTM);
                    name = "*****";
                }
    "&"{name}   {
                    String text = yytext();
                    name = text.substring(1, text.length());
                    Complex tm1 = new Complex(name);
                    tm1.add(tags.get(name));
                    TM addTo = tm.stack.peek();
                    addTo.add(tm1);
                    name = "*****";
                }
}

<MTCALL> {
    "L]"        {   yybegin(MT);
                    TM toAdd = new ElementOne('L', 'L', "|");
                    if(!tm.stack.isEmpty())
                    {
                      TM aux = tm.stack.peek();
                      aux.add(toAdd);
                    }
                }
    "R]"        {   yybegin(MT);
                    TM toAdd = new ElementOne('R', 'R', "|");
                    if(!tm.stack.isEmpty())
                    {
                      TM aux = tm.stack.peek();
                      aux.add(toAdd);
                    }
                }
    {symbol}"]" {   yybegin(MT);
                    String text = yytext();
                    TM toAdd = new ElementOne('H', text.charAt(0), "|");            //H = hold
                    if(!tm.stack.isEmpty())
                    {
                      TM aux = tm.stack.peek();
                      aux.add(toAdd);
                    }
                }
    "L("        {   yybegin(ELEMENT);
                    moveHead = 'L';
                }
    "R("        {   yybegin(ELEMENT);
                    moveHead = 'R';
                }
    {name}"]"   {   yybegin(MT);
                    String text = yytext();
                    String auxName = text.substring(0, text.length() - 1);
                    Complex found = (Complex) tm.fileTM.get(auxName);
                    if(found!= null)
                    {
                          TM aux = (Complex) tm.stack.peek();
                          aux.add(found);
                    }
                }
    "&"{name}"]"  {   yybegin(MT);
                      String text = yytext();
                      String auxName = text.substring(1, text.length() - 1);
                      if(aliases.contains(auxName))
                      {
                        TM toAdd = new ElementOne('H', '&', "|&");            //H = hold
                        TM aux = (Complex) tm.stack.peek();
                        aux.add(toAdd);
                      }
                }                    

<ELEMENT> {
    {symbol}")]" {  yybegin(MT);
                    String text = yytext();
                    TM toAdd = new ElementMore(moveHead, text.charAt(0), "+");
                    if(!tm.stack.isEmpty())
                    {
                      Complex aux = (Complex) tm.stack.peek();
                      aux.add(toAdd);
                    }
                }
    "<"{name}">)]" {  yybegin(MT);
                    String text = yytext();
                    String aux = text.substring(1, text.length() - 3);
                    ArrayList <Character> arr = alphabet.getByName(aux);
                    if(arr != null)
                    {
                        TM toAdd = new ElementMore(moveHead, arr.get(0), "+");
                        if(!tm.stack.isEmpty())
                        {
                          Complex a = (Complex) tm.stack.peek();
                          a.add(toAdd);
                        }
                    }
                    }
                   
    "!"{symbol}")]" {   yybegin(MT);
                        String text = yytext();
                        TM toAdd = new ElementMoreDiff(moveHead, text.charAt(1), "+");
                        if(!tm.stack.isEmpty())
                        {
                          Complex aux = (Complex) tm.stack.peek();
                          aux.add(toAdd);
                        }
                    }
       
}


<SETTRANS> {
    "{"{symbol}"}"    { yybegin(TRANS);
                        transSet.clear();
                        String text = yytext();
                        transSet.add(text.charAt(1));
                      }
    "{"{symbol}","    { transSet.clear();
                        String text = yytext();
                        transSet.add(text.charAt(1));
                      }
    {symbol}"}"       { yybegin(TRANS);
                        String text = yytext();
                        transSet.add(text.charAt(0));
                      }
    {symbol}","       { String text = yytext();
                        transSet.add(text.charAt(0));
                      }
    ")"               {
                        yybegin(MT);
                        TM tm1 = tm.pop();
                        TM tm2 = tm.stack.peek();
                        tm2.add(tm1);
                      }
    "<"{name}">"    {   yybegin(TRANS);
                        transSet.clear();
                        String text = yytext();
                        String aux = text.substring(1, text.length() - 1);
                        transSet = alphabet.getByName(aux);
                    }
    "<"{name}">"    {   yybegin(TRANS);
                        transSet.clear();
                        String text = yytext();
                        String aux = text.substring(1, text.length() - 1);
                        transSet = alphabet.getByName(aux);
                    }
    {name}"@"       {      
                        String text = yytext();
                        String aux = text.substring(0, text.length() - 1);
                        aliases.add(aux);
                    }
    "{<"{name}">}"  {
                        yybegin(TRANS);
                        String text = yytext();
                        String aux = text.substring(2, text.length() - 2);
                        aliases.add(aux);
                        transSet = alphabet.getByName(aux);
                    }

}


<TRANS> "->"{ws}* {  yybegin(MT);
                    Character first = transSet.get(0);
                    Trans aux = (Trans) tm.stack.peek();
                    Complex cMT = new Complex("c**" + first);
                    for(Character c:transSet)
                    {    //for all the symbols in a set, the same TM will be run, only the tape will be different

                         aux.addTrans(c, cMT); 
                    }
                    tm.push( aux.transitions.get(first));
                    i++;
                  }
}


