#pragma once
#include <iostream>
struct MouseState
{
	double last_frame_x;
	double last_frame_y;

	double dx;
	double dy;

	float x_sensitivity;
	float y_sensitivity;

	bool first_frame;
};

MouseState create_mouse_state()
{
	MouseState mouse_state{};
	mouse_state.first_frame = true;

	mouse_state.x_sensitivity = 0.5f;
	mouse_state.y_sensitivity = 0.5f;

	return mouse_state;
}

void update_mouse_deltas(MouseState* mouse_state, double x, double y)
{
	if (mouse_state->first_frame)
	{
		mouse_state->last_frame_x = x;
		mouse_state->last_frame_y = y;
		mouse_state->first_frame = false;
	}

	// Update the deltas
	mouse_state->dx = (x - mouse_state->last_frame_x);
	mouse_state->dy = (mouse_state->last_frame_y - y);

	mouse_state->dx *= mouse_state->x_sensitivity;
	mouse_state->dy *= mouse_state->y_sensitivity;

	// Set last frame positions
	mouse_state->last_frame_x = x;
	mouse_state->last_frame_y = y;
}

void reset_mouse_deltas(MouseState* mouse_state)
{
	mouse_state->dx = 0;
	mouse_state->dy = 0;
}