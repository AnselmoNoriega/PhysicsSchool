#pragma once
#include "entt/single_include/entt/entt.hpp"

namespace jm
{
	using Entity_id = entt::entity;
	using Entity_registry = entt::registry;
	constexpr auto Null_entity_id = entt::null;
}