#ifndef __VIRTUACITY_APPLICATION_HPP__
#define __VIRTUACITY_APPLICATION_HPP__

#include <System/DataTypes.hpp>

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
	};
}

#endif

