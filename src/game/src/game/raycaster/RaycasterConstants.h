#pragma once

namespace game
{
	constexpr float FOV = PI / 3.f; // 60 degrees
	constexpr int X_PX_STEP = 1;
	constexpr int COLUMNS = SCREEN_WIDTH / X_PX_STEP;
	constexpr float START_ANGLE = 0.f;
}