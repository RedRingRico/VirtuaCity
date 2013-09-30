#include <GLWindow.hpp>
#include <cstdio>
#include <cstring>
#include <GitVersion.hpp>
#include <unistd.h>

PFNGLXCREATECONTEXTATTRIBSARBPROC glXCreateContextAttribsARB = NULL;

namespace VirtuaCity
{
	GLWindow::GLWindow( )
	{
		m_pDisplay = NULL;
		m_pVisualInfo = NULL;
		m_pDestroyedWindow = NULL;

		m_Window = 0;
		m_Context = 0;
		m_FBConfig = 0;
	}

	GLWindow::~GLWindow( )
	{
		this->Destroy( );
	}

	int GLWindow::Initialise( WindowDestroyedCallback p_pCallback )
	{
		// Register the destroyed window callback
		if( !p_pCallback )
		{
			printf( "No callback specified for when the window is "
				"destroyed\n" );
			return 0;
		}

		m_pDestroyedWindow = p_pCallback;

		// Just open any display
		m_pDisplay = XOpenDisplay( NULL );

		if( !m_pDisplay )
		{
			printf( "Failed to open display\n" );
			this->Destroy( );
			return 0;
		}

		int VisualAttribs [ ] =
		{
			GLX_X_RENDERABLE,	True,
			GLX_DRAWABLE_TYPE,	GLX_WINDOW_BIT,
			GLX_RENDER_TYPE,	GLX_RGBA_BIT,
			GLX_X_VISUAL_TYPE,	GLX_TRUE_COLOR,
			GLX_RED_SIZE,		8,
			GLX_GREEN_SIZE,		8,
			GLX_BLUE_SIZE,		8,
			GLX_ALPHA_SIZE,		8,
			GLX_DEPTH_SIZE,		24,
			GLX_STENCIL_SIZE,	8,
			GLX_DOUBLEBUFFER,	True,
			None
		};

		int Major, Minor;

		if( ( !glXQueryVersion( m_pDisplay, &Major, &Minor ) ) ||
			( ( Major == 1 ) && ( Minor < 3 ) ) || ( Major < 1 ) )
		{
			printf( "GLX version too low: 1.3 or greater expected, "
				"got %d.%d instead\n",
				Major, Minor );

			this->Destroy( );
			return 0;
		}

		int FBCount;
		GLXFBConfig *pFBConfigs = glXChooseFBConfig( m_pDisplay,
			DefaultScreen( m_pDisplay ), VisualAttribs, &FBCount );

		if( !pFBConfigs )
		{
			printf( "Failed to obtain frame buffer configurations\n" );
			this->Destroy( );
			return 0;
		}

		m_FBConfig = pFBConfigs[ 0 ];

		XFree( pFBConfigs );

		m_pVisualInfo = glXGetVisualFromFBConfig( m_pDisplay, m_FBConfig );

		XSetWindowAttributes WinAttribs;

		WinAttribs.colormap = XCreateColormap( m_pDisplay,
			RootWindow( m_pDisplay, m_pVisualInfo->screen ),
			m_pVisualInfo->visual, AllocNone );

		WinAttribs.background_pixmap = None;
		WinAttribs.border_pixel = 0;
		WinAttribs.event_mask = StructureNotifyMask | ExposureMask |
			KeyPressMask | KeyReleaseMask |
			ButtonPressMask | ButtonReleaseMask |
			FocusChangeMask | EnterWindowMask | LeaveWindowMask;

		m_Window = XCreateWindow( m_pDisplay,
			RootWindow( m_pDisplay, m_pVisualInfo->screen ), 0, 0, 1280, 720,
			0, m_pVisualInfo->depth, InputOutput, m_pVisualInfo->visual,
			CWBorderPixel | CWColormap | CWEventMask, &WinAttribs );

		if( !m_Window )
		{
			printf( "Failed to crate window\n" );
			this->Destroy( );
			return 0;
		}

		Atom DeleteMessage = XInternAtom( m_pDisplay, "WM_DELETE_WINDOW",
			False );
		XSetWMProtocols( m_pDisplay, m_Window, &DeleteMessage, 1 );

		char Title[ 1024 ];
		memset( Title, '\0', sizeof( Title ) );
		sprintf( Title, "VirtuaCity | [Rev. %s]", GIT_COMMITHASH );

		XStoreName( m_pDisplay, m_Window, Title );

		XMapWindow( m_pDisplay, m_Window );

		glXCreateContextAttribsARB = ( PFNGLXCREATECONTEXTATTRIBSARBPROC )
			glXGetProcAddressARB(
				( const GLubyte * )"glXCreateContextAttribsARB" );

		if( !glXCreateContextAttribsARB )
		{
			printf( "Failed to bind to glXCreatecontextAttribsARB\n" );
			this->Destroy( );
			return 0;
		}
		
		m_Context = 0;

		int ContextAttribs[ ] =
		{
			GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
			GLX_CONTEXT_MINOR_VERSION_ARB, 0,
			None
		};

		m_Context = glXCreateContextAttribsARB( m_pDisplay, m_FBConfig, 0,
			True, ContextAttribs );

		XSync( m_pDisplay, False );

		if( !glXIsDirect( m_pDisplay, m_Context ) )
		{
			printf( "GLX is not a direct renderer\n" );
			this->Destroy( );
			return 0;
		}

		glXMakeCurrent( m_pDisplay, m_Window, m_Context );
		glClearColor( 0.14f, 0.0f, 0.14f, 1.0f );

		return 1;
	}

	void GLWindow::Update( )
	{
		int Pending = XPending( m_pDisplay );
		XEvent Event;

		glClear( GL_COLOR_BUFFER_BIT );

		for( int i = 0; i < Pending; ++i )
		{
			XNextEvent( m_pDisplay, &Event );

			switch( Event.type )
			{
				case KeyPress:
				{
					KeySym Key;

					Key = XLookupKeysym( &Event.xkey, 0 );
					if( Key == 'q' )
					{
						m_pDestroyedWindow( );
					}
					
					break;
				}
				case ClientMessage:
				{
					if( *XGetAtomName( m_pDisplay,
						Event.xclient.message_type ) == *"WM_PROTOCOLS" )
					{
						m_pDestroyedWindow( );
					}
					break;
				}
				default:
				{
					//XPutBackEvent( m_pDisplay, &Event );
					break;
				}
			}
		}

		glXSwapBuffers( m_pDisplay, m_Window );
	}

	void GLWindow::Destroy( )
	{
		if( m_pVisualInfo )
		{
			XFree( m_pVisualInfo );
			m_pVisualInfo = NULL;
		}

		if( m_Context )
		{
			glXMakeCurrent( m_pDisplay, 0, 0 );
			glXDestroyContext( m_pDisplay, m_Context );
			m_Context = 0;
		}

		if( m_Window )
		{
			XDestroyWindow( m_pDisplay, m_Window );
			m_Window = 0;
		}

		if( m_pDisplay )
		{
			XCloseDisplay( m_pDisplay );
			m_pDisplay = NULL;
		}
	}
}

