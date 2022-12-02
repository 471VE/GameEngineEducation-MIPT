#include "ecsControl.h"
#include "ecsMesh.h"
#include "ecsPhys.h"
#include "ecsScript.h"
#include "ecsSystems.h"

#include "../GameEngine/InputHandler.h"

#include "../ScriptSystem/ScriptProxy.h"
#include "../ScriptSystem/ScriptSystem.h"


void initialize_script_state(flecs::world& ecs, std::shared_ptr<CScriptProxy> scriptProxy, InputHandler* inputHandler) {

  sol::state& state = scriptProxy->GetState();
  state.open_libraries(sol::lib::math);
  state.new_usertype<Position>("Position",
    "x", &Position::x,
    "y", &Position::y,
    "z", &Position::z);
  state.new_usertype<Velocity>("Velocity",
    "x", &Velocity::x,
    "y", &Velocity::y,
    "z", &Velocity::z);
  state.new_usertype<BouncePlane>("BouncePlane",
    "x", &BouncePlane::x,
    "y", &BouncePlane::y,
    "z", &BouncePlane::z,
    "w", &BouncePlane::w);
  state.new_usertype<CursorPosition>("CursorPosition",
    "x", &CursorPosition::x,
    "y", &CursorPosition::y);

  state.new_usertype<InputHandler>("InputHandler",
    "isActionKeyPressed", &InputHandler::IsActionKeyPressed,
    "getMouseCoordinates", &InputHandler::GetMouseCoordinates);
  state.new_usertype<ReloadTimer>("ReloadTimer",
    "numberOfBulletsToAdd", &ReloadTimer::numberOfBulletsToAdd,
    "time", &ReloadTimer::time,
    "timeElapsed", &ReloadTimer::timeElapsed);
  state.new_usertype<GiveGun>("GiveGun",
    "bullet", &GiveGun::bullet,
    "numberOfBullets", &GiveGun::numberOfBullets,
    "shootKeyPressed", &GiveGun::shootKeyPressed);
  state.new_usertype<flecs::world>("World",
    "createEntity", [](flecs::world& world) {return world.entity(); },
    "getEntity", [](flecs::world& world, flecs::entity_t id) { return world.entity(id); });
  state.new_usertype<flecs::entity>("Entity",
    "getVelocity", [](flecs::entity e) { return e.get<Velocity>(); },
    "setVelocity", [](flecs::entity e, float x, float y, float z) { e.set(Velocity{ x, y, z }); },
    "getBouncePlane", [](flecs::entity e) { return e.get<BouncePlane>(); },
    "getPosition", [](flecs::entity e) { return e.get<Position>(); },
    "setPosition", [](flecs::entity e, float x, float y, float z) { e.set(Position{ x, y, z }); },
    "addIcosahedronMesh", [](flecs::entity e) { e.add<IcosahedronMesh>(); },
    "setOwner", [](flecs::entity e, flecs::entity parent) { e.is_a(parent); },
    "getGun", [](flecs::entity e) { return e.get<GiveGun>(); },
    "getReloadTimer", [](flecs::entity e) { return e.get<ReloadTimer>(); });

  auto fields = inputHandler->GetFields();
  for (size_t i = 0; i < fields.size(); ++i)
    state[fields[i]] = i;
  state["world"] = std::ref(ecs);
}


void register_ecs_script_systems(flecs::world& ecs)
{
  static auto scriptSystemQuery = ecs.query<ScriptSystemPtr>();
  static auto inputHandlerQuery = ecs.query<InputHandlerPtr>();
  ecs.system<Scripts>()
    .each([&](flecs::entity e, Scripts& scripts)
      {inputHandlerQuery.each([&](InputHandlerPtr& inputHandler) {
          scriptSystemQuery.each([&](ScriptSystemPtr& scriptSystem) {
              for (const auto& script : scripts.names){
                auto scriptProxy = scriptSystem.ptr->CreateProxy(script.c_str());
                initialize_script_state(ecs, scriptProxy, inputHandler.ptr);
                scriptProxy->PassValueToLuaScript("inputHandler", std::ref(*inputHandler.ptr));
                scriptProxy->PassValueToLuaScript("entity_id", e.id());
              }
            });
        });
        e.remove<Scripts>();
      });
}
