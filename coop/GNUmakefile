#
# Quake2 gamei386.so Makefile for Linux
#
# Jan '98 by Zoid <zoid@idsoftware.com>
#
# ELF only
#
# Edited May 5, 2016 by QwazyWabbit
#
# Requires GNU make
#

# this nice line comes from the linux kernel makefile
ARCH := $(shell uname -m | sed -e s/i.86/i386/ -e s/sun4u/sparc64/ -e s/arm.*/arm/ -e s/sa110/arm/ -e s/alpha/axp/)

# On 64-bit OS use the command: setarch i386 make all
# to obtain the 32-bit binary DLL on 64-bit Linux.

CC = gcc -std=c17 -Wall

# on x64 machines do this preparation:
# sudo apt-get install ia32-libs
# sudo apt-get install libc6-dev-i386
# On Ubuntu 16.x use sudo apt install libc6-dev-i386
# this will let you build 32-bits on ia64 systems
#
# This is for native build
CFLAGS=-O3 -DARCH="$(ARCH)"
# This is for 32-bit build on 64-bit host
ifeq ($(ARCH),i386)
CFLAGS =-m32 -O3 -fPIC -DARCH="$(ARCH)" -DSTDC_HEADERS -I/usr/include
endif

# use this when debugging
#CFLAGS=-g -Og -DDEBUG -DARCH="$(ARCH)" -Wall

# flavors of Linux
ifeq ($(shell uname),Linux)
#SVNDEV := -D'SVN_REV="$(shell svnversion -n .)"'
#CFLAGS += $(SVNDEV)
CFLAGS += -DLINUX
LIBTOOL = ldd
endif

# OS X wants to be Linux and FreeBSD too.
ifeq ($(shell uname),Darwin)
#SVNDEV := -D'SVN_REV="$(shell svnversion -n .)"'
#CFLAGS += $(SVNDEV)
CFLAGS += -DLINUX
LIBTOOL = otool
endif

SHLIBEXT=so
#set position independent code
SHLIBCFLAGS=-fPIC

ORIGDIR=Source

DO_SHLIB_CC=$(CC) $(CFLAGS) $(SHLIBCFLAGS) -o $@ -c $<

.c.o:
	$(DO_SHLIB_CC)

#############################################################################
# SETUP AND BUILD
# GAME
#############################################################################

GAME_OBJS = \
	b_cam.o dm_ball.o dm_tag.o flashlight.o g_ai.o g_chase.o g_cmds.o g_combat.o \
	g_coop.o g_func.o g_items.o g_main.o g_misc.o g_monster.o g_newai.o g_newdm.o \
	g_newfnc.o g_newtarg.o g_newtrig.o g_newweap.o g_offworld.o g_phys.o \
	g_save.o g_spawn.o g_sphere.o g_svcmds.o g_target.o g_trigger.o \
	g_turret.o g_utils.o g_vote.o g_weapon.o menu.o m_actor.o \
	m_berserk.o m_boss2.o m_boss3.o m_boss31.o m_boss32.o m_boss5.o \
	m_brain.o m_carrier.o m_chick.o m_fixbot.o m_flash.o m_flipper.o \
	m_float.o m_flyer.o m_gekk.o m_gladb.o m_gladiator.o m_gunner.o \
	m_hover.o m_infantry.o m_insane.o m_medic.o m_move.o m_mutant.o \
	m_parasite.o m_soldier.o m_stalker.o m_supertank.o m_tank.o \
	m_turret.o m_widow.o m_widow2.o p_client.o p_hook.o p_hud.o p_radio.o \
	p_trail.o p_view.o p_weapon.o q_shared.o supergrenade.o z_acannon.o \
	z_ai.o z_boss.o z_camera.o z_handler.o z_hound.o z_item.o z_mtest.o \
	z_sentien.o z_spawn.o z_trigger.o z_weapon.o

game$(ARCH).real.$(SHLIBEXT) : $(GAME_OBJS)
	$(CC) $(CFLAGS) -shared -o $@ $(GAME_OBJS) -ldl -lm
	$(LIBTOOL) -r $@


#############################################################################
# MISC
#############################################################################

clean:
	/bin/rm -f $(GAME_OBJS)

depend:
	$(CC) -MM $(GAME_OBJS:.o=.c)

depends:
	$(CC) $(CFLAGS) -MM *.c > .depends

all:
	make clean
	make depends
	make -j

-include dependencies

