#
# Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
# All rights reserved.
# This component and the accompanying materials are made available
# under the terms of "Eclipse Public License v1.0""
# which accompanies this distribution, and is available
# at the URL "http://www.eclipse.org/legal/epl-v10.html".
#
# Initial Contributors:
# Nokia Corporation - initial contribution.
#
# Contributors:
#
# Description: Icons makefile for project Calcsoft
#

ifeq (WINS,$(findstring WINS, $(PLATFORM)))
ZDIR=\epoc32\release\$(PLATFORM)\$(CFG)\z
else
ZDIR=\epoc32\data\z
endif

# ----------------------------------------------------------------------------
# Configure these
# ----------------------------------------------------------------------------

TARGETDIR=$(ZDIR)\resource\apps
HEADERDIR=\epoc32\include
ICONTARGETFILENAME=$(TARGETDIR)\calcsoft.mif
HEADERFILENAME=$(HEADERDIR)\calcsoft.mbg

do_nothing :
	@rem do_nothing

MAKMAKE : do_nothing

BLD : do_nothing

CLEAN : do_nothing

LIB : do_nothing

CLEANLIB : do_nothing

# ----------------------------------------------------------------------------
# Configure these.
#
# NOTE 1: DO NOT DEFINE MASK FILE NAMES! They are included automatically by
# MifConv if the mask detph is defined.
#
# NOTE 2: Usually, source paths should not be included in the bitmap
# definitions. MifConv searches for the icons in all icon directories in a
# predefined order, which is currently \s60\icons, \s60\bitmaps2.
# The directory \s60\icons is included in the search only if the feature flag
# __SCALABLE_ICONS is defined.
# ----------------------------------------------------------------------------

RESOURCE :
	mifconv $(ICONTARGETFILENAME)  /h$(HEADERFILENAME) \
/c8,1 qgn_graf_calc_display.svg\
/c8,8 qgn_graf_calc_paper.svg\
/c8,8 qsn_fr_calc_display_side_l.svg\
/c8,8 qsn_fr_calc_display_center.svg\
/c8,8 qsn_fr_calc_display_side_r.svg\
/c8,8 qsn_fr_calc_paper_center.svg\
/c8,8 qsn_fr_calc_paper_corner_tl.svg\
/c8,8 qsn_fr_calc_paper_corner_tr.svg\
/c8,8 qsn_fr_calc_paper_corner_bl.svg\
/c8,8 qsn_fr_calc_paper_corner_br.svg\
/c8,8 qsn_fr_calc_paper_side_t.svg\
/c8,8 qsn_fr_calc_paper_side_b.svg\
/c8,8 qsn_fr_calc_paper_side_l.svg\
/c8,8 qsn_fr_calc_paper_side_r.svg\
/c8,1 qgn_indi_calc_button_plus.svg\
/c8,1 qgn_indi_calc_button_minus.svg\
/c8,1 qgn_indi_calc_button_multiply.svg\
/c8,1 qgn_indi_calc_button_divide.svg\
/c8,1 qgn_indi_calc_button_sign.svg\
/c8,1 qgn_indi_calc_button_equals.svg\
/c8,1 qgn_indi_calc_button_squareroot.svg\
/c8,1 qgn_indi_calc_button_percent.svg\
/c8,1 qgn_indi_calc_button_up.svg\
/c8,1 qgn_indi_calc_button_down.svg\
/c8,1 qgn_indi_calc_button_plus_pressed.svg\
/c8,1 qgn_indi_calc_button_minus_pressed.svg\
/c8,1 qgn_indi_calc_button_multiply_pressed.svg\
/c8,1 qgn_indi_calc_button_divide_pressed.svg\
/c8,1 qgn_indi_calc_button_sign_pressed.svg\
/c8,1 qgn_indi_calc_button_equals_pressed.svg\
/c8,1 qgn_indi_calc_button_squareroot_pressed.svg\
/c8,1 qgn_indi_calc_button_percent_pressed.svg\
/c8,1 qgn_indi_calc_button_up_pressed.svg\
/c8,1 qgn_indi_calc_button_down_pressed.svg\
/c8,8 qgn_indi_calc_button_up_inactive.svg\
/c8,8 qgn_indi_calc_button_down_inactive.svg\
/c8,8 qgn_indi_calc_button_sign_inactive.svg\
/c8,8 qgn_indi_calc_button_squareroot_inactive.svg\
/c8,8 qgn_indi_calc_button_percent_inactive.svg

FREEZE : do_nothing

SAVESPACE : do_nothing

RELEASABLES :
	@echo $(HEADERFILENAME)&& \
	@echo $(ICONTARGETFILENAME)

FINAL : do_nothing
