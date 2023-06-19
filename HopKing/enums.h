#pragma once

enum class ActionState{idle, falling, jumping, flat, moving, charging, bouncing};

enum class GameState{Menu, Game, Ending, TESTING};

enum class KeybindingAction{LEFT, RIGHT, UP, DOWN, JUMP, CONFIRM, CANCEL, PAUSE, LENGTH = 7};

static const char* KeybindingActionToString[]{ "LEFT", "RIGHT", "UP", "DOWN", "JUMP", "CONFIRM", "CANCEL", "PAUSE" };

enum class MenuType{MainMenu, HUD, OptionsMenu, InputSettings, KeyRebindingMenu, LENGTH = 4};

enum class SurfaceTypes{Normal, Slippery};