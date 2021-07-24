#include "g_local.h"
#include "p_radio.h"

void RadioToggle_f(edict_t *self)
{
	if (self->client->resp.radio_power)
	{
		self->client->resp.radio_power = 0;
		gi.cprintf(self, PRINT_HIGH, "Radio OFF\n");
	}
	else
	{
		self->client->resp.radio_power = 1;
		gi.cprintf(self, PRINT_HIGH, "Radio ON\n");
	}
}


// this functions is used to send commands to the clients.
void stuffcmd(edict_t *ent, char *s) 	
{
   	gi.WriteByte (11);	        
	gi.WriteString (s);
	gi.unicast (ent, true);	
}

void Radio_f(edict_t *self, char *channel, char *msg)
{
	edict_t	*player;
	int		i;
	char	*pos;
    char    cmd[1024];
	char	*prefix;
	char	*info;



      // if the players radio is off, lets not bother doing anything
	if (!self->client->resp.radio_power)
		return;

	// Well this is where we check the model to see what sound to send out.
	// Based this check from the IsFemale() Subroutine

	info = Info_ValueForKey (self->client->pers.userinfo, "skin");
	if (info[0] == 'f' || info[0] == 'F')
		prefix = "fem_";
	else if (info[0] == 'c' || info[0] == 'C')
	{
		if (info[1] == 'y' || info[1] == 'Y')
			prefix = "cyb_";
		if (info[1] == 'r' || info[1] == 'R')
			prefix = "crk_";
	}
      // If its an unidentifiable model, use the male sounds by default
	else
		prefix = "male_";

	// Okay gotta make sure people can't stuff extra commands down to kill someone.
	// This is also why i send everything to the guy that sent it originally also
	if (pos = strstr(msg,";"))
		pos[0] = 0;
		
	
      // If the sound is bound for all players in the game this is what is executed
      // .WAVs are stored in sound/world/*.wav
	if (Q_stricmp (channel, "ALL") == 0)
	{
		for_each_player(player, i)
		{
			if (player->client->resp.radio_power)
			{
                //gi.dprintf("RADIO DEBUG : %s\n", msg);
				sprintf(cmd, "play world/%s\n", msg);
				stuffcmd(player, cmd);
			}
		}
		return;
	}
      // If it's only based for players nearby AKA the player is yelling to people in game this is ran.
      // .WAVs are stored in sound/voice/*.wav
	else if (Q_stricmp (channel, "ROOM") == 0)
	{
		sprintf(cmd, "voice/%s.wav", msg);
		gi.sound (self, CHAN_VOICE, gi.soundindex(cmd), 1, ATTN_NORM, 0);
		return;
	}
}