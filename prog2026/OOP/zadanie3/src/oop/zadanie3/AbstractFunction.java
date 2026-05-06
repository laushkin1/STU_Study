package oop.zadanie3;

public abstract class AbstractFunction implements Function {
    protected int input;

    public AbstractFunction(int input) {
        this.input = input;
    }

    @Override
    public void setInput(int input) {
        this.input = input;
    }
}
