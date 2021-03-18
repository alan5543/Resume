/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author USER
 */
import java.util.HashMap;
import java.util.Map;
import java.util.Date;

public class Node {
    int key;
    int value;
    long weight;
    Date last_access_time;
    Node pre;
    Node next;
    
    public Node (int key, int value, long weight, Date time)
    {
        this.key = key;
        this.value = value;
        this.weight = weight;
        this.last_access_time = time;
    }
    public void Modify_Time(Date time)
    {
        this.last_access_time = time;
    }
    public double Calculate_Score(long weight)
    {
        Date current = new Date();
        double difference = (double)(current.getTime()-this.last_access_time.getTime());
        double score = weight/(Math.log(difference));
        return score;
    }
}

class Cache{
    private HashMap<Integer, Node> map;
    private int capacity, count;
    
    public Cache(int cap) //constructor
    {
        this.capacity = cap;
        map = new HashMap<>();
        count = 0; //calculate exceed the capacity or not
    }
    
    //The get method works in O(1)
    public int get(int key)
    {
        if(map.get(key) != null){
            Node node = map.get(key);
            int output = node.value;
            node.Modify_Time(new Date()); //update the last access time
            System.out.println("Got the value : " + output + " for the key: " + key + " Time: " + node.last_access_time);
            
            return output;
        }
        else{
            System.out.println("Not Found : " + key);
            return -1;
        }
    }
    
    public void put(int key, int value, long weight)
    {
        if(map.get(key) == null) //new arrival
        {
            Date current_time = new Date();
            Node node = new Node(key, value, weight, current_time);
            if(count<capacity){
                count++;
                map.put(key, node);
            }
            else{
                double score = 0;
                int target = 0;
                for(Map.Entry me : map.entrySet()){
                    Node last = (Node)me.getValue();
                    if(current_time != last.last_access_time ) //find the minimum score
                    {
                        if(last.Calculate_Score(weight) < score || score == 0){
                            score = last.Calculate_Score(weight);
                            target = last.key;
                        }
                    }
                    else{
                        score = last.weight/-100;
                        target = last.key;
                    }
                }
                map.remove(target); //remove the old
                map.put(key, node); //add the new arrival
            }
            System.out.println("Get in : " + key + " , "+ value + " , "+ weight + " , "+ current_time + " , ");
        }
        else{
            Node node = map.get(key);
            node.value = value;
            System.out.println("The map value is updated : " + key + " , "+ value + " , "+ weight);
        }
    }
}
/*
Assumptions:
1. We use hashMap to construct the cache with the capacity of n.
2. Assume all the weight is positive integer
3. Assume the access time is based on the getTime() function

Observation:
1. Using put(key, value, weight) to set the cache content first. 
2. When we can use get(key), it will update the last access time of the target key node
3. We can use put(key, value, weight) to update the value of key or invalidate the node with the least scored key

Explanation:
1. For get Method:
    - we just get the cache value from the node.value in the hashmap and update the access_time (O(1) cost)
2. For put Method:
    - When the capacity is not full, we can use put() directly inserting the value
    - When the capacity is not full and the key from put() is already exists in cache, we update the value of the node
    - When the capacity is full, we calculate the score by the difference of last access time and current time

Time Complexity:
- get: O(1) by using hashmap
- put: O(n) by reading and computing all the scores in hashmap, to update/ insert/ invalidate the value of the target key.

*/

