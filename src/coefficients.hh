#ifndef COEFFICIENTS
#define COEFFICIENTS

#include <dune/pdelab/common/function.hh>

// Point Dune::FieldVector<double, dim>
template <typename Point>
double exact(Point const & globalPoint){
    auto dim = Point::dimension;
    assert(dim == 2);
    double x = globalPoint[0];
    double y = globalPoint[1];
    return x*x-y*y*y;
}

template <typename Point>
Point grad_exact(Point const & globalPoint){
    auto dim = Point::dimension;
    assert(dim == 2);
    double x = globalPoint[0];
    double y = globalPoint[1];
    Point tmp;
    tmp[0] = 2*x;
    tmp[1] = -3*y*y;
    return tmp;
}

template <typename Point>
double laplace_exact(Point const & globalPoint){
    auto dim = Point::dimension;
    assert(dim == 2);
    double x = globalPoint[0];
    double y = globalPoint[1];
    return 2-6*y;
}

template <typename Point>
double fun_a(Point const & globalPoint){
    auto dim = Point::dimension;
    assert(dim == 2);
    double x = globalPoint[0];
    double y = globalPoint[1];
    return 1.0+x*x + y*y;
}

template <typename Point>
Point grad_fun_a(Point const & globalPoint){
    auto dim = Point::dimension;
    assert(dim == 2);
    double x = globalPoint[0];
    double y = globalPoint[1];
    Point tmp;
    tmp[0] = 2*x;
    tmp[1] = 2*y;
    return tmp;
}

template <typename Point>
double fun_b(Point const & globalPoint){
    auto dim = Point::dimension;
    assert(dim == 2);
    double x = globalPoint[0];
    double y = globalPoint[1];
    return 1.0+std::sin(M_PI*x*y);
}
// -div(a grad u) + b u = -a div grad u - grad a . grad u + b u = f
template <typename Point>
double fun_f(Point const & globalPoint){
    auto dim = Point::dimension;
    assert(dim == 2);

    double tmp = -fun_a(globalPoint)*laplace_exact(globalPoint)
                 - (grad_fun_a(globalPoint) * grad_exact(globalPoint))
                 + fun_b(globalPoint) * exact(globalPoint);
    return tmp;
}

template <typename GV>
class ExactSolution : public Dune::PDELab::AnalyticGridFunctionBase<
      Dune::PDELab::AnalyticGridFunctionTraits<GV, double, 1>,
      ExactSolution<GV> > {
public:
      typedef Dune::PDELab::AnalyticGridFunctionTraits<GV, double, 1> Traits;

      ExactSolution(GV const & gv) :
             Dune::PDELab::AnalyticGridFunctionBase<Traits,ExactSolution<GV> > (gv) {}

      void 	evaluate (const typename Traits::ElementType &e,
                      const typename Traits::DomainType &x,
                      typename Traits::RangeType &y) const
      {
          auto xglobal = e.geometry().global(x);
          y = exact(xglobal);
      }

};


#endif
