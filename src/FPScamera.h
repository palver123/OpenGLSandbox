#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct FPSCamera {
	enum class Direction {
		Forward,
		Backward,
		Left,
		Right
	};

	float fovY = 45.0f;
	float aspectRatio = 4 / 3.0f;
	float near = 0.1f;
	float far = 100.0f;

	glm::mat4 ViewMatrix() const {
		return glm::lookAt(Pos, Pos + Front, Up);
	}

	glm::mat4 ProjectionMatrix() const {
		return glm::perspective(fovY, aspectRatio, near, far);
	}

	void Walk(float speed, const Direction dir) {
		switch (dir)
		{
		case Direction::Forward:
				Pos += speed * Front;
				break;
		case Direction::Backward:
				Pos -= speed * Front;
				break;
		case Direction::Left:
				Pos -= glm::normalize(glm::cross(Front, Up)) * speed;
				break;
		case Direction::Right:
				Pos += glm::normalize(glm::cross(Front, Up)) * speed;
				break;
		}
	}

	void Rotate(double xoffset, double yoffset) {
		yaw += xoffset;
		pitch += yoffset;
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		glm::vec3 front;
		front.x = static_cast<float>(cos(glm::radians(pitch)) * cos(glm::radians(yaw)));
		front.y = static_cast<float>(sin(glm::radians(pitch)));
		front.z = static_cast<float>(cos(glm::radians(pitch)) * sin(glm::radians(yaw)));
		Front = glm::normalize(front);
	}

private:
	glm::vec3 Pos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 Front = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);

	double yaw = -90;
	double pitch = 0;
};