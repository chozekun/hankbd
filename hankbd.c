#include "hankbd.h"

typedef struct wchar_range_t {
  wchar_t start;
  wchar_t end;
} wchar_range_t;

static const wchar_range_t SYLLABLE = {L'가', L'힣'};
static const wchar_range_t JAUM = {L'ㄱ', L'ㅎ'};
static const wchar_range_t MOUM = {L'ㅏ', L'ㅣ'};

static const wchar_t CHO[] = L"ㄱㄲㄴㄷㄸㄹㅁㅂㅃㅅㅆㅇㅈㅉㅊㅋㅌㅍㅎ";  // 19
static const wchar_t JUNG[] = L"ㅏㅐㅑㅒㅓㅔㅕㅖㅗㅘㅙㅚㅛㅜㅝㅞㅟㅠㅡㅢㅣ";  // 21
static const wchar_t JONG[] = L" ㄱㄲㄳㄴㄵㄶㄷㄹㄺㄻㄼㄽㄾㄿㅀㅁㅂㅄㅅㅆㅇㅈㅊㅋㅌㅍㅎ";  // 28

static const wchar_t LOWER[] = L"ㅁㅠㅊㅇㄷㄹㅎㅗㅑㅓㅏㅣㅡㅜㅐㅔㅂㄱㄴㅅㅕㅍㅈㅌㅛㅋ";
static const wchar_t UPPER[] = L"ㅁㅠㅊㅇㄸㄹㅎㅗㅑㅓㅏㅣㅡㅜㅒㅖㅃㄲㄴㅆㅕㅍㅉㅌㅛㅋ";

static const wchar_t COMBINED_JUNG[][3] = {
  { L'ㅗ', L'ㅏ', L'ㅘ' },
  { L'ㅗ', L'ㅐ', L'ㅙ' },
  { L'ㅗ', L'ㅣ', L'ㅚ' },
  { L'ㅜ', L'ㅓ', L'ㅝ' },
  { L'ㅜ', L'ㅔ', L'ㅞ' },
  { L'ㅜ', L'ㅣ', L'ㅟ' },
  { L'ㅡ', L'ㅣ', L'ㅢ' },
};

static const wchar_t COMBINED_JONG[][3] = {
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
};


static wchar_pair_t separate_compo(const wchar_t set[][3], int count, wchar_t ch)
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

static wchar_t combine_compo(const wchar_t set[][3], int count, wchar_t ch1, wchar_t ch2)
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

static int find_index(const wchar_t* set, int count, wchar_t ch)
{
  for (int i = 0; i < count; ++i) {
    if (ch == set[i]) {
      return i;
    }
  }
  return -1;
}

wchar_pair_t separate_jung(wchar_t ch)
{
  return separate_compo(COMBINED_JUNG, countof(COMBINED_JUNG), ch);
}

wchar_pair_t separate_jong(wchar_t ch)
{
  return separate_compo(COMBINED_JONG, countof(COMBINED_JONG), ch);
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

wchar_t ctx_combine_preedit(const ctx_t* ctx)
{
  return combine(ctx->preedit.cho, ctx->preedit.jung, ctx->preedit.jong);
}

void ctx_commit_preedit(ctx_t* ctx)
{
  ctx->commit[0] = ctx_combine_preedit(ctx);
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

bool ctx_backspace(ctx_t* ctx)
{
  preedit_t* p = &ctx->preedit;
  if (p->jong) {
    wchar_pair_t separated_jong = separate_jong(p->jong);
    if (separated_jong.ch1) {
      p->jong = separated_jong.ch0;
    } else {
      p->jong = 0;
    }
  } else if (p->jung) {
    wchar_pair_t separated_jung = separate_jung(p->jung);
    if (separated_jung.ch1) {
      p->jung = separated_jung.ch0;
    } else {
      p->jung = 0;
    }
  } else if (p->cho) {
    p->cho = 0;
  } else {
    return false;
  }
  return true;
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
      if (p->cho) {
        if (find_index_jong(ch) >= 1) {
          p->jong = ch;
        } else {
          ctx_commit_preedit(ctx);
          p->cho = ch;
          return true;
        }
      } else {
        ctx_commit_preedit(ctx);
        p->cho = ch;
        return true;
      }
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
      wchar_t combined_jong = combine_jong(p->cho, ch);
      /* 초성을 입력한 combine 함수에서 종성이 나오게 된다면
       * 이전 초성도 종성으로 바꿔 주는 편이 나머지 처리에 편리하다.
       * 이 기능은 MS IME 호환기능으로 ㄳ을 입력하는데 사용한다. */
      if (combined_jong) {
        p->cho = 0;
        p->jong = combined_jong;
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
