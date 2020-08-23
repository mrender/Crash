#pragma once

//Application constants
#define TARGET_FPS 60
#define LIMIT_FPS true
#define WINDOW_TITLE "Crash"
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define WINDOW_IS_FULLSCREEN false
#define BACKGROUND_CLEAR_COLOR Color::OuterSpaceBlackColor()
#define DEBUG_DRAW_FPS 0
#define DEBUG_DRAW_DELTA_TIME 0
#define DEBUG_DRAW_ELAPSED_TIME 0
#define DEBUG_DRAW_ALLOCATED_TEXTURE_MEMORY 0
#define DEBUG_DRAW_SPRITE_RECT 0
#define DEBUG_DRAW_SPRITE_AABB 0
#define DEBUG_DRAW_MOUSE_POSITION 0
#define OPENGL_MAJOR_VERSION 3
#define OPENGL_MINOR_VERSION 1
#define THROW_EXCEPTION_ON_ERROR 1
#define LOG_TO_FILE 0
#define LOG_FILE "/Log.txt"
#define LOG_VERBOSITY_MASK Log::Verbosity_Debug | Log::Verbosity_Application
#define CHECK_FOR_MEMORY_LEAKS 0
//#define RANDOM_SEED 1729