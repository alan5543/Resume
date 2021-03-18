// Student name: Yang Kai Yam
// Student ID  : 55676537

// Submission deadline: Friday, 13 Nov 2020, 11 am

// Implement the method search.
// Do not change other parts of the class.

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Optional;

public class Course 
{
    private static List<Course> courseList = new ArrayList();
    
    private String courseCode;
    private String courseTitle;
    private Integer credit;
   
    
    public Course(String cc, String t, int d)
    {
        courseCode = cc;
        courseTitle = t;
        credit = d;        
    }
        
    public String getCourseCode()
    {
        return courseCode;
    }
    
    public String getCourseTitle()
    {
        return courseTitle;
    }
    
    public Integer getCredit()
    {
        return credit;
    }
    
    public static void add(Course c)
    {
        courseList.add(c);
    }
    
    public static double gradePoint(String letterGrade)
    {
        double gp = 0;
        if (letterGrade.charAt(0) >= 'A' && letterGrade.charAt(0) <= 'D')
        {
            gp = 'E' - letterGrade.charAt(0);
            if (letterGrade.length() > 1)
                if (letterGrade.charAt(1) == '+')
                    gp += 0.3;
                else if (letterGrade.charAt(1) == '-')
                    gp -= 0.3;            
        }
        return gp;
    }    
    
    public static Optional<Course> search(String courseCode)
    {
        // Implement this method.
        for(Course s : courseList)         //search the Course with the same courseCode
            if(s.getCourseCode().equals(courseCode))
                return Optional.of(s);
        
        return Optional.empty();
    }
}
