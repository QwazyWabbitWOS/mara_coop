// g_local.h -- local definitions for game module

#ifndef COOP_LOCAL_H
#define COOP_LOCAL_H
#include "q_shared.h"
#include "z_anim.h" /* FS: Zaero specific */

// define GAME_INCLUDE so that game.h does not define the
// short, server-visible gclient_t and edict_t structures,
// because we define the full size ones in this file
#define	GAME_INCLUDE
#include "game.h"
#include "menu.h"
#include "b_cam.h" /* FS: Blinky's Coop camera */

/* FS: Coop: Zaero specific defines */
#ifdef Z_MAX
#undef Z_MAX
#endif // Z_MAX
#define Z_MAX(a,b)	((a) > (b) ? (a) : (b))

#ifdef Z_MIN
#undef Z_MIN
#endif // Z_MIN
#define Z_MIN(a,b)	((a) < (b) ? (a) : (b))

//QW// Delete pointless macros
//#define Z_MALLOC(size)	gi.TagMalloc(size, TAG_GAME)
//#define Z_FREE(block)	gi.TagFree(block)

// the "gameversion" client command will print this plus compile date
#define GAMEVERSION "maracoop v2.1"

// protocol bytes that can be directly added to messages
#define	svc_muzzleflash		1
#define	svc_muzzleflash2	2
#define	svc_temp_entity		3
#define	svc_layout			4
#define	svc_inventory		5
#define	svc_stufftext		11

//==================================================================

// view pitching times
#define DAMAGE_TIME		0.5f
#define	FALL_TIME		0.3f

// ROGUE- id killed this weapon
//#define	KILL_DISRUPTOR	1	// Knightmare- re-enabled this due to public interest
// rogue

// edict->spawnflags
// these are set with checkboxes on each entity in the map editor
#define	SPAWNFLAG_NOT_EASY			0x00000100
#define	SPAWNFLAG_NOT_MEDIUM		0x00000200
#define	SPAWNFLAG_NOT_HARD			0x00000400
#define	SPAWNFLAG_NOT_DEATHMATCH	0x00000800
#define	SPAWNFLAG_NOT_COOP			0x00001000

// edict->spawnflags2
// these are set with checkboxes on each entity in the map editor
#define	SPAWNFLAG2_MIRRORLEVEL	0x0001 /* FS: Zaero specific game dll changes */
#define	SPAWNFLAG2_NOT_COOP		0x0002
#define SPAWNFLAG2_NOT_SINGLE	0x0004

// edict->flags
#define	FL_FLY					0x00000001
#define	FL_SWIM					0x00000002	// implied immunity to drowining
#define FL_IMMUNE_LASER			0x00000004
#define	FL_INWATER				0x00000008
#define	FL_GODMODE				0x00000010
#define	FL_NOTARGET				0x00000020
#define FL_IMMUNE_SLIME			0x00000040
#define FL_IMMUNE_LAVA			0x00000080
#define	FL_PARTIALGROUND		0x00000100	// not all corners are valid
#define	FL_WATERJUMP			0x00000200	// player jumping out of water
#define	FL_TEAMSLAVE			0x00000400	// not the first on the team
#define FL_NO_KNOCKBACK			0x00000800
#define FL_POWER_ARMOR			0x00001000	// power armor (if any) is active
#define FL_DONTSETOLDORIGIN		0x00002000  // Don't set the old_origin for this entity  /* FS: Zaero specific game dll changes */
#define FL_BFGMISSFIRE			0x00004000  // BFG Miss Fire /* FS: Zaero specific game dll changes */
#define FL_RESPAWN				0x80000000	// used for item respawning

//ROGUE
#define FL_MECHANICAL			0x00002000	// entity is mechanical, use sparks not blood
#define FL_SAM_RAIMI			0x00004000	// entity is in sam raimi cam mode
#define FL_DISGUISED			0x00008000	// entity is in disguise, monsters will not recognize.
#define	FL_NOGIB				0x00010000	// player has been vaporized by a nuke, drop no gibs
//ROGUE

#define	FRAMETIME		0.1f

// memory tags to allow dynamic memory to be cleaned up
#define	TAG_GAME	765		// clear when unloading the dll
#define	TAG_LEVEL	766		// clear when loading a new level


#define MELEE_DISTANCE	80

#define BODY_QUEUE_SIZE		8

typedef enum damage_e
{
	DAMAGE_NO,
	DAMAGE_YES,			// will take damage if hit
	DAMAGE_AIM,			// auto targeting recognizes this
	DAMAGE_IMMORTAL		// similar to DAMAGE_YES, but health is not deducted /* FS: Zaero specific game dll changes */
} damage_t;

typedef enum weaponstate_e
{
	WEAPON_READY,
	WEAPON_ACTIVATING,
	WEAPON_DROPPING,
	WEAPON_FIRING
} weaponstate_t;

typedef enum ammo_e
{
	AMMO_BULLETS,
	AMMO_SHELLS,
	AMMO_ROCKETS,
	AMMO_GRENADES,
	AMMO_CELLS,
	AMMO_SLUGS,

	/* FS: Coop: Xatrix specific */
	AMMO_MAGSLUG,
	AMMO_TRAP,

	/* FS: Coop: Rogue specific */
	AMMO_FLECHETTES,
	AMMO_TESLA,
	AMMO_PROX,

	/* FS: Coop: Zaero specific */
	AMMO_FLARES, /* FS: Zaero specific game dll changes */
	AMMO_LASERTRIPBOMB, /* FS: Zaero specific game dll changes */
	AMMO_EMPNUKE, /* FS: Zaero specific game dll changes */
	AMMO_A2K, /* FS: Zaero specific game dll changes */
	AMMO_PLASMASHIELD /* FS: Zaero specific game dll changes */
} ammo_t;

typedef enum vote_e	/* FS: Coop: Voting */
{
	NOT_VOTED,
	VOTE_YES,
	VOTE_NO
} vote_t;

//deadflag
#define DEAD_NO					0
#define DEAD_DYING				1
#define DEAD_DEAD				2
#define DEAD_RESPAWNABLE		3

//range
#define RANGE_MELEE				0
#define RANGE_NEAR				1
#define RANGE_MID				2
#define RANGE_FAR				3

//gib types
#define GIB_ORGANIC				0
#define GIB_METALLIC			1

//monster ai flags
#define AI_STAND_GROUND			0x00000001
#define AI_TEMP_STAND_GROUND	0x00000002
#define AI_SOUND_TARGET			0x00000004
#define AI_LOST_SIGHT			0x00000008
#define AI_PURSUIT_LAST_SEEN	0x00000010
#define AI_PURSUE_NEXT			0x00000020
#define AI_PURSUE_TEMP			0x00000040
#define AI_HOLD_FRAME			0x00000080
#define AI_GOOD_GUY				0x00000100
#define AI_BRUTAL				0x00000200
#define AI_NOSTEP				0x00000400
#define AI_DUCKED				0x00000800
#define AI_COMBAT_POINT			0x00001000
#define AI_MEDIC				0x00002000
#define AI_RESURRECTING			0x00004000

//ROGUE
#define AI_WALK_WALLS			0x00008000
#define AI_MANUAL_STEERING		0x00010000
#define AI_TARGET_ANGER			0x00020000
#define AI_DODGING				0x00040000
#define AI_CHARGING				0x00080000
#define AI_HINT_PATH			0x00100000
#define	AI_IGNORE_SHOTS			0x00200000
// PMM - FIXME - last second added for E3 .. there's probably a better way to do this, but
// this works
#define	AI_DO_NOT_COUNT			0x00400000	// set for healed monsters
#define	AI_SPAWNED_CARRIER		0x00800000	// both do_not_count and spawned are set for spawned monsters
#define	AI_SPAWNED_MEDIC_C		0x01000000	// both do_not_count and spawned are set for spawned monsters
#define	AI_SPAWNED_WIDOW		0x02000000	// both do_not_count and spawned are set for spawned monsters
#define AI_SPAWNED_MASK			0x03800000	// mask to catch all three flavors of spawned
#define	AI_BLOCKED				0x04000000	// used by blocked_checkattack: set to say I'm attacking while blocked 
											// (prevents run-attacks)
//ROGUE

#define AI_SCHOOLING			0x00008000
#define AI_REDUCEDDAMAGE		0x00010000
#define AI_MONREDUCEDDAMAGE		0x00200000
#define AI_ONESHOTTARGET		0x00400000

//monster attack state
#define AS_STRAIGHT				1
#define AS_SLIDING				2
#define	AS_MELEE				3
#define	AS_MISSILE				4
#define	AS_BLIND				5	// PMM - used by boss code to do nasty things even if it can't see you

#define AS_FLY_STRAFE			5

// armor types
#define ARMOR_NONE				0
#define ARMOR_JACKET			1
#define ARMOR_COMBAT			2
#define ARMOR_BODY				3
#define ARMOR_SHARD				4

// power armor types
#define POWER_ARMOR_NONE		0
#define POWER_ARMOR_SCREEN		1
#define POWER_ARMOR_SHIELD		2

// handedness values
#define RIGHT_HANDED			0
#define LEFT_HANDED				1
#define CENTER_HANDED			2


// game.serverflags values
#define SFL_CROSS_TRIGGER_1		0x00000001
#define SFL_CROSS_TRIGGER_2		0x00000002
#define SFL_CROSS_TRIGGER_3		0x00000004
#define SFL_CROSS_TRIGGER_4		0x00000008
#define SFL_CROSS_TRIGGER_5		0x00000010
#define SFL_CROSS_TRIGGER_6		0x00000020
#define SFL_CROSS_TRIGGER_7		0x00000040
#define SFL_CROSS_TRIGGER_8		0x00000080
#define SFL_CROSS_TRIGGER_MASK	0x000000ff


// noise types for PlayerNoise
#define PNOISE_SELF				0
#define PNOISE_WEAPON			1
#define PNOISE_IMPACT			2


// edict->movetype values
typedef enum movetype_e
{
	MOVETYPE_NONE,			// never moves
	MOVETYPE_NOCLIP,		// origin and angles change with no interaction
	MOVETYPE_PUSH,			// no clip to world, push on box contact
	MOVETYPE_STOP,			// no clip to world, stops on box contact

	MOVETYPE_WALK,			// gravity
	MOVETYPE_STEP,			// gravity, special edge handling
	MOVETYPE_FLY,
	MOVETYPE_TOSS,			// gravity
	MOVETYPE_FLYMISSILE,	// extra size to monsters
	MOVETYPE_BOUNCE,

	MOVETYPE_WALLBOUNCE,	/* FS: Coop: Xatrix specific */

	MOVETYPE_NEWTOSS,		/* FS: Coop: Rogue specific */

	/* FS: Zaero specific game dll changes */
	MOVETYPE_BOUNCEFLY,
	MOVETYPE_FREEZE,       // player freeze, used for Zaero Camera
	MOVETYPE_FALLFLOAT,		// falls down slopes and floats in water
	MOVETYPE_RIDE			// basically won't move unless it rides on a MOVETYPE_PUSH entity
} movetype_t;



typedef struct gitem_armor_s
{
	int		base_count;
	int		max_count;
	float	normal_protection;
	float	energy_protection;
	int		armor;
} gitem_armor_t;


// gitem_t->flags
#define	IT_WEAPON			0x00000001		// use makes active weapon
#define	IT_AMMO				0x00000002
#define IT_ARMOR			0x00000004
#define IT_STAY_COOP		0x00000008
#define IT_KEY				0x00000010
#define IT_POWERUP			0x00000020
#define IT_XATRIX			0x00000040 /* FS: Coop: Xatrix specific */
#define IT_ROGUE			0x00000080 /* FS: Coop: Rogue specific */
#define IT_ZAERO			0x00000100 /* FS: Coop: Zaero specific */

// ROGUE
#define IT_MELEE			0x00000040
#define IT_NOT_GIVEABLE		0x00000080	// item can not be given
// ROGUE

// gitem_t->weapmodel for weapons indicates model index
#define WEAP_BLASTER			1 
#define WEAP_SHOTGUN			2 
#define WEAP_SUPERSHOTGUN		3 
#define WEAP_MACHINEGUN			4 
#define WEAP_CHAINGUN			5 
#define WEAP_GRENADES			6 
#define WEAP_GRENADELAUNCHER	7 
#define WEAP_ROCKETLAUNCHER		8 
#define WEAP_HYPERBLASTER		9 
#define WEAP_RAILGUN			10
#define WEAP_BFG				11
#define WEAP_PLASMARIFLE		20
//#define WEAP_CLUSTERLAUNCHER	21

#define WEAP_DISRUPTOR			12 /* FS: Coop: Rogue specific */
#define WEAP_ETFRIFLE			13 /* FS: Coop: Rogue specific */
#define WEAP_PLASMA				14 /* FS: Coop: Rogue specific */
#define WEAP_PROXLAUNCH			15 /* FS: Coop: Rogue specific */
#define WEAP_CHAINFIST			16 /* FS: Coop: Rogue specific */

#define WEAP_PHALANX			17 /* FS: Coop: Xatrix specific */
#define WEAP_BOOMER				18 /* FS: Coop: Xatrix specific */

#define WEAP_NONE				19 /* FS: Coop: Zaero specifc */

void Weapon_Generic(edict_t* ent, int FRAME_ACTIVATE_LAST, int FRAME_FIRE_LAST, int FRAME_IDLE_LAST, int FRAME_DEACTIVATE_LAST, int* pause_frames, int* fire_frames, void (*fire)(edict_t* ent));


/* FS: Zaero specific game dll changes */
// hide flags
#define HIDE_FROM_INVENTORY	1	// don't list this item in the inventory
#define HIDE_DONT_KEEP		2	// don't keep in the lastweapon variable
#define HIDE_FROM_SELECTION	4	// don't autoselect or weapnext thru

typedef struct gitem_s
{
	char* classname;	// spawning name
	qboolean(*pickup)(struct edict_s* ent, struct edict_s* other);
	void		(*use)(struct edict_s* ent, struct gitem_s* item);
	void		(*drop)(struct edict_s* ent, struct gitem_s* item);
	void		(*weaponthink)(struct edict_s* ent);
	char* pickup_sound;
	char* world_model;
	int			world_model_flags;
	char* view_model;

	// client side info
	char* icon;
	char* pickup_name;	// for printing on pickup
	int			count_width;		// number of digits to display by icon

	int			quantity;		// for ammo how much, for weapons how much is used per shot
	char* ammo;			// for weapons
	int			flags;			// IT_* flags

	int			weapmodel;		// weapon model index (for weapons)

	void* info;
	int			tag;

	char* precaches;		// string of all models, sounds, and images this item will use
	int			hideFlags; /* FS: Zaero specific game dll changes */
	char* weapon;        // weapon used by ammo  /* FS: Zaero specific game dll changes */
} gitem_t;

/* FS: Coop: Hint for which rule sets we apply */
typedef enum gametype_e
{
	vanilla_coop,
	xatrix_coop,
	rogue_coop,
	zaero_coop
} gametype_t;

#define MAX_GAMEMODES		256
#define GAMEMODE_ERROR		-2
#define GAMEMODE_AVAILABLE	-1

/* FS: Coop: Dynamic gamemode voting menu */
typedef struct gamemode_s
{
	char mapname[64];
	char gamemode[64];
	char realgamemode[64];
}gamemode_t;

//
// this structure is left intact through an entire game
// it should be initialized at dll load time, and read/written to
// the server.ssv file for savegames
//
typedef struct game_locals_s
{
	char		helpmessage1[512];
	char		helpmessage2[512];
	int			helpchanged;	// flash F1 icon if non 0, play sound
								// and increment only if 1, 2, or 3

	gclient_t* clients;		// [maxclients]

	// can't store spawnpoint in level, because
	// it would get overwritten by the savegame restore
	char		spawnpoint[512];	// needed for coop respawns

	// store latched cvars here that we want to get at often
	int			maxclients;
	int			maxentities;

	// cross level triggers
	int			serverflags;

	// items
	int			num_items;

	qboolean	autosaved;
	gametype_t	gametype; /* FS: Coop: Hint for which rule sets we apply */
	int			inventory[MAX_ITEMS]; /* FS: Coop: Globalized inventory of weapons and keys we should be having. */
} game_locals_t;


//
// this structure is cleared as each map is entered
// it is read/written to the level.sav file for savegames
//
typedef struct level_locals_s
{
	int			framenum;
	float		time;

	char		level_name[MAX_QPATH];	// the descriptive name (Outer Base, etc)
	char		mapname[MAX_QPATH];		// the server name (base1, etc)
	char		nextmap[MAX_QPATH];		// go here when fraglimit is hit

	// intermission state
	float		intermissiontime;		// time the intermission was started
	float		intermissionupdate;		// Phatman: seconds at last update
	char* changemap;
	int			exitintermission;
	int			fadeFrames; /* FS: Zaero specific game dll changes */
	vec3_t		intermission_origin;
	vec3_t		intermission_angle;

	edict_t* sight_client;	// changed once each frame for coop games

	edict_t* sight_entity;
	int			sight_entity_framenum;
	edict_t* sound_entity;
	int			sound_entity_framenum;
	edict_t* sound2_entity;
	int			sound2_entity_framenum;

	int			pic_health;

	int			total_secrets;
	int			found_secrets;

	int			total_goals;
	int			found_goals;

	int			total_monsters;
	int			killed_monsters;

	edict_t* current_entity;	// entity running from G_RunFrame
	int			body_que;			// dead bodies

	int			power_cubes;		// ugly necessity for coop

	// ROGUE
	edict_t* disguise_violator;
	int			disguise_violation_framenum;
	// ROGUE
	edict_t* current_coop_checkpoint; /* FS: Coop: Added Info_coop_checkpoint */
} level_locals_t;


// spawn_temp_t is only used to hold entity field values that
// can be set from the editor, but aren't actualy present
// in edict_t during gameplay
typedef struct spawn_temp_s
{
	// world vars
	char* sky;
	float		skyrotate;
	vec3_t		skyaxis;
	char* nextmap;

	int			lip;
	int			distance;
	int			height;
	char* noise;
	float		pausetime;
	char* item;
	char* gravity;

	float		minyaw;
	float		maxyaw;
	float		minpitch;
	float		maxpitch;
} spawn_temp_t;


typedef struct moveinfo_s
{
	// fixed data
	vec3_t		start_origin;
	vec3_t		start_angles;
	vec3_t		end_origin;
	vec3_t		end_angles;

	int			sound_start;
	int			sound_middle;
	int			sound_end;

	float		accel;
	float		speed;
	float		decel;
	float		distance;

	float		wait;

	// state data
	int			state;
	vec3_t		dir;
	float		current_speed;
	float		move_speed;
	float		next_speed;
	float		remaining_distance;
	float		decel_distance;
	void		(*endfunc)(edict_t*);
} moveinfo_t;


typedef struct mframe_s
{
	void	(*aifunc)(edict_t* self, float dist);
	float	dist;
	void	(*thinkfunc)(edict_t* self);
} mframe_t;

typedef struct mmove_s
{
	int			firstframe;
	int			lastframe;
	mframe_t* frame;
	void		(*endfunc)(edict_t* self);
} mmove_t;

typedef struct monsterinfo_s
{
	mmove_t* currentmove;
	unsigned int	aiflags;		// PGM - unsigned, since we're close to the max
	int			nextframe;
	float		scale;

	void		(*stand)(edict_t* self);
	void		(*idle)(edict_t* self);
	void		(*search)(edict_t* self);
	void		(*walk)(edict_t* self);
	void		(*run)(edict_t* self);
	void		(*dodge)(edict_t* self, edict_t* other, float eta, trace_t* tr);
	void		(*attack)(edict_t* self);
	void		(*melee)(edict_t* self);
	void		(*sight)(edict_t* self, edict_t* other);
	qboolean(*checkattack)(edict_t* self);
	void		(*backwalk)(edict_t* self); /* FS: Zaero specific game dll changes */
	void		(*sidestepright)(edict_t* self); /* FS: Zaero specific game dll changes */
	void		(*sidestepleft)(edict_t* self); /* FS: Zaero specific game dll changes */

	float		pausetime;
	float		attack_finished;

	vec3_t		saved_goal;
	float		search_time;
	float		trail_time;
	vec3_t		last_sighting;
	int			attack_state;
	int			lefty;
	float		idle_time;
	int			linkcount;

	int			power_armor_type;
	int			power_armor_power;

	//ROGUE
	qboolean(*blocked)(edict_t* self, float dist);
	//	edict_t		*last_hint;			// last hint_path the monster touched
	float		last_hint_time;		// last time the monster checked for hintpaths.
	edict_t* goal_hint;			// which hint_path we're trying to get to
	int			medicTries;
	edict_t* badMedic1, * badMedic2;	// these medics have declared this monster "unhealable"
	edict_t* healer;	// this is who is healing this monster
	void		(*duck)(edict_t* self, float eta);
	void		(*unduck)(edict_t* self);
	void		(*sidestep)(edict_t* self);
	//  while abort_duck would be nice, only monsters which duck but don't sidestep would use it .. only the brain
	//  not really worth it.  sidestep is an implied abort_duck
//	void		(*abort_duck)(edict_t *self);
	float		base_height;
	float		next_duck_time;
	float		duck_wait_time;
	edict_t* last_player_enemy;
	// blindfire stuff .. the boolean says whether the monster will do it, and blind_fire_time is the timing
	// (set in the monster) of the next shot
	qboolean	blindfire;		// will the monster blindfire?
	float		blind_fire_delay;
	vec3_t		blind_fire_target;
	// used by the spawners to not spawn too much and keep track of #s of monsters spawned
	int			monster_slots;
	int			monster_used;
	edict_t* commander;
	// powerup timers, used by widow, our friend
	float		quad_framenum;
	float		invincible_framenum;
	float		double_framenum;
	//ROGUE

	int flashTime; /* FS: Zaero specific game dll changes */
	int flashBase; /* FS: Zaero specific game dll changes */

	// strafing
	float flyStrafePitch; /* FS: Zaero specific game dll changes */
	float flyStrafeTimeout; /* FS: Zaero specific game dll changes */

	//schooling info
	float zSchoolSightRadius; /* FS: Zaero specific game dll changes */
	float zSchoolMaxSpeed, zSchoolMinSpeed; /* FS: Zaero specific game dll changes */
	float zSpeedStandMax, zSpeedWalkMax; /* FS: Zaero specific game dll changes */
	float zSchoolDecayRate, zSchoolMinimumDistance; /* FS: Zaero specific game dll changes */
	int   zSchoolFlags; /* FS: Zaero specific game dll changes */

	float reducedDamageAmount; /* FS: Zaero specific game dll changes */

	float dodgetimeout; /* FS: Zaero specific game dll changes */

	vec3_t shottarget; /* FS: Zaero specific game dll changes */
} monsterinfo_t;

// ROGUE
// this determines how long to wait after a duck to duck again.  this needs to be longer than
// the time after the monster_duck_up in all of the animation sequences
#define	DUCK_INTERVAL	0.5
// ROGUE

extern	game_locals_t	game;
extern	level_locals_t	level;
extern	game_import_t	gi;
extern	game_export_t	globals;
extern	spawn_temp_t	st;
extern	gamemode_t gamemode_array[MAX_GAMEMODES]; /* FS */

extern	int	sm_meat_index;
extern	int	snd_fry;

extern	int	jacket_armor_index;
extern	int	combat_armor_index;
extern	int	body_armor_index;

/* FS: Zaero specific changes */
extern	int	shells_index;
extern	int	bullets_index;
extern	int	grenades_index;
extern	int	rockets_index;
extern	int	cells_index;
extern	int	slugs_index;
extern	int	flares_index;
extern	int	tbombs_index;
extern	int	empnuke_index;
extern	int	plasmashield_index;

extern	int gibsthisframe;
extern	int lastgibframe;

// means of death
#define MOD_UNKNOWN			0
#define MOD_BLASTER			1
#define MOD_SHOTGUN			2
#define MOD_SSHOTGUN		3
#define MOD_MACHINEGUN		4
#define MOD_CHAINGUN		5
#define MOD_GRENADE			6
#define MOD_G_SPLASH		7
#define MOD_ROCKET			8
#define MOD_R_SPLASH		9
#define MOD_HYPERBLASTER	10
#define MOD_RAILGUN			11
#define MOD_BFG_LASER		12
#define MOD_BFG_BLAST		13
#define MOD_BFG_EFFECT		14
#define MOD_HANDGRENADE		15
#define MOD_HG_SPLASH		16
#define MOD_WATER			17
#define MOD_SLIME			18
#define MOD_LAVA			19
#define MOD_CRUSH			20
#define MOD_TELEFRAG		21
#define MOD_FALLING			22
#define MOD_SUICIDE			23
#define MOD_HELD_GRENADE	24
#define MOD_EXPLOSIVE		25
#define MOD_BARREL			26
#define MOD_BOMB			27
#define MOD_EXIT			28
#define MOD_SPLASH			29
#define MOD_TARGET_LASER	30
#define MOD_TRIGGER_HURT	31
#define MOD_HIT				32
#define MOD_TARGET_BLASTER	33
// Xatrix: RAFAEL 14-APR-98
#define MOD_RIPPER				34
#define MOD_PHALANX				35
#define MOD_BRAINTENTACLE		36
#define MOD_BLASTOFF			37
#define MOD_GEKK				38
#define MOD_TRAP				39
// Xatrix: END 14-APR-98
#define MOD_FRIENDLY_FIRE	0x8000000

//========
//ROGUE
#define MOD_CHAINFIST			40
#define MOD_DISINTEGRATOR		41
#define MOD_ETF_RIFLE			42
#define MOD_BLASTER2			43
#define MOD_HEATBEAM			44
#define MOD_TESLA				45
#define MOD_PROX				46
#define MOD_NUKE				47
#define MOD_VENGEANCE_SPHERE	48
#define MOD_HUNTER_SPHERE		49
#define MOD_DEFENDER_SPHERE		50
#define MOD_TRACKER				51
#define MOD_DBALL_CRUSH			52
#define MOD_DOPPLE_EXPLODE		53
#define MOD_DOPPLE_VENGEANCE	54
#define MOD_DOPPLE_HUNTER		55
//ROGUE
//========

#define MOD_SNIPERRIFLE		  56 /* FS: Zaero specific changes */
#define MOD_TRIPBOMB		    57 /* FS: Zaero specific changes */
#define MOD_FLARE			      58 /* FS: Zaero specific changes */
#define MOD_A2K				      59 /* FS: Zaero specific changes */
#define MOD_SONICCANNON		  60 /* FS: Zaero specific changes */
#define MOD_AUTOCANNON		  61 /* FS: Zaero specific changes */
#define MOD_GL_POLYBLEND	62 /* FS: Zaero specific changes */
#define MOD_PLASMA_RIFLE	63 /* Stross & Asa */
#define MOD_GRAPPLE         64 /* Stross & Asa */

extern	int	meansOfDeath;


#define q_offsetof(t, m)    ((size_t)&((t *)0)->m)

extern	edict_t* g_edicts;

#define FOFS(x)     q_offsetof(edict_t, x)
#define STOFS(x)    q_offsetof(spawn_temp_t, x)
#define	LLOFS(x) (size_t)&(((level_locals_t *)0)->x)
#define	CLOFS(x) (size_t)&(((gclient_t *)0)->x)

#define random()	((rand () & 0x7fff) / ((float)0x7fff))
#define crandom()	(2.0 * (random() - 0.5))

extern	cvar_t* maxentities;
extern	cvar_t* deathmatch;
extern	cvar_t* coop;
extern	cvar_t* coop_item_respawn; /* FS: Coop: Added */
extern	cvar_t* coop_checkpoints; /* FS: Coop: Added */
extern	cvar_t* sv_vote_enabled; /* FS: Coop: Voting */
extern	cvar_t* sv_vote_disallow_flags; /* FS: Coop: Voting */
extern	cvar_t* sv_vote_assume_yes; /* FS: Coop: Voting */
extern	cvar_t* sv_vote_timer; /* FS: Coop: Voting */
extern	cvar_t* sv_vote_private; /* FS: Coop: Voting */
extern	cvar_t* sv_vote_chat_commands; /* FS: Coop: Voting */
extern	cvar_t* sv_coop_gamemode; /* FS: Coop: Added */
extern	cvar_t* sv_coop_gamemode_vote; /* FS: Coop: Added */
extern	cvar_t* sv_coop_reset_hack; /* FS: Coop: Gross reset frametime hack shit */
extern	cvar_t* sv_coop_maplist; /* FS: External file we need for the map voting */
extern	cvar_t* sv_coop_check_player_exit; /* FS: Check player distance to exit a level */
extern	cvar_t* sv_coop_summon_time; /* FS: Added */
extern	cvar_t* sv_coop_announce_name_change; /* FS: Added */
extern	cvar_t* sv_coop_name_timeout; /* FS: Added */
extern	cvar_t* sv_coop_blinky_cam_disallowflags; /* FS: Added */
extern	cvar_t* sv_drop_timeout; /* FS: Added */
extern	cvar_t* sv_spawn_protection; /* FS: Coop: Spawn protection */
extern	cvar_t* sv_spawn_protection_time; /* FS: Coop: Spawn protection */
extern	cvar_t* motd; /* FS: Coop: Added */
extern	cvar_t* adminpass; /* FS: Coop: Admin goodies */
extern  cvar_t  *checkpoints_password; /* Phatman: Coop: Password to edit checkpoints */
extern  cvar_t  *home_gamemode; /* Phatman: Coop: Go to this game mode after any game mode has finished */
extern  cvar_t  *cycle_gamemode; /* Phatman: Coop: Game mode to go to after the final one */
extern	cvar_t	*victory_pcx; /* Phatman: Overrides victory.pcx */
extern	cvar_t	*readiness_timer; /* Phatman: Intermission readiness timeout in seconds */
extern  cvar_t  *playlist_current; /* Phatman: Current playlist index */
extern  cvar_t  *playlist_random; /* Phatman: Randomize playlist */
extern	cvar_t* vippass; /* FS: Coop: VIP goodies */
extern	cvar_t* gamedir; /* FS: Coop: Added */
extern	cvar_t* nextserver; /* FS: Coop: Added */
extern	cvar_t* coop_cameraoffset; /* FS: Blinky's Coop Camera */
extern	cvar_t* dmflags;
extern	cvar_t* zdmflags; /* FS: Zaero specific game dll changes */
extern	cvar_t* skill;
extern	cvar_t* fraglimit;
extern	cvar_t* timelimit;
extern	cvar_t* password;
extern	cvar_t* spectator_password;
extern	cvar_t* needpass;
extern	cvar_t* g_select_empty;
extern	cvar_t* dedicated;
extern	cvar_t* motd_time; //QW// Added to force scr_centertime.
extern	cvar_t* filterban;
extern	cvar_t* flashlightmode; //QW/ mode for flashlight code.

extern	cvar_t* sv_gravity;
extern	cvar_t* sv_maxvelocity;

extern	cvar_t* gun_x, * gun_y, * gun_z;
extern	cvar_t* sv_rollspeed;
extern	cvar_t* sv_rollangle;

extern	cvar_t* run_pitch;
extern	cvar_t* run_roll;
extern	cvar_t* bob_up;
extern	cvar_t* bob_pitch;
extern	cvar_t* bob_roll;

extern	cvar_t* sv_cheats;
extern	cvar_t* maxclients;
extern	cvar_t* maxspectators;

extern	cvar_t* flood_msgs;
extern	cvar_t* flood_persecond;
extern	cvar_t* flood_waitdelay;

extern	cvar_t* sv_maplist;

extern	cvar_t* plasma_alpha;
extern	cvar_t* g_allow_give;

//cvar_t	*clusterlauncher_aplha; //TODO

extern	cvar_t* sv_stopspeed;		// PGM - this was a define in g_phys.c

//ROGUE
extern	cvar_t* gamerules;
extern	cvar_t* huntercam;
extern	cvar_t* strong_mines;
extern	cvar_t* randomrespawn;

extern	cvar_t* grenadeammotype; /* FS: Zaero specific game dll changes */
extern	cvar_t* grenadeammo; /* FS: Zaero specific game dll changes */
extern	cvar_t* bettyammo; /* FS: Zaero specific game dll changes */

// this is for the count of monsters
#define ENT_SLOTS_LEFT		(ent->monsterinfo.monster_slots - ent->monsterinfo.monster_used)
#define SELF_SLOTS_LEFT		(self->monsterinfo.monster_slots - self->monsterinfo.monster_used)
//ROGUE

#define world	(&g_edicts[0])

// item spawnflags
#define ITEM_TRIGGER_SPAWN		0x00000001
#define ITEM_NO_TOUCH			0x00000002
// 6 bits reserved for editor flags
// 8 bits used as power cube id bits for coop games
#define DROPPED_ITEM			0x00010000
#define	DROPPED_PLAYER_ITEM		0x00020000
#define ITEM_TARGETS_USED		0x00040000

//
// fields are needed for spawning from the entity string
// and saving / loading games
//
#define FFL_SPAWNTEMP		1
#define FFL_NOSPAWN			2

typedef enum fieldtype_e {
	F_INT,
	F_FLOAT,
	F_LSTRING,			// string on disk, pointer in memory, TAG_LEVEL
	F_GSTRING,			// string on disk, pointer in memory, TAG_GAME
	F_VECTOR,
	F_ANGLEHACK,
	F_EDICT,			// index on disk, pointer in memory
	F_ITEM,				// index on disk, pointer in memory
	F_CLIENT,			// index on disk, pointer in memory
	F_FUNCTION,
	F_MMOVE,
	F_IGNORE
} fieldtype_t;

typedef struct field_s
{
	char* name;
	int		ofs;
	fieldtype_t	type;
	int		flags;
} field_t;


extern	field_t fields[];
extern	gitem_t	itemlist[];


//
// g_cmds.c
//
void Cmd_Help_f(edict_t* ent);
void Cmd_Score_f(edict_t* ent);

//
// g_items.c
//
void PrecacheItem(gitem_t* it);
void InitItems(void);
void SetItemNames(void);
gitem_t* FindItem(char* pickup_name);
gitem_t* FindItemByClassname(char* classname);
#define	ITEM_INDEX(x) ((x)-itemlist)
edict_t* Drop_Item(edict_t* ent, gitem_t* item);
void SetRespawn(edict_t* ent, float delay);
void ChangeWeapon(edict_t* ent);
void SpawnItem(edict_t* ent, gitem_t* item);
void Think_Weapon(edict_t* ent);
void Think_Airstrike (edict_t *ent);
int ArmorIndex(edict_t* ent);
int PowerArmorType(edict_t* ent);
gitem_t* GetItemByIndex(int index);
qboolean Add_Ammo(edict_t* ent, gitem_t* item, int count);
void Touch_Item(edict_t* ent, edict_t* other, cplane_t* plane,
	csurface_t* surf);
void Spawn_CoopBackpack(edict_t* ent); /* FS: Coop: Spawn a backpack with our stuff */

//
// g_utils.c
//

#if defined _WIN32
//
// Define a noreturn wrapper for gi.error
//
//__declspec(noreturn) void GameError(char* fmt, ...);
//#else
//__attribute__((noreturn)) void GameError(char* fmt, ...);
#endif

qboolean	KillBox(edict_t* ent);
qboolean MonsterKillBox(edict_t* ent); /* FS: Zaero specific game dll changes */
qboolean MonsterPlayerKillBox(edict_t* ent); /* FS: Zaero specific game dll changes */
void G_ProjectSource(vec3_t point, vec3_t distance, vec3_t forward,
	vec3_t right, vec3_t result);
edict_t* G_Find(edict_t* from, int fieldofs, char* match);
edict_t* findradius(edict_t* from, vec3_t org, float rad);
edict_t* G_PickTarget(char* targetname);
void	G_UseTargets(edict_t* ent, edict_t* activator);
void	G_SetMovedir(vec3_t angles, vec3_t movedir);

void	G_InitEdict(edict_t* e);
edict_t* G_Spawn(void);
void	G_FreeEdict(edict_t* e);

void	G_TouchTriggers(edict_t* ent);
void	G_TouchSolids(edict_t* ent);

char* G_CopyString(const char* in);

float* tv(float x, float y, float z);
char* vtos(vec3_t v);

float vectoyaw(vec3_t vec);
void vectoangles(vec3_t vec, vec3_t angles);
edict_t* Find_LikePlayer(edict_t* ent, char* name, qboolean exact); /* FS: People want this for various Tastyspleen-like commands */
qboolean G_SpawnCheck(int cap); /* FS: Added */

//ROGUE
void G_ProjectSource2(vec3_t point, vec3_t distance, vec3_t forward, vec3_t right,
	vec3_t up, vec3_t result);
float	vectoyaw2(vec3_t vec);
void	vectoangles2(vec3_t vec, vec3_t angles);
edict_t* findradius2(edict_t* from, vec3_t org, float rad);
//ROGUE

//
// g_combat.c
//
qboolean OnSameTeam(edict_t* ent1, edict_t* ent2);
qboolean CanDamage(edict_t* targ, edict_t* inflictor);
void T_Damage(edict_t* targ, edict_t* inflictor, edict_t* attacker, vec3_t dir,
	vec3_t point, vec3_t normal, int damage, int knockback, int dflags, int mod);
void T_RadiusDamage(edict_t* inflictor, edict_t* attacker, float damage, edict_t* ignore,
	float radius, int mod);
char* GetCoopInsult(void); /* FS: Coop: Pick a random insult */
char* GetProperMonsterName(char* monsterName); /* FS: Coop: Get proper name for classname */

//ROGUE
void T_RadiusNukeDamage(edict_t* inflictor, edict_t* attacker, float damage,
	edict_t* ignore, float radius, int mod);
void T_RadiusClassDamage(edict_t* inflictor, edict_t* attacker, float damage,
	char* ignoreClass, float radius, int mod);
void cleanupHealTarget(edict_t* ent);
//ROGUE

void T_RadiusDamagePosition(vec3_t origin, edict_t* inflictor, edict_t* attacker, float damage, edict_t* ignore, float radius, int mod); /* FS: Zaero specific game dll changes */

// damage flags
#define DAMAGE_RADIUS			0x00000001	// damage was indirect
#define DAMAGE_NO_ARMOR			0x00000002	// armour does not protect from this damage
#define DAMAGE_ENERGY			0x00000004	// damage is from an energy based weapon
#define DAMAGE_NO_KNOCKBACK		0x00000008	// do not affect velocity, just view angles
#define DAMAGE_BULLET			0x00000010  // damage is from a bullet (used for ricochets)
#define DAMAGE_NO_PROTECTION	0x00000020  // armor, shields, invulnerability, and godmode have no effect
//ROGUE
#define DAMAGE_DESTROY_ARMOR	0x00000040	// damage is done to armor and health.
#define DAMAGE_NO_REG_ARMOR		0x00000080	// damage skips regular armor
#define DAMAGE_NO_POWER_ARMOR	0x00000100	// damage skips power armor
//ROGUE

#define DAMAGE_ARMORMOSTLY		0x00000040  // reduces the armor more than the health /* FS: Zaero specific game dll changes */


#define DEFAULT_BULLET_HSPREAD	300
#define DEFAULT_BULLET_VSPREAD	500
#define DEFAULT_SHOTGUN_HSPREAD	1000
#define DEFAULT_SHOTGUN_VSPREAD	500
#define DEFAULT_DEATHMATCH_SHOTGUN_COUNT	12
#define DEFAULT_SHOTGUN_COUNT	12
#define DEFAULT_SSHOTGUN_COUNT	20

//
// g_monster.c
//
void monster_fire_bullet(edict_t* self, vec3_t start, vec3_t dir, int damage,
	int kick, int hspread, int vspread, int flashtype);
void monster_fire_shotgun(edict_t* self, vec3_t start, vec3_t aimdir, int damage,
	int kick, int hspread, int vspread, int count, int flashtype);
void monster_fire_blaster(edict_t* self, vec3_t start, vec3_t dir, int damage,
	int speed, int flashtype, int effect);
void monster_fire_grenade(edict_t* self, vec3_t start, vec3_t aimdir, int damage,
	int speed, int flashtype);
void monster_fire_rocket(edict_t* self, vec3_t start, vec3_t dir, int damage,
	int speed, int flashtype);
void monster_fire_railgun(edict_t* self, vec3_t start, vec3_t aimdir, int damage,
	int kick, int flashtype);
void monster_fire_bfg(edict_t* self, vec3_t start, vec3_t aimdir, int damage,
	int speed, int kick, float damage_radius, int flashtype);

/* FS: Coop: Rogue specific */
void monster_fire_blaster2(edict_t* self, vec3_t start, vec3_t dir, int damage,
	int speed, int flashtype, int effect);
void monster_fire_tracker(edict_t* self, vec3_t start, vec3_t dir, int damage,
	int speed, edict_t* enemy, int flashtype);
void monster_fire_heat_rogue(edict_t* self, vec3_t start, vec3_t dir, vec3_t offset,
	int damage, int kick, int flashtype);
void stationarymonster_start(edict_t* self);
void monster_done_dodge(edict_t* self);

/* FS: Coop: Xatrix specific */
void monster_fire_ionripper(edict_t* self, vec3_t start, vec3_t dir, int damage, int speed, int flashtype, int effect);
void monster_fire_heat_xatrix(edict_t* self, vec3_t start, vec3_t dir, int damage, int speed, int flashtype);
void monster_dabeam(edict_t* self);
void monster_fire_blueblaster(edict_t* self, vec3_t start, vec3_t dir, int damage, int speed, int flashtype, int effect);


void M_droptofloor(edict_t* ent);
void monster_think(edict_t* self);
void walkmonster_start(edict_t* self);
void swimmonster_start(edict_t* self);
void flymonster_start(edict_t* self);
void AttackFinished(edict_t* self, float time);
void monster_death_use(edict_t* self);
void M_CatagorizePosition(edict_t* ent);
qboolean M_CheckAttack(edict_t* self);
void M_FlyCheck(edict_t* self);
void M_CheckGround(edict_t* ent);

//
// g_misc.c
//
void ThrowHead(edict_t* self, char* gibname, int damage, int type);
void ThrowClientHead(edict_t* self, int damage);
void ThrowGib(edict_t* self, char* gibname, int damage, int type);
void BecomeExplosion1(edict_t* self);
/* XATRIX */
void ThrowHeadACID(edict_t* self, char* gibname, int damage, int type);
void ThrowGibACID(edict_t* self, char* gibname, int damage, int type);
void SP_misc_teleporter_dest(edict_t* ent);

//
// g_ai.c
//
void AI_SetSightClient(void);

void ai_stand(edict_t* self, float dist);
void ai_move(edict_t* self, float dist);
void ai_walk(edict_t* self, float dist);
void ai_turn(edict_t* self, float dist);
void ai_run(edict_t* self, float dist);
void ai_charge(edict_t* self, float dist);
int range(edict_t* self, edict_t* other);

void FoundTarget(edict_t* self);
qboolean infront(edict_t* self, edict_t* other);
qboolean visible(edict_t* self, edict_t* other);
qboolean FacingIdeal(edict_t* self);
qboolean inweaponLineOfSight(edict_t* self, edict_t* other); /* FS: Zaero specific game dll changes */

//
// g_weapon.c
//
void ThrowDebris(edict_t* self, char* modelname, float speed, vec3_t origin);
qboolean fire_hit(edict_t* self, vec3_t aim, int damage, int kick);
void fire_bullet(edict_t* self, vec3_t start, vec3_t aimdir, int damage,
	int kick, int hspread, int vspread, int mod);
void fire_shotgun(edict_t* self, vec3_t start, vec3_t aimdir, int damage,
	int kick, int hspread, int vspread, int count, int mod);
void fire_blaster(edict_t* self, vec3_t start, vec3_t aimdir, int damage,
	int speed, int effect, qboolean hyper);
void fire_grenade(edict_t* self, vec3_t start, vec3_t aimdir, int damage,
	int speed, float timer, float damage_radius);
void fire_grenade2(edict_t* self, vec3_t start, vec3_t aimdir, int damage,
	int speed, float timer, float damage_radius, qboolean held);
void fire_rocket(edict_t* self, vec3_t start, vec3_t dir, int damage,
	int speed, float damage_radius, int radius_damage);
void fire_rail(edict_t* self, vec3_t start, vec3_t aimdir, int damage, int kick);
void fire_bfg(edict_t* self, vec3_t start, vec3_t dir,
	int damage, int speed, float damage_radius);
/* FS: Coop: Xatrix specific */
void fire_ionripper(edict_t* self, vec3_t start, vec3_t aimdir, int damage, int speed, int effect);
void fire_heat(edict_t* self, vec3_t start, vec3_t dir, int damage, int speed, float damage_radius, int radius_damage);
void fire_blueblaster(edict_t* self, vec3_t start, vec3_t aimdir, int damage, int speed, int effect);
void fire_plasma(edict_t* self, vec3_t start, vec3_t dir, int damage, int speed, float damage_radius, int radius_damage);
//Plasma rifle mod
void fire_plasma2(edict_t* self, vec3_t start, vec3_t aimdir, int damage, int speed);
void fire_trap(edict_t* self, vec3_t start, vec3_t aimdir, int damage, int speed, float timer, float damage_radius, qboolean held);
void fire_cluster(edict_t* self, vec3_t start, vec3_t aimdir, int damage, int speed, float timer, float damage_radius);

//
// g_ptrail.c
//
void PlayerTrail_Init(void);
void PlayerTrail_Add(vec3_t spot);
void PlayerTrail_New(vec3_t spot);
edict_t* PlayerTrail_PickFirst(edict_t* self);
edict_t* PlayerTrail_PickNext(edict_t* self);
edict_t* PlayerTrail_LastSpot(void);

//
// g_client.c
//
void respawn(edict_t* ent);
void BeginIntermission(edict_t* targ);
void PutClientInServer(edict_t* ent);
void InitClientPersistant(gclient_t* client);
void InitClientResp(gclient_t* client);
void InitBodyQue(void);
void ClientBeginServerFrame(edict_t* ent);

//
// g_player.c
//
void player_pain(edict_t* self, edict_t* other, float kick, int damage);
void player_die(edict_t* self, edict_t* inflictor, edict_t* attacker,
	int damage, vec3_t point);

//
// g_svcmds.c
//
void	ServerCommand(void);
qboolean SV_FilterPacket(char* from);

//
// p_view.c
//
void ClientEndServerFrame(edict_t* ent);

//
// p_hud.c
//
void MoveClientToIntermission(edict_t* client);
void G_SetStats(edict_t* ent);
void G_SetSpectatorStats(edict_t* ent);
void G_CheckChaseStats(edict_t* ent);
void ValidateSelectedItem(edict_t* ent);
void DeathmatchScoreboardMessage(edict_t* client, edict_t* killer);
void HelpComputerMessage(edict_t* client);
void InventoryMessage(edict_t* client);
void MapSummaryMessage(void);
float ReadinessTime(void);

//
// g_pweapon.c
//
void PlayerNoise(edict_t* who, vec3_t where, int type);

//
// m_move.c
//
qboolean M_CheckBottom(edict_t* ent);
qboolean M_walkmove(edict_t* ent, float yaw, float dist);
void M_MoveToGoal(edict_t* ent, float dist);
void M_ChangeYaw(edict_t* ent);

//
// g_phys.c
//
void G_RunEntity(edict_t* ent);

//
// g_main.c
//
void SaveClientData(void);
void FetchClientEntData(edict_t* ent);

//
// g_chase.c
//
void UpdateChaseCam(edict_t* ent);
void ChaseNext(edict_t* ent);
void ChasePrev(edict_t* ent);
void GetChaseTarget(edict_t* ent);

//
// p_client.c
//
void RemoveAttackingPainDaemons(edict_t* self);
int P_Clients_Connected(qboolean spectators); /* FS: Coop: Find out how many players are in the game */

//
// g_vote.c
//
void vote_Think(void);
void vote_Reset(void);
void vote_command(edict_t* ent);
void vote_connect(edict_t* ent);
void vote_disconnect_recalc(edict_t* ent);
void vote_stop(edict_t* ent);

//
// g_coop.c
//
void CoopGamemodeInit(void);
int CoopGamemodeExists(const char* gamemode);
int CoopGamemodeCount ();

//====================
// ROGUE PROTOTYPES
//
// g_newweap.c
//
//extern float nuke_framenum;

void fire_flechette(edict_t* self, vec3_t start, vec3_t dir, int damage,
	int speed, int kick);
void fire_prox(edict_t* self, vec3_t start, vec3_t aimdir, int damage,
	int speed);
void fire_nuke(edict_t* self, vec3_t start, vec3_t aimdir, int speed);
void fire_flame(edict_t* self, vec3_t start, vec3_t aimdir, int damage,
	int speed);
void fire_burst(edict_t* self, vec3_t start, vec3_t aimdir, int damage,
	int speed);
void fire_maintain(edict_t*, edict_t*, vec3_t start, vec3_t aimdir,
	int damage, int speed);
void fire_incendiary_grenade(edict_t* self, vec3_t start, vec3_t aimdir,
	int damage, int speed, float timer, float damage_radius);
void fire_player_melee(edict_t* self, vec3_t start, vec3_t aim, int reach,
	int damage, int kick, int quiet, int mod);
void fire_tesla(edict_t* self, vec3_t start, vec3_t aimdir, int damage,
	int speed);
void fire_blaster2(edict_t* self, vec3_t start, vec3_t aimdir, int damage,
	int speed, int effect, qboolean hyper);
void fire_heat_rogue(edict_t* self, vec3_t start, vec3_t aimdir, vec3_t offset,
	int damage, int kick,
	qboolean monster);
void fire_heat_xatrix(edict_t* self, vec3_t start, vec3_t dir, int damage, int speed, float damage_radius, int radius_damage); /* FS: Coop: Xatrix specific */
void fire_tracker(edict_t* self, vec3_t start, vec3_t dir, int damage,
	int speed, edict_t* enemy);

//
// g_newai.c
//
qboolean blocked_checkshot(edict_t* self, float shotChance);
qboolean blocked_checkplat(edict_t* self, float dist);
qboolean blocked_checkjump(edict_t* self, float dist, float maxDown, float maxUp);
qboolean blocked_checknewenemy(edict_t* self);
qboolean monsterlost_checkhint(edict_t* self);
qboolean inback(edict_t* self, edict_t* other);
float realrange(edict_t* self, edict_t* other);
edict_t* SpawnBadArea(vec3_t mins, vec3_t maxs, float lifespan, edict_t* owner);
edict_t* CheckForBadArea(edict_t* ent);
qboolean MarkTeslaArea(edict_t* self, edict_t* tesla);
void InitHintPaths(void);
void PredictAim(edict_t* target, vec3_t start, float bolt_speed, qboolean eye_height,
	float offset, vec3_t aimdir, vec3_t aimpoint);
qboolean below(edict_t* self, edict_t* other);
void drawbbox(edict_t* self);
void M_MonsterDodge(edict_t* self, edict_t* attacker, float eta, trace_t* tr);
void monster_duck_down(edict_t* self);
void monster_duck_hold(edict_t* self);
void monster_duck_up(edict_t* self);
qboolean has_valid_enemy(edict_t* self);
void TargetTesla(edict_t* self, edict_t* tesla);
void hintpath_stop(edict_t* self);
edict_t* PickCoopTarget(edict_t* self);
int CountPlayers(void);
void monster_jump_start(edict_t* self);
qboolean monster_jump_finished(edict_t* self);


//
// g_sphere.c
//
void Defender_Launch(edict_t* self);
void Vengeance_Launch(edict_t* self);
void Hunter_Launch(edict_t* self);

//
// g_newdm.c
//
void InitGameRules(void);
edict_t* DoRandomRespawn(edict_t* ent);
void PrecacheForRandomRespawn(void);
qboolean Tag_PickupToken(edict_t* ent, edict_t* other);
void Tag_DropToken(edict_t* ent, gitem_t* item);
void Tag_PlayerDeath(edict_t* targ, edict_t* inflictor, edict_t* attacker);
void fire_doppleganger(edict_t* ent, vec3_t start, vec3_t aimdir);

//
// g_radio.c
//
void stuffcmd(edict_t* ent, char* s); //QW// for use in g_offworld.c

//
// g_spawn.c
//
edict_t* CreateMonster(vec3_t origin, vec3_t angles, char* classname);
edict_t* CreateFlyMonster(vec3_t origin, vec3_t angles, vec3_t mins,
	vec3_t maxs, char* classname);
edict_t* CreateGroundMonster(vec3_t origin, vec3_t angles, vec3_t mins,
	vec3_t maxs, char* classname, int height);
qboolean FindSpawnPoint(vec3_t startpoint, vec3_t mins, vec3_t maxs,
	vec3_t spawnpoint, float maxMoveUp);
qboolean CheckSpawnPoint(vec3_t origin, vec3_t mins, vec3_t maxs);
qboolean CheckGroundSpawnPoint(vec3_t origin, vec3_t entMins, vec3_t entMaxs,
	float height, float gravity);
void DetermineBBox(char* classname, vec3_t mins, vec3_t maxs);
void SpawnGrow_Spawn(vec3_t startpos, int size);
void Widowlegs_Spawn(vec3_t startpos, vec3_t angles);

// ROGUE PROTOTYPES
//====================

//
// z_item.c
//
qboolean EMPNukeCheck(edict_t* ent, vec3_t pos); /* FS: Zaero specific game dll changes */

//
// z_weapon.c
//
void fire_bb(edict_t* self, vec3_t start, vec3_t aimdir, int damage, int speed, float timer, float damage_radius); /* FS: Zaero specific game dll changes */
void fire_flare(edict_t* self, vec3_t start, vec3_t dir, int damage, int speed, float damage_radius, int radius_damage); /* FS: Zaero specific game dll changes */

//
// z_ai.c
//
void ai_schoolStand(edict_t* self, float dist); /* FS: Zaero specific game dll changes */
void ai_schoolRun(edict_t* self, float dist); /* FS: Zaero specific game dll changes */
void ai_schoolWalk(edict_t* self, float dist); /* FS: Zaero specific game dll changes */
void ai_schoolCharge(edict_t* self, float dist); /* FS: Zaero specific game dll changes */
void ai_schoolBackWalk(edict_t* self, float dist); /* FS: Zaero specific game dll changes */
void ai_schoolSideStepRight(edict_t* self, float dist); /* FS: Zaero specific game dll changes */
void ai_schoolSideStepLeft(edict_t* self, float dist); /* FS: Zaero specific game dll changes */


//============================================================================

// client_t->anim_priority
#define	ANIM_BASIC		0		// stand / run
#define	ANIM_WAVE		1
#define	ANIM_JUMP		2
#define	ANIM_PAIN		3
#define	ANIM_ATTACK		4
#define	ANIM_DEATH		5
#define	ANIM_REVERSE	6

#define COOP_BANDOLIER	1 /* FS: Coop: For respawn */
#define COOP_BACKPACK	2 /* FS: Coop: For respawn */

// client data that stays across multiple level loads
typedef struct client_persistant_s
{
	char		userinfo[MAX_INFO_STRING];
	char		netname[16];
	int			hand;

	qboolean	connected;			// a loadgame will leave valid entities that
									// just don't have a connection yet

	// values saved and restored from edicts when changing levels
	int			health;
	int			max_health;
	int			savedFlags;

	int			selected_item;
	int			inventory[MAX_ITEMS];

	// ammo capacities
	int			max_bullets;
	int			max_shells;
	int			max_rockets;
	int			max_flares; /* FS: Zaero specific game dll changes */
	int			max_grenades;
	int			max_cells;
	int			max_slugs;

	/* XATRIX */
	int			max_magslug;
	int			max_trap;

	int			max_tbombs; /* FS: Zaero specific game dll changes */
	int			max_a2k; /* FS: Zaero specific game dll changes */
	int			max_empnuke; /* FS: Zaero specific game dll changes */
	int			max_plasmashield; /* FS: Zaero specific game dll changes */

	gitem_t* weapon;
	gitem_t* lastweapon;
	gitem_t* lastweapon2; // this if for inventory hiding items /* FS: Zaero specific game dll changes */

	int			power_cubes;	// used for tracking the cubes in coop games
	int			score;			// for calculating total unit score in coop games

	int			game_helpchanged;
	int			helpchanged;

	int			ammoUpgrade; /* FS: Coop: For respawn */

	qboolean	spectator;			// client is a spectator
	qboolean	isAdmin; /* FS: Coop: Admin goodies */
	qboolean	isVIP; /* FS: Coop: VIP goodies */
	qboolean	didMotd; /* FS: Coop: MOTD */
	qboolean	noSummon; /* FS: Blinky Cam */

	int			scanner_active; /* Phatman: Scanner tutorial by Yaya */

//=========
//ROGUE
	int			max_tesla;
	int			max_prox;
	int			max_mines;
	int			max_flechettes;
	int			max_rounds;
	//ROGUE
	//=========

	float visorFrames; /* FS: Zaero specific game dll changes */
	float name_timeout; /* FS: Added */
} client_persistant_t;

// client data that stays across deathmatch respawns
typedef struct client_respawn_s
{
	client_persistant_t	coop_respawn;	// what to set client->pers to on a respawn
	int			enterframe;			// level.framenum the client entered the game
	int			score;				// frags, etc
	vec3_t		cmd_angles;			// angles sent over in the last command

	qboolean	spectator;			// client is a spectator
	qboolean	isAdmin; /* FS: Coop: Admin goodies */
	qboolean	isVIP; /* FS: Coop: VIP goodies */
	qboolean	didMotd; /* FS: Coop: MOTD */
	qboolean	noSummon; /* FS: Blinky Cam */
	int         radio_power; // Its a toggle that functions as the power
} client_respawn_t;

// this structure is cleared on each PutClientInServer(),
// except for 'client->pers'
struct gclient_s
{
	// known to server
	player_state_t	ps;				// communicated by server to clients
	int				ping;

	// private to game
	client_persistant_t	pers;
	client_respawn_t	resp;
	pmove_state_t		old_pmove;	// for detecting out-of-pmove changes

	qboolean	showscores;			// set layout stat
	qboolean inmenu;                /* in menu */
	pmenuhnd_t* menu;               /* current menu */
	void		(*menu_update)(edict_t* ent); /* FS: Added */
	qboolean	showinventory;		// set layout stat
	qboolean	showhelp;
	qboolean	showhelpicon;

	int			ammo_index;

	int			buttons;
	int			oldbuttons;
	int			latched_buttons;

	qboolean	weapon_thunk;

	gitem_t* newweapon;

	// sum up damage over an entire frame, so
	// shotgun blasts give a single big kick
	int			damage_armor;		// damage absorbed by armor
	int			damage_parmor;		// damage absorbed by power armor
	int			damage_blood;		// damage taken out of health
	int			damage_knockback;	// impact damage
	vec3_t		damage_from;		// origin for vector calculation

	float		killer_yaw;			// when dead, look at killer

	weaponstate_t	weaponstate;
	vec3_t		kick_angles;	// weapon kicks
	vec3_t		kick_origin;
	float		v_dmg_roll, v_dmg_pitch, v_dmg_time;	// damage kicks
	float		fall_time, fall_value;		// for view drop on fall
	float		damage_alpha;
	float		bonus_alpha;
	vec3_t		damage_blend;
	vec3_t		v_angle;			// aiming direction
	float		bobtime;			// so off-ground doesn't change it
	vec3_t		oldviewangles;
	vec3_t		oldvelocity;

	float		next_drown_time;
	int			old_waterlevel;
	int			breather_sound;

	int			machinegun_shots;	// for weapon raising

	// animation vars
	int			anim_end;
	int			anim_priority;
	qboolean	anim_duck;
	qboolean	anim_run;

	// powerup timers
	float		quad_framenum;
	float		invincible_framenum;
	float		breather_framenum;
	float		enviro_framenum;
	float		a2kFramenum; /* FS: Zaero specific game dll changes */

	qboolean	grenade_blew_up;
	float		grenade_time;
	// RAFAEL
	float		quadfire_framenum; /* FS: Coop: Xatrix specific */
	qboolean	trap_blew_up; /* FS: Coop: Xatrix specific */
	float		trap_time; /* FS: Coop: Xatrix specific */

	int			silencer_shots;
	int			weapon_sound;

	float		pickup_msg_time;

	float		flood_locktill;		// locked from talking
	float		flood_when[10];		// when messages were said
	int			flood_whenhead;		// head pointer for when said

	float		respawn_time;		// can respawn when time > this

	edict_t* chase_target;		// player we are chasing
	qboolean	update_chase;		// need to update chase info?
	BlinkyClient_t blinky_client; /* FS: Blinky's Coop camera */
	float menutime;                 /* time to update menu */
	qboolean menudirty;

	//=======
	//ROGUE
	float		double_framenum;
	float		ir_framenum;
	//	float		torch_framenum;
	float		nuke_framenum;
	float		tracker_pain_framenum;

	edict_t* owned_sphere;		// this points to the player's sphere
//ROGUE
//=======

	qboolean	spawn_protection; /* FS: Coop: Spawn protection */
	qboolean	spawn_protection_msg; /* FS: Coop: Spawn protection */

	// used for blinding
	int flashTime; /* FS: Zaero specific game dll changes */
	int flashBase; /* FS: Zaero specific game dll changes */

	edict_t* zCameraTrack;    // the entity to see through /* FS: Zaero specific game dll changes */
	vec3_t     zCameraOffset;   // offset from camera origin /* FS: Zaero specific game dll changes */
	edict_t* zCameraLocalEntity; /* FS: Zaero specific game dll changes */
	float zCameraStaticFramenum; /* FS: Zaero specific game dll changes */

	qboolean showOrigin; /* FS: Zaero specific game dll changes */

	// for sniper rifle
	int sniperFramenum; /* FS: Zaero specific game dll changes */

	// for sonic cannon
	float startFireTime; /* FS: Zaero specific game dll changes */

	float summon_time; /* FS: Added */
	float dropTimeout; /* FS: Added */

	// AirStrike Variables
	int		airstrike_called; 	// TRUE if Airstrike called
	vec3_t	airstrike_start; 	// Position of Targeted Entity
	vec3_t	airstrike_targetdir; // Position of Targeted Entity
	vec3_t	airstrike_entry;
	float	airstrike_time;
	int		airstrike_type; 		// 1=Rocket, 2=Cluster

	// flashlight
	edict_t* flashlight;
	int		flashtype;

	int             hook_state;
	edict_t* hook;
	edict_t        *offworld; /* Phatman: Offworld transport */
	qboolean       ready; // Phatman: Ready signal for intermissions
	float          last_ready; // Phatman: Ready signal limiter
};

struct edict_s
{
	entity_state_t	s;
	struct gclient_s* client;	// NULL if not a player
									// the server expects the first part
									// of gclient_s to be a player_state_t
									// but the rest of it is opaque

	qboolean	inuse;
	int			linkcount;

	// FIXME: move these fields to a server private sv_entity_t
	link_t		area;				// linked to a division node or leaf

	int			num_clusters;		// if -1, use headnode instead
	int			clusternums[MAX_ENT_CLUSTERS];
	int			headnode;			// unused if num_clusters != -1
	int			areanum, areanum2;

	//================================

	int			svflags;
	vec3_t		mins, maxs;
	vec3_t		absmin, absmax, size;
	solid_t		solid;
	int			clipmask;
	edict_t* owner;


	// DO NOT MODIFY ANYTHING ABOVE THIS, THE SERVER
	// EXPECTS THE FIELDS IN THAT ORDER!

	//================================
	int			movetype;
	int			flags;

	char* model;
	char* model2; /* FS: Zaero specific game dll changes */
	char* model3; /* FS: Zaero specific game dll changes */
	char* model4; /* FS: Zaero specific game dll changes */
	float		freetime;			// sv.time when the object was freed

	//
	// only used locally in game, not by server
	//
	char* message;
	char* classname;
	int			spawnflags;

	float		timestamp;

	float		angle;			// set in qe3, -1 = up, -2 = down
	char* target;
	char* targetname;
	char* killtarget;
	char* team;
	char* pathtarget;
	char* deathtarget;
	char* combattarget;
	edict_t* target_ent;

	float		speed, accel, decel, aspeed; /* FS: Zaero specific game dll changes: aspeed added */
	vec3_t		movedir;
	vec3_t		pos1, pos2;

	vec3_t		velocity;
	vec3_t		avelocity;
	int			mass;
	float		air_finished;
	float		gravity;		// per entity gravity multiplier (1.0 is normal)
								// use for lowgrav artifact, flares

	edict_t* goalentity;
	edict_t* movetarget;
	float		yaw_speed;
	float		ideal_yaw;

	float		nextthink;
	void		(*prethink) (edict_t* ent);
	void		(*think)(edict_t* self);
	void		(*blocked)(edict_t* self, edict_t* other);	//move to moveinfo?
	void (*touch)(edict_t* self, edict_t* other, cplane_t* plane,
		csurface_t* surf);
	void		(*use)(edict_t* self, edict_t* other, edict_t* activator);
	void		(*pain)(edict_t* self, edict_t* other, float kick, int damage);
	void (*die)(edict_t* self, edict_t* inflictor, edict_t* attacker,
		int damage, vec3_t point);

	float		touch_debounce_time;		// are all these legit?  do we need more/less of them?
	float		pain_debounce_time;
	float		damage_debounce_time;
	float		fly_sound_debounce_time;	//move to clientinfo
	float		last_move_time;

	int			health;
	int			max_health;
	int			gib_health;
	int			deadflag;
	int			show_hostile;

	float		powerarmor_time;

	char* map;			// target_changelevel

	int			viewheight;		// height above origin where eyesight is determined
	int			takedamage;
	int			dmg;
	int			radius_dmg;
	float		dmg_radius;
	int			sounds;			//make this a spawntemp var?
	int			count;

	edict_t* chain;
	edict_t* enemy;
	edict_t* oldenemy;
	edict_t* activator;
	edict_t* groundentity;
	int			groundentity_linkcount;
	edict_t* teamchain;
	edict_t* teammaster;

	edict_t* mynoise;		// can go in client only
	edict_t* mynoise2;

	int			noise_index;
	int			noise_index2;
	float		volume;
	float		attenuation;

	// timing variables
	float		wait;
	float		delay;			// before firing targets
	float		random;

	float		last_sound_time;

	int			watertype;
	int			waterlevel;

	vec3_t		move_origin;
	vec3_t		move_angles;

	float		timer;		//for supergrenades

	// move this to clientinfo?
	int			light_level;

	int			style;			// also used as areaportal number

	gitem_t* item;			// for bonus items

	// common data blocks
	moveinfo_t		moveinfo;
	monsterinfo_t	monsterinfo;

	char* musictrack;	// Knightmare- for specifying OGG or CD track

	int			orders; /* FS: Coop: Xatrix specific */
//=========
//ROGUE
	int			plat2flags;
	vec3_t		offset;
	vec3_t		gravityVector;
	edict_t* bad_area;
	edict_t* hint_chain;
	edict_t* monster_hint_chain;
	edict_t* target_hint_chain;
	int			hint_chain_id;
	//ROGUE
	//=========

	int			coopBackpackInventory[MAX_ITEMS]; /* FS: Coop: Spawn a backpack with our stuff */
	char* coopBackpackNetname; /* FS: Coop: Spawn a backpack with our stuff */
	int			coopBackpackMaxHealth; /* FS: Coop: Backpack max health */
	int			coopBackpackAmmoUpgrade; /* FS: Coop: Backpack bandolier and ammo packs */

	qboolean	voteInitiator; /* FS: Coop: Voting */
	vote_t		hasVoted; /* FS: Coop: Voting */

/* FS: Zaero specific game dll changes */
	// can use this for misc. timeouts
	float timeout;

	// for func_door, also used by monster_autocannon, and misc_securitycamera
	int active;
	int seq;

	// between level saves/loads
	int spawnflags2;
	int oldentnum;

	// titan laser
	edict_t* laser;

	float weaponsound_time;

	// schooling info
	edict_t* zRaduisList, * zSchoolChain;
	float zDistance;

	// this is for MOVETYPE_RIDE
	edict_t* rideWith[2];
	vec3_t rideWithOffset[2];

	// camera number
	vec3_t mangle;

	// time left for the visor (stored if a visor is dropped)
	int visorFrames;

	// monster team
	char* mteam;

	// for random func_timer targets
	char targets[16][MAX_QPATH];
	int numTargets;

	// used by floor-mounted autocannon
	int onFloor;

	float bossFireTimeout;
	int bossFireCount;

	edict_t* hook_laser;
};

//zaero debug includes (need type info)

// Zaero dmflags
#define ZDM_NO_GL_POLYBLEND_DAMAGE	1
#define ZDM_ZAERO_ITEMS				2

//=============
//ROGUE
#define ROGUE_GRAVITY	1

#define SPHERE_DEFENDER			0x0001
#define SPHERE_HUNTER			0x0002
#define SPHERE_VENGEANCE		0x0004
#define SPHERE_DOPPLEGANGER		0x0100

#define SPHERE_TYPE				0x00FF
#define SPHERE_FLAGS			0xFF00

//
// deathmatch games
//
#define		RDM_TAG			2
#define		RDM_DEATHBALL	3

typedef struct dm_game_rs
{
	void		(*GameInit)(void);
	void		(*PostInitSetup)(void);
	void		(*ClientBegin) (edict_t* ent);
	void		(*SelectSpawnPoint) (edict_t* ent, vec3_t origin, vec3_t angles);
	void		(*PlayerDeath)(edict_t* targ, edict_t* inflictor, edict_t* attacker);
	void		(*Score)(edict_t* attacker, edict_t* victim, int scoreChange);
	void		(*PlayerEffects)(edict_t* ent);
	void		(*DogTag)(edict_t* ent, edict_t* killer, char** pic);
	void		(*PlayerDisconnect)(edict_t* ent);
	int			(*ChangeDamage)(edict_t* targ, edict_t* attacker, int damage, int mod);
	int			(*ChangeKnockback)(edict_t* targ, edict_t* attacker, int knockback, int mod);
	int			(*CheckDMRules)(void);
} dm_game_rt;

extern dm_game_rt	DMGame;

void Tag_GameInit(void);
void Tag_PostInitSetup(void);
void Tag_PlayerDeath(edict_t* targ, edict_t* inflictor, edict_t* attacker);
void Tag_Score(edict_t* attacker, edict_t* victim, int scoreChange);
void Tag_PlayerEffects(edict_t* ent);
void Tag_DogTag(edict_t* ent, edict_t* killer, char** pic);
void Tag_PlayerDisconnect(edict_t* ent);
int  Tag_ChangeDamage(edict_t* targ, edict_t* attacker, int damage, int mod);

void DBall_GameInit(void);
void DBall_ClientBegin(edict_t* ent);
void DBall_SelectSpawnPoint(edict_t* ent, vec3_t origin, vec3_t angles);
int  DBall_ChangeKnockback(edict_t* targ, edict_t* attacker, int knockback, int mod);
int  DBall_ChangeDamage(edict_t* targ, edict_t* attacker, int damage, int mod);
void DBall_PostInitSetup(void);
int  DBall_CheckDMRules(void);
//void Tag_PlayerDeath (edict_t *targ, edict_t *inflictor, edict_t *attacker);
//void Tag_Score (edict_t *attacker, edict_t *victim, int scoreChange);
//void Tag_PlayerEffects (edict_t *ent);
//void Tag_DogTag (edict_t *ent, edict_t *killer, char **pic);
//void Tag_PlayerDisconnect (edict_t *ent);
//int  Tag_ChangeDamage (edict_t *targ, edict_t *attacker, int damage);

#endif /* COOP_LOCAL_H */
