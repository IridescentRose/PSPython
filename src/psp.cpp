#include "psp.hpp"
#include "ModuleBuilder.hpp"
#include "module/color.hpp"
#include <pspdebug.h>

auto psp_text_color(int color) {
    pspDebugScreenSetTextColor(color);
}

auto setup_vm_psp_module(pkpy::VM* vm, pkpy::PyObject* mod) -> void {
    using namespace pkpy;

    setup_vm_color_module(vm, mod);
    ModuleBuilder builder(vm, mod);

    builder.func("text_color(color: int) -> void", [](VM* vm, ArgsView args) {
        psp_text_color(CAST(u32, args[0]));
        return vm->None;
    });
}