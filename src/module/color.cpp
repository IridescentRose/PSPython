#include "color.hpp"
#include <psptypes.h>

auto make_color(u8 r, u8 g, u8 b) -> u32 {
    return 0xFF000000 | (r << 16) | (g << 8) | b;
}

auto setup_vm_color_module(pkpy::VM* vm, pkpy::PyObject* mod) -> void {
    using namespace pkpy;

    // Create the color module
    auto color_mod = vm->new_module("color");
    mod->attr().set("color", color_mod);
    mod = color_mod;

    // Add the color constants
    mod->attr().set("BLACK", VAR(make_color(0x00, 0x00, 0x00)));
    mod->attr().set("BLUE", VAR(make_color(0x00, 0x00, 0xFF)));
    mod->attr().set("GREEN", VAR(make_color(0x00, 0xFF, 0x00)));
    mod->attr().set("CYAN", VAR(make_color(0x00, 0xFF, 0xFF)));
    mod->attr().set("RED", VAR(make_color(0xFF, 0x00, 0x00)));
    mod->attr().set("MAGENTA", VAR(make_color(0xFF, 0x00, 0xFF)));
    mod->attr().set("YELLOW", VAR(make_color(0xFF, 0xFF, 0x00)));
    mod->attr().set("LIGHT_GRAY", VAR(make_color(0xC0, 0xC0, 0xC0)));
    mod->attr().set("GRAY", VAR(make_color(0x80, 0x80, 0x80)));
    mod->attr().set("LIGHT_RED", VAR(make_color(0xFF, 0x80, 0x80)));
    mod->attr().set("LIGHT_GREEN", VAR(make_color(0x80, 0xFF, 0x80)));
    mod->attr().set("LIGHT_BLUE", VAR(make_color(0x80, 0x80, 0xFF)));
    mod->attr().set("LIGHT_YELLOW", VAR(make_color(0xFF, 0xFF, 0x80)));
    mod->attr().set("LIGHT_CYAN", VAR(make_color(0x80, 0xFF, 0xFF)));
    mod->attr().set("LIGHT_MAGENTA", VAR(make_color(0xFF, 0x80, 0xFF)));
    mod->attr().set("WHITE", VAR(make_color(0xFF, 0xFF, 0xFF)));

    // Add the create function
    vm->bind(mod, "create(r: int, g: int, b: int) -> int", [](VM* vm, ArgsView args) {
        return VAR(make_color(CAST(u8, args[0]), CAST(u8, args[1]), CAST(u8, args[2])));
    });
}