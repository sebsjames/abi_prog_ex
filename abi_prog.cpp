/*
 * Our example third party library has to be included before import std;, as we otherwise see an
 * error:
 *
 * In file included from /home/seb/src/abi_prog_ex/abi_prog.cpp:6:
 * In file included from /usr/local/include/abi_ex/abi_ex.h:3:
 * In file included from /usr/lib/llvm-20/bin/../include/c++/v1/vector:312:
 * In file included from /usr/lib/llvm-20/bin/../include/c++/v1/__vector/comparison.h:12:
 * In file included from /usr/lib/llvm-20/bin/../include/c++/v1/__algorithm/equal.h:17:
 * In file included from /usr/lib/llvm-20/bin/../include/c++/v1/__iterator/distance.h:14:
 * /usr/lib/llvm-20/bin/../include/c++/v1/__iterator/concepts.h:248:11: error: requires clause differs in template redeclaration
 *   248 |   requires(indirectly_readable<_Its> && ...) && invocable<_Fp, iter_reference_t<_Its>...>
 *       |           ^
 * /usr/lib/llvm-20/bin/../include/c++/v1/__iterator/concepts.h:248:11: note: previous template declaration is here
 *   248 |   requires(indirectly_readable<_Its> && ...) && invocable<_Fp, iter_reference_t<_Its>...>
 *       |           ^
 * 1 error generated.
 *
 *
 * However, we can place
 * #include <vector>
 * #include <iostream>
 *
 * before #include <abi_ex.h>
 * (and get rid of import std;)
 */
#include <abi_ex.h>

#include "MulticamScene.h"
#include "libEyeRenderer.h"

import std; // And a modules build of some maths

import sm.random;
import sm.vvec;

extern MulticamScene* scene;

int main()
{
    // Boilerplate memory alloc for compound-ray
    multicamAlloc();

    std::int32_t ncam = static_cast<std::int32_t>(getCameraCount());

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

    stop(); // stop compound-ray from running
    multicamDealloc(); // De-allocate compound-ray memory
}
