// These are functions for the team radio

#define for_each_player(PLAYER,INDEX)				\
for(INDEX=1;INDEX<=maxclients->value;INDEX++)			\
	if ((PLAYER=&g_edicts[i]) && PLAYER->inuse)


void RadioToggle_f(edict_t *self);

void Radio_f(edict_t *self, char *channel, char *msg);
