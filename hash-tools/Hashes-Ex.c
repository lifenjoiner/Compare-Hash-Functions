#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "GeneralHashFunctions.h"

unsigned int BKDRHash_x(const char* str, unsigned int len, unsigned int seed)
{
   // unsigned int seed = 131; /* 31 131 1313 13131 131313 etc.. */
   unsigned int hash = 0;
   unsigned int i    = 0;

   for(i = 0; i < len; str++, i++)
   {
      hash = (hash * seed) + (*str);
   }

   return hash;
}

unsigned int ELFHash2(const char* str, unsigned int len)
{
   unsigned int hash = 0;
   unsigned int x    = 0;
   unsigned int i    = 0;

   for(i = 0; i < len; str++, i++)
   {
      hash += *str;
      hash <<= 4;
      if((x = hash & 0xF0000000L) != 0)
      {
         hash ^= (x >> 24);
      }
      hash &= ~x;
   }

   return hash;
}


int main(int argc, char* argv[])
{
    FILE *fp;
    char line[256] = {0};
    int len;

    if (argc != 2) {
        printf("usage: %s <list-file>", argv[0]);
        return -1;
    }
    
    if ((fp = fopen(argv[1], "r")) == NULL) return -2;
    
    printf("string\t"
           "BKDR1313\t"
           "BKDR13131\t"
           "BKDR131313\t"
           "ELFHash2\n"
          );
    while (fscanf(fp, "%256[^\r\n]%*[\r\n]", line) > 0) {
        len = strlen(line);
        printf("%s\t"
               "%08X\t"
               "%08X\t"
               "%08X\t"
               "%08X\n",
               line,
               BKDRHash_x(line, len, 1313),
               BKDRHash_x(line, len, 13131),
               BKDRHash_x(line, len, 131313),
               ELFHash2(line, len)
              );
    }
    
    fclose(fp);

    return 0;
}
