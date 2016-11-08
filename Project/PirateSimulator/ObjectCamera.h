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
        private:
            Transform m_tranform;
            Transform *m_target;

        public:
		};
	}
}


#endif //!OBJECTCAMERA_H