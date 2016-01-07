#pragma once

#include "core/sox/measure.h"
#include "common/core/base_svid.h"

#define SCOPED_MEASURE_HELPER(id) scoped_measure measure##id( ((id << 8) & 0xFFFFFF00)| protocol::BASEAPP_SVID, 0, 0)


