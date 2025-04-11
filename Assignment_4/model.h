#ifndef MODELH
#define MODELH

#include <rarray>

struct ModelParameters
{
    double alpha;  // friction constant
    double g;      // gravitation accelleration
    double v0;     // initial (vertical) velocity
    double z0;     // initial height
};

double z(double t, const ModelParameters& p);

double v(double t, const ModelParameters& p);

rvector<double> compute_model_v(double t1, double t2, double dt,
                               const ModelParameters& p);

rvector<double> compute_model_z(double t1, double t2, double dt,
                               const ModelParameters& p);

#endif
