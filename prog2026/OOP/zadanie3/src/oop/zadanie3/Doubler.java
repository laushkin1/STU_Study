package oop.zadanie3;

public class Doubler extends AbstractFunction {

    public Doubler(int input) {
        super(input);
    }

    public Doubler() {
        super(0);
    }

    @Override
    public int getOutput() {
        return this.input * this.input;
    }
}
