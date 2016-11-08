#ifndef FLATCLOTH_ECS_H
#define FLATCLOTH_ECS_H

#include "fc_tools.h"

#include <initializer_list>
#include <iostream>
#include <array>
#include <vector>

#define MAX_ENTITIES 100
#define MAX_COMPONENTS 32

using IdType = uint32_t;

typedef struct {
    IdType value;
} ComponentId;

using Entity = uint32_t;
using EntityId = uint32_t;
using Signature = uint32_t;

// create signature from list of ComponentId s
template <typename T>
int make_signature(std::initializer_list<T> list)
{
    int sum = 0;
    for (auto &id : list) {
        sum += 1 << id.value;
    }
    return sum;
}

template <typename Derived, int Size>
struct Component {
    static ComponentId id;
    static uint32_t top; // stack top
    static const uint32_t MAX_COMPONENT_DATA;
    static Derived data[Size];
};

// global component id counter
extern uint32_t component_id;

// assign family/type id on template resolve
template <typename Derived, int Size>
ComponentId Component<Derived, Size>::id {component_id++};

template <typename Derived, int Size>
uint32_t Component<Derived, Size>::top = 0;

// component array init
template <typename Derived, int Size>
const uint32_t Component<Derived, Size>::MAX_COMPONENT_DATA = Size;

// component array init
template <typename Derived, int Size>
Derived Component<Derived, Size>::data[Size];


struct EntityData {
    Signature signature {0};
    IdType component_data_id[MAX_COMPONENTS] {0,};
};

extern Entity entity_counter;
extern EntityData entity_data[MAX_ENTITIES];

template <typename DerivedComponent>
DerivedComponent *assign_component(Entity e)
{
    auto &sig = entity_data[e].signature;
    if ((sig & (1 << DerivedComponent::id.value)) == 0) {
        sig |= (1 << DerivedComponent::id.value);
        auto top = DerivedComponent::top++;
        if (top >= DerivedComponent::MAX_COMPONENT_DATA) {
            log_error("ECS", "component data exceeded");
            exit(1);
        }
        entity_data[e].component_data_id[DerivedComponent::id.value] = top;
        return &DerivedComponent::data[top];
    }

    log_error("ECS", "assign_component: entity already has component: %d", DerivedComponent::id.value);
    exit(1);
}

template <typename DerivedComponent>
DerivedComponent *get_component(Entity e)
{
    auto &sig = entity_data[e].signature;
    if ((sig & (1 << DerivedComponent::id.value)) == 0) {
        log_error("ECS", "entity %d get_component -> no component", e);
        exit(1);
    }

    auto cid = entity_data[e].component_data_id[DerivedComponent::id.value];
    return &DerivedComponent::data[cid];
}


class World
{
    std::vector <Entity> m_entities {};

public:
    Entity create_entity()
    {
        Entity top = entity_counter++;
        m_entities.push_back(top);
        return top;
    }

    Signature get_entity_signature(Entity e)
    {
        return entity_data[e].signature;
    }

    std::vector<Entity> get_entities(Signature signature)
    {
        std::vector<Entity> entities;
        for (auto &entity : m_entities) {
            if ((entity_data[entity].signature & signature) == signature) {
                entities.push_back(entity);
            }
        }
        return entities;
    }
};


class System
{
    Signature m_signature {0};
    Signature m_filter_out {0};

protected:
    std::vector<Entity> m_entities {};

public:
    Signature get_signature() const
    {
        return m_signature;
    }

    template <typename ...T>
    void with_entities()
    {
        m_signature = make_signature({T::id...});
    }

    template <typename ...T>
    void without_entities()
    {
        m_filter_out = make_signature({T::id...});
    }

    void register_world(World *world)
    {
        m_entities.clear();
        auto entities = world->get_entities(m_signature);

        for (auto &entity : entities) {
            auto sig = entity_data[entity].signature;

            if (m_filter_out != 0 && (sig & m_filter_out) == m_filter_out) {
                continue;
            } else {
                m_entities.push_back(entity);
            }
        }
    }
};

#endif // FLATCLOTH_ECS_H
