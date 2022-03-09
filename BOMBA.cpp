#include "mbed.h"
#include "stm32746g_discovery_lcd.h"
#include "stm32746g_discovery_ts.h"

Timer t1;
int timeout = 120 * 1000;

class Button{
    
    uint16_t _Xpos;
    uint16_t _Ypos;
    uint16_t _Width;
    uint16_t _Height;
    
    void (*clickFunc)(void);
 
    
    public:
    Button(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height, void (*functionToCall)(void)){
        _Xpos = Xpos;
        _Ypos = Ypos;
        _Width = Width;
        _Height = Height;
        clickFunc = functionToCall;
    }
    
    void checkClick(uint16_t x, uint16_t y){
        if(x > _Xpos && x < _Xpos + _Width){
            if(y > _Ypos && y < _Ypos + _Height)
                {
                clickFunc();
                }
            }
    }
    
    void draw(){
        BSP_LCD_FillRect(_Xpos, _Ypos, _Width, _Height);  
    }
    
};




void resetTimer(){
    t1.reset();
}

void incTimeout(){
    timeout += 500;
}

void decTimeout(){
    timeout -= 500;
}

int main()
{
    TS_StateTypeDef TS_State;
    uint16_t x, y;
    uint8_t text[30];
    uint8_t status;
    uint8_t idx;
    uint8_t cleared = 0;
    uint8_t prev_nb_touches = 0;

    BSP_LCD_Init();
    BSP_LCD_LayerDefaultInit(LTDC_ACTIVE_LAYER, LCD_FB_START_ADDRESS);
    BSP_LCD_SelectLayer(LTDC_ACTIVE_LAYER);

    BSP_LCD_DisplayStringAt(0, LINE(5), (uint8_t *)"TOUCHSCREEN DEMO", CENTER_MODE);
    HAL_Delay(1000);

    status = BSP_TS_Init(BSP_LCD_GetXSize(), BSP_LCD_GetYSize());
    if (status != TS_OK) {
        BSP_LCD_Clear(LCD_COLOR_RED);
        BSP_LCD_SetBackColor(LCD_COLOR_RED);
        BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
        BSP_LCD_DisplayStringAt(0, LINE(5), (uint8_t *)"TOUCHSCREEN INIT FAIL", CENTER_MODE);
    } else {
        BSP_LCD_Clear(LCD_COLOR_GREEN);
        BSP_LCD_SetBackColor(LCD_COLOR_GREEN);
        BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
        BSP_LCD_DisplayStringAt(0, LINE(5), (uint8_t *)"TOUCHSCREEN INIT OK", CENTER_MODE);
    }

    HAL_Delay(1000);
    BSP_LCD_SetFont(&Font12);
    BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    
    Button bt1 = Button(100, 100, 50, 50, &resetTimer);
    
    Button bt2 = Button(300, 100, 50, 50, &incTimeout);
    Button bt3 = Button(300, 200, 50, 50, &decTimeout);

    t1.start();


    while(1) {
        bt1.draw();
        bt2.draw();
        bt3.draw();
        
        int msElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(t1.elapsed_time()).count();
        int disp = timeout - msElapsed;
        
        if(disp < 0){
            BSP_LCD_Clear(LCD_COLOR_BLUE);
            HAL_Delay(100);
            BSP_LCD_Clear(LCD_COLOR_WHITE);
            HAL_Delay(100);
            BSP_LCD_Clear(LCD_COLOR_BLUE);
            HAL_Delay(100);
            BSP_LCD_Clear(LCD_COLOR_WHITE);
            HAL_Delay(100);
            BSP_LCD_Clear(LCD_COLOR_BLUE);
            sprintf((char*)text, "BOOM");
            BSP_LCD_SetFont(&Font24);
            BSP_LCD_DisplayStringAt(0, LINE(2), (uint8_t *)&text, CENTER_MODE);
            BSP_LCD_SetFont(&Font12);
            HAL_Delay(1000);
            t1.reset();
        } else {
        
            sprintf((char*)text, "TimeOut: %d.%3d", timeout/1000, timeout%1000);
            BSP_LCD_DisplayStringAt(0, LINE(0), (uint8_t *)&text, RIGHT_MODE);
            
            sprintf((char*)text, "Timer: %d.%3d", disp/1000, disp%1000);
            BSP_LCD_SetFont(&Font24);
            BSP_LCD_DisplayStringAt(0, LINE(2), (uint8_t *)&text, CENTER_MODE);
            BSP_LCD_SetFont(&Font12);
        }
        
        BSP_TS_GetState(&TS_State);
        if (TS_State.touchDetected) {
            // Clear lines corresponding to old touches coordinates
            if (TS_State.touchDetected < prev_nb_touches) {
                for (idx = (TS_State.touchDetected + 1); idx <= 5; idx++) {
                    BSP_LCD_ClearStringLine(idx);
                }
            }
            prev_nb_touches = TS_State.touchDetected;

            cleared = 0;

            sprintf((char*)text, "Touches: %d", TS_State.touchDetected);
            BSP_LCD_DisplayStringAt(0, LINE(0), (uint8_t *)&text, LEFT_MODE);

            for (idx = 0; idx < TS_State.touchDetected; idx++) {
                x = TS_State.touchX[idx];
                y = TS_State.touchY[idx];
                bt1.checkClick(x,y);
                bt2.checkClick(x,y);
                bt3.checkClick(x,y);
                sprintf((char*)text, "Touch %d: x=%d y=%d    ", idx+1, x, y);
                BSP_LCD_DisplayStringAt(0, LINE(idx+1), (uint8_t *)&text, LEFT_MODE);
            }

        } else {
            if (!cleared) {
                BSP_LCD_Clear(LCD_COLOR_BLUE);
                sprintf((char*)text, "Touches: 0");
                BSP_LCD_DisplayStringAt(0, LINE(0), (uint8_t *)&text, LEFT_MODE);
                cleared = 1;
            }
        }
    }
}
