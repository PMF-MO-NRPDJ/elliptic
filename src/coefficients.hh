#ifndef COEFFICIENTS
#define COEFFICIENTS

// Point Dune::FieldVector<double, dim>
template <typename Point>
double exact(Point const & globalPoint){
    auto dim = Point::dimension;
    double x = globalPoint[0];
    if(dim == 1){
        return x*x*x;
    }
    else if(dim > 1){
        double y = globalPoint[1];
        if(dim == 2)
        return x*x-y*y*y;
        else{
            // dim = 3
            double z = globalPoint[2];
            return x*y*z;
        }
    }
}

#endif
