#include <stdlib.h>
#include "han.h"

#define ARR_SIZE(a) (sizeof(a) / sizeof(a[0]))

enum { KEY_NONE = -1 };
typedef enum state_t_ { S_CHO, S_JUNG, S_JONG } state_t;

struct han_ctx_t_
{
  state_t state;
  char cho;
  char jung;
  char jong;
};

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

static const char COMBINE_JUNG[][3] = {
  {_O_, _A_, _WA_}, // ㅗㅏ->ㅘ
  {_O_, _AE_, _WAE_}, // ㅗㅐ->ㅙ
  {_O_, _I_, _OE_}, // ㅗㅣ->ㅚ
  {_U_, _EO_, _WEO_}, // ㅜㅓ->ㅝ
  {_U_, _E_, _WE_}, // ㅜㅔ->ㅞ
  {_U_, _I_, _WI_}, // ㅜㅣ->ㅟ
  {_EU_, _I_, _YI_}, // ㅡㅣ->ㅢ
};

static const char COMBINE_JONG[][3] = {
  {__G, __S, __GS}, // ㄱㅅ->ㄳ
  {__N, __J, __NJ}, // ㄴㅈ->ㄵ
  {__N, __H, __NH}, // ㄴㅎ->ㄶ
  {__L, __G, __LG}, // ㄹㄱ->ㄺ
  {__L, __M, __LM}, // ㄹㅁ->ㄻ
  {__L, __B, __LB}, // ㄹㅂ->ㄼ
  {__L, __S, __LS}, // ㄹㅅ->ㄽ
  {__L, __T, __LT}, // ㄹㅌ->ㄾ
  {__L, __P, __LP}, // ㄹㅍ->ㄿ
  {__L, __H, __LH}, // ㄹㅎ->ㅀ
  {__B, __S, __BS}, // ㅂㅅ->ㅄ
};

static char combine(const char tbl[][3], int tbl_size, char ch1, char ch2)
{
  for (int i = 0; i < tbl_size; ++i) {
    if (ch1 == tbl[i][0] && ch2 == tbl[i][1]) {
      return tbl[i][2];
    }
  }
  return -1;
}

static char combine_jung(char ch1, char ch2)
{
  return combine(COMBINE_JUNG, ARR_SIZE(COMBINE_JUNG), ch1, ch2);
}

static char combine_jong(char ch1, char ch2)
{
  return combine(COMBINE_JONG, ARR_SIZE(COMBINE_JONG), ch1, ch2);
}

static uint16_t make_code(han_ctx_t* ctx)
{
  static const uint16_t CHO_UNICODE[] = {
    1, 2, 4, 7, 8, 9, 17, 18, 19, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30
  };

  if (ctx->cho >= 0 && ctx->jung >= 0) {
    return 0xAC00 + ctx->cho * (21 * 28) + ctx->jung * 28 + ctx->jong;
  } else if (ctx->cho >= 0) {
    return 0x3130 + CHO_UNICODE[ctx->cho];
  } else if (ctx->jung >= 0) {
    return 0x314F + ctx->jung;
  }
  return 0;
}

han_ctx_t* han_new()
{
  han_ctx_t* ctx = (han_ctx_t*)malloc(sizeof(han_ctx_t));
  han_init(ctx);
  return ctx;
}

void han_init(han_ctx_t* ctx)
{
  ctx->state = S_CHO;
  ctx->cho = -1;
  ctx->jung = -1;
  ctx->jong = 0;
}

void han_free(han_ctx_t* han)
{
  if (han) {
    free(han);
  }
}

bool han_is_empty(han_ctx_t* ctx)
{
  return ctx->cho == -1 && ctx->jung == -1 && ctx->jong == 0;
}

uint16_t han_preedit(han_ctx_t* ctx)
{
  return make_code(ctx);
}

uint16_t han_flush(han_ctx_t* ctx)
{
  uint16_t ret = make_code(ctx);
  han_init(ctx);
  return ret;
}

bool han_backspace(han_ctx_t* ctx)
{
  if (ctx->jong > 0) {
    ctx->jong = JONG_BS[ctx->jong];
  } else if (ctx->jung >= 0) {
    ctx->jung = JUNG_BS[ctx->jung];
  } else if (ctx->cho >= 0) {
    ctx->cho = -1;
  } else {
    return false;
  }
  return true;
}

uint16_t han_process(han_ctx_t* ctx, char key)
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
    switch (ctx->state) {
      case S_CHO:
        c = shift ? KEY_CHO_SH[key] : KEY_CHO[key];
        if (c >= 0) {
          if (ctx->cho == -1) {
            ctx->cho = c;
            key = KEY_NONE;
          } else {
            ret = make_code(ctx);
            ctx->cho = -1;
          }
        } else {
          ctx->state = S_JUNG;
        }
        break;

      case S_JUNG:
        c = shift ? KEY_JUNG_SH[key] : KEY_JUNG[key];
        if (c >= 0 && (ctx->jung == -1 || (c = combine_jung(ctx->jung, c)) >= 0)) {
          ctx->jung = c;
          key = KEY_NONE;
        } else if (ctx->cho == -1) {
          ret = make_code(ctx);
          ctx->jung = -1;
          ctx->state = S_CHO;
        } else {
          ctx->state = S_JONG;
        }
        break;

      case S_JONG:
        c = shift ? KEY_JONG_SH[key] : KEY_JONG[key];
        if (c >= 0 && (ctx->jong == 0 || (c = combine_jong(ctx->jong, c)) >= 0)) {
          ctx->jong = c;
          key = KEY_NONE;
        } else {
          if (KEY_JUNG[key] >= 0) {
            c = JONG_TO_CHO[ctx->jong];
            ctx->jong = JONG_BS[ctx->jong];
          } else {
            c = -1;
          }

          ret = make_code(ctx);
          ctx->cho = c;
          ctx->jung = -1;
          ctx->jong = 0;
          ctx->state = S_CHO;
        }
        break;
    }
  }

  return ret;
}
