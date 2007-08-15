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

/**
 * \file
 * \brief Simple debugger interface
 *
 * This module reads/writes to the WPC debugger port.  A simple
 * companion utility has been written to display these messages,
 * and to pass input keystrokes to the running program.  This
 * requires a patched version of pinmame.  On real hardware, these
 * functions are not required.
 *
 * Define DEBUGGER if you want to compile in debugger support.
 * Without it, the program will run slightly faster, and be
 * considerably smaller.
 *
 * Also, you can now define CONFIG_PARALLEL_DEBUG if you want the
 * debug output to go out the parallel port instead.
 */
#include <freewpc.h>
#ifdef __m6809__
#include <m6809/math.h>
#else
#include <native/math.h>
#endif

/** Initially zero, this is set to 1 when a remote debugger is
 * detected */
U8 db_attached;


/** Nonzero when the system (all tasks except for interrupts)
is paused */
U8 db_paused;



/** Read a character from the debug port, and wait for it if
 * it isn't there yet. */
U8 db_read_sync (void)
{
	while (!wpc_debug_read_ready ())
	{
		task_dispatching_ok = TRUE;
	}
	return wpc_debug_read ();
}


U16 db_read_address (void)
{
	U16 addr = 0;
	U16 i;

	for (i=0 ; i < 4; i++)
	{
		U8 c = db_read_sync ();
		if (c > '9')
			c = c - 'A' + 10;
		else
			c = c - '0';
		addr = (addr << 4) | c;
	}
	return addr;
}


void db_idle (void)
{
#ifdef DEBUGGER
	if (!db_attached)
	{
		if (wpc_debug_read_ready ())
		{
			wpc_debug_write (0);
			db_attached = 1;
#ifdef CONFIG_SHORT_STRINGS_ONLY
			db_puts ("FREEWPC\n");
#else
			db_puts ("\n\n"
				"----------------------------------------------------------\n"
				"FREEWPC DEBUGGER\n"
				"----------------------------------------------------------\n\n");
#endif
		}
	}
	else
	{
		if (wpc_debug_read_ready ())
		{
			char c = wpc_debug_read (); 
			switch (c)
			{
				case 'a':
					SECTION_VOIDCALL (__common__, audio_dump);
					break;

				case 't':
					VOIDCALL (task_dump);
					break;

				case 'g':
					VOIDCALL (dump_game);
					break;

				case 'd':
					VOIDCALL (dump_deffs);
					break;

				case 'r':
				{
					U8 *addr = (U8 *)db_read_address ();
					dbprintf ("%02X\n", *addr);
					break;
				}

				case 'p':
				{
					/* Toggle the pause state.  When paused, tasks
					do not run and the system polls for debugger
					commands in a hard loop. */
					db_paused = 1 - db_paused;
					while (db_paused == 1)
					{
						task_dispatching_ok = TRUE;
						db_idle ();
					}
					break;
				}

				default:
				{
#ifdef MACHINE_DEBUGGER_HOOK
					/* Allow the machine to define additional commands.
					 * This function must reside in the system page. */
					extern void MACHINE_DEBUGGER_HOOK (U8);
					MACHINE_DEBUGGER_HOOK (c);
#endif
					break;
				}
			}
		}
	}
#endif /* DEBUGGER */
}


/** Initialize the debugger */
void db_init (void)
{
#ifdef CONFIG_PLATFORM_LINUX
	db_attached = 1;
#else
#ifdef CONFIG_PARALLEL_DEBUG
	db_attached = 1;
#else
	db_attached = 0;
#endif
#endif
	db_paused = 0;
}

