#include "dpi.hpp"

#ifdef _WIN32
    #include <windows.h>

    unsigned int getWindowsDPI()
    {
       const HDC hDC = GetDC(NULL);
       const unsigned int dpix = GetDeviceCaps(hDC, LOGPIXELSX);
       ReleaseDC(NULL, hDC);
       return dpix;
    }

    float getWindowsScaleFactor()
    {
       return getWindowsDPI() / 96.f;
    }
#endif//_WIN32