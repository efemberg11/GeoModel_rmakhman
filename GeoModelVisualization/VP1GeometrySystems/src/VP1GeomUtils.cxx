/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
 */

#include "VP1GeometrySystems/VP1GeomUtils.h"

#include "GeoModelKernel/GeoMaterial.h"
#include "GeoModelKernel/GeoElement.h"
#include "GeoModelKernel/Units.h"
#define SYSTEM_OF_UNITS GeoModelKernelUnits // --> 'GeoModelKernelUnits::cm'

#include<QStringList>


//______________________________________________________________________________________
QStringList VP1GeomUtils::geoMaterialToStringList( const GeoMaterial* mat )
{
	QStringList out;
	out << mat->getName().c_str() << "-";
	out << "density:" << QString::number(mat->getDensity());
	return out;
}


//______________________________________________________________________________________
QStringList VP1GeomUtils::geoElementToStringList( const GeoElement* elem )
{
	QStringList out;
	out	<< QString::fromStdString(elem->getName()) << ","
	    << "Z:" << QString::number(elem->getZ()) << ","
	    << "A:" << QString::number(elem->getA()) << ","
	    << "( " + QString::number(elem->getA() / (SYSTEM_OF_UNITS::gram/SYSTEM_OF_UNITS::mole)) + " [g/mole] )";
	return out;
}
