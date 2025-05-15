#ifndef DATA_H
#define DATA_H

#define NUMBER_OF_ITEMS 16  // number of available items in the database
#define NUMBER_OF_UNITS 16  // number of available units in the database
#define MAX_NAME 100        // maximum length of item or unit name
#define MIN_ARMY 1          // minimum number of combat units in an army
#define MAX_ARMY 5          // maximum number of combat units in an army
#define FIELD_ROWS 5        // number of battlefield rows
#define FIELD_COLS 5        // number of battlefield cols
#include <stdbool.h>


// Structure representing an item in inventory
typedef struct item {
    char name[MAX_NAME + 1];        // name of the item
    int att;                        // attack value
    int def;                        // defense value
    int slots;                      // number of inventory slots the item occupies
    int range;                      // attack range of the item
    int radius;                     // damage radius or area of effect
} ITEM;

// Structure representing a combat unit
typedef struct unit {
    char name[MAX_NAME + 1];    // name of the unit
    const ITEM *item1;          // first item in inventory
    const ITEM *item2;          // second item in inventory
    int hp;                     // hit points
    const char *description;    // description of the unit
} UNIT;

// Structure representing a window dimensions
typedef struct {
    int x;          // horizontal position (top-left corner)
    int y;          // vertical position (top-left corner)
    int width;      // width of the window in characters
    int height;     // height of the window in characters
} WindowDimensions;

// Structure representing a menu item (button)
typedef struct {
    char *label;    // display label of the menu item
    int selected;   // flag indicating if the item is currently selected (1 = selected, 0 = not selected)
} MenuItem;

// Structure representing a selectable unit in a menu
typedef struct {
    const UNIT* unit;   // pointer to the original unit data
    bool selected;      // whether this unit is currently selected by the player
    bool available;     // whether this unit can be selected (true = selectable)
} MENU_UNIT;

// Database of items available to combat units
extern const ITEM items[NUMBER_OF_ITEMS];
extern const UNIT units[NUMBER_OF_UNITS];

#endif // DATA_H
