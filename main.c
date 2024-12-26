#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "util.h"

#define VERSION "0.2"
#define HELP_MSG "ccwc [OPTION] [FILE] ...:\n" \
"\t-c:\treturns number of bytes\n" \
"\t-l:\treturns number of lines\n" \
"\t-w:\treturns number of words\n" \
"\t-m:\treturns number of characters\n" \
"\t-v --version:\toutput version information\n" \
"\t-h --help:\tdisplay this help\n"

static short unsigned int flags = 0;

int main(int argc, char **argv) {
    if (argc >= 2 && (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help"))) {
        printf("%s", HELP_MSG);
        return 0;
    } else if (argc >= 2 && ((!strcmp(argv[1], "-v")) || !strcmp(argv[1], "--version"))) {
        fprintf(stdout, "ccwc %s\n", VERSION);
        return 0;
    } else if (argc >= 2 && argv[1][0] == '-') {
        switch (argv[1][1]) {
        case 'c':
            flags = flags | 1;
            break;
        case 'l':
            flags = flags | (1<<1);
            break;
        case 'w':
            flags = flags | (1<<2);
            break;
        case 'm':
            flags = flags | (1<<3);
            break;
        default:
            printf("ccwc: invalid option -- \'%s\'\n"
                    "try \'ccwc --help\' for more information\n", argv[1][1]);
            return -1;
        }
    }

    // read from stdin
    if ((flags != 0 && argc == 2) ||
            (argc == 1)) {
        char* input = malloc(4096 * sizeof(char));
        int bytes, lines, words, mbchars = 0;

        while (read(STDIN_FILENO, input, 4096) > 0) {
            if (flags & 1) { // -c flag
                bytes += cbytes(input, 1);
            } else if (flags & (1<<1)) { // -l flag
                lines += clines(input, 1);
            } else if (flags & (1<<2)) { // -w flag
                words += cwords(input, 1);
            } else if (flags & (1<<3)) { // -m flag
                mbchars += cmbchars(input, 1);
            }
        }
        if (flags & 1) { // -c flag
            fprintf(stdout, "%d\n", bytes);
        } else if (flags & (1<<1)) { // -l flag
            fprintf(stdout, "%d\n", lines);
        } else if (flags & (1<<2)) { // -w flag
            fprintf(stdout, "%d\n", words);
        } else if (flags & (1<<3)) { // -m flag
            fprintf(stdout, "%d\n", mbchars);
        } else {
            fprintf(stdout, "%d\t%d\t%d\n", lines, words, bytes);
        }

        free(input);
        return 0;
    }

    char output[2048];
    for (int i = flags ? 2 : 1; i<argc; i++) {
        if (!fileinpwd(argv[i]))
            die("file was not found");
        if (flags & 1) { // -c flag
            sprintf(output, "%d\t", cbytes(argv[i], 0));
        } else if (flags & (1<<1)) { // -l flag
            sprintf(output, "%d\t", clines(argv[i], 0));
        } else if (flags & (1<<2)) { // -w flag
            sprintf(output, "%d\t", cwords(argv[i], 0));
        } else if (flags & (1<<3)) { // -m flag
            sprintf(output, "%d\t", cmbchars(argv[i], 0));
        } else {
            sprintf(output, "%d\t%d\t%d\t", clines(argv[i], 0), cwords(argv[i], 0), cbytes(argv[i], 0));
        }
        sprintf(output, "%s%s\n", output, argv[i]);
        fprintf(stdout, output);
    }
	return 0;
}
