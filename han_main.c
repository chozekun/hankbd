#include <stdio.h>
#include <locale.h>
#include "han.h"

int main(int argc, char* argv[])
{
  setlocale(LC_ALL, "");
  han_ctx_t* ctx = han_new();
  while (true) {
    int c = getchar();
    if (c == '\r' || c == '\n') {
      continue;
    }
    if (c == '\\') {
      han_backspace(ctx);
      continue;
    }
    uint16_t ret = han_process(ctx, c);
    if (ret) {
      printf("%lc\n", ret);
    }
  }
  han_free(ctx);
  return 0;
}
