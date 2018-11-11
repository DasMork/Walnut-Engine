#pragma once

#ifdef WN_PLATFORM_WINDOWS
	#ifdef WN_BUILD_DLL
		#define ALNUT_API __declspec(dllexport)
	#else
		#define WALNUT_API __declspec(dllimport)
	#endif
#else
	#error Walnut only supports Windows at the moment!
#endif