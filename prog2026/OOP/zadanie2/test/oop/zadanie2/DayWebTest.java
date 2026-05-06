package oop.zadanie2;

import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.*;

class DayWebTest {
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

    @Test
    void konstruktorSoSpravnymiParametrami() {
        DayWeb w = new DayWeb(createUsers());
    }

    @Test
    void byId() {
        DayWeb w1 = new DayWeb(createUsers());
        DayWeb w2 = new DayWeb(createUsers2());

        Response r1a = w1.getById(new Request("pavol", "efgh", 1));
        assertTrue(r1a.getStatus());
        assertEquals("pondelok", r1a.getData());

        Response r1b = w1.getById(new Request("pavol", "efgh", 2));
        assertTrue(r1b.getStatus());
        assertEquals("utorok", r1b.getData());

        Response r1c = w1.getById(new Request("pavol", "efgh", 7));
        assertTrue(r1c.getStatus());
        assertEquals("nedela", r1c.getData());

        Response r2a = w2.getById(new Request("milos", "kl", 5));
        assertTrue(r2a.getStatus());
        assertEquals("piatok", r2a.getData());

    }

    @Test
    void byId_badAuthentication() {
        DayWeb w = new DayWeb(createUsers());

        Response r1 = w.getById(new Request("peter", "abcd", 0));
        assertFalse(r1.getStatus());
        assertEquals("", r1.getData());

        Response r2 = w.getById(new Request("petra", "abc", 0));
        assertFalse(r2.getStatus());
        assertEquals("", r2.getData());
    }

    @Test
    void byId_badId() {
        DayWeb w = new DayWeb(createUsers());

        Response r1 = w.getById(new Request("peter", "abc", -1));
        assertFalse(r1.getStatus());
        assertEquals("", r1.getData());

        Response r2 = w.getById(new Request("peter", "abc", 0));
        assertFalse(r2.getStatus());
        assertEquals("", r2.getData());

        Response r3 = w.getById(new Request("peter", "abc", 8));
        assertFalse(r3.getStatus());
        assertEquals("", r3.getData());

        Response r4 = w.getById(new Request("peter", "abc", 9));
        assertFalse(r4.getStatus());
        assertEquals("", r4.getData());
    }

    @Test
    void all() {
        DayWeb w = new DayWeb(createUsers());

        Response r = w.getAll(new Request("pavol", "efgh", 0));
        assertTrue(r.getStatus());
        assertEquals("pondelok, utorok, streda, stvrtok, piatok, sobota, nedela", r.getData());
    }

    @Test
    void all_badAuthentication() {
        DayWeb w = new DayWeb(createUsers());

        Response r1 = w.getAll(new Request("peter", "abcd", 0));
        assertFalse(r1.getStatus());
        assertEquals("", r1.getData());

        Response r2 = w.getAll(new Request("petra", "abc", 0));
        assertFalse(r2.getStatus());
        assertEquals("", r2.getData());
    }
}
