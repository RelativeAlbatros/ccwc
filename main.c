#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "util.h"

#define VERSION "0.1"
#define HELP_MSG "ccwc:\n"

static short unsigned int flags = 0;

int main(int argc, char **argv) {
    if (argc >= 2 && (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help"))) {
        printf("%s", HELP_MSG);
        return 0;
    } else if (argc >= 2 && (!strcmp(argv[1], "-v")) || !strcmp(argv[1], "--version")) {
        fprintf(stdout, "ccwc %s\n", VERSION);
        return 0;
    }
	if (argc >= 2 && argv[1][0] == '-') {
        switch (argv[1][1]) {
        case 'c':
            flags = flags | 1;
        case 'l':
            flags = flags | (1<<1);
        case 'w':
            flags = flags | (1<<2);
        case 'm':
            flags = flags | (1<<3);
        default:
            printf("ccwc: invalid option -- \'%s\'\n \
                    try \'ccwc --help\' for more information.\n", argv[1][1]);
            return -1;
        }
    }

    if ((flags != 0 && argc == 2) ||
            (argc == 1)) {
        char* input = malloc(4096 * sizeof(char));
        fgets(input, sizeof(input), stdin);
        if (flags & 1) { // -c flag
            // ERR: change util functions to accept strings
            fprintf(stdout, "%d\t", cbytes(input));
        } else if (flags & (1<<1)) { // -l flag
            fprintf(stdout, "%d\t", clines(input));
        } else if (flags & (1<<2)) { // -w flag
            fprintf(stdout, "%d\t", cwords(input));
        } else if (flags & (1<<3)) { // -m flag
            fprintf(stdout, "%d\t", mbchars(input));
        } else {
            fprintf(stdout, "%d\t%d\t%d\t", clines(input), cwords(input), cbytes(input));
        }

        free(input);
        return 0;
    }

    char output[2048];
    for (int i = flags ? 2 : 1; i<argc; i++) {
        if (!fileinpwd(argv[i]))
            die("file was not found");
        if (flags & 1) { // -c flag
            sprintf(output, "%d\t", cbytes(argv[i]));
        } else if (flags & (1<<1)) { // -l flag
            sprintf(output, "%d\t", clines(argv[i]));
        } else if (flags & (1<<2)) { // -w flag
            sprintf(output, "%d\t", cwords(argv[i]));
        } else if (flags & (1<<3)) { // -m flag
            sprintf(output, "%d\t", mbchars(argv[i]));
        } else {
            sprintf(output, "%d\t%d\t%d\t", clines(argv[i]), cwords(argv[i]), cbytes(argv[i]));
        }
        sprintf(output, "%s%s\n", output, argv[i]);
        fprintf(stdout, output);
    }
	return 0;
}
