#include <pspkernel.h>
#include <pspdebug.h>
#include <pspdisplay.h>
#include <pspctrl.h>
#include "glib2d.h"


PSP_MODULE_INFO("doolhofdegame", 0, 1, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);


//global variables
float x = 12.5, y = 136;

bool input = false; 
bool dead = false;
bool finished = false;
bool wallc = true, finishc = true;


//stops the game 
int exit_callback(int arg1, int arg2, void *common) {
    sceKernelExitGame();
    return 0;
}


int callbackthread(SceSize args, void *argp) {
    int cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
    sceKernelRegisterExitCallback(cbid);

    sceKernelSleepThreadCB();
    return 0;
}


void setupcallbacks () {
    int thid = sceKernelCreateThread("update_thread", callbackthread, 0x11, 0xfa0, 0, NULL);
    if (thid >= 0) {
        sceKernelStartThread(thid, 0, NULL);
    }
}



int resetplayer() {
    //reset player
    x = 12.5; 
    y = 136;
    dead = false; 
    finished = false;
    return 0;
}

//death screen
int die() {

    
    
    SceCtrlData ctrldata;
    pspDebugScreenInit();
    pspDebugScreenSetTextColor(0xFFFFFFFF);
    pspDebugScreenPrintf("je ben dood, druk op X om opnieuw te beginnen.\n");
   
    while (true) {
        sceCtrlReadBufferPositive(&ctrldata, 1);
        input = true;
        if (ctrldata.Buttons & PSP_CTRL_CROSS) {
            //reset player 
            resetplayer();
            return 0;
        }
        
    }
    
}
//end screen
int finish() {
    
    SceCtrlData ctrldata;
    pspDebugScreenInit();
    pspDebugScreenSetTextColor(0xFFFFFFFF);
    pspDebugScreenPrintf("je bent gefinished! druk op x om opnieuw te starten.\nscore coming soonisch probably i hope!");
    
    while (true) {
        sceCtrlReadBufferPositive(&ctrldata, 1);
        input = true;
        if (ctrldata.Buttons & PSP_CTRL_CROSS) {
            //reset player
            
            resetplayer();
            return 0;
        }
    }
}


//wall variables
int muurtop[8] = {47, 151, 0, 157, 0, 266, 6, 161};
int muurleft[8] = {45, 0, 0, 0, 6, 6, 474, 474};
int muurwidth[8] = {6, 43, 6, 6, 479, 479, 6, 6};
int muurheight[8] = {68, 6, 116, 115, 6, 6, 105, 161};

int drawwalls() {
    //draw the walls    
    for (int j = 0; j < 8; j++) {
        g2dBeginRects(NULL); // No texture
        g2dSetColor(WHITE);
        g2dSetScaleWH(muurwidth[j],muurheight[j]);
        g2dSetCoordXY(muurleft[j],muurtop[j]);
        g2dAdd();
        g2dEnd();
    
    }

    //draw the finish
    g2dBeginRects(NULL); // No texture
        g2dSetColor(GREEN);
        g2dSetScaleWH(50,50);
        g2dSetCoordXY(430,111);
        g2dAdd();
        g2dEnd();

    return 0;
}

int collision() {
    
    if (wallc) {
        //0.1 difference so only collision on overlap
        float playertop = y - 12.4;
        float playerbottom = y + 12.4;
        float playerleft = x - 12.4;
        float playerright = x + 12.4;

        for (int i = 0; i < 8; i++) {

            int muur_top = muurtop[i];
            int muur_bottom = muurtop[i] + muurheight[i];
            int muur_left = muurleft[i];
            int muur_right = muurleft[i] + muurwidth[i];

            if (playertop > muur_bottom || playerright < muur_left || playerbottom < muur_top || playerleft > muur_right) {

                //pspDebugScreenPrintf("geen collision\n");

            }
            else {
                //pspDebugScreenPrintf("wel collision\n");

                die();
            }

        }
    }
    else {
        //wall collision is off
    }
    return 0;
     
}

int finishcheck() {
    if (finishc) {
        //check if touching finish and end game if it does, 0.1 difference so only collision on overlap.
        float playertop = y - 12.4;
        float playerbottom = y + 12.4;
        float playerleft = x - 12.4;
        float playerright = x + 12.4;


        if (playertop > 161 || playerright < 430 || playerbottom < 111 || playerleft > 480) {
            //not touching the finish, so do nothing.
        }
        else {       
            finish();
        }     
    }
    else {
        //finish collision is off
    }
    return 0;
}





auto main() -> int {
    
    setupcallbacks();

    //player sprite
    g2dTexture* ric = g2dTexLoad("ricardo.png",G2D_SWIZZLE);
    int w = (ric == NULL ? 10 : ric->w),
    h = (ric == NULL ? 10 : ric->h);

    //movement under here
    SceCtrlData ctrldata;
    while(1) {
            
            sceCtrlReadBufferPositive(&ctrldata, 1);

                 

                if (ctrldata.Buttons & PSP_CTRL_UP) {
                    //pspDebugScreenPrintf("up is pressed \n");
                    if (!input && y > 12.5){
                        y-=15;
                        input = true;
                    }
                    else {
   
                    }  
                    
                }

                else if (ctrldata.Buttons & PSP_CTRL_DOWN) {
                    //pspDebugScreenPrintf("down is pressed \n");
                    if (!input && y < 259.5){
                        y+=15;
                        input = true;
                    }
                    else {

                    } 
                    
                }

                else if (ctrldata.Buttons & PSP_CTRL_RIGHT) {
                    //pspDebugScreenPrintf("right is pressed \n");
                    if (!input && x < 467.5) {
                        x+=15;
                        input = true;
                     }
                     else {

                     }

                }

                else if (ctrldata.Buttons & PSP_CTRL_LEFT) {
                    //pspDebugScreenPrintf("left is pressed \n");
                    if (!input && x > 12.5){
                        x-=15;
                        input = true;
                    }

                    else {

                    }
                     
                    
                }
                else if (ctrldata.Buttons & PSP_CTRL_RTRIGGER && ctrldata.Buttons & PSP_CTRL_SELECT) {
                    pspDebugScreenInit();
                    pspDebugScreenSetTextColor(0xFFFF0000);
                    if (!input && wallc) {
                        wallc = false;
                        input = true;
                        pspDebugScreenClear();
                        pspDebugScreenPrintf("muur collision uitgeschakeld");

                    }
                    else if (!input && !wallc) {
                        wallc = true;
                        input = true;
                        pspDebugScreenClear();
                        pspDebugScreenPrintf("muur collision ingeschakeld");
                    } 
                    else {
            
                    }

                }
                else if (ctrldata.Buttons & PSP_CTRL_LTRIGGER && ctrldata.Buttons & PSP_CTRL_SELECT) {
                    pspDebugScreenInit();
                    pspDebugScreenSetTextColor(0xFFFF0000);
                    if (!input && finishc) {
                        finishc = false;
                        input = true;
                        pspDebugScreenClear();
                        pspDebugScreenPrintf("finish collision uitgeschakeld");
                    }
                    else if (!input && !finishc) {
                        finishc = true;
                        input = true;
                        pspDebugScreenClear();
                        pspDebugScreenPrintf("finish collision ingeschakeld");
                    } 
                    else {
            
                    }

                }


                
                else {
               
                    input = false;
                }
                
                //draw stuff
                g2dClear(BLACK);

                drawwalls();

                g2dBeginRects(ric);
                if (ric == NULL) {
                    g2dSetColor(RED);
                }
                g2dSetCoordMode(G2D_CENTER);
                g2dSetAlpha(255);
                g2dSetScaleWH(w,h);
                g2dSetCoordXY(x,y);
                g2dSetRotation(0);
                g2dAdd();
                g2dEnd();
  
                g2dFlip(G2D_VSYNC);
        

                collision();
                finishcheck();
           
        
    
        /*push sprite that is off screen back, not used for performance
        //might bring back but needs to be rewritten for that. cause it uses the old drawing system.
        if (x > 455) {
            x = 455;
            GFX::clear(0xFF000000);
            drawwalls();
            GFX::drawRect(x, y, 25, 25, 0xFF00FFFF);

            GFX::swapBuffers();
            sceDisplayWaitVblankStart(); 
        }
        if (y > 247) {
            y = 247;
            GFX::clear(0xFF000000);
            drawwalls();
            GFX::drawRect(x, y, 25, 25, 0xFF00FFFF);

            GFX::swapBuffers();
            sceDisplayWaitVblankStart(); 
        }
        if (x < 0) {
            x = 0;
            GFX::clear(0xFF000000);
            drawwalls();
            GFX::drawRect(x, y, 25, 25, 0xFF00FFFF);

            GFX::swapBuffers();
            sceDisplayWaitVblankStart(); 
        }
        if (y < 0) {
            y = 0;
            GFX::clear(0xFF000000);
            drawwalls();
            GFX::drawRect(x, y, 25, 25, 0xFF00FFFF);
            GFX::swapBuffers();
            sceDisplayWaitVblankStart(); 
        }*/
            
    }
    
}
