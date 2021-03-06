#include <pspkernel.h>
#include <pspdebug.h>
#include <pspdisplay.h>
#include <pspctrl.h>
#include "glib2d.h"
#include <stdlib.h>


PSP_MODULE_INFO("doolhofdegame", 0, 1, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);


//global variables
float x = 12.5, y = 136;
bool inputleft = false,
inputright = false,
inputup = false,
inputdown = false,
input = false; 
bool dead = false;
bool finished = false;
bool wallc = true, finishc = true;
bool collected = false;
int score = 10000;
int coinrng = rand() % 3;
int coinw = 25;
int coinh = 25;
bool debug = false;



//stops the game.
//this is just code that works, so dont change this. (upcoming 3 functions)
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
    score = 10000;
    collected = false;
    coinrng = rand() % 3;
    return 0;
}

//death screen
int die() {

    
    
    SceCtrlData ctrldata;
    pspDebugScreenInit();
    pspDebugScreenSetTextColor(0xFF0000FF);
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
    pspDebugScreenSetTextColor(0xFFFF0000);
    pspDebugScreenPrintf("je bent gefinished! druk op x om opnieuw te starten.\nje score is: %i\n", score);
    pspDebugScreenSetTextColor(0xFF0000FF);
    if (!wallc) {
        pspDebugScreenPrintf("\nmuur collision stond uit dus geen valide run.");
    }
    else {
        
    }   

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
int coincollect() {
    if (!collected) {
    collected = true;
    score+=1000;
    }
    else {
        
    }
    return 0;
}


//wall variables
int muurtop[10] = {47, 151, 0, 157, 0, 266, 6, 161, 195, 80};
int muurleft[10] = {45, 0, 0, 0, 6, 6, 474, 474, 43, 90};
int muurwidth[10] = {6, 43, 6, 6, 479, 479, 6, 6, 6, 6};
int muurheight[10] = {68, 6, 116, 115, 6, 6, 105, 161, 30, 100};

int drawwalls() {
    //draw the walls    
    for (int j = 0; j < 10; j++) {
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
int cointop[3] = {100, 200, 200};
int coinleft[3] = {200, 100, 200};

int drawcoin() {
    //coin sprite
    g2dTexture* coin = g2dTexLoad("coin.png",G2D_SWIZZLE);

    
    g2dBeginRects(coin);
    if (coin == NULL) {
        g2dSetColor(YELLOW);
    }
    g2dSetCoordMode(G2D_UP_LEFT);
    g2dSetAlpha(255);
    g2dSetScaleWH(coinw,coinh);
    g2dSetCoordXY(coinleft[coinrng],cointop[coinrng]);
    g2dSetRotation(0);
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
        //wall collision is off, so dont check for collision
    }
    return 0;
     
}

int coincollision() {
    float playertop = y - 12.4;
    float playerbottom = y + 12.4;
    float playerleft = x - 12.4;
    float playerright = x + 12.4;
    
    int coinbottom = cointop[coinrng] + coinh;
    int coinright = coinleft[coinrng] + coinw;


    if (playertop > coinbottom || playerright < coinleft[coinrng] || playerbottom < cointop[coinrng] || playerleft > coinright) {
        //not touching coin, so do nothing.
    }
    else {       
        coincollect();
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
        //finish collision is off, so dont check for collision
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
                    if (!inputup && y > 12.5){
                        y-=15;
                        score-=5;
                        inputup = true;
                    }
                    else {
   
                    }  
                    
                }
                else {
                    inputup = false;
                }

                if (ctrldata.Buttons & PSP_CTRL_DOWN) {
                    //pspDebugScreenPrintf("down is pressed \n");
                    if (!inputdown && y < 259.5){
                        y+=15;
                        score-=5;
                        inputdown = true;
                    }
                    else {

                    } 
                    
                }
                else {
                    inputdown = false;
                }

                if (ctrldata.Buttons & PSP_CTRL_RIGHT) {
                    //pspDebugScreenPrintf("right is pressed \n");
                    if (!inputright && x < 467.5) {
                        x+=15;
                        score-=5;
                        inputright = true;
                     }
                     else {

                     }

                }
                else {
                    inputright = false;
                }

                if (ctrldata.Buttons & PSP_CTRL_LEFT) {
                    //pspDebugScreenPrintf("left is pressed \n");
                    if (!inputleft && x > 12.5){
                        x-=15;
                        score-=5;
                        inputleft = true;
                    }

                    else {

                    }
                     
                    
                }
                else {
                    inputleft = false;
                }
                /*if (ctrldata.Buttons & PSP_CTRL_RTRIGGER && ctrldata.Buttons & PSP_CTRL_SELECT) {
                    //muur collision aan en uit zetten
                    if (!input && wallc) {
                        wallc = false;
                        input = true;
                    }
                    else if (!input && !wallc) {
                        wallc = true;
                        input = true;
                    } 
                    else {
            
                    }

                }
                else if (ctrldata.Buttons & PSP_CTRL_LTRIGGER && ctrldata.Buttons & PSP_CTRL_SELECT) {
                    //finish collision aan en uit zetten
                    if (!input && finishc) {
                        finishc = false;
                        input = true;
                        
                    }
                    else if (!input && !finishc) {
                        finishc = true;
                        input = true;
                        
                    } 
                    else {
            
                    }

                }


                
                else {
                    //als je niks indruk word hier input false. zodat je de knoppen moet klikken en niet in houden
                    input = false;
                }*/
                
                //draw stuff
                g2dClear(BLACK);

                drawwalls();
                if (!collected) {
                    drawcoin();
                }

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
        
                score-=1;

                collision();
                coincollision();
                finishcheck();
                    
                    
            
    }
    
}
