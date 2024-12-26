#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#include <wchar.h>
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

int cbytes(const char* input, Mode mode) {
    int bytes = 0;

    if (mode == FILE_READ) {
        FILE* fp = fopen(input, "r");
        int c;
        if (fp == NULL) {
            die("can't open file %s", input);
        }

        while ((c = getc(fp)) != EOF) {
            bytes++;
        }

        fclose(fp);
        return bytes;
    } else if (mode == STR_READ) {
        return strlen(input);
    }
}

int clines(const char* input, Mode mode) {
    int lines = 0;

    if (mode == FILE_READ) {
        FILE* fp = fopen(input, "r");
        int c;
        if (fp == NULL) {
            die("can't open file %s", input);
        }

        while ((c = getc(fp)) != EOF) {
            if (c == '\n')
                lines++;
        }

        fclose(fp);
        return lines;
    } else if (mode == STR_READ) {
        for (int i=0; i<strlen(input); i++) {
            if (input[i] == '\n')
                lines++;
        }
        return lines;
    }
}

int cwords(const char* input, Mode mode) {
    int words = 0;
    bool inword = true;

    if (mode == FILE_READ) {
        int c;
        FILE* fp = fopen(input, "r");
        if (fp == NULL) {
            die("can't open file %s", input);
        }

        while ((c = getc(fp)) != EOF) {
            if ((c == ' ' || c == '\n' || c == '\t') && inword)
                inword = false;
            else if ((c != ' ' || c != '\n' || c!= '\t') && !inword) {
                inword = true;
                words++;
            }
        }

        fclose(fp);
        return words;
    } else if (mode == STR_READ) {
        for (int i=0; i<strlen(input); i++) {
            if ((input[i] == ' ' || input[i] == '\n' || input[i] == '\t') && inword) {
                inword = false;
            } else if ((input[i] != ' ' || input[i] != '\n' || input[i]!= '\t') && !inword) {
                inword = true;
                words++;
            }
        }

        return words;
    }
}

int cmbchars(const char* input, Mode mode) {
    setlocale(LC_ALL, ""); // multi-byte character handling
    // buffer to hold multi-byte characters
    // MB_CUR_MAX: maximum number of bytes in a multi-byte character
    char  buffer[MB_CUR_MAX]; 
    int mbchars = 0;
    int bytes_read;

    if (mode == FILE_READ) {
        FILE* fp = fopen(input, "r");
        if (fp == NULL) {
            die("can't open file %s", input);
        }

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
    } else if (mode == STR_READ) {
        size_t charlen;
        mbstate_t mbs;

        mbsinit(&mbs);
        while ((charlen = mbrlen(input, MB_CUR_MAX, &mbs)) != 0
                && charlen != (size_t)-1
                && charlen != (size_t)-2) {
            input += charlen;
            mbchars++;
        }   

        return mbchars;
    }
}
