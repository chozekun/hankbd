#include <stdio.h>
#include <stdbool.h>
#include <locale.h>
#include "hankbd.h"


#define CRESET "\e[0m"
#define RED "\e[0;31m"
#define GRN "\e[0;32m"


#define TEST(a, b) { \
  fwprintf(stderr, L"[%s:%d] TEST(%S, %S)\n", __FILE__, __LINE__, a, b); \
}

#define ASSERT(expr, ...) { \
  fwprintf(stderr, L"[%s:%d] ", __FILE__, __LINE__); \
  if (!(expr)) { \
    fwprintf(stderr, L"\t%s[FAIL]%s ", RED, CRESET); \
    ret = 1; \
  } else { \
    fwprintf(stderr, L"\t%s[SUCCESS]%s ", GRN, CRESET); \
  } \
  fwprintf(stderr, __VA_ARGS__); \
  fwprintf(stderr, L"\n"); \
}

static int ret = 0;


bool check_preedit(wchar_t* input, wchar_t* preedit)
{
  TEST(input, preedit);
  ctx_t ctx;
  ctx_init(&ctx);
  int k = 0;
  for (int i = 0; input[i]; ++i) {
    if (input[i] == L'\b') {
      ctx_backspace(&ctx);
      continue;
    }
    if (ctx_process(&ctx, input[i])) {
      ctx_clear_commit(&ctx);
    }
  }
  wchar_t combined = ctx_combine_preedit(&ctx);
  ASSERT(combined == preedit[0], L"%lc == %lc", combined, preedit[0]);
}


bool check_commit(wchar_t* input, wchar_t* output)
{
  TEST(input, output);
  ctx_t ctx;
  ctx_init(&ctx);
  int k = 0;
  for (int i = 0; input[i]; ++i) {
    if (ctx_process(&ctx, input[i])) {
      for (int j = 0; j < countof(ctx.commit) && ctx.commit[j]; ++j) {
        ASSERT(ctx.commit[j] == output[k], L"%lc == %lc", ctx.commit[j], output[k]);
        ++k;
      }
      ctx_clear_commit(&ctx);
    }
  }
  ASSERT(output[k] == 0, L"%S[%d] == %d", output, k, output[k]);
}

int main(int argc, char* argv[])
{
  setlocale(LC_ALL, "");
  check_commit(L"ㄱㅏㅉ", L"가");
  check_preedit(L"ㄱㅏㅉ", L"ㅉ");
  check_commit(L"ㅂㅓㅆㅡ", L"버");
  check_preedit(L"ㅂㅓㅆㅡ", L"쓰");
  check_preedit(L"ㅁㅏㄹㄱ", L"맑");
  check_commit(L"ㅁㅏㄹㄱㅗ", L"말");
  check_preedit(L"ㅁㅏㄹㄱㅗ", L"고");

  check_preedit(L"ㄱㅅ", L"ㄳ");
  check_commit(L"ㄱㅅㅏ", L"ㄱ");
  check_preedit(L"ㄱㅅㅏ", L"사");

  /* backspace */
  check_preedit(L"ㄱㅏㅆ\b", L"가");
  check_preedit(L"ㄱㅅ\bㅏ", L"가");
  check_preedit(L"ㅁㅏㄹㄱ\b", L"말");
  check_preedit(L"ㅇㅜㅔ\b", L"우");
  check_preedit(L"ㅂㅂㅜㅔㄹㄱ\b\b\b\b\b\b", L"");
  check_preedit(L"ㅃㅜㅔㄹㄱ\b\b\b\b\b", L"");
  return ret;
}
