/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author USER
 */

//Driver Function for answer2.java

import java.util.Scanner;
        
public class Test {
    public static void main(String[] args){
        System.out.println("Going to test the LRU " + " Cache Implementation");
        Cache cache = new Cache(2);
        Scanner input = new Scanner(System.in);

        // take input from the user
        System.out.println("Set the original key, value, weight:");
        System.out.print("Key:");
        int k1 = input.nextInt();
        System.out.print("Value:");
        int v1 = input.nextInt();
        System.out.print("Weight:");
        long w1 = input.nextLong();
        cache.put(k1, v1, w1);
        
        System.out.print("Key:");
        int k2 = input.nextInt();
        System.out.print("Value:");
        int v2 = input.nextInt();
        System.out.print("Weight:");
        long w2 = input.nextLong();
        cache.put(k2, v2, w2);
        
        System.out.println("The key you want to search:");
        int g1 = input.nextInt();
        cache.get(g1);
        
        System.out.println("New Arrival:");
        System.out.print("Key:");
        int k3 = input.nextInt();
        System.out.print("Value:");
        int v3 = input.nextInt();
        System.out.print("Weight:");
        long w3 = input.nextLong();
        cache.put(k3, v3, w3);
        
        System.out.println("The key you want to search:");
        int g2 = input.nextInt();
        cache.get(g2);
        
        System.out.println("The key you want to search:");
        int g3 = input.nextInt();
        cache.get(g3);
        
        System.out.println("New Arrival:");
        System.out.print("Key:");
        int k4 = input.nextInt();
        System.out.print("Value:");
        int v4 = input.nextInt();
        System.out.print("Weight:");
        long w4 = input.nextLong();
        cache.put(k4, v4, w4);
        
        System.out.println("The key you want to search:");
        int g4 = input.nextInt();
        cache.get(g4);
        
    }
}

