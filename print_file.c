#include <stdio.h>
void print_file(char* path)
{
  FILE* file = fopen(path, "r");
  size_t len = 0;
  char *buffer;
  long filelen;


  // Finn lengden av filen
  fseek(file, 0, SEEK_END);
  filelen = ftell(file);
  rewind(file);

  // Alloker minne og les fila
  buffer = (char *)malloc((filelen)*sizeof(char));
  fread(buffer, filelen, 1, file);
  write(1,buffer,filelen);

  // Lukk fila og frigjør minnet
  fclose(file);
  free(buffer);
}
