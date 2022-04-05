#include "util.h"
#include <stdio.h>
#include <stdlib.h>

void util::exit_with_error(const char *message)
{
    printf("%s", message);
    getchar();
    exit(0);
}
