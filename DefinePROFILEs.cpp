// The functions to define the contents for each operating Profile.
//
// Discman777    08/24/2021
// Version 1.08.25
// This freeware is provided “AS IS”.  Non-commercial use, modification, distribution at your own risk.


#include <arduino.h>
#include <GlobalVars.h>
#include <DefinePAGEs.h>
#include <DefinePROFILEs.h>


// - = - = - = - = - = - = - = -  Function to define Profiles  - = - = - = - = - = - = - = -

// Build up all available Profiles.  Profiles are initialized after reset,
//  and are not designed to be changeable at runtime.
void DefineAllProfiles(void)
{
// Profile 0: FS2020 Generic
    g_Profile[PF_MS_GENERIC].sTitle = "MSFS";           // Name (not used)
    g_Profile[PF_MS_GENERIC].client = SMC_MSFS;         // One of two client types

    g_Profile[PF_MS_GENERIC].scrn[0] = PG_AUTOPILOT;    // List all logical pages. Order is significant
    g_Profile[PF_MS_GENERIC].scrn[1] = PG_SITUATION;    // The first 6 will appear on the Sidebar
    g_Profile[PF_MS_GENERIC].scrn[2] = PG_MS_RADIOS;
    g_Profile[PF_MS_GENERIC].scrn[3] = PG_MS_DEVICES;
    g_Profile[PF_MS_GENERIC].scrn[4] = PG_SIMOPTIONS;
    // It's hightly recommeded to have the MORE page on the bottom spot of the Sidebar [5]
    g_Profile[PF_MS_GENERIC].scrn[5] = PG_MORE;

    g_Profile[PF_MS_GENERIC].scrn[6] = PG_CONFIG;    // These remaining pages will be displaced
    g_Profile[PF_MS_GENERIC].scrn[7] = PG_TESTPAD;     //  inside the MORE page
    g_Profile[PF_MS_GENERIC].scrn[8] = PG_DATAMON;
    g_Profile[PF_MS_GENERIC].numScreens = 9;     // Must match the number of pages above


// Profile 1: X-Plane 11 Generic.
    g_Profile[PF_XP_GENERIC].sTitle = "X-Plane11";
    g_Profile[PF_XP_GENERIC].client = SMC_XPLN;

    g_Profile[PF_XP_GENERIC].scrn[0] = PG_XP_AUTOP;
    g_Profile[PF_XP_GENERIC].scrn[1] = PG_SITUATION;
    g_Profile[PF_XP_GENERIC].scrn[2] = PG_XP_RADIOS;
    g_Profile[PF_XP_GENERIC].scrn[3] = PG_XP_DEVICES;
    g_Profile[PF_XP_GENERIC].scrn[4] = PG_G1000;
    g_Profile[PF_XP_GENERIC].scrn[5] = PG_MORE;

    g_Profile[PF_XP_GENERIC].scrn[6] = PG_CONFIG;
    g_Profile[PF_XP_GENERIC].scrn[7] = PG_SIMOPTIONS;
    g_Profile[PF_XP_GENERIC].scrn[8] = PG_TESTPAD;
    g_Profile[PF_XP_GENERIC].scrn[9] = PG_DATAMON;
    g_Profile[PF_XP_GENERIC].numScreens = 10;     // Must match the number of pages above


// Profile 2: X-Plane 11 Zib Mod.
    g_Profile[PF_ZIBO].sTitle = "Zibo";
    g_Profile[PF_ZIBO].client = SMC_XPLN;

    g_Profile[PF_ZIBO].scrn[0] = PG_ZB_AUTOP;
    g_Profile[PF_ZIBO].scrn[1] = PG_ZB_SITU;
    g_Profile[PF_ZIBO].scrn[2] = PG_XP_RADIOS;
    g_Profile[PF_ZIBO].scrn[3] = PG_ZB_PANEL1;
    g_Profile[PF_ZIBO].scrn[4] = PG_ZB_NAVIG;
    g_Profile[PF_ZIBO].scrn[5] = PG_MORE;

    g_Profile[PF_ZIBO].scrn[6] = PG_CONFIG;
    g_Profile[PF_ZIBO].scrn[7] = PG_SIMOPTIONS;
    g_Profile[PF_ZIBO].scrn[8] = PG_ZB_PANEL2;
    g_Profile[PF_ZIBO].scrn[9] = PG_TESTPAD;
    g_Profile[PF_ZIBO].scrn[10] = PG_DATAMON;
    g_Profile[PF_ZIBO].numScreens = 11;     // Must match the number of pages above


// Profile 2: X-Plane 11 ToLiss Airbus..
    g_Profile[PF_TOLISS].sTitle = "ToLiss";
    g_Profile[PF_TOLISS].client = SMC_XPLN;

    g_Profile[PF_TOLISS].scrn[0] = PG_TL_AUTOP;
    g_Profile[PF_TOLISS].scrn[1] = PG_ZB_SITU;
    g_Profile[PF_TOLISS].scrn[2] = PG_XP_RADIOS;
    g_Profile[PF_TOLISS].scrn[3] = PG_TL_PANEL;
    g_Profile[PF_TOLISS].scrn[4] = PG_SIMOPTIONS;
    g_Profile[PF_TOLISS].scrn[5] = PG_MORE;

    g_Profile[PF_TOLISS].scrn[6] = PG_CONFIG;
    g_Profile[PF_TOLISS].scrn[7] = PG_TESTPAD;
    g_Profile[PF_TOLISS].scrn[8] = PG_DATAMON;
    g_Profile[PF_TOLISS].numScreens = 9;     // Must match the number of pages above
}


// Test for the profile in use.  Use it to skipping non-existing daatrefs/commands.
bool IsGenericProfile(void)
{
    return g_Config.curProf == PF_MS_GENERIC || g_Config.curProf == PF_XP_GENERIC;
}

bool IsZiboProfile(void)
{
    return g_Config.curProf == PF_ZIBO;
}


bool IsToLissProfile(void)
{
    return g_Config.curProf == PF_TOLISS;
}
