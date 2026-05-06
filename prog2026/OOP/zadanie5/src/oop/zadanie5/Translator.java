package oop.zadanie5;

import java.util.Map;
import java.util.HashMap;


public class Translator {
    private Map<String, String> dic;

    public Translator() {
        this.dic = new HashMap<>();
    }

    public void set(String word, String translation) {
        dic.put(word, translation);
    }

    public String translate(String word) {
        return dic.get(word);
    }

    public boolean canTranslate(String word) {
        return dic.containsKey(word);
    }

    public int getSize(){
        return dic.size();
    }
}
