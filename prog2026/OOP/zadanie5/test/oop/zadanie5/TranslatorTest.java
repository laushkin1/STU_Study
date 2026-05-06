package oop.zadanie5;

import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.*;

class TranslatorTest {

    @Test
    void setAndTranslate() {
        Translator tr1 = new Translator();
        Translator tr2 = new Translator();

        assertNull(tr1.translate("write"));

        tr1.set("write", "pisat");
        tr1.set("read", "citat");
        assertEquals("pisat", tr1.translate("write"));
        assertEquals("citat", tr1.translate("read"));
        assertNull(tr1.translate("pisat"));

        assertNull(tr2.translate("write"));
    }

    @Test
    void setAgain() {
        Translator tr = new Translator();

        tr.set("write", "pisat");
        assertEquals("pisat", tr.translate("write"));

        tr.set("write", "zapis");
        assertEquals("zapis", tr.translate("write"));
    }

    @Test
    void canTranslate() {
        Translator tr = new Translator();
        assertFalse(tr.canTranslate("write"));

        tr.set("write", "pisat");
        assertTrue(tr.canTranslate("write"));

        tr.set("write", "zapis");
        assertTrue(tr.canTranslate("write"));
    }

    @Test
    void size() {
        Translator tr1 = new Translator();
        Translator tr2 = new Translator();

        assertEquals(0, tr1.getSize());
        assertEquals(0, tr2.getSize());

        tr2.set("write", "pisat");
        assertEquals(0, tr1.getSize());
        assertEquals(1, tr2.getSize());

        tr2.set("read", "citat");
        assertEquals(0, tr1.getSize());
        assertEquals(2, tr2.getSize());
    }
}
