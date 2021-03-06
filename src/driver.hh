#ifndef DRIVER_HH
#define DRIVER_HH

#include <dune/istl/bvector.hh>
#include <dune/istl/io.hh>
#include <dune/istl/operators.hh>
#include <dune/istl/preconditioners.hh>
#include <dune/istl/solvers.hh>
#include <dune/istl/superlu.hh>

#include <dune/pdelab/backend/istl.hh>
#include <dune/pdelab/common/function.hh>
#include <dune/pdelab/common/vtkexport.hh>
#include <dune/pdelab/constraints/conforming.hh>
#include <dune/pdelab/finiteelementmap/qkfem.hh>
#include <dune/pdelab/gridfunctionspace/genericdatahandle.hh>
#include <dune/pdelab/gridfunctionspace/gridfunctionspace.hh>
#include <dune/pdelab/gridfunctionspace/gridfunctionspaceutilities.hh>
#include <dune/pdelab/gridfunctionspace/interpolate.hh>
#include <dune/pdelab/gridoperator/gridoperator.hh>
#include <dune/pdelab/stationary/linearproblem.hh>

#include <dune/grid/io/file/gmshreader.hh>
#include <dune/grid/io/file/vtk/subsamplingvtkwriter.hh>

#include "bctype.hh"
#include "operator.hh"
#include "l2error.hh"

#include <memory>

template<class GV>
void driver(const GV& gv)
{
  using namespace Dune::PDELab;  // Da skratimo imena
//  typedef typename GV::Grid::ctype Coord;

  // Prostor konačnih elemenata (grid function space) i GridOperator.
  const int k = 1;
  typedef QkLocalFiniteElementMap<GV,double,double,k>                FEM;
  //  NoConstraints
  typedef ConformingDirichletConstraints                            CONSTRAINTS;
  typedef ISTL::VectorBackend<>                                     VBE;
  typedef GridFunctionSpace<GV,FEM,CONSTRAINTS,VBE>                 GFS;
  typedef typename GFS::template ConstraintsContainer<double>::Type CC;
  typedef DiffusionLocalOperator<DirichletBdry, FEM>                LOP;
  typedef ISTL::BCRSMatrixBackend<>                                 MBE;
  typedef GridOperator<
    GFS,GFS,              /* prostor KE rješenja i test funkcije */
    LOP,                  /* lokalni operator */
    MBE,                  /* matrix backend */
    double,double,double, /* tipovi u domeni, slici i jakobijanu */
    CC,CC                 /* ograničenja za prostor rješenja i test funkcija. */
    > GO;

  FEM fem(gv);
  GFS gfs(gv,fem);
  CC cc;
  DirichletBdry bctype;
  constraints(bctype, gfs, cc); // asembliranje ograničenja Dirichletovog tipa
  std::cout << "constrained dofs=" << cc.size() << " of " << gfs.globalSize() << std::endl;
  LOP lop(bctype, fem);
  MBE mbe(9);  // traži prosječan broj ne-nul elemenata u redu (=9)
  GO go(gfs,cc,gfs,cc,lop,mbe);

  // <<<2>>> Konstrukcija rješavača.
  typedef typename GO::Traits::Domain            U;
  typedef BCExtension<GV,double>                 G;
  typedef ISTLBackend_SEQ_BCGS_SSOR              LS;
  typedef StationaryLinearProblemSolver<GO,LS,U> SLP;

  U u(gfs,0.0), u_exact(gfs, 0.0), u_error(gfs, 0.0);
  G g(gv);
  interpolate(g,gfs,u);
  LS ls(5000,true);        // max 5000 iteracija, verbosity = true
  SLP slp(go,ls,u,1e-10);  // redukcija = 1e-10
  slp.apply();

  // <<<3>>> grafički izlaz (VTK)
  typedef DiscreteGridFunction<GFS,U> DGF;

  DGF udgf(gfs,u);
  ExactSolution<GV> exact_dgf(gv);
  interpolate(exact_dgf, gfs, u_exact);
  u_error = u;
  u_error -= u_exact;
  DGF errordgf(gfs, u_error);

  std::cout << "L2 greška = " << l2_norm(gv, errordgf) << std::endl;

  // k je stupanj polinoma u konačnom elementu. Za k=1 nema profinjenja;
  // Za k=2 svaka se stranica elementa dijeli na dvije itd.
  Dune::SubsamplingVTKWriter<GV> vtkwriter(gv,Dune::refinementIntervals(1));
  vtkwriter.addVertexData(
    std::make_shared<VTKGridFunctionAdapter<DGF>>(udgf,"u"));
  vtkwriter.addVertexData(
    std::make_shared<VTKGridFunctionAdapter<DGF>>(errordgf,"error"));
  vtkwriter.addVertexData(
    std::make_shared<VTKGridFunctionAdapter< ExactSolution<GV> >>
                                                 (exact_dgf ,"exact"));
  vtkwriter.write("simple", Dune::VTK::ascii); //Dune::VTK::appendedraw);
}



#endif
