#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>


int foooo(int x)
{
  int dodo = 1;
  sleep(dodo);
  return x + 1;
};


int main(int argc, char** argv)
{
  int number = 10000;


  if(argc > 1)
    number = atoi(argv[1]);

  printf("pid = %d\n", getpid());


  int i = 0;
  while(i < number)
  {
    i = foooo(i);
    printf("i = %d\n", i);
    number -= 1;
    printf("number = %d\n", number);
    sleep(1);
  }

  return 0;
}