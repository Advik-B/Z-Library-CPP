#ifndef ZLIBRARY_EXPORT_H
#define ZLIBRARY_EXPORT_H

#ifdef _WIN32 // Windows specific
    #ifdef ZLIBRARY_STATIC // Building or using the static lib
        #define ZLIBRARY_API
    #else // Building or using the shared lib
        #ifdef ZLIBRARY_EXPORTS // Building the shared lib (DLL)
            #define ZLIBRARY_API __declspec(dllexport)
        #else // Using the shared lib (DLL)
            #define ZLIBRARY_API __declspec(dllimport)
        #endif
    #endif
#else // Non-windows
    #define ZLIBRARY_API
#endif

#endif // ZLIBRARY_EXPORT_H