#ifndef RENDERBOI__WINDOW__MONITOR_HPP
#define RENDERBOI__WINDOW__MONITOR_HPP

#include <memory>
#include <string>
#include <vector>

namespace Renderboi::Window
{

/// @brief Class to represent a monitor on the system. All methods of this class 
/// must only be called from the main thread.
class Monitor
{
private:
    /// @brief Instance count used as a rolling ID system.
    static unsigned int _count;

public:
    Monitor();

    virtual ~Monitor();

    /// @brief Data structure describing a video mode for a monitor.
    struct VideoMode
    {
        int width;
        int height;
        int redBits;
        int greenBits;
        int blueBits;
        int refreshRate;
    };

    /// @brief Data structure describing the gamma ramp of a monitor.
    class GammaRamp
    {
    private:
        /// @brief Size component of the gamma ramp.
        unsigned int _size;
        
        /// @brief Red component of the gamma ramp.
        std::vector<unsigned short> _red;
        
        /// @brief Green component of the gamma ramp.
        std::vector<unsigned short> _green;
        
        /// @brief Blue component of the gamma ramp.
        std::vector<unsigned short> _blue;
        
    public:
        /// @param size Size of the gamma ramp.
        GammaRamp(unsigned int size);

        /// @param size Size of the gamma ramp.
        /// @param ramp Ramp to use for all three RGB components of the final
        /// gamma ramp.
        GammaRamp(unsigned int size, std::vector<unsigned short> ramp);

        /// @param size Size of the gamma ramp.
        /// @param red Ramp for the red component of the final gamma ramp.
        /// @param green Ramp for the green component of the final gamma ramp.
        /// @param blue Ramp for the blue component of the final gamma ramp.
        GammaRamp(
            unsigned int size,
            std::vector<unsigned short> red,
            std::vector<unsigned short> green,
            std::vector<unsigned short> blue
        );

        /// @brief Get the size of the gamma ramp.
        ///
        /// @return The size of the gamma ramp.
        unsigned int getSize() const;

        /// @brief Get the red component of the gamma ramp.
        ///
        /// @return The red component of the gamma ramp.
        const std::vector<unsigned short>& getRed() const;

        /// @brief Set the red component of the gamma ramp.
        ///
        /// @param red The red component to set for the gamma ramp.
        void setRed(std::vector<unsigned short> red);

        /// @brief Get the green component of the gamma ramp.
        ///
        /// @return The green component of the gamma ramp.
        const std::vector<unsigned short>& getGreen() const;

        /// @brief Set the green component of the gamma ramp.
        ///
        /// @param green The green component to set for the gamma ramp.
        void setGreen(std::vector<unsigned short> green);

        /// @brief Get the blue component of the gamma ramp.
        ///
        /// @return The blue component of the gamma ramp.
        const std::vector<unsigned short>& getBlue() const;

        /// @brief Set the blue component of the gamma ramp.
        ///
        /// @param blue The blue component to set for the gamma ramp.
        void setBlue(std::vector<unsigned short> blue);

        /// @brief Generate a gamma ramp from a single exponent.
        ///
        /// @param size Size of the ramp to generate.
        /// @param e Exponent to compute the gamma ramp from.
        ///
        /// @return The generated gamma ramp.
        static GammaRamp FromExponent(unsigned int size, float e);

        /// @brief Generate a gamma ramp from a single exponent.
        ///
        /// @param size Size of the ramp to generate.
        /// @param eRed Exponent to compute the red ramp from.
        /// @param eGreen Exponent to compute the green ramp from.
        /// @param eBlue Exponent to compute the blue ramp from.
        ///
        /// @return The generated gamma ramp.
        static GammaRamp FromExponents(unsigned int size, float eRed, float eGreen, float eBlue);
    };

    /// @brief Get the current video mode of the monitor.
    ///
    /// @return The current video mode of the monitor.
    virtual VideoMode getCurrentVideoMode() const = 0;

    /// @brief Get the video modes supported by the monitor.
    ///
    /// @return An array filled with the video modes support by the monitor.
    virtual std::vector<VideoMode> getVideoModes() const = 0;

    /// @brief Get the physical size of the monitor in millimetres.
    ///
    /// @param[out] width_mm To receive the width of the monitor.
    /// @param[out] height_mm To receive the height of the monitor.
    virtual void getPhysicalSize(int& width_mm, int& height_mm) const = 0;

    /// @brief Get the content scale of the monitor.
    ///
    /// @param[out] xscale To receive the X content scale of the monitor.
    /// @param[out] yscale To receive the Y content scale of the monitor.
    virtual void getContentScale(float& xscale, float& yscale) const = 0;

    /// @brief Get the virtual position of the monitor in screen coordinates
    /// (which are not necessarily 1:1 to pixels).
    ///
    /// @param[out] xpos To receive the X position of the monitor.
    /// @param[out] ypos To receive the Y position of the monitor.
    virtual void getPosition(int& xpos, int& ypos) const = 0;

    /// @brief Get the work area of the monitor, that is the area of the monitor
    /// which is not occupied by system widgets like task bars and such. The 
    /// coordinates of the area are screen coordinates.
    ///
    /// @param[out] xpos To receive the X position of the work area.
    /// @param[out] ypos To receive the Y position of the work area.
    /// @param[out] width To receive the width of the work area.
    /// @param[out] height To receive the height of the work area.
    virtual void getWorkArea(int& xscale, int& yscale, int& width, int& height) const = 0;

    /// @brief Get the human-readable name of the monitor.
    ///
    /// @return The human-readable name of the monitor.
    virtual std::string getName() const = 0;

    /// @brief Get the gamma ramp of the monitor.
    ///
    /// @return The gamma ramp of the monitor.
    virtual GammaRamp getGammaRamp() const = 0;

    /// @brief Set the gamma ramp of the monitor. The size of the ramp 
    ///
    /// @return The gamma ramp to set for the monitor.
    virtual void setGammaRamp(const GammaRamp& gammaRamp) const = 0;

    /// @brief Get the largest video mode supported by the monitor.
    ///
    /// @return The largest video mode supported by the monitor.
    virtual VideoMode getLargestVideoMode() const = 0;

    /// @brief ID of the monitor.
    const unsigned int id;
};

using MonitorPtr = std::shared_ptr<Monitor>;

}//namespace Renderboi::Window

#endif//RENDERBOI__WINDOW__MONITOR_HPP