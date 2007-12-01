/*
 * Copyright 2006, 2007 by Brian Dominy <brian@oddchange.com>
 *
 * This file is part of FreeWPC.
 *
 * FreeWPC is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * FreeWPC is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with FreeWPC; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <freewpc.h>
#include <test.h>
#include <coin.h>

/**
 * \file
 * \brief Handle the service button switches inside the coin door.
 */

/* TODO : coin door may be closed, in which case button presses
ought to generate a warning message */

void coin_door_buttons_deff (void)
{
	dmd_alloc_low_clean ();
	font_render_string_center (&font_var5, 64, 1, "COIN DOOR IS CLOSED");
	font_render_string_center (&font_var5, 64, 8, "OPEN COIN DOOR TO");
	font_render_string_center (&font_var5, 64, 14, "USE BUTTONS");
	dmd_show_low ();
	task_sleep_sec (3);
	deff_exit ();
}


CALLSET_ENTRY (service, sw_escape)
{
	if (!switch_poll_logical (SW_COIN_DOOR_CLOSED))
		deff_start (DEFF_COIN_DOOR_BUTTONS);
	else if (!in_test)
	{
		add_credit ();
		audit_increment (&system_audits.service_credits);
	}
}

CALLSET_ENTRY (service, sw_down)
{
	if (!switch_poll_logical (SW_COIN_DOOR_CLOSED))
		deff_start (DEFF_COIN_DOOR_BUTTONS);
	else if (!in_test)
		button_invoke (SW_DOWN, volume_down, TIME_500MS, TIME_100MS);
	else	
		test_down_button ();
}

CALLSET_ENTRY (service, sw_up)
{
	if (!switch_poll_logical (SW_COIN_DOOR_CLOSED))
		deff_start (DEFF_COIN_DOOR_BUTTONS);
	else if (!in_test)
		button_invoke (SW_UP, volume_up, TIME_500MS, TIME_100MS);
	else
		test_up_button ();
}


void coin_door_opened (void)
{
	dbprintf ("Coin door is open\n");
	/* TODO : Print a message that high power coils are disabled */
	/* TODO : Enable Stern's "coindoor ballsave" feature */
}


void coin_door_closed (void)
{
	dbprintf ("Coin door is closed\n");
}


CALLSET_ENTRY (service, sw_coin_door_closed)
{
	/* Be kind and ignore slam tilt switch briefly after the
	coin door is opened/closed */
	event_can_follow (sw_coin_door_closed, sw_slam_tilt, TIME_5S);

	if (switch_poll_logical (SW_COIN_DOOR_CLOSED))
		coin_door_closed ();
	else
		coin_door_opened ();
}

