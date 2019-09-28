#include "Camera.h"
#include <src/Settings.h>

Camera::Camera(float fov, float aspect, float near, float far, const Transform& transform) : fov(fov), aspect(aspect), near(near), far(far)
{
}

Camera::Camera()
{
	Settings* settings = Settings::get();
	fov = settings->getFOV();
	aspect = settings->getAspect();
	near = settings->getScreenNear();
	far = settings->getScreenFar();
}

void Camera::Update()
{
	transform.Update();
	m_viewMatrix = transform.rotation.Inverse().toMatrix() * Matrix4::Translate(transform.position*-1);
	m_projectionMatrix = Matrix4::Perspective(fov, aspect, near, far);
}
