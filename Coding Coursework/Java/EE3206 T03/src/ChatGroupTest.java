// Student name: Yang Kai Yam 
// Student ID  : 55676537

// Submission deadline: Friday, 25 Sept 2020, 11 am

// Implement the method getMutualFriends().
// Lookup class Arrays and ArrayList, and interface List and Collections.sort() in java.util

import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Scanner;
import java.util.Set;
import java.util.stream.Collectors;

public class ChatGroupTest 
{        
    // You may define other supporting methods where appropriate.
    static List<User> Remove(List<User> groupList) //Remove the duplicate case in the ascending order list
    {
        for(int i=0; i<groupList.size(); i++)
            for(int k=i+1; k<groupList.size(); k++)
                if(groupList.get(i).compareTo(groupList.get(k)) == 0){
                    groupList.remove(k);
                    i = i-1;   //Avoid many duplicate cases, so relocate the i and do the remove again
                }
        return groupList;
    }
    
    static List<User> GroupFriends(List<ChatGroup> groupList, User u) //find the friends list of the specific person
    {
        List<User> ListFrd = new ArrayList();
        for(int k=0; k<groupList.size();k++)
         {
             if(groupList.get(k).getUser(u.getTel()) != null) //determine whether the target is in this group or not
             {
                 for(int i=0; i<groupList.get(k).getMembers().length; i++)
                     if(u.compareTo(groupList.get(k).getMembers()[i]) != 0)
                         ListFrd.add(groupList.get(k).getMembers()[i]);
             }
         }
        
        Collections.sort(ListFrd);                 // sorting the list in ascending order by tel
        ListFrd = ChatGroupTest.Remove(ListFrd); //call the method to remove the duplicate friends in the list
        return ListFrd;
    }
    
    static List<User> getMutualFriends(List<ChatGroup> groupList, User u1, User u2)
    {
        // Implement this static method.
        // Users in the same chat group are considered to be friends.

        // Return a list of users (without duplicate) that are mutual friends 
        // of u1 and u2 derived from the list of chat groups.
        
        // groupList should not be modified
        
List<String>nam = groupList.stream()
                .flatMap(p->Arrays.stream(p.getMembers()))
                .collect(Collectors.groupingBy(User::getName, Collectors.counting()))
                .entrySet()   //map need entryset to function
                .stream()
                .filter(a->a.getValue()>2) //filter map entry
                .sorted((Map.Entry<String, Long>a, Map.Entry<String, Long>b)-> a.getKey().compareTo(b.getKey()))
                .map(Map.Entry::getKey) //Take keyset
                .collect(Collectors.toList());  //finally need to collect to list?!?!?!?!
      //          .forEach(System.out::println);
        
        Set<User> setter = groupList.stream()   //define in a set with defied equal() and hashCode
                .flatMap(p->Arrays.stream(p.getMembers()))
                .collect(Collectors.toSet());
        
        User[] arrayset = setter.toArray(new User[setter.size()]);
        List<User> listset = Arrays.asList(arrayset);
        
        for(User a: listset)
            System.out.println("set: "+ a + listset.size());
        
        List<String> tep = groupList.stream()  //phone list
                .flatMap(p-> Arrays.stream(p.getMembers()))
                .collect(Collectors.groupingBy(User::getTel, Collectors.counting()))
                .keySet()  //take key
                .stream()
                .collect(Collectors.toList());
        
        for(String a:tep)
            System.out.println(a);
        
        List<User> t = 
                groupList.stream()
                .flatMap(p-> Arrays.stream(p.getMembers()))  //Use Map, it is a map of maps
                .sorted((a,b)->a.getName().compareTo(b.getName()))
                .distinct() //need to define the equal and hashcode override
                .limit(10)
                .collect(Collectors.toList());                //Use FlatMap, it is combination of maps
        
        for(User a: t)
            System.out.println(a);
        
        List<User> temp = new ArrayList();
        List<User> result = new ArrayList();

        // Your codes
        result = ChatGroupTest.GroupFriends(groupList, u1); // the list is already sorted
        temp = ChatGroupTest.GroupFriends(groupList, u2); // the list is already sorted
        
        for(int i=0; i<result.size(); i++)
        {
            boolean found = false;
            for(int j=0; j<temp.size(); j++)
            {
                if(result.get(i).compareTo(temp.get(j)) == 0)
                {
                    found = true;
                    break;
                }
            }
            if(!found)
            {
                int a = i;
                result.remove(a);
                i = i - 1;
            }
        }
        
        return result;
    }
    
    // -------------------------------------- methods given to you
    
    public static void main(String[] args) throws FileNotFoundException
    {
        ArrayList<ChatGroup> groupList = new ArrayList();
        try (Scanner sc = new Scanner(new File("chatGroups-data.txt")))
        {
            int numGroup = sc.nextInt();
            for (int g = 0; g < numGroup; g++)
            {
                String groupName = sc.next();
                int k = sc.nextInt();
            
                ChatGroup group = new ChatGroup(groupName);
                for (int i = 0; i < k; i++)
                {
                    String tel = sc.next();
                    String name = sc.nextLine().trim();
                    group.addUser(new User(name, tel));
                }
                groupList.add(group);
            }
        }
        
        try (Scanner sc = new Scanner(new File("mutualFriend-tests.txt")))
        {
            int tests = sc.nextInt();
            for (int i = 0; i < tests; i++)
            {
                System.out.println("-----------------------------------------------");
                String tel1 = sc.next();
                String name1 = sc.nextLine().trim();
                String tel2 = sc.next();
                String name2 = sc.nextLine().trim();
                
                User u1 = new User(name1, tel1);     
                User u2 = new User(name2, tel2);
        
                List<User> friends = getMutualFriends(groupList, u1, u2);
                System.out.println("Mutual friends of " + u1.getName() + " and " + u2.getName());
                System.out.println("Number of mutual friends = " + friends.size()); 

                friends.forEach(System.out::println);  // Functional operation
                // Equivalent to
                // for (User u : friends)
                //     System.out.println(u);
                //
            }
        }
    }
}
