#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <wchar.h>
#include <locale.h>

#ifndef countof
#define countof(array) (sizeof (array) / sizeof ((array)[0]))
#endif

typedef struct wchar_range_t {
  wchar_t start;
  wchar_t end;
} wchar_range_t;

typedef struct wchar_pair_t {
  wchar_t ch0;
  wchar_t ch1;
} wchar_pair_t;

typedef struct preedit_t {
  wchar_t cho;
  wchar_t jung;
  wchar_t jong;
} preedit_t;

typedef struct ctx_t {
  preedit_t preedit;
  wchar_t commit[2];
} ctx_t;

const wchar_range_t SYLLABLE = {L'가', L'힣'};
const wchar_range_t JAUM = {L'ㄱ', L'ㅎ'};
const wchar_range_t MOUM = {L'ㅏ', L'ㅣ'};

const wchar_t CHO[] = L"ㄱㄲㄴㄷㄸㄹㅁㅂㅃㅅㅆㅇㅈㅉㅊㅋㅌㅍㅎ";  // 19
const wchar_t JUNG[] = L"ㅏㅐㅑㅒㅓㅔㅕㅖㅗㅘㅙㅚㅛㅜㅝㅞㅟㅠㅡㅢㅣ";  // 21
const wchar_t JONG[] = L" ㄱㄲㄳㄴㄵㄶㄷㄹㄺㄻㄼㄽㄾㄿㅀㅁㅂㅄㅅㅆㅇㅈㅊㅋㅌㅍㅎ";  // 28

const wchar_t LOWER[] = L"ㅁㅠㅊㅇㄷㄹㅎㅗㅑㅓㅏㅣㅡㅜㅐㅔㅂㄱㄴㅅㅕㅍㅈㅌㅛㅋ";
const wchar_t UPPER[] = L"ㅁㅠㅊㅇㄸㄹㅎㅗㅑㅓㅏㅣㅡㅜㅒㅖㅃㄲㄴㅆㅕㅍㅉㅌㅛㅋ";

const wchar_t COMBINED_CHO[][3] = {
  { L'ㄱ', L'ㄱ', L'ㄲ' },
  { L'ㄷ', L'ㄷ', L'ㄸ' },
  { L'ㅂ', L'ㅂ', L'ㅃ' },
  { L'ㅅ', L'ㅅ', L'ㅆ' },
  { L'ㅈ', L'ㅈ', L'ㅉ' },
};

const wchar_t COMBINED_JUNG[][3] = {
  { L'ㅗ', L'ㅏ', L'ㅘ' },
  { L'ㅗ', L'ㅐ', L'ㅙ' },
  { L'ㅗ', L'ㅣ', L'ㅚ' },
  { L'ㅜ', L'ㅓ', L'ㅝ' },
  { L'ㅜ', L'ㅔ', L'ㅞ' },
  { L'ㅜ', L'ㅣ', L'ㅟ' },
  { L'ㅡ', L'ㅣ', L'ㅢ' },
};

const wchar_t COMBINED_JONG[][3] = {
  { L'ㄱ', L'ㄱ', L'ㄲ' },
  { L'ㄱ', L'ㅅ', L'ㄳ' },
  { L'ㄴ', L'ㅈ', L'ㄵ' },
  { L'ㄴ', L'ㅎ', L'ㄶ' },
  { L'ㄹ', L'ㄱ', L'ㄺ' },
  { L'ㄹ', L'ㅁ', L'ㄻ' },
  { L'ㄹ', L'ㅂ', L'ㄼ' },
  { L'ㄹ', L'ㅅ', L'ㄽ' },
  { L'ㄹ', L'ㅌ', L'ㄾ' },
  { L'ㄹ', L'ㅍ', L'ㄿ' },
  { L'ㄹ', L'ㅎ', L'ㅀ' },
  { L'ㅂ', L'ㅅ', L'ㅄ' },
  { L'ㅅ', L'ㅅ', L'ㅆ' },
};

wchar_pair_t separate_compo(const wchar_t set[][3], int count, wchar_t ch)
{
  wchar_pair_t ret = { ch, 0 };
  for (int i = 0; i < count; ++i) {
    if (ch == set[i][2]) {
      ret.ch0 = set[i][0];
      ret.ch1 = set[i][1];
      break;
    }
  }
  return ret;
}

wchar_t combine_compo(const wchar_t set[][3], int count, wchar_t ch1, wchar_t ch2)
{
  if (ch2 == 0) {
    return ch1;
  }
  for (int i = 0; i < count; ++i) {
    if (ch1 == set[i][0] && ch2 == set[i][1]) {
      return set[i][2];
    }
  }
  return 0;
}

int find_index(const wchar_t* set, int count, wchar_t ch)
{
  for (int i = 0; i < count; ++i) {
    if (ch == set[i]) {
      return i;
    }
  }
  return -1;
}

wchar_pair_t separate_cho(wchar_t ch)
{
  return separate_compo(COMBINED_CHO, countof(COMBINED_CHO), ch);
}

wchar_pair_t separate_jung(wchar_t ch)
{
  return separate_compo(COMBINED_JUNG, countof(COMBINED_JUNG), ch);
}

wchar_pair_t separate_jong(wchar_t ch)
{
  return separate_compo(COMBINED_JONG, countof(COMBINED_JONG), ch);
}

wchar_t combine_cho(wchar_t ch1, wchar_t ch2)
{
  return combine_compo(COMBINED_CHO, countof(COMBINED_CHO), ch1, ch2);
}

wchar_t combine_jung(wchar_t ch1, wchar_t ch2)
{
  return combine_compo(COMBINED_JUNG, countof(COMBINED_JUNG), ch1, ch2);
}

wchar_t combine_jong(wchar_t ch1, wchar_t ch2)
{
  return combine_compo(COMBINED_JONG, countof(COMBINED_JONG), ch1, ch2);
}

int find_index_cho(wchar_t ch)
{
  return find_index(CHO, countof(CHO), ch);
}

int find_index_jung(wchar_t ch)
{
  return find_index(JUNG, countof(JUNG), ch);
}

int find_index_jong(wchar_t ch)
{
  return find_index(JONG, countof(JONG), ch);
}

wchar_t combine(wchar_t cho, wchar_t jung, wchar_t jong)
{
  int i1 = cho ? find_index_cho(cho) : -1;
  int i2 = jung ? find_index_jung(jung) : -1;
  int i3 = jong ? find_index_jong(jong) : 0;
  if (i1 >= 0 && i2 >= 0) {
    return SYLLABLE.start + (21 * 28 * i1) + (28 * i2) + i3;
  }
  return cho ? cho : jung ? jung : jong ? jong : 0;
}

bool is_jaum(wchar_t ch)
{
  return JAUM.start <= ch && ch <= JAUM.end;
}

bool is_moum(wchar_t ch)
{
  return MOUM.start <= ch && ch <= MOUM.end;
}

bool is_jamo(wchar_t ch)
{
  return is_jaum(ch) || is_moum(ch);
}

void ctx_init(ctx_t* ctx)
{
  ctx->preedit.cho = 0;
  ctx->preedit.jung = 0;
  ctx->preedit.jong = 0;
  ctx->commit[0] = 0;
  ctx->commit[1] = 0;
}

void ctx_commit_preedit(ctx_t* ctx)
{
  ctx->commit[0] = combine(ctx->preedit.cho, ctx->preedit.jung, ctx->preedit.jong);
  ctx->preedit.cho = 0;
  ctx->preedit.jung = 0;
  ctx->preedit.jong = 0;
}

void ctx_commit_char(ctx_t* ctx, wchar_t ch)
{
  ctx->commit[1] = ch;
}

void ctx_clear_commit(ctx_t* ctx)
{
  ctx->commit[0] = 0;
  ctx->commit[1] = 0;
}

bool ctx_process(ctx_t* ctx, wchar_t ch)
{
  if (!is_jamo(ch) && ch > 0) {
    ctx_commit_preedit(ctx);
    ctx_commit_char(ctx, ch);
    return true;
  }
  preedit_t* p = &ctx->preedit;
  if (p->jong) {
    if (is_jaum(ch)) {
      wchar_t combined_jong = combine_jong(p->jong, ch);
      if (combined_jong) {
        p->jong = combined_jong;
      } else {
        ctx_commit_preedit(ctx);
        p->cho = ch;
        return true;
      }
    } else if (is_moum(ch)) {
      wchar_pair_t separated_jong = separate_jong(p->jong);
      if (separated_jong.ch1) {
        p->jong = separated_jong.ch0;
        ctx_commit_preedit(ctx);
        p->cho = separated_jong.ch1;
        p->jung = ch;
        return true;
      } else {
        p->jong = 0;
        ctx_commit_preedit(ctx);
        p->cho = separated_jong.ch0;
        p->jung = ch;
        return true;
      }
    }
  } else if (p->jung) {
    if (is_jaum(ch)) {
      p->jong = ch;
    } else if (is_moum(ch)) {
      wchar_t combined_jung = combine_jung(p->jung, ch);
      if (combined_jung) {
        p->jung = combined_jung;
      } else {
        ctx_commit_preedit(ctx);
        p->jung = ch;
        // preedit is not perfect
        return true;
      }
    }
  } else if (p->cho) {
    if (is_jaum(ch)) {
       wchar_t combined_cho = combine_cho(p->cho, ch);
       if (combined_cho) {
         p->cho = combined_cho;
       } else {
        // commit is not perfect
        ctx_commit_preedit(ctx);
        p->cho = ch;
        return true;
       }
    } else if (is_moum(ch)) {
      p->jung = ch;
    }
  } else {
    if (is_jaum(ch)) {
      p->cho = ch;
    } else if (is_moum(ch)) {
      // preedit is not perfect
      p->jung = ch;
    }
  }
  return false;
}

int main(int argc, char* argv[])
{
  setlocale(LC_ALL, "");
  ctx_t ctx;
  ctx_init(&ctx);
  while (true) {
    wint_t wc = getwchar();
    if (wc == L'\r' || wc == L'\n') {
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
