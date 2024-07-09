#include <stdio.h>
#include <locale.h>
#include "han.h"

int main(int argc, char* argv[])
{
  setlocale(LC_ALL, "");
  while (true) {
    int c = getchar();
    if (c == '\r' || c == '\n') {
      continue;
    }
    if (c == '\\') {
      han_backspace();
      continue;
    }
    uint16_t ret = han_process(c);
    if (ret) {
      printf("%lc\n", ret);
    }
  }
}
