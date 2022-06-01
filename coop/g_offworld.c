
//****************************************************
//		Offworld Teleports
//****************************************************

//
// Original concept and code by Phlem<WOS> 08/29/2005
// Spawn function and revisions by QwazyWabbit<WOS> 06/03/2006
//

#include "g_local.h"


/*
To use this module in Quake II:

In the spawn function hash at spawn_t spawns[] in g_spawn.c, add:
	{"misc_offworld_teleporter", SP_misc_teleporter_offworld},

In the prototype list
void SP_misc_teleporter_offworld (edict_t *self);

Everything else in this module is private.
*/

void offworld_teleporter_touch(edict_t* self, edict_t* other, cplane_t* plane, csurface_t* surf)
{

	//=================================================================================
	//  offworld_teleporter_touch - Expanded to include transporting to other servers
	// 
	// Working implementation summary:
	// 
	// 1 - User creates map with teleporter target set to special syntax:
	//
	//
	//   example:    q2@67.15.20.10:27910
	//
	//        where:  "q2@" indicates that this is an external q2 server
	//                "67.15.20.10:27910" indicates the IP address (or DNS name)
	//                     and port number of external server
	//
	// 2 - For good behavior on systems not supporting this feature, always
	//     include a teleport destination pad with same label.  This can be 
	//     hidden or simply placed in a back room of the map to take it out of view. 
	//
	// 3 - Supports launching of other game types, based on environment variables:
	//   
	//      q3    indicates a Quake3 game - Env Variable Q3_LAUNCHER will send appropriate
	//               launch command
	// 
	//      hl    indicates a Half-Life game - Env Variable is HL_LAUNCHER 
	//
	//      h2    indicates a Half-Life 2 game - Env Variable is H2_LAUNCHER
	// 
	//      bf    indicates a BF1942 Server - Env Variable is BF_LAUNCHER
	//
	//      b2    BF2 Server - Env is B2_LAUNCHER
	// 
	//      ef    Elite Force Server - Env is EF_LAUNCHER
	//   
	//      e2	  Elite Force 2 Server - Env is E2_LAUNCHER
	//    
	//      u9    Unreal 99 Server - Env is U9_LAUNCHER
	//
	// 4 - If game is NON-Q2, the current Q2 game will be exited just after spawning
	//     of sub-process to start external game.
	//
	// 5 - A "I went to Server X" message is displayed in-game just prior to launch.
	// 
	// NOTE: Code for non-Q2 games is still in progress.
	//
	//=================================================================================

	int			i, j, k;
	char		external_server[80];
	char		exit_message[256];
	char		connect_message[120];

	if (!other->client) // touched by someone we don't know :)
		return;

	Com_sprintf(external_server, sizeof(external_server), "%s", self->target);

	if (strstr(external_server, "q2@") != NULL)  // we have a match for remote q2 server
	{
		i = sprintf(exit_message, "say Offworld transport to %s ...\n", external_server);
		if (i != -1)
			stuffcmd(other, exit_message);
		else
		{
			gi.cprintf(other, PRINT_HIGH, "Server exit message format error.\n");
			return;
		}
		k = (int)strlen(external_server);

		for (j = 3; j < k; j++)
			external_server[j - 3] = external_server[j];

		external_server[k - 3] = 0;
		Com_sprintf(connect_message, sizeof(connect_message), "connect %s\n", external_server);
		stuffcmd(other, connect_message);
	}

	//
	// handling for other games would go here
	//

	// catch any target format problems here
	else
	{
		gi.cprintf(other, PRINT_HIGH, "Server target format error.\n");
		return;
	}
}

// the wait time has passed, so set back up for another activation
void pad_wait(edict_t* ent)
{
	ent->nextthink = 0;
}

void offworld_teleporter_approach(edict_t* self, edict_t* other, cplane_t* plane, csurface_t* surf)
{
	if (!other->client)
		return;		// not a player

	if (self->nextthink)
		return;		// already been triggered

	gi.centerprintf(other, self->message);
	gi.sound(other, CHAN_AUTO, gi.soundindex("misc/talk1.wav"), 1, ATTN_NORM, 0);

	if (self->wait > 0)
	{
		self->think = pad_wait;
		self->nextthink = level.time + self->wait;
	}
	else
	{	// we can't just remove (self) here, because this is a touch function
		// called while looping through area links...
		self->touch = NULL;
		self->nextthink = level.time + FRAMETIME;
		self->think = G_FreeEdict;
	}
}

/*QUAKED misc_offworld_teleporter (1 0 0) (-32 -32 -24) (32 32 -16)
Stepping onto this pad will teleport players to a different server.
*/

/* our new entity:
{
"message" "Offworld transport to\nLOXophilia\n(67.15.20.10:27910)"
"wait" "3"
"sounds" "2"
"target" "q2@67.15.20.10"
"origin" "576 736 116"
"classname" "misc_offworld_teleporter"
}
*/

void SP_misc_teleporter_offworld(edict_t* ent)
{
	edict_t* trig, * trig1;

	if (!ent->target || !ent->message)
	{
		gi.dprintf("Teleporter is missing target or message %s %s %s.\n",
			ent->classname, ent->target, ent->message);
		G_FreeEdict(ent);
		return;
	}

	gi.setmodel(ent, "models/objects/dmspot/tris.md2");
	ent->s.skinnum = 1;
	ent->s.effects = EF_TELEPORTER | EF_ROTATE; // obviously different pads
	ent->s.sound = gi.soundindex("world/amb10.wav");
	ent->solid = SOLID_BBOX;

	VectorSet(ent->mins, -32, -32, -24);
	VectorSet(ent->maxs, 32, 32, -16);
	gi.linkentity(ent);

	// create the notification zone around the pad
	trig = G_Spawn();
	trig->touch = offworld_teleporter_approach;
	trig->solid = SOLID_TRIGGER;
	trig->target = ent->target; // the target game and IP
	trig->owner = ent;
	trig->message = ent->message; //the text message to send
	if (!ent->sounds)	// index for multiple sound file selection (not implemented)
		ent->sounds = 1;	// was 2
	trig->sounds = ent->sounds;
	if (!ent->wait)
		ent->wait = 3.0f;	//if added to map it'll probably be an int.
	trig->wait = ent->wait;
	trig->classname = "trigger_multiple";

	VectorCopy(ent->s.origin, trig->s.origin);
	VectorSet(trig->mins, -120, -120, 8);	// boundaries for the touch planes
	VectorSet(trig->maxs, 120, 120, 24);
	gi.linkentity(trig);

	// create the touch zone on the pad
	trig1 = G_Spawn();
	trig1->touch = offworld_teleporter_touch;
	trig1->solid = SOLID_TRIGGER;
	trig1->target = ent->target; // the target game and IP
	trig1->message = ent->message; //the text message to send
	trig1->owner = ent;
	trig1->classname = ent->classname;

	VectorCopy(ent->s.origin, trig1->s.origin);
	VectorSet(trig1->mins, -8, -8, 8);
	VectorSet(trig1->maxs, 8, 8, 24);
	gi.linkentity(trig1);

}
