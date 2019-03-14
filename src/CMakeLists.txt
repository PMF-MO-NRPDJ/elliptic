add_executable("elliptic" elliptic.cc driver.hh operator.hh
               bctype.hh coefficients.hh)
target_link_dune_default_libraries("elliptic")
