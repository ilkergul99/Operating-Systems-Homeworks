#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//For compilation
//gcc -o filename.out filename.c
//./filename.out

int main()
{
  char file_name[25] = "loremipsum.txt";
  char ch;
  FILE *countFile;

  countFile = fopen(file_name, "r");  //read-mode
  if(countFile != NULL)
  {
    int characterCount = 0;
    while((ch = getc(countFile)) != EOF)
    {
      if(ch == 'a')
      {
        characterCount = characterCount + 1;
      }
    }
    printf("The occurrence of character ’a’ in the file is %d\n", characterCount);
  }
  else
  {
    printf("Error while opening the file.\n");

  }
  fclose(countFile);
  return 0;
}
