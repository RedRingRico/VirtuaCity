#ifndef __VIRTUACITY_GLWINDOW_HPP__
#define __VIRTUACITY_GLWINDOW_HPP__

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glx.h>
#include <GL/glxext.h>

typedef void ( *WindowDestroyedCallback )( );

namespace VirtuaCity
{
	class GLWindow
	{
	public:
		GLWindow( );
		~GLWindow( );

		int Initialise( WindowDestroyedCallback m_pCallback );

		void Update( );

		void Destroy( );

	private:
		Window		m_Window;
		Display		*m_pDisplay;
		GLXFBConfig	m_FBConfig;
		XVisualInfo	*m_pVisualInfo;
		GLXContext	m_Context;

		WindowDestroyedCallback m_pDestroyedWindow;
	};
}

#endif

