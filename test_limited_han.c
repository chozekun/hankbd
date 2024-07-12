#include <stdio.h>
#include <locale.h>
#include <string.h>
#include "han.h"
#include "ks_x_1001.h"


#define CRESET "\e[0m"
#define RED "\e[0;31m"
#define GRN "\e[0;32m"


#define TEST(a, b, c) { \
  fprintf(stderr, "[%s:%d] %s(%s, %lc, %lc)\n", __FILE__, __LINE__, __FUNCTION__, a, b, c); \
}

#define ASSERT(expr, ...) { \
  fprintf(stderr, "[%s:%d] ", __FILE__, __LINE__); \
  if (!(expr)) { \
    fprintf(stderr, "\t%s[FAIL]%s ", RED, CRESET); \
    ret = 1; \
  } else { \
    fprintf(stderr, "\t%s[SUCCESS]%s ", GRN, CRESET); \
  } \
  fprintf(stderr, __VA_ARGS__); \
  fprintf(stderr, "\n"); \
}

static int ret = 0;


bool check(char* input, uint16_t commit, uint16_t preedit)
{
  TEST(input, commit, preedit);
  han_ctx_t* ctx = han_new();
  han_set_charset(ctx, KS_X_1001, countof(KS_X_1001));
  for (int i = 0; i < strlen(input); ++i) {
    if (input[i] == '\b') {
      han_backspace(ctx);
      continue;
    }
    uint16_t ret = han_process(ctx, input[i]);
    if (ret) {
      ASSERT(ret == commit, "%lc == %lc", ret, commit);
      break;
    }
  }
  uint16_t combined = han_preedit(ctx);
  ASSERT(combined == preedit, "%lc == %lc", combined, preedit)
  han_free(ctx);
}

int main(int argc, char* argv[])
{
  setlocale(LC_ALL, "");
  check("rs", L'ㄱ', L'ㄴ');
  check("rh", 0, L'고');
  check("rhk", 0, L'과');
  check("sO", L'ㄴ', L'ㅒ');
  check("h", 0, L'ㅗ');
  check("hj", L'ㅗ', L'ㅓ');
  check("hg", L'ㅗ', L'ㅎ');
  check("rkr", 0, L'각');
  check("rkfr", 0, L'갉');
  check("rkz", L'가', L'ㅋ');
  check("rkrt", L'각', L'ㅅ');
  check("rkrk", L'가', L'가');
  check("rkfrk", L'갈', L'가');
  check("rksO", L'간', L'ㅒ');
  check("rkfaO", L'갊', L'ㅒ');
  check("rkrs", L'각', L'ㄴ');
  return ret;
}
