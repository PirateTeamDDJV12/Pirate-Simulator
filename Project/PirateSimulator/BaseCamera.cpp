#include "stdafx.h"

#include "BaseCamera.h"
#include "../Chapitre 10/PetitMoteur3D/MoteurWindows.h"

using namespace PirateSimulator;
using namespace PirateSimulator::cameraModule;
using namespace DirectX;



BaseCamera::BaseCamera(const CameraProjectionParameters& defaultParameters, const CameraMovingParameters& moveParams, const XMVECTOR camPos, const XMVECTOR camDir, const XMVECTOR camUp) :
    m_Parameters{defaultParameters},
    m_moveParams{moveParams}
{
    m_position = camPos;
    m_direction = camDir;
    m_up = camUp;

    setMatrixView(XMMatrixLookToLH(camPos, camDir, camUp));

    this->initViewMatrix();
    this->initProjMatrix();
}


void BaseCamera::initViewMatrix()
{}


void BaseCamera::initProjMatrix()
{
    m_proj = XMMatrixPerspectiveFovLH(
        m_Parameters.angle,
        m_Parameters.aspectRatio(),
        m_Parameters.nearest,
        m_Parameters.farthest);
}


void BaseCamera::onResize(unsigned int width, unsigned int height)
{
    m_Parameters.clientWidth = width;
    m_Parameters.clientHeight = height;
    initProjMatrix();
}


void BaseCamera::position(const DirectX::XMVECTOR& position)
{
    m_position = position;

    setMatrixView(XMMatrixLookToLH(position,
                                   m_direction,
                                   m_up));
}

void BaseCamera::listenInput()
{
    // Pour les mouvements, nous utilisons le gestionnaire de saisie
    PM3D::CMoteurWindows& rMoteur = PM3D::CMoteurWindows::GetInstance();
    CDIManipulateur& rGestionnaireDeSaisie = rMoteur.GetGestionnaireDeSaisie();

    if(rGestionnaireDeSaisie.ToucheAppuyee(DIK_A))
    {
        move(PirateSimulator::Move::Translation::LEFT);
    }

    if(rGestionnaireDeSaisie.ToucheAppuyee(DIK_D))
    {
        move(PirateSimulator::Move::Translation::RIGHT);
    }

    if(rGestionnaireDeSaisie.ToucheAppuyee(DIK_W))
    {
        move(PirateSimulator::Move::Translation::FORWARD);
    }

    if(rGestionnaireDeSaisie.ToucheAppuyee(DIK_S))
    {
        move(PirateSimulator::Move::Translation::BACKWARD);
    }

    if(rGestionnaireDeSaisie.ToucheAppuyee(DIK_SPACE))
    {
        move(PirateSimulator::Move::Translation::UP);
    }

    if(rGestionnaireDeSaisie.ToucheAppuyee(DIK_F))
    {
        move(PirateSimulator::Move::Translation::DOWN);
    }


    if(rGestionnaireDeSaisie.ToucheAppuyee(DIK_LEFT))
    {
        rotate(PirateSimulator::Move::Rotation::Y_CLOCKWISE);
    }

    if(rGestionnaireDeSaisie.ToucheAppuyee(DIK_RIGHT))
    {
        rotate(PirateSimulator::Move::Rotation::Y_INVERT_CLOCKWISE);
    }

    if(rGestionnaireDeSaisie.ToucheAppuyee(DIK_UP))
    {
        rotate(PirateSimulator::Move::Rotation::X_INVERT_CLOCKWISE);
    }

    if(rGestionnaireDeSaisie.ToucheAppuyee(DIK_DOWN))
    {
        rotate(PirateSimulator::Move::Rotation::X_CLOCKWISE);
    }

    if(rGestionnaireDeSaisie.ToucheAppuyee(DIK_CAPSLOCK))
    {
        changeVelocity();
    }
}
