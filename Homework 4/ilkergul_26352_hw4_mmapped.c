#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
  int countFile = open("loremipsum.txt", O_RDONLY);
  struct stat s;
  size_t size;
  int status = fstat(countFile, &s);
  size = s.st_size;
  char *fileptr = mmap(0, size, PROT_READ, MAP_PRIVATE, countFile, 0);

  if(fileptr != MAP_FAILED)
  {
    int character_a_Count = 0;
    size_t i = 0;
    for(; i < size; i++)
    {
      if(fileptr[i] == 'a')
      {
        character_a_Count++;
      }
    }
    printf("The occurrence of character ’a’ in the file is %d\n", character_a_Count);

    status = munmap(fileptr, size);
    if(status != 0)
    {
      printf("Unmapping failed\n");
      return 1;
    }
  }
  else
  {
    printf("Mapping for counting occurrences has failed\n");
    return 1;
  }
  close(countFile);

  return 0;
}
