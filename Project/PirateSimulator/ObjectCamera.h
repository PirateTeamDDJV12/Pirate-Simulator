/*
Created by Sun-lay Gagneux
*/
#ifndef OBJECTCAMERA_H
#define OBJECTCAMERA_H


#include "BaseCamera.h"
#include "Transform.h"


namespace PirateSimulator
{
	namespace cameraModule
	{
		class ObjectCamera : public BaseCamera
		{
        public:
            enum
            {
                DEFAULT_OFFSET = 10
            };

        private:
            Transform *m_target;
            float m_offset;

        public:
            ObjectCamera(const CameraProjectionParameters& defaultProjParameters,
                const CameraMovingParameters& moveParams,
                const Transform &transform,
                Transform* target = nullptr,
                float offset = DEFAULT_OFFSET)
                : BaseCamera(defaultProjParameters, moveParams, transform),
                m_offset{ offset },
                m_target{ target }
            {}

            ~ObjectCamera() {}

            virtual void move(Move::Translation::Direction direction);

            virtual void rotate(Move::Rotation::Direction direction);

            virtual void listenInput();
		};
	}
}


#endif //!OBJECTCAMERA_H