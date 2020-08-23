#ifndef GameDev2D_stdafx_h
#define GameDev2D_stdafx_h

#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <WindowsX.h>
#include <Xinput.h>
#include <mmsystem.h>
#include <regstr.h>

#include "Shlwapi.h"
#pragma comment(lib, "shlwapi.lib")

#pragma comment(lib, "Xinput9_1_0")
#pragma comment(lib, "Winmm")

#include <algorithm>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <sstream>
#include <vector>

#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <time.h>


#include "Application.h"
#include "GameLoop.h"
#include "GameWindow.h"
#include "../Animation/Animator.h"
#include "../Animation/Easing.h"
#include "../Audio/Audio.h"
#include "../Audio/AudioTypes.h"
#include "../Core/Drawable.h"
#include "../Core/Transformable.h"
#include "../Debug/Log.h"
#include "../Debug/Profile.h"
#include "../Events/AudioEvent.h"
#include "../Events/Event.h"
#include "../Events/EventDispatcher.h"
#include "../Events/EventHandler.h"
#include "../Events/FullscreenEvent.h"
#include "../Events/GamePadButtonDownEvent.h"
#include "../Events/GamePadButtonUpEvent.h"
#include "../Events/GamePadConnectedEvent.h"
#include "../Events/GamePadDisconnectedEvent.h"
#include "../Events/GamePadLeftThumbStickEvent.h"
#include "../Events/GamePadLeftTriggerEvent.h"
#include "../Events/GamePadRightThumbStickEvent.h"
#include "../Events/GamePadRightTriggerEvent.h"
#include "../Events/KeyDownEvent.h"
#include "../Events/KeyUpEvent.h"
#include "../Events/KeyRepeatEvent.h"
#include "../Events/MouseButtonDownEvent.h"
#include "../Events/MouseButtonUpEvent.h"
#include "../Events/MouseMovementEvent.h"
#include "../Events/MouseScrollWheelEvent.h"
#include "../Events/ResizeEvent.h"
#include "../Events/TextureResourceEvent.h"
#include "../Graphics/AnimatedSprite.h"
#include "../Graphics/Camera.h"
#include "../Graphics/Color.h"
#include "../Graphics/GraphicTypes.h"
#include "../Graphics/Polygon.h"
#include "../Graphics/RenderTarget.h"
#include "../Graphics/Shader.h"
#include "../Graphics/Sprite.h"
#include "../Graphics/SpriteAtlas.h"
#include "../Graphics/SpriteBatch.h"
#include "../Graphics/SpriteFont.h"
#include "../Graphics/Texture.h"
#include "../Graphics/VertexData.h"
#include "../Input/Keyboard.h"
#include "../Input/Mouse.h"
#include "../IO/File.h"
#include "../Math/Math.h"
#include "../Math/Matrix.h"
#include "../Math/Rotation.h"
#include "../Math/Vector2.h"
#include "../Services/Services.h"
#include "../Services/DebugUI/DebugUI.h"
#include "../Services/Graphics/Graphics.h"
#include "../Services/InputManager/InputManager.h"
#include "../Services/ResourceManager/ResourceManager.h"
#include "../Utils/Png/Png.h"
#include "../Utils/Text/Text.h"

#endif