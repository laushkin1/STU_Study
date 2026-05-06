package oop.zadanie3;

public class Squarer extends AbstractFunction {

    public Squarer(int input) {
        super(input);
    }

    public Squarer() {
        super(0);
    }

    @Override
    public int getOutput() {
        return this.input * this.input;
    }
}