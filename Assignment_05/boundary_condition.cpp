#include "boundary_condition.h"
#include <cmath>
#include <rarray>

// Applying the boundary conditions to the solution vector u
void boundary_condition(rvector<double>& u, const double A, const double time){
    // Applying the boundary conditions to the solution vector u
    u[0] = A*std::pow(sin(time),2);             // boundary condition at x = 0, the left boundary, u(0,t) = A*sin(t)^2
    u[u.size()-1] = 0.0;                    // boundary condition at x = 1, the right boundary, u(L,t) = 0
}

// Constructing the source term f(x,t) = u*(1 - u) for the Fisher-KPP reaction term
void source_term(rvector<double>& f, const rvector<double>& u) {
    for (int i = 0; i < f.size(); i++) {
        f[i] = u[i] * (1.0 - u[i]);  // Applying the Fisher-KPP reaction term
    }
}

