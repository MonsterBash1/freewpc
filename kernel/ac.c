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

/**
 * \file
 * \brief AC/zerocross circuit handling
 *
 * This module is not yet complete.
 */

__fastram__ U8 ac_zc_count;

__fastram__ U8 ac_zc_period;

__fastram__ U8 ac_zc_poll_count;

__fastram__ bool ac_zc_broken;


void ac_rtt (void)
{
#if 0
	if (ac_zc_broken)
	{
		ac_zc_count = 7;
	}
	else
#endif
	if (wpc_read_ac_zerocross () )
	{
		/* At zero cross point */
		ac_zc_count = 0;
	}
	else
	{
		/* Not at zero cross point */
		ac_zc_count++;
		ac_zc_count %= 8; /* limit to 0-7 */
	}
}


CALLSET_ENTRY (ac, idle)
{
	if (!ac_zc_broken)
	{
		if (ac_zc_poll_count < 250)
		{
			++ac_zc_poll_count;
			if (ac_zc_count != 0)
			{
				ac_zc_poll_count = 0;
			}
		}
		else
		{
			ac_zc_broken = TRUE;
			dbprintf ("Zerocross circuit not working.\n");
		}
	}
}


void ac_init (void)
{
	ac_zc_count = 0;
	ac_zc_period = 8;
	ac_zc_poll_count = 0;
	ac_zc_broken = FALSE;
}

