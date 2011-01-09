/* === S Y N F I G ========================================================= */
/*!	\file widthpoint.h
**	\brief Template Header for the implementation of a Width Point
**
**	$Id$
**
**	\legal
**	Copyright (c) 2002-2005 Robert B. Quattlebaum Jr., Adrian Bentley
**	Copyright (c) 2010 Carlos López
**
**	This package is free software; you can redistribute it and/or
**	modify it under the terms of the GNU General Public License as
**	published by the Free Software Foundation; either version 2 of
**	the License, or (at your option) any later version.
**
**	This package is distributed in the hope that it will be useful,
**	but WITHOUT ANY WARRANTY; without even the implied warranty of
**	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
**	General Public License for more details.
**	\endlegal
*/
/* ========================================================================= */

/* === S T A R T =========================================================== */

#ifndef __SYNFIG_WIDTHPOINT_H
#define __SYNFIG_WIDTHPOINT_H

/* === H E A D E R S ======================================================= */

#include "uniqueid.h"
#include "real.h"

/* === M A C R O S ========================================================= */

/* === T Y P E D E F S ===================================================== */

/* === C L A S S E S & S T R U C T S ======================================= */

namespace synfig {

class WidthPoint : public UniqueID
{
private:
	Real position_;
	Real width_;
	int side_type_[2]; // Before [0] and After[1] side types

public:

	enum SideType
	{
		TYPE_INTERPOLATE     =0,
		TYPE_ROUNDED         =1,
		TYPE_SQUARED         =2,
		TYPE_PEAK            =3
	};

	WidthPoint();
	WidthPoint(Real position, Real width, int sidebefore=TYPE_INTERPOLATE,
		int sideafter=TYPE_INTERPOLATE);

	const Real& get_position()const;
	void set_position(const Real& x);
	// gets the normalised position: converts it to be inside [0,1]
	Real get_norm_position()const;

	const Real& get_width()const;
	void set_width(Real x);

	int get_side_type_before();
	void set_side_type_before(int sidebefore);
	int get_side_type_after();
	void set_side_type_after(int sideafter);
	int get_side_type(int i);
	bool operator < (const WidthPoint& rhs) { return position_  < rhs.position_; }

}; // END of class WidthPoint

}; // END of namespace synfig

/* === E N D =============================================================== */

#endif
