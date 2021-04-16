#include <pspkernel.h>
#include <pspdebug.h>
#include <pspdisplay.h>
#include <pspctrl.h>
#include "gfx.hpp"


PSP_MODULE_INFO("doolhofdegame", 0, 1, 0);

//variables for the whole file
bool input = false; 
int x = 0, y = 124;
bool dead = false;
bool nomessage = true;
bool finished = false;


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



//wall variables
int muurtop[7] = {116, 151, 110, 0, 157, 0, 266};
int muurleft[7] = {0, 0, 67, 0, 0, 6, 6};
int muurwidth[7] = {38, 38, 6, 6, 6, 479, 479};
int muurheight[7] = {6, 6, 48, 116, 115, 6, 6};

int drawwalls() {
    //draw the walls
    for (int j = 0; j < 7; j++) {
    GFX::drawRect(muurleft[j], muurtop[j], muurwidth[j], muurheight[j], 0xFFFFFFFF);
    }
    

    //finish
    GFX::drawRect(430, 111, 50, 50, 0xFF00FF00);
    return 0;
}

int collision() {
    

    pspDebugScreenInit();
    int playertop = y;
    int playerbottom = y + 25;
    int playerleft = x;
    int playerright = x + 25;
 
    for (int i = 0; i < 4; i++) {

    int muur_top = muurtop[i];
    int muur_bottom = muurtop[i] + muurheight[i];
    int muur_left = muurleft[i];
    int muur_right = muurleft[i] + muurwidth[i];
    
    if (playertop > muur_bottom || playerright < muur_left || playerbottom < muur_top || playerleft > muur_right) {
        
        //pspDebugScreenPrintf("geen collision\n");
        
    }
    else {
        //pspDebugScreenPrintf("wel collision\n");
        
        

        dead = true;
        nomessage = true;
    }
    
    }
    return 0;
     
}

int finishcheck() {
    //check if touching finish and end game if it does,
    int playertop = y;
    int playerbottom = y + 25;
    int playerleft = x;
    int playerright = x + 25;
 

    int finishtop = 111;
    int finishbottom = 161;
    int finishleft = 430;
    int finishright = 480;
    
    if (playertop > finishbottom || playerright < finishleft || playerbottom < finishtop || playerleft > finishright) {
        
        
        //not touching the finish, so do nothing.
    }
    else {
        finished = true;
        nomessage = true;
    }
    
    
 return 0;
}

int resetplayer() {
    //reset player
    x = 0; 
    y = 124;
    dead = false;
    input = false; 
    finished = false;
    return 0;
}


auto main() -> int {
    
    setupcallbacks();
    GFX::init();

    /* rectangle drawing, left here as example
    while(1) { 
        GFX::clear(0xFFFFCA02);
        //GFX::drawRect(10, 10, 30, 30, 0xFF00FFFF);
        GFX::swapBuffers();
        sceDisplayWaitVblankStart();
    }*/
    
    // old stuff, not used any more
    //bestand 1
    //std::ofstream file("test.txt");
    //file << "1 2 3 hello PSP!" << std::endl;
    //file.close();

    //bestand 2
    //std::ifstream file2("test.txt");
    //std::string str;
    //std::getline(file2, str);
    //file2.close();

    //bestand 3
    //std::ifstream file3("ending.txt");
    //std::string str2;
    //std::getline(file3, str2);
    //file3.close();

    

    //pspDebugScreenInit();
    //pspDebugScreenPrintf(str.c_str());

    //pspDebugScreenPrintf("\n\nricardo coming soon \n");
    //pspDebugScreenPrintf(str2.c_str());

    
    //sceCtrlSetSamplingCycle(0);
    //sceCtrlSetSamplingCycle(PSP_CTRL_MODE_ANALOG);

    
    //movement under here
    SceCtrlData ctrldata;
    while(1) {
            
            sceCtrlReadBufferPositive(&ctrldata, 1);

            if (!dead && !finished) {

                GFX::clear(0xFF000000);
                

                if (ctrldata.Buttons & PSP_CTRL_UP) {
                    //pspDebugScreenPrintf("up is pressed \n");
                    if (!input && y > 0){
                        y = y - 20;
                        input = true;
                    }
                    else {
   
                    }  
                    
                }

                else if (ctrldata.Buttons & PSP_CTRL_DOWN) {
                    //pspDebugScreenPrintf("down is pressed \n");
                    if (!input && y < 247){
                        y = y + 20;
                        input = true;
                    }
                    else {

                    } 
                    
                }

                else if (ctrldata.Buttons & PSP_CTRL_RIGHT) {
                    //pspDebugScreenPrintf("right is pressed \n");
                    if (!input && x < 455) {
                        x = x + 20;
                        input = true;
                     }
                     else {

                     }

                }

                else if (ctrldata.Buttons & PSP_CTRL_LEFT) {
                    //pspDebugScreenPrintf("left is pressed \n");
                    if (!input && x > 0){
                        x = x - 20;
                        input = true;
                    }

                    else {

                    }
                     
                    
                }

                
                else {
               
                    input = false;
                }
                
                
                drawwalls();
                GFX::drawRect(x, y, 25, 25, 0xFF00FFFF);
                GFX::swapBuffers();
                sceDisplayWaitVblankStart(); 
                collision();
                finishcheck();
           
            }
        
        

        else {
            
            

            if (dead && nomessage) {
                //death screen
                pspDebugScreenInit();
                pspDebugScreenPrintf("je ben dood, druk op X om opnieuw te beginnen.\n");
                nomessage = false;
                 
            }
            
            else if (finished && nomessage) {
                //finish screen
                pspDebugScreenInit();
                pspDebugScreenPrintf("je bent gefinished! score coming soon. druk op x om opnieuw te starten.\n");
                nomessage = false;
                
            }
            
            else if (ctrldata.Buttons & PSP_CTRL_CROSS) {
                //reset player
                resetplayer();
            }

            
        }
        
        /*push sprite that is off screen back, not used for performance
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
