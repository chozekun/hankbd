#include <locale.h>
#include "hankbd.h"

int main(int argc, char* argv[])
{
  ctx_t ctx;
  setlocale(LC_ALL, "");
  ctx_init(&ctx);
  while (true) {
    wint_t wc = getwchar();
    if (wc == L'\r' || wc == L'\n') {
      continue;
    }
    if (wc == L'\\') {
      ctx_backspace(&ctx);
      continue;
    }
    if (ctx_process(&ctx, wc)) {
      for (int i = 0; i < countof(ctx.commit) && ctx.commit[i]; ++i) {
        wprintf(L"Commit: %lc\n", ctx.commit[i]);
      }
      ctx_clear_commit(&ctx);
    }
  }
  return 0;
}
