// DO not modify this class.
// No need to upload this file to Canvas.

public class User implements Comparable<User>
{
    private final String name;
    private final String tel;  // A User is uniquely identified by tel
    
    public User(String n, String t)
    {
        name = n;
        tel = t;
    }

    public String getName()
    {
        return name;
    }
    
    public String getTel()
    {
        return tel;
    }
    
    @Override
    public String toString()
    {
        return tel + " " + name;
    }

    @Override
    public int compareTo(User other) 
    {
        return tel.compareTo(other.tel);
    }
    
    @Override
    public boolean equals(Object other)
    {
        if(other instanceof User)
        {
            User u = (User)other;
            return name.equalsIgnoreCase(u.name) && tel.equals(u.tel);
        }
        else
            return false;
    }
    
    @Override
    public int hashCode()
    {
        return name.toUpperCase().concat(tel).hashCode();
    }
    
}

