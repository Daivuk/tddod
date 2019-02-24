#ifndef ECS_H_INCLUDED
#define ECS_H_INCLUDED

#include <entt/entt.hpp>

using Registry = entt::DefaultRegistry;
using Entity = Registry::entity_type;

static const Entity NULL_ENTITY = entt::null;

#endif
