#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <3ds.h>
#include "draw.h"

#include "keyboard.h"


unsigned char check_keybord(touchPosition touch, int *keyboard_image_no, int *last_key_shift, int *last_key_row, int *last_key_index, const unsigned char **u16p);
void key_size(int key_row,int key_index,struct xy *size,int *x);
void str16(const unsigned char *u16p,char *str);
void printChar(int x,int y,unsigned char code,const unsigned char *u16p);

int main()
{
    // Initializations
    srvInit();        // services
    aptInit();        // applets
    hidInit(NULL);    // input
    gfxInitDefault(); // graphics
    gfxSet3D(false);  // stereoscopy (true: enabled / false: disabled)
    u32 kDown;        // keys down
    u32 kHeld;        // keys pressed
    u32 kUp;          // keys up
//
    int last_key_shift = NOT_PRESSED;
    int last_key_row = -1;
    int last_key_index = -1;
    int keyboard_image_no = 0;
//
    int cursorX = 0;
    int cursorY = 0;
//

    gfxDrawSprite(GFX_BOTTOM, GFX_LEFT, (u8*)keyboard_image[keyboard_image_no], KEY_IMAGE_HEIGHT, 0,0, 240, 320, 0, 0);
    // Flush and swap framebuffers
    gfxFlushBuffers();
    gfxSwapBuffers();
    gspWaitForVBlank();
    gfxDrawSprite(GFX_BOTTOM, GFX_LEFT, (u8*)keyboard_image[keyboard_image_no], KEY_IMAGE_HEIGHT, 0,0, 240, 320, 0, 0);
    gfxFlushBuffers();
    gfxSwapBuffers();
    gspWaitForVBlank();

    // Main loop
    while (aptMainLoop())
    {
        
        // Wait for next frame
        gspWaitForVBlank();
        
        // Read which buttons are currently pressed or not
        hidScanInput();
        kDown = hidKeysDown();
        kHeld = hidKeysHeld();
        kUp = hidKeysUp();
        
        // If START button is pressed, break loop and quit
        if (kDown & KEY_START){
            break;
        }
        
        
        /** Your code goes here **/
        touchPosition touch;
        //Read the touch screen coordinates
        hidTouchRead(&touch);
        
        //Print the touch screen coordinates

        // Check keyboard
        const unsigned char *u16p = 0;
        unsigned char code = check_keybord(touch,&keyboard_image_no,&last_key_shift,&last_key_row, &last_key_index, &u16p);
        if (code != 0) {
            cursorX = 19;
            cursorY = 11;
            printChar(cursorX+4, cursorY+3, code, u16p);
        }
        // Flush and swap framebuffers
        gfxFlushBuffers();
        gfxSwapBuffers();
    }
    
    // Exit
    gfxExit();
    hidExit();
    aptExit();
    srvExit();
    
    // Return to hbmenu
    return 0;
}

void printChar(int x,int y,unsigned char code,const unsigned char *u16p){
    int font_x = 0;
    int font_y = 0;
    if (0 <= code && code < 0x7F) {
        font_x = (code & 0xF0) >> 4;
        font_y = code & 0xF;
//  }else if(0xc2 <= code && code <= 0xDF ){ // 2byte code
    }else if(code == 0xE3){
        unsigned int code2;
        u16p++;
        unsigned char byte2 = *u16p++;
        unsigned char byte3 = *u16p;
        if      (byte2 == 0x81 && 0x81 <= byte3 && byte3 <= 0xBF ){
            code2 = (unsigned int)byte3 - 0x80 + 0x80;
            font_y = code2 % 16;
            font_x = code2 / 16;
        }else if(byte2 == 0x82 && 0x80 <= byte3 && byte3 <= 0x93 ){
            code2 = (unsigned int)byte3 - 0x80 + 0xC0;
            font_y = code2 % 16;
            font_x = code2 / 16;
        }else if(byte2 == 0x82 && 0xA1 <= byte3 && byte3 <= 0xBF ){
            code2 = (unsigned int)byte3 - 0xA0 + 0xE0;
            font_y = code2 % 16;
            font_x = code2 / 16;
        }else if(byte2 == 0x83 && 0x80 <= byte3 && byte3 <= 0xB6 ){
            code2 = (unsigned int)byte3 - 0x80 + 0x100;
            font_y = code2 % 16;
            font_x = code2 / 16;
        }else{
        }
    }
    
    
    gfxDrawSprite(GFX_TOP, GFX_LEFT, (u8*)font_ascii_bin, FONT_ASCII_IMAGE_HEIGHT, FONT_WIDTH*font_y, (FONT_Y_MAX-font_x)*FONT_HEIGHT, 8, 8, x*FONT_WIDTH,  (SCREEN_HEIGHT-y)*FONT_HEIGHT);
    // Flush and swap framebuffers
    gfxFlushBuffers();
    gfxSwapBuffers();
    gspWaitForVBlank();

    gfxDrawSprite(GFX_TOP, GFX_LEFT, (u8*)font_ascii_bin, FONT_ASCII_IMAGE_HEIGHT, FONT_WIDTH*font_y, (FONT_Y_MAX-font_x)*FONT_HEIGHT, 8, 8, x*FONT_WIDTH,  (SCREEN_HEIGHT-y)*FONT_HEIGHT);
    // Flush and swap framebuffers
    gfxFlushBuffers();
    gfxSwapBuffers();
    gspWaitForVBlank();

}

void key_size(int key_row, int key_index, struct xy *size,int *x){
    size->px = KEY_NORMAL_WIDTH;
    size->py = KEY_NORMAL_HEIGHT;
    if (key_row == 0) {
        *x = *x + 11;
        size->px = KEY_FUNCTION_WIDTH;
        size->py = KEY_FUNCTION_HEIGHT;
    }else if(key_row == 2){
        if (key_index == 0)      { size->px = KEY_TAB_WIDTH; }
        else if(key_index == 13) { size->px = KEY_DELETE_WIDTH; }
    }else if(key_row == 3){
        if (key_index == 0)      { size->px = KEY_CAPS_WIDTH; }
        else if(key_index == 12) { size->px = KEY_RETURN_WIDTH; }
    }else if(key_row == 4){
        if (key_index == 0 || key_index == 11) { size->px = KEY_SHIFT_WIDTH; }
    }else if(key_row == 5){
        *x = *x + 9;
        size->py = KEY_SPACE_HEIGHT;
        if (key_index == 3) {     size->px = KEY_SPACE_WIDTH;
        }else if(key_index == 4){ size->px = KEY_COMMAND_WIDTH; }
    }
}

void str16(const unsigned char *u16p,char *str){
    while (*u16p != 0) {
        char up = (*u16p   & 0xf0) >> 4;
        char dn = (*u16p++ & 0x0f);
        if (up >= 10) {
            up = 'A'+ up -10;
        }else{
            up = '0'+ up;
        }
        *str++ = up;
        if (dn >= 10) {
            dn = 'A'+ dn -10;
        }else{
            dn = '0'+ dn;
        }
        *str++ = dn;
    }
    *str = '\0';
}

unsigned char check_keybord(touchPosition touch, int *keyboard_image_no, int *last_key_shift, int *last_key_row, int *last_key_index, const unsigned char **u16p){
    unsigned char key = 0;
    //    *u16p = 0;
    int key_row = -1;
    int key_index = -1;
    int j;
    int size = sizeof(keyboards_rows)/sizeof(int);
    for (j=0; j<size; j++) {
        if(keyboard_line_y[j].y_start <= touch.py && touch.py <= keyboard_line_y[j].y_end) {
            key_row = j;
            int i;
            for(i=0;i< keyboards_rows[j];i++){
                if(keyboard_line_x[j][i].x_start <= touch.px && touch.px <= keyboard_line_x[j][i].x_end){
                    key_index = i;
                }
            }
        }
    }

    if(*last_key_row == -1 && *last_key_index == -1 && key_row > -1 && key_index > -1){
        //        int now_key_code = 0;
        
        int last_keyboard_image_no = *keyboard_image_no;
        if       (key_row == 3 &&  key_index == 0){
            switch (*last_key_shift){
                case NOT_PRESSED:
                case SHIFT_PRESSED:
                    *keyboard_image_no = CAPS_PRESSED;          break;
                case CAPS_PRESSED:
                    *keyboard_image_no = NOT_PRESSED;           break;
                case KANA_PRESSED:
                case KANA_SHIFT_PRESSED:
                    *keyboard_image_no = KANA_CAPS_PRESSED;     break;
                case KANA_CAPS_PRESSED:
                    *keyboard_image_no = KANA_PRESSED;          break;
                case KATAKANA_PRESSED:
                case KATAKANA_SHIFT_PRESSED:
                    *keyboard_image_no = KATAKANA_CAPS_PRESSED; break;
                case KATAKANA_CAPS_PRESSED:
                    *keyboard_image_no = KATAKANA_PRESSED;      break;
            }
        }else if (key_row == 4 && (key_index == 0 || key_index == 11)){
            switch(*last_key_shift){
                case NOT_PRESSED:
                case CAPS_PRESSED:
                    *keyboard_image_no = SHIFT_PRESSED;         break;
                case SHIFT_PRESSED:
                    *keyboard_image_no = NOT_PRESSED;           break;
                case KANA_PRESSED:
                    *keyboard_image_no = KANA_SHIFT_PRESSED;    break;
                case KANA_SHIFT_PRESSED:
                case KANA_CAPS_PRESSED:
                    *keyboard_image_no = KANA_PRESSED;          break;
                case KATAKANA_PRESSED:
                    *keyboard_image_no = KATAKANA_SHIFT_PRESSED;break;
                case KATAKANA_SHIFT_PRESSED:
                case KATAKANA_CAPS_PRESSED:
                    *keyboard_image_no = KATAKANA_PRESSED;      break;
            }
        }else if (key_row == 5 && key_index == 0){
            switch(*last_key_shift){
                case NOT_PRESSED:
                case SHIFT_PRESSED:
                case CAPS_PRESSED:
                    break;
                case KANA_PRESSED:
                case KANA_SHIFT_PRESSED:
                case KANA_CAPS_PRESSED:
                case KATAKANA_PRESSED:
                case KATAKANA_SHIFT_PRESSED:
                case KATAKANA_CAPS_PRESSED:
                    *keyboard_image_no = NOT_PRESSED;       break;
            }
        }else if (key_row == 5 && key_index == 1){
            switch(*last_key_shift){
                case KANA_PRESSED:
                case KANA_SHIFT_PRESSED:
                case KANA_CAPS_PRESSED:
                    break;
                case NOT_PRESSED:
                case SHIFT_PRESSED:
                case CAPS_PRESSED:
                case KATAKANA_PRESSED:
                case KATAKANA_SHIFT_PRESSED:
                case KATAKANA_CAPS_PRESSED:
                    *keyboard_image_no = KANA_PRESSED;      break;
            }
        }else if (key_row == 5 && key_index == 2){
            switch(*last_key_shift){
                case KATAKANA_PRESSED:
                case KATAKANA_SHIFT_PRESSED:
                case KATAKANA_CAPS_PRESSED:
                    break;
                case NOT_PRESSED:
                case SHIFT_PRESSED:
                case CAPS_PRESSED:
                case KANA_PRESSED:
                case KANA_SHIFT_PRESSED:
                case KANA_CAPS_PRESSED:
                    *keyboard_image_no = KATAKANA_PRESSED;  break;
            }
        }
        
        int x = 202 - keyboard_line_y[key_row].y_start;
        int y = keyboard_line_x[key_row][key_index].x_start;
        struct xy key_wh = {-1,-1};
        key_size(key_row, key_index, &key_wh, &x);
        if(last_keyboard_image_no != *keyboard_image_no) {
            gfxDrawSprite(GFX_BOTTOM, GFX_LEFT, (u8*)keyboard_image[*keyboard_image_no], KEY_IMAGE_HEIGHT, 0,0, 240, 320, 0, 0);
        }
        gfxDrawSprite(GFX_BOTTOM, GFX_LEFT, (u8*)keyboard_image[*keyboard_image_no], KEY_IMAGE_HEIGHT, y-2, x, key_wh.py+2, key_wh.px+2, y,  x-2);
        gfxFlushBuffers();
        gfxSwapBuffers();
        gspWaitForVBlank();
        if(last_keyboard_image_no != *keyboard_image_no) {
            gfxDrawSprite(GFX_BOTTOM, GFX_LEFT, (u8*)keyboard_image[*keyboard_image_no], KEY_IMAGE_HEIGHT, 0,0, 240, 320, 0, 0);
        }
        gfxDrawSprite(GFX_BOTTOM, GFX_LEFT, (u8*)keyboard_image[*keyboard_image_no], KEY_IMAGE_HEIGHT, y-2, x, key_wh.py+2, key_wh.px+2, y,  x-2);
        if       (key_row == 3 &&  key_index == 0){
            switch(*last_key_shift){
                case NOT_PRESSED:
                case SHIFT_PRESSED:
                    *last_key_shift = CAPS_PRESSED;         break;
                case CAPS_PRESSED:
                    *last_key_shift = NOT_PRESSED;          break;
                case KANA_PRESSED:
                case KANA_SHIFT_PRESSED:
                    *last_key_shift = KANA_CAPS_PRESSED;    break;
                case KANA_CAPS_PRESSED:
                    *last_key_shift = KANA_PRESSED;         break;
                case KATAKANA_PRESSED:
                case KATAKANA_SHIFT_PRESSED:
                    *last_key_shift = KATAKANA_CAPS_PRESSED;break;
                case KATAKANA_CAPS_PRESSED:
                    *last_key_shift = KATAKANA_PRESSED;     break;
            }
        }else if (key_row == 4 && (key_index == 0 || key_index == 11)){
            switch(*last_key_shift){
                case NOT_PRESSED:
                case CAPS_PRESSED:
                    *last_key_shift = SHIFT_PRESSED;        break;
                case SHIFT_PRESSED:
                    *last_key_shift = NOT_PRESSED;          break;
                case KANA_PRESSED:
                    *last_key_shift = KANA_SHIFT_PRESSED;   break;
                case KANA_SHIFT_PRESSED:
                case KANA_CAPS_PRESSED:
                    *last_key_shift = KANA_PRESSED;         break;
                case KATAKANA_PRESSED:
                    *last_key_shift = KATAKANA_SHIFT_PRESSED;   break;
                case KATAKANA_SHIFT_PRESSED:
                case KATAKANA_CAPS_PRESSED:
                    *last_key_shift = KATAKANA_PRESSED;         break;
            }
        }else if (key_row == 5 && key_index == 0){
            switch(*last_key_shift){
                case NOT_PRESSED:
                case SHIFT_PRESSED:
                case CAPS_PRESSED:
                    break;
                case KANA_PRESSED:
                case KANA_SHIFT_PRESSED:
                case KANA_CAPS_PRESSED:
                case KATAKANA_PRESSED:
                case KATAKANA_SHIFT_PRESSED:
                case KATAKANA_CAPS_PRESSED:
                    *last_key_shift = NOT_PRESSED;          break;
            }
        }else if (key_row == 5 && key_index == 1){
            switch(*last_key_shift){
                case KANA_PRESSED:
                case KANA_SHIFT_PRESSED:
                case KANA_CAPS_PRESSED:
                    break;
                case NOT_PRESSED:
                case SHIFT_PRESSED:
                case CAPS_PRESSED:
                case KATAKANA_PRESSED:
                case KATAKANA_SHIFT_PRESSED:
                case KATAKANA_CAPS_PRESSED:
                    *last_key_shift = KANA_PRESSED;         break;
            }
        }else if (key_row == 5 && key_index == 2){
            switch(*last_key_shift){
                case KATAKANA_PRESSED:
                case KATAKANA_SHIFT_PRESSED:
                case KATAKANA_CAPS_PRESSED:
                    break;
                case NOT_PRESSED:
                case SHIFT_PRESSED:
                case CAPS_PRESSED:
                case KANA_PRESSED:
                case KANA_SHIFT_PRESSED:
                case KANA_CAPS_PRESSED:
                    *last_key_shift = KATAKANA_PRESSED;         break;
            }
        }
        *last_key_row = key_row;
        *last_key_index = key_index;
    }else{
        if(key_row == -1 && key_index == -1 && *last_key_row != -1 && *last_key_index != -1){
            int last_keyboard_image_no = *keyboard_image_no;
            if      (*last_key_shift == SHIFT_PRESSED ){
                if      (*last_key_row == 4 && (*last_key_index==0 || *last_key_index==11)){
                }else if(*last_key_row == 3 &&  *last_key_index==0 ){
                }else if(*last_key_row == 5 &&  *last_key_index==0 ){
                }else if(*last_key_row == 5 &&  *last_key_index==1 ){
                }else if(*last_key_row == 5 &&  *last_key_index==2 ){
                }else{
                    *keyboard_image_no = NOT_PRESSED;
                }
            }else if(*last_key_shift == KANA_SHIFT_PRESSED     ){
                if      (*last_key_row == 4 && (*last_key_index==0 || *last_key_index==11)){
                }else if(*last_key_row == 3 &&  *last_key_index==0 ){
                }else if(*last_key_row == 5 &&  *last_key_index==0 ){
                }else if(*last_key_row == 5 &&  *last_key_index==1 ){
                }else if(*last_key_row == 5 &&  *last_key_index==2 ){
                }else{
                    *keyboard_image_no = KANA_PRESSED;
                }
            }else if(*last_key_shift == KATAKANA_SHIFT_PRESSED     ){
                if      (*last_key_row == 4 && (*last_key_index==0 || *last_key_index==11)){
                }else if(*last_key_row == 3 &&  *last_key_index==0 ){
                }else if(*last_key_row == 5 &&  *last_key_index==0 ){
                }else if(*last_key_row == 5 &&  *last_key_index==1 ){
                }else if(*last_key_row == 5 &&  *last_key_index==2 ){
                }else{
                    *keyboard_image_no = KATAKANA_PRESSED;
                }
            }
            int x = 200 - keyboard_line_y[*last_key_row].y_start;
            int y = keyboard_line_x[*last_key_row][*last_key_index].x_start;
            struct xy key_wh = {-1,-1};
            key_size(*last_key_row, *last_key_index, &key_wh, &x);
            if(last_keyboard_image_no != *keyboard_image_no) {
                gfxDrawSprite(GFX_BOTTOM, GFX_LEFT, (u8*)keyboard_image[*keyboard_image_no], KEY_IMAGE_HEIGHT, 0,0, 240, 320, 0, 0);
            }
            gfxDrawSprite(GFX_BOTTOM, GFX_LEFT, (u8*)keyboard_image[*keyboard_image_no], KEY_IMAGE_HEIGHT, y, x, key_wh.py+3, key_wh.px+2, y,  x);
            gfxFlushBuffers();
            gfxSwapBuffers();
            gspWaitForVBlank();
            if(last_keyboard_image_no != *keyboard_image_no) {
                gfxDrawSprite(GFX_BOTTOM, GFX_LEFT, (u8*)keyboard_image[*keyboard_image_no], KEY_IMAGE_HEIGHT, 0,0, 240, 320, 0, 0);
            }
            gfxDrawSprite(GFX_BOTTOM, GFX_LEFT, (u8*)keyboard_image[*keyboard_image_no], KEY_IMAGE_HEIGHT, y, x, key_wh.py+3, key_wh.px+2, y,  x);
            if      (*last_key_shift == SHIFT_PRESSED){
                if      (*last_key_row == 4 && (*last_key_index==0 || *last_key_index==11)){
                }else if(*last_key_row == 3 &&  *last_key_index==0 ){
                }else if(*last_key_row == 5 &&  *last_key_index==0 ){
                }else if(*last_key_row == 5 &&  *last_key_index==1 ){
                }else if(*last_key_row == 5 &&  *last_key_index==2 ){
                }else{
                    *last_key_shift = NOT_PRESSED;
                }
            }else if(*last_key_shift == KANA_SHIFT_PRESSED       ){
                if      (*last_key_row == 4 && (*last_key_index==0 || *last_key_index==11)){
                }else if(*last_key_row == 3 &&  *last_key_index==0 ){
                }else if(*last_key_row == 5 &&  *last_key_index==0 ){
                }else if(*last_key_row == 5 &&  *last_key_index==1 ){
                }else if(*last_key_row == 5 &&  *last_key_index==2 ){
                }else{
                    *last_key_shift = KANA_PRESSED;
                }
            }else if(*last_key_shift == KATAKANA_SHIFT_PRESSED   ){
                if      (*last_key_row == 4 && (*last_key_index==0 || *last_key_index==11)){
                }else if(*last_key_row == 3 &&  *last_key_index==0 ){
                }else if(*last_key_row == 5 &&  *last_key_index==0 ){
                }else if(*last_key_row == 5 &&  *last_key_index==1 ){
                }else if(*last_key_row == 5 &&  *last_key_index==2 ){
                }else{
                    *last_key_shift = KATAKANA_PRESSED;
                }
            }
            *last_key_row   = -1;
            *last_key_index = -1;
        }
    }
    if (key_row != -1 && key_index != -1) {
        switch (*last_key_shift) {
            case NOT_PRESSED:
                key = keyboard_code_01[key_row][key_index];
                break;
            case SHIFT_PRESSED:
                key = keyboard_code_02[key_row][key_index];
                break;
            case CAPS_PRESSED:
                key = keyboard_code_03[key_row][key_index];
                break;
            case KANA_PRESSED:
                *u16p = keyboard_code_05[key_row][key_index];
                key = **u16p;
                break;
            case KANA_SHIFT_PRESSED:
            case KANA_CAPS_PRESSED:
                *u16p = keyboard_code_06[key_row][key_index];
                key = **u16p;
                break;
            case KATAKANA_PRESSED:
                *u16p = keyboard_code_09[key_row][key_index];
                key = **u16p;
                break;
            case KATAKANA_SHIFT_PRESSED:
            case KATAKANA_CAPS_PRESSED:
                *u16p = keyboard_code_10[key_row][key_index];
                key = **u16p;
                break;
            default:
                break;
        }
    }
    return key;
}



