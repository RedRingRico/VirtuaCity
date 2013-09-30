#ifndef __VIRTUACITY_APPLICATION_HPP__
#define __VIRTUACITY_APPLICATION_HPP__

#include <System/DataTypes.hpp>
#include <System/InputManager.hpp>
#include <System/Keyboard.hpp>
#include <System/Window.hpp>
#include <Renderer/CanvasDescription.hpp>
#include <Renderer/Renderer.hpp>

namespace VirtuaCity
{
	class Application
	{
	public:
		Application( );
		~Application( );

		ZED_UINT32 Initialise( );
		ZED_UINT32 Execute( );

	private:
		void Update( );
		void Render( );

		ZED::Renderer::Renderer				*m_pRenderer;
		ZED::System::Window					*m_pWindow;
		ZED::Renderer::CanvasDescription	m_Canvas;
		ZED::System::InputManager			*m_pInputManager;
		ZED::System::Keyboard				m_Keyboard;
	};
}

#endif

