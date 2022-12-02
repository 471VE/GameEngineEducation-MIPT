#pragma once
#include <vector>
#include <string>
#include <memory>
#include "flecs.h"

class CScriptProxy;

struct Scripts {
    template<typename... Args>
    Scripts(Args... args) : names{ args... } {}
    std::vector<std::string> names;
};

void register_ecs_script_systems(flecs::world& ecs);
