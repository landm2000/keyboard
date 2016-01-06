// Keyboard
//
// This source code is using the UTF-8 unicode
//

#include "keyboard01_bin.h"
#include "keyboard02_bin.h"
#include "keyboard03_bin.h"

#include "keyboard05_bin.h"
#include "keyboard06_bin.h"
#include "keyboard07_bin.h"

#include "keyboard09_bin.h"
#include "keyboard10_bin.h"
#include "keyboard11_bin.h"

#include "font_ascii_bin.h"



#define KEY_FUNCTION_WIDTH  28
#define KEY_FUNCTION_HEIGHT 28
#define KEY_NORMAL_WIDTH    19
#define KEY_NORMAL_HEIGHT   39
#define KEY_TAB_WIDTH       29
#define KEY_DELETE_WIDTH    28
#define KEY_RETURN_WIDTH    39
#define KEY_CAPS_WIDTH      39
#define KEY_SHIFT_WIDTH     49
#define KEY_SPACE_WIDTH    125
#define KEY_SPACE_HEIGHT    31
#define KEY_COMMAND_WIDTH   39
#define KEY_ALT_WIDTH       19

#define KEY_IMAGE_WIDTH    320
#define KEY_IMAGE_HEIGHT   240
#define FONT_ASCII_IMAGE_WIDTH    128
#define FONT_ASCII_IMAGE_HEIGHT   192
#define FONT_WIDTH          8
#define FONT_HEIGHT         8
#define FONT_Y_MAX         23
#define SCREEN_WIDTH        0
#define SCREEN_HEIGHT      29

#define NOT_PRESSED             0
#define SHIFT_PRESSED           1
#define CAPS_PRESSED            2

#define KANA_PRESSED            3
#define KANA_SHIFT_PRESSED      4
#define KANA_CAPS_PRESSED       5

#define KATAKANA_PRESSED        6
#define KATAKANA_SHIFT_PRESSED  7
#define KATAKANA_CAPS_PRESSED   8


typedef struct xy { int px; int py; } XY;
typedef struct xx { int x_start; int x_end; } XX;
typedef struct yy { int y_start; int y_end; } YY;

typedef struct unicode { unsigned int code; wchar_t *str; } UNICODE;

struct yy keyboard_line_y [] = {{4,30},{38,74},{80,116},{122,158},{164,200},{206,234}};
int keyboards_rows[] = {7,15,14,13,12,6};
struct xx keyboard_line_x [][15] = {
    { {3,30},{34,61},{65,92},{96,123},{127,154},{158,185},{189,215}},
    { {3,21},{24,42},{45,63},{66,84}, {87,105}, {108,126},{129,147},{150,168},{171,189},{192,210},{213,231},{234,252},{255,273},{276,294},{297,316}},
    { {4,31},{35,54},{56,74},{77,96}, {98,116}, {119,137},{140,158},{161,179},{182,200},{203,222},{224,242},{245,264},{266,284},{287,316},    {0,0}},
    { {4,42},{45,63},{66,84},{87,105},{108,126},{129,147},{150,168},{171,189},{192,210},{213,231},{234,252},{255,273},{276,316},    {0,0},    {0,0}},
    { {4,52},{56,74},{77,96},{98,116},{119,137},{140,158},{161,179},{182,200},{203,222},{224,242},{245,264},{266,316},    {0,0},    {0,0},    {0,0}},
    {        {35,53},{56,74},{77,96},{98,222},{225,263},{266,284}}
};
const unsigned char keyboard_code_01 [][15] = {
    {    1,   2,   3,   4,   5,   6},
    {    0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=',0x5C, '`'},
    { 0x09, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']',0x08 },
    { 0x0E, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',0x27,0x0A},
    { 0x0F, 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',0x0F},
    {    0,   0,   0, ' ',   0,   0}
};
const unsigned char keyboard_code_02 [][15] = {
    {    1,   2,   3,   4,   5,   6},
    {    0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '|', '~'},
    { 0x09, 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}',0x08 },
    { 0x0E, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"',0x0A},
    { 0x0F, 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?',0x0F},
    {    0,   0,   0, ' ',   0,   0}
};
const unsigned char keyboard_code_03 [][15] = {
    {    1,   2,   3,   4,   5,   6},
    {    0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=',0x5c, '`'},
    { 0x09, 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']',0x08 },
    { 0x0E, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';',0x27,0x0A},
    { 0x0F, 'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',', '.', '/',0x0F},
    {    0,   0,   0, ' ',   0,   0}
};
const unsigned char *keyboard_code_05 [][15] = {
    {(unsigned char *)1,(unsigned char *)2,(unsigned char *)3,(unsigned char *)4,(unsigned char *)5,(unsigned char *)6},
    {(unsigned char *)"あ",(unsigned char *)"い",(unsigned char *)"う",(unsigned char *)"え",(unsigned char *)"お",(unsigned char *)"か",(unsigned char *)"き",(unsigned char *)"く",(unsigned char *)"け",(unsigned char *)"こ",(unsigned char *)"さ",(unsigned char *)"し",(unsigned char *)"す",(unsigned char *)"せ",(unsigned char *)"そ"},
    {(unsigned char *)"\t",(unsigned char *)"た",(unsigned char *)"ち",(unsigned char *)"つ",(unsigned char *)"て",(unsigned char *)"と",(unsigned char *)"な",(unsigned char *)"に",(unsigned char *)"ぬ",(unsigned char *)"ね",(unsigned char *)"の",(unsigned char *)"、",(unsigned char *)"。",(unsigned char *)"\b",0},
    {0,(unsigned char *)"は",(unsigned char *)"ひ",(unsigned char *)"ふ",(unsigned char *)"へ",(unsigned char *)"ほ",(unsigned char *)"ま",(unsigned char *)"み",(unsigned char *)"む",(unsigned char *)"め",(unsigned char *)"も",(unsigned char *)"や",(unsigned char *)"\n",0,0},
    {0,(unsigned char *)"ら",(unsigned char *)"り",(unsigned char *)"る",(unsigned char *)"れ",(unsigned char *)"ろ",(unsigned char *)"わ",(unsigned char *)"を",(unsigned char *)"ん",(unsigned char *)"ゆ",(unsigned char *)"よ",0,0,0,0},
    {0,0,0,(unsigned char *)" ",0,0,0,0,0,0,0,0,0,0,0},
};
const unsigned char *keyboard_code_06 [][15] = {
    {(unsigned char *)1,(unsigned char *)2,(unsigned char *)3,(unsigned char *)4,(unsigned char *)5,(unsigned char *)6},
    {(unsigned char *)"ぁ",(unsigned char *)"ぃ",(unsigned char *)"ぅ",(unsigned char *)"ぇ",(unsigned char *)"ぉ",(unsigned char *)"が",(unsigned char *)"ぎ",(unsigned char *)"ぐ",(unsigned char *)"げ",(unsigned char *)"ご",(unsigned char *)"ざ",(unsigned char *)"じ",(unsigned char *)"ず",(unsigned char *)"ぜ",(unsigned char *)"ぞ"},
    {(unsigned char *)"\t",(unsigned char *)"だ",(unsigned char *)"ぢ",(unsigned char *)"づ",(unsigned char *)"で",(unsigned char *)"ど",0,0,0,(unsigned char *)"・",(unsigned char *)"ー",(unsigned char *)"「",(unsigned char *)"」",(unsigned char *)"\b",0},
    {0,(unsigned char *)"ば",(unsigned char *)"び",(unsigned char *)"ぶ",(unsigned char *)"べ",(unsigned char *)"ぼ",(unsigned char *)"ぱ",(unsigned char *)"ぴ",(unsigned char *)"ぷ",(unsigned char *)"ぺ",(unsigned char *)"ぽ",(unsigned char *)"ゃ",(unsigned char *)"\n",0,0},
    {0,0,0,0,0,0,0,0,0,(unsigned char *)"ゅ",(unsigned char *)"ょ",0,0,0,0},
    {0,0,0,(unsigned char *)" ",0,0,0,0,0,0,0,0,0,0,0},
};
const unsigned char *keyboard_code_09 [][15] = {
    {(unsigned char *)1,(unsigned char *)2,(unsigned char *)3,(unsigned char *)4,(unsigned char *)5,(unsigned char *)6},
    {(unsigned char *)"ア",(unsigned char *)"イ",(unsigned char *)"ウ",(unsigned char *)"エ",(unsigned char *)"オ",(unsigned char *)"カ",(unsigned char *)"キ",(unsigned char *)"ク",(unsigned char *)"ケ",(unsigned char *)"コ",(unsigned char *)"サ",(unsigned char *)"シ",(unsigned char *)"ス",(unsigned char *)"セ",(unsigned char *)"ソ"},
    {(unsigned char *)"\t",(unsigned char *)"タ",(unsigned char *)"チ",(unsigned char *)"ツ",(unsigned char *)"テ",(unsigned char *)"ト",(unsigned char *)"ナ",(unsigned char *)"ニ",(unsigned char *)"ヌ",(unsigned char *)"ネ",(unsigned char *)"ノ",(unsigned char *)"、",(unsigned char *)"。",(unsigned char *)"\b",0},
    {0,(unsigned char *)"ハ",(unsigned char *)"ヒ",(unsigned char *)"フ",(unsigned char *)"ヘ",(unsigned char *)"ホ",(unsigned char *)"マ",(unsigned char *)"ミ",(unsigned char *)"ム",(unsigned char *)"メ",(unsigned char *)"モ",(unsigned char *)"ヤ",(unsigned char *)"\n",0,0},
    {0,(unsigned char *)"ラ",(unsigned char *)"リ",(unsigned char *)"ル",(unsigned char *)"レ",(unsigned char *)"ロ",(unsigned char *)"ワ",(unsigned char *)"ヲ",(unsigned char *)"ン",(unsigned char *)"ユ",(unsigned char *)"ヨ",0,0,0,0},
    {0,0,0,(unsigned char *)" ",0,0,0,0,0,0,0,0,0,0,0},
};
const unsigned char *keyboard_code_10 [][15] = {
    {(unsigned char *)1,(unsigned char *)2,(unsigned char *)3,(unsigned char *)4,(unsigned char *)5,(unsigned char *)6},
    {(unsigned char *)"ァ",(unsigned char *)"ィ",(unsigned char *)"ゥ",(unsigned char *)"ェ",(unsigned char *)"ォ",(unsigned char *)"ガ",(unsigned char *)"ギ",(unsigned char *)"グ",(unsigned char *)"ゲ",(unsigned char *)"ゴ",(unsigned char *)"ザ",(unsigned char *)"ジ",(unsigned char *)"ズ",(unsigned char *)"ゼ",(unsigned char *)"ゾ"},
    {(unsigned char *)"\t",(unsigned char *)"ダ",(unsigned char *)"ヂ",(unsigned char *)"ヅ",(unsigned char *)"デ",(unsigned char *)"ド",0,0,0,(unsigned char *)"・",(unsigned char *)"ー",(unsigned char *)"「",(unsigned char *)"」",(unsigned char *)"\b",0},
    {0,(unsigned char *)"バ",(unsigned char *)"ビ",(unsigned char *)"ブ",(unsigned char *)"ベ",(unsigned char *)"ボ",(unsigned char *)"パ",(unsigned char *)"ピ",(unsigned char *)"プ",(unsigned char *)"ペ",(unsigned char *)"ポ",(unsigned char *)"ャ",(unsigned char *)"\n",0,0},
    {0,0,0,0,0,0,0,0,(unsigned char *)"ヴ",(unsigned char *)"ュ",(unsigned char *)"ョ",0,0,0,0},
    {0,0,0,(unsigned char *)" ",0,0,0,0,0,0,0,0,0,0,0},
};


u8 *keyboard_image[] = {(u8 *)keyboard01_bin, (u8 *)keyboard02_bin, (u8 *)keyboard03_bin,
    (u8 *)keyboard05_bin, (u8 *)keyboard06_bin, (u8 *)keyboard07_bin,
    (u8 *)keyboard09_bin, (u8 *)keyboard10_bin, (u8 *)keyboard11_bin};




