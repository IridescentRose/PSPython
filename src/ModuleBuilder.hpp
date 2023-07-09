#pragma once
#include <pocketpy.h>

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