#pragma once
#include <vector>
#include <memory>
#include "Object.h"
#include "WaterSurface.h"
#include "DirectionalLight.h"
#include "HemisphericAmbientLight.h"

namespace World
{
class Map
{
public:
    Map() { }

    void UpdateDynamicObject(float dt)
    {
        for (auto& obj : m_dynamicObjects) {
            obj->Simulate(dt);
        }
    }
    void Bind()
    {
        for (auto& obj : m_staticObjects) {
            obj->Bind();
        }
        for (auto& obj : m_dynamicObjects) {
            obj->Bind();
        }
        for (auto& obj : m_directionalLightObjects) {
            obj->Bind();
        }
        for (auto& obj : m_ambientLightObjects) {
            obj->Bind();
        }
    }
public:
    std::vector<std::unique_ptr<World::Object::Object>>       m_staticObjects;
    std::vector<std::unique_ptr<World::Object::WaterSurface>> m_dynamicObjects;
    std::vector<std::unique_ptr<World::Object::DirectionalLight>>
        m_directionalLightObjects;
    std::vector<std::unique_ptr<World::Object::HemisphericAmbientLight>>
        m_ambientLightObjects;
};
}
