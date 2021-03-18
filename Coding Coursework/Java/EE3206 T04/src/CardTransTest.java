// Student name: Yang Kai Yam
// Student ID  : 55676537

// Submission deadline: Friday, 9 Oct 2020, 11 am.

/*
	A data file with credit transaction records is given to you.
	Assume the card company has launched a reward program.
	The reward program is as follows:
		- Transaction with amount greater than or equal to minExp is a qualified transaction.
		- Total amount of qualified transactions in a month is greater than or equal to monthlyExp.
		- For each qualified transaction, the card will receive a fixed rebate.

	For example, the minExp for qualified transaction is $500, the monthlyExp is $2500,
	and rebate for each qualified transaction is $10.

	A card has 3 qualified transactions, say $1000, $600, $1500 in a given month.
	Total amount of the qualified transactions is $3100.
	This card will earn 3 x $10 = $30 reward for that month.

	You need to implement the method getReward().
	The method returns a list of cards and the total rewards received by each card.
	Only cards with non-zero reward are included in the output list.
*/

import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import static java.util.Comparator.comparing;
import java.util.Iterator;
import java.util.List;
import java.util.Scanner;

public class CardTransTest 
{            
    
    static int getMonth (CardTransaction a) //method for getting the month
    {
        String result = a.getDate().substring(5, 7);
        int month = Integer.parseInt(result);
        return month;
    }
    
    static List<Pair<String, Double>> getReward(List<CardTransaction> list, 
                                                double monthlyExp, double minExp,
                                                double rebate)
    {
        // Input list is initially sorted by date.
        // Use Iterator to traverse the list in your implementation.
        // Do not traverse the list using integer index.
        
        List<Pair<String, Double>> result = new ArrayList();
        
        
        Collections.sort(list, (e1, e2) -> e1.getCard().compareTo(e2.getCard())); // sorting the list by the name and time
        
        Iterator<CardTransaction> cur = list.iterator();
        Iterator<CardTransaction> prev = list.iterator();
        Iterator<CardTransaction> compar = list.iterator();
        

       // while (iter.hasNext())
        //    System.out.println(iter.next());
        
        // Your codes
        while (prev.hasNext()){                    // the loop is til the end
            
            CardTransaction first = prev.next();
            CardTransaction monfirst = first;
            double reward = 0;
            int count = 0;
            String ID;
            
            while(first.getCard().equals(monfirst.getCard()))  //loop the card ID number
            {
                double mon = 0;
                CardTransaction second = compar.next();
                while(CardTransTest.getMonth(second) == CardTransTest.getMonth(first)) //collect the money within one month
                {
                    mon += second.getAmount();
                    second = compar.next();
                }
                if(mon >= 2500) // test whether the money within one month is whether greater than 2500
                {
                    while(monfirst != second){
                         if(monfirst.getAmount() >= 500) //loop the month (>2500), test the day amount is whether greater than 500
                            count++;
                        monfirst = cur.next();
                    } 
                }
                cur = compar;
                boolean arrival = false;
                while(!arrival)                         //redefine the location of the next loop
                {
                    CardTransaction test = prev.next();
                    if(test == second)
                        arrival = true;
                }
            }
            reward =  count*10;   // reward = count * 10 (find the total reward of that card ID)
            ID = first.getCard();
            result.add(new Pair(ID, reward));
        }
      
        

        
        return result;
    }
    
    
    // -------------------------------------- methods given to you
    
    public static void main(String[] args) throws FileNotFoundException
    {
        String filename = "creditCard-data.txt";  // data records sorted by date
        ArrayList<CardTransaction> list = new ArrayList();
        try (Scanner sc = new Scanner(new File(filename)))
        {
            while (sc.hasNext())
            {
                String date = sc.next();
                String card = sc.next().trim();
                double amount = sc.nextDouble();
                CardTransaction t = new CardTransaction(card, date, amount);
                list.add(t);
            }
        }
        
        List<Pair<String, Double>> result = getReward(list, 2500.0, 500.0, 10.0);
        System.out.println("List of cards earning rewards:");
        for (Pair<String, Double> p : result)
            System.out.println(p);
        
    }
}
