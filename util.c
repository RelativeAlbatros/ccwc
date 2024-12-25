#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#include "util.h"

void die(const char* fmt, ...) {
    va_list arg;

    va_start(arg, fmt);
    fprintf(stderr, "error: ");
    vfprintf(stderr, fmt, arg);
    fputc('\n', stderr);
    va_end(arg);

    exit(1);
}

bool fileinpwd(const char* file) {
    DIR *d = opendir(".");
    struct dirent *dir;

    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (dir->d_type == DT_REG && !strncmp(file, dir->d_name, strlen(file)))
                return true;
        }
    } else {
        die("opening folder");
    }
    return 0;
}

int cbytes(const char* file) {
    FILE* fp = fopen(file, "r");
    if (fp == NULL) {
        die("file is not open.");
    }

    int bytes = 0;
    int c;
    do {
        c = getc(fp);
        bytes++;
    } while (c != EOF);

    fclose(fp);
    return bytes-1;
}

int clines(const char* file) {
    FILE* fp = fopen(file, "r");
    if (fp == NULL) {
        die("file is not open.");
    }

    int lines = 0;
    int c;
    do {
        c = getc(fp);
        if (c == '\n')
            lines++;
    } while (c != EOF);

    fclose(fp);
    return lines;
}

int cwords(const char* file) {
    FILE* fp = fopen(file, "r");
    if (fp == NULL) {
        die("file is not open.");
    }

    int words = 0;
    int inword = 0;
    int c;

    while ((c = getc(fp)) != EOF) {
        if ((c == ' ' || c == '\n') && inword)
            inword = 0;
        else if (c != ' ' && !inword) {
            inword = 1;
            words++;
        }
    }

    fclose(fp);
    return words;
}

int mbchars(const char* file) {
    FILE* fp = fopen(file, "r");
    if (fp == NULL) {
        die("file is not open.");
    }

    setlocale(LC_ALL, ""); // multi-byte character handling
    // buffer to hold multi-byte characters
    // MB_CUR_MAX: maximum number of bytes in a multi-byte character
    char  buffer[MB_CUR_MAX]; 
    int mbchars = 0;
    int bytes_read;

    while ((bytes_read = fread(buffer, 1, 1, fp)) > 0) {
        wchar_t wc;
        int length = mbtowc(&wc, buffer, MB_CUR_MAX);
        if (length > 0) {
            // valid multi-byte character
            mbchars++;
            // move file pointer forward by rest of the multib-character
            fseek(fp, length-1, SEEK_CUR);
        } else if (length == -1) {
            mbtowc(NULL, NULL, 0); // reset conversion state
        }
    }

    fclose(fp);
    return mbchars;
}
