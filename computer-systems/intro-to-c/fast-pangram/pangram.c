#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

bool ispangram(char *s)
{
  int sum = 0;
  char seen[26] = {0};

  for (int i = 0; s[i]; i++)
  {
    int c = tolower(s[i]); // returns ascii int of char
    int ascii = c - 'a';

    if (!seen[ascii] && isalpha(s[i]))
    {
      if (++sum == 26)
        return true;
      seen[ascii] = 1;
    }
  }

  return false;
}

int main()
{
  size_t len;
  ssize_t read;
  char *line = NULL;
  while ((read = getline(&line, &len, stdin)) != -1)
  {
    if (ispangram(line))
      printf("%s", line);
  }

  if (ferror(stdin))
    fprintf(stderr, "Error reading from stdin");

  free(line);
  fprintf(stderr, "ok\n");
}
