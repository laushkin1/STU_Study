package oop.zadanie2;

public class DayWeb extends AbstractWeb{
    private final String[] week = {"pondelok", "utorok" , "streda", "stvrtok", "piatok", "sobota", "nedela"};

    public DayWeb(User[] users) {
        super(users);
    }

    @Override
    public Response getById(Request request) {
        if (this.authenticate(request)) {
            int index = request.getId();
            if (index >= 1 && index <= 7){
                return new Response(true, this.week[index-1]);
            }
        }
        return new Response(false, "");
    }

    @Override
    public Response getAll(Request request) {
        if (this.authenticate(request)) {
            return new Response(true, String.join(", ", this.week));
        }
        return new Response(false, "");
    }
}
