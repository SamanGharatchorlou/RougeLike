#pragma once

#pragma warning(disable : 4018)

// --- Standard Library --- 

// Strings
#include <string>
#include <sstream>
#include <cstdio>
#include <iostream>

// Containers
#include <stack>
#include <vector>
#include <map>
#include <unordered_map>
#include <queue>
#include <deque>
#include <unordered_set>

// Logging
#include <cstdarg>
#include <assert.h>

// Maths
#include <algorithm>

// Other
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;

// SDL
#include <SDL_events.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <SDL_render.h>
#include <SDL_timer.h>
#include <SDL_ttf.h>


// --- Custom ---

// NEW operator override
#include "Debug/DebugSettings.h"
#if MEMORY_TRACKING
#include "Debug/MemTrack.h"
#else
#define PRINT_MEMORY 0
#endif

// Include these first as they are be required in some of the below includes
#include "Debug/Logging.h"

// Utilities / helper classes
#include "Utilities/BasicString.h"
#include "Utilities/Vector2D.h"
#include "Utilities/Rect.h"
#include "Utilities/Helpers.h"
#include "Utilities/Grid.h"
#include "Utilities/Timer.h"
#include "Utilities/UniqueQueue.h"

// File accessors
using ValueMap = std::unordered_map<BasicString, float>;
using StringMap = std::unordered_map<BasicString, BasicString>;

#include "System/Files/Attributes.h"
#include "System/Files/XMLParser.h"
#include "System/Files/FileManager.h"

// Globals
#include "Graphics/Renderer.h"
