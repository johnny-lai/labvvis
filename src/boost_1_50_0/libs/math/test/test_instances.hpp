// Copyright John Maddock 2011.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)


namespace boost{ namespace math{

   typedef policies::policy<policies::overflow_error<policies::throw_on_error> > overflow_policy;

   // Beta functions.
   template tools::promote_args<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE>::type 
         beta(BOOST_MATH_TEST_TYPE a, BOOST_MATH_TEST_TYPE b); // Beta function (2 arguments).

   template tools::promote_args<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE>::type 
         beta(BOOST_MATH_TEST_TYPE a, BOOST_MATH_TEST_TYPE b, policies::policy<>); // Beta function (3 arguments).
   template tools::promote_args<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE>::type 
         beta(BOOST_MATH_TEST_TYPE a, BOOST_MATH_TEST_TYPE b, BOOST_MATH_TEST_TYPE); // Beta function (3 arguments).

   template tools::promote_args<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE>::type 
         beta(BOOST_MATH_TEST_TYPE a, BOOST_MATH_TEST_TYPE b, BOOST_MATH_TEST_TYPE x, const policies::policy<>& pol); // Beta function (3 arguments).

   template tools::promote_args<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE>::type 
         betac(BOOST_MATH_TEST_TYPE a, BOOST_MATH_TEST_TYPE b, BOOST_MATH_TEST_TYPE x);

   template tools::promote_args<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE>::type 
         betac(BOOST_MATH_TEST_TYPE a, BOOST_MATH_TEST_TYPE b, BOOST_MATH_TEST_TYPE x, const policies::policy<>& pol);

   template tools::promote_args<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE>::type 
         ibeta(BOOST_MATH_TEST_TYPE a, BOOST_MATH_TEST_TYPE b, BOOST_MATH_TEST_TYPE x); // Incomplete beta function.

   template tools::promote_args<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE>::type 
         ibeta(BOOST_MATH_TEST_TYPE a, BOOST_MATH_TEST_TYPE b, BOOST_MATH_TEST_TYPE x, const policies::policy<>& pol); // Incomplete beta function.

   template tools::promote_args<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE>::type 
         ibetac(BOOST_MATH_TEST_TYPE a, BOOST_MATH_TEST_TYPE b, BOOST_MATH_TEST_TYPE x); // Incomplete beta complement function.

   template tools::promote_args<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE>::type 
         ibetac(BOOST_MATH_TEST_TYPE a, BOOST_MATH_TEST_TYPE b, BOOST_MATH_TEST_TYPE x, const policies::policy<>& pol); // Incomplete beta complement function.

   template tools::promote_args<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE>::type  
         ibeta_inv(BOOST_MATH_TEST_TYPE a, BOOST_MATH_TEST_TYPE b, BOOST_MATH_TEST_TYPE p, BOOST_MATH_TEST_TYPE* py);

   template tools::promote_args<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE>::type  
         ibeta_inv(BOOST_MATH_TEST_TYPE a, BOOST_MATH_TEST_TYPE b, BOOST_MATH_TEST_TYPE p, BOOST_MATH_TEST_TYPE* py, const policies::policy<>& pol);

   template tools::promote_args<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE>::type 
         ibeta_inv(BOOST_MATH_TEST_TYPE a, BOOST_MATH_TEST_TYPE b, BOOST_MATH_TEST_TYPE p); // Incomplete beta inverse function.

   template tools::promote_args<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE>::type 
         ibeta_inv(BOOST_MATH_TEST_TYPE a, BOOST_MATH_TEST_TYPE b, BOOST_MATH_TEST_TYPE p, const policies::policy<>&); // Incomplete beta inverse function.

   template tools::promote_args<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE>::type 
         ibeta_inva(BOOST_MATH_TEST_TYPE a, BOOST_MATH_TEST_TYPE b, BOOST_MATH_TEST_TYPE p); // Incomplete beta inverse function.

   template tools::promote_args<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE>::type 
         ibeta_inva(BOOST_MATH_TEST_TYPE a, BOOST_MATH_TEST_TYPE b, BOOST_MATH_TEST_TYPE p, const policies::policy<>&); // Incomplete beta inverse function.

   template tools::promote_args<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE>::type 
         ibeta_invb(BOOST_MATH_TEST_TYPE a, BOOST_MATH_TEST_TYPE b, BOOST_MATH_TEST_TYPE p); // Incomplete beta inverse function.

   template tools::promote_args<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE>::type 
         ibeta_invb(BOOST_MATH_TEST_TYPE a, BOOST_MATH_TEST_TYPE b, BOOST_MATH_TEST_TYPE p, const policies::policy<>&); // Incomplete beta inverse function.

   template tools::promote_args<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE>::type 
         ibetac_inv(BOOST_MATH_TEST_TYPE a, BOOST_MATH_TEST_TYPE b, BOOST_MATH_TEST_TYPE q, BOOST_MATH_TEST_TYPE* py);

   template tools::promote_args<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE>::type 
         ibetac_inv(BOOST_MATH_TEST_TYPE a, BOOST_MATH_TEST_TYPE b, BOOST_MATH_TEST_TYPE q, BOOST_MATH_TEST_TYPE* py, const policies::policy<>& pol);

   template tools::promote_args<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE>::type 
         ibetac_inv(BOOST_MATH_TEST_TYPE a, BOOST_MATH_TEST_TYPE b, BOOST_MATH_TEST_TYPE q); // Incomplete beta complement inverse function.

   template tools::promote_args<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE>::type 
         ibetac_inv(BOOST_MATH_TEST_TYPE a, BOOST_MATH_TEST_TYPE b, BOOST_MATH_TEST_TYPE q, const policies::policy<>&); // Incomplete beta complement inverse function.

   template tools::promote_args<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE>::type 
         ibetac_inva(BOOST_MATH_TEST_TYPE a, BOOST_MATH_TEST_TYPE b, BOOST_MATH_TEST_TYPE q); // Incomplete beta complement inverse function.

   template tools::promote_args<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE>::type 
         ibetac_inva(BOOST_MATH_TEST_TYPE a, BOOST_MATH_TEST_TYPE b, BOOST_MATH_TEST_TYPE q, const policies::policy<>&); // Incomplete beta complement inverse function.

   template tools::promote_args<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE>::type 
         ibetac_invb(BOOST_MATH_TEST_TYPE a, BOOST_MATH_TEST_TYPE b, BOOST_MATH_TEST_TYPE q); // Incomplete beta complement inverse function.

   template tools::promote_args<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE>::type 
         ibetac_invb(BOOST_MATH_TEST_TYPE a, BOOST_MATH_TEST_TYPE b, BOOST_MATH_TEST_TYPE q, const policies::policy<>&); // Incomplete beta complement inverse function.

   template tools::promote_args<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE>::type 
         ibeta_derivative(BOOST_MATH_TEST_TYPE a, BOOST_MATH_TEST_TYPE b, BOOST_MATH_TEST_TYPE x);  // derivative of incomplete beta

   template tools::promote_args<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE>::type 
         ibeta_derivative(BOOST_MATH_TEST_TYPE a, BOOST_MATH_TEST_TYPE b, BOOST_MATH_TEST_TYPE x, const policies::policy<>& pol);  // derivative of incomplete beta

   // erf & erfc error functions.
   template tools::promote_args<BOOST_MATH_TEST_TYPE>::type erf(BOOST_MATH_TEST_TYPE z);
   template tools::promote_args<BOOST_MATH_TEST_TYPE>::type erf(BOOST_MATH_TEST_TYPE z, const policies::policy<>&);

   template tools::promote_args<BOOST_MATH_TEST_TYPE>::type erfc(BOOST_MATH_TEST_TYPE z);
   template tools::promote_args<BOOST_MATH_TEST_TYPE>::type erfc(BOOST_MATH_TEST_TYPE z, const policies::policy<>&);

   template tools::promote_args<BOOST_MATH_TEST_TYPE>::type erf_inv(BOOST_MATH_TEST_TYPE z);
   template tools::promote_args<BOOST_MATH_TEST_TYPE>::type erf_inv(BOOST_MATH_TEST_TYPE z, const policies::policy<>& pol);

   template tools::promote_args<BOOST_MATH_TEST_TYPE>::type erfc_inv(BOOST_MATH_TEST_TYPE z);
   template tools::promote_args<BOOST_MATH_TEST_TYPE>::type erfc_inv(BOOST_MATH_TEST_TYPE z, const policies::policy<>& pol);

   // Polynomials:
   template tools::promote_args<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE>::type 
         legendre_next(unsigned l, BOOST_MATH_TEST_TYPE x, BOOST_MATH_TEST_TYPE Pl, BOOST_MATH_TEST_TYPE Plm1);

   template tools::promote_args<BOOST_MATH_TEST_TYPE>::type 
         legendre_p(int l, BOOST_MATH_TEST_TYPE x);

   template tools::promote_args<BOOST_MATH_TEST_TYPE>::type 
         legendre_p(int l, BOOST_MATH_TEST_TYPE x, const policies::policy<>& pol);

   template tools::promote_args<BOOST_MATH_TEST_TYPE>::type 
         legendre_q(unsigned l, BOOST_MATH_TEST_TYPE x);

   template tools::promote_args<BOOST_MATH_TEST_TYPE>::type 
         legendre_q(unsigned l, BOOST_MATH_TEST_TYPE x, const policies::policy<>& pol);

   template tools::promote_args<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE>::type 
         legendre_next(unsigned l, unsigned m, BOOST_MATH_TEST_TYPE x, BOOST_MATH_TEST_TYPE Pl, BOOST_MATH_TEST_TYPE Plm1);

   template tools::promote_args<BOOST_MATH_TEST_TYPE>::type 
         legendre_p(int l, int m, BOOST_MATH_TEST_TYPE x);

   template tools::promote_args<BOOST_MATH_TEST_TYPE>::type 
         legendre_p(int l, int m, BOOST_MATH_TEST_TYPE x, const policies::policy<>& pol);

   template tools::promote_args<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE>::type  
         laguerre_next(unsigned n, BOOST_MATH_TEST_TYPE x, BOOST_MATH_TEST_TYPE Ln, BOOST_MATH_TEST_TYPE Lnm1);

   template tools::promote_args<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE>::type  
      laguerre_next(unsigned n, unsigned l, BOOST_MATH_TEST_TYPE x, BOOST_MATH_TEST_TYPE Pl, BOOST_MATH_TEST_TYPE Plm1);

   template tools::promote_args<BOOST_MATH_TEST_TYPE>::type 
      laguerre(unsigned n, BOOST_MATH_TEST_TYPE x);

   template tools::promote_args<BOOST_MATH_TEST_TYPE>::type 
      laguerre(unsigned n, unsigned m, BOOST_MATH_TEST_TYPE x, const policies::policy<>& pol);

   template laguerre_result<int, BOOST_MATH_TEST_TYPE>::type 
      laguerre(unsigned n, int m, BOOST_MATH_TEST_TYPE x);

   template laguerre_result<unsigned, BOOST_MATH_TEST_TYPE>::type 
      laguerre(unsigned n, unsigned m, BOOST_MATH_TEST_TYPE x);

   template tools::promote_args<BOOST_MATH_TEST_TYPE>::type 
      hermite(unsigned n, BOOST_MATH_TEST_TYPE x);

   template tools::promote_args<BOOST_MATH_TEST_TYPE>::type 
      hermite(unsigned n, BOOST_MATH_TEST_TYPE x, const policies::policy<>& pol);

   template tools::promote_args<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE>::type 
      hermite_next(unsigned n, BOOST_MATH_TEST_TYPE x, BOOST_MATH_TEST_TYPE Hn, BOOST_MATH_TEST_TYPE Hnm1);

   template std::complex<BOOST_MATH_TEST_TYPE> 
         spherical_harmonic(unsigned n, int m, BOOST_MATH_TEST_TYPE theta, BOOST_MATH_TEST_TYPE phi);

   template std::complex<BOOST_MATH_TEST_TYPE> 
      spherical_harmonic(unsigned n, int m, BOOST_MATH_TEST_TYPE theta, BOOST_MATH_TEST_TYPE phi, const policies::policy<>& pol);

   template tools::promote_args<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE>::type 
         spherical_harmonic_r(unsigned n, int m, BOOST_MATH_TEST_TYPE theta, BOOST_MATH_TEST_TYPE phi);

   template tools::promote_args<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE>::type 
      spherical_harmonic_r(unsigned n, int m, BOOST_MATH_TEST_TYPE theta, BOOST_MATH_TEST_TYPE phi, const policies::policy<>& pol);

   template tools::promote_args<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE>::type 
         spherical_harmonic_i(unsigned n, int m, BOOST_MATH_TEST_TYPE theta, BOOST_MATH_TEST_TYPE phi);

   template tools::promote_args<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE>::type 
      spherical_harmonic_i(unsigned n, int m, BOOST_MATH_TEST_TYPE theta, BOOST_MATH_TEST_TYPE phi, const policies::policy<>& pol);

    // Elliptic integrals:
   template tools::promote_args<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE>::type 
         ellint_rf(BOOST_MATH_TEST_TYPE x, BOOST_MATH_TEST_TYPE y, BOOST_MATH_TEST_TYPE z);

   template tools::promote_args<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE>::type 
         ellint_rf(BOOST_MATH_TEST_TYPE x, BOOST_MATH_TEST_TYPE y, BOOST_MATH_TEST_TYPE z, const policies::policy<>& pol);

   template tools::promote_args<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE>::type 
         ellint_rd(BOOST_MATH_TEST_TYPE x, BOOST_MATH_TEST_TYPE y, BOOST_MATH_TEST_TYPE z);

   template tools::promote_args<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE>::type 
         ellint_rd(BOOST_MATH_TEST_TYPE x, BOOST_MATH_TEST_TYPE y, BOOST_MATH_TEST_TYPE z, const policies::policy<>& pol);

   template tools::promote_args<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE>::type 
         ellint_rc(BOOST_MATH_TEST_TYPE x, BOOST_MATH_TEST_TYPE y);

   template tools::promote_args<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE>::type 
         ellint_rc(BOOST_MATH_TEST_TYPE x, BOOST_MATH_TEST_TYPE y, const policies::policy<>& pol);

   template tools::promote_args<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE>::type 
         ellint_rj(BOOST_MATH_TEST_TYPE x, BOOST_MATH_TEST_TYPE y, BOOST_MATH_TEST_TYPE z, BOOST_MATH_TEST_TYPE p);

   template tools::promote_args<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE>::type 
         ellint_rj(BOOST_MATH_TEST_TYPE x, BOOST_MATH_TEST_TYPE y, BOOST_MATH_TEST_TYPE z, BOOST_MATH_TEST_TYPE p, const policies::policy<>& pol);

   template tools::promote_args<BOOST_MATH_TEST_TYPE>::type ellint_2(BOOST_MATH_TEST_TYPE k);

   template tools::promote_args<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE>::type ellint_2(BOOST_MATH_TEST_TYPE k, BOOST_MATH_TEST_TYPE phi);

   template tools::promote_args<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE>::type ellint_2(BOOST_MATH_TEST_TYPE k, BOOST_MATH_TEST_TYPE phi, const policies::policy<>& pol);

   template tools::promote_args<BOOST_MATH_TEST_TYPE>::type ellint_1(BOOST_MATH_TEST_TYPE k);

   template tools::promote_args<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE>::type ellint_1(BOOST_MATH_TEST_TYPE k, BOOST_MATH_TEST_TYPE phi);

   template tools::promote_args<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE>::type ellint_1(BOOST_MATH_TEST_TYPE k, BOOST_MATH_TEST_TYPE phi, const policies::policy<>& pol);

   template detail::ellint_3_result<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE>::type ellint_3(BOOST_MATH_TEST_TYPE k, BOOST_MATH_TEST_TYPE v, BOOST_MATH_TEST_TYPE phi);

   template tools::promote_args<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE>::type ellint_3(BOOST_MATH_TEST_TYPE k, BOOST_MATH_TEST_TYPE v, BOOST_MATH_TEST_TYPE phi, const policies::policy<>& pol);

   template tools::promote_args<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE>::type ellint_3(BOOST_MATH_TEST_TYPE k, BOOST_MATH_TEST_TYPE v);

   // Gamma functions.
   template tools::promote_args<BOOST_MATH_TEST_TYPE>::type tgamma(BOOST_MATH_TEST_TYPE z);

   template tools::promote_args<BOOST_MATH_TEST_TYPE>::type tgamma1pm1(BOOST_MATH_TEST_TYPE z);

   template tools::promote_args<BOOST_MATH_TEST_TYPE>::type tgamma1pm1(BOOST_MATH_TEST_TYPE z, const policies::policy<>& pol);

   template tools::promote_args<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE>::type tgamma(BOOST_MATH_TEST_TYPE a, BOOST_MATH_TEST_TYPE z);

   template tools::promote_args<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE>::type tgamma(BOOST_MATH_TEST_TYPE a, BOOST_MATH_TEST_TYPE z, const policies::policy<>& pol);

   template tools::promote_args<BOOST_MATH_TEST_TYPE>::type lgamma(BOOST_MATH_TEST_TYPE z, int* sign);

   template tools::promote_args<BOOST_MATH_TEST_TYPE>::type lgamma(BOOST_MATH_TEST_TYPE z, int* sign, const policies::policy<>& pol);

   template tools::promote_args<BOOST_MATH_TEST_TYPE>::type lgamma(BOOST_MATH_TEST_TYPE x);

   template tools::promote_args<BOOST_MATH_TEST_TYPE>::type lgamma(BOOST_MATH_TEST_TYPE x, const policies::policy<>& pol);

   template tools::promote_args<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE>::type tgamma_lower(BOOST_MATH_TEST_TYPE a, BOOST_MATH_TEST_TYPE z);

   template tools::promote_args<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE>::type tgamma_lower(BOOST_MATH_TEST_TYPE a, BOOST_MATH_TEST_TYPE z, const policies::policy<>&);

   template tools::promote_args<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE>::type gamma_q(BOOST_MATH_TEST_TYPE a, BOOST_MATH_TEST_TYPE z);

   template tools::promote_args<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE>::type gamma_q(BOOST_MATH_TEST_TYPE a, BOOST_MATH_TEST_TYPE z, const policies::policy<>&);

   template tools::promote_args<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE>::type gamma_p(BOOST_MATH_TEST_TYPE a, BOOST_MATH_TEST_TYPE z);

   template tools::promote_args<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE>::type gamma_p(BOOST_MATH_TEST_TYPE a, BOOST_MATH_TEST_TYPE z, const policies::policy<>&);

   template tools::promote_args<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE>::type tgamma_delta_ratio(BOOST_MATH_TEST_TYPE z, BOOST_MATH_TEST_TYPE delta);

   template tools::promote_args<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE>::type tgamma_delta_ratio(BOOST_MATH_TEST_TYPE z, BOOST_MATH_TEST_TYPE delta, const policies::policy<>&);

   template tools::promote_args<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE>::type tgamma_ratio(BOOST_MATH_TEST_TYPE a, BOOST_MATH_TEST_TYPE b);

   template tools::promote_args<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE>::type tgamma_ratio(BOOST_MATH_TEST_TYPE a, BOOST_MATH_TEST_TYPE b, const policies::policy<>&);

   template tools::promote_args<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE>::type gamma_p_derivative(BOOST_MATH_TEST_TYPE a, BOOST_MATH_TEST_TYPE x);

   template tools::promote_args<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE>::type gamma_p_derivative(BOOST_MATH_TEST_TYPE a, BOOST_MATH_TEST_TYPE x, const policies::policy<>&);

   // gamma inverse.
   template tools::promote_args<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE>::type gamma_p_inv(BOOST_MATH_TEST_TYPE a, BOOST_MATH_TEST_TYPE p);

   template tools::promote_args<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE>::type gamma_p_inva(BOOST_MATH_TEST_TYPE a, BOOST_MATH_TEST_TYPE p, const policies::policy<>&);

   template tools::promote_args<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE>::type gamma_p_inva(BOOST_MATH_TEST_TYPE a, BOOST_MATH_TEST_TYPE p);

   template tools::promote_args<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE>::type gamma_p_inv(BOOST_MATH_TEST_TYPE a, BOOST_MATH_TEST_TYPE p, const policies::policy<>&);

   template tools::promote_args<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE>::type gamma_q_inv(BOOST_MATH_TEST_TYPE a, BOOST_MATH_TEST_TYPE q);

   template tools::promote_args<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE>::type gamma_q_inv(BOOST_MATH_TEST_TYPE a, BOOST_MATH_TEST_TYPE q, const policies::policy<>&);

   template tools::promote_args<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE>::type gamma_q_inva(BOOST_MATH_TEST_TYPE a, BOOST_MATH_TEST_TYPE q);

   template tools::promote_args<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE>::type gamma_q_inva(BOOST_MATH_TEST_TYPE a, BOOST_MATH_TEST_TYPE q, const policies::policy<>&);

   // digamma:
   template tools::promote_args<BOOST_MATH_TEST_TYPE>::type digamma(BOOST_MATH_TEST_TYPE x);

   template tools::promote_args<BOOST_MATH_TEST_TYPE>::type digamma(BOOST_MATH_TEST_TYPE x, const policies::policy<>&);

   // Hypotenuse function sqrt(x ^ 2 + y ^ 2).
   template tools::promote_args<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE>::type 
         hypot(BOOST_MATH_TEST_TYPE x, BOOST_MATH_TEST_TYPE y);

   template tools::promote_args<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE>::type 
         hypot(BOOST_MATH_TEST_TYPE x, BOOST_MATH_TEST_TYPE y, const policies::policy<>&);

   // cbrt - cube root.
   template tools::promote_args<BOOST_MATH_TEST_TYPE>::type cbrt(BOOST_MATH_TEST_TYPE z);

   template tools::promote_args<BOOST_MATH_TEST_TYPE>::type cbrt(BOOST_MATH_TEST_TYPE z, const policies::policy<>&);

   // log1p is log(x + 1)
   template tools::promote_args<BOOST_MATH_TEST_TYPE>::type log1p(BOOST_MATH_TEST_TYPE);

   template tools::promote_args<BOOST_MATH_TEST_TYPE>::type log1p(BOOST_MATH_TEST_TYPE, const policies::policy<>&);
   template tools::promote_args<BOOST_MATH_TEST_TYPE>::type log1p<BOOST_MATH_TEST_TYPE, overflow_policy>(BOOST_MATH_TEST_TYPE, const overflow_policy&);

   // log1pmx is log(x + 1) - x
   template tools::promote_args<BOOST_MATH_TEST_TYPE>::type log1pmx(BOOST_MATH_TEST_TYPE);

   template tools::promote_args<BOOST_MATH_TEST_TYPE>::type log1pmx(BOOST_MATH_TEST_TYPE, const policies::policy<>&);
   template tools::promote_args<BOOST_MATH_TEST_TYPE>::type log1pmx(BOOST_MATH_TEST_TYPE, const overflow_policy&);

   // Exp (x) minus 1 functions.
   template tools::promote_args<BOOST_MATH_TEST_TYPE>::type expm1(BOOST_MATH_TEST_TYPE);

   template tools::promote_args<BOOST_MATH_TEST_TYPE>::type expm1(BOOST_MATH_TEST_TYPE, const policies::policy<>&);
   template tools::promote_args<BOOST_MATH_TEST_TYPE>::type expm1(BOOST_MATH_TEST_TYPE, const overflow_policy&);

   // Power - 1
   template tools::promote_args<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE>::type 
         powm1(const BOOST_MATH_TEST_TYPE a, const BOOST_MATH_TEST_TYPE z);

   template tools::promote_args<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE>::type 
         powm1(const BOOST_MATH_TEST_TYPE a, const BOOST_MATH_TEST_TYPE z, const policies::policy<>&);

   // sqrt(1+x) - 1
   template tools::promote_args<BOOST_MATH_TEST_TYPE>::type sqrt1pm1(const BOOST_MATH_TEST_TYPE& val);

   template tools::promote_args<BOOST_MATH_TEST_TYPE>::type sqrt1pm1(const BOOST_MATH_TEST_TYPE& val, const policies::policy<>&);

   // Bessel functions:
   template detail::bessel_traits<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE, policies::policy<> >::result_type cyl_bessel_j(BOOST_MATH_TEST_TYPE v, BOOST_MATH_TEST_TYPE x, const policies::policy<> & pol);

   template detail::bessel_traits<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE, policies::policy<> >::result_type cyl_bessel_j(BOOST_MATH_TEST_TYPE v, BOOST_MATH_TEST_TYPE x);
   template detail::bessel_traits<int, BOOST_MATH_TEST_TYPE, policies::policy<> >::result_type cyl_bessel_j(int v, BOOST_MATH_TEST_TYPE x);

   template detail::bessel_traits<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE, policies::policy<> >::result_type sph_bessel(unsigned v, BOOST_MATH_TEST_TYPE x, const policies::policy<> & pol);

   template detail::bessel_traits<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE, policies::policy<> >::result_type sph_bessel(unsigned v, BOOST_MATH_TEST_TYPE x);

   template detail::bessel_traits<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE, policies::policy<> >::result_type cyl_bessel_i(BOOST_MATH_TEST_TYPE v, BOOST_MATH_TEST_TYPE x, const policies::policy<> & pol);

   template detail::bessel_traits<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE, policies::policy<> >::result_type cyl_bessel_i(BOOST_MATH_TEST_TYPE v, BOOST_MATH_TEST_TYPE x);
   template detail::bessel_traits<int, BOOST_MATH_TEST_TYPE, policies::policy<> >::result_type cyl_bessel_i(int v, BOOST_MATH_TEST_TYPE x);

   template detail::bessel_traits<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE, policies::policy<> >::result_type cyl_bessel_k(BOOST_MATH_TEST_TYPE v, BOOST_MATH_TEST_TYPE x, const policies::policy<> & pol);

   template detail::bessel_traits<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE, policies::policy<> >::result_type cyl_bessel_k(BOOST_MATH_TEST_TYPE v, BOOST_MATH_TEST_TYPE x);
   template detail::bessel_traits<int, BOOST_MATH_TEST_TYPE, policies::policy<> >::result_type cyl_bessel_k(int v, BOOST_MATH_TEST_TYPE x);

   template detail::bessel_traits<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE, policies::policy<> >::result_type cyl_neumann(BOOST_MATH_TEST_TYPE v, BOOST_MATH_TEST_TYPE x, const policies::policy<> & pol);

   template detail::bessel_traits<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE, policies::policy<> >::result_type cyl_neumann(BOOST_MATH_TEST_TYPE v, BOOST_MATH_TEST_TYPE x);
   template detail::bessel_traits<int, BOOST_MATH_TEST_TYPE, policies::policy<> >::result_type cyl_neumann(int v, BOOST_MATH_TEST_TYPE x);

   template detail::bessel_traits<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE, policies::policy<> >::result_type sph_neumann(unsigned v, BOOST_MATH_TEST_TYPE x, const policies::policy<> & pol);

   template detail::bessel_traits<BOOST_MATH_TEST_TYPE, BOOST_MATH_TEST_TYPE, policies::policy<> >::result_type sph_neumann(unsigned v, BOOST_MATH_TEST_TYPE x);

   template tools::promote_args<BOOST_MATH_TEST_TYPE>::type expint(unsigned n, BOOST_MATH_TEST_TYPE z, const policies::policy<> &);

   template detail::expint_result<int, BOOST_MATH_TEST_TYPE>::type expint(int const z, BOOST_MATH_TEST_TYPE const u);

   template tools::promote_args<BOOST_MATH_TEST_TYPE>::type expint(BOOST_MATH_TEST_TYPE z);

   // Zeta:
   template tools::promote_args<BOOST_MATH_TEST_TYPE>::type zeta(BOOST_MATH_TEST_TYPE s, const policies::policy<>&);

   template tools::promote_args<BOOST_MATH_TEST_TYPE>::type zeta(BOOST_MATH_TEST_TYPE s);

}} // namespaces
