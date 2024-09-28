#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

void dies(char* format, ...) {
  
  va_list val;

  va_start(val,format);
  vprintf(format,val);
  va_end(val);

  exit(1);
}
