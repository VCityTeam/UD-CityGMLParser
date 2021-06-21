// Copyright University of Lyon, 2012 - 2017
// Distributed under the GNU Lesser General Public License Version 2.1 (LGPLv2)
// (Refer to accompanying file LICENSE.md or copy at
//  https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html )

////////////////////////////////////////////////////////////////////////////////
#ifndef __OGRGDALTOOLS_HPP__
#define __OGRGDALTOOLS_HPP__

#include <vector>
#ifdef _MSC_VER                  // Inhibit dll-interface warnings concerning
# pragma warning(disable:4251) // gdal-1.11.4 internals (cpl_string.h) when
#endif                           // including ogrsf_frmts.h on VC++
#include <ogrsf_frmts.h>

#include "../../CityModel/Vecs.hpp"

/// Precision pour les points des Wall qui ne se superposent pas parfaitement
/// aux emprises au sol issue du Roff
static double Precision_Vect = 0.00001;


TVec2f CalculUV(std::vector<TVec3d>* Poly,
	std::vector<TVec2f>* UVs, TVec3d Point);

OGRGeometry* CutPolyGMLwithShape(
	OGRPolygon* GMLPoly,
	OGRPolygon* BuildingShp,
	std::vector<TVec2f>* TexUV,
	std::vector<std::vector<TVec2f>>* TexUVout);

double DistanceHausdorff(OGRMultiPolygon * Geo1,
	OGRMultiPolygon * Geo2);

OGRMultiPolygon* GetEnveloppe(OGRMultiPolygon* MP);

std::vector<OGRMultiPoint*> GetPointsFromPolygon(
	OGRPolygon* Polygon);

double Hausdorff(OGRMultiPolygon * GeoPoints,
	OGRMultiPolygon * Geo);

OGRPoint* ProjectPointOnPolygon3D(OGRPoint* Point,
	OGRPolygon* Polygon);

OGRPolygon* ProjectPolyOn3DPlane(OGRPolygon* Poly2D,
	OGRPolygon* Poly3D);

OGRMultiPolygon* ProjectPolyOn3DPlane(
	OGRMultiPolygon* Poly2D,
	OGRPolygon * Poly3D);

// FIXME: the following declarations don't seem to be part of the API.
// Remove them (the declarations not the defenitions within the cpp of course).

OGRLinearRing* ChangePointsOrderForNormal(OGRLinearRing* Ring, std::vector<TVec2f>* Tex);

std::vector<OGRMultiLineString*> GetLineStringsFromPolygon(OGRPolygon* Polygon);

std::vector<OGRMultiLineString*> GetLineStringsFromMultiPolygon(OGRGeometryCollection* MultiPolygon);

////////////////////////////////////////////////////////////////////////////////
#endif // __OGRGDALTOOLS_HPP__
