// Copyright University of Lyon, 2012 - 2017
// Distributed under the GNU Lesser General Public License Version 2.1 (LGPLv2)
// (Refer to accompanying file LICENSE.md or copy at
//  https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html )

////////////////////////////////////////////////////////////////////////////////

#include "DataProfile.hpp"

////////////////////////////////////////////////////////////////////////////////
DataProfile::DataProfile()
	: m_bboxLowerBound(), m_bboxUpperBound(), m_offset(), m_xStep(), m_yStep()
{

}

float DataProfile::getTileHeight() const
{
	return m_yStep;
}
float DataProfile::getTileWidth() const
{
	return m_xStep;
}
double DataProfile::getTileXOffset() const
{
	return m_offset.x;
}
double DataProfile::getTileYOffset() const
{
	return m_offset.y;
}

DataProfile DataProfile::createDataProfileDefault()
{
	return DataProfile::createDataProfileLyon();
}
DataProfile DataProfile::createDataProfileNone()
{
	DataProfile dp;
	dp.m_offset.x = 0.0;
	dp.m_offset.y = 0.0;

	dp.m_bboxLowerBound.x = 0.0;
	dp.m_bboxLowerBound.y = 00;

	dp.m_bboxUpperBound.x = 0.0;
	dp.m_bboxUpperBound.y = 0.0;

	dp.m_xStep = 500;
	dp.m_yStep = 500;

	dp.m_TileIdOriginX = 0;
	dp.m_TileIdOriginY = 0;

	dp.m_id = 0;
	dp.m_name = "None";

	return dp;
}

DataProfile DataProfile::createDataProfileLyon()
{
	DataProfile dp;
	dp.m_offset.x = 1837500.0;//1833000.0;//1837500.0;//1840000.0;
	dp.m_offset.y = 5169000.0;//5166000.0;//5169000.0;//5172500.0;

	dp.m_bboxLowerBound.x = 1800000.0;//1837500.0;//1840000.0;
	dp.m_bboxLowerBound.y = 5100000.0;//5169000.0;//5172500.0;

	dp.m_bboxUpperBound.x = 1851000;//1849500.0;
	dp.m_bboxUpperBound.y = 5184500;//5180500.0;

	dp.m_xStep = 500;
	dp.m_yStep = 500;

	dp.m_TileIdOriginX = (int)(1800000.0 / 500);
	dp.m_TileIdOriginY = (int)(5100000.0 / 500);

	dp.m_id = 2;
	dp.m_name = "Lyon";

	return dp;
}
////////////////////////////////////////////////////////////////////////////////
