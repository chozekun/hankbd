#include <stdio.h>
#include <locale.h>
#include "han.h"
#include "ks_x_1001.h"

int main(int argc, char* argv[])
{
  setlocale(LC_ALL, "");
  han_ctx_t* ctx = han_new();
  han_set_charset(ctx, KS_X_1001, countof(KS_X_1001));
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
