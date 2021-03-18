// Student name: Yang Kai Yam
// Student ID  : 55676537

// Submission deadline: Friday, 13 Nov 2020, 11 am

import java.io.File;
import java.io.IOException;
import java.text.NumberFormat;
import java.util.ArrayList;
import java.util.Arrays;
import static java.util.Comparator.comparing;
import java.util.List;
import java.util.Map;
import java.util.Optional;
import java.util.Scanner;
import java.util.function.BiConsumer;
import java.util.function.Function;
import java.util.stream.Collectors;

public class Tut_08 
{
    public static void main(String[] args) throws IOException
    {
        // Read a data file of grades of a semester
        ArrayList<Triple<String, Integer, String>> grades = new ArrayList();
        try (Scanner sc1 = new Scanner(new File("grades.txt")))
        {
            while (sc1.hasNextLine())
            {
                String line = sc1.nextLine();
                String[] token = line.split("\\s");
                grades.add(new Triple(token[0], Integer.parseInt(token[1]), token[2]));
            }
        }
        
        // Your codes to read a data file of courses into Course.courseList
        try(Scanner sc2 = new Scanner(new File("courses.txt")))
        {
            while(sc2.hasNextLine())
            {
                String line = sc2.nextLine();
                String[] token = line.split("\\s");
                
                String code = token[0];                //determine the courseCode
                int credit = Integer.parseInt(token[1]); //determine the courseCredit
                String title = "";
                for(int i=2; i<token.length; i++){
                    title = title + token[i];          //determine the courseTitle
                    if(i<token.length-1)
                        title = title + " ";
                }
                Course input = new Course(code, title, credit);
                Course.add(input);
            }
        }
        //Course.show();
        
        // Your codes to read a data file of students into Student.studentList
        try(Scanner sc3 = new Scanner(new File("students.txt")))
        {
            while(sc3.hasNextLine())
            {
                String line = sc3.nextLine();
                String[] token = line.split("\\s");
                
                int id = Integer.parseInt(token[0]);     //determine the SID
                String major = token[1];               //determine the majorName
                String name = "";
                for(int i=2; i<token.length; i++){
                    name = name + token[i];            //determine the studentName
                    if(i<token.length-1)
                        name = name + " ";
                }
                Student.add(new Student(name, id, major));
            }
            //Student.show();
        }
        
        // Find the students with the highest GPA in each major
        // Implement the computation using Functional Programming approach

        List<Pair<Student, Double>> result = bestStudents(grades);
        
        NumberFormat nf = NumberFormat.getInstance();
        nf.setMaximumFractionDigits(2);
        for (Pair<Student, Double> item : result)
        {
            Student s = item.getFirst();
            System.out.println(s.getMajor() + ", " + s.getSid() + ", " + s.getName() +
                               ", GPA = " + nf.format(item.getSecond()));
        }        
    }
     
    static List<Pair<Student, Double>> bestStudents(List<Triple<String, Integer, String>> grades)
    {
        //FunctionUtil.forEach(grades, System.out::println);
        //grades.sort((a,b)->(a.getSecond()-b.getSecond()));
        
        //grades.sort(comparing(triple -> triple.getFirst()));
        //grades.sort(comparing(Triple<String, Integer, String>::getFirst).thenComparing(Triple<String, Integer, String>::getThird));
        //grades.forEach(p -> System.out.println(p.getFirst()));
        
        
        
        BiConsumer<ArrayList<Triple<Integer, Double, Integer>>, Triple<String, Integer, String>> acc =
                (res, v)->{
                  Course item = Course.search(v.getFirst()).get();
                  double gpa = Course.gradePoint(v.getThird())*item.getCredit();
                  if(res.isEmpty())
                      res.add(new Triple(v.getSecond(), gpa, item.getCredit()));
                  else{
                      Triple<Integer, Double, Integer> s = res.get(res.size()-1);
                      if(s.getFirst().equals(v.getSecond())){
                          s.setSecond(s.getSecond()+gpa);
                          s.setThird(s.getThird()+item.getCredit());
                      }
                      else
                          res.add(new Triple(v.getSecond(), gpa, item.getCredit()));
                  }
                };
        
        Function<Triple<Integer, Double, Integer>, Pair<Student,Double>> mapp =
                a -> {
                    double cgpa = a.getSecond()/a.getThird();
                    Student item = Student.search(a.getFirst()).get();
                    return new Pair(item, cgpa);
                };
        
        List<Integer  > nums = Arrays.asList(1,2,3,4);
        int sum2 = nums.stream()
                .reduce(0,(o,n)->o+n);
        
        List<Double> numbers = Arrays.asList(1.2,1.3,3.3,2.3);
        double sum = numbers.stream()
                .mapToDouble(p->p)
                .average()
                .getAsDouble();
        
        Map<Long,Double> kg = numbers.stream()
                .collect(Collectors.toMap(p->1L,p->p,(o,n)->o+n));
        
        System.out.println("SUM: " + sum);
        System.out.println(sum2/nums.size());
        
        BiConsumer<List<Triple<Integer, Double, Integer>>, Triple<String, Integer, String>> action1 =
                (result, v)->{
                    double gp = Course.gradePoint(v.getThird());
                    Optional<Student> a = Student.search(v.getSecond());
                    Optional<Course> b = Course.search(v.getFirst());
                    int credit = b.get().getCredit();
                    
                    if(result.isEmpty()){
                        result.add(new Triple(v.getSecond(), gp*credit, credit));
                    }
                    else{
                        Triple<Integer, Double, Integer> item = result.get(result.size()-1);
                        if(v.getSecond().equals(item.getFirst())){
                            item.setSecond(item.getSecond() + gp*credit);
                            item.setThird(item.getThird() + credit);
                        }
                        else{
                            result.add(new Triple(v.getSecond(), gp*credit, credit));
                        }
                    }
                };
        
        Function<Triple<Integer, Double, Integer>, Pair<Student, Double>> mapper =
                t -> {
                    double gpa = t.getThird() > 0 ? t.getSecond()/t.getThird() : 0;
                    Student s = Student.search(t.getFirst()).get();
                    return new Pair(s, gpa);
                };
        
        BiConsumer<List<Pair<Student, Double>>, Pair<Student, Double>>
                action2 = (result, p) ->{
                    if(result.isEmpty()){
                        result.add(new Pair(p.getFirst(), p.getSecond()));
                    }
                    else{
                        Pair<Student, Double> item = result.get(result.size() - 1);
                        if(item.getFirst().getMajor().equals(p.getFirst().getMajor()))
                        {
                            if(p.getSecond()>item.getSecond()){
                                item.setFirst(p.getFirst());
                                item.setSecond(p.getSecond());
                            }
                        }
                        else
                            result.add(new Pair(p.getFirst(), p.getSecond()));
                    }
                };

         BiConsumer<ArrayList<Pair<Student, Double>>, Map.Entry<Integer, Double>> accd =
                 (r,v)->{
                     r.add(new Pair(Student.search(v.getKey()).get(), v.getValue()));
                 };
         
         BiConsumer<ArrayList<Integer>, Map.Entry<Integer, Double>> fil =
                 (r,v)->{
                    r.add(v.getKey());
                 };
         
//List<Pair<Student, Double>> e = 
            grades.stream()
                .collect(ArrayList::new, acc, ArrayList::addAll)
                .stream()
                .collect(Collectors.toMap(Triple<Integer, Double, Integer>::getFirst, (p->p.getSecond()/p.getThird())))
                .entrySet()
                .stream()
                .filter(p->p.getValue()>3.5)
                .collect(Collectors.toMap(Map.Entry::getKey, Map.Entry::getValue)) //Map.enry to map Again
                .keySet() //Directly take the map key, not use the map entry
                .stream()
                .collect(Collectors.toList()) //collect map key to list
                .forEach(System.out::println);
          //      .entrySet()
          //      .stream()
          //      .filter(p->p.getValue()>3.5)
                
          //      .collect(ArrayList::new, accd, ArrayList::addAll);
                
         //  for (Map.Entry<Integer, Double> entry : e.entrySet()) {
         //       System.out.println("Key : " + entry.getKey() + " Value : " + entry.getValue());
         //    }        
        List<Pair<Student, Double>> a = new ArrayList();
        return a;
        
  /*      List<Triple<Integer, Double, Integer>> List1 = FunctionUtil.transform(grades, action1);
        List<Pair<Student, Double>> temp2 = FunctionUtil.map(List1, mapper);
        temp2.sort((a,b) -> a.getFirst().getMajor().compareTo(b.getFirst().getMajor()));
        List<Pair<Student, Double>> result = FunctionUtil.transform(temp2, action2);
        List<Pair<Student, Double>> r = new ArrayList();
        return result; */
        
        // Input list of Triple<courseCode, sid, grade>, already sorted by sid
        // All course grades in the input list are between A+ to F.
        // That is, all course grades are counted in the GPA calculation.

        // You may assume that the data in the data files are consistent.
        // That is, all student IDs, and course codes are valid.

        // Implement the computation using Functional Programming approach.        
/*
        BiConsumer<List<Triple<Course, Student, String>>, Triple<String, Integer, String>> action =
                (result, v)->{
                    Optional<Student> s = Student.search(v.getSecond());  //search the Student from the given sid
                    Optional<Course> c = Course.search(v.getFirst());     //search the Course from the given courseCode
                    if(s.isPresent() && c.isPresent()){
                        result.add(new Triple(c.get(), s.get(), v.getThird()));
                    }
                };
        
        List<Triple<Course, Student, String>>
                collect = FunctionUtil.transform(grades, action); //transform Triple<String(courseCode), Integer(ID), String(Grade)> to
                                                                //Triple<Course, Student, String(Grade)>
        
        BiConsumer<List<Triple<Student, Double, Integer>>, Triple<Course, Student, String>> calculate =
                (result, v)->{
                    int credit = v.getFirst().getCredit();
                    Double total = Course.gradePoint(v.getThird())* credit;
                  if(result.isEmpty())
                      result.add(new Triple(v.getSecond(), total, credit));
                  else
                  {
                      Triple<Student, Double, Integer> item = result.get(result.size()-1);
                      if(item.getFirst().equals(v.getSecond())){
                          item.setSecond(item.getSecond() + total); //generate the accumulated mark
                          item.setThird(item.getThird() + credit);  //generate the total credit
                      }
                      else
                          result.add(new Triple(v.getSecond(), total, credit));
                  }
                };
        
        List<Triple<Student, Double, Integer>>                          //transform Triple<Course, Student, String(Grade)> to
                finalcount = FunctionUtil.transform(collect, calculate); //Triple<Student, Double(accumulated mark), Integer(total credit)>
        
        List<Pair<Student,Double>>                                     //transform to Pair<Student, Double(gpa)>
                flist = FunctionUtil.transform(finalcount, (gpa,s)->gpa.add(new Pair(s.getFirst(), (double)Math.round(s.getSecond()/s.getThird()*100)/100))); //round up the GPA
        flist.sort((p1,p2)->(p1.getFirst().getMajor()).compareTo(p2.getFirst().getMajor()));

        
        List<Pair<Student,Double>>                                    //transform the Pair<Student, Double(gpa)>
                result = FunctionUtil.transform(flist, (output, v)->{  //to Pair<Student, Double(highest gpa)>
                    if(output.isEmpty())
                      output.add(new Pair(v.getFirst(), v.getSecond()));
                  else{
                      Pair<Student,Double> item = output.get(output.size()-1);
                      if(item.getFirst().getMajor().equals(v.getFirst().getMajor())){
                          if(v.getSecond() > item.getSecond()){              //whether v's gpa is higher than item's gpa (same major)
                              item.setFirst(v.getFirst());
                              item.setSecond(v.getSecond());
                          }
                      }
                      else
                          output.add(new Pair(v.getFirst(),v.getSecond()));  //switch to next major
                  }
                });

        return result; */
    }
     
}
