#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <array>
#include <bitset>

#include <dune/common/fvector.hh>
#include <dune/common/parallel/mpihelper.hh>
#include <dune/grid/yaspgrid.hh>

#include "driver.hh"

//===============================================================
// Main program with grid setup
//===============================================================

int main(int argc, char** argv)
{
  Dune::MPIHelper::instance(argc, argv);

  int level = 0;
  if (argc > 1)
		 level = std::stoi(argv[1]);

  constexpr int dim = 2;
	// sekvencijalna verzija -- kreiraj Grid
  Dune::FieldVector<double,dim> L(1.0);
  std::array<int,dim>           N{10,10};
  Dune::YaspGrid<dim> grid(L,N);

  grid.globalRefine(level);
  const auto& gv=grid.leafGridView();
  driver(gv);

  return 0;
}
