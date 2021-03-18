// Student name: Yang Kai Yam
// Student ID  : 55676537

// Submission deadline: Friday, 20 Nov 2020, 11 am
// Implement the method bestStudents() using the Stream API.

import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.text.NumberFormat;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Optional;
import java.util.Scanner;
import java.util.function.BiConsumer;
import java.util.function.Function;
import java.util.stream.Stream;

public class Tut_09 
{
    public static void main(String[] args) throws IOException
    {       
        try (Scanner sc2 = new Scanner(new File("students.txt")))
        {
            while (sc2.hasNextLine())
            {
                String line = sc2.nextLine();
                String[] token = line.split("\\s");
                String name = token[2];
                for (int i = 3; i < token.length; i++)
                    name = name + " " + token[i];

                Student.add(new Student(name, Integer.parseInt(token[0]), token[1]));
            }
        }

        try (Scanner sc3 = new Scanner(new File("courses.txt")))
        {
            while (sc3.hasNextLine())
            {
                String line = sc3.nextLine();
                String[] token = line.split("\\s");
                String title = token[2];
                for (int i = 3; i < token.length; i++)
                    title = title + " " + token[i];
            
                Course.add(new Course(token[0], title, Integer.parseInt(token[1])));
            }
        }
        
        NumberFormat nf = NumberFormat.getInstance();
        nf.setMaximumFractionDigits(2);

        System.out.println("Students with highest GPA in each major");
        List<Pair<Student, Double>> result = bestStudents("grades.txt");
        for (Pair<Student, Double> item : result)
        {
            Student s = item.getFirst();
            System.out.println(s.getMajor() + ", " + s.getSid() + ", " + s.getName() +
                               ", GPA = " + nf.format(item.getSecond()));
        }     
    }
     
    static List<Pair<Student, Double>> bestStudents(String filename) throws IOException
    {
        // Find the student with the highest GPA in each major.
        // Implement this method using Stream API.
        // Do not use any explicit loop in your design.

        // Format your program statements properly.
        // Align the 'dots' of the Stream pipeline vertically.
        
        
        Function<String, Triple<String, Integer, String>> mapper =
                line -> {
                   String[] token = line.split("\\s");   //split it to build a Triple
                   return new Triple(token[0], Integer.parseInt(token[1]), token[2]);
                };
        
        BiConsumer<ArrayList<Triple<Integer, Double, Integer>>, Triple<String, Integer, String>>
            accumulator = (sumList, t) -> {
                double gp = Course.gradePoint(t.getThird());
                Optional<Course> r = Course.search(t.getFirst());  //search the CourseCode in the CourseList
                int credit = r.get().getCredit();
                           
                if (sumList.isEmpty())
                    sumList.add(new Triple(t.getSecond(), gp * credit, credit)); //update the Triple<SID, Sum of marks, TotalCredit>
                else
                {
                    Triple<Integer, Double, Integer> item = sumList.get(sumList.size()-1);
                    if (item.getFirst().equals(t.getSecond()))
                    {
                        item.setSecond(item.getSecond() + gp * credit);
                        item.setThird(item.getThird() + credit);
                    }
                    else
                        sumList.add(new Triple(t.getSecond(), gp * credit, credit)); //update the Triple<SID, Sum of marks, TotalCredit>
                }
            };
        
        Function<Triple<Integer, Double, Integer>, Pair<Student, Double>> mapper2 =
            t -> {
                double gpa = 0;
                if(t.getThird() > 0)
                    gpa = (double)Math.round(t.getSecond()/t.getThird()*100)/100; //calculate the GPA and round up it
                Student s = Student.search(t.getFirst()).get(); //search Student by the SID
                return new Pair(s, gpa);
            };

        BiConsumer<ArrayList<Pair<Student, Double>>, Pair<Student, Double>>
            accumulator2 = (r, p) -> {
                int k = Collections.binarySearch(r, p,  //find out the same major
                            (t1, t2) -> t1.getFirst().getMajor().compareTo(t2.getFirst().getMajor()));
                if (k >= 0)
                {
                    Pair<Student, Double> item = r.get(k);
                    if (item.getSecond() < p.getSecond())                        
                        r.set(k, p);
                }
                else
                    r.add(-(k+1), p);
            };

        Stream<Integer> intStream = Stream.of(1,2,3,4,5,6);
        long coun = intStream.count();
        System.out.println(coun);
        
        Stream.iterate(1L, n-> n+2)
                .limit(10)
                .forEach(System.out::println);
        
        Path filepath = Paths.get(filename);
        try(Stream<String> lines = Files.lines(filepath))
        {
            return lines.map(mapper)
                 .collect(ArrayList::new, accumulator, ArrayList::addAll)
                 .stream()
                 .map(mapper2)
                 .filter(n->n.getSecond()>3)
                 .peek(e-> System.out.println(e.getSecond()))
                 .collect(ArrayList::new, accumulator2, ArrayList::addAll);
        }
        catch(IOException e)
        {
               List<Pair<Student,Double>> result = new ArrayList();
               return result;   //dummy output for exception
        }
        /*
        try (Stream<String> lines = Files.lines(filepath))
        {
            return lines.map(mapper)    //map the line to Triple<String(CourseCode), Integer(SID), String(LetterGrade)>
                    .collect(ArrayList::new, accumulator, ArrayList::addAll) //collect Triple<String, Integer, String> to be <Triple<Integer(SID), Double(TotalMark), Integer(TotalCredit)>>
                    .stream()        //change it to stream, so we can map it again
                    .map(mapper2)    //map the stream to Pair<Student, Double(GPA)>
                    .collect(ArrayList::new, accumulator2, ArrayList::addAll);   //collect the highest Pair<Student, Double(GPA)> in each major    
        }
        catch(IOException e)
        {
                    List<Pair<Student,Double>> result = new ArrayList();
                    return result;   //dummy output for exception
        }
        */
        
    }
     
}
