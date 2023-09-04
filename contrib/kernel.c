#include <stdio.h>
#include <string.h>

#include "../util.h"
#include "kernel.h"

// KERNEL:  //
#define ICONk                           "" COL0

#define KERNELFILE                         "/proc/version"

size_t
kernelu(char *str, int sigval)
{
    FILE *fp;
    char *line;
    /* Here the lenght of temp_string should be larger than the longest name of kernel you have */
    char temp_string[20];
    size_t len = 0;
    ssize_t read;
    if (!(fp = fopen(KERNELFILE, "r"))) {
                *str = '\0';
                return 1;
    }
    /* Scan /proc/version file where kernel info is */
    read = getline(&line, &len, fp);
    if ( read!=-1 ) {
        //printf("Read line of size %zu \n", read);
        //printf("The line contains: %s \n", line);
        strtok(line, "(");
        char * rest_tokens = strtok(line, " ");
        while( rest_tokens != NULL ) {
            strcpy(temp_string, rest_tokens);
            rest_tokens = strtok(NULL, " ");
        }
    }
    fclose(fp);


    return SPRINTF(str, ICONk " %s", temp_string);
}

void
kernelc(int button)
{
    switch (button) {
            case 1:
                    break;
            case 2:
                    break;
            case 3:
                    break;
    }
}
