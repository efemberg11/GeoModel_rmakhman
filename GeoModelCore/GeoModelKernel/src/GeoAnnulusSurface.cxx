# include "GeoModelKernel/GeoAnnulusSurface.h"

const std::string GeoAnnulusSurface::s_classType = "AnnulusSurface";
const ShapeType GeoAnnulusSurface::s_classTypeID = 36;

GeoAnnulusSurface::GeoAnnulusSurface(double Ox, double Oy, double radius_in, double radius_out, double phi){
    
    m_Ox = Ox;
    m_Oy = Oy;
    m_radius_in = radius_in;
    m_radius_out = radius_out;
    m_phi = phi;

    // if the deviation point is out of the inner ring, throw an error
    if (Ox * Ox + Oy * Oy > radius_in * radius_in){
        THROW_EXCEPTION("\n \n ERROR!!! \n THE DEVIATION POINT IS OUT OF THE INNER RING!!! \n \n");
    }

    // if the outer ring is smaller than the inner ring, throw an error
    if (radius_out < radius_in){
        THROW_EXCEPTION("\n \n ERROR!!! \n THE OUTTER RING IS SMALLER THAN THE INNER RING!!! \n \n");
    }

    // if the inner ring has negative radius, throw an error
    if (radius_in < 0){
        THROW_EXCEPTION("\n \n ERROR!!! \n THE INNER RING IS NEGATIVE!!! \n \n");
    }

    // if the outer ring has non-positive radius, throw an error
    if (radius_out <= 0){
        THROW_EXCEPTION("\n \n ERROR!!! \n THE OUTTER RING MUST BE POSITIVE!!! \n \n");
    }
}

void GeoAnnulusSurface::exec(GeoShapeAction *action) const{
    action->handleAnnulusSurface(this);
}

// bool GeoAnnulusSurface::isInside(double x, double y){}

// double GeoAnnulusSurface::area() const{}
