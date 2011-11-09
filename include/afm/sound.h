/*
 * Copyright 2006 by Brian Dominy <brian@oddchange.com>
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

#ifndef _MACH_SOUND_H
#define _MACH_SOUND_H

#define MUS_DEFAULT_AT_PLUNGER 0x1
#define MUS_DEFAULT 0x2
#define MUS_HURRYUP 0x5
#define MUS_ANNIHILIATION 0x8
#define MUS_MARTIAN_ATTACK 0x9
#define MUS_MULTIBALL 0xB
#define MUS_STROBE_MB 0xC
#define MUS_SUPER_JACKPOT_LIT 0xF

#define SND_LIT_MARTIAN 0x6A
#define SND_RIGHT_KICKOUT_WARNING 0x73
#define SND_RIGHT_KICKOUT 0x74
#define SND_LEFT_KICKOUT_WARNING 0x8B
#define SND_LEFT_KICKOUT 0x8D
#define SND_BALL_LOCKED 0x94
#define SND_FLYOVER 0x98
#define SND_DRAIN 0x9E
#define SND_SAUCER_FINISH 0xD0
#define SND_LOCK_LIT 0xD4
#define SND_HURRYUP_COLLECT 0xE8
#define SND_EXTRA_BALL_LIT 0xEC
#define SND_MOO 0x0100UL
#define SND_EXPLODE 0x0104UL
#define SND_EXTRA_BALL_COLLECT_1 0x122UL
#define SND_EXTRA_BALL_COLLECT_2 0x148UL
#define SND_RANDOM_CHOICE 0x014EUL
#define SND_SUPER_BLIP 0x0150UL
#define SND_INSERT_COIN 0x152UL
#define SND_APPLAUSE 0x156UL
#define SND_CREDIT_ADDED 0x15CUL
#define SND_TILT_WARNING 0x15EUL

#define SPCH_BIG_O_BEAM 0x192UL
#define SPCH_USE_ATOMIC_BLASTER 0x193UL
#define SPCH_GET_EVERY_MAN 0x196UL
#define SPCH_LOOKOUT_MARTIANS 0x19CUL
#define SPCH_LOOKOUT_MORE_MARTIANS 0x19DUL
#define SPCH_MISSILES_LOCKED 0x19EUL
#define SPCH_TAKE_THAT_MUFFINHEAD 0x1A0UL
#define SPCH_RETURN_TO_BATTLE 0x1A5UL
#define SPCH_SOMEBODY_SHOOT 0x1A7UL
#define SPCH_TOTAL_ANNIHILIATION_HOOHOO 0x1A9UL
#define SPCH_EXTRA_BALL 0x1B5UL
#define SPCH_JACKPOT 0x1B6UL
#define SPCH_MULTIBALL 0x1B8UL
#define SPCH_SHOOT_AGAIN 0x1BEUL
#define SPCH_FIVE_BILLION 0x1C0UL

#endif /* _MACH_SOUND_H */
