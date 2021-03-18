// Student name: Yang Kai Yam
// Student ID  : 55676537

// Implement the method gradePoint() in this class.

// Submission deadline: Friday, 18 Sept 2020, 11 am
// Upload 2 files, Grade.java and Student.java to Canvas

public class Grade 
{
    private final String courseCode;
    private String grade;             // letter grade, A+, A, A-, B+, B, etc.
    private final int credit;         // number of credits
    private final char mode;          // grade mode, L = letter grade, P = pass/fail
                                      //             N = not counted in GPA calculation
    private final String sem;         // semester, e.g. 2019-09

    
    public Grade(String c, String g, int cu, char m, String s)
    {
        courseCode = c;
        grade = g;
        credit = cu;
        mode = m;
        sem = s;
    }
    
    public String getCourseCode()
    {
        return courseCode;
    }
    
    public String getGrade()
    {
        return grade;
    }
    
    public String getSem()
    {
        return sem;
    }
    
    public char getMode()
    {
        return mode;
    }
    
    public int getCredit()
    {
        return credit;
    }
    
    @Override
    public String toString()
    {
        return courseCode + "\t" + grade + "\t" + credit + "\t" + mode + "\t" + sem;
    }
    
    public static double gradePoint(String letterGrade)
    {

        if (null != letterGrade)
            // Method to convert a letter grade to numeric grade point.
        // Your codes.
        switch (letterGrade) {
            case "A+":
                return 4.3;
            case "A":
                return 4;
            case "A-":
                return 3.7;
            case "B+":
                return 3.3;
            case "B":
                return 3;
            case "B-":
                return 2.7;
            case "C+":
                return 2.3;
            case "C":
                return 2;
            case "C-":
                return 1.7;
            case "D":
                return 1;
            default:
                break;
        }
        
        return 0;
    }    
}
