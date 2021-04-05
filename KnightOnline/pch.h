#pragma once

#define ENABLE_IMGUI // #undef to disable ImGui windows from showing
#define D3D8TO9NOLOG // #undef for d3d8to9 dll to output logs

//#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <assert.h>
#include <fstream>
#include <functional>
#include <filesystem>

#include "Common/Logger.h"
#include "Common/Utility.h"
#include "Common/HookUtility.h"
#include "Common/Constants.h"

namespace fs = std::filesystem;
