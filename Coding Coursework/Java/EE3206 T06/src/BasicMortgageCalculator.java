// Student name: Yang Kai Yam
// Studnet ID  : 55676537

// Submission deadline: Friday, 30 Oct 2020, 11 am.

// Make suitable modifications to the class.
// Implement the ActionListener

import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.text.NumberFormat;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;


public class BasicMortgageCalculator extends JFrame
{
    final int DEFAULT_FRAME_WIDTH = 420;
    final int DEFAULT_FRAME_HEIGHT = 200;

    private final JButton calculate, advCalculator;
    private final JTextField loanAmount, interestRate, loanTenor, installment;
    public AdvMortgageCalculator calculator = new AdvMortgageCalculator();
    // Other instance variable where appropriate.
     
    public BasicMortgageCalculator()  // Can modify the constructor, but not changing the layout.
    {
        setSize(DEFAULT_FRAME_WIDTH, DEFAULT_FRAME_HEIGHT);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setTitle("Basic Mortgage Calculator");
        ActionListener listener = new ButtonListener();  
        
        calculate = new JButton("Calculate");
        advCalculator = new JButton("Adv. Mortgage Calculator");
        calculate.addActionListener(listener);
        advCalculator.addActionListener(listener);
        
        loanAmount = new JTextField(10);
        loanTenor = new JTextField(10);
        interestRate = new JTextField(10);
        installment = new JTextField(10);
        installment.setEditable(false);
        
        JLabel label1 = new JLabel("Loan Amount (HKD)");
        JLabel label2 = new JLabel("Loan Tenor (year)");
        JLabel label3 = new JLabel("Mortgage Interest Rate (% p.a.)");
        JLabel label4 = new JLabel("Monthly Repayment (HKD)");
        
        JPanel panel = new JPanel();
        panel.setLayout(new GridLayout(5, 2));
        panel.add(label1);
        panel.add(loanAmount);
        panel.add(label2);
        panel.add(loanTenor);
        panel.add(label3);
        panel.add(interestRate);
        panel.add(label4);
        panel.add(installment);
        panel.add(calculate);
        panel.add(advCalculator);
        
        add(panel, "Center");
    }
    
    private class ButtonListener implements ActionListener 
    {
        // Implement this class.
        @Override
        public void actionPerformed(java.awt.event.ActionEvent event)
        {
            Object source = event.getSource();
            if(source == calculate)             //react with the calculate button
            {
                double loan = 0, rate = 0, result = 0;
                int year = 0;
                boolean numbercheck = true;
                try{
                    loan = Double.parseDouble(loanAmount.getText());
                }
                catch(NumberFormatException e){             //exception case without numerical input
                    loanAmount.setText("Error");
                    numbercheck = false;
                }
                try{
                    year = Integer.parseInt(loanTenor.getText())* 12;
                }
                catch(NumberFormatException e){             //exception case without numerical input
                    loanTenor.setText("Error");
                    numbercheck = false;
                }
                try{
                    rate = Double.parseDouble(interestRate.getText());
                }
                catch(NumberFormatException e){             //exception case without numerical input
                    interestRate.setText("Error");
                    numbercheck = false;
                }
                result = resultcalculate(loan, year, rate);
                String output = String.format("%.02f", result);
                if(numbercheck)                 // check whether have output or not
                    installment.setText(output);
                else
                    installment.setText(" ");
            }
            else if(source == advCalculator) // switch to another mode
            {
                calculator.setVisible(true);
                setVisible(false);
            }
        }
        private double resultcalculate (double loan, int year, double rate) //fixed monthly payment
        {
            rate = (rate/12)/100;
            double factor = Math.pow(1+rate, year)/(Math.pow(1+rate, year)-1);
            double result = loan*rate*factor;
            return result;
        }
        
    }
}
