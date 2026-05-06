package oop.zadanie2;

import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.*;

class CityWebTest {

    private User[] createUsers() {
        return new User[] {
                new User("peter", "abc"),
                new User("pavol", "efgh"),
        };
    }

    private User[] createUsers2() {
        return new User[] {
                new User("tomas", "ij"),
                new User("milos", "kl"),
        };
    }

    private String[] createCities() {
        return new String[] {
                "Bratislava",
                "Trnava",
                "Nitra"
        };
    }

    private String[] createCities2() {
        return new String[] {
                "Kosice",
                "Presov"
        };
    }

    @Test
    void konstruktorSoSpravnymiParamerami() {
        CityWeb webPage = new CityWeb(createUsers(), createCities());
    }

    @Test
    void byId() {
        CityWeb w1 = new CityWeb(createUsers(), createCities());
        CityWeb w2 = new CityWeb(createUsers2(), createCities2());

        Response r1a = w1.getById(new Request("peter", "abc", 0));
        assertTrue(r1a.getStatus());
        assertEquals("Bratislava", r1a.getData());

        Response r1b = w1.getById(new Request("pavol", "efgh", 2));
        assertTrue(r1b.getStatus());
        assertEquals("Nitra", r1b.getData());

        Response r2a = w2.getById(new Request("tomas", "ij", 1));
        assertTrue(r2a.getStatus());
        assertEquals("Presov", r2a.getData());
    }

    @Test
    void byId_badAuthentication() {
        CityWeb w = new CityWeb(createUsers(), createCities());

        Response r1 = w.getById(new Request("peter", "abcd", 0));
        assertFalse(r1.getStatus());
        assertEquals("", r1.getData());

        Response r2 = w.getById(new Request("petra", "abc", 0));
        assertFalse(r2.getStatus());
        assertEquals("", r2.getData());
    }

    @Test
    void byId_badId() {
        CityWeb w = new CityWeb(createUsers(), createCities());

        Response r1 = w.getById(new Request("peter", "abc", -1));
        assertFalse(r1.getStatus());
        assertEquals("", r1.getData());

        Response r2 = w.getById(new Request("peter", "abc", 3));
        assertFalse(r2.getStatus());
        assertEquals("", r2.getData());
    }

    @Test
    void all() {
        CityWeb w1 = new CityWeb(createUsers(), createCities());
        CityWeb w2 = new CityWeb(createUsers2(), createCities2());

        Response r1 = w1.getAll(new Request("peter", "abc", 0));
        assertTrue(r1.getStatus());
        assertEquals("Bratislava, Trnava, Nitra", r1.getData());

        Response r2 = w2.getAll(new Request("tomas", "ij", 1));
        assertTrue(r2.getStatus());
        assertEquals("Kosice, Presov", r2.getData());
    }

    @Test
    void all_badAuthentication() {
        CityWeb w = new CityWeb(createUsers(), createCities());

        Response r1 = w.getAll(new Request("peter", "abcd", 0));
        assertFalse(r1.getStatus());
        assertEquals("", r1.getData());

        Response r2 = w.getById(new Request("petra", "abc", 0));
        assertFalse(r2.getStatus());
        assertEquals("", r2.getData());
    }
}
