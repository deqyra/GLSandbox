#ifndef DPI_HPP
#define DPI_HPP

#ifdef _WIN32
    unsigned int getWindowsDPI();
    float getWindowsScaleFactor();
#endif//_WIN32

#endif//DPI_HPP