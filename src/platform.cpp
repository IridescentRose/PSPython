#include <pspkernel.h>

PSP_MODULE_INFO("PSPython", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);
PSP_HEAP_SIZE_KB(-1024 * 1024);

int exit_callback(int arg1, int arg2, void *common)
{
    sceKernelExitGame();
    return 0;
}

int callback_thread(SceSize args, void *argp)
{
    int cbid = sceKernelCreateCallback("Exit Callback",
                                       exit_callback, nullptr);
    sceKernelRegisterExitCallback(cbid);
    sceKernelSleepThreadCB();
    return 0;
}

void platform_init() {
    int thid = sceKernelCreateThread("update_thread",
                                     callback_thread, 0x11, 0xFA0, 0, 0);

    if(thid >= 0)
        sceKernelStartThread(thid, 0, nullptr);

    pspDebugScreenInit();
    pspDebugScreenClear();
    pspDebugScreenSetXY(0, 0);
}