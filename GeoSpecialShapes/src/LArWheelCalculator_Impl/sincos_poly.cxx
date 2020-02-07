/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoSpecialShapes/LArWheelCalculator.h"
#include "GeoSpecialShapes/sincos.h"

#include <Eigen/Dense>
#include <vector>
#include <sys/time.h>
#include <iostream>
#include <iomanip>
#include <math.h>
#include <mutex>

// Physical constants
#include "GeoModelKernel/Units.h"
#define SYSTEM_OF_UNITS GeoModelKernelUnits
#define DEBUGPRINT 0

using namespace Eigen;

//template<typename T>
//std::ostream & operator << (std::ostream & ostr, const TVectorT<T> & v)
//{
//  std::ios_base::fmtflags save_flags(ostr.flags());
//  ostr << '[';
//  ostr << std::scientific;
//  for(int idx=v.GetLwb();idx<v.GetUpb();idx++) {
//    ostr << v[idx] << ", ";
//  }
//  ostr << v[v.GetUpb()];
//  ostr << ']';
//  ostr.flags(save_flags);
//  return ostr;
//}


// find best approximation of y values using linear combination of basis functions in bf

//static std::vector<double>
static RowVectorXd
findLinearApproximation(
    const int dataLen, const int nBasisFuntions,
    const RowVectorXd &y, const MatrixXd & bf)
{
  //TMatrixDSym A(nBasisFuntions);
  MatrixXd A (nBasisFuntions, nBasisFuntions);
  //TVectorD vY(nBasisFuntions);
  VectorXd vY(nBasisFuntions);

  for(int j = 0; j < nBasisFuntions; ++ j){
    for(int k = 0; k < nBasisFuntions; ++ k){
      double Ajk = 0.0;
      for(int i = 0; i < dataLen; ++ i){
        Ajk += bf(j, i) * bf(k, i);
      }
      A(j, k) = Ajk;
    }
  }

  for(int k = 0; k < nBasisFuntions; ++ k){
    double vYk = 0.0;
    for(int i = 0; i < dataLen; ++ i){
      vYk += y[i]*bf(k,i);
    }
    vY[k] = vYk;
  }

  //TMatrixDSym Ainv(A);
  MatrixXd Ainv(A);
  //Ainv.Invert();
  Ainv.inverse();

  // std::cout << "Ainv: " << std::endl;
  // std::cout << Ainv << std::endl;
  // std::cout << "vY: " << std::endl;
  // std::cout << vY << std::endl;

  return Ainv*vY;
}

void LArWheelCalculator::fill_sincos_parameterization()
{
  const int nrPolyDegree = LARWC_SINCOS_POLY;
#if LARWC_SINCOS_POLY > 4 && DEBUGPRINT
  std::cout << "LARWC_SINCOS_POLY: " << LARWC_SINCOS_POLY << std::endl;
#endif
  const int nBasisFunctions = nrPolyDegree + 1;

  // We compute the polynomial approximations once per side, and store them in
  // the static variables below for reuse in successive calculator instances.
  // For multi-threading, then, this code needs to be mutex locked.
  // FIXME: this could done in a cleaner way.
  static std::mutex fillParamMutex;
  std::lock_guard<std::mutex> lock(fillParamMutex);

  static bool filled[2] = { false, false };
  static double sin_parametrization[2][nBasisFunctions];
  static double cos_parametrization[2][nBasisFunctions];

  // Reuse the computation if already performed
  size_t S = m_isInner? 0: 1;
  if(filled[S]){
    for(int i = 0; i < nBasisFunctions; ++ i){
      m_sin_parametrization[i] = sin_parametrization[S][i];
      m_cos_parametrization[i] = cos_parametrization[S][i];
    }
    return;
  }

  //const Double_t Rmin = m_isInner? 290.*mm: 600.*mm;
  //const Double_t Rmax = m_isInner? 710.*mm: 2050.*mm;
  const double Rmin = m_isInner? 250.*SYSTEM_OF_UNITS::mm: 560.*SYSTEM_OF_UNITS::mm;
  const double Rmax = m_isInner? 750.*SYSTEM_OF_UNITS::mm: 2090.*SYSTEM_OF_UNITS::mm;
  //const Double_t Rmin = m_isInner? 220.*mm: 530.*mm;
  //const Double_t Rmax = m_isInner? 780.*mm: 2120.*mm;
  const double Rstep = 1.*SYSTEM_OF_UNITS::mm;
  const int nrPoints = (Rmax - Rmin) * (1./Rstep);
  const int dataLen = nrPoints + 1;

//  TVectorD x(dataLen);  // angle points
//  TVectorD ysin(dataLen);  // to be approximated function values at angle points - sin
//  TVectorD ycos(dataLen);  // to be approximated function values at angle points - cos
  RowVectorXd x(dataLen);    // angle points
  RowVectorXd ysin(dataLen); // to be approximated function values at angle points - sin
  RowVectorXd ycos(dataLen); // to be approximated function values at angle points - cos

  //TMatrixD bf(nBasisFunctions, dataLen); // Matrix of values of basis functions at angle points
  MatrixXd bf (nBasisFunctions, dataLen);

  for(int i = 0; i < dataLen; ++ i){
    const double a = Rmin + i * Rstep;
    x[i] = a;
    CxxUtils::sincos scalpha(parameterized_slant_angle(a));
    ysin[i] = scalpha.sn;
    ycos[i] = scalpha.cs;
    for(int n = 0; n < nBasisFunctions; ++ n) {
      bf(n, i) = pow(a, n);
    }
  }

  //std::vector<double>
    RowVectorXd
 params_sin =
    findLinearApproximation(dataLen, nBasisFunctions, ysin, bf);
  //std::vector<double>
    RowVectorXd
 params_cos =
    findLinearApproximation(dataLen, nBasisFunctions, ycos, bf);

  for(int i = 0; i < nBasisFunctions; ++ i){
    m_sin_parametrization[i] = params_sin[i];
    m_cos_parametrization[i] = params_cos[i];
    sin_parametrization[S][i] = params_sin[i];
    cos_parametrization[S][i] = params_cos[i];
  }

  filled[S] = true;

  // FIXME: nothing below is needed unless debug printing

#if DEBUGPRINT
  std::cout << "sin params:" << params_sin << std::endl;
  std::cout << "cos params:" << params_cos << std::endl;

  double dsinr = 0., dcosr = 0.;
  double dtrigr = 0;
#endif

  double dsin = 0., dcos = 0.;
  double dtrig = 0.;
  for(double r = Rmin + 40.; r < Rmax - 40.; r += Rstep / 10.){
    CxxUtils::sincos scalpha(parameterized_slant_angle(r));
    double sin_a, cos_a;
    parameterized_sincos(r, sin_a, cos_a);
    double ds = fabs(scalpha.sn - sin_a);
    if(ds > dsin){
      dsin = ds;
#if DEBUGPRINT
      dsinr = r;
#endif
    }
    double dc = fabs(scalpha.cs - cos_a);
    if(dc > dcos){
      dcos = dc;
#if DEBUGPRINT
      dcosr = r;
#endif
    }
    double dt = fabs(sin_a*sin_a + cos_a*cos_a - 1.);
    if(dt > dtrig){
      dtrig = dt;
#if DEBUGPRINT
      dtrigr = r;
#endif
    }
  }

#if DEBUGPRINT
  std::cout << "Max. difference: " << std::endl
            << "\tsin: " << dsin << " at " << dsinr << std::endl
            << "\tcos: " << dcos << " at " << dcosr << std::endl
            << "\tsin^2+cos^2: " << dtrig << " at " << dtrigr << std::endl;
#endif

#ifdef HARDDEBUG
  std::vector<double> y_test(dataLen);
  const int nIter=10000;
  std::cout << "Perfomance test started, " << nIter << " iterations" << std::endl;

  double y_testsin[dataLen];
  double y_testcos[dataLen];
  struct timeval tvsincos_start, tvsincos_stop;
  gettimeofday(&tvsincos_start, 0);
  for(int iIter=0;iIter<nIter;iIter++) {
    for(int i=0;i<dataLen;i++) {
      sincos(parameterized_slant_angle(x[i]), &y_testsin[i], &y_testcos[i]);
    }
  }
  gettimeofday(&tvsincos_stop, 0);
  double timeSinCos=(tvsincos_stop.tv_sec-tvsincos_start.tv_sec + 1E-6*(tvsincos_stop.tv_usec-tvsincos_start.tv_usec))/nIter;

  std::cout.unsetf ( std::ios::fixed | std::ios::scientific );
  std::cout << "Time to fill 2x" << dataLen << " elements using sincos function: " << timeSinCos << std::endl;

  struct timeval tvpoly_start, tvpoly_stop;
  gettimeofday(&tvpoly_start, 0);
  for(int iIter=0;iIter<nIter;iIter++) {
    for(int i=0;i<dataLen;i++) {
      parameterized_sincos(x[i], y_testsin[i], y_testcos[i]);
    }
  }
  gettimeofday(&tvpoly_stop, 0);
  double timePoly=(tvpoly_stop.tv_sec - tvpoly_start.tv_sec + 1E-6*(tvpoly_stop.tv_usec - tvpoly_start.tv_usec))/nIter;
  std::cout << "Time to fill 2x" << dataLen << " elements using approximation sin&cos: " << timePoly << std::endl;
  std::cout.unsetf ( std::ios::fixed | std::ios::scientific );
  std::cout << "Approximation is " << timeSinCos/timePoly << " faster " << std::endl;
#endif

}
