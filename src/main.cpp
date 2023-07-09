#include "Util.hpp"
#include "ModuleBuilder.hpp"
#include "platform.hpp"
#include <pspdisplay.h>
#include <pspdebug.h>
#include <pspkernel.h>
#include "psp.hpp"

auto setup_vm_output(pkpy::VM* vm) {
    vm->_stdout = [](pkpy::VM* vm, const pkpy::Str& s) {
        PK_UNUSED(vm);

        pspDebugScreenPrintf("%s", s.c_str());

        std::cout << s << std::endl;
    };
    vm->_stderr = [](pkpy::VM* vm, const pkpy::Str& s) {
        PK_UNUSED(vm);

        pspDebugScreenSetTextColor(0xFF0000FF);
        pspDebugScreenPrintf("%s", s.c_str());

        std::cerr << s << std::endl;
        throw std::runtime_error("Python error");
    };
}

auto main() -> int {
    platform_init();

    try {
        auto vm = new pkpy::VM(true);
        setup_vm_output(vm);
        setup_vm_psp_module(vm, vm->new_module("psp"));

        auto source_code = read_file_to_string("script.py");
        vm->exec(source_code, "script.py", pkpy::EXEC_MODE);
        delete vm;

        pspDebugScreenSetTextColor(0xFF00FF00);
        pspDebugScreenPrintf("Program completed.\n");
        while (true) {
            sceDisplayWaitVblankStart();
        }
    } catch (const std::exception& e) {
        pspDebugScreenSetXY(0, 0);
        pspDebugScreenSetTextColor(0xFF0000FF);
        pspDebugScreenPrintf("Exception: %s\n", e.what());
        std::cerr << "Exception: " << e.what() << std::endl;
        sceKernelDelayThread(5 * 1000 * 1000);
    }

    return EXIT_SUCCESS;
}