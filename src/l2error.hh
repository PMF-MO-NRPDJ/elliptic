#ifndef L2ERROR_HH_INCLUDED
#define L2ERROR_HH_INCLUDED

#include <cmath>

template <typename GV, typename DGF>
double l2_norm(GV const & gv, DGF const & dgf, int intorder = 3){
     double norm2 = 0.0;  // int_{\Omega} f^2 dx
     // integracijska formula
     const int dim = GV::dimension;
     for(auto elem : elements(gv)){
        // int_K f^2 dx
        double loc_integral =0.0;
        auto gt = elem.geometry().type();
        auto& rule = Dune::QuadratureRules<double,dim>::rule(gt,intorder);

        for (auto qpoint : rule){
            Dune::FieldVector<double, 1> y;
            dgf.evaluate(elem, qpoint.position(), y);
            double y2 = y*y;
            double factor = qpoint.weight()
                      * elem.geometry().integrationElement(qpoint.position());
            loc_integral += y2 * factor;
        }
        norm2 += loc_integral;
     }

     return std::sqrt(norm2);
}

#endif
