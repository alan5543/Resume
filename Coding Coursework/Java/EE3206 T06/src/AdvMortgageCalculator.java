// Student name: Yang Kai Yam
// Studnet ID  : 55676537

// Submission deadline: Friday, 30 Oct 2020, 11 am.

// Make suitable modifications to the class.
// Implement the ActionListener

import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.text.NumberFormat;
import javax.swing.JFrame;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;


public class AdvMortgageCalculator extends JFrame
{
    final int DEFAULT_FRAME_WIDTH = 550;
    final int DEFAULT_FRAME_HEIGHT = 240;

    private final JButton calculate, basicCalculator;
    private final JTextField loanAmount, loanTenor;
    private final JTextField installment1, installment2, interestRate1, interestRate2; 
    private final JTextField year_rate1;
    private final JLabel labelC, labelD;
    
    // Other instance variable where appropriate.
          
    public AdvMortgageCalculator()  // Can modify the constructor, but not changing the layout.
    {
        setSize(DEFAULT_FRAME_WIDTH, DEFAULT_FRAME_HEIGHT);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setTitle("Advanced Mortgage Calculator");
        ActionListener listener = new ButtonListener();  
        
        calculate = new JButton("Calculate");
        basicCalculator = new JButton("Basic Mortgage Calculator");
        calculate.addActionListener(listener);
        basicCalculator.addActionListener(listener);
        
        loanAmount = new JTextField(10);
        loanTenor = new JTextField(10);
        interestRate1 = new JTextField(10);
        interestRate2 = new JTextField(10);
        installment1 = new JTextField(10);
        installment1.setEditable(false);
        installment2 = new JTextField(10);
        installment2.setEditable(false);
        
        year_rate1 = new JTextField(2);
        
        JLabel label1 = new JLabel("Loan Amount (HKD)");
        JLabel label2 = new JLabel("Loan Tenor (year)");
        JLabel label3 = new JLabel("Mortgage Interest Rate (% p.a.)");
        JLabel label4 = new JLabel("Mortgage Interest Rate (% p.a.)");
        JLabel label5 = new JLabel("Monthly Repayment (HKD)");
        JLabel label6 = new JLabel("Monthly Repayment (HKD)");
        
        JLabel labelA = new JLabel("years 1 to ");
        JLabel labelB = new JLabel("remaining years");
        
        JLabel dummy1 = new JLabel(" ");
        JLabel dummy2 = new JLabel(" ");
        JLabel dummy3 = new JLabel(" ");

        
        JPanel subPanel = new JPanel();
        subPanel.setLayout(new GridLayout(1,2));
        subPanel.add(labelA);
        subPanel.add(year_rate1);
        
        labelC = new JLabel("To be determined");
        labelD = new JLabel("To be determined");
        
        JPanel panel = new JPanel();
        panel.setLayout(new GridLayout(7, 3));
        panel.add(label1);
        panel.add(loanAmount);
        panel.add(dummy1);
        
        panel.add(label2);
        panel.add(loanTenor);
        panel.add(dummy2);
        
        panel.add(label3);
        panel.add(interestRate1);
        panel.add(subPanel);
        
        panel.add(label4);
        panel.add(interestRate2);
        panel.add(labelB);
        
        panel.add(label5);
        panel.add(installment1);
        panel.add(labelC);
        
        panel.add(label6);
        panel.add(installment2);
        panel.add(labelD);
        
        panel.add(calculate);
        panel.add(basicCalculator);
        panel.add(dummy3);
       
        add(panel, "Center");
    }
    
    private class ButtonListener implements ActionListener 
    {
        // Implement this class.
        private double resultcalculate (double loan, int year, double rate)
        {
            rate = (rate/12)/100;
            year = year*12;
            double factor = Math.pow(1+rate, year)/(Math.pow(1+rate, year)-1);
            double result = loan*rate*factor;
            return result;
        }
        
        private double remain_outstanding(double loan, int year1, int year2, double rate)
        {
            rate = 1 + ((rate/12)/100);
            double first = Math.pow(rate, year1*12);
            double factor = (first-Math.pow(rate, year2*12))/(first-1);
            double result = loan*factor;
            return result;
        }
        
        public void actionPerformed(ActionEvent event)
        {
            Object source = event.getSource();
            if(source == calculate)              //react with the calculate button
            {
                double loan = 0, rate1 = 0, rate2 = 0;
                boolean numbercheck = true;      //determine the value is vaild or not
                int start = 0, year = 0;
                
                try{
                    loan = Double.parseDouble(loanAmount.getText());
                }
                catch(NumberFormatException e){  //exception with non-numerical input
                    loanAmount.setText("Error");
                    numbercheck = false;
                }
                try{
                    year = Integer.parseInt(loanTenor.getText());
                }
                catch(NumberFormatException e){  //exception with non-numerical input
                    loanTenor.setText("Error");
                    numbercheck = false;
                }
                try{
                    rate1 = Double.parseDouble(interestRate1.getText());
                }
                catch(NumberFormatException e){  //exception with non-numerical input
                    interestRate1.setText("Error");
                    numbercheck = false;
                }
                try{
                    start = Integer.parseInt(year_rate1.getText());
                }
                catch(NumberFormatException e){  //exception with non-numerical input
                    year_rate1.setText("Error");
                    numbercheck = false;
                }
                try{
                    rate2 = Double.parseDouble(interestRate2.getText());
                }
                catch(NumberFormatException e){  //exception with non-numerical input
                    interestRate2.setText("Error");
                    numbercheck = false;
                }
                
                if(numbercheck){
                    installment1.setText(String.format("%.02f", resultcalculate(loan, year, rate1))); 
                    labelC.setText("for years 1 to " + Integer.toString(start));  //output the 1st repayment
                    
                    double result = remain_outstanding(loan, year, start, rate1);
                    installment2.setText(String.format("%.02f", resultcalculate(result, year-start, rate2)));
                    labelD.setText("for years " + Integer.toString(start+1) + " to " + Integer.toString(year));  //output the 2nd repayment
                }
                else{
                    installment1.setText(" ");
                    installment2.setText(" ");
                }
            }
            else if(source == basicCalculator)  //switch to basic mode
            {
                BasicMortgageCalculator calculator = new BasicMortgageCalculator();
                calculator.setVisible(true);
                setVisible(false);
            }
        }
    }
}
