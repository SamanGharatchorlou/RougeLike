#pragma once


// --- Standard Library --- 

// Strings
#include <string>
#include <sstream>
#include <cstdio>
#include <iostream>

// Containers
#include <stack>
#include <vector>
#include <unordered_map>
#include <queue>

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

// Include these first as they are be required in some of the below includes
#include "Debug/Logging.h"
#include "System/Files/Attributes.h"

// Utilities / helper classes
#include "Utilities/Vector2D.h"
#include "Utilities/Rect.h"
#include "Utilities/Helpers.h"
#include "Utilities/Grid.h"
#include "Utilities/Timer.h"

// Debug
#include "Debug/DebugDraw.h"

// File accessors
#include "System/Files/XMLParser.h"
#include "System/Files/FileManager.h"
#include "System/Files/XMLValueMap.h"
