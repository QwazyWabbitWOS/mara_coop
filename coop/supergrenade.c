#include "g_local.h"

//functions from maj.bitch start
void G_Spawn_Explosion(int type, vec3_t start, vec3_t origin) {
	gi.WriteByte(svc_temp_entity);
	gi.WriteByte(type);
	gi.WritePosition(start);
	gi.multicast(origin, MULTICAST_PVS);
}

qboolean G_EntExists(edict_t* ent) {
	return ((ent) && (ent->client) && (ent->inuse));
}
//functions from maj.bitch end

void earthquake_think(edict_t* self) {
	int i;
	edict_t* e;

	if (self->last_move_time < level.time) {
		gi.positioned_sound(self->s.origin, self, CHAN_AUTO, self->noise_index, 1.0, ATTN_NONE, 0);
		self->last_move_time = level.time + 0.5;
	}

	for (i = 1, e = g_edicts + i; i < globals.num_edicts; i++, e++) {
		if (!e->inuse)
			continue;
		if (!e->client)
			continue;
		if (!e->groundentity)
			continue;

		e->groundentity = NULL;
		e->velocity[0] += crandom() * 150;
		e->velocity[1] += crandom() * 150;
		e->velocity[2] = self->speed * (100.0 / e->mass);
	}

	if (level.time < self->timestamp)
		self->nextthink = level.time + FRAMETIME;
	else
		G_FreeEdict(self);
}

void earthquake(edict_t* ent) {
	edict_t* self;

	self = G_Spawn();

	self->count = 10;
	self->speed = 600;
	self->svflags |= SVF_NOCLIENT;
	self->think = earthquake_think;
	self->timestamp = level.time + self->count;
	self->nextthink = level.time + FRAMETIME;
	self->last_move_time = 0;

	self->noise_index = gi.soundindex("world/quake.wav");
}

void SG_Think2(edict_t* ent) {
	G_Spawn_Explosion(TE_EXPLOSION2, ent->s.origin, ent->s.origin);
	T_RadiusDamage(ent, ent->activator, 5000, NULL, 10000, MOD_BFG_BLAST);
	earthquake(ent);
	G_FreeEdict(ent);
}

void SG_Think(edict_t* ent) {
	ent->owner = NULL;
	ent->nextthink = level.time + 0.1;

	if (ent->timer < level.time)
		ent->think = SG_Think2;

}

static void SG_Touch(edict_t* ent, edict_t* other, cplane_t* plane, csurface_t* surf) {
	if (!G_EntExists(other))
		return;

	if (surf && (surf->flags & SURF_SKY)) {
		G_FreeEdict(ent);
		return;
	}

	if (!other->takedamage)
		return;
	else
		SG_Think2(ent);

}

void Super_Grenade(edict_t* ent, vec3_t start, vec3_t aimdir, int speed) {
	edict_t* SG;
	vec3_t dir;
	vec3_t forward, right, up;

	vectoangles(aimdir, dir);
	AngleVectors(dir, forward, right, up);

	if (ent->client->pers.inventory[ent->client->ammo_index] < 10)
		return;
	else
		ent->client->pers.inventory[ent->client->ammo_index] -= 10;

	SG = G_Spawn();
	SG->classname = "SuperGrenade";
	VectorCopy(start, SG->s.origin);
	VectorScale(aimdir, speed, SG->velocity);
	VectorMA(SG->velocity, 200 + crandom() * 10.0, up, SG->velocity);
	VectorMA(SG->velocity, crandom() * 10.0, right, SG->velocity);
	VectorSet(SG->avelocity, 300, 300, 300);
	SG->movetype = MOVETYPE_BOUNCE;
	SG->solid = SOLID_BBOX;
	SG->s.effects = 0;
	VectorSet(SG->mins, -3, -3, 0);
	VectorSet(SG->maxs, 3, 3, 6);
	SG->owner = ent;
	SG->activator = ent;
	SG->clipmask = MASK_SHOT;
	SG->s.renderfx |= RF_FULLBRIGHT;
	SG->s.renderfx |= RF_SHELL_RED | RF_SHELL_GREEN | RF_SHELL_BLUE;
	SG->s.skinnum = 0;
	SG->s.modelindex = gi.modelindex("models/objects/flash/tris.md2");
	SG->touch = SG_Touch;
	SG->mass = 100;
	SG->nextthink = 5.0;
	SG->think = SG_Think;
	SG->timer = level.time + 15;

	gi.linkentity(SG);

}

void SG_Away(edict_t* ent) {
	vec3_t offset = { 0 };
	vec3_t forward, right;
	vec3_t start;

	VectorSet(offset, 8, 8, ent->viewheight - 8);
	AngleVectors(ent->client->v_angle, forward, right, NULL);
	P_ProjectSource(ent->client, ent->s.origin, offset, forward, right, start);

	Super_Grenade(ent, start, forward, 700);

	ent->client->ps.gunframe++;
}

void Weapon_SuperGrenade(edict_t* ent) {
	static int pause_frames[] = {
		34,
		51,
		59,
		0
	};
	static int fire_frames[] = {
		6,
		0
	};

	Weapon_Generic(ent, 5, 16, 59, 64, pause_frames, fire_frames, SG_Away);
}