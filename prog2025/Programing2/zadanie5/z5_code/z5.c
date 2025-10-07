#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

// NEMENIT, inak vas kod neprejde testom !!!
#include "data.h"

typedef struct {
    ITEM *items;
    int count; // size_t count;
} JSONObject;

typedef struct {
    UNIT *units;
    int count;
} TXTObject;


void skip_space(char **pointer);
bool expect_char(char **pointer, char c);
char *get_string(char **pointer);
bool get_int(char **pointer, int *number);

char* read_file_to_buffer(char *path);
int read_json(char *pointer, JSONObject *outObject);
int read_txt(char *pointer, TXTObject *outObject);
bool error_text_file(int err_num);

void print_army(TXTObject army1, TXTObject army2);


JSONObject jsonFileObject;

int main(const int argc, char *argv[]) {
	// if cmd err
	if (!(argc == 3 || argc == 1)){
		fprintf(stderr, "%s", ERR_CMD);
		return 0;
	}

	// read json and catch all errors
	char *buffer = read_file_to_buffer(ITEMS_FILE);
	if (!buffer) {
		fprintf(stderr, "%s", ERR_FILE);
		goto PROGRAM_END;
	}

	int err_num = read_json(buffer, &jsonFileObject);
	if (err_num == 1){ fprintf(stderr, "%s", ERR_MISSING_ATTRIBUTE);	goto PROGRAM_END; }
	if (err_num == 2){ fprintf(stderr, "%s", ERR_MISSING_VALUE); 		goto PROGRAM_END; }
	if (err_num == 3){ fprintf(stderr, "%s", ERR_BAD_VALUE); 			goto PROGRAM_END; }
	

	if (argc == 1){
		// Output items if no arguments
		for (int i = 0; i < jsonFileObject.count; i++){
			fprintf(stdout, "Name: %s\n", jsonFileObject.items[i].name);
			fprintf(stdout, "Attack: %d\n", jsonFileObject.items[i].att);
			fprintf(stdout, "Defense: %d\n", jsonFileObject.items[i].def);
			fprintf(stdout, "Slots: %d\n", jsonFileObject.items[i].slots);
			fprintf(stdout, "Range: %d\n", jsonFileObject.items[i].range);
			fprintf(stdout, "Radius: %d\n", jsonFileObject.items[i].radius);

			printf("\n");
		}
		
	} else if (argc == 3){
		// open file
		char *army1Path = argv[1];
		char *army2Path = argv[2];

		char *firstFileBuffer = read_file_to_buffer(army1Path);
		if (!firstFileBuffer) {
			fprintf(stderr, "%s", ERR_FILE);
			free(firstFileBuffer);
			goto PROGRAM_END;
		}
		TXTObject objectArmy1;
		// check if army valide
		if (error_text_file(read_txt(firstFileBuffer, &objectArmy1))) goto ELSE_ENDF;


		char *secondFileBuffer = read_file_to_buffer(army2Path);
		if (!secondFileBuffer) {
			fprintf(stderr, "%s", ERR_FILE);
			free(secondFileBuffer);
			goto PROGRAM_END;
		}
		TXTObject objectArmy2;
		// check if army valide
		if (error_text_file(read_txt(secondFileBuffer, &objectArmy2))) goto ELSE_END;

		// Output armis
		print_army(objectArmy1, objectArmy2);

		ELSE_END: free(secondFileBuffer);
		ELSE_ENDF: free(firstFileBuffer);
		
	}

	PROGRAM_END:
	free(jsonFileObject.items);
	free(buffer);

	return 0;
}


void skip_space(char **pointer){
	while (isspace(**pointer)) (*pointer)++;
}

bool expect_char(char **pointer, char c) {
    skip_space(pointer);
    if (**pointer == c) return true;
    return false;
}

char *get_string(char **pointer){
	skip_space(pointer);
	if (**pointer != '"') return NULL;
	(*pointer)++;
	const char *start = *pointer;
	while (**pointer && **pointer != '"') (*pointer)++;
	size_t len = *pointer - start;
	char *string = malloc(len+1);
	memcpy(string, start, len);
	string[len] = '\0';
	if (**pointer == '"') (*pointer)++;
	return string;
}

bool get_int(char **pointer, int *number) {
    skip_space(pointer);

    char *start = *pointer;
    int num = 0;

    while (isdigit((unsigned char)**pointer)) {
        num = num * 10 + (**pointer - '0');
        (*pointer)++;
    }
	
    if (*pointer == start) return false;

    *number = num;
    return true;
}


char* read_file_to_buffer(char *path){
	FILE *file = fopen(path, "r");
    if (!file) return NULL;

	fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    char *buffer = malloc(size + 1);
    if (!buffer) {
        fclose(file);
        return NULL;
    }

	size_t read_bytes = fread(buffer, 1, size, file);
    buffer[read_bytes] = '\0';

	fclose(file);
    return buffer;
}

int read_json(char *pointer, JSONObject *outObject){
	outObject->items = NULL;
	outObject->count = 0;

	expect_char(&pointer, '[');

	while(*pointer != ']'){

		char *unitName;
		int attValue;
		int defValue;
		int slotsValue;
		int rangeValue;
		int radiusValue;

		pointer++;
		// ----------------FIRST-------------------
		if (!expect_char(&pointer, '{')) return 0;
		pointer++;

		// Name
		char *keyName = get_string(&pointer);
		if (!keyName) return 3; // TODO: handle error -> key is not a string
		
		expect_char(&pointer, ':'); // skip ':'
		pointer++;
		if(*pointer == ',') return 2;
		
		if (strcmp(keyName, "name") == 0){
			unitName = get_string(&pointer);
			if (!unitName){
				free(keyName);
				return 3; // TODO handle error -> val is not a string
			}
		} else{
			return 1; // TODO handle error -> key is not a name
		}

		expect_char(&pointer, ','); // skip ','
		if(*pointer != ',') return 1;
		pointer++;

		// Att
		char *keyAtt = get_string(&pointer);
		if (!keyAtt) return 3; // TODO: handle error -> key is not a string
		
		expect_char(&pointer, ':'); // skip ':'
		pointer++;
		if(*pointer == ',') return 2;

		if (strcmp(keyAtt, "att") == 0){
			if (!get_int(&pointer, &attValue)){
				free(keyName);
				free(keyAtt);
				return 3; // TODO: handle error -> val is not a num
			}
		} else{
			return 1; // TODO handle error -> key is not a name
		}
		
		expect_char(&pointer, ','); // skip ','
		if(*pointer != ',') return 1;
		pointer++;
		
		// Def
		
		char *keyDef = get_string(&pointer);
		if (!keyDef) return 3; // TODO: handle error -> key is not a string

		expect_char(&pointer, ':'); // skip ':'
		pointer++;
		if(*pointer == ',') return 2;

		if (strcmp(keyDef, "def") == 0){
			if (!get_int(&pointer, &defValue)){
				free(keyName);
				free(keyAtt);
				free(keyDef);
				return 3; // TODO: handle error -> val is not a num
			}
		} else{
			return 1; // TODO handle error -> key is not a name
		}

		expect_char(&pointer, ','); // skip ','
		if(*pointer != ',') return 1;
		pointer++;

		//Slots
		char *keySlots = get_string(&pointer);
		if (!keySlots) return 0; // TODO: handle error -> key is not a string

		expect_char(&pointer, ':'); // skip ':'
		pointer++;
		if(*pointer == ',') return 2;

		if (strcmp(keySlots, "slots") == 0){
			if (!get_int(&pointer, &slotsValue)){
				free(keyName);
				free(keyAtt);
				free(keyDef);
				free(keySlots);
				return 3; // TODO: handle error -> val is not a num
			}
		} else{
			return 1; // TODO handle error -> key is not a name
		}

		expect_char(&pointer, ','); // skip ','
		if(*pointer != ',') return 1;
		pointer++;

		//Range
		char *keyRange = get_string(&pointer);
		if (!keyRange) return 3; // TODO: handle error -> key is not a string

		expect_char(&pointer, ':'); // skip ':'
		pointer++;
		if(*pointer == ',') return 2;

		if (strcmp(keyRange, "range") == 0){
			if (!get_int(&pointer, &rangeValue)){
				free(keyName);
				free(keyAtt);
				free(keyDef);
				free(keySlots);
				free(keyRange);
				return 3; // TODO: handle error -> val is not a num
			}
		} else{
			return 1; // TODO handle error -> key is not a name
		}

		expect_char(&pointer, ','); // skip ','
		if(*pointer != ',') return 1;
		pointer++;

		//Radius
		char *keyRadius = get_string(&pointer);
		if (!keyRadius) return 3; // TODO: handle error -> key is not a string

		expect_char(&pointer, ':'); // skip ':'
		pointer++;
		skip_space(&pointer);
		if(*pointer == '}') return 2;

		if (strcmp(keyRadius, "radius") == 0){
			if (!get_int(&pointer, &radiusValue)){
				free(keyName);
				free(keyAtt);
				free(keyDef);
				free(keySlots);
				free(keyRange);
				free(keyRadius);
				return 3; // TODO: handle error -> val is not a num
			}
		} else{
			return 1; // TODO handle error -> key is not a name
		}

		expect_char(&pointer, '}');
		pointer++;
		
		
		// ----------------------------------------
		outObject->items = realloc(outObject->items, (outObject->count + 1) * sizeof(ITEM));

		strncpy(outObject->items[outObject->count].name, unitName, MAX_NAME);
		outObject->items[outObject->count].name[MAX_NAME] = '\0';
		outObject->items[outObject->count].att = attValue;
		outObject->items[outObject->count].def = defValue;
		outObject->items[outObject->count].slots = slotsValue;
		outObject->items[outObject->count].range = rangeValue;
		outObject->items[outObject->count].radius = radiusValue;

		outObject->count++;
		if (*pointer != ',') break;
	}

	return 0;
}

int read_txt(char *pointer, TXTObject *outObject){
	if (!get_int(&pointer, &outObject->count)) return 1; // No army count
	if (outObject->count < MIN_ARMY || outObject->count > MAX_ARMY) return 1;
	outObject->units = malloc(outObject->count * sizeof(UNIT));

	skip_space(&pointer);
	int count_iteration = 0;
	
	while (*pointer){
		char *unitName = NULL;
		char *firstItemName = NULL;
		char *secondItemName = NULL;
		bool second_item = false;

		char *unitNameTemp = pointer;
		while (*pointer && !isspace(*pointer)) pointer++;
		if (*pointer != ' ') return 2; // No item after name
		*pointer = '\0';
		unitName = strdup(unitNameTemp);
		if (unitName == NULL) return -1;
		pointer++;
		skip_space(&pointer);
		

		char *firstItemNameTemp = pointer;
		while (*pointer && *pointer != ' ' && *pointer != '\n') pointer++;
		if (*pointer == ' ') second_item = true;
		
		if(!second_item)
			while (97 > *(pointer-1) || *(pointer-1) > 122) pointer--;
		
		*pointer = '\0';
		firstItemName = strdup(firstItemNameTemp);
		if (firstItemName == NULL) return -1;
		pointer++;
		skip_space(&pointer);
		
		if (second_item){
			char *secondItemNameTemp = pointer;
			while (*pointer && *pointer != ' ' && *pointer != '\n') pointer++;
			if (*pointer == ' ') return 2; // after second item theare another item
			while (97 > *(pointer-1) || *(pointer-1) > 122) pointer--;
			*pointer = '\0';
			secondItemName = strdup(secondItemNameTemp);
			if (secondItemName == NULL) return -1;
			pointer++;
			skip_space(&pointer);
		}

		
		// Create unit
		UNIT unit = {.name = "", .item1 = NULL, .item2 = NULL, .hp = 0};
		strcpy(unit.name, unitName); // strncpy(unit.name, unitName, MAX_NAME);

		for (int n = 0; n < jsonFileObject.count; n++)
			if (strcmp(firstItemName, jsonFileObject.items[n].name) == 0)
				unit.item1 = &jsonFileObject.items[n];
		
		if (unit.item1 == NULL) return 3;
		
		if (second_item){
			second_item = false;
			for (int n = 0; n < jsonFileObject.count; n++)
				if (strcmp(secondItemName, jsonFileObject.items[n].name) == 0)
					unit.item2 = &jsonFileObject.items[n];
					
			if (unit.item2 == NULL) return 3;
			if ((unit.item1->slots + unit.item2->slots)>2) return 4;
		}
		unit.hp = 100;

		outObject->units[count_iteration] = unit;
		count_iteration++;
	}
	
	return 0;
}

bool error_text_file(int err_num){
	if (err_num == 1){ fprintf(stderr, "%s", ERR_UNIT_COUNT); 		return true; }
	if (err_num == 2){ fprintf(stderr, "%s", ERR_ITEM_COUNT); 		return true; }
	if (err_num == 3){ fprintf(stderr, "%s", ERR_WRONG_ITEM); 		return true; }
	if (err_num == 4){ fprintf(stderr, "%s", ERR_SLOTS); 			return true; }
	return false;
}

void print_army(TXTObject army1, TXTObject army2){
    printf("Army 1\n");
    for (int i = 0; i < army1.count; i++)
    {
        printf("    Unit: %d\n", i);
        printf("    Name: %s\n", army1.units[i].name);
        printf("    HP: %d\n", army1.units[i].hp);
        printf("    Item 1: %s,%d,%d,%d,%d,%d\n", army1.units[i].item1->name,
                                                  army1.units[i].item1->att,
                                                  army1.units[i].item1->def,
                                                  army1.units[i].item1->slots,
                                                  army1.units[i].item1->range,
                                                  army1.units[i].item1->radius);
        if (army1.units[i].item2 != NULL)
            printf("    Item 2: %s,%d,%d,%d,%d,%d\n", army1.units[i].item2->name,
                                                      army1.units[i].item2->att,
                                                      army1.units[i].item2->def,
                                                      army1.units[i].item2->slots,
                                                      army1.units[i].item2->range,
                                                      army1.units[i].item2->radius);

        printf("\n");
    }


    printf("Army 2\n");
    for (int i = 0; i < army2.count; i++)
    {
        printf("    Unit: %d\n", i);
        printf("    Name: %s\n", army2.units[i].name);
        printf("    HP: %d\n", army2.units[i].hp);
        printf("    Item 1: %s,%d,%d,%d,%d,%d\n", army2.units[i].item1->name,
                                                  army2.units[i].item1->att,
                                                  army2.units[i].item1->def,
                                                  army2.units[i].item1->slots,
                                                  army2.units[i].item1->range,
                                                  army2.units[i].item1->radius);
        if (army2.units[i].item2 != NULL)
            printf("    Item 2: %s,%d,%d,%d,%d,%d\n", army2.units[i].item2->name,
                                                      army2.units[i].item2->att,
                                                      army2.units[i].item2->def,
                                                      army2.units[i].item2->slots,
                                                      army2.units[i].item2->range,
                                                      army2.units[i].item2->radius);


        // if (i != army2.count-1) printf("\n");
		printf("\n");
    }
}
