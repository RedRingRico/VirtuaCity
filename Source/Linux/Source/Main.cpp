#include <iostream>
#include <cstdio>
#include <GLWindow.hpp>

bool g_Quit = false;

void WinDestroyed( );

int main( int p_Argc, char **p_ppArgv )
{
	std::cout << "VirtuaCity" << std::endl;

	VirtuaCity::GLWindow TmpWin;

	if( TmpWin.Initialise( &WinDestroyed ) == 0 )
	{
		printf( "Failed to initialise window\n" );
		return 1;
	}

	while( g_Quit == false )
	{
		TmpWin.Update( );
	}

	return 0;
}

void WinDestroyed( )
{
	printf( "Window destroyed\n" );
	g_Quit = true;
}

