#include "INITM.hpp"
#include "INITL.hpp"

#include "LaplaceCartesian.hpp"
#include "LaplaceSpherical.hpp"
#include "YukawaCartesian.hpp"

#include "Math.hpp"

#include <iostream>

template <typename Kernel>
void two_level_test(const Kernel& K)
{
  typedef Kernel kernel_type;
  typedef typename kernel_type::point_type point_type;
  typedef typename kernel_type::source_type source_type;
  typedef typename kernel_type::target_type target_type;
  typedef typename kernel_type::charge_type charge_type;
  typedef typename kernel_type::result_type result_type;
  typedef typename kernel_type::multipole_type multipole_type;
  typedef typename kernel_type::local_type local_type;

  // init source
  source_type s = source_type(0,0,0);

  // init charge
  charge_type c = 1;

  // init target
  target_type t = target_type(1,0,0);

  // init results vectors for exact, FMM
  result_type rexact;
  result_type rm2p;
  result_type rfmm;

  // test direct
  rexact = K(t,s) * c;

  // setup intial multipole expansion
  multipole_type M;
  point_type M_center(0.05, 0.05, 0.05);
  INITM::eval(K, M, 0.10);
  K.P2M(s, c, M_center, M);

  // perform M2M
  multipole_type M2;
  point_type M2_center(0.1, 0.1, 0.1);
  INITM::eval(K, M2, 0.2);
  K.M2M(M, M2, M2_center - M_center);

  // test M2P
  K.M2P(M, M2_center, t, rm2p);

  // test M2L, L2P
  local_type L2;
  point_type L2_center(0.9, 0.1, 0.1);
  INITL::eval(K, L2, 0.2);
  K.M2L(M2, L2, L2_center - M2_center);

  // test L2L
  local_type L;
  point_type L_center(0.95, 0.05, 0.05);
  INITL::eval(K, L, 0.1);
  K.L2L(L2, L, L_center - L2_center);

  // test L2P
  K.L2P(L, L_center, t, rfmm);

  // check errors
  std::cout << "rexact = " << rexact << std::endl;
  std::cout << "rm2p = " << rm2p << std::endl;
  std::cout << "rfmm = " << rfmm << std::endl;

  std::cout << "M2P L1 rel error: "
            << std::scientific << l1_rel_error(rm2p, rexact) << std::endl;
  std::cout << "M2P L2 error:     "
            << std::scientific << l2_error(rm2p, rexact) << std::endl;
  std::cout << "M2P L2 rel error: "
            << std::scientific << l2_rel_error(rm2p, rexact) << std::endl;

  std::cout << "FMM L1 rel error: "
            << std::scientific << l1_rel_error(rfmm, rexact) << std::endl;
  std::cout << "FMM L2 error:     "
            << std::scientific << l2_error(rfmm, rexact) << std::endl;
  std::cout << "FMM L2 rel error: "
            << std::scientific << l2_rel_error(rfmm, rexact) << std::endl;
}


int main(int argc, char **argv)
{
  (void) argc;
  (void) argv;

  //LaplaceCartesian<5> K;
  LaplaceSpherical K(5);
  //YukawaCartesian K(5, 0);

  two_level_test(K);
}

