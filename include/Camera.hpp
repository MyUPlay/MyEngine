#pragma once

namespace MyEngine {
	template<typename T>
	class Camera;
}

#include "Object3D.hpp"
#include "Matrix4.hpp"
#include "Quaternion.hpp"

template<typename T = float>
class MyEngine::Camera : public Object3D<T> {

protected:

	Camera() : Object3D<T>() {}

	Camera(const Camera& camera) : Object3D<T>(camera) {
		matrixWorldInverse = camera.matrixWorldInverse;
		projectionMatrix = camera.projectionMatrix;
	}

	Camera& copy(const Camera& camera) {

		Object3D<T>::copy(camera);

		matrixWorldInverse = camera.matrixWorldInverse;
		projectionMatrix = camera.projectionMatrix;

		return *this;

	}

public:

	Matrix4 <T> matrixWorldInverse;
	Matrix4 <T> projectionMatrix;

	virtual ~Camera() = default;

	Vector3 <T> getWorldDirection() const {

		Quaternion<T> quaternion;

		this->getWorldQuaternion(quaternion);

		return Vector3<T>(0, 0, -1).applyQuaternion(quaternion);
	}

	virtual void updateProjectionMatrix() = 0;

	void lookAt(const Vector3 <T>& point) {
		Matrix4<T> m1;
		m1.lookAt(this->position, point, this->up);
		this->quaternion.setFromRotationMatrix(m1);
	}
};
