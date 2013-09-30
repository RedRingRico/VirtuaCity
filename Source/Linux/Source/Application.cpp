#include <Application.hpp>
#include <System/Memory.hpp>
#include <System/LinuxWindow.hpp>
#include <System/LinuxInputManager.hpp>
#include <Renderer/LinuxRendererOGL3.hpp>

namespace VirtuaCity
{
	Application::Application( )
	{
		m_pRenderer = ZED_NULL;
		m_pWindow = ZED_NULL;
		m_pInputManager = ZED_NULL;
	}

	Application::~Application( )
	{
		zedSafeDelete( m_pRenderer );
		zedSafeDelete( m_pInputManager );
		zedSafeDelete( m_pWindow );
	}

	ZED_UINT32 Application::Initialise( )
	{
		m_pRenderer = new ZED::Renderer::LinuxRendererOGL3( );
		m_pWindow = new ZED::System::LinuxWindow( );

		ZED_UINT32 WindowStyle = 0;

		WindowStyle |= ZED_WINDOW_STYLE_MINIMISE | ZED_WINDOW_STYLE_CLOSE |
			ZED_WINDOW_STYLE_TITLEBAR | ZED_WINDOW_STYLE_MOVE;

		m_pWindow->Create( 0, 0, 1280, 720, WindowStyle );

		m_Canvas.Width( 1280 );
		m_Canvas.Height( 720 );
		m_Canvas.BackBufferCount( 1 );
		m_Canvas.ColourFormat( ZED_FORMAT_ARGB8 );
		m_Canvas.DepthStencilFormat( ZED_FORMAT_D24S8 );

		m_pRenderer->Create( m_Canvas, ( *m_pWindow ) );

		m_pRenderer->ClearColour( 1.0f, 0.0f, 0.0f );
		m_pRenderer->SetRenderState( ZED_RENDERSTATE_CULLMODE,
			ZED_CULLMODE_NONE );
		m_pRenderer->SetRenderState( ZED_RENDERSTATE_DEPTH, ZED_ENABLE );
		m_pRenderer->SetClippingPlanes( 1.0f, 100000.0f );
		m_pRenderer->PerspectiveProjectionMatrix( 45.0f, 1280.0f/720.0f );
		ZED::System::ZED_WINDOWDATA WinData = m_pWindow->WindowData( );

		m_pInputManager =
			new ZED::System::LinuxInputManager( WinData.pX11Display );
		m_pInputManager->AddDevice( &m_Keyboard );

		return ZED_OK;
	}

	ZED_UINT32 Application::Execute( )
	{
		return ZED_OK;
	}

	void Application::Update( )
	{
	}

	void Application::Render( )
	{
		m_pRenderer->BeginScene( ZED_TRUE, ZED_TRUE, ZED_TRUE );
		m_pRenderer->EndScene( );
	}
}

