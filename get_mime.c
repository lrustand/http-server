#include <string.h>
#include <stdio.h>

#define MIMEFILE "./etc/mime.types"

char* get_mime(char* path)
{
  // get file name
  const char* file = strrchr(path, '/') + 1;

  // get file extension
  const char* dot = strrchr(file, '.');
  if(!dot || dot == file) dot = " ";
  dot++;

  // open mime file
  FILE* mimes = fopen(MIMEFILE, "r");
  char* txt = NULL;
  size_t len = 0;

  // read mime file
  while(getline(&txt, &len, mimes) != -1)
  {
    if(strstr(strrchr(txt, '\t') + 1, dot))
    {
      fclose(mimes);
      return strtok(txt, "\t");
    }
  }
  fclose(mimes);
  return NULL;
}
