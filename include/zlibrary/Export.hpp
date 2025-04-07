//
// Created by Advik on 07-04-2025.
//

#ifndef EXPORT_HPP
#define EXPORT_HPP

// For Windows DLL export
#ifdef _WIN32
	#ifdef ZLIBRARY_EXPORTS
        // If we are building the DLL, export the functions
		#define ZLIBRARY_API __declspec(dllexport)
	#else
        // If we are using the DLL, import the functions
		#define ZLIBRARY_API __declspec(dllimport)
	#endif
#else
	#define ZLIBRARY_API
#endif


#endif //EXPORT_HPP
