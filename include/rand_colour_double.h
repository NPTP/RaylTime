#include <random>

std::uniform_real_distribution<double> urd(0.0, 1.0);
std::default_random_engine dre;

/*
Global random colour generator (globalized to prevent repeated numbers)
*/
double G_rand_colour_double()
{
    return urd(dre);
}