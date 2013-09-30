#include <iostream>
#include <Application.hpp>

int main( int p_Argc, char **p_ppArgv )
{
	std::cout << "VirtuaCity" << std::endl;

	VirtuaCity::Application TheProgram;

	if( TheProgram.Initialise( ) != ZED_OK )
	{
		std::cout << "Failed to initialise application" << std::endl;
		return ZED_FAIL;
	}

	return TheProgram.Execute( );
}

