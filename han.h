#pragma once

#include <stdint.h>
#include <stdbool.h>

bool han_is_empty();
uint16_t han_flush();
bool han_backspace();
uint16_t han_preedit();
uint16_t han_process(char key);
