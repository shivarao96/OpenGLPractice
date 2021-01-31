#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace CameraObject {

	enum class CameraMovement {
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT,
		UP,
		DOWN
	};

	class Camera
	{
	public:
		Camera(
			glm::vec3 pos,
			glm::vec3 worldUp
		);
		Camera(
			float posX,
			float posY,
			float posZ,
			float worldUpX,
			float worldUpY,
			float worldUpZ
		);

		//..::setter::..//
		void setYaw(float val);
		void setPitch(float val);
		void setSpeed(float val);
		void setSensitivity(float val);
		void setZoom(float val);
		void setPosition(glm::vec3 val);
		void setWorldUp(glm::vec3 val);

		//..::getter::..//
		float getZoomVal() const;
		float getSpeed() const;
		const glm::vec3 getPosition() const;

		glm::mat4 getViewMatrix();

		void processMouseMovement(float xOffSet, float yOffSet, bool constrainPitch);
		void processMouseScroll(float yOffSet);
		void processKeyboard(CameraMovement direction, float deltaTime);

	private:
		//..::configuration::..//
		float m_Yaw = -90.0f;
		float m_Pitch = 0.0f;
		float m_Speed = 3.5f;
		float m_Sensitivity = 0.1f;
		float m_Zoom = 45.0f;
		float m_cZoom = 45.0f;

		glm::vec3 m_Position;
		glm::vec3 m_Front = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 m_Up;
		glm::vec3 m_Right;
		glm::vec3 m_WorldUp;

		void updateCameraVectors();
	};
}


