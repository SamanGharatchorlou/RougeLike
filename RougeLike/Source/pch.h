#pragma once

#pragma warning(disable : 4018)

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

// --- Standard Library --- 

// Strings
#include <string>
#include <sstream>
#include <cstdio>
#include <iostream>
#include <fstream>

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

// I should change the name, this also includes some Release settings needed
#include "Debug/DebugSettings.h"


// Include these first as they are be required in some of the below includes
#include "Debug/Logging.h"

// Containers
#include "Utilities/BasicString.h"
#include "Utilities/Vector2D.h"
#include "Utilities/Rect.h"
#include "Utilities/Quad2D.h"
#include "Utilities/RenderColour.h"
#include "Utilities/Helpers.h"
#include "Utilities/Grid.h"
#include "Utilities/Timer.h"
#include "Utilities/Queue.h"
#include "Utilities/UniqueQueue.h"

#include "Utilities/Maps/DataMap.h"
#include "Utilities/Maps/StringMap.h"
#include "Utilities/Maps/PropertyMap.h"


// File accessors
#include "System/Files/XMLParser.h"
#include "System/Files/FileManager.h"


// --- Debug Only Includes ---
#if DEBUG_CHECK
#include "Debug/DebugDraw.h"
#endif

// NEW operator override
#if MEMORY_TRACKING
#include "Debug/MemTrack.h"
#else
#define PRINT_MEMORY 0
#endif

