#pragma once

#include <stdint.h>
#include <stdbool.h>

#ifndef countof
#define countof(a) (sizeof(a) / sizeof(a[0]))
#endif

typedef struct han_ctx_t_ han_ctx_t;
typedef bool (*han_callback_t)(char cho, char jung, char jong);

uint16_t make_code(char cho, char jung, char jong);

han_ctx_t* han_new();
void han_free(han_ctx_t* ctx);
void han_init(han_ctx_t* ctx);
void han_set_callback(han_ctx_t* ctx, han_callback_t callback);
bool han_is_empty(han_ctx_t* ctx);
uint16_t han_preedit(han_ctx_t* ctx);
uint16_t han_flush(han_ctx_t* ctx);
bool han_backspace(han_ctx_t* ctx);
uint16_t han_process(han_ctx_t* ctx, char key);
