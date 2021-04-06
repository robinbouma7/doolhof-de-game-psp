#include <pspkernel.h>
#include <pspdebug.h>
#include <pspdisplay.h>
#include <pspctrl.h>
#include "gfx.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <pspjpeg.h>

PSP_MODULE_INFO("doolhofdegame", 0, 1, 0);

//variabelen voor in hele bestand
bool input= false; 
int x = 0, y = 124;
bool colision = false;
bool nomessage = true;
bool finished = false;

//stopt spel 
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
//muur variabelen
int muurtop[2] = {100, 200};
int muurleft[2] = {50, 200};
int muurwidth[2] = {20, 50};
int muurheight[2] = {70, 50};

int drawwalls() {
    //muren worden hier getekend
    GFX::drawRect(muurleft[0], muurtop[0], muurwidth[0], muurheight[0], 0xFFFFFFFF);
    GFX::drawRect(muurleft[1], muurtop[1], muurwidth[1], muurheight[1], 0xFFFFFFFF);

    //finish
    GFX::drawRect(462, 111, 50, 50, 0xFF00FF00);
    return 0;
}

int collision() {
    
    pspDebugScreenInit();
    int playertop = y;
    int playerbottom = y + 25;
    int playerleft = x;
    int playerright = x + 25;
 
    for (int i = 0; i < 2; i++) {

    int muur_top = muurtop[i];
    int muur_bottom = muurtop[i] + muurheight[i];
    int muur_left = muurleft[i];
    int muur_right = muurleft[i] + muurwidth[i];
    
    if (playertop > muur_bottom || playerright < muur_left || playerbottom < muur_top || playerleft > muur_right) {
        
        //pspDebugScreenPrintf("geen collision\n");
        
    }
    else {
        //pspDebugScreenPrintf("wel collision\n");
        
        

        colision = true;
        nomessage = true;
    }
    
    }
    return 0;
     
}

int finishcheck() {
 //hier komt finish check, word collision maar dan met de finish en hij gaat niet dood bij collision
 int playertop = y;
    int playerbottom = y + 25;
    int playerleft = x;
    int playerright = x + 25;
 

    int finishtop = 111;
    int finishbottom = 161;
    int finishleft = 462;
    int finishright = 512;
    
    if (playertop > finishbottom || playerright < finishleft || playerbottom < finishtop || playerleft > finishright) {
        
        
        //raakt finish niet
    }
    else {
        finished = true;
        nomessage = true;
    }
    
    
 return 0;
}

int resetplayer() {
 //reset de player
    x = 0; 
    y = 124;
    colision = false;
    input= false; 
    finished = false;
    
    GFX::clear(0xFF000000);
    drawwalls();
    GFX::drawRect(x, y, 25, 25, 0xFF00FFFF);
    GFX::swapBuffers();
    sceDisplayWaitVblankStart(); 
    return 0;
}


auto main() -> int {
    
    setupcallbacks();
    GFX::init();

    /*while(1) {
        GFX::clear(0xFFFFCA02);
        //GFX::drawRect(10, 10, 30, 30, 0xFF00FFFF);
        GFX::swapBuffers();
        sceDisplayWaitVblankStart();
    }*/
    // ooude dingen met bestanden. word niet meer gebruikt.
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

    
    sceCtrlSetSamplingCycle(0);
    sceCtrlSetSamplingCycle(PSP_CTRL_MODE_ANALOG);

    SceCtrlData ctrldata;

    //movement hieronder (dont touch)
    

    while(true) {
        

        sceCtrlReadBufferPositive(&ctrldata, 1);
        //GFX::swapBuffers();
        //sceDisplayWaitVblankStart();

        //death screen
        if (colision) {
           
            if (nomessage){
            
            GFX::clear(0xFF000000);
            GFX::swapBuffers();
            sceDisplayWaitVblankStart(); 

            pspDebugScreenInit();

            pspDebugScreenPrintf("je ben dood, druk op X om opnieuw te beginnen.\n");
            nomessage = false;
            }
            else {
                //niks, message al laten zien.
            }

            if (ctrldata.Buttons & PSP_CTRL_CROSS) {
                //player resetten
                resetplayer();
            }
            else {
                //niks doen, geen input
            }

          
        }

        //finish gebeurtenissen
        else if (finished) {
           
            if (nomessage){
            
            GFX::clear(0xFF000000);
            GFX::swapBuffers();
            sceDisplayWaitVblankStart(); 

            pspDebugScreenInit();

            pspDebugScreenPrintf("je bent gefinished! score coming soon. druk op x om opnieuw te starten.\n");
            nomessage = false;
            }
            else {
                //niks, message al laten zien.
            }

            if (ctrldata.Buttons & PSP_CTRL_CROSS) {
                //player resetten
                resetplayer(); 
            }
            else {
                //niks doen, geen input
            }

          
        }

        else {

            if (ctrldata.Buttons & PSP_CTRL_UP && y > 0) {
                //pspDebugScreenPrintf("up is pressed \n");
             if (!input){
                y = y - 15;
                GFX::clear(0xFF000000);
                drawwalls();
                GFX::drawRect(x, y, 25, 25, 0xFF00FFFF);

                GFX::swapBuffers();
                sceDisplayWaitVblankStart(); 
                collision();
                finishcheck();
                input = true;
             }
             else {
                 GFX::clear(0xFF000000);
                drawwalls();
                GFX::drawRect(x, y, 25, 25, 0xFF00FFFF);

                GFX::swapBuffers();
                sceDisplayWaitVblankStart();
             }
            }

            else if (ctrldata.Buttons & PSP_CTRL_DOWN && y < 245) {
                //pspDebugScreenPrintf("down is pressed \n");
             if (!input){
                y = y + 15;
                GFX::clear(0xFF000000);
                drawwalls();
                GFX::drawRect(x, y, 25, 25, 0xFF00FFFF);

                GFX::swapBuffers();
                sceDisplayWaitVblankStart();
                collision();
                finishcheck();
               input = true;
             }
             else {
                  GFX::clear(0xFF000000);
                drawwalls();
                GFX::drawRect(x, y, 25, 25, 0xFF00FFFF);

                GFX::swapBuffers();
                sceDisplayWaitVblankStart();
             }
            }

            else if (ctrldata.Buttons & PSP_CTRL_RIGHT && x < 455) {
                //pspDebugScreenPrintf("right is pressed \n");
             if (!input){
                x = x + 15;
                GFX::clear(0xFF000000);
                drawwalls();
                GFX::drawRect(x, y, 25, 25, 0xFF00FFFF);

                GFX::swapBuffers();
                sceDisplayWaitVblankStart(); 
                collision();    
                finishcheck();
               input = true;
             }
             else {
                 GFX::clear(0xFF000000);
                drawwalls();
                GFX::drawRect(x, y, 25, 25, 0xFF00FFFF);

                GFX::swapBuffers();
                sceDisplayWaitVblankStart();
             }
            }

            else if (ctrldata.Buttons & PSP_CTRL_LEFT && x > 0) {
                //pspDebugScreenPrintf("left is pressed \n");
             if (!input){
                x = x - 15;

                GFX::clear(0xFF000000);
                drawwalls(); 
                GFX::drawRect(x, y, 25, 25, 0xFF00FFFF);

                GFX::swapBuffers();
                sceDisplayWaitVblankStart();  
                collision();
                finishcheck(); 
               input = true;
             }
             else {
                 GFX::clear(0xFF000000);
                drawwalls();
                GFX::drawRect(x, y, 25, 25, 0xFF00FFFF);

                GFX::swapBuffers();
                sceDisplayWaitVblankStart();
             }
            }

            else {
             input = false;
                GFX::clear(0xFF000000);
                drawwalls();
                GFX::drawRect(x, y, 25, 25, 0xFF00FFFF);

                GFX::swapBuffers();
                sceDisplayWaitVblankStart();
                input = false;
            }
        }
        

            
    }
    
}
