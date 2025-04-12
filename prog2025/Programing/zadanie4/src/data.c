/*
 *  Definicie
 */

#include "data.h"

// databaza predmetov, ktore moze mat bojova jednotka v inventari
const ITEM items[NUMBER_OF_ITEMS] = {
        // predmety, ktore zaberaju 1 slot, zoradene podla atributu 'att'
        {.name = "wand", .att = 12, .def = 4, .slots = 1, .range = 4, .radius = 2}, // kuzelna palica   0
        {.name = "fireball", .att = 11, .def = 0, .slots = 1, .range = 3, .radius = 3}, // ohniva gula  1
        {.name = "sword", .att = 9, .def = 2, .slots = 1, .range = 0, .radius = 0}, // mec              2
        {.name = "spear", .att = 6, .def = 1, .slots = 1, .range = 1, .radius = 1}, // ostep            3
        {.name = "dagger", .att = 4, .def = 0, .slots = 1, .range = 0, .radius = 0}, // dyka            4
        {.name = "rock", .att = 3, .def = 0, .slots = 1, .range = 2, .radius = 1}, // skala             5
        {.name = "armor", .att = 2, .def = 7, .slots = 1, .range = 0, .radius = 0}, // brnenie          6
        {.name = "shield", .att = 2, .def = 6, .slots = 1, .range = 0, .radius = 0}, // stit            7
        {.name = "gloves", .att = 1, .def = 4, .slots = 1, .range = 0, .radius = 0}, // rukavice        8
        {.name = "helmet", .att = 1, .def = 5, .slots = 1, .range = 0, .radius = 0}, // helma           9
        {.name = "aura", .att = 0, .def = 8, .slots = 1, .range = 0, .radius = 0}, // ochranna aura     10

        // predmety, ktore zaberaju 2 sloty, zoradene podla atributu 'att'
        {.name = "cannon", .att = 12, .def = 0, .slots = 2, .range = 4, .radius = 4}, // kanon          11
        {.name = "axe", .att = 10, .def = 2, .slots = 2, .range = 1, .radius = 1}, // sekera            12
        {.name = "hammer", .att = 8, .def = 2, .slots = 2, .range = 1, .radius = 2}, // kladivo         13
        {.name = "crossbow", .att = 5, .def = 1, .slots = 2, .range = 3, .radius = 0}, // kusa          14
        {.name = "slingshot", .att = 2, .def = 0, .slots = 2, .range = 2, .radius = 1} // prak          15
};
