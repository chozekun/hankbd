#include "limited_han.h"

static const uint16_t SYLLABLES[] = {
  L'ㄱ', L'ㄲ', L'ㄴ', L'ㄷ', L'ㄸ', L'ㄹ', L'ㅁ', L'ㅂ', L'ㅃ', L'ㅅ', L'ㅆ',
  L'ㅇ', L'ㅈ', L'ㅉ', L'ㅊ', L'ㅋ', L'ㅌ', L'ㅍ', L'ㅎ',
  L'ㅏ', L'ㅐ', L'ㅑ', L'ㅒ', L'ㅓ', L'ㅔ', L'ㅕ', L'ㅖ', L'ㅗ', L'ㅘ', L'ㅙ',
  L'ㅚ', L'ㅛ', L'ㅜ', L'ㅝ', L'ㅞ', L'ㅟ', L'ㅠ', L'ㅡ', L'ㅢ', L'ㅣ',
  L'가', L'각', L'간'
};

static int16_t binary_search(const uint16_t chars[], int16_t l, int16_t r, uint16_t x)
{
  // the loop will run till there are elements in the
  // subarray as l > r means that there are no elements to
  // consider in the given subarray
  while (l <= r) {
    int16_t m = l + ((r - l) >> 1);
    if (chars[m] == x) {
      return m;
    }
    if (chars[m] < x) {
      l = m + 1;
    } else {
      r = m - 1;
    }
  }
  return -1;
}

static bool find_syllable(char cho, char jung, char jong)
{
  uint16_t code = make_code(cho, jung, jong);
  return binary_search(SYLLABLES, 0, countof(SYLLABLES)-1, code) >= 0;
}

han_ctx_t* limited_han_new()
{
  han_ctx_t* ret = han_new();
  han_set_callback(ret, find_syllable);
  return ret;
}
