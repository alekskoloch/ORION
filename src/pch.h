#include "TextureManager.h"
#include "FontManager.h"
#include "SoundManager.h"
#include "EventManager.h"
#include "SceneManager.h"

#include "GraphicsOperations.h"
#include "MathOperations.h"
#include "Mouse.h"
#include "StringOperations.h"

#include "components.h"
#include "tagComponents.h"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <unordered_map>
#include <string>
#include <random>
#include <ranges>
#include <concepts>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <entt/entt.hpp>
#include <nlohmann/json.hpp>