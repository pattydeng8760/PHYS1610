#include "initial_boundary_condition.h"
#include <cmath>
#include <rarray>

// Applying the periodic boundary conditions to the solution vector u
void boundary_condition(rvector<double>& u){
    u[0] = u[u.size()-1] ;
}

// Applying the initial condition to the solution vector u at time t = 0
void initial_condition(rvector<double>& u, const double A, const double L){
    for (int i = 0; i < u.size(); i++) {
        double x = L * i / (u.size() - 1);
        u[i] = A*std::pow(sin(M_PI*x/L),100);  // initial condition u(x,0) = A*sin(pi*x)^2
    }
}
