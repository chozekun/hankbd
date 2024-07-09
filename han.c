#include "han.h"

#define ARR_SIZE(a) (sizeof(a) / sizeof(a[0]))

enum { KEY_NONE = -1 };

/*
// 1161 ᅡ HANGUL JUNGSEONG A
// 1162 ᅢ HANGUL JUNGSEONG AE
// 1163 ᅣ HANGUL JUNGSEONG YA
// 1164 ᅤ HANGUL JUNGSEONG YAE
// 1165 ᅥ HANGUL JUNGSEONG EO
// 1166 ᅦ HANGUL JUNGSEONG E
// 1167 ᅧ HANGUL JUNGSEONG YEO
// 1168 ᅨ HANGUL JUNGSEONG YE
// 1169 ᅩ HANGUL JUNGSEONG O
// 116A ᅪ HANGUL JUNGSEONG WA
// 116B ᅫ HANGUL JUNGSEONG WAE
// 116C ᅬ HANGUL JUNGSEONG OE
// 116D ᅭ HANGUL JUNGSEONG YO
// 116E ᅮ HANGUL JUNGSEONG U
// 116F ᅯ HANGUL JUNGSEONG WEO
// 1170 ᅰ HANGUL JUNGSEONG WE
// 1171 ᅱ HANGUL JUNGSEONG WI
// 1172 ᅲ HANGUL JUNGSEONG YU
// 1173 ᅳ HANGUL JUNGSEONG EU
// 1174 ᅴ HANGUL JUNGSEONG YI
// 1175 ᅵ HANGUL JUNGSEONG I
*/

// 19
enum CHO {
//ㄱ   ㄲ    ㄴ   ㄷ   ㄸ    ㄹ   ㅁ   ㅂ   ㅃ    ㅅ   ㅆ    ㅇ   ㅈ   ㅉ    ㅊ    ㅋ   ㅌ   ㅍ   ㅎ
  G__, GG__, N__, D__, DD__, R__, M__, B__, BB__, S__, SS__, O__, J__, JJ__, CH__, K__, T__, P__, H__
};

// 21
enum JUNG {
//ㅏ   ㅐ    ㅑ    ㅒ     ㅓ    ㅔ   ㅕ     ㅖ    ㅗ   ㅘ    ㅙ     ㅚ    ㅛ    ㅜ   ㅝ     ㅞ    ㅟ    ㅠ    ㅡ    ㅢ    ㅣ
  _A_, _AE_, _YA_, _YAE_, _EO_, _E_, _YEO_, _YE_, _O_, _WA_, _WAE_, _OE_, _YO_, _U_, _WEO_, _WE_, _WI_, _YU_, _EU_, _YI_, _I_
};

// 28
enum JONG {
//     ㄱ   ㄲ    ㄳ    ㄴ   ㄵ    ㄶ    ㄷ   ㄹ   ㄺ    ㄻ    ㄼ    ㄽ    ㄾ    ㄿ    ㅀ    ㅁ   ㅂ   ㅄ    ㅅ   ㅆ    ㅇ    ㅈ   ㅊ    ㅋ   ㅌ   ㅍ   ㅎ
  ___, __G, __GG, __GS, __N, __NJ, __NH, __D, __L, __LG, __LM, __LB, __LS, __LT, __LP, __LH, __M, __B, __BS, __S, __SS, __NG, __J, __CH, __K, __T, __P, __H
};


static const char KEY_CHO[] = {
//a    b   c     d    e    f    g    h   i   j   k   l   m   n   o   p   q    r    s    t    u   v    w    x    y   z
  M__, -1, CH__, O__, D__, R__, H__, -1, -1, -1, -1, -1, -1, -1, -1, -1, B__, G__, N__, S__, -1, P__, J__, T__, -1, K__
};

static const char KEY_CHO_SH[] = {
//A    B   C     D    E     F    G    H   I   J   K   L   M   N   O   P   Q     R     S    T     U   V    W     X    Y   Z
  M__, -1, CH__, O__, DD__, R__, H__, -1, -1, -1, -1, -1, -1, -1, -1, -1, BB__, GG__, N__, SS__, -1, P__, JJ__, T__, -1, K__
};

static const char KEY_JUNG[] = {
// a   b     c   d   e   f   g   h    i     j     k    l    m     n    o     p    q   r   s   t   u      v   w   x   y     z
  -1, _YU_, -1, -1, -1, -1, -1, _O_, _YA_, _EO_, _A_, _I_, _EU_, _U_, _AE_, _E_, -1, -1, -1, -1, _YEO_, -1, -1, -1, _YO_, -1
};

static const char KEY_JUNG_SH[] = {
// A   B     C   D   E   F   G   H    I     J     K    L    M     N    O      P     Q   R   S   T   U      V   W   X   Y     Z
  -1, _YU_, -1, -1, -1, -1, -1, _O_, _YA_, _EO_, _A_, _I_, _EU_, _U_, _YAE_, _YE_, -1, -1, -1, -1, _YEO_, -1, -1, -1, _YO_, -1
};

static const char KEY_JONG[] = {
//  a   b    c     d     e    f    g   h   i   j   k   l   m   n   o   p    q    r    s    t   u    v    w    x   y    z
  __M, -1, __CH, __NG, __D, __L, __H, -1, -1, -1, -1, -1, -1, -1, -1, -1, __B, __G, __N, __S, -1, __P, __J, __T, -1, __K
};

static const char KEY_JONG_SH[] = {
//  A   B    C     D     E    F    G   H   I   J   K   L   M   N   O   P    Q    R     S    T   U     V    W    X   Y    Z
  __M, -1, __CH, __NG, __D, __L, __H, -1, -1, -1, -1, -1, -1, -1, -1, -1, __B, __GG, __N, __SS, -1, __P, __J, __T, -1, __K
};

static const char JONG_TO_CHO[] = {
//    ㄱ   ㄲ    ㄳ   ㄴ   ㄵ   ㄶ   ㄷ   ㄹ   ㄺ   ㄻ   ㄼ   ㄽ   ㄾ   ㄿ   ㅀ   ㅁ   ㅂ   ㅄ   ㅅ   ㅆ    ㅇ   ㅈ   ㅊ    ㅋ   ㅌ   ㅍ   ㅎ
  -1, G__, GG__, S__, N__, J__, H__, D__, R__, G__, M__, B__, S__, T__, P__, H__, M__, B__, S__, S__, SS__, O__, J__, CH__, K__, T__, P__, H__ 
};

static const char JONG_BS[] = {
//  ㄱ ㄲ   ㄳ  ㄴ  ㄵ   ㄶ ㄷ ㄹ   ㄺ   ㄻ   ㄼ   ㄽ   ㄾ   ㄿ   ㅀ ㅁ ㅂ   ㅄ ㅅ ㅆ ㅇ ㅈ ㅊ ㅋ ㅌ ㅍ ㅎ
  0, 0, 0, __G, 0, __N, __N, 0, 0, __L, __L, __L, __L, __L, __L, __L, 0, 0, __B, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

static const char JUNG_BS[] = {
//ㅏ  ㅐ  ㅑ  ㅒ  ㅓ  ㅔ  ㅕ  ㅖ  ㅗ   ㅘ   ㅙ   ㅚ  ㅛ  ㅜ   ㅝ   ㅞ  ㅟ   ㅠ  ㅡ   ㅢ   ㅣ
  -1, -1, -1, -1, -1, -1, -1, -1, -1, _O_, _O_, _O_, -1, -1, _U_, _U_, _U_, -1, -1, _EU_, -1
};

static enum STATE { S_CHO, S_JUNG, S_JONG } state = S_CHO;
static int16_t cho = -1, jung = -1, jong = 0;

typedef struct combine_table_t
{
  char org;
  char begin;
  uint32_t mask;
} combine_table_t;

static const combine_table_t COMBINE_JUNG[] =
{
  //           ㅗㅏ->ㅘ,    ㅗㅐ->ㅙ,     ㅗㅣ->ㅚ
  {_O_, _O_+1, (1 << _A_) | (1 << _AE_) | (1 << _I_)},
  //           ㅜㅓ->ㅝ,     ㅜㅔ->ㅞ,    ㅜㅣ->ㅟ
  {_U_, _U_+1, (1 << _EO_) | (1 << _E_) | (1 << _I_)},
  //             ㅡㅣ->ㅢ
  {_EU_, _EU_+1, (1 << _I_)}
};

static const combine_table_t COMBINE_JONG[] =
{
  //            ㄱㅅ->ㄳ (+2 for skip ㄲ)
  {__G, __G+2, (1 << __S)},
  //            ㄴㅈ->ㄵ,   ㄴㅎ->ㄶ
  {__N, __N+1, (1 << __J) | (1 << __H)},
  //            ㄹㄱ->ㄺ,   ㄹㅁ->ㄻ,    ㄹㅂ->ㄼ,    ㄹㅅ->ㄽ,    ㄹㅌ->ㄾ,    ㄹㅍ->ㄿ,    ㄹㅎ->ㅀ
  {__L, __L+1, (1 << __G) | (1 << __M) | (1 << __B) | (1 << __S) | (1 << __T) | (1 << __P) | (1 << __H)},
  //            ㅂㅅ->ㅄ
  {__B, __B+1, (1 << __S)}
};

static int16_t combine(const combine_table_t* tbl, int tbl_len, char org, char input)
{
  const uint32_t bit = 1 << input;
  while (tbl_len--) {
    if (tbl[tbl_len].org == org) {
      const uint32_t mask = tbl[tbl_len].mask;
      if (mask & bit) {
        return tbl[tbl_len].begin + __builtin_popcount(mask & (bit - 1));
      }
      break;
    }
  }
  return -1;
}

static int16_t combine_jung(char org, char c)
{
  return combine(COMBINE_JUNG, ARR_SIZE(COMBINE_JUNG), org, c);
}

static int16_t combine_jong(char org, char c)
{
  return combine(COMBINE_JONG, ARR_SIZE(COMBINE_JONG), org, c);
}

static uint16_t make_code()
{
  static const uint16_t CHO_UNICODE[] = {
    1, 2, 4, 7, 8, 9, 17, 18, 19, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30
  };

  if (cho >= 0 && jung >= 0) {
    return 0xAC00 + cho * (21 * 28) + jung * 28 + jong;
  } else if (cho >= 0) {
    return 0x3130 + CHO_UNICODE[cho];
  } else if (jung >= 0) {
    return 0x314F + jung;
  }
  return 0;
}

uint16_t han_process(char key)
{
  bool shift;
  int16_t c;
  uint16_t ret = 0;

  if ('a' <= key && key <= 'z') {
    key -= 'a';
    shift = 0;
  } else if ('A' <= key && key <= 'Z') {
    key -= 'A';
    shift = 1;
  } else {
    return 0;
  }

  while (key >= 0) {
    switch (state) {
      case S_CHO:
        c = shift ? KEY_CHO_SH[key] : KEY_CHO[key];
        if (c >= 0) {
          if (cho == -1) {
            cho = c;
            key = KEY_NONE;
          } else {
            ret = make_code();
            cho = -1;
          }
        } else {
          state = S_JUNG;
        }
        break;

      case S_JUNG:
        c = shift ? KEY_JUNG_SH[key] : KEY_JUNG[key];
        if (c >= 0 && (jung == -1 || (c = combine_jung(jung, c)) >= 0)) {
          jung = c;
          key = KEY_NONE;
        } else if (cho == -1) {
          ret = make_code();
          jung = -1;
          state = S_CHO;
        } else {
          state = S_JONG;
        }
        break;

      case S_JONG:
        c = shift ? KEY_JONG_SH[key] : KEY_JONG[key];
        if (c >= 0 && (jong == 0 || (c = combine_jong(jong, c)) >= 0)) {
          jong = c;
          key = KEY_NONE;
        } else {
          if (KEY_JUNG[key] >= 0) {
            c = JONG_TO_CHO[jong];
            jong = JONG_BS[jong];
          } else {
            c = -1;
          }

          ret = make_code();
          cho = c;
          jung = -1;
          jong = 0;
          state = S_CHO;
        }
        break;
    }
  }

  return ret;
}

uint16_t han_preedit()
{
  return make_code();
}

bool han_is_empty()
{
  return cho == -1 && jung == -1 && jong == 0;
}

uint16_t han_flush()
{
  uint16_t ret = make_code();

  state = S_CHO;
  cho = -1;
  jung = -1;
  jong = 0;

  return ret;
}

bool han_backspace()
{
  if (jong > 0) {
    jong = JONG_BS[jong];
  } else if (jung >= 0) {
    jung = JUNG_BS[jung];
  } else if (cho >= 0) {
    cho = -1;
  } else {
    return false;
  }
  return true;
}
