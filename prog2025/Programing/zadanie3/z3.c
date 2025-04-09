#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MMAX 1024
#define FLOW 100


const char* readline();
int parser(const char* input);

bool error_overflow(int len);
bool error_position(const char* sp, const char* ep);
bool error_out_of_range(const char* p, const char* op, int n);


int main()
{
  char op[FLOW+1];
  char mem[FLOW+1];
  op[0] = '\0';
  mem[0] = '\0';
  char* sp = op;
  char* ep = sp;


  while (true)
  {
    // printf("sp '%c'\n", *sp);
    // printf("ep '%c'\n", *ep);
    const char* text = readline();

    if (parser(text) == 0) break;
    else if (parser(text) == 1)                                 // in [text]
    {
      while (*text != ' ') text++;
      text++;
      if (error_overflow(strlen(text))) break;
      strcpy(op, text);
      sp = op;
      op[strlen(text)] = '\0';
      ep = sp;

      while (*ep != '\0') ep++;
    }
    else if (parser(text) == 2)                                 // out op
    {
      printf("%s\n", op);
    }
    else if (parser(text) == 3)                                 // out range
    {
      if (error_position(sp, ep)) break;

      char* tempp = sp;
      while (tempp != ep)
      {
        printf("%c", *tempp); // temp[0]
        tempp++;
      }
      printf("\n");
    }
    else if (parser(text) == 4)                                 // out mem
    {
      printf("%s\n", mem);
    }
    else if (parser(text) == 5)                                 // move {sp/ep} n
    {
      while (*text != ' ') text++;
      text++;
      if (strncmp(text, "sp", 2) == 0)
      {
        text+=3;
        int n = atoi(text);
        if (error_out_of_range(sp, op, n)) break;
        sp+= n;
      }
      else if (strncmp(text, "ep", 2) == 0)
      {
        text+=3;
        int n = atoi(text);
        if (error_out_of_range(ep, op, n)) break;
        ep+= n;
      }
    }
    else if (parser(text) == 6)                                 // start {sp/ep}
    {
      while (*text != ' ') text++;
      text++;
      if (strncmp(text, "sp", 2) == 0) sp = op;
      if (strncmp(text, "ep", 2) == 0) ep = op;
    }
    else if (parser(text) == 7)                                 // end {sp/ep}
    {
      while (*text != ' ') text++;
      text++;
      if (strncmp(text, "sp", 2) == 0) while (*sp != '\0') sp++;
      if (strncmp(text, "ep", 2) == 0) while (*ep != '\0') ep++;
    }
    else if (parser(text) == 8)                                 // first
    {
      sp = op;
      ep = op;

      while (*sp != '\0' && !isalnum(*sp)) {sp++; ep++;}
      while (*ep != '\0' && isalnum(*ep)) ep++;

      if (*sp == '\0')
      {
        sp = op;
        ep = op;
      }
    }
    else if (parser(text) == 9)                                 // last
    {
      while (*sp != '\0') sp++;
      while (*ep != '\0') ep++;

      while (ep != op && !isalnum(*ep)) {ep--; sp--;}
      while (sp != op && isalnum(*sp)) sp--;
      if (sp != op) sp++;
      if (*ep == *op)
      {
        while (*ep != '\0') ep++;
        ep--;
        while (*sp != '\0') sp++;
      }
      ep++;
    }
    else if (parser(text) == 10)                                // prev
    {
      if (sp == op) ep = op;
      else
      {
        while (sp != op && isalnum(*sp)) sp--;
        while (sp != op && !isalnum(*sp)) sp--;
        while (sp != op && isalnum(*sp)) sp--;
        ep = sp;
        ep++;
        while (*ep != '\0' && isalnum(*ep)) ep++;
        if (sp == op && !isalnum(*sp))
        {
          sp = op;
          sp--;
          ep = op;
        }
        if (sp != op) sp++;
      }
    }
    else if (parser(text) == 11)                                // next
    {
      while (*sp != '\0' && isalnum(*sp)) sp++;
      while (*sp != '\0' && !isalnum(*sp)) sp++;
      ep = sp;
      while (*ep != '\0' && isalnum(*ep)) ep++;
    }
    else if (parser(text) == 12)                                // del
    {
      if (error_position(sp, ep)) break;

      memmove(sp, ep, strlen(ep)+1);
      ep = sp;
    }
    else if (parser(text) == 13)                                // crop
    {
      if (error_position(sp, ep)) break;

      int i = 0;
      char* tempp = sp;
      while (tempp != ep)
      {
        op[i++] = *tempp;
        tempp++;
      }
      op[i] = '\0';

      sp = op;
      ep = sp;
      while (*ep != '\0') ep++;
    }
    else if (parser(text) == 14)                                // copy
    {
      if (error_position(sp, ep)) break;

      int i = 0;
      char* tempp = sp;
      while (tempp != ep)
      {
        mem[i++] = *tempp;
        tempp++;
      }
      if (sp!=ep) mem[i] = '\0';
    }
    else if (parser(text) == 15)                                // insert {n/mem}
    {
      if (error_position(sp, ep)) break;
      while (*text != ' ') text++;
      text++;


      if (strncmp(text, "mem", 3) == 0)
      {
        int mem_len = strlen(mem);
        if (error_overflow(mem_len + strlen(op))) break;
        memmove(sp+mem_len, sp, strlen(sp)+1);
        memcpy(sp, mem, mem_len);
        ep += mem_len;
      }
      else
      {
        int n = atoi(text);
        if (error_overflow(n + strlen(op))) break;
        memmove(sp+n, sp, strlen(sp)+1);
        for (int i = 0; i < n; i++)
        {
          *sp = ' ';
          sp++;
        }
        sp -= n;
        ep += n;
      }
    }
    else if (parser(text) == 16)                                // replace
    {
      if (error_position(sp, ep)) break;
      int n = ep-sp;
      for (int i = 0; i < strlen(mem); i++)
      {
        if (n > 0) sp[i] = mem[i];
        n--;
      }
    }
  }

  return 0;
}

const char *readline() {
  static char buffer[MMAX + 1] = {0};
  fgets(buffer, sizeof(buffer), stdin);
  buffer[strcspn(buffer, "\n")] = '\0';
  return buffer;
}

int parser(const char* input)
{
  if (strcmp(input, "quit") == 0)
    return 0;

  if (strncmp(input, "in ", 3) == 0)
    return 1;

  if (strcmp(input, "out op") == 0)
    return 2;

  if (strcmp(input, "out range") == 0)
    return 3;

  if (strcmp(input, "out mem") == 0)
    return 4;

  if (strncmp(input, "move ", 5) == 0)
    return 5;

  if (strncmp(input, "start ", 6) == 0)
    return 6;

  if (strncmp(input, "end ", 4) == 0)
    return 7;

  if (strcmp(input, "first") == 0)
    return 8;

  if (strcmp(input, "last") == 0)
    return 9;

  if (strcmp(input, "prev") == 0)
    return 10;

  if (strcmp(input, "next") == 0)
    return 11;

  if (strcmp(input, "del") == 0)
    return 12;

  if (strcmp(input, "crop") == 0)
    return 13;

  if (strcmp(input, "copy") == 0)
    return 14;

  if (strncmp(input, "insert ", 7) == 0)
    return 15;

  if (strcmp(input, "replace") == 0)
    return 16;

  return -1;
}


bool error_overflow(int len)
{
  if (len >= FLOW + 1) {
    printf("ERR_OVERFLOW");
    return true;
  }
  return false;
}

bool error_position(const char* sp, const char* ep)
{
  if (sp > ep)
  {
    printf("ERR_POSITION");
    return true;
  }
  return false;
}

bool error_out_of_range(const char* p, const char* op, int n)
{
  const char* last_c = op;
  while (*last_c != '\0') last_c++;

  if ((p+n) < op || (p+n) > last_c)
  {
    printf("ERR_OUT_OF_RANGE");
    return true;
  }
  return false;
}
