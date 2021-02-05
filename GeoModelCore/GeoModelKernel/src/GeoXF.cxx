/*
  Copyright (C) 2002-2021 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoModelKernel/GeoXF.h"
#include "GeoModelKernel/GeoDefinitions.h"
#include <stdexcept>
namespace GeoXF
{



  //------------------------------------------------------------------//
  //                                                                  //
  //  Implementation of Function                                      //
  //                                                                  //
  //------------------------------------------------------------------//

  Function::Function ()
  {
  }

  Function::~Function ()
  {
  }

  Product operator * (const Function & a, const Function & b)
  {
    return Product (&a, &b);
  }

  PreMult operator * (const GeoTrf::Transform3D & xf, const Function & b)
  {
    return PreMult (xf, &b);
  }

  PostMult operator * (const Function & a, const GeoTrf::Transform3D & xf)
  {
    return PostMult (&a, xf);
  }

  unsigned int Function::dimensionality () const
  {
    return 1;
  }

  //------------------------------------------------------------------//
  //                                                                  //
  //  Implementation of Product                                       //
  //                                                                  //
  //------------------------------------------------------------------//

  Product::Product (const Function * arg1,
        const Function * arg2):m_arg1 (arg1->clone ()),
    m_arg2 (arg2->clone ())
  {
    if (arg1->dimensionality () != arg2->dimensionality ())
      {
  throw std::
    runtime_error ("Warning:  dimension mismatch in GeoXF::Product");
      }
  }


  // Every function must override this:
  Product *Product::clone () const
  {
    return new Product (*this);
  }

  // Copy constructor:
  Product::Product (const Product & right):Function (),
    m_arg1 (right.m_arg1->clone ()), m_arg2 (right.m_arg2->clone ())
  {
  }


  Product::~Product ()
  {
    delete m_arg1;
    delete m_arg2;
  }

  unsigned int Product::dimensionality () const
  {
    return m_arg1->dimensionality ();
  }

  GeoTrf::Transform3D Product::operator        () (double x) const
  {
    return (*m_arg1) (x) * (*m_arg2) (x);
  }

  GeoTrf::Transform3D Product::operator        () (const GeoGenfun::Argument & x) const
  {
    return (*m_arg1) (x) * (*m_arg2) (x);
  }



  //------------------------------------------------------------------//
  //                                                                  //
  //  Implementation of PreMult                                       //
  //                                                                  //
  //------------------------------------------------------------------

  PreMult::PreMult (const GeoTrf::Transform3D & arg1,
        const Function * arg2):m_arg1 (arg1),
    m_arg2 (arg2->clone ())
  {
  }


  // Every function must override this:
  PreMult *PreMult::clone () const
  {
    return new PreMult (*this);
  }

  // Copy constructor:
  PreMult::PreMult (const PreMult & right):Function (), m_arg1 (right.m_arg1),
    m_arg2 (right.m_arg2->clone ())
  {
  }


  PreMult::~PreMult ()
  {
    delete m_arg2;
  }

  unsigned int PreMult::dimensionality () const
  {
    return m_arg2->dimensionality ();
  }

#if defined(FLATTEN) && defined(__GNUC__)
// We compile this package with optimization, even in debug builds; otherwise,
// the heavy use of Eigen makes it too slow.  However, from here we may call
// to out-of-line Eigen code that is linked from other DSOs; in that case,
// it would not be optimized.  Avoid this by forcing all Eigen code
// to be inlined here if possible.
__attribute__ ((flatten))
#endif
  GeoTrf::Transform3D PreMult::operator        () (double x) const
  {
    return m_arg1 * (*m_arg2) (x);
  }

#if defined(FLATTEN) && defined(__GNUC__)
// See above.
__attribute__ ((flatten))
#endif
  GeoTrf::Transform3D PreMult::operator        () (const GeoGenfun::Argument & x) const
  {
    return m_arg1 * (*m_arg2) (x);
  }


  //------------------------------------------------------------------//
  //                                                                  //
  //  Implementation of PostMult                                      //
  //                                                                  //
  //------------------------------------------------------------------//

  PostMult::PostMult (const Function * arg1,
          const GeoTrf::Transform3D & arg2):m_arg1 (arg1->clone ()),
    m_arg2 (arg2)
  {
  }


  // Every function must override this:
  PostMult *PostMult::clone () const
  {
    return new PostMult (*this);
  }

  // Copy constructor:
  PostMult::PostMult (const PostMult & right):Function (),
    m_arg1 (right.m_arg1->clone ()), m_arg2 (right.m_arg2)
  {
  }


  PostMult::~PostMult ()
  {
    delete m_arg1;
  }

  unsigned int PostMult::dimensionality () const
  {
    return m_arg1->dimensionality ();
  }

#if defined(FLATTEN) && defined(__GNUC__)
// See above.
__attribute__ ((flatten))
#endif
  GeoTrf::Transform3D PostMult::operator        () (double x) const
  {
    return (*m_arg1) (x) * m_arg2;
  }

#if defined(FLATTEN) && defined(__GNUC__)
// See above.
__attribute__ ((flatten))
#endif
  GeoTrf::Transform3D PostMult::operator        () (const GeoGenfun::Argument & x) const
  {
    return (*m_arg1) (x) * m_arg2;
  }


  Pow::Pow (const GeoTrf::Transform3D & xform, GeoGenfun::GENFUNCTION f):m_xf (xform),
    m_function (f.clone ())
  {
  }

  Pow::~Pow ()
  {
    delete m_function;
  }

#if defined(FLATTEN) && defined(__GNUC__)
// See above.
__attribute__ ((flatten))
#endif
  GeoTrf::Transform3D Pow::operator() (double x) const
  {
    //
    // Get the translation part and the rotation part:
    //
    GeoTrf::RotationMatrix3D rotate = m_xf.rotation ();
    GeoTrf::Vector3D translate = m_xf.translation ();
    Eigen::AngleAxis<double> aa(rotate);
    //
    // Evaluate the function
    //
    double nTimes = (*m_function) (x);
    //
    // Modify:
    //
    translate *= nTimes;
    double& delta = aa.angle();
    delta *= nTimes;
    //
    // Now compose these and return a result:
    //
    return GeoTrf::Translation3D (translate) * GeoTrf::Transform3D(aa);
  }

  GeoTrf::Transform3D Pow::operator        () (const GeoGenfun::Argument & argument) const
  {
    return operator        () (argument[0]);
  }

  Pow *Pow::clone () const
  {
    return new Pow (*this);
  }

  Pow::Pow (const Pow & right):Function (), m_xf (right.m_xf),
    m_function (right.m_function->clone ())
  {
  }

}
