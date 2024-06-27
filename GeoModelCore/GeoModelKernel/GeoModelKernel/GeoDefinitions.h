/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEODEFINITIONS_H
#define GEOMODELKERNEL_GEODEFINITIONS_H

// for GNU: ignore this specific warning, otherwise just include Eigen/Dense
#if defined(__GNUC__) && !defined(__clang__) && !defined(__INTEL_COMPILER)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmisleading-indentation"
#include <Eigen/Dense>
#pragma GCC diagnostic pop
#else
#include <Eigen/Dense>
#endif

namespace GeoTrf {
  using Rotation3D = Eigen::Quaternion<double>;
  using Translation3D = Eigen::Translation<double, 3>;
  using AngleAxis3D =  Eigen::AngleAxisd;
  using Transform3D = Eigen::Affine3d;
  template<size_t N> using VectorN =  Eigen::Matrix<double, N, 1>;
  using Vector3D = VectorN<3>;
  using Vector2D = VectorN<2>;
  using RotationMatrix3D =  Eigen::Matrix<double, 3, 3>;
  using Diagonal3DMatrix = Eigen::DiagonalMatrix <double, 3>;

  /// check if we can use Eigen Hyperplane for this
  struct Plane3D {
  protected:
    double a_{0.};
    double b_{0.};
    double c_{1.};
    double d_{0.};
    
  public:
    /**
     *  Default constructor - creates plane z=0. */
    Plane3D() = default;
    /**
     * Constructor from four numbers - creates plane a*x+b*y+c*z+d=0. */
    Plane3D(double a1, double b1, double c1, double d1)
    :  a_(a1), b_(b1), c_(c1), d_(d1) 
    {}

    /**
     * Constructor from normal and point. */
    Plane3D(const Vector3D& n, const Vector3D& p)
    : a_(n.x()), b_(n.y()), c_(n.z()), d_(-n.dot(p)) 
    {}
    
    /**
     * Returns the a-coefficient in the plane equation: a*x+b*y+c*z+d=0. */
    double a() const { return a_; }
    /**
     * Returns the b-coefficient in the plane equation: a*x+b*y+c*z+d=0. */
    double b() const { return b_; }
    /**
     * Returns the c-coefficient in the plane equation: a*x+b*y+c*z+d=0. */
    double c() const { return c_; }
    /**
     * Returns the free member of the plane equation: a*x+b*y+c*z+d=0. */
    double d() const { return d_; }
    
    /**
     * Returns normal. */
    Vector3D normal() const { return Vector3D(a_,b_,c_); }
  };

  class Translate3D : public Transform3D {
  public:
    Translate3D(double x, double y, double z)
      : Transform3D(Translation3D(x,y,z))
      {}
    virtual ~Translate3D() = default;
  };

 class Scale3D : public Transform3D {
 public:
   Scale3D(double x, double y, double z)
     : Transform3D(Diagonal3DMatrix(x,y,z))
     {}
   virtual ~Scale3D() = default;
 };

  class TranslateX3D : public Transform3D {
  public:
    TranslateX3D(double x)
      : Transform3D(Translation3D(x,0,0))
      {}
    virtual ~TranslateX3D() = default;
  };

  class TranslateY3D : public Transform3D {
  public:
    TranslateY3D(double y)
      : Transform3D(Translation3D(0,y,0))
      {}
    virtual ~TranslateY3D() = default;
  };

  class TranslateZ3D : public Transform3D {
  public:
    TranslateZ3D(double z)
      : Transform3D(Translation3D(0,0,z))
      {}
    virtual ~TranslateZ3D() = default;
  };

  /**   Starting from the three rotation matrices
    *
    *                      | 1          0            0      |
    *     R_{x}(\alpha) =  | 0   cos(\alpha)   -sin(\alpha) |,
    *                      | 0   sin(\alpha)    cos(\alpha) |  
    *
    *                      | cos(\alpha)  0     sin(\alpha) |
    *     R_{y}(\alpha) =  |      0       1        0        |,
    *                      | -sin(\alpha) 0     cos(\alpha) |
    *
    *                      | cos(\alpha) -sin(\alpha)   0 |
    *     R_{z}(\alpha) =  | sin(\alpha)  cos(\alpha)   0 |,
    *                      |    0           0           1 |
    *
    *   a genericrotation can be either expressed in terms of the three rotation angles (\alpha, \beta, \gamma) 
    *
    *           R(\alpha, \beta, \gamma) = R_{z}(\gamma) * R_{y}(\beta) * R_{x}(\alpha)
    *
    *   or as a function of the three rotation angles (\phi, \psi, \theta)
    *
    *          R(\phi, \psi, \theta) = R_{z}(\phi) * R_{x}(\psi) * R_{z}(\phi)
    *
    *   The first one is referred to as coordinate Euler angles and the second one is referred to geometric Euler angles
  */
  inline RotationMatrix3D get3DRotMatX(const double angle) {
      return RotationMatrix3D{AngleAxis3D{angle, Vector3D::UnitX()}};
  }
  inline RotationMatrix3D get3DRotMatY(const double angle) {
      return RotationMatrix3D{AngleAxis3D{angle, Vector3D::UnitY()}};
  }
  inline RotationMatrix3D get3DRotMatZ(const double angle) {
      return RotationMatrix3D{AngleAxis3D{angle, Vector3D::UnitZ()}};
  }
  class Rotate3D : public Transform3D {
  public:
    /** @param angle: Rotation angle
     *  @param axis: Arbitrary rotation axis
    */
    Rotate3D(double angle, const Vector3D& axis)
      : Transform3D(AngleAxis3D(angle,axis))
      {}
    virtual ~Rotate3D() = default;
  };
  
  /// @brief Rotation around the x-axis
  class RotateX3D : public Transform3D {
  public:
    RotateX3D(double angle)
      : Transform3D(get3DRotMatX(angle))
      {}
    virtual ~RotateX3D() = default;
  };

  /// @brief Rotation around the y-axis
  class RotateY3D : public Transform3D {
  public:
    RotateY3D(double angle)
      : Transform3D(get3DRotMatY(angle))
      {}
    virtual ~RotateY3D() = default;
  };

  /// @brief Rotation around the z-axis
  class RotateZ3D : public Transform3D {
  public:
    RotateZ3D(double angle)
      : Transform3D(get3DRotMatZ(angle))
      {}
    virtual ~RotateZ3D() = default;
  };

  /// @brief Representation of the Euler angles in the geometric description
  struct EulerAngles{
      
      EulerAngles() = default;
      /// @brief Constructor taking the Euler angles values (phi, theta, psi)
      EulerAngles(const double _ph, const double _th, const double _ps):
          phi{_ph}, theta{_th}, psi{_ps} {}
      /// @brief Rotation angle around the z-axis
      double phi{0.};
      /// @brief Rotation angle around the y-axis
      double theta{0.};
      /// @brief Rotation angle around the z-axis
      double psi{0.};
      /// @brief Ordering operator to put the angles into a set
      bool operator<(const EulerAngles& other) const;
      /// @brief Simple comparison returning -1, 0, 1
      int compare(const EulerAngles& other) const;
      operator bool() const;
      /// @brief Returns the set of Euler angles to invert the rotation
      EulerAngles inverse() const;

  };

  /// @brief Representation of the Euler angles in the coordinate description
  struct CoordEulerAngles {
    CoordEulerAngles() = default;
    /// @brief Constructor taking the Euler angles values (alpha, beta, gamma)
    CoordEulerAngles(const double _a, const double _b, const double _c):
        alpha{_a}, beta{_b}, gamma{_c} {}

    /// @brief Rotation angles around the x-axis
    double alpha{0.};   
    /// @brief Rotation angle around the y-axis
    double beta{0.};
     /// @brief Rotation angle around the z-axis
    double gamma{0.};

    /// @brief Ordering operator to put the angles into a set
    bool operator<(const CoordEulerAngles& other) const;
    /// @brief Simple comparison returning -1, 0, 1
    int compare(const CoordEulerAngles& other) const;
    /// @brief  Returns the set of CoordEulerAngles to invert the Rotation
    CoordEulerAngles inverse() const;
    operator bool() const;
  };
  
  /// @brief Calculates the geometrical Euler angles
  /// @param trans: Input transform  
  /// @return Euler angles in the geometrical representation
  EulerAngles getGeoRotationAngles(const Transform3D& trans);
  EulerAngles getGeoRotationAngles(const RotationMatrix3D& rotMat);
  
  /// @brief Calculates the coordinate Euler angles
  /// @param trans: Input transform  
  /// @return Euler angles in the coordinate representation  
  CoordEulerAngles getCoordRotationAngles(const Transform3D& trans);
  CoordEulerAngles getCoordRotationAngles(const RotationMatrix3D& rotMat);

  /// @brief Arbitrary rotation around an axis using the Euler angles
  class GeoRotation : public RotationMatrix3D {
  public:
    /// @brief Constructor taking the Euler angles in the Geometrical representation
    /// @param phi:   Euler angle around the z-axis 
    /// @param theta: Euler angle around the x-axis
    /// @param psi:   Euler angle around the z-axis
    GeoRotation(double phi, double theta, double psi);

    GeoRotation(const EulerAngles& angles);
    GeoRotation(const CoordEulerAngles& angles);

    virtual ~GeoRotation() = default;
  };

  class GeoTransformRT : public Transform3D {
  public:
    GeoTransformRT(const GeoRotation& rot, const Vector3D& trans);    
    GeoTransformRT(const EulerAngles& angles, const Vector3D& trans);
    GeoTransformRT(const CoordEulerAngles&angles, const Vector3D& trans);

    virtual ~GeoTransformRT() = default;
  };
}

#endif // GEOMODELKERNEL_GEODEFINITIONS_H
