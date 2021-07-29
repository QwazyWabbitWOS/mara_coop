
#include "g_local.h"
#include "p_hook.h"
#include "m_player.h"

static edict_t *current_player;
static gclient_t *current_client;

static vec3_t forward, right, up;
float xyspeed;

float bobmove;
int bobcycle;
float bobfracsin;

void updateVisorHud(edict_t *ent); /* FS: Zaero specific game dll changes */
void startVisorStatic(edict_t *ent); /* FS: Zaero specific game dll changes */
void stopCamera(edict_t *self); /* FS: Zaero specific game dll changes */

float
SV_CalcRoll(vec3_t angles, vec3_t velocity)
{
	float sign;
	float side;
	float value;

	side = DotProduct(velocity, right);
	sign = side < 0 ? -1 : 1;
	side = fabs(side);

	value = sv_rollangle->value;

	if (side < sv_rollspeed->value)
	{
		side = side * value / sv_rollspeed->value;
	}
	else
	{
		side = value;
	}

	return side * sign;
}

/*
 * Handles color blends and view kicks
 */
void
P_DamageFeedback(edict_t *player)
{
	gclient_t *client;
	float side;
	float realcount, count, kick;
	vec3_t v;
	int r, l;
	static vec3_t power_color = {0.0, 1.0, 0.0};
	static vec3_t acolor = {1.0, 1.0, 1.0};
	static vec3_t bcolor = {1.0, 0.0, 0.0};

	if (!player)
	{
		return;
	}

	client = player->client;

	/* flash the backgrounds behind the status numbers */
	client->ps.stats[STAT_FLASHES] = 0;

	if (client->damage_blood)
	{
		client->ps.stats[STAT_FLASHES] |= 1;
	}

	if (client->damage_armor && !(player->flags & FL_GODMODE) &&
		(client->invincible_framenum <= level.framenum))
	{
		client->ps.stats[STAT_FLASHES] |= 2;
	}

	/* total points of damage shot at the player this frame */
	count = (client->damage_blood + client->damage_armor + client->damage_parmor);

	if (count == 0)
	{
		return; /* didn't take any damage */
	}

	/* start a pain animation if still in the player model */
	if ((client->anim_priority < ANIM_PAIN) && (player->s.modelindex == 255))
	{
		static int i;

		client->anim_priority = ANIM_PAIN;

		if (client->ps.pmove.pm_flags & PMF_DUCKED)
		{
			player->s.frame = FRAME_crpain1 - 1;
			client->anim_end = FRAME_crpain4;
		}
		else
		{
			i = (i + 1) % 3;

			switch (i)
			{
				case 0:
					player->s.frame = FRAME_pain101 - 1;
					client->anim_end = FRAME_pain104;
					break;
				case 1:
					player->s.frame = FRAME_pain201 - 1;
					client->anim_end = FRAME_pain204;
					break;
				case 2:
					player->s.frame = FRAME_pain301 - 1;
					client->anim_end = FRAME_pain304;
					break;
			}
		}
	}

	realcount = count;

	if (count < 10)
	{
		count = 10; /* always make a visible effect */
	}

	/* play an apropriate pain sound */
	if ((level.time > player->pain_debounce_time) &&
		!(player->flags & FL_GODMODE) &&
		(client->invincible_framenum <= level.framenum))
	{
		r = 1 + (rand() & 1);
		player->pain_debounce_time = level.time + 0.7;

		if (player->health < 25)
		{
			l = 25;
		}
		else if (player->health < 50)
		{
			l = 50;
		}
		else if (player->health < 75)
		{
			l = 75;
		}
		else
		{
			l = 100;
		}

		gi.sound(player, CHAN_VOICE, gi.soundindex(va("*pain%i_%i.wav", l, r)), 1, ATTN_NORM, 0);
	}

	/* the total alpha of the blend is always proportional to count */
	if (client->damage_alpha < 0)
	{
		client->damage_alpha = 0;
	}

	client->damage_alpha += count * 0.01;

	if (client->damage_alpha < 0.2f)
	{
		client->damage_alpha = 0.2f;
	}

	if (client->damage_alpha > 0.6f)
	{
		client->damage_alpha = 0.6f; /* don't go too saturated */
	}

	/* the color of the blend will vary based on how
	   much was absorbed by different armors */
	VectorClear(v);

	if (client->damage_parmor)
	{
		VectorMA(v, (float)client->damage_parmor / realcount, power_color, v);
	}

	if (client->damage_armor)
	{
		VectorMA(v, (float)client->damage_armor / realcount, acolor, v);
	}

	if (client->damage_blood)
	{
		VectorMA(v, (float)client->damage_blood / realcount, bcolor, v);
	}

	VectorCopy(v, client->damage_blend);

	/* calculate view angle kicks */
	kick = abs(client->damage_knockback);

	if (kick && (player->health > 0)) /* kick of 0 means no view adjust at all */
	{
		kick = kick * 100 / player->health;

		if (kick < count * 0.5)
		{
			kick = count * 0.5;
		}

		if (kick > 50)
		{
			kick = 50;
		}

		VectorSubtract(client->damage_from, player->s.origin, v);
		VectorNormalize(v);

		side = DotProduct(v, right);
		client->v_dmg_roll = kick * side * 0.3;

		side = -DotProduct(v, forward);
		client->v_dmg_pitch = kick * side * 0.3;

		client->v_dmg_time = level.time + DAMAGE_TIME;
	}

	/* clear totals */
	client->damage_blood = 0;
	client->damage_armor = 0;
	client->damage_parmor = 0;
	client->damage_knockback = 0;
}

/*
 * Auto pitching on slopes?
 *
 * fall from 128: 400 = 160000
 * fall from 256: 580 = 336400
 * fall from 384: 720 = 518400
 * fall from 512: 800 = 640000
 * fall from 640: 960 =
 *
 * damage = deltavelocity*deltavelocity  * 0.0001
 */
void
SV_CalcViewOffset(edict_t *ent)
{
	float *angles;
	float bob;
	float ratio;
	float delta;
	vec3_t v;

	if (!ent)
	{
		return;
	}


	/* base angles */
	angles = ent->client->ps.kick_angles;

	/* if dead, fix the angle and don't add any kick */
	if (ent->deadflag)
	{
		VectorClear(angles);

		if ((game.gametype == rogue_coop) && (ent->flags & FL_SAM_RAIMI)) /* FS: Coop: Rogue specific */
		{
			ent->client->ps.viewangles[ROLL] = 0;
			ent->client->ps.viewangles[PITCH] = 0;
		}
		else
		{
			ent->client->ps.viewangles[ROLL] = 40;
			ent->client->ps.viewangles[PITCH] = -15;
		}

		ent->client->ps.viewangles[YAW] = ent->client->killer_yaw;
	}
	else
	{
		/* add angles based on weapon kick */
		VectorCopy(ent->client->kick_angles, angles);

		/* add angles based on damage kick */
		ratio = (ent->client->v_dmg_time - level.time) / DAMAGE_TIME;

		if (ratio < 0)
		{
			ratio = 0;
			ent->client->v_dmg_pitch = 0;
			ent->client->v_dmg_roll = 0;
		}

		angles[PITCH] += ratio * ent->client->v_dmg_pitch;
		angles[ROLL] += ratio * ent->client->v_dmg_roll;

		/* add pitch based on fall kick */
		ratio = (ent->client->fall_time - level.time) / FALL_TIME;

		if (ratio < 0)
		{
			ratio = 0;
		}

		angles[PITCH] += ratio * ent->client->fall_value;

		/* add angles based on velocity */
		delta = DotProduct(ent->velocity, forward);
		angles[PITCH] += delta * run_pitch->value;

		delta = DotProduct(ent->velocity, right);
		angles[ROLL] += delta * run_roll->value;

		/* add angles based on bob */
		delta = bobfracsin * bob_pitch->value * xyspeed;

		if (ent->client->ps.pmove.pm_flags & PMF_DUCKED)
		{
			delta *= 6; /* crouching */
		}

		angles[PITCH] += delta;
		delta = bobfracsin * bob_roll->value * xyspeed;

		if (ent->client->ps.pmove.pm_flags & PMF_DUCKED)
		{
			delta *= 6; /* crouching */
		}

		if (bobcycle & 1)
		{
			delta = -delta;
		}

		angles[ROLL] += delta;
	}

	/* =================================== */

	/* base origin */
	VectorClear(v);

	/* add view height */
	v[2] += ent->viewheight;

	/* add fall height */
	ratio = (ent->client->fall_time - level.time) / FALL_TIME;

	if (ratio < 0)
	{
		ratio = 0;
	}

	v[2] -= ratio * ent->client->fall_value * 0.4f;

	/* add bob height */
	bob = bobfracsin * xyspeed * bob_up->value;

	if (bob > 6)
	{
		bob = 6;
	}

	v[2] += bob;

	/* add kick offset */
	VectorAdd(v, ent->client->kick_origin, v);

	/* absolutely bound offsets so the view can
	   never be outside the player box */

	if (ent->client->blinky_client.cam_target)
	{
		Blinky_CalcViewOffsets(ent, v); /* FS: Blinky's Coop Camera */
		VectorCopy(v, ent->client->ps.viewoffset);
	}
	else
	{
		if(ent->client->zCameraTrack) /* FS: Zaero specific game dll changes */
		{
			int i;

			VectorAdd(ent->client->zCameraTrack->s.origin, ent->client->zCameraOffset, v);

			if(ent->client->zCameraTrack->client)
			{
				vec3_t f;

				VectorAdd(ent->client->zCameraTrack->client->ps.viewoffset, v, v);
				AngleVectors (ent->client->zCameraTrack->s.angles, f, NULL, NULL);
				VectorMA(v, 10, f, v);
			}
			else if (ent->client->zCameraTrack->classname && Q_stricmp(ent->client->zCameraTrack->classname, "misc_securitycamera") == 0)
			{
				float framepercent = sin(((float)(level.framenum & 63) / 64.0) * M_PI * 2);
				VectorCopy(ent->client->zCameraTrack->move_origin, v);
				VectorCopy(ent->client->zCameraTrack->move_angles, ent->client->ps.viewangles);

				// adjust yaw a bit due to sway
				ent->client->ps.viewangles[YAW] += framepercent * 15;
			}
			else
			{
				VectorCopy (ent->client->zCameraTrack->s.angles, ent->client->ps.viewangles);  
			}

			for(i = 0; i < 3; i++)
			{
				ent->client->ps.pmove.origin[i] = v[i] * 8;
			}

			VectorSet (ent->client->ps.viewoffset, 0, 0, 0);
			// make our "double" do what we're doing
			if (ent->client->zCameraLocalEntity)
			{
				edict_t *e = ent->client->zCameraLocalEntity;
				VectorCopy(ent->s.origin, e->s.origin);
				e->s.frame = ent->s.frame;
				e->s.modelindex = ent->s.modelindex;
				e->s.modelindex2 = ent->s.modelindex2;
				e->s.skinnum = ent->s.skinnum;
			}
		}
		else
		{
			if (v[0] < -14)
			{
				v[0] = -14;
			}
			else if (v[0] > 14)
			{
				v[0] = 14;
			}

			if (v[1] < -14)
			{
				v[1] = -14;
			}
			else if (v[1] > 14)
			{
				v[1] = 14;
			}

			if (v[2] < -22)
			{
				v[2] = -22;
			}
			else if (v[2] > 30)
			{
				v[2] = 30;
			}

			VectorCopy(v, ent->client->ps.viewoffset);
		}
	}
}

void
SV_CalcGunOffset(edict_t *ent)
{
	int i;
	float delta;
	static gitem_t *heatbeam;
	static gitem_t *sniper;

	if (!ent)
	{
		return;
	}

	if (!heatbeam) /* FS: Coop: Rogue specific.  Probably OK to leave as-is. */
	{
		heatbeam = FindItemByClassname("weapon_plasmabeam");
	}

	if (!sniper) /* FS: Zaero specific */
	{
		sniper = FindItemByClassname("weapon_snipperrifle");
	}

	/* heatbeam and sniper shouldn't bob. */
	if ((ent->client->pers.weapon) && (ent->client->pers.weapon != heatbeam) && (ent->client->pers.weapon != sniper))
	{
		/* gun angles from bobbing */
		ent->client->ps.gunangles[ROLL] = xyspeed * bobfracsin * 0.005;
		ent->client->ps.gunangles[YAW] = xyspeed * bobfracsin * 0.01;

		if (bobcycle & 1)
		{
			ent->client->ps.gunangles[ROLL] = -ent->client->ps.gunangles[ROLL];
			ent->client->ps.gunangles[YAW] = -ent->client->ps.gunangles[YAW];
		}

		ent->client->ps.gunangles[PITCH] = xyspeed * bobfracsin * 0.005;

		/* gun angles from delta movement */
		for (i = 0; i < 3; i++)
		{
			delta = ent->client->oldviewangles[i] -
					ent->client->ps.viewangles[i];

			if (delta > 180)
			{
				delta -= 360;
			}

			if (delta < -180)
			{
				delta += 360;
			}

			if (delta > 45)
			{
				delta = 45;
			}

			if (delta < -45)
			{
				delta = -45;
			}

			if (i == YAW)
			{
				ent->client->ps.gunangles[ROLL] += 0.1 * delta;
			}

			ent->client->ps.gunangles[i] += 0.2 * delta;
		}
	}
	else
	{
		for (i = 0; i < 3; i++)
		{
			ent->client->ps.gunangles[i] = 0;
		}
	}

	/* gun height */
	VectorClear(ent->client->ps.gunoffset);

	/* gun_x / gun_y / gun_z are development tools */
	for (i = 0; i < 3; i++)
	{
		ent->client->ps.gunoffset[i] += forward[i] * (gun_y->value);
		ent->client->ps.gunoffset[i] += right[i] * gun_x->value;
		ent->client->ps.gunoffset[i] += up[i] * (-gun_z->value);
	}
}

void
SV_AddBlend(float r, float g, float b, float a, float *v_blend)
{
	float a2, a3;

	if (!v_blend)
	{
		return;
	}

	if (a <= 0)
	{
		return;
	}

	a2 = v_blend[3] + (1 - v_blend[3]) * a; /* new total alpha */
	a3 = v_blend[3] / a2; /* fraction of color from old */

	v_blend[0] = v_blend[0] * a3 + r * (1 - a3);
	v_blend[1] = v_blend[1] * a3 + g * (1 - a3);
	v_blend[2] = v_blend[2] * a3 + b * (1 - a3);
	v_blend[3] = a2;
}

void
SV_CalcBlend(edict_t *ent)
{
	int contents;
	vec3_t vieworg;
	int remaining;

	if (!ent)
	{
		return;
	}

	ent->client->ps.blend[0] = ent->client->ps.blend[1] = ent->client->ps.blend[2] = ent->client->ps.blend[3] = 0;

	/* add for contents */
	VectorAdd(ent->s.origin, ent->client->ps.viewoffset, vieworg);
	contents = gi.pointcontents(vieworg);

	if (contents & (CONTENTS_LAVA | CONTENTS_SLIME | CONTENTS_WATER))
	{
		ent->client->ps.rdflags |= RDF_UNDERWATER;
	}
	else
	{
		ent->client->ps.rdflags &= ~RDF_UNDERWATER;
	}

	if (contents & (CONTENTS_SOLID | CONTENTS_LAVA))
	{
		SV_AddBlend(1.0, 0.3f, 0.0, 0.6f, ent->client->ps.blend);
	}
	else if (contents & CONTENTS_SLIME)
	{
		SV_AddBlend(0.0, 0.1f, 0.05f, 0.6f, ent->client->ps.blend);
	}
	else if (contents & CONTENTS_WATER)
	{
		SV_AddBlend(0.5, 0.3f, 0.2f, 0.4f, ent->client->ps.blend);
	}

	/* add for powerups */
	if (ent->client->quad_framenum > level.framenum)
	{
		remaining = ent->client->quad_framenum - level.framenum;

		if (remaining == 30) /* beginning to fade */
		{
			gi.sound(ent, CHAN_ITEM, gi.soundindex("items/damage2.wav"), 1, ATTN_NORM, 0);
		}

		if ((remaining > 30) || (remaining & 4))
		{
			SV_AddBlend(0, 0, 1, 0.08f, ent->client->ps.blend);
		}
	}
	else if (ent->client->double_framenum > level.framenum) /* FS: Coop: Rogue specific */
	{
		remaining = ent->client->double_framenum - level.framenum;

		if (remaining == 30) /* beginning to fade */
		{
			gi.sound(ent, CHAN_ITEM, gi.soundindex("misc/ddamage2.wav"), 1, ATTN_NORM, 0);
		}

		if ((remaining > 30) || (remaining & 4))
		{
			SV_AddBlend(0.9f, 0.7f, 0, 0.08f, ent->client->ps.blend);
		}
	}
	else if (ent->client->quadfire_framenum > level.framenum) /* FS: Coop: Xatrix specific */
	{
		remaining = ent->client->quadfire_framenum - level.framenum;

		if (remaining == 30) /* beginning to fade */
		{
			gi.sound(ent, CHAN_ITEM, gi.soundindex("items/quadfire2.wav"),
				   	1, ATTN_NORM, 0);
		}

		if ((remaining > 30) || (remaining & 4))
		{
			SV_AddBlend(1, 0.2f, 0.5, 0.08f, ent->client->ps.blend);
		}
	}
	else if (ent->client->invincible_framenum > level.framenum)
	{
		remaining = ent->client->invincible_framenum - level.framenum;

		if (remaining == 30) /* beginning to fade */
		{
			gi.sound(ent, CHAN_ITEM, gi.soundindex("items/protect2.wav"), 1, ATTN_NORM, 0);
		}

		if (ent->client->spawn_protection) /* FS: Coop: Spawn protection */
		{
			if(ent->client->spawn_protection_msg)
			{
				char bSeconds[9]; /* FS: Boolean to detect if it's 1.5 seconds or lower for spawn protection time */

				if (sv_spawn_protection_time->value >= 1.5)
					strncpy(bSeconds, "seconds", sizeof(bSeconds)-1);
				else
					strncpy(bSeconds, "second", sizeof(bSeconds)-1);

				gi.cprintf(ent, PRINT_HIGH, "Spawn protection expires in %1.f %s.\n", sv_spawn_protection_time->value, bSeconds);

				ent->client->spawn_protection_msg = false;
			}

			if(remaining <= 30)
			{
				ent->client->spawn_protection = false;
				ent->solid = SOLID_BBOX; /* FS: We're done being SOLID_NOTs, get back to normal. */
				gi.unlinkentity(ent); /* FS: Have to unlink temporarily for KillBox to work. */
				KillBox(ent);
				gi.linkentity(ent);
			}
		}

		if ((remaining > 30) || (remaining & 4))
		{
			SV_AddBlend(1, 1, 0, 0.08f, ent->client->ps.blend);
		}
	}
	else if (ent->client->enviro_framenum > level.framenum)
	{
		remaining = ent->client->enviro_framenum - level.framenum;

		if (remaining == 30) /* beginning to fade */
		{
			gi.sound(ent, CHAN_ITEM, gi.soundindex("items/airout.wav"), 1, ATTN_NORM, 0);
		}

		if ((remaining > 30) || (remaining & 4))
		{
			SV_AddBlend(0, 1, 0, 0.08f, ent->client->ps.blend);
		}
	}
	else if (ent->client->breather_framenum > level.framenum)
	{
		remaining = ent->client->breather_framenum - level.framenum;

		if (remaining == 30) /* beginning to fade */
		{
			gi.sound(ent, CHAN_ITEM, gi.soundindex("items/airout.wav"), 1, ATTN_NORM, 0);
		}

		if ((remaining > 30) || (remaining & 4))
		{
			SV_AddBlend(0.4f, 1, 0.4f, 0.04f, ent->client->ps.blend);
		}
	}

	if (ent->client->nuke_framenum > level.framenum) /* FS: Coop: Rogue specific */
	{
		float brightness;
		brightness = (ent->client->nuke_framenum - level.framenum) / 20.0f;
		SV_AddBlend(1, 1, 1, brightness, ent->client->ps.blend);
	}

	if (ent->client->ir_framenum > level.framenum) /* FS: Coop: Rogue specific */
	{
		remaining = ent->client->ir_framenum - level.framenum;

		if ((remaining > 30) || (remaining & 4))
		{
			ent->client->ps.rdflags |= RDF_IRGOGGLES;
			SV_AddBlend(1, 0, 0, 0.2f, ent->client->ps.blend);
		}
		else
		{
			ent->client->ps.rdflags &= ~RDF_IRGOGGLES;
		}
	}
	else
	{
		ent->client->ps.rdflags &= ~RDF_IRGOGGLES;
	}

	/* add for damage */
	if (ent->client->damage_alpha > 0)
	{
		SV_AddBlend(ent->client->damage_blend[0],
				ent->client->damage_blend[1],
				ent->client->damage_blend[2],
				ent->client->damage_alpha,
				ent->client->ps.blend);
	}

	if (ent->client->bonus_alpha > 0)
	{
		SV_AddBlend(0.85f, 0.7f, 0.3f, ent->client->bonus_alpha, ent->client->ps.blend);
	}

	if (game.gametype == zaero_coop) /* FS: Zaero specific game dll changes */
	{
		/* for blinding */
		if (ent->client->flashTime > 0)
		{
			float alpha = (float)ent->client->flashTime / (float)ent->client->flashBase;
			if (alpha > 1)
				alpha = 1;
			SV_AddBlend(1, 1, 1, alpha, ent->client->ps.blend);
			ent->client->flashTime--;
		}

		if (ent->client->zCameraStaticFramenum > level.time)
		{
			SV_AddBlend(1,1,1,1, ent->client->ps.blend);
		}
	}

	/* drop the damage value */
	ent->client->damage_alpha -= 0.06f;

	if (ent->client->damage_alpha < 0)
	{
		ent->client->damage_alpha = 0;
	}

	/* drop the bonus value */
	ent->client->bonus_alpha -= 0.1f;

	if (ent->client->bonus_alpha < 0)
	{
		ent->client->bonus_alpha = 0;
	}
}

void
P_FallingDamage(edict_t *ent)
{
	float delta;
	int damage;
	vec3_t dir;

	if (!ent)
	{
		return;
	}

	if (ent->s.modelindex != 255)
	{
		return; /* not in the player model */
	}

	if (ent->movetype == MOVETYPE_NOCLIP)
	{
		return;
	}

	if ((ent->client->oldvelocity[2] < 0) &&
		(ent->velocity[2] > ent->client->oldvelocity[2]) &&
		(!ent->groundentity))
	{
		delta = ent->client->oldvelocity[2];
	}
	else
	{
		if (!ent->groundentity)
		{
			return;
		}

		delta = ent->velocity[2] - ent->client->oldvelocity[2];
	}

	delta = delta * delta * 0.0001;

	/* never take falling damage if completely underwater */
	if (ent->waterlevel == 3)
	{
		return;
	}

	if (ent->client->hook_state == HOOK_ON)
	{
        return;
	}

	if (ent->waterlevel == 2)
	{
		delta *= 0.25;
	}

	if (ent->waterlevel == 1)
	{
		delta *= 0.5;
	}

	if (delta < 1)
	{
		return;
	}

	if (delta < 15)
	{
		ent->s.event = EV_FOOTSTEP;
		return;
	}

	ent->client->fall_value = delta * 0.5;

	if (ent->client->fall_value > 40)
	{
		ent->client->fall_value = 40;
	}

	ent->client->fall_time = level.time + FALL_TIME;

	if (delta > 30)
	{
		if (ent->health > 0)
		{
			if (delta >= 55)
			{
				ent->s.event = EV_FALLFAR;
			}
			else
			{
				ent->s.event = EV_FALL;
			}
		}

		ent->pain_debounce_time = level.time; /* no normal pain sound */
		damage = (delta - 30) / 2;

		if (damage < 1)
		{
			damage = 1;
		}

		VectorSet(dir, 0, 0, 1);

		if (!deathmatch->value || !((int)dmflags->value & DF_NO_FALLING))
		{
			T_Damage(ent, world, world, dir, ent->s.origin, vec3_origin,
					damage, 0, 0, MOD_FALLING);
		}
	}
	else
	{
		ent->s.event = EV_FALLSHORT;
		return;
	}
}

void
P_WorldEffects(void)
{
	qboolean breather;
	qboolean envirosuit;
	int waterlevel, old_waterlevel;

	if (current_player->movetype == MOVETYPE_NOCLIP)
	{
		current_player->air_finished = level.time + 12; /* don't need air */
		return;
	}

	waterlevel = current_player->waterlevel;
	old_waterlevel = current_client->old_waterlevel;
	current_client->old_waterlevel = waterlevel;

	breather = current_client->breather_framenum > level.framenum;
	envirosuit = current_client->enviro_framenum > level.framenum;

	/* if just entered a water volume, play a sound */
	if (!old_waterlevel && waterlevel)
	{
		PlayerNoise(current_player, current_player->s.origin, PNOISE_SELF);

		if (current_player->watertype & CONTENTS_LAVA)
		{
			gi.sound(current_player, CHAN_BODY, gi.soundindex("player/lava_in.wav"), 1, ATTN_NORM, 0);
		}
		else if (current_player->watertype & CONTENTS_SLIME)
		{
			gi.sound(current_player, CHAN_BODY, gi.soundindex("player/watr_in.wav"), 1, ATTN_NORM, 0);
		}
		else if (current_player->watertype & CONTENTS_WATER)
		{
			gi.sound(current_player, CHAN_BODY, gi.soundindex("player/watr_in.wav"), 1, ATTN_NORM, 0);
		}

		current_player->flags |= FL_INWATER;

		/* clear damage_debounce, so the pain sound will play immediately */
		current_player->damage_debounce_time = level.time - 1;
	}

	/* if just completely exited a water volume, play a sound */
	if (old_waterlevel && !waterlevel)
	{
		PlayerNoise(current_player, current_player->s.origin, PNOISE_SELF);
		gi.sound(current_player, CHAN_BODY, gi.soundindex("player/watr_out.wav"), 1, ATTN_NORM, 0);
		current_player->flags &= ~FL_INWATER;
	}

	/* check for head just going under water */
	if ((old_waterlevel != 3) && (waterlevel == 3))
	{
		gi.sound(current_player, CHAN_BODY, gi.soundindex("player/watr_un.wav"), 1, ATTN_NORM, 0);
	}

	/* check for head just coming out of water */
	if ((old_waterlevel == 3) && (waterlevel != 3))
	{
		if (current_player->air_finished < level.time)
		{
			/* gasp for air */
			gi.sound(current_player, CHAN_VOICE, gi.soundindex("player/gasp1.wav"), 1, ATTN_NORM, 0);
			PlayerNoise(current_player, current_player->s.origin, PNOISE_SELF);
		}
		else if (current_player->air_finished < level.time + 11)
		{
			/* just break surface */
			gi.sound(current_player, CHAN_VOICE, gi.soundindex("player/gasp2.wav"), 1, ATTN_NORM, 0);
		}
	}

	/* check for drowning */
	if (waterlevel == 3)
	{
		/* breather or envirosuit give air */
		if (breather || envirosuit)
		{
			current_player->air_finished = level.time + 10;

			if (((int)(current_client->breather_framenum - level.framenum) % 25) == 0)
			{
				if (!current_client->breather_sound)
				{
					gi.sound(current_player, CHAN_AUTO, gi.soundindex("player/u_breath1.wav"), 1, ATTN_NORM, 0);
				}
				else
				{
					gi.sound(current_player, CHAN_AUTO, gi.soundindex("player/u_breath2.wav"), 1, ATTN_NORM, 0);
				}

				current_client->breather_sound ^= 1;
				PlayerNoise(current_player, current_player->s.origin, PNOISE_SELF);
			}
		}

		/* if out of air, start drowning */
		if (current_player->air_finished < level.time)
		{
		 	/* drown! */
			if ((current_player->client->next_drown_time < level.time) &&
				(current_player->health > 0))
			{
				current_player->client->next_drown_time = level.time + 1;

				/* take more damage the longer underwater */
				current_player->dmg += 2;

				if (current_player->dmg > 15)
				{
					current_player->dmg = 15;
				}

				/* play a gurp sound instead of a normal pain sound */
				if (current_player->health <= current_player->dmg)
				{
					gi.sound(current_player, CHAN_VOICE, gi.soundindex("player/drown1.wav"), 1, ATTN_NORM, 0);
				}
				else if (rand() & 1)
				{
					gi.sound(current_player, CHAN_VOICE, gi.soundindex("*gurp1.wav"), 1, ATTN_NORM, 0);
				}
				else
				{
					gi.sound(current_player, CHAN_VOICE, gi.soundindex("*gurp2.wav"), 1, ATTN_NORM, 0);
				}

				current_player->pain_debounce_time = level.time;

				T_Damage(current_player, world, world, vec3_origin, current_player->s.origin, vec3_origin,
						current_player->dmg, 0, DAMAGE_NO_ARMOR, MOD_WATER);
			}
		}
	}
	else
	{
		current_player->air_finished = level.time + 12;
		current_player->dmg = 2;
	}

	/* check for sizzle damage */
	if (waterlevel && (current_player->watertype & (CONTENTS_LAVA | CONTENTS_SLIME)))
	{
		if (current_player->watertype & CONTENTS_LAVA)
		{
			if ((current_player->health > 0) &&
				(current_player->pain_debounce_time <= level.time) &&
				(current_client->invincible_framenum < level.framenum))
			{
				if (rand() & 1)
				{
					gi.sound(current_player, CHAN_VOICE, gi.soundindex("player/burn1.wav"), 1, ATTN_NORM, 0);
				}
				else
				{
					gi.sound(current_player, CHAN_VOICE, gi.soundindex("player/burn2.wav"), 1, ATTN_NORM, 0);
				}

				current_player->pain_debounce_time = level.time + 1;
			}

			if (envirosuit) /* take 1/3 damage with envirosuit */
			{
				T_Damage(current_player, world, world, vec3_origin, current_player->s.origin,
						vec3_origin, 1 * waterlevel, 0, 0, MOD_LAVA);
			}
			else
			{
				T_Damage(current_player, world, world, vec3_origin, current_player->s.origin,
						vec3_origin, 3 * waterlevel, 0, 0, MOD_LAVA);
			}
		}

		if (current_player->watertype & CONTENTS_SLIME)
		{
			if (!envirosuit)
			{
			 	/* no damage from slime with envirosuit */
				T_Damage(current_player, world, world, vec3_origin, current_player->s.origin,
						vec3_origin, 1 * waterlevel, 0, 0, MOD_SLIME);
			}
		}
	}
}

void
G_SetClientEffects(edict_t *ent)
{
	int pa_type;
	int remaining;

	if (!ent)
	{
		return;
	}

	ent->s.effects = 0;

	/* player is always ir visible, even dead. */
	ent->s.renderfx = RF_IR_VISIBLE; /* FS: Coop: Rogue specific */

	if ((ent->health <= 0) || level.intermissiontime)
	{
		return;
	}

	if (ent->flags & FL_DISGUISED) /* FS: Coop: Rogue specific */
	{
		ent->s.renderfx |= RF_USE_DISGUISE;
	}

	if (gamerules && gamerules->value) /* FS: Coop: Rogue specific */
	{
		if (DMGame.PlayerEffects)
		{
			DMGame.PlayerEffects(ent);
		}
	}

	if (ent->powerarmor_time > level.time)
	{
		pa_type = PowerArmorType(ent);

		if (pa_type == POWER_ARMOR_SCREEN)
		{
			ent->s.effects |= EF_POWERSCREEN;
		}
		else if (pa_type == POWER_ARMOR_SHIELD)
		{
			ent->s.effects |= EF_COLOR_SHELL;
			ent->s.renderfx |= RF_SHELL_GREEN;
		}
	}

	if (ent->client->quad_framenum > level.framenum)
	{
		remaining = ent->client->quad_framenum - level.framenum;

		if ((remaining > 30) || (remaining & 4))
		{
			ent->s.effects |= EF_QUAD;
		}
	}

	if (ent->client->quadfire_framenum > level.framenum) /* FS: Coop: Xatrix specific */
	{
		remaining = ent->client->quadfire_framenum - level.framenum;

		if ((remaining > 30) || (remaining & 4))
		{
			ent->s.effects |= EF_QUAD;
		}
	}

	if (ent->client->double_framenum > level.framenum) /* FS: Coop: Rogue specific */
	{
		remaining = ent->client->double_framenum - level.framenum;

		if ((remaining > 30) || (remaining & 4))
		{
			ent->s.effects |= EF_DOUBLE;
		}
	}

	/* FS: Coop: Rogue specific */
	if ((ent->client->owned_sphere) &&
		(ent->client->owned_sphere->spawnflags == 1))
	{
		ent->s.effects |= EF_HALF_DAMAGE;
	}

	if (ent->client->tracker_pain_framenum > level.framenum) /* FS: Coop: Rogue specific */
	{
		ent->s.effects |= EF_TRACKERTRAIL;
	}

	if (ent->client->invincible_framenum > level.framenum)
	{
		remaining = ent->client->invincible_framenum - level.framenum;

		if ((remaining > 30) || (remaining & 4))
		{
			ent->s.effects |= EF_PENT;
		}
	}

	/* show cheaters!!! */
	if (ent->flags & FL_GODMODE)
	{
		ent->s.effects |= EF_COLOR_SHELL;
		ent->s.renderfx |= (RF_SHELL_RED | RF_SHELL_GREEN | RF_SHELL_BLUE);
	}

	if(ent->client->zCameraLocalEntity) /* FS: Zaero specific game dll changes */
	{
		VectorCopy (ent->s.origin, ent->client->zCameraLocalEntity->s.origin);
		VectorCopy (ent->s.angles, ent->client->zCameraLocalEntity->s.angles);
		VectorCopy (ent->s.old_origin, ent->client->zCameraLocalEntity->s.old_origin);

		ent->client->zCameraLocalEntity->s.effects = ent->s.effects;
	}
}

void
G_SetClientEvent(edict_t *ent)
{
	if (!ent)
	{
		return;
	}

	if (ent->s.event)
	{
		return;
	}

	if (ent->groundentity && (xyspeed > 225))
	{
		if ((int)(current_client->bobtime + bobmove) != bobcycle)
		{
			ent->s.event = EV_FOOTSTEP;
		}
	}
}

void
G_SetClientSound(edict_t *ent)
{
	char *weap;

	if (!ent)
	{
		return;
	}

	if (ent->client->pers.game_helpchanged != game.helpchanged)
	{
		ent->client->pers.game_helpchanged = game.helpchanged;
		ent->client->pers.helpchanged = 1;
	}

	/* help beep (no more than three times) */
	if (ent->client->pers.helpchanged && (ent->client->pers.helpchanged <= 3) && !(level.framenum & 63))
	{
		ent->client->pers.helpchanged++;
		gi.sound(ent, CHAN_VOICE, gi.soundindex("misc/pc_up.wav"), 1, ATTN_STATIC, 0);
	}

	if (ent->client->pers.weapon)
	{
		weap = ent->client->pers.weapon->classname;
	}
	else
	{
		weap = "";
	}

	if (ent->waterlevel && (ent->watertype & (CONTENTS_LAVA | CONTENTS_SLIME)))
	{
		ent->s.sound = snd_fry;
	}
	else if (strcmp(weap, "weapon_railgun") == 0)
	{
		ent->s.sound = gi.soundindex("weapons/rg_hum.wav");
	}
	else if (strcmp(weap, "weapon_bfg") == 0)
	{
		ent->s.sound = gi.soundindex("weapons/bfg_hum.wav");
	}
	else if (strcmp(weap, "weapon_phalanx") == 0) /* FS: Coop: Xatrix specific */
	{
		ent->s.sound = gi.soundindex("weapons/phaloop.wav");
	}
	else if (strcmp(weap, "weapon_soniccannon") == 0) /* FS: Zaero specific game dll changes */
	{
		ent->s.sound = gi.soundindex("weapons/sonic/sc_idle.wav");
	}
	else if (ent->client->weapon_sound)
	{
		ent->s.sound = ent->client->weapon_sound;
	}
	else
	{
		ent->s.sound = 0;
	}
}

void
G_SetClientFrame(edict_t *ent)
{
	gclient_t *client;
	qboolean duck, run;

	if (!ent)
	{
		return;
	}

	if (ent->s.modelindex != 255)
	{
		return; /* not in the player model */
	}

	client = ent->client;

	if (client->ps.pmove.pm_flags & PMF_DUCKED)
	{
		duck = true;
	}
	else
	{
		duck = false;
	}

	if (xyspeed)
	{
		run = true;
	}
	else
	{
		run = false;
	}

	/* check for stand/duck and stop/go transitions */
	if ((duck != client->anim_duck) && (client->anim_priority < ANIM_DEATH))
	{
		goto newanim;
	}

	if ((run != client->anim_run) && (client->anim_priority == ANIM_BASIC))
	{
		goto newanim;
	}

	if (!ent->groundentity && (client->anim_priority <= ANIM_WAVE))
	{
		goto newanim;
	}

	if (client->anim_priority == ANIM_REVERSE)
	{
		if (ent->s.frame > client->anim_end)
		{
			ent->s.frame--;
			return;
		}
	}
	else if (ent->s.frame < client->anim_end)
	{
	 	/* continue an animation */
		ent->s.frame++;
		return;
	}

	if (client->anim_priority == ANIM_DEATH)
	{
		return; /* stay there */
	}

	if (client->anim_priority == ANIM_JUMP)
	{
		if (!ent->groundentity)
		{
			return; /* stay there */
		}

		ent->client->anim_priority = ANIM_WAVE;
		ent->s.frame = FRAME_jump3;
		ent->client->anim_end = FRAME_jump6;
		return;
	}

newanim:

	/* return to either a running or standing frame */
	client->anim_priority = ANIM_BASIC;
	client->anim_duck = duck;
	client->anim_run = run;

	if (!ent->groundentity)
	{
		client->anim_priority = ANIM_JUMP;

		if (ent->s.frame != FRAME_jump2)
		{
			ent->s.frame = FRAME_jump1;
		}

		client->anim_end = FRAME_jump2;
	}
	else if (run)
	{
		/* running */
		if (duck)
		{
			ent->s.frame = FRAME_crwalk1;
			client->anim_end = FRAME_crwalk6;
		}
		else
		{
			ent->s.frame = FRAME_run1;
			client->anim_end = FRAME_run6;
		}
	}
	else
	{
		/* standing */
		if (duck)
		{
			ent->s.frame = FRAME_crstnd01;
			client->anim_end = FRAME_crstnd19;
		}
		else
		{
			ent->s.frame = FRAME_stand01;
			client->anim_end = FRAME_stand40;
		}
	}
}

/*
 * Called for each player at the end of
 * the server frame and right after spawning
 */
void
ClientEndServerFrame(edict_t *ent)
{
	float bobtime;
	int i;

	if (!ent)
	{
		return;
	}

	current_player = ent;
	current_client = ent->client;

	/* If the origin or velocity have changed since ClientThink(),
	   update the pmove values.  This will happen when the client
	   is pushed by a bmodel or kicked by an explosion.
	   If it wasn't updated here, the view position would lag a frame
	   behind the body position when pushed -- "sinking into plats" */
	for (i = 0; i < 3; i++)
	{
		current_client->ps.pmove.origin[i] = ent->s.origin[i] * 8.0;
		current_client->ps.pmove.velocity[i] = ent->velocity[i] * 8.0;
	}

	/* If the end of unit layout is displayed, don't give
	   the player any normal movement attributes */
	if (level.intermissiontime)
	{
		current_client->ps.blend[3] = 0;
		current_client->ps.fov = 90;
		G_SetStats(ent);

		if ((game.gametype == zaero_coop) && (level.fadeFrames > 0)) /* FS: Zaero specific game dll changes */
		{
			float ratio = (float)(50 - level.fadeFrames) / 50.0f;
			SV_AddBlend (1, 1, 1, ratio, current_client->ps.blend);
		}

		return;
	}

	AngleVectors(ent->client->v_angle, forward, right, up);

	/* burn from lava, etc */
	P_WorldEffects();

	/* set model angles from view angles so other things in
	   the world can tell which direction you are looking */
	if (ent->client->v_angle[PITCH] > 180)
	{
		ent->s.angles[PITCH] = (-360 + ent->client->v_angle[PITCH]) / 3;
	}
	else
	{
		ent->s.angles[PITCH] = ent->client->v_angle[PITCH] / 3;
	}

	ent->s.angles[YAW] = ent->client->v_angle[YAW];
	ent->s.angles[ROLL] = 0;
	ent->s.angles[ROLL] = SV_CalcRoll(ent->s.angles, ent->velocity) * 4;

	/* calculate speed and cycle to be
	   used for all cyclic walking effects */
	xyspeed = sqrtf(
			ent->velocity[0] * ent->velocity[0] + ent->velocity[1] *
			ent->velocity[1]);

	if (xyspeed < 5)
	{
		bobmove = 0;
		current_client->bobtime = 0; /* start at beginning of cycle again */
	}
	else if (ent->groundentity)
	{
	  	/* so bobbing only cycles when on ground */
		if (xyspeed > 210)
		{
			bobmove = 0.25;
		}
		else if (xyspeed > 100)
		{
			bobmove = 0.125;
		}
		else
		{
			bobmove = 0.0625;
		}
	}

	bobtime = (current_client->bobtime += bobmove);

	if (current_client->ps.pmove.pm_flags & PMF_DUCKED)
	{
		bobtime *= 4;
	}

	bobcycle = (int)bobtime;
	bobfracsin = fabs(sin(bobtime * M_PI));

	/* detect hitting the floor */
	P_FallingDamage(ent);

	/* apply all the damage taken this frame */
	P_DamageFeedback(ent);

	/* determine the view offsets */
	SV_CalcViewOffset(ent);

	/* determine the gun offsets */
	SV_CalcGunOffset(ent);

	/* determine the full screen color blend must be after viewoffset,
	   so eye contents can be accurately determined */
	SV_CalcBlend(ent);

	/* chase cam stuff */
	if (ent->client->resp.spectator)
	{
		G_SetSpectatorStats(ent);
	}
	else
	{
		G_SetStats(ent);
	}

	G_CheckChaseStats(ent);
	G_SetClientEvent(ent);
	G_SetClientEffects(ent);
	G_SetClientSound(ent);
	G_SetClientFrame(ent);

	VectorCopy(ent->velocity, ent->client->oldvelocity);
	VectorCopy(ent->client->ps.viewangles, ent->client->oldviewangles);

	/* clear weapon kicks */
	VectorClear(ent->client->kick_origin);
	VectorClear(ent->client->kick_angles);

	/* if the scoreboard is up, update it */
	if (ent->client->showscores && !(level.framenum & 31))
	{
		if (ent->client->menu)
		{
			PMenu_Do_Update(ent);
			ent->client->menudirty = false;
			ent->client->menutime = level.time;
		}
		/* if the help computer is up, update it */
		else if (ent->client->showhelp)
		{
			ent->client->pers.helpchanged = 0;
			HelpComputerMessage(ent);
		}
		else if (ent->client->zCameraTrack) /* FS: Zaero specific game dll changes */
		{
			updateVisorHud(ent);
		}
		else
		{
			DeathmatchScoreboardMessage(ent, ent->enemy);
		}
		gi.unicast(ent, false);
	}

	/* this we want to do regardless */
	if (ent->client->zCameraTrack)  /* FS: Zaero specific game dll changes */
	{
		// decrease the visor frame time
		ent->client->pers.visorFrames--;
		if (ent->client->pers.visorFrames == 0)
		{
			stopCamera(ent);
			ent->client->pers.inventory[ITEM_INDEX(FindItem("Visor"))]--;
			ValidateSelectedItem (ent);
		}
	}

	/* if the inventory is up, update it */
	if (ent->client->showinventory)
	{
		InventoryMessage(ent);
		gi.unicast(ent, false);
	}
}
