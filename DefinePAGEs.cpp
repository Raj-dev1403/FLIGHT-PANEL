// The key function to define the contents for every screen node on each control page.
//
// Discman777    08/24/2021
// Version 1.08.25
// This freeware is provided “AS IS”.  Non-commercial use, modification, distribution at your own risk.


#include <arduino.h>
#include <GlobalVars.h>
#include <Supports.h>
#include <DefineNODEs.h>
#include <DefinePAGEs.h>
#include <Pages_AP_NAV.h>
#include <Page_RADIOS.h>
#include <Pages_DEV_SIT.h>
#include <Pages_MORE.h>
#include <Page_CONFIG.h>
#include <XPLDirect.h>



// Define the contents and attributes for every available pages.  That includes all pages
//  from every profiles.
void DefineAllPages(void)
{

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
// PAGE: AUTOPILOT.
// Autopilot controls.
    // For each page, give it a title to show up on screen
    page[PG_AUTOPILOT].SetTitle("AUTOPILOT");
    // The page name is the Nextion page ID to tell the Nextion to display that page.
    page[PG_AUTOPILOT].SetNexPageName("AUTOPILOT");
    // Assign the function to be called at every page entry
    page[PG_AUTOPILOT].InitSelectFunc(&SelectApPrimary);
    // Let the page know that it owns these nodes (an inclusive range)
    page[PG_AUTOPILOT].AddNodes(SNABAUTO, SNATABRK);

    // Clone the base for XP
    page[PG_XP_AUTOP].CloneFrom(PG_AUTOPILOT);
    page[PG_XP_AUTOP].AddNodes(SNABVNAV, SNABFLCH);

// Autopilot controls for Zibo.

    page[PG_ZB_AUTOP].SetTitle("AUTOPILOT");
    page[PG_ZB_AUTOP].SetNexPageName("ZB_AUTOP");
    page[PG_ZB_AUTOP].InitSelectFunc(&SelectApPrimary);
    page[PG_ZB_AUTOP].AddNodes(SNZTSPED, SNZTMAXB);
    page[PG_ZB_AUTOP].AddNode(SNZBMCAU);

// Autopilot controls for ToLiss.

    page[PG_TL_AUTOP].SetTitle("AUTOPILOT");
    page[PG_TL_AUTOP].SetNexPageName("TL_AUTOP");
    page[PG_TL_AUTOP].InitSelectFunc(&SelectApPrimary);
    page[PG_TL_AUTOP].AddNodes(SNZTSPED, SNZTABRK);
    page[PG_TL_AUTOP].AddNodes(SNTMSELH, SNTBTERR);
    page[PG_TL_AUTOP].AddNodes(SNABSPSY, SNABVSSY);
    page[PG_TL_AUTOP].AddNodes(SNZKMAPM, SNZKRANG);
    page[PG_TL_AUTOP].AddNode(SNZTBARO);
    page[PG_TL_AUTOP].AddNode(SNZKBAIN);


// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
// PAGE: NAVIGATE
// ND controls for Zibo.

    page[PG_ZB_NAVIG].SetTitle("NAVIGATE");
    page[PG_ZB_NAVIG].SetNexPageName("ZB_ND");
    page[PG_ZB_NAVIG].InitSelectFunc(&SelectAllZB_ND);
    page[PG_ZB_NAVIG].AddNodes(SNZKMIRA, SNZBXMAP);
    page[PG_ZB_NAVIG].AddNode(SNZTBARO);


    // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
// PAGE: MS RADIOS.
// Communication frequency, transponder setting.

    // First define the base range common to both sims
    page[PG_MS_RADIOS].SetTitle("RADIOS");
    page[PG_MS_RADIOS].InitEntryFunc(&RefreshStdbySelection);
    page[PG_MS_RADIOS].InitSelectFunc(&SelectAllRadios);
    page[PG_MS_RADIOS].SetTextColor(RGB16(31, 32, 8));   // 64520
    page[PG_MS_RADIOS].AddNodes(SNCTSTBY, SNCTADF1);
    // Clone the base for XP
    page[PG_XP_RADIOS].CloneFrom(PG_MS_RADIOS);

    // XP RADIOS page only
    page[PG_XP_RADIOS].SetNexPageName("XP_RADIOS");
    page[PG_XP_RADIOS].AddNodes(SNCTXPMO, SNCBNV2X);       // XP-only
     
    // MS RADIOS page only
    page[PG_MS_RADIOS].SetNexPageName("MS_RADIOS");
    page[PG_MS_RADIOS].AddNodes(SNCBNV1M, SNCBNV2M);       // MSFS-only

    // Perform any other one-time common initalization for the Radios page
    InitRadiosPage();


// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
// PAGE: G1000.
    // GPS G1000 controls.

    page[PG_G1000].SetTitle("GPS");
    page[PG_G1000].SetNexPageName("GPS1000");
    page[PG_G1000].InitEntryFunc(&XP_GpsPageEntry);
    page[PG_G1000].InitSelectFunc(&SelectAllGPS);
    page[PG_G1000].AddNodes(SNG1BPNS, SNG1BKP0);
    page[PG_G1000].AddNodes(SNGB1SEL, SNGB5SEL);
    page[PG_G1000].AddNode(SNZBXMAP);


// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
// PAGE: G530.
    // GPS G530 controls.

    page[PG_G530].SetTitle("GPS 530");
    page[PG_G530].SetNexPageName("GPS530");
    page[PG_G530].InitEntryFunc(&SetupGpsRotaryCmds);
    page[PG_G530].InitSelectFunc(&SelectAllGPS);
    page[PG_G530].AddNodes(SNGB1SEL, SNG5BRG0);
    page[PG_G530].AddNode(SNZBXMAP);


// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
// PAGE: DEVICES.
// Aircraft system controls.

    // First define the base range common to both sims
    // X-Plane version
    page[PG_MS_DEVICES].SetTitle("DEVICES");
    page[PG_MS_DEVICES].SetNexPageName("DEVICES");
    page[PG_MS_DEVICES].InitSelectFunc(&SelectDevicesPrimary);
    page[PG_MS_DEVICES].AddNodes(SNDBANTI, SNDBSTR2);
    // Clone the base for XP
    page[PG_XP_DEVICES].CloneFrom(PG_MS_DEVICES);

    // XP DEVICES page only
    page[PG_XP_DEVICES].AddNodes(SNDBST1X, SNDBST2X);

    // MS DEVICES page only
    page[PG_MS_DEVICES].AddNodes(SNDBIGN1, SNDBIGN2);


// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
// PAGE: ZIBO PANEL1.
// B738 ZIBO Lights and Anti-Ice controls.

    page[PG_ZB_PANEL1].SetTitle("PANEL");
    page[PG_ZB_PANEL1].SetNexPageName("ZB_PANEL1");
    // Assign the function to be called at every page entry
    page[PG_ZB_PANEL1].InitSelectFunc(&SelectPanelLights);
    page[PG_ZB_PANEL1].AddNodes(SNZBMCAU, SNZBTAXI);

// B738 ZIBO AUX PANEL.

    page[PG_ZB_PANEL2].SetTitle("AUX PANEL");
    page[PG_ZB_PANEL2].SetNexPageName("ZB_PANEL2");
    page[PG_ZB_PANEL2].AddNodes(SNZBWHLS, SNZBWHRS);

// ToLiss Lights and Anti-Ice controls.

    page[PG_TL_PANEL].SetTitle("PANEL");
    page[PG_TL_PANEL].SetNexPageName("TL_PANEL");
    // Assign the function to be called at every page entry
    page[PG_TL_PANEL].InitSelectFunc(&SelectPanelLights);
    // Reuse from the Zibo panel, although not every one will be used
    page[PG_TL_PANEL].AddNodes(SNZBMCAU, SNZBTAXI);


    // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
// PAGE: SITUATION.
// Situation awareness display.

    page[PG_SITUATION].SetTitle("SITUATION");
    page[PG_SITUATION].SetNexPageName("SITUATION");    
    page[PG_SITUATION].SetTextColor(CLR_TEXT_STANDARD);
    page[PG_SITUATION].InitEntryFunc(&EntrySetup_Situation);
    page[PG_SITUATION].InitSelectFunc(&SelectAllSituation);
    page[PG_SITUATION].AddNodes(SNSTWVEL, SNSTRAD2);

    // Clone the base for Zibo
    page[PG_ZB_SITU].CloneFrom(PG_SITUATION);

    // Zibo SITUATION page only
    page[PG_ZB_SITU].AddNode(SNZTBARO);


// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
// PAGE: MORE.
// Contain additional copntrol and system diagnostic pages.

    page[PG_MORE].SetTitle("MORE...");
    page[PG_MORE].SetNexPageName("MORE");
    page[PG_MORE].InitEntryFunc(&PopulateMOREPage);
    page[PG_MORE].AddNodes(SNMORB01, SNMORB12);

    
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
// PAGE: SIM OPTIONS.
// Simulator options.

    page[PG_SIMOPTIONS].SetTitle("SIM OPTIONS");
    page[PG_SIMOPTIONS].SetNexPageName("SIMOPTIONS");
    page[PG_SIMOPTIONS].InitSelectFunc(&SelectAllSimOptions);
    page[PG_SIMOPTIONS].AddNodes(SNOTHOUR, SNOTVOLM);


// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
// PAGE: CONFIG.
// T.M.P. system configuration.

    page[PG_CONFIG].SetTitle("CONFIGURATION");
    page[PG_CONFIG].SetNexPageName("CONFIG");
    page[PG_CONFIG].InitEntryFunc(&EntrySetup_Config);
    page[PG_CONFIG].InitSelectFunc(&SelectAllConfig);
    page[PG_CONFIG].AddNodes(SNFTBRIG, SNFTICAO);


// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
// PAGE: FS2020 Event-Out Test Pad (this page contains no node).

    page[PG_TESTPAD].SetTitle("Event Sender");
    page[PG_TESTPAD].SetNexPageName("TESTPAD");
    page[PG_TESTPAD].SetType(PTY_MODAL);


// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
// PAGE: FS2020 Incoming Data Monitor (this page contains no node).

    page[PG_DATAMON].SetTitle("Data Monitor");
    page[PG_DATAMON].SetNexPageName("DATAMON");
    page[PG_DATAMON].SetType(PTY_MODAL);


// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

}
