#pragma once

#include <stdint.h>
#include <stdbool.h>

#ifndef countof
#define countof(a) (sizeof(a) / sizeof(a[0]))
#endif

typedef struct han_ctx_t_ han_ctx_t;

han_ctx_t* han_new();
void han_free(han_ctx_t* ctx);
void han_init(han_ctx_t* ctx);
void han_set_charset(han_ctx_t* ctx, uint16_t* charset, uint16_t size);
bool han_is_empty(han_ctx_t* ctx);
uint16_t han_preedit(han_ctx_t* ctx);
uint16_t han_flush(han_ctx_t* ctx);
bool han_backspace(han_ctx_t* ctx);
uint16_t han_process(han_ctx_t* ctx, char key);
