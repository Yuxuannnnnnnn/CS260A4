#pragma once

#include "../Math/Math.h"
#include "../Math/Transform.h"
#include "FrameBuffer.h"

namespace Graphics
{
	class Camera
	{
		FrameBuffer frameBuffer;
		float camWidth; // true viewport size
		float aspect, aspectFixed; // width over height
	public:
		float size; // orthographic
		float zNear, zFar; // perspective
		Color backgroundColor;
		bool perspective;
		static Camera* camera;

		Camera();
		void Begin();
		void End();
		const FrameBuffer& GetFrameBuffer() const;

		Matrix4x4 GetOrthographicMatrix() const;
		Matrix4x4 GetPerspectiveMatrix() const;
		Matrix4x4 GetProjectionMatrix();
		Matrix4x4 GetViewMatrix(Transform transform) const;
	};
}
