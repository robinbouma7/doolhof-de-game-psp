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
        GFX::drawRect(muur_top, muur_left, muurheight[i], muurwidth[i], 0xFFFFFFFF);
    }
    else {
        //pspDebugScreenPrintf("wel collision\n");
        colision = true;
    }
    
    }
    return 0;
     
}

int finishcheck() {
 //hier komt finish check, word collision maar dan met de finish en hij gaat niet dood bij collision
}

int resetplayer() {
 //reset de player, nog niet getest.
 x = 0; 
 y = 124;
 colision = false;
 input= false; 
 
 GFX::clear(0xFF000000);
 drawwalls();
 GFX::drawRect(x, y, 25, 25, 0xFF00FFFF);
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

    // controls, werkt nog niet
    sceCtrlSetSamplingCycle(0);
    sceCtrlSetSamplingCycle(PSP_CTRL_MODE_ANALOG);

    SceCtrlData ctrldata;

    //movement hieronder (dont touch)
    

    while(true) {
        

        sceCtrlReadBufferPositive(&ctrldata, 1);
        //GFX::swapBuffers();
        //sceDisplayWaitVblankStart();

        if (colision) {
            pspDebugScreenPrintf("je ben dood, druk op X om opnieuw te beginnen.\n");
            if (ctrldata.Buttons & PSP_CTRL_CROSS && y > 0) {
              resetplayer();
            }
         else {
          
        }

        else {

            if (ctrldata.Buttons & PSP_CTRL_UP && y > 0) {
                //pspDebugScreenPrintf("up is pressed \n");
             if (!input){
                y = y - 3;
                GFX::clear(0xFF000000);
                drawwalls();
                GFX::drawRect(x, y, 25, 25, 0xFF00FFFF);

                GFX::swapBuffers();
                sceDisplayWaitVblankStart(); 
                collision();
              input = true;
             }
             else {
              
             {
            }

            else if (ctrldata.Buttons & PSP_CTRL_DOWN && y < 245) {
                //pspDebugScreenPrintf("down is pressed \n");
             if (!input){
                y = y + 3;
                GFX::clear(0xFF000000);
                drawwalls();
                GFX::drawRect(x, y, 25, 25, 0xFF00FFFF);

                GFX::swapBuffers();
                sceDisplayWaitVblankStart();
                collision();
               input = true;
             }
             else {
              
             {
            }

            else if (ctrldata.Buttons & PSP_CTRL_RIGHT && x < 455) {
                //pspDebugScreenPrintf("right is pressed \n");
             if (!input){
                x = x + 3;
                GFX::clear(0xFF000000);
                drawwalls();
                GFX::drawRect(x, y, 25, 25, 0xFF00FFFF);

                GFX::swapBuffers();
                sceDisplayWaitVblankStart(); 
                collision();    
               input = true;
             }
             else {
              
             {
            }

            else if (ctrldata.Buttons & PSP_CTRL_LEFT && x > 0) {
                //pspDebugScreenPrintf("left is pressed \n");
             if (!input){
                x = x - 3;

                GFX::clear(0xFF000000);
                drawwalls(); 
                GFX::drawRect(x, y, 25, 25, 0xFF00FFFF);

                GFX::swapBuffers();
                sceDisplayWaitVblankStart();  
                collision(); 
               input = true;
             }
             else {
              
             {
            }

            else {
             input = false;
                GFX::clear(0xFF000000);
                drawwalls();
                GFX::drawRect(x, y, 25, 25, 0xFF00FFFF);

                GFX::swapBuffers();
                sceDisplayWaitVblankStart();
                collision();
            }
        }
        

            
    }
    
}
