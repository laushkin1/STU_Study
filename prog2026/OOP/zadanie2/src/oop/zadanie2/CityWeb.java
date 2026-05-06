package oop.zadanie2;

public class CityWeb extends AbstractWeb{
    private String[] cities;

    public CityWeb(User[] users, String[] cities) {
        super(users);
        this.cities = cities;
    }

    @Override
    public Response getById(Request request) {
        if (this.authenticate(request)) {
            int index = request.getId();
            if (index >= 0 && index < cities.length){
                return new Response(true, this.cities[index]);
            }
        }
        return new Response(false, "");
    }

    @Override
    public Response getAll(Request request) {
        if (this.authenticate(request)) {
            return new Response(true, String.join(", ", this.cities));
        }
        return new Response(false, "");
    }
}
