/* === S Y N F I G ========================================================= */
/*!	\file keyframe.h
**	\brief Template Header
**
**	\legal
**	Copyright (c) 2002-2005 Robert B. Quattlebaum Jr., Adrian Bentley
**	Copyright (c) 2012-2013 Konstantin Dmitriev
**
**	This file is part of Synfig.
**
**	Synfig is free software: you can redistribute it and/or modify
**	it under the terms of the GNU General Public License as published by
**	the Free Software Foundation, either version 2 of the License, or
**	(at your option) any later version.
**
**	Synfig is distributed in the hope that it will be useful,
**	but WITHOUT ANY WARRANTY; without even the implied warranty of
**	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**	GNU General Public License for more details.
**
**	You should have received a copy of the GNU General Public License
**	along with Synfig.  If not, see <https://www.gnu.org/licenses/>.
**	\endlegal
*/
/* ========================================================================= */

/* === S T A R T =========================================================== */

#ifndef __SYNFIG_KEYFRAME_H
#define __SYNFIG_KEYFRAME_H

/* === H E A D E R S ======================================================= */

#include <vector>

#include "guid.h"
#include "string.h"
#include "time.h"
#include "uniqueid.h"
#include "waypoint.h"

/* === M A C R O S ========================================================= */

/* === T Y P E D E F S ===================================================== */

/* === C L A S S E S & S T R U C T S ======================================= */

namespace synfig {

/**
 * Keyframe is used to record the state of the animation at a given time point.
 *
 * It can have a description, be enabled or disabled and have an associated Waypoint::Model.
 */
class Keyframe : public UniqueID
{
private:

	Time time_;
	String desc_;
	GUID guid_;

	/**
	 * @c true if the keyframe is active, @c false if it is to be skipped (marker)
	 * @see set_active(), enable(), disable(), active()
	 */
	bool active_;

	Waypoint::Model waypoint_model_;
	/**
	 * @c true a waypoint model has been set, @c false when keyframe is created
	 * @see apply_model(const Waypoint::Model &x)
	 */
	bool has_waypoint_model_;

public:

	Keyframe();
	Keyframe(const Time &time);

	~Keyframe();

	void set_time(Time x) { time_=x; }
	Time get_time() const { return time_; }

	void set_description(String x) { desc_=x; }
	String get_description() const { return desc_; }

	const GUID& get_guid() const { return guid_; }
	void set_guid(const GUID& x) { guid_=x; }

	/** Enables the keyframe, by making it @em active) */
	void enable() { set_active(true); }

	/**
	 * Disables the keyframe, by making it @em inactive.
	 * When keyframe is disabled, it will be acting as a simple time marker.
	 */
	void disable() { set_active(false); }
	
	/**
	 * Sets the 'active' flag to the state described by @p x.
	 * When keyframe is disabled, it will be acting as a simple time marker.
	*/
	void set_active(bool x);

	/** Returns the status of the 'active' flag */
	bool active()const { return active_; }

	using UniqueID::operator<;
	using UniqueID::operator==;
	using UniqueID::operator!=;
	using UniqueID::operator=;

	bool operator<(const Keyframe &rhs)const { return time_<rhs.time_; }
	bool operator<(const Time &rhs)const { return time_<rhs; }

	bool operator==(const Time &rhs)const { return time_==rhs; }

	bool operator!=(const Time &rhs)const { return time_!=rhs; }

	const Waypoint::Model &get_waypoint_model()const { return waypoint_model_; }
	//! Keep a trace of the associated waypoint Model.
	void apply_model(const Waypoint::Model &x);
    //! Returns the status of the 'waypoint model' flag
	bool has_model() const {return has_waypoint_model_; }
}; // END of class Keyframe

/**
 * A sorted list of Keyframes.
 * @see Keyframe
 */
class KeyframeList : public std::vector<Keyframe>
{

public:

	/** Add a new keyframe and sort the list */
	void add(const Keyframe &x);

	/**
	 * Remove the keyframe with the given UniqueID @c x.
	 * Nothing happens if not found
	 */
	void erase(const UniqueID &x);

	/**
	 * Search for keyframe with the given UniqueID @c x.
	 * @param[in] x the unique ID of the wanted keyframe
	 * @param[out] out iterator for the found keyframe. end() if not found
	 * @return @c true if a keyframe was found
	 * @see bool find(const Time &x, KeyframeList::iterator &out)
	 */
	bool find(const UniqueID &x, KeyframeList::iterator &out);

	/**
	 * Find the keyframe at an exact point in time
	 * @param[in] x the exact time the keyframe marks
	 * @param[out] out iterator for the found keyframe. end() if not found
	 * @return @c true if a keyframe was found
	 */
	bool find(const Time &x, KeyframeList::iterator &out);

	/**
	 * Find the first keyframe after a point in time
	 * @param[in] x the starting time for the keyframe search (this time is not included)
	 * @param[out] out iterator for the found keyframe. end() if not found
	 * @param[in] ignore_disabled if the search must skip disabled keyframes
	 * @return @c true if a keyframe was found
	 */
	bool find_next(const Time &x, KeyframeList::iterator &out, bool ignore_disabled = true);

	/**
	 * Find the nearest keyframe before a point in time
	 * @param[in] x the starting time for the keyframe search (this time is not included)
	 * @param[out] out iterator for the found keyframe. end() if not found
	 * @param[in] ignore_disabled if the search must skip disabled keyframes
	 * @return @c true if a keyframe was found
	 */
	bool find_prev(const Time &x, KeyframeList::iterator &out, bool ignore_disabled = true);

	/**
	 * Convenient method equivalent to calling find_next() and find_previous().
	 * The difference is the value of \c prev iterator is \c begin() if not found.
	 * @param[in] time the reference time point
	 * @param[out] prev iterator for the found keyframe before time @c x. begin() if not found
	 * @param[out] next iterator for first keyframe after time @c x. end() if not found
	 * @param[in] ignore_disabled if the search must skip disabled keyframes
	 * @see find_next(), find_previous()
	 */
	void find_prev_next(const Time& time, Time &prev, Time &next, bool ignore_disabled = true);

	/**
	 * Sort the list.
	 * List must be sorted before calling find*() methods to they work properly
	 */
	void sync();
};

}; // END of namespace synfig

/* === E N D =============================================================== */

#endif
