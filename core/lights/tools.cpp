#include "tools.hpp"

#include <functional>

namespace
{
    /**
     * Light attenuation calculation factors depending on the desired range of
     * a light:
     * (http://wiki.ogre3d.org/tiki-index.php?page=-Point+Light+Attenuation)
     * 
     * Distance 	Constant 	Linear 	Quadratic
     * ==========================================
     * 7            1.0         0.7     1.8
     * 13           1.0         0.35    0.44
     * 20           1.0         0.22    0.20
     * 32           1.0         0.14    0.07
     * 50           1.0         0.09    0.032
     * 65           1.0         0.07    0.017
     * 100          1.0         0.045   0.0075
     * 160          1.0         0.027   0.0028
     * 200          1.0         0.022   0.0019
     * 325          1.0         0.014   0.0007
     * 600          1.0         0.007   0.0002
     * 3250         1.0         0.0014  0.000007
     * 
     * Factors involved in calculating the attenuation of a given light depend
     * on the range of said light.
     * 
     * The parameters defined hereafter are used to calculate the _actual_ 
     * attenuation factors, and were computed to best fit the previous data.
     **/

    constexpr float QuadraticComponentInQuadraticFactor = (float)(0.0108699);
    constexpr float LinearComponentInQuadraticFactor = (float)(0.066653);
    constexpr float ConstantComponentInQuadraticFactor = (float)(-0.443622);

    constexpr float LinearComponentInLinearFactor = (float)(0.234686);
    constexpr float ConstantComponentInLinearFactor = (float)(-0.212775);

    const std::function<float(float)> quadraticFactorForRange = [](float range)
    {
        float quadratic = QuadraticComponentInQuadraticFactor * range * range;
        float linear = LinearComponentInQuadraticFactor * range;
        return (float)(1.f / (quadratic + linear + ConstantComponentInQuadraticFactor));
    };

    const std::function<float(float)> linearFactorForRange = [](float range)
    {
        float linear = LinearComponentInLinearFactor * range;
        return (float)(1.f / (linear + ConstantComponentInLinearFactor));
    };
}

void attenuationFromRange(float range, float& quadraticFactor, float& linearFactor, float& constant)
{
    quadraticFactor = quadraticFactorForRange(range);
    linearFactor = linearFactorForRange(range);
    constant = 1.f;
}