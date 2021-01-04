#ifndef CAMERA_H
#define CAMERA_H

// ---------------------
// Abstract camera class:
// the purpose of this class is to return 
// a view matrix back to the program
// ---------------------

#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"

class Camera
{
public:

	glm::mat4 getViewMatrix() const;

	virtual void setPosition(const glm::vec3& position) {}
	virtual void rotate(float yaw, float pitch) {}  // angles in degrees
	virtual void move(const glm::vec3& offSetPos) {}

	const glm::vec3& getLook() const;
	const glm::vec3& getRight() const;
	const glm::vec3& getUp() const;
	const glm::vec3& getPosition() const;

	float  getFOV() const { return mFOV; }
	void setFOV(float fov) { mFOV = fov; }


protected:
	Camera();

	virtual void updateCameraVectors() {}

	glm::vec3 mPosition;
	glm::vec3 mTargetPos;
	glm::vec3 mUp;
	glm::vec3 mLook;
	glm::vec3 mRight;
	const glm::vec3 WORLD_UP;

	// Euler angles in radians
	float mYaw;
	float mPitch;
	// float roll;
	
	// Camera parameters
	float mFOV; // degrees

};

//--------------------------------------------------------------
// FPS Camera Class
//--------------------------------------------------------------
class FPSCamera : public Camera
{
public:

	FPSCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), float yaw = glm::pi<float>(), float pitch = 0.0f); // (yaw) initial angle faces -Z

	virtual void setPosition(const glm::vec3& position);
	virtual void rotate(float yaw, float pitch);	// in degrees
	virtual void move(const glm::vec3& offsetPos);

private:

	void updateCameraVectors();
};

class OrbitCamera : public Camera
{
public:
	OrbitCamera();

	//virtual void setPosition(const glm::vec3& position);
	virtual void rotate(float yaw, float pitch);  // angles in degrees
	//virtual void move(const glm::vec3& offSetPos);

	// Camera controls
	void setLookAt(const glm::vec3& target); // a point to look at
	void setRadius(float radius);	// the radius to rotate by

private:

	void updateCameraVectors();
	float mRadius;

};

#endif // CAMERA_H
