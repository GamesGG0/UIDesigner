#pragma once

#define GEODE_DEFINE_EVENT_EXPORTS
#include <VirtualNodeAPI.hpp>
using namespace uidesigner;
#undef GEODE_EVENT_EXPORT_NORES
#define GEODE_EVENT_EXPORT_NORES(...)
#undef MY_MOD_ID
#undef GEODE_DEFINE_EVENT_EXPORTS