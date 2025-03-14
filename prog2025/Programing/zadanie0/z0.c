#include <stdio.h>

void primeFactors(int a, int m);

int main(){
  int a, b;

  if(scanf("%d %d", &a, &b) != 2 || a<=1){
    printf("ERROR\n");
    return 0;
  }

  primeFactors(a, b);
  return 0;
}

void primeFactors(int a, int m){
  for(int i = 2; i<=a && m>0; i++){
    if(a % i == 0){
      printf("%d\n", i);
      m--;
      do {
        a = a/i;
      } while (a % i == 0);
    }
  }
}
