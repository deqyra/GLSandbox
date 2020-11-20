float attenuate(float constant, float linear, float quadratic, float dist)
{
	return 1.f / (  constant
				 + (linear    * dist)
				 + (quadratic * dist * dist)
				 );
}
