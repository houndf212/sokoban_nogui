#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
void util::exit_with_error(const char *message)
{
    std::cout << message <<std::endl;
    getchar();
    exit(0);
}
