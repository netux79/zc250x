AUDIO_LIBS = -lgme -lalogg -lalmp3 -laldmb -ldumb
IMAGE_LIBS = -ljpgal -lldpng -lpng -lz
#LINKOPTS = -pg -g
#LINKOPTS = -pg
#OPTS = -pg -g
#OPTS = -pg
#OPTS = -O3
OPTS = -DPTW32_STATIC_LIB -O3

ALLEG_LIB = `allegro-config --libs --static`
SFLAG = -s
ZC_PLATFORM = Linux
CC = g++ 
CFLAG = -DALLEGRO_LINUX

ZELDA_PREFIX = zelda

ZELDA_EXE = $(ZELDA_PREFIX)$(EXEEXT)
ZQUEST_EXE = $(ZQUEST_PREFIX)$(EXEEXT)
ROMVIEW_EXE = $(ROMVIEW_PREFIX)$(EXEEXT)

ZELDA_OBJECTS = aglogo.o colors.o debug.o decorations.o defdata.o editbox.o EditboxModel.o EditboxView.o ending.o ffscript.o gamedata.o gui.o guys.o init.o items.o jwin.o jwinfsel.o link.o load_gif.o maps.o matrix.o md5.o midi.o pal.o particles.o qst.o save_gif.o script_drawing.o single_instance.o sprite.o subscr.o tab_ctl.o tiles.o title.o weapons.o win32.o zc_custom.o zc_init.o zc_items.o zc_sprite.o zc_subscr.o zc_sys.o zcmusic.o zelda.o zscriptversion.o zsys.o

.PHONY: default veryclean clean all msg linux done

default: all
msg:
	@echo Compiling Zelda Classic for $(ZC_PLATFORM)...
done:
	@echo Done!
clean:
	rm -f $(ZELDA_OBJECTS)
veryclean: clean
	rm -f $(ZELDA_EXE)

all: msg $(ZELDA_EXE) done

$(ZELDA_EXE): $(ZELDA_OBJECTS)
	$(CC) $(LINKOPTS) -o $(ZELDA_EXE) $(ZELDA_OBJECTS) $(IMAGE_LIBS) $(AUDIO_LIBS) $(ALLEG_LIB) $(SFLAG)

aglogo.o: aglogo.cpp gamedata.h zc_alleg.h zdefs.h zeldadat.h
	$(CC) $(OPTS) $(CFLAG) -c aglogo.cpp -o aglogo.o $(SFLAG)
colors.o: colors.cpp colors.h gamedata.h zc_alleg.h zdefs.h
	$(CC) $(OPTS) $(CFLAG) -c colors.cpp -o colors.o $(SFLAG)
debug.o: debug.cpp zc_alleg.h
	$(CC) $(OPTS) $(CFLAG) -c debug.cpp -o debug.o $(SFLAG)
decorations.o: decorations.cpp decorations.h gamedata.h jwin.h maps.h sfx.h sprite.h tab_ctl.h zc_alleg.h zc_custom.h zc_sys.h zcmusic.h zdefs.h zelda.h zeldadat.h zsys.h
	$(CC) $(OPTS) $(CFLAG) -c decorations.cpp -o decorations.o $(SFLAG)
defdata.o: defdata.cpp defdata.h gamedata.h guys.h items.h sfx.h sprite.h weapons.h zc_alleg.h zdefs.h
	$(CC) $(OPTS) $(CFLAG) -c defdata.cpp -o defdata.o $(SFLAG)
editbox.o: editbox.cpp EditboxNew.h jwin.h tab_ctl.h zc_alleg.h
	$(CC) $(OPTS) $(CFLAG) -c editbox.cpp -o editbox.o $(SFLAG)
EditboxModel.o: EditboxModel.cpp editbox.h EditboxNew.h gamedata.h gui.h jwin.h tab_ctl.h zc_alleg.h zdefs.h
	$(CC) $(OPTS) $(CFLAG) -c EditboxModel.cpp -o EditboxModel.o $(SFLAG)
EditboxView.o: EditboxView.cpp EditboxNew.h jwin.h tab_ctl.h zc_alleg.h
	$(CC) $(OPTS) $(CFLAG) -c EditboxView.cpp -o EditboxView.o $(SFLAG)
ending.o: ending.cpp aglogo.h colors.h ending.h gamedata.h guys.h items.h jwin.h jwinfsel.h link.h maps.h matrix.h pal.h qst.h sfx.h sprite.h subscr.h tab_ctl.h tiles.h title.h weapons.h zc_alleg.h zc_custom.h zc_sys.h zcmusic.h zdefs.h zelda.h zeldadat.h zsys.h init.h
	$(CC) $(OPTS) $(CFLAG) -c ending.cpp -o ending.o $(SFLAG)
ffasm.o: ffasm.cpp ffasm.h ffscript.h gamedata.h jwin.h jwinfsel.h midi.h sprite.h tab_ctl.h zc_alleg.h zcmusic.h zdefs.h zquest.h zsys.h
	$(CC) $(OPTS) $(CFLAG) -c ffasm.cpp -o ffasm.o $(SFLAG)
ffscript.o: ffscript.cpp aglogo.h colors.h ffscript.h gamedata.h guys.h items.h jwin.h jwinfsel.h link.h maps.h matrix.h pal.h qst.h sfx.h sprite.h subscr.h tab_ctl.h tiles.h weapons.h zc_alleg.h zc_custom.h zc_init.h zc_sys.h zcmusic.h zdefs.h zelda.h zeldadat.h zsys.h
	$(CC) $(OPTS) $(CFLAG) -c ffscript.cpp -o ffscript.o $(SFLAG)
font.o: /allegro/tools/datedit.h font.cpp font.h zc_alleg.h
	$(CC) $(OPTS) $(CFLAG) -c font.cpp -o font.o $(SFLAG)
gamedata.o: gamedata.cpp gamedata.h items.h jwin.h sfx.h sprite.h tab_ctl.h zc_alleg.h zc_sys.h zcmusic.h zdefs.h zelda.h zeldadat.h zsys.h
	$(CC) $(OPTS) $(CFLAG) -c gamedata.cpp -o gamedata.o $(SFLAG)
gui.o: gui.cpp colors.h debug.h gamedata.h gui.h items.h jwin.h jwinfsel.h midi.h pal.h qst.h sfx.h sprite.h subscr.h tab_ctl.h tiles.h zc_alleg.h zc_sys.h zcmusic.h zdefs.h zelda.h zeldadat.h zquest.h zsys.h
	$(CC) $(OPTS) $(CFLAG) -c gui.cpp -o gui.o $(SFLAG)
guys.o: guys.cpp aglogo.h colors.h defdata.h ffscript.h gamedata.h guys.h items.h jwin.h jwinfsel.h link.h maps.h matrix.h pal.h qst.h sfx.h sprite.h subscr.h tab_ctl.h tiles.h weapons.h zc_alleg.h zc_custom.h zc_sys.h zcmusic.h zdefs.h zelda.h zeldadat.h zsys.h
	$(CC) $(OPTS) $(CFLAG) -c guys.cpp -o guys.o $(SFLAG)
init.o: init.cpp gamedata.h gui.h init.h jwin.h sfx.h tab_ctl.h zc_alleg.h zc_sys.h zcmusic.h zdefs.h zelda.h zeldadat.h zsys.h
	$(CC) $(CFLAG) -c init.cpp -o init.o $(SFLAG)
items.o: items.cpp gamedata.h items.h jwin.h maps.h sfx.h sprite.h tab_ctl.h zc_alleg.h zc_sys.h zcmusic.h zdefs.h zelda.h zeldadat.h zsys.h
	$(CC) $(OPTS) $(CFLAG) -c items.cpp -o items.o $(SFLAG)
jmenu.o: jmenu.cpp gamedata.h jwin.h sfx.h tab_ctl.h zc_alleg.h zc_sys.h zcmusic.h zdefs.h zelda.h zeldadat.h zsys.h
	$(CC) $(OPTS) $(CFLAG) -c jmenu.cpp -o jmenu.o $(SFLAG)
jwin.o: jwin.cpp editbox.h gamedata.h jwin.h tab_ctl.h zc_alleg.h zdefs.h zsys.h
	$(CC) $(OPTS) $(CFLAG) -c jwin.cpp -o jwin.o $(SFLAG)
jwinfsel.o: jwinfsel.cpp jwin.h jwinfsel.h tab_ctl.h zc_alleg.h
	$(CC) $(OPTS) $(CFLAG) -c jwinfsel.cpp -o jwinfsel.o $(SFLAG)
link.o: link.cpp aglogo.h colors.h decorations.h ffscript.h gamedata.h guys.h items.h jwin.h jwinfsel.h link.h maps.h matrix.h pal.h qst.h sfx.h sprite.h subscr.h tab_ctl.h tiles.h title.h weapons.h zc_alleg.h zc_custom.h zc_subscr.h zc_sys.h zcmusic.h zdefs.h zelda.h zeldadat.h zsys.h
	$(CC) $(OPTS) $(CFLAG) -c link.cpp -o link.o $(SFLAG)
load_gif.o: load_gif.cpp load_gif.h zc_alleg.h
	$(CC) $(OPTS) $(CFLAG) -c load_gif.cpp -o load_gif.o $(SFLAG)
maps.o: maps.cpp aglogo.h colors.h ffscript.h gamedata.h guys.h items.h jwin.h jwinfsel.h link.h maps.h matrix.h pal.h particles.h qst.h sfx.h sprite.h subscr.h tab_ctl.h tiles.h weapons.h zc_alleg.h zc_custom.h zc_subscr.h zc_sys.h zcmusic.h zdefs.h zelda.h zeldadat.h zsys.h
	$(CC) $(OPTS) -O3 $(CFLAG) -c maps.cpp -o maps.o $(SFLAG)
matrix.o: matrix.cpp gamedata.h matrix.h zc_alleg.h zc_sys.h zdefs.h
	$(CC) $(OPTS) $(CFLAG) -c matrix.cpp -o matrix.o $(SFLAG)
md5.o: md5.cpp md5.h
	$(CC) $(OPTS) $(CFLAG) -c md5.cpp -o md5.o $(SFLAG)
midi.o: midi.cpp gamedata.h jwin.h midi.h tab_ctl.h zc_alleg.h zdefs.h zsys.h
	$(CC) $(OPTS) $(CFLAG) -c midi.cpp -o midi.o $(SFLAG)
pal.o: pal.cpp aglogo.h colors.h gamedata.h items.h jwin.h jwinfsel.h link.h maps.h matrix.h pal.h qst.h sfx.h sprite.h subscr.h tab_ctl.h tiles.h zc_alleg.h zc_custom.h zc_sys.h zcmusic.h zdefs.h zelda.h zeldadat.h zsys.h
	$(CC) $(OPTS) $(CFLAG) -c pal.cpp -o pal.o $(SFLAG)
particles.o: particles.cpp gamedata.h particles.h sprite.h zc_alleg.h zdefs.h
	$(CC) $(OPTS) $(CFLAG) -c particles.cpp -o particles.o $(SFLAG)
qst.o: qst.cpp colors.h defdata.h font.h gamedata.h guys.h items.h jwin.h jwinfsel.h md5.h midi.h qst.h sfx.h sprite.h subscr.h tab_ctl.h tiles.h weapons.h zc_alleg.h zc_custom.h zcmusic.h zdefs.h zquest.h zsys.h
	$(CC) $(OPTS) $(CFLAG) -c qst.cpp -o qst.o $(SFLAG)
save_gif.o: save_gif.cpp save_gif.h zc_alleg.h
	$(CC) $(OPTS) $(CFLAG) -c save_gif.cpp -o save_gif.o $(SFLAG)
script_drawing.o: script_drawing.cpp ffscript.h maps.h rendertarget.h script_drawing.h tiles.h zc_alleg.h zelda.h
	$(CC) $(OPTS) $(CFLAG) -c script_drawing.cpp -o script_drawing.o $(SFLAG)
single_instance.o: single_instance_unix.cpp single_instance.h
	$(CC) $(OPTS) $(CFLAG) -c single_instance_unix.cpp -o single_instance.o $(SFLAG)
sprite.o: sprite.cpp gamedata.h sprite.h tiles.h zc_alleg.h zdefs.h
	$(CC) $(OPTS) $(CFLAG) -c sprite.cpp -o sprite.o $(SFLAG)
subscr.o: subscr.cpp aglogo.h colors.h gamedata.h guys.h items.h jwin.h jwinfsel.h link.h maps.h matrix.h pal.h qst.h sfx.h sprite.h subscr.h tab_ctl.h tiles.h weapons.h zc_alleg.h zc_custom.h zc_sys.h zcmusic.h zdefs.h zelda.h zeldadat.h zsys.h
	$(CC) $(OPTS) $(CFLAG) -c subscr.cpp -o subscr.o $(SFLAG)
tab_ctl.o: tab_ctl.cpp tab_ctl.h zc_alleg.h
	$(CC) $(OPTS) $(CFLAG) -c tab_ctl.cpp -o tab_ctl.o $(SFLAG)
tiles.o: tiles.cpp gamedata.h jwin.h tab_ctl.h tiles.h zc_alleg.h zdefs.h zsys.h
	$(CC) $(OPTS) -O3 $(CFLAG) -c tiles.cpp -o tiles.o $(SFLAG)
title.o: title.cpp aglogo.h colors.h gamedata.h gui.h items.h jwin.h jwinfsel.h link.h maps.h matrix.h pal.h qst.h sfx.h sprite.h subscr.h tab_ctl.h tiles.h title.h zc_alleg.h zc_custom.h zc_sys.h zcmusic.h zdefs.h zelda.h zeldadat.h zsys.h
	$(CC) $(OPTS) $(CFLAG) -c title.cpp -o title.o $(SFLAG)
weapons.o: weapons.cpp aglogo.h colors.h gamedata.h items.h jwin.h jwinfsel.h link.h maps.h matrix.h pal.h qst.h sfx.h sprite.h subscr.h tab_ctl.h tiles.h weapons.h zc_alleg.h zc_custom.h zc_sys.h zcmusic.h zdefs.h zelda.h zeldadat.h zsys.h
	$(CC) $(OPTS) $(CFLAG) -c weapons.cpp -o weapons.o $(SFLAG)
zc_custom.o: zc_custom.cpp gamedata.h jwin.h sfx.h tab_ctl.h zc_alleg.h zc_custom.h zc_sys.h zcmusic.h zdefs.h zelda.h zeldadat.h zsys.h
	$(CC) $(OPTS) $(CFLAG) -c zc_custom.cpp -o zc_custom.o $(SFLAG)
zc_init.o: zc_init.cpp aglogo.h colors.h gamedata.h gui.h init.h items.h jwin.h jwinfsel.h link.h maps.h matrix.h midi.h pal.h qst.h sfx.h sprite.h subscr.h tab_ctl.h tiles.h zc_alleg.h zc_custom.h zc_init.h zc_sys.h zcmusic.h zdefs.h zelda.h zeldadat.h zq_init.h zquest.h zsys.h
	$(CC) $(OPTS) $(CFLAG) -c zc_init.cpp -o zc_init.o $(SFLAG)
zc_items.o: zc_items.cpp gamedata.h guys.h jwin.h maps.h sfx.h sprite.h tab_ctl.h weapons.h zc_alleg.h zc_sys.h zcmusic.h zdefs.h zelda.h zeldadat.h zsys.h
	$(CC) $(OPTS) $(CFLAG) -c zc_items.cpp -o zc_items.o $(SFLAG)
zc_sprite.o: zc_sprite.cpp gamedata.h jwin.h maps.h sfx.h sprite.h tab_ctl.h tiles.h zc_alleg.h zc_sys.h zcmusic.h zdefs.h zelda.h zeldadat.h zsys.h
	$(CC) $(OPTS) $(CFLAG) -c zc_sprite.cpp -o zc_sprite.o $(SFLAG)
zc_subscr.o: zc_subscr.cpp aglogo.h colors.h gamedata.h guys.h items.h jwin.h jwinfsel.h link.h maps.h matrix.h pal.h qst.h sfx.h sprite.h subscr.h tab_ctl.h tiles.h weapons.h zc_alleg.h zc_custom.h zc_subscr.h zc_sys.h zcmusic.h zdefs.h zelda.h zeldadat.h zsys.h
	$(CC) $(OPTS) $(CFLAG) -c zc_subscr.cpp -o zc_subscr.o $(SFLAG)
zc_sys.o: zc_sys.cpp aglogo.h colors.h debug.h gamedata.h gui.h guys.h init.h items.h jwin.h jwinfsel.h link.h maps.h matrix.h midi.h pal.h particles.h qst.h sfx.h sprite.h subscr.h tab_ctl.h tiles.h title.h weapons.h zc_alleg.h zc_custom.h zc_init.h zc_sys.h zcmusic.h zdefs.h zelda.h zeldadat.h zquest.h zsys.h
	$(CC) $(OPTS) $(CFLAG) -c zc_sys.cpp -o zc_sys.o $(SFLAG)
zcmusic.o: zcmusic.cpp nothread.h zc_alleg.h zcmusic.h zsys.h
	$(CC) $(OPTS) $(CFLAG) -c zcmusic.cpp -o zcmusic.o $(SFLAG)
zelda.o: zelda.cpp aglogo.h colors.h ending.h ffscript.h fontsdat.h gamedata.h guys.h init.h items.h jwin.h jwinfsel.h link.h load_gif.h maps.h matrix.h pal.h particles.h qst.h save_gif.h sfx.h sprite.h subscr.h tab_ctl.h tiles.h title.h weapons.h zc_alleg.h zc_custom.h zc_sys.h zcmusic.h zdefs.h zelda.h zeldadat.h zsys.h
	$(CC) $(OPTS) $(CFLAG) -c zelda.cpp -o zelda.o $(SFLAG)
zscriptversion.o: zscriptversion.cpp zscriptversion.h zelda.h link.h zdefs.h
	$(CC) $(OPTS) $(CFLAG) -c zscriptversion.cpp -o zscriptversion.o $(SFLAG)
win32.o: win32.cpp win32.h
	$(CC) $(OPTS) $(CFLAG) -c win32.cpp -o win32.o $(SFLAG)
zsys.o: zsys.cpp gamedata.h jwin.h tab_ctl.h zc_alleg.h zc_sys.h zdefs.h zsys.h
	$(CC) $(OPTS) $(CFLAG) -c zsys.cpp -o zsys.o $(SFLAG)

