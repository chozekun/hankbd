#include <stdio.h>
#include <locale.h>
#include <string.h>
#include "limited_han.h"

int main(int argc, char* argv[])
{
  setlocale(LC_ALL, "");
  han_ctx_t* ctx = limited_han_new();
  const char TEST[] = "rkd";
  for (int i = 0; i < strlen(TEST); ++i) {
    fprintf(stderr, ".");
    uint16_t ret = han_process(ctx, TEST[i]);
    if (ret) {
      printf("%lc\n", ret);
    }
  }
  han_free(ctx);
  return 0;
}
