#include <stdio.h>
#include <locale.h>
#include <string.h>
#include "han.h"


#define CRESET "\e[0m"
#define RED "\e[0;31m"
#define GRN "\e[0;32m"


#define TEST(a, b) { \
  fprintf(stderr, "[%s:%d] %s(%s, %lc)\n", __FILE__, __LINE__, __FUNCTION__, a, b); \
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


bool check_preedit(char* input, uint16_t preedit)
{
  TEST(input, preedit);
  han_ctx_t* ctx = han_new();
  for (int i = 0; i < strlen(input); ++i) {
    if (input[i] == '\b') {
      han_backspace(ctx);
      continue;
    }
    uint16_t ret = han_process(ctx, input[i]);
  }
  uint16_t combined = han_preedit(ctx);
  ASSERT(combined == preedit, "%lc == %lc", combined, preedit)
  han_free(ctx);
}

bool check_commit(char* input, uint16_t commit)
{
  TEST(input, commit);
  han_ctx_t* ctx = han_new();
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
  han_free(ctx);
}


int main(int argc, char* argv[])
{
  setlocale(LC_ALL, "");
  check_commit("rkW", L'가');
  check_preedit("rkW", L'ㅉ');
  check_commit("qjTm", L'버');
  check_preedit("qjTm", L'쓰');
  check_preedit("akfr", L'맑');
  check_commit("akfrh", L'말');
  check_preedit("akfrh", L'고');

  check_preedit("rt", L'ㅅ');
  check_commit("rtk", L'ㄱ');
  check_preedit("rtk", L'사');

  /* backspace */
  check_preedit("rkT\b", L'가');
  check_preedit("rt\bk", L'ㅏ');
  check_preedit("akfr\b", L'말');
  check_preedit("dnp\b", L'우');
  check_preedit("qqnpfr\b\b\b\b\b\b", L'\0');
  check_preedit("Qnpfr\b\b\b\b\b", L'\0');
  return ret;
}
