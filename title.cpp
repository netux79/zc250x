#include <stdio.h>
#include <string.h>

#include "zelda.h"
#include "qst.h"
#include "tiles.h"
#include "pal.h"
#include "sprite.h"
#include "subscr.h"
#include "title.h"
#include "link.h"

extern int skipcont;

/***********************************/
/****  Game Selection Screens  *****/
/***********************************/

/* first the game saving & loading system */

static char SAVE_FILE[1048] = {'\0'};

int readsaves(gamedata *savedata, PACKFILE *f)
{
    word qstpath_len;
    word save_count;
    char name[9];
    byte tempbyte;
    short tempshort;
    word tempword;
    dword tempdword;
    int section_id=0;
    word section_version=0;
    word section_cversion=0;
    int section_size;
    
    //section id
    if(!p_mgetl(&section_id,f,true))
    {
        return 1;
    }
    
    //section version info
    if(!p_igetw(&section_version,f,true))
    {
        return 2;
    }
    
    if(!p_igetw(&section_cversion,f,true))
    {
        return 3;
    }
    
    if(section_version < 11) //Sorry!
    {
        //Currently unsupported
        return 1;
    }
    
    //section size
    if(!p_igetl(&section_size,f,true))
    {
        return 4;
    }
    
    if(!p_igetw(&save_count,f,true))
    {
        return 5;
    }

    if(save_count > MAXSAVES)
        save_count = MAXSAVES;
    
    for(int i=0; i<save_count; i++)
    {
        if(!pfread(name,9,f,true))
        {
            return 6;
        }
        
        savedata[i].set_name(name);
        
        if(!p_getc(&tempbyte,f,true))
        {
            return 7;
        }
        
        /* Custom quest only accepted */
        if (tempbyte!=0xFF)
        {
            return 7;
        }
        
        savedata[i].set_quest(tempbyte);
        
        if(section_version<3)
        {
            if(!p_igetw(&tempword,f,true))
            {
                return 8;
            }
            
            savedata[i].set_counter(tempword, 0);
            savedata[i].set_dcounter(tempword, 0);
            
            if(!p_igetw(&tempword,f,true))
            {
                return 9;
            }
            
            savedata[i].set_maxcounter(tempword, 0);
            
            if(!p_igetw(&tempshort,f,true))
            {
                return 10;
            }
            
            savedata[i].set_dcounter(tempshort, 1);
            
            if(!p_igetw(&tempword,f,true))
            {
                return 11;
            }
            
            savedata[i].set_counter(tempword, 1);
            
            if(!p_igetw(&tempword,f,true))
            {
                return 12;
            }
            
            savedata[i].set_counter(tempword, 3);
            savedata[i].set_dcounter(tempword, 3);
        }
        
        if(!p_igetw(&tempword,f,true))
        {
            return 13;
        }
        
        savedata[i].set_deaths(tempword);
        
        if(section_version<3)
        {
            if(!p_getc(&tempbyte,f,true))
            {
                return 14;
            }
            
            savedata[i].set_counter(tempbyte, 5);
            savedata[i].set_dcounter(tempbyte, 5);
            
            if(!p_getc(&tempbyte,f,true))
            {
                return 15;
            }
            
            savedata[i].set_maxcounter(tempbyte, 2);
        }
        
        if(section_version<4)
        {
            if(!p_getc(&tempbyte,f,true))
            {
                return 16;
            }
            
            savedata[i].set_wlevel(tempbyte);
        }
        
        if(!p_getc(&tempbyte,f,true))
        {
            return 17;
        }
        
        savedata[i].set_cheat(tempbyte);
        char temp;
        
        for(int j=0; j<MAXITEMS; j++)
        {
            if(!p_getc(&temp, f, true))
                return 18;
                
            savedata[i].set_item(j, (temp != 0));
        }
        
        if(!pfread(savedata[i].version,sizeof(savedata[i].version),f,true))
        {
            return 20;
        }

        /* Check if version is valid */
        if (strcmp(savedata[i].version, QHeader.minver) < 0)
        {
            return 20;
        }

        if(!pfread(savedata[i].title,sizeof(savedata[i].title),f,true))
        {
            return 21;
        }
        
        /* Check if quest title matches */
        if (strcmp(savedata[i].title, QHeader.title))
        {
            return 21;
        }

        if(!p_getc(&tempbyte,f,true))
        {
            return 22;
        }
        
        savedata[i].set_hasplayed(tempbyte);
        
        if(!p_igetl(&tempdword,f,true))
        {
            return 23;
        }
        
        savedata[i].set_time(tempdword);
        
        if(!p_getc(&tempbyte,f,true))
        {
            return 24;
        }
        
        savedata[i].set_timevalid(tempbyte);
        
        if(section_version <= 5)
        {
            for(int j=0; j<OLDMAXLEVELS; ++j)
            {
                if(!p_getc(&(savedata[i].lvlitems[j]),f,true))
                {
                    return 25;
                }
            }
        }
        else
        {
            for(int j=0; j<MAXLEVELS; ++j)
            {
                if(!p_getc(&(savedata[i].lvlitems[j]),f,true))
                {
                    return 25;
                }
            }
        }
        
        if(section_version<4)
        {
            if(!p_getc(&tempbyte,f,true))
            {
                return 26;
            }
            
            savedata[i].set_HCpieces(tempbyte);
        }
        
        if(!p_getc(&tempbyte,f,true))
        {
            return 27;
        }
        
        savedata[i].set_continue_scrn(tempbyte);
        
        if(section_version <= 5)
        {
            if(!p_getc(&tempbyte,f,true))
            {
                return 28;
            }
            
            savedata[i].set_continue_dmap(tempbyte);
        }
        else
        {
            if(!p_igetw(&tempword,f,true))
            {
                return 28;
            }
            
            savedata[i].set_continue_dmap(tempword);
        }
        
        if(section_version<3)
        {
            if(!p_igetw(&tempword,f,true))
            {
                return 29;
            }
            
            savedata[i].set_counter(tempword, 4);
            
            if(!p_igetw(&tempword,f,true))
            {
                return 30;
            }
            
            savedata[i].set_maxcounter(tempword, 4);
            
            if(!p_igetw(&tempshort,f,true))
            {
                return 31;
            }
            
            savedata[i].set_dcounter(tempshort, 4);
        }
        
        if(section_version<4)
        {
            if(!p_getc(&tempbyte,f,true))
            {
                return 32;
            }
            
            savedata[i].set_magicdrainrate(tempbyte);
            
            if(!p_getc(&tempbyte,f,true))
            {
                return 33;
            }
            
            savedata[i].set_canslash(tempbyte);
        }
        
        if(section_version <= 5)
        {
            for(int j=0; j<OLDMAXDMAPS; ++j)
            {
                if(!p_getc(&(savedata[i].visited[j]),f,true))
                {
                    return 34;
                }
            }
            
            for(int j=0; j<OLDMAXDMAPS*64; ++j)
            {
                if(!p_getc(&(savedata[i].bmaps[j]),f,true))
                {
                    return 35;
                }
            }
        }
        else
        {
            for(int j=0; j<MAXDMAPS; ++j)
            {
                if(!p_getc(&(savedata[i].visited[j]),f,true))
                {
                    return 34;
                }
            }
            
            for(int j=0; j<MAXDMAPS*64; ++j)
            {
                if(!p_getc(&(savedata[i].bmaps[j]),f,true))
                {
                    return 35;
                }
            }
        }
        
        for(int j=0; j<MAXMAPS2*MAPSCRSNORMAL; j++)
        {
            if(!p_igetw(&savedata[i].maps[j],f,true))
            {
                return 36;
            }
        }
        
        for(int j=0; j<MAXMAPS2*MAPSCRSNORMAL; ++j)
        {
            if(!p_getc(&(savedata[i].guys[j]),f,true))
            {
                return 37;
            }
        }
        
        if(!p_igetw(&qstpath_len,f,true))
        {
            return 38;
        }
        
        if(!pfread(savedata[i].qstpath,qstpath_len,f,true))
        {
            return 39;
        }
        
        // Convert path separators so save files work across platforms (hopefully)
        for(int j=0; j<qstpath_len; j++)
        {
            if(savedata[i].qstpath[j]=='\\')
            {
                savedata[i].qstpath[j]='/';
            }
        }
        
        savedata[i].qstpath[qstpath_len]=0;
        
        if(!pfread(savedata[i].icon,sizeof(savedata[i].icon),f,true))
        {
            return 40;
        }
        
        if(!pfread(savedata[i].pal,sizeof(savedata[i].pal),f,true))
        {
            return 41;
        }
        
        if(section_version <= 5)
        {
            for(int j=0; j<OLDMAXLEVELS; ++j)
            {
                if(!p_getc(&(savedata[i].lvlkeys[j]),f,true))
                {
                    return 42;
                }
            }
        }
        else
        {
            for(int j=0; j<MAXLEVELS; ++j)
            {
                if(!p_getc(&(savedata[i].lvlkeys[j]),f,true))
                {
                    return 42;
                }
            }
        }
        
        if(section_version>1)
        {
            if(section_version <= 5)
            {
                for(int j=0; j<OLDMAXDMAPS*64; j++)
                {
                    for(int k=0; k<8; k++)
                    {
                        if(!p_igetl(&savedata[i].screen_d[j][k],f,true))
                        {
                            return 43;
                        }
                    }
                }
            }
            else if(section_version < 10)
            {
                for(int j=0; j<MAXDMAPS*64; j++)
                {
                    for(int k=0; k<8; k++)
                    {
                        if(!p_igetl(&savedata[i].screen_d[j][k],f,true))
                        {
                            return 43;
                        }
                    }
                }
            }
            else
            {
                for(int j=0; j<MAXDMAPS*MAPSCRSNORMAL; j++)
                {
                    for(int k=0; k<8; k++)
                    {
                        if(!p_igetl(&savedata[i].screen_d[j][k],f,true))
                        {
                            return 43;
                        }
                    }
                }
            }
            
            for(int j=0; j<256; j++)
            {
                if(!p_igetl(&savedata[i].global_d[j],f,true))
                {
                    return 45;
                }
            }
        }
        
        if(section_version>2)
        {
            for(int j=0; j<32; j++)
            {
                if(!p_igetw(&tempword,f,true))
                {
                    return 46;
                }
                
                savedata[i].set_counter(tempword, j);
                
                if(!p_igetw(&tempword,f,true))
                {
                    return 47;
                }
                
                savedata[i].set_maxcounter(tempword, j);
                
                if(!p_igetw(&tempshort,f,true))
                {
                    return 48;
                }
                
                savedata[i].set_dcounter(tempshort, j);
            }
        }
        
        if(section_version>3)
        {
            for(int j=0; j<256; j++)
            {
                if(!p_getc(&tempbyte,f,true))
                {
                    return 49;
                }
                
                savedata[i].set_generic(tempbyte, j);
            }
        }
        
        if(section_version>6)
        {
            if(!p_getc(&tempbyte, f, true))
            {
                return 50;
            }
            
            savedata[i].awpn = tempbyte;
            
            if(!p_getc(&tempbyte, f, true))
            {
                return 51;
            }
            
            savedata[i].bwpn = tempbyte;
        }
        else
        {
            savedata[i].awpn = 0;
            savedata[i].bwpn = 0;
        }
        
        //First we get the size of the vector
        if(!p_igetl(&tempdword, f, true))
            return 53;
            
        if(tempdword != 0) //Might not be any at all
        {
            //Then we allocate the vector
            savedata[i].globalRAM.resize(tempdword);
            
            for(dword j = 0; j < savedata[i].globalRAM.size(); j++)
            {
                ZScriptArray& a = savedata[i].globalRAM[j];
                
                //We get the size of each container
                if(!p_igetl(&tempdword, f, true))
                    return 54;
                    
                //We allocate the container
                a.Resize(tempdword);
                
                //And then fill in the contents
                for(dword k = 0; k < a.Size(); k++)
                    if(!p_igetl(&(a[k]), f, true))
                        return 55;
            }
        }
    }
    
    return 0;
}

// call once at startup
int load_savedgames()
{
    PACKFILE *f=NULL;
    
    /* Calculate the savefile name based on the quest filename */
    replace_extension(SAVE_FILE, quest_path, "sav", sizeof(SAVE_FILE));

    if(saves == NULL)
    {
        saves = new gamedata[MAXSAVES];
        
        if(saves==NULL)
            return 1;
    }

    for(int i=0; i<MAXSAVES; i++)
        saves[i].Clear();

    // see if it's there
    if(!exists(SAVE_FILE))
    {
        goto newdata;
    }
    
    if(file_size_ex(SAVE_FILE) == 0)
    {
        if(errno==0) // No error, file's empty
        {
            goto init;
        }
        else // Error...
        {
            goto cantopen;
        }
    }
    
    // load the games
    f = pack_fopen(SAVE_FILE, F_READ_PACKED);
    
    if(!f)
        goto cantopen;
        
    if(readsaves(saves,f)!=0)
        goto reset;
     
    pack_fclose(f);
    return 0;
    
newdata:
    Z_message("Save file not found: %s. Creating new save file.", SAVE_FILE);
    goto init;
    
cantopen:
    Z_message("Can't Open Saved Game File: %s, exiting...", SAVE_FILE);
    exit(1);
    
reset:
    if(f)
        pack_fclose(f);
        
    Z_message("Format error or entries don't match with quest: %s. Resetting game data... ", SAVE_FILE);
    
    for(int i=0; i<MAXSAVES; i++)
        saves[i].Clear();
    
init:
    
    return 0;
}


int writesaves(gamedata *savedata, PACKFILE *f)
{
    int section_id=ID_SAVEGAME;
    int section_version=V_SAVEGAME;
    int section_cversion=CV_SAVEGAME;
    int section_size=0;
    word save_count=0;

    /* Calculate the # of saves */
    while(save_count < MAXSAVES && saves[save_count].get_quest()>0)
        ++save_count;

    /* Nothing to do if there are no save slots to write */
    if (!save_count)
        return -1;

    //section id
    if(!p_mputl(section_id,f))
    {
        return 1;
    }
    
    //section version info
    if(!p_iputw(section_version,f))
    {
        return 2;
    }
    
    if(!p_iputw(section_cversion,f))
    {
        return 3;
    }
    
    //section size
    if(!p_iputl(section_size,f))
    {
        return 4;
    }
    
    if(!p_iputw(save_count,f))
    {
        return 5;
    }
    
    for(int i=0; i<save_count; i++)
    {
        word qstpath_len=(word)strlen(savedata[i].qstpath);
        
        if(!pfwrite(savedata[i].get_name(),9,f))
        {
            return 6;
        }
        
        if(!p_putc(savedata[i].get_quest(),f))
        {
            return 7;
        }
        
        if(!p_iputw(savedata[i].get_deaths(),f))
        {
            return 13;
        }
        
        if(!p_putc(savedata[i].get_cheat(),f))
        {
            return 17;
        }
        
        for(int j=0; j<MAXITEMS; j++)
        {
            if(!p_putc(savedata[i].get_item(j) ? 1 : 0,f))
                return 18;
        }
        
        if(!pfwrite(savedata[i].version,sizeof(savedata[i].version),f))
        {
            return 20;
        }
        
        if(!pfwrite(savedata[i].title,sizeof(savedata[i].title),f))
        {
            return 21;
        }
        
        if(!p_putc(savedata[i].get_hasplayed(),f))
        {
            return 22;
        }
        
        if(!p_iputl(savedata[i].get_time(),f))
        {
            return 23;
        }
        
        if(!p_putc(savedata[i].get_timevalid(),f))
        {
            return 24;
        }
        
        if(!pfwrite(savedata[i].lvlitems,MAXLEVELS,f))
        {
            return 25;
        }
        
        if(!p_putc(savedata[i].get_continue_scrn(),f))
        {
            return 27;
        }
        
        if(!p_iputw(savedata[i].get_continue_dmap(),f))
        {
            return 28;
        }
        
        if(!pfwrite(savedata[i].visited,MAXDMAPS,f))
        {
            return 34;
        }
        
        if(!pfwrite(savedata[i].bmaps,MAXDMAPS*64,f))
        {
            return 35;
        }
        
        for(int j=0; j<MAXMAPS2*MAPSCRSNORMAL; j++)
        {
            if(!p_iputw(savedata[i].maps[j],f))
            {
                return 36;
            }
        }
        
        if(!pfwrite(savedata[i].guys,MAXMAPS2*MAPSCRSNORMAL,f))
        {
            return 37;
        }
        
        if(!p_iputw(qstpath_len,f))
        {
            return 38;
        }
        
        if(!pfwrite(savedata[i].qstpath,qstpath_len,f))
        {
            return 39;
        }
        
        if(!pfwrite(savedata[i].icon,sizeof(savedata[i].icon),f))
        {
            return 40;
        }
        
        if(!pfwrite(savedata[i].pal,sizeof(savedata[i].pal),f))
        {
            return 41;
        }
        
        if(!pfwrite(savedata[i].lvlkeys,MAXLEVELS,f))
        {
            return 42;
        }
        
        for(int j=0; j<MAXDMAPS*MAPSCRSNORMAL; j++)
        {
            for(int k=0; k<8; k++)
            {
                if(!p_iputl(savedata[i].screen_d[j][k],f))
                {
                    return 43;
                }
            }
        }
        
        for(int j=0; j<256; j++)
        {
            if(!p_iputl(savedata[i].global_d[j],f))
            {
                return 44;
            }
        }
        
        for(int j=0; j<32; j++)
        {
            if(!p_iputw(savedata[i].get_counter(j), f))
            {
                return 45;
            }
            
            if(!p_iputw(savedata[i].get_maxcounter(j), f))
            {
                return 46;
            }
            
            if(!p_iputw(savedata[i].get_dcounter(j), f))
            {
                return 47;
            }
        }
        
        for(int j=0; j<256; j++)
        {
            if(!p_putc(savedata[i].get_generic(j), f))
            {
                return 48;
            }
        }
        
        if(!p_putc(savedata[i].awpn, f))
        {
            return 49;
        }
        
        if(!p_putc(savedata[i].bwpn, f))
        {
            return 50;
        }
        
        //First we put the size of the vector
        if(!p_iputl(savedata[i].globalRAM.size(), f))
            return 51;
            
        for(dword j = 0; j < savedata[i].globalRAM.size(); j++)
        {
            ZScriptArray& a = savedata[i].globalRAM[j];
            
            //Then we put the size of each container
            if(!p_iputl(a.Size(), f))
                return 52;
                
            //Followed by its contents
            for(dword k = 0; k < a.Size(); k++)
                if(!p_iputl(a[k], f))
                    return 53;
        }
    }
    
    return 0;
}

int save_savedgames()
{
    if(saves==NULL)
        return 1;
        
    PACKFILE *f = pack_fopen(SAVE_FILE, F_WRITE_PACKED);
    
    if(!f)
    {
        return 2;
    }
    
    if(writesaves(saves, f)!=0)
    {
        pack_fclose(f);
        return 3;
    }
    
    pack_fclose(f);
    
    return 0;
}

void load_game_icon(gamedata *g)
{
    flushItemCache();
    int maxringid = getHighestLevelOfFamily(g, itemsbuf, itype_ring);
    
    int ring = (maxringid != -1) ? itemsbuf[maxringid].fam_type-1 : 0;
    
    int t = QMisc.icons[ring];
    
    if(t<0 || t>=NEWMAXTILES)
    {
        t=0;
    }
    
    int tileind = t ? t : 28;
    
    byte *si = newtilebuf[tileind].data;
    
    if(newtilebuf[tileind].format==tf8Bit)
    {
        for(int j=0; j<128; j++)
        {
            g->icon[j] = 0;
        }
    }
    else
    {
        for(int j=0; j<128; j++)
        {
            g->icon[j] = *(si++);
        }
    }
    
    if(t)
    {
        si = colordata + CSET(pSprite(ring+spICON1))*3;
    }
    else
    {
        if(ring)
        {
            si = colordata + CSET(pSprite(ring-1+spBLUE))*3;
        }
        else
        {
            si = colordata + CSET(6)*3;
        }
    }
    
    if(newtilebuf[tileind].format==tf8Bit)
    {
        for(int j=0; j<48; j++)
        {
            g->pal[j] = 0;
        }
    }
    else
    {
        for(int j=0; j<48; j++)
        {
            g->pal[j] = *(si++);
        }
    }
}

static void select_mode()
{
    textout_ex(scrollbuf,zfont,"REGISTER YOUR NAME",48,152,1,-1);
    textout_ex(scrollbuf,zfont,"COPY FILE",48,168,1,-1);
    textout_ex(scrollbuf,zfont,"DELETE FILE",48,184,1,-1);
}

static void register_mode()
{
    textout_ex(scrollbuf,zfont,"REGISTER YOUR NAME",48,152,2,-1);
}

static void copy_mode()
{
    textout_ex(scrollbuf,zfont,"COPY FILE",48,168,2,-1);
}

static void delete_mode()
{
    textout_ex(scrollbuf,zfont,"DELETE FILE",48,184,2,-1);
}

static void selectscreen()
{
    loadfullpal();
    clear_bitmap(scrollbuf);
    frame2x2(scrollbuf,&QMisc,24,48,QMisc.colors.blueframe_tile,QMisc.colors.blueframe_cset,26,20,0,1,0);
    textout_ex(scrollbuf,zfont,"- S E L E C T -",64,24,1,0);
    textout_ex(scrollbuf,zfont," NAME ",80,48,1,0);
    textout_ex(scrollbuf,zfont," LIFE ",152,48,1,0);
    select_mode();
    /* Setup the font color for the selection screen */
    /*RAMpal[0] = _RGB(0,0,0);     Screen background */
    RAMpal[1] = _RGB(63,63,63); /* Regular text */
    RAMpal[2] = _RGB(57,0,22);  /* Selected text */
}

static byte left_arrow_str[] = {132,0};
static byte right_arrow_str[] = {133,0};

static int savecnt;

static void list_save(int save_num, int ypos)
{
    bool r = refreshpal;
    
    if(save_num<savecnt)
    {
        game->set_maxlife(saves[save_num].get_maxlife());
        game->set_life(saves[save_num].get_maxlife());
        wpnsbuf[iwQuarterHearts].tile = 4;
        //boogie!
        lifemeter(framebuf,144,ypos+((game->get_maxlife()>16*(HP_PER_HEART))?8:0),0,0);
        textout_ex(framebuf,zfont,saves[save_num].get_name(),72,ypos+16,1,-1);
        
        if(saves[save_num].get_quest())
            textprintf_ex(framebuf,zfont,72,ypos+24,1,-1,"%3d",saves[save_num].get_deaths());
        
        textprintf_ex(framebuf,zfont,72,ypos+16,1,-1,"%s",saves[save_num].get_name());
    }
    
    byte *hold = newtilebuf[0].data;
    byte holdformat=newtilebuf[0].format;
    newtilebuf[0].format=tf4Bit;
    newtilebuf[0].data = saves[save_num].icon;
    overtile16(framebuf,0,48,ypos+17,(save_num%3)+csICON,0);               //link
    newtilebuf[0].format=holdformat;
    newtilebuf[0].data = hold;
    
    hold = colordata;
    colordata = saves[save_num].pal;
    loadpalset((save_num%3)+csICON,0);
    colordata = hold;
    
    textout_ex(framebuf,zfont,"-",136,ypos+16,1,-1);
    
    refreshpal = r;
}

static void list_saves()
{
    
    for(int i=0; i<3; i++)
    {
        list_save(listpos+i,i*24+56);
    }
    
    // Draw the arrows above the lifemeter!
    if(savecnt>3)
    {
        if(listpos>=3)
            textout_ex(framebuf,zfont,(char *)left_arrow_str,96,60,2,-1);
            
        if(listpos+3<savecnt)
            textout_ex(framebuf,zfont,(char *)right_arrow_str,176,60,2,-1);
            
        textprintf_ex(framebuf,zfont,112,60,2,-1,"%2d - %-2d",listpos+1,listpos+3);
    }
}

static void draw_cursor(int pos)
{
    if(pos<3)
        overtile8(framebuf,0,40,pos*24+77,1,0);
    else
        overtile8(framebuf,0,40,(pos-3)*16+153,1,0);
}

static bool register_name()
{
    if(savecnt>=MAXSAVES)
        return false;
    
    saves[savecnt].set_maxlife(3*HP_PER_HEART);
    saves[savecnt].set_maxbombs(8);
    saves[savecnt].set_continue_dmap(0);
    saves[savecnt].set_continue_scrn(0xFF);
    
    int s=savecnt;
    ++savecnt;
    listpos=(s/3)*3;
    
    int y=72;
    int x=0;
    int spos=0;
    char name[9];
    
    memset(name,0,9);
    register_mode();
    clear_keybuf();
    refreshpal=true;
    bool done=false;
    bool cancel=false;
    
    int letter_grid_x=34;
    int letter_grid_y=120;
    int letter_grid_offset=10;
    int letter_grid_width=16;
    int letter_grid_height=6;
    int letter_grid_spacing=12;
    const char *complete_grid=" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
    
    clear_bitmap(framebuf);
    frame2x2(framebuf,&QMisc,24,48,QMisc.colors.blueframe_tile,QMisc.colors.blueframe_cset,26,8,0,1,0);
    textout_ex(framebuf,zfont," NAME ",80,48,1,0);
    textout_ex(framebuf,zfont," LIFE ",152,48,1,0);
    
    frame2x2(framebuf,&QMisc,letter_grid_x-letter_grid_offset,letter_grid_y-letter_grid_offset,QMisc.colors.blueframe_tile,QMisc.colors.blueframe_cset,26,11,0,1,0);
    
    textout_ex(framebuf,zfont,"   \"  $  &  (  *  ,  .",letter_grid_x,   letter_grid_y,   1,-1);
    textout_ex(framebuf,zfont, "!  #  %  '  )  +  -  /",letter_grid_x+12,letter_grid_y,   1,-1);
    textout_ex(framebuf,zfont,"0  2  4  6  8  :  <  >", letter_grid_x,   letter_grid_y+12,1,-1);
    textout_ex(framebuf,zfont, "1  3  5  7  9  ;  =  ?",letter_grid_x+12,letter_grid_y+12,1,-1);
    textout_ex(framebuf,zfont,"@  B  D  F  H  J  L  N", letter_grid_x,   letter_grid_y+24,1,-1);
    textout_ex(framebuf,zfont, "A  C  E  G  I  K  M  O",letter_grid_x+12,letter_grid_y+24,1,-1);
    textout_ex(framebuf,zfont,"P  R  T  V  X  Z  \\  ^",letter_grid_x,   letter_grid_y+36,1,-1);
    textout_ex(framebuf,zfont, "Q  S  U  W  Y  [  ]  _",letter_grid_x+12,letter_grid_y+36,1,-1);
    textout_ex(framebuf,zfont,"`  b  d  f  h  j  l  n", letter_grid_x,   letter_grid_y+48,1,-1);
    textout_ex(framebuf,zfont, "a  c  e  g  i  k  m  o",letter_grid_x+12,letter_grid_y+48,1,-1);
    textout_ex(framebuf,zfont,"p  r  t  v  x  z  |  ~", letter_grid_x,   letter_grid_y+60,1,-1);
    textout_ex(framebuf,zfont, "q  s  u  w  y  {  }",   letter_grid_x+12,letter_grid_y+60,1,-1);
    
    advanceframe(true);
    blit(framebuf,scrollbuf,0,0,0,0,256,224);
    
    int grid_x=0;
    int grid_y=0;
    
    
    do
    {
        spos = grid_y*letter_grid_width+grid_x;
        load_control_state();
        
        if(rLeft())
        {
            --grid_x;
            
            if(grid_x<0)
            {
                grid_x=letter_grid_width-1;
                --grid_y;
                
                if(grid_y<0)
                {
                    grid_y=letter_grid_height-1;
                }
            }
            
            sfx(WAV_CHIME);
        }
        else if(rRight())
        {
            ++grid_x;
            
            if(grid_x>=letter_grid_width)
            {
                grid_x=0;
                ++grid_y;
                
                if(grid_y>=letter_grid_height)
                {
                    grid_y=0;
                }
            }
            
            sfx(WAV_CHIME);
        }
        else if(rUp())
        {
            --grid_y;
            
            if(grid_y<0)
            {
                grid_y=letter_grid_height-1;
            }
            
            sfx(WAV_CHIME);
        }
        else if(rDown())
        {
            ++grid_y;
            
            if(grid_y>=letter_grid_height)
            {
                grid_y=0;
            }
            
            sfx(WAV_CHIME);
        }
        else if(rBbtn())
        {
            ++x;
            
            if(x>=8)
            {
                x=0;
            }
        }
        else if(rAbtn())
        {
            name[zc_min(x,7)]=complete_grid[spos];
            ++x;
            
            if(x>=8)
            {
                x=0;
            }
            
            sfx(WAV_PLACE);
        }
        else if(rSbtn())
        {
            done=true;
            int ltrs=0;
            
            for(int i=0; i<8; i++)
            {
                if(name[i]!=' ' && name[i]!=0)
                {
                    ++ltrs;
                }
            }
            
            if(!ltrs)
            {
                cancel=true;
            }
        }
        
        saves[s].set_name(name);
        blit(scrollbuf,framebuf,0,0,0,0,256,224);
        list_save(s,56);
        
        int x2=letter_grid_x + grid_x*letter_grid_spacing;
        int y2=letter_grid_y + grid_y*letter_grid_spacing;
        
        if(frame&8)
        {
            int tx=(zc_min(x,7)<<3)+72;
            
            for(int dy=0; dy<8; dy++)
            {
                for(int dx=0; dx<8; dx++)
                {
                    if(framebuf->line[y+dy][tx+dx]!=1)
                    {
                        framebuf->line[y+dy][tx+dx]=2;
                    }
                    
                    if(framebuf->line[y2+dy][x2+dx]!=1)
                    {
                        framebuf->line[y2+dy][x2+dx]=2;
                    }
                }
            }
        }
        
        draw_cursor(0);
        advanceframe(true);
    }
    while(!done && !Quit);
    
    if(x<0 || cancel)
    {
        done=false;
    }
    
    if(done)
    {
        /* Need to temporary point game global to the new 
           save slot to correctly initilize it */
        gamedata *oldgame = game;
        game = saves+s;

        game->set_quest(0xFF); /* Now it will always be a custom quest */
        sprintf(game->qstpath, "%s", quest_path);
        strcpy(game->version, QHeader.version);
        strcpy(game->title, QHeader.title);

        flushItemCache();
        game->set_maxlife(zinit.hc*HP_PER_HEART);
        int maxringid = getHighestLevelOfFamily(&zinit, itemsbuf, itype_ring);
        if(maxringid != -1)
        {
            getitem(maxringid, true);
        }
        ringcolor(false);
        load_game_icon(game);
        game->set_timevalid(1);

        /* Restore original game global */
        game = oldgame;
    }
    
    if(x<0 || cancel)
    {
        for(int i=s; i<MAXSAVES-1; i++)
            saves[i]=saves[i+1];
            
        saves[MAXSAVES-1].Clear();
        --savecnt;
        
        if(listpos>savecnt-1)
            listpos=zc_max(listpos-3,0);
    }
    
    selectscreen();
    list_saves();
    select_mode();
    return done;
}

static bool copy_file(int file)
{
    if(savecnt<MAXSAVES && file<savecnt)
    {
        saves[savecnt]=saves[file];
        ++savecnt;
        listpos=((savecnt-1)/3)*3;
        sfx(WAV_SCALE);
        select_mode();
        return true;
    }
    
    return false;
}

static bool delete_save(int file)
{
    if(file<savecnt)
    {
        for(int i=file; i<MAXSAVES-1; i++)
        {
            saves[i]=saves[i+1];
        }
        
        saves[MAXSAVES-1].Clear();
        --savecnt;
        
        if(listpos>savecnt-1)
            listpos=zc_max(listpos-3,0);
            
        sfx(WAV_OUCH);
        select_mode();
        return true;
    }
    
    return false;
}

/** game mode stuff **/

static int game_details(int file)
{

    int pos=file%3;
    
    if(saves[file].get_quest()==0)
        return 0;
        
   BITMAP *info = create_bitmap_ex(8, 160, 26);
   blit(framebuf, info, 48, pos * 24 + 70, 0, 0, 160, 26);
   rectfill(framebuf, 40, 60, 216, 192, 0);
   frame2x2(framebuf,&QMisc,24,48,QMisc.colors.blueframe_tile,QMisc.colors.blueframe_cset,26,20,0,1,0);
   textout_ex(framebuf, zfont, " NAME ", 80, 48, 1, 0);
   textout_ex(framebuf, zfont, " LIFE ", 152, 48, 1, 0);
    
    int i=pos*24+70;
    
    do
    {
        blit(info,framebuf,0,0,48,i,160,26);
        advanceframe(true);
        i-=pos+pos;
    }
    while(pos && i>=70);
    
    destroy_bitmap(info);
    
    textout_ex(framebuf,zfont,"GAME TYPE",40,104,2,-1);
    textout_ex(framebuf,zfont,"QUEST",40,112,2,-1);
    textout_ex(framebuf,zfont,"STATUS",40,120,2,-1);
    
    textout_ex(framebuf,zfont,"Custom Quest",120,104,1,-1);
    textprintf_ex(framebuf,zfont,120,112,1,-1,"%s", get_filename(saves[file].qstpath));
    
    if(!saves[file].get_hasplayed())
        textout_ex(framebuf,zfont,"Empty Game",120,120,1,-1);
    else if(!saves[file].get_timevalid())
        textout_ex(framebuf,zfont,"Time Unknown",120,120,1,-1);
    else
        textout_ex(framebuf,zfont,time_str_med(saves[file].get_time()),120,120,1,-1);
        
    if(saves[file].get_cheat())
        textout_ex(framebuf,zfont,"Uses Cheats",120,128,1,-1);
        
    textout_ex(framebuf,zfont,"START: PLAY GAME",56,152,1,-1);
    textout_ex(framebuf,zfont,"    B: CANCEL",56,168,1,-1);
    
    while(!Quit)
    {
        advanceframe(true);
        load_control_state();
        
        if(rBbtn())
        {
            blit(scrollbuf,framebuf,0,0,0,0,256,224);
            return 0;
        }
        
        if(rSbtn())
        {
            blit(framebuf,scrollbuf,0,0,0,0,256,224);
            return 1;
        }
    }
    
    return 0;
}

static int saveslot = -1;

static void select_game()
{
    
    int pos = zc_max(zc_min(currgame-listpos,3),0);
    int mode = 0;
    
    selectscreen();
    
    savecnt=0;
    
    while(savecnt < MAXSAVES && saves[savecnt].get_quest()>0)
        ++savecnt;
        
    if(savecnt == 0)
        pos=3;
        
    bool done=false;
    refreshpal=true;
    
    do
    {
        load_control_state();
        sfxdat=1;
        blit(scrollbuf,framebuf,0,0,0,0,256,224);
        list_saves();
        draw_cursor(pos);
        advanceframe(true);
        saveslot = pos + listpos;
        
        if(rSbtn())
            switch(pos)
            {
            case 3:
                if(!register_name())
                    pos = 3;
                else
                    pos = (savecnt-1)%3;
                    
                refreshpal=true;
                break;
                
            case 4:
                if(savecnt && savecnt<MAXSAVES)
                {
                    mode=2;
                    pos=0;
                    copy_mode();
                }
                
                refreshpal=true;
                break;
                
            case 5:
                if(savecnt)
                {
                    mode=3;
                    pos=0;
                    delete_mode();
                }
                
                refreshpal=true;
                break;
                
            default:
                switch(mode)
                {
                case 0:
                    currgame=saveslot;
                    if(saves[currgame].get_quest())
                        done=true;
                        
                    break;
                    
                case 2:
                    if(copy_file(saveslot))
                    {
                        mode=0;
                        pos=(savecnt-1)%3;
                        refreshpal=true;
                    }
                    
                    break;
                    
                case 3:
                    if(delete_save(saveslot))
                    {
                        mode=0;
                        pos=3;
                        refreshpal=true;
                    }
                    
                    break;
                }
            }
            
        if(rUp())
        {
            --pos;
            
            if(pos<0)
                pos=(mode)?2:5;
                
            sfx(WAV_CHIME);
        }
        
        if(rDown())
        {
            ++pos;
            
            if(pos>((mode)?2:5))
                pos=0;
                
            sfx(WAV_CHIME);
        }
        
        if(rLeft() && listpos>2)
        {
            listpos-=3;
            sfx(WAV_CHIME);
            refreshpal=true;
        }
        
        if(rRight() && listpos+3<savecnt)
        {
            listpos+=3;
            sfx(WAV_CHIME);
            refreshpal=true;
        }
        
        if(rBbtn() && mode)
        {
            if(mode==2) pos=4;
            
            if(mode==3) pos=5;
            
            mode=0;
            select_mode();
        }
        
        if(rAbtn() && !mode && pos<3)
        {
            if(game_details(saveslot))
            {
                currgame=saveslot;
                if(saves[currgame].get_quest())
                    done=true;
            }
        }
    }
    while(!Quit && !done);
    
    saveslot = -1;
}

/**************************************/
/****  Main title screen routine  *****/
/**************************************/

void titlescreen()
{
    int q=Quit;
    
    Quit=0;
    playing=false;
    
    if(q==qCONT)
    {
        cont_game();
        return;
    }
    
    if(q==qRESET)
    {
        show_subscreen_dmap_dots=true;
        show_subscreen_numbers=true;
        show_subscreen_items=true;
        show_subscreen_life=true;
        reset_combo_animations();
        reset_combo_animations2();
    }
    
    if(!Quit)
    {
        select_game();
    }
    
    if(!Quit)
    {
        init_game();
    }
}

void game_over(int type)
{
    kill_sfx();
    music_stop();
    clear_to_color(screen,BLACK);
    loadfullpal();
    
    if(Quit==qGAMEOVER)
        jukebox(ZC_MIDI_GAMEOVER);
        
    Quit=0;
    
    clear_bitmap(framebuf);
    textout_ex(framebuf,zfont,"CONTINUE",88,72,QMisc.colors.msgtext,-1);
    
    if(!type)
    {
        textout_ex(framebuf,zfont,"SAVE",88,96,QMisc.colors.msgtext,-1);
        textout_ex(framebuf,zfont,"RETRY",88,120,QMisc.colors.msgtext,-1);
    }
    else
        textout_ex(framebuf,zfont,"RETRY",88,96,QMisc.colors.msgtext,-1);
        
    int pos = 0;
    int f=-1;
    int htile = 2;
    bool done=false;
    
    do load_control_state();
    
    while(rSbtn());
    
    do
    {
        load_control_state();
        
        if(f==-1)
        {
            if(rUp())
            {
                sfx(WAV_CHINK);
                pos=(pos==0)?2:pos-1;
                
                if(type)
                {
                    if(pos==1) pos--;
                }
            }
            
            if(rDown())
            {
                sfx(WAV_CHINK);
                pos=(pos+1)%3;
                
                if(type)
                {
                    if(pos==1) pos++;
                }
            }
            
            if(rSbtn()) ++f;
        }
        
        if(f>=0)
        {
            if(++f == 65)
                done=true;
                
            if(!(f&3))
            {
                int c = (f&4) ? QMisc.colors.msgtext : QMisc.colors.caption;
                
                switch(pos)
                {
                case 0:
                    textout_ex(framebuf,zfont,"CONTINUE",88,72,c,-1);
                    break;
                    
                case 1:
                    textout_ex(framebuf,zfont,"SAVE",88,96,c,-1);
                    break;
                    
                case 2:
                    if(!type)
                        textout_ex(framebuf,zfont,"RETRY",88,120,c,-1);
                    else textout_ex(framebuf,zfont,"RETRY",88,96,c,-1);
                    
                    break;
                }
            }
        }
        
        rectfill(framebuf,72,72,79,127,0);
        puttile8(framebuf,htile,72,pos*(type?12:24)+72,1,0);
        advanceframe(true);
    }
    while(!Quit && !done);
    
    clear_bitmap(framebuf);
    advanceframe(true);
    
    if(done)
    {
        if(pos)
        {
            Quit=qQUIT;
        }
        else
        {
            Quit=qCONT;
        }
        
        if(pos==1&&(!type))
        {
            saves[currgame]=*game;
            load_game_icon(saves+currgame);
            show_saving(screen);
            save_savedgames();
        }
    }
}

void save_game(bool savepoint)
{
    if(savepoint)
    {
        game->set_continue_scrn(homescr);
        game->set_continue_dmap(currdmap);
        lastentrance_dmap = currdmap;
        lastentrance = game->get_continue_scrn();
    }
    
    saves[currgame]=*game;
    load_game_icon(saves+currgame);
    show_saving(screen);
    save_savedgames();
}

bool save_game(bool savepoint, int type)
{
    kill_sfx();
    clear_to_color(screen,BLACK);
    loadfullpal();
    
    int htile = 2;
    bool done=false;
    bool saved=false;
    
    do
    {
        int pos = 0;
        int f=-1;
        bool done2=false;
        clear_bitmap(framebuf);
        
        if(type)
        {
            textout_ex(framebuf,zfont,"SAVE AND QUIT",88,72,QMisc.colors.msgtext,-1);
        }
        else
        {
            textout_ex(framebuf,zfont,"SAVE",88,72,QMisc.colors.msgtext,-1);
        }
        
        textout_ex(framebuf,zfont,"DON'T SAVE",88,96,QMisc.colors.msgtext,-1);
        textout_ex(framebuf,zfont,"QUIT",88,120,QMisc.colors.msgtext,-1);
        
        do
        {
            load_control_state();
            
            if(f==-1)
            {
                if(rUp())
                {
                    sfx(WAV_CHINK);
                    pos=(pos==0)?2:pos-1;
                }
                
                if(rDown())
                {
                    sfx(WAV_CHINK);
                    pos=(pos+1)%3;
                }
                
                if(rSbtn()) ++f;
            }
            
            if(f>=0)
            {
                if(++f == 65)
                    done2=true;
                    
                if(!(f&3))
                {
                    int c = (f&4) ? QMisc.colors.msgtext : QMisc.colors.caption;
                    
                    switch(pos)
                    {
                    case 0:
                        if(type)
                            textout_ex(framebuf,zfont,"SAVE AND QUIT",88,72,c,-1);
                        else textout_ex(framebuf,zfont,"SAVE",88,72,c,-1);
                        
                        break;
                        
                    case 1:
                        textout_ex(framebuf,zfont,"DON'T SAVE",88,96,c,-1);
                        break;
                        
                    case 2:
                        textout_ex(framebuf,zfont,"QUIT",88,120,c,-1);
                        break;
                    }
                }
            }
            
            rectfill(framebuf,72,72,79,127,0);
            puttile8(framebuf,htile,72,pos*24+72,1,0);
            advanceframe(true);
        }
        while(!Quit && !done2);
        
        //reset_combo_animations();
        clear_bitmap(framebuf);
        
        if(done2)
        {
            if(pos==1||pos==0) done=true;
            
            if(pos==0)
            {
                if(savepoint)
                {
                    game->set_continue_scrn(homescr);
                    game->set_continue_dmap(currdmap);
                    lastentrance_dmap = currdmap;
                    lastentrance = game->get_continue_scrn();
                }
                
                saves[currgame]=*game;
                load_game_icon(saves+currgame);
                show_saving(screen);
                save_savedgames();
                saved=true;
                
                if(type)
                {
                    Quit = qQUIT;
                    done=true;
                    skipcont=1;
                }
            }
            
            if(pos==2)
            {
                clear_bitmap(framebuf);
                textout_ex(framebuf,zfont,"ARE YOU SURE?",88,72,QMisc.colors.msgtext,-1);
                textout_ex(framebuf,zfont,"YES",88,96,QMisc.colors.msgtext,-1);
                textout_ex(framebuf,zfont,"NO",88,120,QMisc.colors.msgtext,-1);
                int pos2=0;
                int g=-1;
                bool done3=false;
                
                do
                {
                    load_control_state();
                    
                    if(g==-1)
                    {
                        if(rUp())
                        {
                            sfx(WAV_CHINK);
                            pos2=(pos2==0)?1:pos2-1;
                        }
                        
                        if(rDown())
                        {
                            sfx(WAV_CHINK);
                            pos2=(pos2+1)%2;
                        }
                        
                        if(rSbtn()) ++g;
                    }
                    
                    if(g>=0)
                    {
                        if(++g == 65)
                            done3=true;
                            
                        if(!(g&3))
                        {
                            int c = (g&4) ? QMisc.colors.msgtext : QMisc.colors.caption;
                            
                            switch(pos2)
                            {
                            case 0:
                                textout_ex(framebuf,zfont,"YES",88,96,c,-1);
                                break;
                                
                            case 1:
                                textout_ex(framebuf,zfont,"NO",88,120,c,-1);
                                break;
                            }
                        }
                    }
                    
                    
                    rectfill(framebuf,72,72,79,127,0);
                    puttile8(framebuf,htile,72,pos2*24+96,1,0);
                    advanceframe(true);
                }
                while(!Quit && !done3);
                
                clear_bitmap(framebuf);
                
                if(pos2==0)
                {
                    Quit = qQUIT;
                    done=true;
                    skipcont=1;
                }
            }
        }
    }
    while(!Quit && !done);
    
    ringcolor(false);
    loadlvlpal(DMaps[currdmap].color);
    
    if(darkroom)
    {
        if(get_bit(quest_rules,qr_FADE))
        {
            interpolatedfade();
        }
        else
        {
            loadfadepal((DMaps[currdmap].color)*pdLEVEL+poFADE3);
        }
    }
    
    return saved;
}
