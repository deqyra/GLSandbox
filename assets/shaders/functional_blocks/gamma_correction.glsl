#ifndef FUNCTIONAL_BLOCKS__GAMMA_CORRECTION
#define FUNCTIONAL_BLOCKS__GAMMA_CORRECTION

vec4 gammaCorrect(vec4 color, float gamma)
{
    return vec4(pow(color.rgb, vec3(1.0 / gamma)), color.a);
}

#endif//FUNCTIONAL_BLOCKS__GAMMA_CORRECTION