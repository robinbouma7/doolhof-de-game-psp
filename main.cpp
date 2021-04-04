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
 
//stopt spel (werkt niet maar dat boeit niks)
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

int drawwalls() {
    //muren worden hier getekend

    GFX::drawRect(100, 60, 25, 30, 0xFFFFFFFF);
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
    int x = 0, y = 0;

    while(true) {
        

        sceCtrlReadBufferPositive(&ctrldata, 1);
        //GFX::swapBuffers();
        //sceDisplayWaitVblankStart();

        if (ctrldata.Buttons & PSP_CTRL_UP && y > 0) {
            //pspDebugScreenPrintf("up is pressed \n");
            y = y - 3;
            GFX::clear(0xFF000000);
            drawwalls();
            GFX::drawRect(x, y, 25, 25, 0xFF00FFFF);

            GFX::swapBuffers();
            sceDisplayWaitVblankStart();            
        }

        else if (ctrldata.Buttons & PSP_CTRL_DOWN && y < 245) {
            //pspDebugScreenPrintf("down is pressed \n");
            y = y + 3;
            GFX::clear(0xFF000000);
            drawwalls();
            GFX::drawRect(x, y, 25, 25, 0xFF00FFFF);

            GFX::swapBuffers();
            sceDisplayWaitVblankStart();
        }

        else if (ctrldata.Buttons & PSP_CTRL_RIGHT && x < 455) {
            //pspDebugScreenPrintf("right is pressed \n");
            x = x + 3;
            GFX::clear(0xFF000000);
            drawwalls();
            GFX::drawRect(x, y, 25, 25, 0xFF00FFFF);

            GFX::swapBuffers();
            sceDisplayWaitVblankStart();       
        }

        else if (ctrldata.Buttons & PSP_CTRL_LEFT && x > 0) {
            //pspDebugScreenPrintf("left is pressed \n");
            x = x - 3;

            GFX::clear(0xFF000000);
            drawwalls(); 
            GFX::drawRect(x, y, 25, 25, 0xFF00FFFF);

            GFX::swapBuffers();
            sceDisplayWaitVblankStart();            
        }

        else {
            GFX::clear(0xFF000000);
            drawwalls();
            GFX::drawRect(x, y, 25, 25, 0xFF00FFFF);

            GFX::swapBuffers();
            sceDisplayWaitVblankStart();
        }

            
    }
    
}