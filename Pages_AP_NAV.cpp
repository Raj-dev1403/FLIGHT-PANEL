// Functions to implement the operations for:
//   Autopilot Page
//   Navigate Page
//
// Discman777    08/24/2021
// Version 1.08.25
// This freeware is provided “AS IS”.  Non-commercial use, modification, distribution at your own risk.


#include <arduino.h>
#include <GlobalVars.h>
#include <Supports.h>
#include <DefineNODEs.h>
#include <DefinePAGEs.h>>
#include <Pages_AP_NAV.h>
#include <SimExchange.h>
#include <Pages_DEV_SIT.h>
#include <Page_RADIOs.h>


// Varaibles to hold the command handles from the GPS 530 and 1000's key arrays.
int16_t cmd_1000Softkey[12];
int16_t cmd_1000Keypad[6];
int16_t cmd_530Coarse[6], cmd_530Fine[6];
int16_t cmd_530Outter[6], cmd_530Inner[6];
int16_t cmd_530Keypad[8], cmd_530KeyRange[8];
int16_t cmd_530Softkey[12];
int16_t cmd_ZB_Softkey[7];


// - = - = - = - = - = - = -  Functions for AUTOPILOT Page  - = - = - = - = - = - = -
// Custom functions to support Autopilot features.

// Handler for selecting the four primary AP values as targets in the A/P Page.
void SelectApPrimary(int16_t dummy)
{
    page[PG_AUTOPILOT].SetEncoAttach(0, SNATSPED);    // SPD
    page[PG_AUTOPILOT].SetEncoAttach(1, SNATHDNG);    // HDG
    page[PG_AUTOPILOT].SetEncoAttach(2, SNATALTI);    // ALT
    page[PG_AUTOPILOT].SetEncoAttach(3, SNATVSPD);    // VS

    page[PG_AUTOPILOT].SetPushAttach(0, SNABSPED);    // SPD
    page[PG_AUTOPILOT].SetPushAttach(1, SNABHDNG);    // HDG
    page[PG_AUTOPILOT].SetPushAttach(2, SNABALTI);    // ALT
    page[PG_AUTOPILOT].SetPushAttach(3, SNABVSPD);    // VS

    page[PG_AUTOPILOT].SetLongAttach(0, SNABSPSY);    // SPD
    page[PG_AUTOPILOT].SetLongAttach(1, SNABHDSY);    // HDG
    page[PG_AUTOPILOT].SetLongAttach(2, SNABALSY);    // ALT
    page[PG_AUTOPILOT].SetLongAttach(3, SNABVSSY);    // VS

    page[PG_XP_AUTOP].SetEncoAttach(0, SNATSPED);     // SPD
    page[PG_XP_AUTOP].SetEncoAttach(1, SNATHDNG);     // HDG
    page[PG_XP_AUTOP].SetEncoAttach(2, SNATALTI);     // ALT
    page[PG_XP_AUTOP].SetEncoAttach(3, SNATVSPD);     // VS

    page[PG_XP_AUTOP].SetPushAttach(0, SNABSPED);     // SPD
    page[PG_XP_AUTOP].SetPushAttach(1, SNABHDNG);     // HDG
    page[PG_XP_AUTOP].SetPushAttach(2, SNABALTI);     // ALT
    page[PG_XP_AUTOP].SetPushAttach(3, SNABVSPD);     // VS

    page[PG_XP_AUTOP].SetLongAttach(0, SNABSPSY);    // SPD
    page[PG_XP_AUTOP].SetLongAttach(1, SNABHDSY);    // HDG
    page[PG_XP_AUTOP].SetLongAttach(2, SNABALSY);    // ALT
    page[PG_XP_AUTOP].SetLongAttach(3, SNABVSSY);    // VS

    page[PG_ZB_AUTOP].SetEncoAttach(0, SNZTSPED);     // SPD
    page[PG_ZB_AUTOP].SetEncoAttach(1, SNZTHDNG);     // HDG
    page[PG_ZB_AUTOP].SetEncoAttach(2, SNZTALTI);     // ALT
    page[PG_ZB_AUTOP].SetEncoAttach(3, SNZTVSPD);     // VS

    page[PG_TL_AUTOP].SetEncoAttach(0, SNZTSPED);     // SPD
    page[PG_TL_AUTOP].SetEncoAttach(1, SNZTHDNG);     // HDG
    page[PG_TL_AUTOP].SetEncoAttach(2, SNZTALTI);     // ALT
    page[PG_TL_AUTOP].SetEncoAttach(3, SNZTVSPD);     // VS

    page[PG_TL_AUTOP].SetPushAttach(0, SNABSPED);     // SPD
    page[PG_TL_AUTOP].SetPushAttach(1, SNABHDNG);     // HDG
    page[PG_TL_AUTOP].SetPushAttach(2, SNABALTI);     // ALT
    page[PG_TL_AUTOP].SetPushAttach(3, SNZTVSPD);     // VS

    page[PG_TL_AUTOP].SetLongAttach(0, SNABSPSY);     // SPD
    page[PG_TL_AUTOP].SetLongAttach(1, SNABHDSY);     // HDG
    page[PG_TL_AUTOP].SetLongAttach(2, SNABALSY);     // ALT
    page[PG_TL_AUTOP].SetLongAttach(3, SNABVSSY);     // VS
    HighlightSelection();
    g_bAtPrimaryEnco = true;        // Signal encoder snapback not needed
}


// Handler for selecting the OBS/Course values as targets in the A/P Page.
void SelectApCourse(int16_t dummy)
{
    page[PG_AUTOPILOT].SetEncoAttach(0, SNATCRS1);    // Course 1
    page[PG_AUTOPILOT].SetEncoAttach(1, SNATCRS2);    // Course 2
    page[PG_AUTOPILOT].SetEncoAttach(2, SNATMAXB);    // Max bank
    page[PG_AUTOPILOT].SetEncoAttach(3, SNATABRK);    // Autobrake

    page[PG_XP_AUTOP].SetEncoAttach(0, SNATCRS1);     // Course 1
    page[PG_XP_AUTOP].SetEncoAttach(1, SNATCRS2);     // Course 2
    page[PG_XP_AUTOP].SetEncoAttach(2, SNATMAXB);     // Max bank
    page[PG_XP_AUTOP].SetEncoAttach(3, SNATABRK);     // Autobrake

    page[PG_ZB_AUTOP].SetEncoAttach(0, SNZTCRS1);     // Course 1
    page[PG_ZB_AUTOP].SetEncoAttach(1, SNZTCRS2);     // Course 2
    page[PG_ZB_AUTOP].SetEncoAttach(2, SNZTMAXB);     // Max bank
    page[PG_ZB_AUTOP].SetEncoAttach(3, SNZTABRK);     // Autobrake

    page[PG_TL_AUTOP].SetEncoAttach(3, SNZTABRK);     // Autobrake
    HighlightSelection();
    g_bAtPrimaryEnco = false;        // Signal encoder snapback pending
}


// Handler for selecting the EFIS controls as targets in the ToLiss A/P Page.
void SelectEFIS_TL(int16_t dummy)
{
    page[PG_TL_AUTOP].SetEncoAttach(0, SNZKMAPM);     // Mode
    page[PG_TL_AUTOP].SetEncoAttach(1, SNZKRANG);     // Range
    page[PG_TL_AUTOP].SetEncoAttach(2, SNZTBARO);     // Baro
    page[PG_TL_AUTOP].SetEncoAttach(3, INVLD_NODEINX);

    page[PG_TL_AUTOP].SetPushAttach(2, SNZTBARO);     // STD

    page[PG_TL_AUTOP].SetLongAttach(2, SNTBBAPL);     // STD

    HighlightSelection();
    g_bAtPrimaryEnco = false;        // Signal encoder snapback pending

}


// Handler for selecting the four items on the Zibo ND Control Page.
void SelectAllZB_ND(int16_t dummy)
{
    page[PG_ZB_NAVIG].SetEncoAttach(0, SNZKMRST);     // Minimums
    page[PG_ZB_NAVIG].SetEncoAttach(1, SNZKMAPM);     // Map mode
    page[PG_ZB_NAVIG].SetEncoAttach(2, SNZKRANG);     // Range
    page[PG_ZB_NAVIG].SetEncoAttach(3, SNZTBARO);     // Baro

    page[PG_ZB_NAVIG].SetPushAttach(0, SNZKMRST);     // RST
    page[PG_ZB_NAVIG].SetPushAttach(1, SNZKMCTR);     // CTR
    page[PG_ZB_NAVIG].SetPushAttach(2, SNZKRTFC);     // TFC
    page[PG_ZB_NAVIG].SetPushAttach(3, SNZKBSTD);     // STD
    HighlightSelection();
}


// Show button on/off for the HDG button.
String XP_ShowHeadingBtn(int16_t scnInx)
{
    // Gather the current status first
    uint16_t sel = scnNode[scnInx].GetIntValue();
    uint16_t picId = scnNode[scnInx].GetMsToSimId(1);    // Base pic ID
    return String((sel == 1) ? picId + 1 : picId);
}


// Set Heading Sync for FS2020, this event requires AP's Heading mode
//  to be Off, so need to turn off Heading mode first.
void MS_SetHeadingSync(int16_t scnInx)
{
    NodeToSimCommon(scnInx, 1, 0);    // Turn off Heading mode
    NodeToSimCommon(scnInx, 2);       // Toggle heading hold
}


// Set the autopilot's VS entry using the real time vertical speed.
void MS_SyncVertSpeed(int16_t scnInx)
{
    // Fetch from the node holding the current VS and set the AP's
    NodeToSimCommon(scnInx, 0, scnNode[SNSTVSPD].GetIntValue());
}


// Show button on/off for either 0, or 1 for any non-0 state.
String XP_Show01Button(int16_t scnInx)
{
    // Gather the current status first
    uint16_t sel = scnNode[scnInx].GetIntValue();
    uint16_t picId = scnNode[scnInx].GetMsToSimId(1);    // Base pic ID
    return String((sel > 0) ? picId + 1 : picId);
}


// Show autopilot's max banking in degrees.
String ShowMaxBank_ZB(int16_t scnInx)
{
    // Gather the current setting first
    uint16_t bank = scnNode[scnInx].GetIntValue();
    // Translate enum (0-6) to max degree banking.  5 degress/increment
    if (scnInx == SNZTMAXB)          // Take care of Zibo's bank enum (0-4)
    {
        bank += 2;
    }
    return String(bank * 5);
}


// Show Zibo autobrake setting by names.
String ShowAutobrake_ZB(int16_t scnInx)
{
    const String sAutobrake[6] = {"RTO", "OFF", "1", "2", "3", "MAX" };
    // Gather the current setting
    uint16_t autobrake = min(scnNode[scnInx].GetIntValue(), 5);
    return sAutobrake[autobrake];
}


// Show ToLiss autobrake setting by names.
String ShowAutobrake_TL(int16_t scnInx)
{
    const String sAutobrake[4] = { "OFF", "LO", "MED", "MAX" };
    // Gather the current setting
    uint16_t autobrake = min(scnNode[scnInx].GetIntValue(), 3);
    return sAutobrake[autobrake];
}


// Show the blinking status for the wanring light.  Tristate indicator: Off, amber, red.
// Make sure the node indexes are consecutively ordered for these screen nodes.
String ShowApWarnLights_ZB(int16_t scnInx)
{
    // Preapre the node indexes to point to the red and amber object
    int16_t discInx = SNZBRAP1;     // Disconnect:Red
    int16_t warnInx = SNZBRAP0;     // Warning:Amber
    if (scnInx == SNZBRAT1 || scnInx == SNZBRAT0)
    {
        discInx = SNZBRAT1;       // Disconnect:Red
        warnInx = SNZBRAT1;       // Warning:Amber
    }
    uint16_t picId = scnNode[discInx].GetMsToSimId(1);    // Light-off pic ID
    if (scnNode[discInx].GetIntValue())
    {
        picId += 2;        // Red
    }
    else if (scnNode[warnInx].GetIntValue())
    {
        picId++;           // Amber
    }
    return String(picId);
}


// Show the ND's range setting by numeric text.
String ShowNdRange_ZB(int16_t scnInx)
{
    const String sRange[8] = { "5", "10", "20", "40", "80", "160", "320", "640" };
    // Gather the current setting
    uint16_t range = min(scnNode[scnInx].GetIntValue(), 7);
    return sRange[range];
}


// Show the map mode setting by names.
String ShowMapMode_ZB(int16_t scnInx)
{
    const String sMode[4] = { "APP", "VOR", "MAP", "PLN"};
    // Gather the current setting
    uint16_t mode = min(scnNode[scnInx].GetIntValue(), 3);
    return sMode[mode];
}


// Show the map range setting by numeric text.
String ShowMapRange_TL(int16_t scnInx)
{
    const String sRange[6] = { "10", "20", "40", "80", "160", "320" };
    // Gather the current setting
    uint16_t range = min(scnNode[scnInx].GetIntValue(), 5);
    return sRange[range];
}


// Show the EFIS display mode setting by names.
String ShowEfisMode_ZB(int16_t scnInx)
{
    const String sMode[5] = { "LS", "VOR", "NAV", "ARC", "PLAN" };
    // Gather the current setting
    uint16_t mode = min(scnNode[scnInx].GetIntValue(), 4);
    return sMode[mode];
}


// Respond to the GPS/NAV1/NAV2 button, send the appropriate event to the sim.
void MS_CycleGpsNav12Sw(int16_t scnInx)
{
    // Presume button image for GPS
    uint16_t imgOfst = 0;

    if (scnNode[SNABNGPS].GetIntValue())
    {
        NodeToSimCommon(scnInx, 0);           // Get out of GPS
        NodeToSimCommon(scnInx, 2, 1);        // On to NAV1
        imgOfst = 1;
    }
    else         // Currently on a NAV
    {
        if (scnNode[SNABNV12].GetIntValue() == 1)
        {
            NodeToSimCommon(scnInx, 2, 2);    // Switch from NAV 1 to 2
            imgOfst = 2;
        }
        else     // Curently on NAV2
        {
            NodeToSimCommon(scnInx, 0);       // Switch to GPS
        }
    }
    // Save the button image offset preapred for upcoming display
    scnNode[scnInx].SetValue(imgOfst);
}


// Respond to the GPS/NAV1/NAV2 button, send the appropriate event to the sim.
void XP_CycleGpsNav12Sw(int16_t scnInx)
{
    // Gather the current status first, increment to the next setting
    uint16_t sel = scnNode[scnInx].GetIntValue();
    sel++;
    sel %= 3;       // Nav1/Nav2/GPS
    scnNode[scnInx].SetValue(sel);
}


// Determine the image for the button based on current GPS/NAV setting.
String MS_ShowGpsNav12Btn(int16_t scnInx)
{
    // Presume button image for GPS
    uint16_t imgOfst = 0;

    // Examine the current status
    if (scnNode[SNABNGPS].GetIntValue() == 0)
    {
        // Not GPS, check for one of the two NAVs
        imgOfst = scnNode[SNABNV12].GetIntValue();
    }
    // Save the button image offset preapred for upcoming display
    scnNode[SNABNVSR].SetValue(imgOfst);

    return String(imgOfst);
}


// Determine the image for the button based on current GPS/NAV setting.
String XP_ShowGpsNav12Btn(int16_t scnInx)
{
    // Gather the current status first
    uint16_t sel = scnNode[scnInx].GetIntValue();
    uint16_t picId = scnNode[scnInx].GetMsToSimId(1);    // Base pic ID

    if (sel < 2)    // Modify pic ID as necessary
    {
        picId += sel + 1;    // Index for Nav 1 or 2
    }
    return String(picId);
}


// Function for display those button state limited to On or Off.
String ShowTruncatedBinaryBtn(int16_t scnInx)
{
    // GFetch the current status and the picture ID
    uint16_t val = scnNode[scnInx].GetIntValue();
    uint16_t picId = scnNode[scnInx].GetMsToSimId(1);    // Base pic ID
    picId += (val >= 1) ? 1 : 0;
    return String(picId);
}


// Zibo/B738 MCP IAS readout requires special display logics
String ShowAutoSpeed_ZB(int16_t scnInx)
{
    float speed = scnNode[scnInx].GetFloatValue();
    char sSpeed[6];
    if (speed > 2. || speed <= 0.)
    {
        sprintf(sSpeed, "%3d", (uint16_t) (speed + 0.5));
    }
    else         // Output as Mach number
    {
        sprintf(sSpeed, "%4.2f", speed);
    }

    return String(sSpeed);
}


// Show an on/off state of the A/THR indicator.
String ShowAutoThrMode_TL(int16_t scnInx)
{
    uint16_t autoThr = scnNode[scnInx].GetIntValue();
    uint16_t picId = scnNode[scnInx].GetMsToSimId(1);

    if (scnInx == SNZBLVLC)    // Is this the EXPED node?
    {
        if (autoThr == 2)      // EXPED On if mode 2
        {
            picId++;
        }
    }
    else if (autoThr != 0)     // For the A/THR light, anything non-zero
    {
        picId++;
    }

    return (String(picId));
}


// Show the lights on the GPWS tristate button.
// (Simplified into Off, Pull and Both)
String ShowGpwsLight_TL(int16_t scnInx)
{
    uint16_t gpwsState = scnNode[scnInx].GetIntValue();
    int16_t picId = scnNode[scnInx].GetMsToSimId(1);
    
    switch (gpwsState)
    {
        case 1:  case 2:
            picId++;
            break;
        case 3:
            picId += 2;
            break;
        case 4:  case 5:
            picId += 3;
    }
    
    return (String(picId));
}



// Custom functions to support Navigation features.

// Show the direction arrow icon to depict the vector to waypoint.
// Called from the Situation's radial display function.
void PlotBearingDir(int16_t scnInx)
{
    // Fetch the current data and calculate the relative bearing
    float bearing = scnNode[scnInx].GetFloatValue();     // Degrees
    float heading = scnNode[SNSTACDR].GetFloatValue();   // Degrees
    float relativeAngle = bearing - heading;

    // Go figure out the image to display
    PlotDirectionArrow(scnInx, relativeAngle);
}


// Determine the directional arrow image for the given heading.
// 0-degree for an arrow pointing straight down. 90 for pointing from 3 o'clock.
void PlotDirectionArrow(int16_t scnInx, float direction)
{
    if (direction < 0)
    {
        direction += 360;
    }

    // Select (index) an image to depict the relative wind direction
    // Each image covers 18 degrees.  Map into one of 20 images (per 360)
    // Add an offset of 10 images to reverse the direction of the arrowhead
    uint16_t picOfst = direction / 18. + 0.5;

    // Push the image to the display
    uint16_t picId = scnNode[scnInx].GetMsToSimId(1);    // Base image ID
    String sPicTag = scnNode[scnInx].GetTagToNex();
    picId += picOfst % 20 + 1;
    nexIo.SendAction(sPicTag, String(picId));
}


// Blank out the direction arrow when data input id invalid.
void BlankDirectionArrow(void)
{
    // Blank out the bearing indicator
    uint16_t picId = scnNode[SNSPRBER].GetMsToSimId(1);
    String sPicTag = scnNode[SNSPRBER].GetTagToNex();
    nexIo.SendAction(sPicTag, String(picId));
}
