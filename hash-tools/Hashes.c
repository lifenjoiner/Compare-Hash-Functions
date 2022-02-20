#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "GeneralHashFunctions.h"


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
           "APHash\t"
           "BKDRHash\t"
           "BPHash\t"
           "DEKHash\t"
           "DJBHash\t"
           "ELFHash\t"
           "FNVHash\t"
           "JSHash\t"
           "PJWHash\t"
           "RSHash\t"
           "SDBMHash\n"
          );
    while (fscanf(fp, "%256[^\r\n]%*[\r\n]", line) > 0) {
        len = strlen(line);
        printf("%s\t"
               "%08X\t"
               "%08X\t"
               "%08X\t"
               "%08X\t"
               "%08X\t"
               "%08X\t"
               "%08X\t"
               "%08X\t"
               "%08X\t"
               "%08X\t"
               "%08X\n",
               line,
               APHash(line, len),
               BKDRHash(line, len),
               BPHash(line, len),
               DEKHash(line, len),
               DJBHash(line, len),
               ELFHash(line, len),
               FNVHash(line, len),
               JSHash(line, len),
               PJWHash(line, len),
               RSHash(line, len),
               SDBMHash(line, len)
              );
    }
    
    fclose(fp);

    return 0;
}
