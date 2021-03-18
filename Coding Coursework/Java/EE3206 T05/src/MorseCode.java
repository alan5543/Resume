// Student name: Yang Kai Yam
// Student ID  : 55676537

// Submission deadline: Friday, 23 Oct 2020, 11 am.

// Complete the implementation of class MorseCode and class Node.

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Scanner;

public class MorseCode
{
    // Define the required instance variables.
    private static final Node root = new Node(' ');  //determine a node to build main tree
    private final ArrayList<String> store = new ArrayList<String>(); //for storing the morse code from each char (for encoding)
    
    public MorseCode(String filename) throws IOException
    {   
        // Read the code table from data file and set up the data structures.
        // You need not provide the exception hander in this method.
        Scanner sc = null;
        sc = new Scanner(new File(filename));  //input the file
        while(sc.hasNextLine())
            {
                String var = sc.nextLine().trim();
                store.add(var);  //update the Arraylist for storing
                int length = var.length();
                char letter = var.charAt(0);
                String sym = var.substring(1).trim();
                
                if(length > 0)
                {
                   add(sym, letter);  //add the the letter to the tree (dot : move left, dash : move right)
                }
            }
    }
    
    public void add (String mcode, char ltr){
        Node current = root; //determine the node in the tree
        char signal ;
        for(int i=0; i<mcode.length(); i++)
        {
            signal = mcode.charAt(i);
            if(signal == '.') //if the char is '.' then move left in the tree
            {
                if(current.getLeft() != null)
                    current = current.getLeft();
                else{
                    current.extendLeft(new Node(' '));
                    current = current.getLeft();
                }
            }
            else if (signal == '-') //if the char is '-' then move right in the tree
            {
                if(current.getRight() != null)
                    current = current.getRight();
                else{
                    current.extendRight(new Node(' '));
                    current = current.getRight();
                }
            }
        }
        current.setSymb(ltr); //set the letter of each node
    }

    public String encode(String text)
    {
        // Return a String of '0' and '1' that represents the Morse code
        // of the input text.
        // Input text may contain uppercase letters, digits, and space char.
        // Your codes
        String result = "";
        String s = "";
        String input = text.toUpperCase();
        char target;
        for (int i=0; i<input.length(); i++)
        {
            target = input.charAt(i);
            s = detect(target); //get the morse code of the target character
            if(target == ' ')   //implementation of whitespace morse code
                result = result + "0000";
            else
            {
                for(int num = 0; num<s.length(); num++){
                    if(s.charAt(num) == '.') //convert the dot morse code to 1
                        result = result + "1";
                    else if (s.charAt(num) == '-') //convert the dash morse code to 1
                        result = result + "111";
                    
                    if(num <s.length()-1) //end a '0' after each implementation
                        result = result + "0";
                }
                if(i<input.length()-1)
                    result = result + "000"; //change to next char, then add "000"
            }
        }
        return result;  // dummy return statement, replace it by your own codes
    }
    
    public String detect(char target) //output the morse code of the target character
    {
        String sym = "";
        for(int i=0; i<store.size(); i++)
        {
            String temp = store.get(i); //take from the store arraylist
            if(target == temp.charAt(0)){
                sym = temp.substring(1).trim();
                break;
            }
        }
        return sym;
    }
    
    public String decode(String mc)
    {
        // mc is a sequence of '0' and '1' that represents the Morse code of a message.
        // Return the decoded message as a String.
        
        
        // Your codes.
        // An efficient approach to do the decoding is to represent the code table by a binary tree.
        // If you use a brute-force approach in your answer, you will get lower marks.
        StringBuffer result = new StringBuffer("");
        char present = ' ';
        Node current = root; //determine the node of tree

        for(int i=0, j =1; i<mc.length() && j<mc.length(); j++)
        {
            present = mc.charAt(i);
            int count = 1; //counting the number of the current consecutive character
            while(mc.charAt(j) == present){
                count ++;
                j++;
                if(j == mc.length()-1) //avoid exceeding the loop
                    break;
            }
            if(present == '1') //Consideration in '1'
            {
                if(count == 1){   //if the number of '1' is 1
                    if(current.getLeft() != null)
                        current = current.getLeft();
                    else{
                        current.extendLeft(new Node(' '));
                        current = current.getLeft();
                    }
                }
                else if(count == 3){  //if the number of '1' is 3
                    if(current.getRight() != null)
                        current = current.getRight();
                    else{
                        current.extendRight(new Node(' '));
                        current = current.getRight();
                    }
                }
            }
            else if(present == '0') //Consideration in '0'
            {
                if(count == 3){  //if the nmber of '0' is 3, which means finished the current char and move on the next
                    result = result.append(current.getSymbol());
                    current = root; // restart
                }
                else if(count == 7){  //if the number of '0' is 7, which is a whitespace
                    result = result.append(current.getSymbol());
                    result = result.append(root.getSymbol());
                    current = root; //restart
                }
            }
            i = j;
        }

        int lastcount = 1;
        for(int i = mc.length()-1; mc.charAt(i) == mc.charAt(i-1); i--) //Consideration of the last char, in which the previous loop didnt include it
            lastcount++;
        if(lastcount == 3)
            current = current.getRight();
        else
            current = current.getLeft();
       
        result = result.append(current.getSymbol());
        return result.toString();  // dummy return statement, replace it by your own codes
    } 
       
    // Other supporting methods where appropriate.
}

// --------------------------------------------------------------------------------

// class to model a tree node to support decoding of Morse code
class Node
{
    private char symb;
    private Node left, right;
    
    public Node(char c)
    {
        symb = c;
        left = right = null;
    }
    public void extendLeft(Node t)
    {
        left = t;
    }
    public void extendRight(Node t)
    {
        right = t;
    }
    public Node getLeft()
    {
        return left;
    }
    public Node getRight()
    {
        return right;
    }
    public char getSymbol()
    {
        return symb;
    }
    public void setSymb(char symb)
    {
        this.symb = symb;
    }
    // other methods

    /*  Hint: a method to create the left child of the implicit Node with symbol c.

    public void extendLeft(char c)
    {
        left = new Node(c);
    }

    */

}

