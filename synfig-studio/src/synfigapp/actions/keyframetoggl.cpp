/* === S Y N F I G ========================================================= */
/*!	\file keyframetoggl.cpp
**	\brief Template File
**
**	$Id$
**
**	\legal
**	Copyright (c) 2002-2005 Robert B. Quattlebaum Jr., Adrian Bentley
**  Copyright (c) 2012 Konstantin Dmitriev
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

/* === H E A D E R S ======================================================= */

#ifdef USING_PCH
#	include "pch.h"
#else
#ifdef HAVE_CONFIG_H
#	include <config.h>
#endif

#include "keyframetoggl.h"
#include <synfigapp/canvasinterface.h>
#include <synfig/valuenode_dynamiclist.h>
#include <synfig/valuenode_animated.h>
#include "activepointremove.h"
#include "waypointremove.h"

#include <synfigapp/general.h>

#endif

using namespace std;
using namespace etl;
using namespace synfig;
using namespace synfigapp;
using namespace Action;

/* === M A C R O S ========================================================= */

ACTION_INIT(Action::KeyframeToggl);
ACTION_SET_NAME(Action::KeyframeToggl,"KeyframeToggl");
ACTION_SET_LOCAL_NAME(Action::KeyframeToggl,N_("Activate/Deactivate Keyframe"));
ACTION_SET_TASK(Action::KeyframeToggl,"disconnect");
ACTION_SET_CATEGORY(Action::KeyframeToggl,Action::CATEGORY_KEYFRAME);
ACTION_SET_PRIORITY(Action::KeyframeToggl,0);
ACTION_SET_VERSION(Action::KeyframeToggl,"0.0");
ACTION_SET_CVS_ID(Action::KeyframeToggl,"$Id$");

/* === G L O B A L S ======================================================= */

/* === P R O C E D U R E S ================================================= */

/* === M E T H O D S ======================================================= */

Action::KeyframeToggl::KeyframeToggl()
{
	keyframe.set_time(Time::begin()-1);
	set_dirty(true);
}

Action::ParamVocab
Action::KeyframeToggl::get_param_vocab()
{
	ParamVocab ret(Action::CanvasSpecific::get_param_vocab());

	ret.push_back(ParamDesc("keyframe",Param::TYPE_KEYFRAME)
		.set_local_name(_("Keyframe"))
		.set_desc(_("Keyframe to be activated or deactivated"))
	);

	return ret;
}

bool
Action::KeyframeToggl::is_candidate(const ParamList &x)
{
	return candidate_check(get_param_vocab(),x);
}

bool
Action::KeyframeToggl::set_param(const synfig::String& name, const Action::Param &param)
{
	if(name=="keyframe" && param.get_type()==Param::TYPE_KEYFRAME)
	{
		keyframe=param.get_keyframe();

		return true;
	}

	return Action::CanvasSpecific::set_param(name,param);
}

bool
Action::KeyframeToggl::is_ready()const
{
	if(keyframe.get_time()==(Time::begin()-1))
		return false;
	return Action::CanvasSpecific::is_ready();
}

void
Action::KeyframeToggl::prepare()
{
	clear();

	try { get_canvas()->keyframe_list().find(keyframe);}
	catch(synfig::Exception::NotFound)
	{
		throw Error(_("Unable to find the given keyframe"));
	}
}


void
Action::KeyframeToggl::perform()
{
	Action::Super::perform();

	if(get_canvas_interface())
	{
		get_canvas_interface()->signal_keyframe_changed()(keyframe);
	}
	else synfig::warning("CanvasInterface not set on action");

	if(keyframe.active())
	{
		keyframe.disable();
	} else {
		keyframe.enable();
	}
}

void
Action::KeyframeToggl::undo()
{
	//try { get_canvas()->keyframe_list().find(keyframe.get_time()); throw Error(_("A Keyframe already exists at this point in time"));}
	//catch(synfig::Exception::NotFound) { }

	//try { get_canvas()->keyframe_list().find(keyframe); throw Error(_("This keyframe is already in the ValueNode"));}
	//catch(synfig::Exception::NotFound) { }

	Action::Super::undo();

	if(keyframe.active())
	{
		keyframe.disable();
	} else {
		keyframe.enable();
	}

	if(get_canvas_interface())
	{
		get_canvas_interface()->signal_keyframe_changed()(keyframe);
	}
	else synfig::warning("CanvasInterface not set on action");
}
