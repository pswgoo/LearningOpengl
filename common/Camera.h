#pragma once

#include "glm/glm.hpp"

enum class CameraDirection {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
};

class Camera {
public:
	glm::vec3 pos_;
	glm::vec3 front_;
	glm::vec3 up_;
	glm::vec3 right_;
	glm::vec3 world_up_;
	float move_speed_;

	float pitch_;
	float yaw_;
	float mouse_sensitivity_;
	float zoom_;

	Camera();
	Camera(glm::vec3 pos, float pitch = 0.f, float yaw = -90.f);

	void Move(CameraDirection dir, float delta_time);

	void DirectionMove(float xoffset, float yoffset);

	void Zoom(float yoffset);

	glm::mat4 GetViewMatrix();


private:
	void UpdateCameraVector();
};
