#include "Camera.h"
#include "glm/gtx/transform.hpp"

#include <iostream>

// Default camera values
const float DEF_FOV = 45.0f; // degrees


Camera::Camera() 
	:	mPosition(glm::vec3(0.0f, 0.0f, 0.0f)),
		mTargetPos(glm::vec3(0.0f, 0.0f, 0.0f)),
		mUp(glm::vec3(0.0f, 1.0f, 0.0f)),
		mRight(0.0f, 0.0f, 0.0f),
		WORLD_UP(0.0f, 1.0f, 0.0f),
		mYaw(0.0f),	  // theta
		mPitch(0.0f), // phi
		mFOV(DEF_FOV)
{
}

glm::mat4 Camera::getViewMatrix() const
{
	return glm::lookAt(mPosition, mTargetPos, mUp);
}

const glm::vec3& Camera::getLook() const
{
	return mLook;
}

const glm::vec3& Camera::getRight() const
{
	return mRight;
}

const glm::vec3& Camera::getUp() const
{
	return mUp;
}

const glm::vec3& Camera::getPosition() const
{
	return mPosition;
}


//----------------------
// FPS Camera Class
//----------------------

// constructor
FPSCamera::FPSCamera(glm::vec3 position, float yaw, float pitch)
{
	mPosition = position;
	mYaw = yaw;
	mPitch = pitch;
}

// set the camera's position
void FPSCamera::setPosition(const glm::vec3& position)
{
	mPosition = position;
}

// move the camera's position
void FPSCamera::move(const glm::vec3& offsetPos)
{
	mPosition += offsetPos;
	updateCameraVectors();
}

// rotate camera using Yaw and Pitch angles passed in degrees
void FPSCamera::rotate(float yaw, float pitch)
{
	mYaw += glm::radians(yaw);
	mPitch += glm::radians(pitch);

	//constrain the pitch
	mPitch = glm::clamp(mPitch, -glm::pi<float>() / 2.0f + 0.1f, glm::pi<float>() / 2.0f - 0.1f);
	updateCameraVectors();
}

// calculates the vectors from the Camera's *updated* Euler angles
void FPSCamera::updateCameraVectors()
{
	// calculate the view direction victor based on yaw and pitch angles (roll not considered)
	// spherical to cartesian conversion
	glm::vec3 look;
	look.x = cosf(mPitch) * sinf(mYaw);
	look.y = sinf(mPitch);
	look.z = cosf(mPitch) * cosf(mYaw);

	mLook = glm::normalize(look);

	// re-calculate the Righht and Up vectors. for simplicity the right vector
	// will be assumed horizontal w/r/t the world's up vector
	mRight = glm::normalize(glm::cross(mLook, WORLD_UP));
	mUp = glm::normalize(glm::cross(mRight, mLook));

	mTargetPos = mPosition + mLook;
}



//----------------------
// OrbitCamera Class
//----------------------
OrbitCamera::OrbitCamera()
	: mRadius(10.0f)
{}

void OrbitCamera::setLookAt(const glm::vec3& target)
{
	mTargetPos = target;
}

void OrbitCamera::setRadius(float radius)
{
	mRadius = glm::clamp(radius, 2.0f, 80.0f);
}

void OrbitCamera::rotate(float yaw, float pitch)
{
	mYaw = glm::radians(yaw);
	mPitch = glm::radians(pitch);

	mPitch = glm::clamp(mPitch, 
				-glm::pi<float>() / 2.0f + 0.1f,
				 glm::pi<float>() / 2.0f - 0.1f);

	updateCameraVectors();
}

void OrbitCamera::updateCameraVectors()
{
	// Spherical to Cartesian coordinates
	// https://en.wikipedia.org/wiki/Spherical_coordinate_system (NOTE: Our coordinate sys has Y up not Z)
	mPosition.x = mTargetPos.x + mRadius * cosf(mPitch) * sinf(mYaw);
	mPosition.y = mTargetPos.y + mRadius * sinf(mPitch);
	mPosition.z = mTargetPos.z + mRadius * cosf(mPitch) * cosf(mYaw);
}