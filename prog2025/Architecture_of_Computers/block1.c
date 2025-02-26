#include <stdio.h>
#include <math.h>

#define line printf("+"); printf("------------------------------"); printf("+"); printf("\n")

#define Pi 3.14159265358979323846264338327950288419716939937510582097494459230781640628620899862803482534211706798214808651328230664709384460955058223172535940812848111745028410270193852110555964462294895493038196442881097566593344612847564823378678316527120190914564856692346034861045432664821339360726024914127372458700660631558817488152092096282925409171536436789259036001133053054882046652138414695194151160943305727036575959195309218611738193261179310511854807446237996274956735188575272489122793818301194912983367336244065664308602139494639522473719070217

int CountOnes = 0;

void ul1();
void ul2();
void ul3();
//void ul4();
void ul5();
//void ul6();

void printBinFromNum(int n);
float normRad(float r);
float sinus(float radians, float epsilon);


int main() {
  ul1();
  line;
  ul2();
  line;
  ul3();
  line;
  ul5();
  line;

  return 0;
}

void ul1() {
  int cislo = 33777;
  char znak = 'X';
    
  printf("ul1\n");
  printf("This task represents number %d and character %c to DEC, HEX, and BIN\n", cislo, znak);

  printf("CISLO: %d\n", cislo);
  printf("DEC: %d | HEX: %x | ", cislo, cislo);
  printf("BIN: ");
  printBinFromNum(cislo);

  printf("\n");
  printf("ZNAK: %c\n", znak);
  printf("DEC: %d | HEX: %x | ", znak, znak);
  printf("BIN: ");
  printBinFromNum(znak); // ASCII 88
}

void ul2() {
  int a, b;

  printf("ul2\n");
  printf("This task takes a number and makes a arithmetic shift of bits to the right\n");
  printf("Enter the positive number for the shift: ");
  scanf("%d", &a);
  if (a > 15 || a < 0) {
    printf("ERROR: number is not valid\n");
    return;
  }
  printf("Enter the number you want to shift by: ");
  scanf("%d", &b);

  // For 0 < numbers <= 15
  int big_count = sizeof(unsigned int);
  b %= big_count;
  unsigned int c = (a<<(big_count-b) | a>>b) & 15; // idk but its not working without & 15, some bit is turn to 1 but should be 0, so I just cut last 4 bits (1111)
  
  // For all numbers
  // int big_count = sizeof(unsigned int) * 8;
  // unsigned int c = a<<(big_count-b) | a>>b;

  printf("\n");
  printf("Your number in bin: ");
  printBinFromNum(a);

  printf("Your number shifted right by %d in bin: ", b);
  printBinFromNum(c);
  printf("In dec: %u\n", c);
}

void ul3() {
  int a;
  printf("ul3\n");
  printf("This task takes a number and counts how many 1's the binary representation of that number has\n");
  printf("Enter number: ");
  scanf("%d", &a);
  printf("\n");

  CountOnes = 0;
  printf("Your number in bin: ");
  printBinFromNum(a);
  printf("This number has %d ones\n", CountOnes);
}

void ul5(){
  float degrees, radians;
  float epsilon = 0.0000000000000000000001;
  
  printf("ul5\n");
  printf("This function emulates sin\n");
  printf("Enter degrees: ");
  scanf("%f", &degrees);
  radians = normRad(degrees * (Pi / 180.00));
  printf("sin(%.02f) ~ %f\n", degrees, sinus(radians, epsilon));
  printf("CHECK: %f\n", sin(radians));
}


// Support functions
void printBinFromNum(int n) {
  int bit;
  int i;
  if (n == 0){
    printf("0000\n");
    return;
  }

  if (n > 0){
    if (n < 16)         i = 3;  // 1111
    else if (n < 256)   i = 7;  // 1111 1111
    else if (n < 65536) i = 15; // 1111 1111 1111 1111
    else                i = 31; // 1111 1111 1111 1111 1111 1111 1111 1111
  } else {
    if (n > -32769)     i = 15; // 1111 1111 1111 1111
    else                i = 31; // 1111 1111 1111 1111 1111 1111 1111 1111
  }

  for (; i>=0; i--){
    bit = (n>>i) & 1;
    printf("%d", bit);

    if (i%4==0) printf(" ");
    if (bit == 1) CountOnes++;
  }
  printf("\n");
}

float normRad(float r){
  while (r<-Pi) r += 2*Pi;
  while (r>Pi) r -= 2*Pi;
  return r;
}

float sinus(float radians, float epsilon){
  float member = radians;
  float sum = radians;
  int n = 1;

  while (fabs(member) > epsilon){
    member *= -radians * radians / ((2*n)*(2*n+1)); // For sin
    // member += -radians * radians / ((2*n-1)*(2*n)); // For cos
    sum += member;
    n++;
  }
  
  return sum;
}
