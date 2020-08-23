#include "Camera.h"
#include "../Events/ResizeEvent.h"
#include "../Events/UpdateEvent.h"
#include "../Math/Math.h"
#include "../Services/Services.h"
#include "../Windows/Application.h"


namespace GameDev2D
{
    Camera::Camera() : Transformable(),
        m_ProjectionMatrix(Matrix::Identity()),
        m_Viewport(0, 0),
		m_IsViewportResizeable(true),
        m_ClipNear(-1.0f),
        m_ClipFar(1.0f),
        m_ShakeEnabled(false),
        m_ShakeMagnitude(0.0f),
        m_ShakeDuration(0.0),
        m_ShakeTimer(0.0f),
        m_ShakeOffset(Vector2(0.0f, 0.0f))
    {
		Services::GetApplication()->AddEventListener(this, RESIZE_EVENT);
		Services::GetApplication()->AddEventListener(this, UPDATE_EVENT);
    }

	Camera::Camera(const Camera& aCamera) : Transformable(aCamera),
		m_ProjectionMatrix(aCamera.m_ProjectionMatrix),
		m_Viewport(aCamera.m_Viewport),
		m_IsViewportResizeable(aCamera.m_IsViewportResizeable),
		m_ClipNear(aCamera.m_ClipNear),
		m_ClipFar(aCamera.m_ClipFar),
		m_ShakeEnabled(aCamera.m_ShakeEnabled),
		m_ShakeMagnitude(aCamera.m_ShakeMagnitude),
		m_ShakeDuration(aCamera.m_ShakeDuration),
		m_ShakeTimer(aCamera.m_ShakeTimer),
		m_ShakeOffset(aCamera.m_ShakeOffset)
	{
		Services::GetApplication()->AddEventListener(this, RESIZE_EVENT);
		Services::GetApplication()->AddEventListener(this, UPDATE_EVENT);
	}

	Camera::Camera(const Viewport& aViewport, bool aIsViewportResizeable) : Transformable(),
		m_ProjectionMatrix(Matrix::Identity()),
		m_Viewport(0, 0),
		m_IsViewportResizeable(aIsViewportResizeable),
		m_ClipNear(-1.0f),
		m_ClipFar(1.0f),
		m_ShakeEnabled(false),
		m_ShakeMagnitude(0.0f),
		m_ShakeDuration(0.0),
		m_ShakeTimer(0.0f),
		m_ShakeOffset(Vector2(0.0f, 0.0f))
	{
		SetViewport(aViewport);
		Services::GetApplication()->AddEventListener(this, RESIZE_EVENT);
		Services::GetApplication()->AddEventListener(this, UPDATE_EVENT);
	}

	Camera::~Camera()
	{
		Services::GetApplication()->RemoveEventListener(this, RESIZE_EVENT);
		Services::GetApplication()->RemoveEventListener(this, UPDATE_EVENT);
	}

    Matrix Camera::GetProjectionMatrix()
    {
        return m_ProjectionMatrix;
    }
    
    Matrix Camera::GetViewMatrix()
    {
        Matrix viewMatrix = GetTransformMatrix().GetInverse();
        viewMatrix.SetTranslation(viewMatrix.GetTranslation() + m_ShakeOffset);
        return viewMatrix;
    }

    void Camera::SetViewport(const Viewport& aViewport)
    {
        //Set the view view and height
        m_Viewport = aViewport;

        //Set's the position of the camera to be in the middle
        SetPosition(Vector2(m_Viewport.x + (float)m_Viewport.width * 0.5f, m_Viewport.y + (float)m_Viewport.height * 0.5f));

        //Reset the project and view matrices
        ResetProjectionMatrix();
    }

    Viewport Camera::GetViewport()
    {
        return m_Viewport;
    }

    void Camera::SetDepthClip(float aNear, float aFar)
    {
        m_ClipNear = aNear;
        m_ClipFar = aFar;
    }

    float Camera::GetNearClip()
    {
        return m_ClipNear;
    }

    float Camera::GetFarClip()
    {
        return m_ClipFar;
    }

    void Camera::Shake(float aMagnitude, double aDuration)
    {
        m_ShakeEnabled = true;
        m_ShakeMagnitude = aMagnitude;
        m_ShakeDuration = aDuration;
        m_ShakeTimer = 0.0;
        m_ShakeOffset = Vector2(0.0f, 0.0f);
    }

	void Camera::HandleEvent(Event* aEvent)
	{
		if (aEvent->GetEventCode() == RESIZE_EVENT)
		{
			if (m_IsViewportResizeable == true)
			{
				ResizeEvent* resizeEvent = (ResizeEvent*)aEvent;
				SetViewport(Viewport((unsigned int)resizeEvent->GetSize().x, (unsigned int)resizeEvent->GetSize().y));
			}
		}
		else if (aEvent->GetEventCode() == UPDATE_EVENT)
		{
			UpdateEvent* updateEvent = (UpdateEvent*)aEvent;
			
			//Is the shake enabled?
			if (m_ShakeEnabled == true)
			{
				//Get the elapsed time
				m_ShakeTimer += updateEvent->GetDelta();

				//Has the shake ended?
				if (m_ShakeTimer >= m_ShakeDuration)
				{
					m_ShakeEnabled = false;
					m_ShakeTimer = 0.0;
					m_ShakeDuration = 0.0;
					m_ShakeOffset = Vector2(0.0f, 0.0f);
				}
				else
				{
					//Calculate the shake offset based on the progress and magnitude
					float progress = (float)(m_ShakeTimer / m_ShakeDuration);
					float magnitude = (float)(m_ShakeMagnitude * (1.0 - (progress * progress)));
					m_ShakeOffset = Vector2(RandomShake(magnitude), RandomShake(magnitude));
				}
			}
		}
	}

    void Camera::ResetProjectionMatrix()
    {
        //Get the view's width and height
        float width = static_cast<float>(m_Viewport.width);
        float height = static_cast<float>(m_Viewport.height);

        //Setup the orthographic projection
        m_ProjectionMatrix = Matrix::Orthographic(-width / 2.0f, width / 2.0f, -height / 2.0f, height / 2.0f, m_ClipNear, m_ClipFar);
    }
    
    float Camera::RandomShake(float aMagnitude)
    {
        return (Math::RandomFloat() * 2.0f - 1.0f) * aMagnitude;
    }
}