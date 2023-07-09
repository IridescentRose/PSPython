#include <string>
#include <fstream>
#include <sstream>
#include <pocketpy.h>
#include "platform.hpp"
#include <pspdisplay.h>
#include <pspdebug.h>
#include <pspkernel.h>

auto read_file_to_string(const std::string& path) -> std::string {
    std::fstream file_in(path);

    if(!file_in.is_open()) {
        throw std::runtime_error("Failed to open file: " + path);
    }

    std::stringstream buffer;
    buffer << file_in.rdbuf();

    if(buffer.str().empty())
        throw std::runtime_error("File is empty: " + path);

    return buffer.str();
}

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

auto make_color(u8 r, u8 g, u8 b) -> u32 {
    return 0xFF000000 | (r << 16) | (g << 8) | b;
}

auto psp_text_color(int color) {
    pspDebugScreenSetTextColor(color);
}

struct ModuleBuilder {
    pkpy::VM* vm;
    pkpy::PyObject* mod;

    ModuleBuilder(pkpy::VM* vm, pkpy::PyObject* mod) : vm(vm), mod(mod) {}

    template <typename T>
    auto var(const std::string& name, T value) {
        using namespace pkpy;
        mod->attr().set(name.c_str(), VAR(value));
    }

    auto func(const std::string& layout, pkpy::NativeFuncC func) {
        using namespace pkpy;
        vm->bind(mod, layout.c_str(), func);
    }
};

auto setup_vm_psp_module(pkpy::VM* vm, pkpy::PyObject* mod) {
    using namespace pkpy;

    ModuleBuilder builder(vm, mod);

    builder.var("TEXT_COLOR_GRAY", make_color(0x80, 0x80, 0x80));
    builder.func("make_color(r: int, g: int, b: int) -> int", [](VM* vm, ArgsView args) {
        return VAR(make_color(CAST(u8, args[0]), CAST(u8, args[1]), CAST(u8, args[2])));
    });

    builder.func("text_color(color: int) -> void", [](VM* vm, ArgsView args) {
        psp_text_color(CAST(u32, args[0]));
        return vm->None;
    });
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