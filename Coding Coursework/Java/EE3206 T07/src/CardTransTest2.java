// Student name: Yang Kai Yam
// Student ID  : 55676537

// Submission deadline: Friday, 6 Nov 2020, 11 am.

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

	Implement the method getReward() using the functional programming approach.
	The class FunctionUtil is given to you.
	That is, you should not use any explicit looping in the method getReward().
	You can use the sort() method, and the methods in FunctionUtil.
	
	You can use the class TempRec or the generic class Triple in your design.
	The class TempRec is to be defined at the bottom of this file.
*/

import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Comparator;
import static java.util.Comparator.comparing;
import java.util.Iterator;
import java.util.List;
import java.util.Scanner;
import java.util.Comparator;
import java.util.Map;
import java.util.function.BiConsumer;
import java.util.stream.Collectors;

public class CardTransTest2 
{            
    static List<Pair<String, Double>> getReward(List<CardTransaction> list, 
                                                double monthlyExp, double minExp,
                                                double rebate)
    {   
        
        BiConsumer<ArrayList<Triple<CardTransaction, Double, Integer>>, CardTransaction> actTri =
                (r,v)->{
                    String month = v.getDate().substring(0,7);
                    if(r.isEmpty())
                        r.add(new Triple(v, v.getAmount(), 1));
                    else{
                        Triple<CardTransaction, Double, Integer> item = r.get(r.size()-1);
                        String month2 = item.getFirst().getDate().substring(0,7);
                        boolean check = item.getFirst().getCard().equals(v.getCard()) && month.equals(month2);
                        if(check){
                            item.setSecond(item.getSecond() + v.getAmount());
                            item.setThird(item.getThird()+1);
                        }
                        else
                            r.add(new Triple(v, v.getAmount(), 1));
                    }
                };
        
         BiConsumer<ArrayList<Pair<String, Double>>, Map.Entry<String,Double>> ac =
                 (r,v)->{
                     r.add(new Pair(v.getKey(), v.getValue()));
                 };
        BiConsumer<ArrayList<Pair<String, Double>>, Map.Entry<String,Integer>> action2 =
                 (r,v)->{
                     r.add(new Pair(v.getKey(), v.getValue()*rebate));
                 };
        
            list.stream()
                .filter(p->p.getAmount()>=minExp)
                .sorted(comparing(CardTransaction::getCard)) //remember to use comparing!!!!!!!!!!!!!!!!!!
                .collect(ArrayList::new, actTri ,ArrayList::addAll)
                .stream() //after stream(), u can do filter!!!!!!!!!!
                .filter(p->p.getSecond()>=monthlyExp)
                .map(p-> new Triple(p.getFirst().getCard(), p.getSecond(), p.getThird()))
                .collect(Collectors.groupingBy(Triple<String, Double, Integer>::getFirst, Collectors.summingInt(Triple<String, Double, Integer>::getThird)))
                .entrySet()
                .stream()
                .forEach(System.out::println);
                    
        
        
        BiConsumer<ArrayList<TempRec>, CardTransaction> action =
                (result, v)->{
                    String month = v.getDate().substring(0, 7);
                  if (result.isEmpty())
                      result.add(new TempRec(v.getCard(), month, v.getAmount(), 1));
                  else{
                      TempRec item = result.get(result.size()-1);
                      if(item.getCard().equals(v.getCard()) && item.getYearMonth().equals(month))
                      {
                          item.addCount(1);
                          item.addExp(v.getAmount());
                      }
                      else
                          result.add(new TempRec(v.getCard(), month, v.getAmount(), 1));
                  }
                };

        
        //using Stream
            List<Pair<String, Double>> slist = list.stream()
                .filter(p->p.getAmount()>=minExp)
                .sorted(comparing(CardTransaction::getCard))
                .collect(ArrayList::new, action, ArrayList::addAll)
                .stream()
                .filter(p->p.getExp()>=monthlyExp)
                .collect(Collectors.groupingBy(TempRec::getCard, Collectors.summingInt(TempRec::getCount)))
                    .entrySet()
                    .stream()
                    .sorted((Map.Entry<String,Integer> a, Map.Entry<String,Integer> b) -> a.getKey().compareTo(b.getKey()))
                    .collect(ArrayList::new, action2, ArrayList::addAll);
            
          // FunctionUtil.forEach(test, System.out::println);
            
          // for (Map.Entry<String, Double> entry : test.entrySet()) {
          //      System.out.println("Key : " + entry.getKey() + " Value : " + entry.getValue());
          //   }

            List<Pair<String, Double>> a = new ArrayList();
            return slist;
        
        /*
        BiConsumer<List<Pair<String,Double>>, TempRec> action2 =
                (res, v)->{
                    if(res.isEmpty())
                        res.add(new Pair(v.getCard(), v.getCount()*rebate));
                    else{
                        Pair<String,Double> i = res.get(res.size()-1);
                        if(i.getFirst().equals(v.getCard()))
                            i.setSecond(i.getSecond()+v.getCount()*rebate);
                        else
                            res.add(new Pair(v.getCard(), v.getCount()*rebate));
                    }
                };
        
        List<CardTransaction> list1 = FunctionUtil.filter(list, p->p.getAmount() >= minExp);
        list1.sort(comparing(CardTransaction::getCard));
        
        List<TempRec> list2 = FunctionUtil.transform(list1, action);
        List<TempRec> list3 = FunctionUtil.filter(list2, p->p.getExp()>=monthlyExp);
        List<Pair<String,Double>> list4 = FunctionUtil.transform(list3, action2);
        
        List<Pair<String, Double>> a = new ArrayList();
        return list4;
        */
        
     /*/////   
        
        Comparator<CardTransaction> cmp = (t1, t2) -> {
            int r = t1.getCard().compareTo(t2.getCard());
            if(r != 0)
                return r;
            
            String d1 = t1.getDate();
            String d2 = t2.getDate();
            if(d1.charAt(5) != d2.charAt(5))
                return d1.charAt(5) - d2.charAt(5);
            return d1.charAt(6) - d2.charAt(6);
        };
        
        BiConsumer<List<Triple<CardTransaction, Double, Integer>>, CardTransaction> action1 =
                (result, v)->{
                    if(result.isEmpty())
                        result.add(new Triple(v, v.getAmount(), 1));
                    else{
                        Triple<CardTransaction, Double, Integer> item = result.get(result.size() - 1);
                        if(cmp.compare(item.getFirst(), v) == 0){
                            item.setSecond(item.getSecond() + v.getAmount());
                            item.setThird(item.getThird() + 1);
                        }
                        else
                            result.add(new Triple(v, v.getAmount(), 1));
                    }
                };
        
        BiConsumer<List<Pair<String, Double>>, Triple<CardTransaction, Double, Integer>> action2 =
                (result, v)->{
                    if(result.isEmpty())
                        result.add(new Pair(v.getFirst().getCard(), v.getSecond()));
                    else{
                        Pair<String, Double> item = result.get(result.size() - 1);
                        if(item.getFirst().equals(v.getFirst().getCard())){
                            item.setSecond(item.getSecond() + v.getSecond());
                        }
                        else
                            result.add(new Pair(v.getFirst().getCard(), v.getSecond()));
                    }
                };
        
        list.sort((r1,r2) -> (r1.getCard().compareTo(r2.getCard())));
        List<CardTransaction> f = FunctionUtil.filter(list, p->p.getAmount()>=minExp);
        List<Triple<CardTransaction, Double, Integer>> r = FunctionUtil.transform(f, action1);
        List<Triple<CardTransaction, Double, Integer>> f2 = FunctionUtil.filter(r, p->p.getSecond()>=monthlyExp);
        List<Pair<String, Double>> ff = FunctionUtil.transform(f2, action2);
        return ff;
        */
        /*
        // Input list is initially sorted by date.
        // Implement this method using the Functional Programming approach.
        // Do not use any explicit loop in the implementation of this method.     
        list.sort((r1, r2) -> (r1.getCard().compareTo(r2.getCard()))); //sort by the card no. first
        List<CardTransaction> minfilter = FunctionUtil.filter(list, p -> p.getAmount() >= minExp); //filter the list with qualified transactions.

        List<Triple<String,String,Double>>  //construct the triple list (card no, YearMonth, Amount) with transformation
                viewlist = FunctionUtil.transform(minfilter, (result, v) -> result.add(new Triple(v.getCard(), v.getDate().substring(0,7), v.getAmount())));

        //list.sort(comparing(CardTransaction::getAmount).thenComparing(CardTransaction::getCard));
       // BiConsumer<List<Pair<Pair<String, Double>,Pair<String,Double>>>, TempRec> act =
        //        (result, v)->{
                    
        //        };
        //list.forEach(System.out::println);
        BiConsumer< List<TempRec>, Triple<String, String, Double>> count =
                (result, v) -> {             //Transform the triple list to the TempRec list
                    if(result.isEmpty()){    //find out the card, YearMonth, TotalMonthAmount, Count
                        result.add(new TempRec(v.getFirst(),v.getSecond(),v.getThird(),1));
                    }
                    else
                    {
                        TempRec item = result.get(result.size()-1);
                        if(item.getCard().equals(v.getFirst()) && item.getYearMonth().equals(v.getSecond())) //same card and same month
                        {
                            item.addExp(v.getThird());
                            item.addCount(1);
                        }
                        else
                            result.add(new TempRec(v.getFirst(),v.getSecond(),v.getThird(),1));
                    }
                };
        
        List<TempRec> viewResult = FunctionUtil.transform(viewlist, count);                 //apply the transformation
        List<TempRec> adder = FunctionUtil.filter(viewResult, p -> p.getExp() >= monthlyExp);  //filter the element's amount is less than monthExp
        
        BiConsumer<List<Pair<String, Double>>, TempRec> action =  //transform the list to pair (counting the reward)
                (result, v) -> {
                    if(result.isEmpty())
                        result.add(new Pair(v.getCard(), v.getCount()*rebate)); //add the suitable rebate
                    else{
                        Pair<String, Double> item = result.get(result.size()-1);
                        if(item.getFirst().equals(v.getCard()))
                            item.setSecond(item.getSecond() + rebate*v.getCount());
                        else
                            result.add(new Pair(v.getCard(), v.getCount()*rebate));
                    }
                };
        
        List<Pair<String, Double>> result = FunctionUtil.transform(adder, action); //create the result list, pair with qualified card number and Reward
        FunctionUtil.forEach(result, p->System.out.println(p.getFirst()+ " " + p.getSecond()));
        List<Double>r2 = FunctionUtil.map(result, p->p.getSecond());
        //FunctionUtil.forEach(list, p->p.setAmount(p.getAmount()+100000));
        //FunctionUtil.forEach(list, p->System.out.println(p.getAmount()));
        FunctionUtil.forEach(r2, p->System.out.println(p));
        List<Pair<String, Double>> r1 = FunctionUtil.filter(result, p->p.getSecond() > 200);
        return r1; */
    }
    
    
    // -------------------------------------- methods given to you
    
    public static void main(String[] args) throws FileNotFoundException
    {
        String filename = "creditCard-data.txt";  // data recors sorted by date
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

class TempRec
{
    String card;
    String yearMonth;  // yyyy-mm
    double totalExp;   
    int count;

    // constructor and other methods
    public TempRec(String d, String c, double b, int a)
    {
        card = d;
        yearMonth = c;
        totalExp = b;
        count = a;
    }

    public String getCard()
    {
        return card;
    }
    
    public String getYearMonth()
    {
        return yearMonth;
    }
    
    public double getExp()
    {
        return totalExp;
    }
    
    public int getCount()
    {
        return count;
    }
    public void addExp(double v)  //update the totalExpt
    {
        this.totalExp = this.totalExp + v;
    }
    public void addCount(int u)  //update the count
    {
        this.count = this.count + u;
    }
    
    @Override
    public String toString()
    {
        return card + "\t" + yearMonth + "\t" + totalExp+ "\t" + count;
    }
   
}

