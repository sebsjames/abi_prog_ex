#include <abi_ex.h> // Our example third party library

import std; // And a modules build of some maths
import sm.random;
import sm.vvec;

int main()
{
    sm::rand_vonmises<double> rng_vonmises(0, 3); // mu = 0, kappa = 3
    double s = rng_vonmises.get();
    std::cout << "sample from the von Mises distribution: " << s << std::endl;

    sm::vvec<double> x, y;
    x.arange (-6.0, 6.0, 0.1);
    for (auto _x : x) {
        y.push_back (rng_vonmises.prob_density (_x));
    }

    std::cout << "Von Mises PDF:\n";
    std::cout << x.str_mat() << std::endl;
    std::cout << y.str_mat() << std::endl;

    std::vector<float> vf = abi_ex::make_a_vector();
    for (auto ve : vf) {
        std::cout << "vector element: " << ve << std::endl;
    }
}
