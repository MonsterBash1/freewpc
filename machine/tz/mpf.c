/*
 * Copyright 2006, 2007, 2008, 2009 by Brian Dominy <brian@oddchange.com
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

/** The number of balls enabled to go to the MPF */
__local__ U8 mpf_enable_count;

/** Number of balls currently on the mini-playfield */
U8 mpf_ball_count;
U8 mpf_round_timer;
U8 mpf_award;
U8 __local__ mpf_level;

/* Where the powerball is */
extern U8 pb_location;
#define PB_MAYBE_IN_PLAY 0x10
extern void reset_unlit_shots (void);

void mpf_round_deff (void)
{
	mpf_award = 10;
	for (;;)
	{
		
		dmd_alloc_low_clean ();
		font_render_string_center (&font_var5, 64, 5, "BATTLE THE POWER");
		sprintf_current_score ();
		font_render_string_center (&font_fixed6, 64, 16, sprintf_buffer);
		sprintf ("%d,000,000", (mpf_award * mpf_level));
		font_render_string_center (&font_var5, 64, 27, sprintf_buffer);
		sprintf ("%d", mpf_round_timer);
		font_render_string (&font_var5, 2, 2, sprintf_buffer);
		font_render_string_right (&font_var5, 126, 2, sprintf_buffer);
		dmd_show_low ();
		task_sleep_sec (1);
		if (mpf_award > 5)
			bounded_decrement (mpf_award, 0);
	}
}

void mpf_award_deff (void)
{
	dmd_alloc_low_clean ();
	sprintf ("%d,000,000", (mpf_award * mpf_level));
	font_render_string_center (&font_fixed6, 64, 16, sprintf_buffer);
	dmd_show_low ();
	task_sleep_sec (2);
	deff_exit ();
}

void mpf_round_begin (void)
{
	deff_start (DEFF_MPF_ROUND);
}

void mpf_round_expire (void)
{
	deff_stop (DEFF_MPF_ROUND);
	mpf_ball_count = 0 ;
}

void mpf_round_end (void)
{
}

void mpf_round_task (void)
{
	timed_mode_task (mpf_round_begin, mpf_round_expire, mpf_round_end,
		&mpf_round_timer, 10, 3);
}

CALLSET_ENTRY (mpf, display_update)
{
	if (timed_mode_timer_running_p (GID_MPF_ROUND_RUNNING,
		&mpf_round_timer) && (mpf_ball_count > 0))
		deff_start_bg (DEFF_MPF_ROUND, 0);
}

CALLSET_ENTRY (mpf, music_refresh)
{
	if (timed_mode_timer_running_p (GID_MPF_ROUND_RUNNING,
		&mpf_round_timer))
		music_request (MUS_POWERFIELD, PRI_GAME_MODE1);
}

CALLSET_ENTRY (mpf, end_ball)
{
	timed_mode_stop (&mpf_round_timer);
	mpf_ball_count = 0;
}

bool mpf_ready_p (void)
{
	return (mpf_enable_count > 0)
		/* Don't allow if PB might be on playfield */
		&& !flag_test (FLAG_POWERBALL_IN_PLAY)
		&& !(pb_location & PB_MAYBE_IN_PLAY)
		&& !flag_test (FLAG_MULTIBALL_RUNNING)
		&& !flag_test (FLAG_QUICK_MB_RUNNING)
		&& !flag_test (FLAG_BTTZ_RUNNING)
		&& !flag_test (FLAG_CHAOSMB_RUNNING)
		&& !flag_test (FLAG_SSSMB_RUNNING);

}


CALLSET_ENTRY (mpf, lamp_update)
{
	if (mpf_ready_p ())
		lamp_tristate_on (LM_RAMP_BATTLE);
	else
		lamp_tristate_off (LM_RAMP_BATTLE);
}

CALLSET_ENTRY (mpf, door_start_battle_power)
{
	mpf_enable_count++;
	sound_send (SND_ARE_YOU_READY_TO_BATTLE);
}

/* Closing this switch does not imply that the
 * mpf award was collected, but we can tell camera.c
 * to expect a ball coming from the mpf */
CALLSET_ENTRY (mpf, sw_mpf_top)
{
	event_should_follow (mpf_top, camera, TIME_4S);
	sound_send (SND_EXPLOSION_3);
	score (SC_500K);
}

/* Called from camera.c */
CALLSET_ENTRY (mpf, mpf_collected)
{
	flipper_enable ();
	leff_start (LEFF_TURN_ON_GI);
	leff_stop (LEFF_MPF_ACTIVE);
	score_multiple(SC_1M, (mpf_award * mpf_level));
	deff_start (DEFF_MPF_AWARD);
	sound_send (SND_EXPLOSION_3);
	flasher_pulse (FLASH_POWERFIELD);
	if (mpf_ball_count > 0)
		bounded_decrement (mpf_ball_count, 0);
	else	
		timed_mode_stop (&mpf_round_timer);
	//task_sleep_sec (4);
	//door_award_if_possible ();
}

CALLSET_ENTRY (mpf, sw_mpf_enter)
{
	/* If tripped immediately after the right ramp opto, then a ball
	has truly entered the mini-playfield.  Note this may trip later
	on when a ball is already in play. */
	if (event_did_follow (right_ramp, mpf_enter))
	{
		reset_unlit_shots ();
		leff_start (LEFF_MPF_ACTIVE);
		mpf_ball_count++;
		mpf_level++;
		bounded_decrement (mpf_enable_count, 0);
		if (mpf_ball_count = (1))
		{	
			timed_mode_start (GID_MPF_ROUND_RUNNING, mpf_round_task);
			leff_start (LEFF_NO_GI_PERM);
			if (!multi_ball_play ())
				flipper_disable ();
		}
	}
	/* A ball sneaked in during multiball */
	else if (multi_ball_play ())
	{
		sound_send (SND_WITH_THE_DEVIL);
		score (SC_5M);
		//TODO Crazy magnet pulses till exit or 3 secs
	}
}

CALLSET_ENTRY (mpf, sw_mpf_exit)
{
	if (mpf_ball_count > 0)
		bounded_decrement (mpf_ball_count, 0);
	if (mpf_ball_count == 0)
	{
		//leff_stop (LEFF_MPF_ACTIVE);
		leff_start (LEFF_TURN_ON_GI);
		leff_stop (LEFF_MPF_ACTIVE);
		timed_mode_stop (&mpf_round_timer);
		flipper_enable ();
	}
	sound_send (SND_HAHA_POWERFIELD_EXIT);
}
//TODO Light show
CALLSET_ENTRY (mpf, sw_mpf_left)
{
	if (mpf_ball_count > 0)
	{	
		sound_send (SND_POWER_GRUNT_1);
		score (SC_250K);
	}
}

CALLSET_ENTRY (mpf, sw_mpf_right)
{
	if (mpf_ball_count > 0)
	{
		sound_send (SND_POWER_GRUNT_2);
		score (SC_250K);
	}
}


CALLSET_ENTRY (mpf, sw_right_ramp)
{
	/* If the mini-playfield is enabled, open the ramp
	divertor fully.  The ordinary catch and drop is bypassed. */
	event_should_follow (right_ramp, mpf_enter, TIME_3S);
}

CALLSET_ENTRY (mpf, start_player)
{
	mpf_enable_count = 1;
	mpf_level = 0;
}

CALLSET_ENTRY (mpf, ball_search)
{
	sol_request (SOL_MPF_LEFT_MAGNET);
	task_sleep_sec (1);
	sol_request (SOL_MPF_RIGHT_MAGNET);
}
