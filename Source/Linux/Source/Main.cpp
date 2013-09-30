#include <iostream>
#include <GLWindow.hpp>

int main( int p_Argc, char **p_ppArgv )
{
	std::cout << "VirtuaCity" << std::endl;

	VirtuaCity::GLWindow TmpWin;

	if( TmpWin.Initialise( ) == 0 )
	{
		printf( "Failed to initialise window\n" );
		return 1;
	}

	return 0;
}

