#include <stdio.h>
void print_file(char* path)
{
  FILE* file = fopen(path, "r");
  size_t len = 0;
  char* txt;

  while(getline(&txt, &len, file) != -1)
  {
    printf("%s", txt);
  }
  fclose(file);
}
