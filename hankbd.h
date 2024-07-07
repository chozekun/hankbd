#pragma once

#include <stdbool.h>
#include <wchar.h>

#ifndef countof
#define countof(array) (sizeof (array) / sizeof ((array)[0]))
#endif

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

wchar_pair_t separate_jung(wchar_t ch);
wchar_pair_t separate_jong(wchar_t ch);
wchar_t combine_jung(wchar_t ch1, wchar_t ch2);
wchar_t combine_jong(wchar_t ch1, wchar_t ch2);
int find_index_cho(wchar_t ch);
int find_index_jung(wchar_t ch);
int find_index_jong(wchar_t ch);
wchar_t combine(wchar_t cho, wchar_t jung, wchar_t jong);
bool is_jaum(wchar_t ch);
bool is_moum(wchar_t ch);
bool is_jamo(wchar_t ch);
void ctx_init(ctx_t* ctx);
wchar_t ctx_combine_preedit(const ctx_t* ctx);
void ctx_commit_preedit(ctx_t* ctx);
void ctx_commit_char(ctx_t* ctx, wchar_t ch);
void ctx_clear_commit(ctx_t* ctx);
bool ctx_backspace(ctx_t* ctx);
bool ctx_process(ctx_t* ctx, wchar_t ch);
