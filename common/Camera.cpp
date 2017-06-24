#include "Camera.h"

#include "glm/gtc/matrix_transform.hpp"

Camera::Camera() :
	pos_(0, 0, 0),
	up_(0, 1, 0),
	world_up_(0, 1, 0),
	move_speed_(20.f),
	pitch_(0.f),
	yaw_(-90.f),
	mouse_sensitivity_(0.1f),
	zoom_(45.f) {

	UpdateCameraVector();
}

Camera::Camera(glm::vec3 pos, float pitch, float yaw): Camera() {
	pos_ = pos;
	pitch_ = pitch;
	yaw_ = yaw;
	UpdateCameraVector();
}

void Camera::Move(CameraDirection dir, float delta_time) {
	if (dir == CameraDirection::FORWARD)
		pos_ += front_ * move_speed_ * delta_time;
	else if (dir == CameraDirection::BACKWARD)
		pos_ -= front_ * move_speed_ * delta_time;
	else if (dir == CameraDirection::LEFT)
		pos_ -= right_ * move_speed_ * delta_time;
	else
		pos_ += right_ * move_speed_ * delta_time;
}

void Camera::DirectionMove(float xoffset, float yoffset) {
	xoffset *= mouse_sensitivity_;
	yoffset *= mouse_sensitivity_;
	
	pitch_ += yoffset;
	yaw_ += xoffset;

	pitch_ = glm::clamp(pitch_, -89.f, 89.f);

	UpdateCameraVector();
}

void Camera::Zoom(float yoffset) {
	zoom_ -= yoffset;
	zoom_ = glm::clamp(zoom_, 1.f, 45.f);
}

glm::mat4 Camera::GetViewMatrix() {
	return glm::lookAt(pos_, pos_ + front_, up_);
}

void Camera::UpdateCameraVector() {
	float radian_pitch = glm::radians(pitch_);
	float radian_yaw = glm::radians(yaw_);
	front_.y = std::sin(radian_pitch);
	front_.x = std::cos(radian_pitch) * std::cos(radian_yaw);
	front_.z = std::cos(radian_pitch) * std::sin(radian_yaw);
	front_ = glm::normalize(front_);

	right_ = glm::normalize(glm::cross(front_, world_up_));
	up_ = glm::normalize(glm::cross(right_, front_));
}