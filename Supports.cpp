// Functions for providing general operational supports.
//
// Discman777    08/24/2021
// Version 1.08.25
// This freeware is provided “AS IS”.  Non-commercial use, modification, distribution at your own risk.


#include <arduino.h>
#include <GlobalVars.h>
#include <Supports.h>
#include <DefineNODEs.h>
#include <DefinePAGEs.h>
#include <DefinePROFILEs.h>
#include <SimExchange.h>
#include <Paging.h>
#include <Pages_AP_NAV.h>


extern void EncoderTask(void);

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = 
// = = = = = = = = = = = = = = = =  TIMER  INTERRUPT  = = = = = = = = = = = = = = = = =
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = 

uint32_t snapTimerPage = millis();     // Start time for rubber band countdown
uint32_t snapTimerEnco = millis();     // Start time for encoder snap countdown


// This is the handler for the once a second timer interrupt.  Timer services
//  include display dimming and rubber band page change.
void TimerServices(void)
{
    // Is current page not the rubberbanded page?  Rubber band countdownn if so
    if (g_RubberbandIndex != -1 &&
        millis() > snapTimerPage + 8000)     // Check for 8 secs (in mS)
    {
        uint16_t bandedPage = scnNode[g_RubberbandIndex].GetMsToSimId(1);
        if (g_CurPage != bandedPage) 
        {
            OpenScreenPage(bandedPage);    // Snap to rubber banded page
        }
    }

    // Encoder auto-snapback countdownn.  Check for 7 secs (in mS).
    if (!g_bAtPrimaryEnco && millis() > snapTimerEnco + 7000)
    {
        page[g_CurPage].ExecuteRotarySelectFunc();
    }

    // Continue monitoring for display dimming countdown
    ScreenSaver(SCS_CONT);
}


// Initialize all necessary software components.
void SoftwareInit(void)
{
    nexIo.SendAction("page SPLASH");         // Show greetings
    delay(25);                               // BLOCK to allow page change to settle
    nexIo.SendAction("Tin.txt=\"Initializing...\"");    // Indicate on screen

    // Clear XP-Direct's existing list of datarefs and command handles
    XPDplugin.sendResetRequest();
    delay(300);                   // BLOCK all activities to let plug-in settle

    DefineAllProfiles();          // Initialize the profile structures
    DefineAllPages();             // The logical screen pages
    DefineAllNodes();             // Set up all the screen nodes
    
    xpdrIo.AttachXpExtras();      // Commands and Datarefs for X-Plane interface

    if (g_Stat.sErrorMsg != "")   // Report any startup error on the splash screen
    {
        nexIo.SendAction("Tin.txt", g_Stat.sErrorMsg);
        delay(15000);
    }

    // Display initialization status
    nexIo.SendAction("Tin.txt=\"Initialized.\"");    // Update screen message
    nexIo.SendAction("Ppf.pic", String(g_Config.curProf + IMG_PROFILE));
    // For debugging: Increase this delay to view the status
    delay(800);                 // BLOCK all activities briefly to show status
}


// Initialize the MCU, prime interrupts.
void HardwareInit(void)
{
    // Prepare rotary encoders to be interrupt-driven
    // Currntly DISABLED
    for (int16_t enInx = 0; enInx < NUM_ENCODERS; enInx++)
    {
        encoders[enInx].InterruptAttach(EncoderTask);
    }

    // Prepare a one-second interrupt timer
    timerSec = timerBegin(0, 80, true);
    timerAttachInterrupt(timerSec, &TimerServices, true);
    timerAlarmWrite(timerSec, 1000000, true);
    timerAlarmEnable(timerSec);

    ScreenSaver(SCS_CONT);       // Ready to arm auto-dim timer
}


// Given a screen node index, go perform the necessary task, i.e. send 
//  command/dtat to the sim, or paging etc.  The task is embedded in the node.
// Called by the Screen Task from the main loop.
void ExecActionForNode(int16_t scnInx)
{
    if (scnInx != INVLD_SCRNOBJ)
    {
        // Check for type Touch Function--Necessary to call the custom handler
        if (scnNode[scnInx].UseTouchFunction())
        {
            // This node executes a custom function when touched
            scnNode[scnInx].ExecuteTouchFunc(scnInx);
        }
        // Check for a Paging item type
        else if (scnNode[scnInx].IsPaging())    // Open the designated page/screen
        {
            OpenScreenPage(scnNode[scnInx].GetMsToSimId(1));
        }
        else     // Otherwise simply send out the primary op ID (held at index 0)
        {
            NodeToSimCommon(scnInx, 0);
        }
    }
}


// Get the value from the given node and write it to the appropriate Nextion object.
// Called by the Disp Task from the main loop.
// Return display-ready string value to be sent to Nextion.
String FormatNodeForDisplay(int16_t scnInx)
{
    String sNexTag = scnNode[scnInx].GetTagToNex();
    // First check if there's anything to work for
    if (sNexTag == "")
    {
        return "";
    }

    String sValue;
    // Update the value onto screen. See if it's necessary to format the numeric
    if (sNexTag.endsWith(".txt"))
    {
        String sFmtCtrl = scnNode[scnInx].GetFmtCtrl();
        if (sFmtCtrl != "")
        {
            char sBuf[20];
            if (sFmtCtrl.endsWith("d") || sFmtCtrl.endsWith("X"))
            {
                int32_t iValue = scnNode[scnInx].GetIntValue();
                if (scnNode[scnInx].IsValueFloat())   // Rounding if from a float
                {
                    float fValue = scnNode[scnInx].GetFloatValue();
                    iValue = fValue + ((fValue >= 0) ? 0.5 : -0.5);
                }
                sprintf(sBuf, sFmtCtrl.c_str(), iValue);
            }
            else if (sFmtCtrl.endsWith("f"))
            {
                sprintf(sBuf, sFmtCtrl.c_str(), scnNode[scnInx].GetFloatValue());
            }
            else
            {
                sprintf(sBuf, sFmtCtrl.c_str(), scnNode[scnInx].GetChrValue());
            }

            sValue = sBuf;
        }
    }
    else if (sNexTag.endsWith(".pic"))    // Determine image index
    {
        // Deal with on/off state of a image/button, based on an index
        // Yes, it is necessary to arrange Nextion's pictures in accordance, and by
        //  preparing the msg sent from Nextion, programmed from the Nextion Editor
        uint16_t iValue = scnNode[scnInx].GetIntValue();
        iValue = scnNode[scnInx].GetMsToSimId(1) + iValue;
        sValue = String(iValue);
    }

    return sValue;
}


// Scan each nodes on current screen and assign text color based on whether they are selected,
//  i.e. attached to an encoder.
void HighlightSelection(void)
{
    int16_t nodeIx = 0;

    // Cycle through all screen nodes to apply each a color for displaying
    //  the value to indicate its selected or unselected state.
    while (nodeIx < page[g_CurPage].GetNumNodes())
    {
        int16_t scnInx = page[g_CurPage].GetInxToNode(nodeIx);

        String sTxtId = scnNode[scnInx].GetTagToNex();
        if (scnNode[scnInx].IsSelectable() && sTxtId.endsWith(".txt"))
        {
            sTxtId.replace(".txt", ".pco");     // Address text's color attribute

            uint16_t color = page[g_CurPage].GetTextColor();
            // Check to see if this screen node has been selected?
            for (uint16_t selEnc = 0; selEnc < NUM_ENCODERS; selEnc++)
            {
                if (page[g_CurPage].GetEncAttach(selEnc) == scnInx)
                {
                    color = CLR_SELECT_STANDARD;
                    break;
                }
            }

            // Set the text color to reflect selected state

            nexIo.SendAction(sTxtId, String(color));
        }

        // Search next node
        nodeIx++;
    }
}


// Function to toggle the on/off state of a button, for the given screen operation index.
void ToggleBinarySwitch(int16_t scnInx)
{
    // Check and flip the state of a for the screen object
    uint16_t flipVal = (scnNode[scnInx].GetIntValue() == 0) ? 1 : 0;
    NodeToSimCommon(scnInx, 0, flipVal);
}


// Function to step through a tristate button, for the given screen operation index.
void StepTristateSwitch(int16_t scnInx)
{
    // Check and flip the state of a for the screen object
    uint16_t newVal = (scnNode[scnInx].GetIntValue() + 1) % 3;
    NodeToSimCommon(scnInx, 0, newVal);
}


// Send the appropriate command from a key array to XP-Direct indexed by the key touched.
void XP_SendOpcodeFromArray(int16_t scnInx)
{
    uint32_t touchIndex = nexIo.GetByte(INX_OP_ID2);   // Retrieve touched key index

    if (touchIndex < scnNode[scnInx].GetInxFromSim())
    // Resolve the command handle via the index and send command to the sim
    {
        // The "int value" in this case is a pointer to an array storing command handles
        // Then add in the index from the kye touched to the touched command handle
        
        // If this softkey group serves two pages, offset the index for second page
        if (scnNode[scnInx].GetIdFromSim() == 1 && g_Stat.curGpsCtrl == 1)
        {
            touchIndex += scnNode[scnInx].GetInxFromSim();
        }
        int16_t* pCmdHandle = (int16_t*) scnNode[scnInx].GetIntValue() + touchIndex;

        // The node (for the key array) now receives the handle during runtime
        scnNode[scnInx].AttachCommand(*pCmdHandle, 0);
        NodeToSimCommon(scnInx, 0);
    }
}


// Some screen nodes are shared in multiple screen items or pages.  When one client
//  consumes and marks the node as used, the other client(s) would not see the
//  updated value.
// This function is called from the main loop once every 250mS to force a refresh.
void PeriodicRevival(void)
{
    static uint16_t housekeepingTimer = 1;
    static uint16_t flashColor = CLR_BGND_STANDARD;

    // List of items requiring periodic updates...
    // Renewal for the Situation page's screen items
    if (g_CurPage == PG_SITUATION || g_CurPage == PG_ZB_SITU)
    {
        scnNode[SNSTBEAR].InvalidateValue();
        scnNode[SNSTRAD1].InvalidateValue();
        scnNode[SNSTRAD2].InvalidateValue();
        scnNode[SNSTACDR].InvalidateValue();

        // Update display labels on the page
        // Nav selected" GPS / VOR #1 or #2
        const String sNameLookup[3] = { "GPS WP", "DME 1", "DME 2" };
        String sNexTag = scnNode[SNSTDMES].GetTagToNex();
        nexIo.SendAction(sNexTag, sNameLookup[g_Stat.curNavDisplay]);

        // Navigation bearing vs. Mag Compass
        nexIo.SendAction("Tnv.txt", g_Stat.bNavCompDisp ? "N" : " ");
        nexIo.SendAction("Tcp.txt", g_Stat.bNavCompDisp ? " " : "C");
        // Go plot the direction arrow
        if (!g_Stat.bNavCompDisp)
        {
            float heading = scnNode[SNSTACDR].GetFloatValue();   // Degrees
            PlotDirectionArrow(SNSPRBER, heading);
        }

        // Terrain vs. GPS waypoint elevation
        nexIo.SendAction("Ttg.txt", g_Stat.bWpElevation ? "WP" : "TER");

        // Radial TO/ FROM
        nexIo.SendAction("Trh.txt", g_Stat.bToRadialDisp ? "TO" : "FR");

        // Touchdown vs. Vert Speed
        String sLabel = g_Stat.bTouchdownDisp ? "T.DOWN" : "VS";
        nexIo.SendAction("Tvs.txt", sLabel);

        // GS vs. IAS
        nexIo.SendAction("Tsl.txt", g_Stat.bCurGsDisp ? "GS" : "IAS");

        // Update status indicators on this page
        if (IsFS2020())
        {
            // Warning flash for certain screen items
            flashColor = flashColor == CLR_BGND_STANDARD ? CLR_BGND_HIGHLITE :
                                                           CLR_BGND_STANDARD;
            uint16_t color;

            // Overspeed alert--via background color change
            color = scnNode[SNSTOVSP].GetIntValue() ? flashColor : CLR_BGND_STANDARD;
            sNexTag = scnNode[SNSTOVSP].GetTagToNex();
            nexIo.SendAction(sNexTag, String(color));
        }

        // Auto-pilot status indicator
        sNexTag = "Tap.txt";
        bool bApOn = scnNode[SNABAUTO].GetIntValue();
        if (!IsGenericProfile())
        {
            bApOn = scnNode[SNZBCMDA].GetIntValue() ||
                    scnNode[SNZBCMDB].GetIntValue();
        }
        nexIo.SendAction(sNexTag, bApOn ? "AP" : "");

        // Auto-throttle status indicator
        sNexTag = "Tat.txt";
        int16_t scnInx = IsGenericProfile() ? SNABSPED : SNZBATAR;
        nexIo.SendAction(sNexTag, scnNode[scnInx].GetIntValue() ? "A" : "");

        // Count down the housekeeping timer, run code once every 5 secs (20x250mS)
        if (--housekeepingTimer == 0)
        {
            PeriodicHousekeep();
            housekeepingTimer = 20;
        }
    }

    g_NextRevivalTime = millis();       // Timestamp time of recent run
}


// Some screen items need to be checked or updated only once in a while. 
// This function is called from the main loop once every 5 secs to do updates.
void PeriodicHousekeep(void)
{
    // List of items requiring infrequent updates...
    if (g_CurPage == PG_SITUATION || g_CurPage == PG_ZB_SITU)
    {
        String sNexTag = scnNode[SNSTNFLP].GetTagToNex();
        nexIo.SendAction(sNexTag, String(scnNode[SNSTNFLP].GetIntValue()));
    }
}


// Check whether currently conneted with XP-Direct.  Update the status indicator
//  located at the bottom-left of the display.  XP-only, not possible with FSTA.
// Return true if currently connected.
bool RefreshConnectStat(void)
{
    const String sFill = "fill 11,271,42,1,";
    // Connection status use
    static float lastSecStamp = 0;
    static uint16_t cycleSkip = 1;

    if (--cycleSkip == 0)
    {
        if (scnNode[SNOTHOUR].GetFloatValue() != lastSecStamp)
        {
            nexIo.SendAction(sFill + "1900");   // Draw a green line
        }
        else
        {
            nexIo.SendAction(sFill + "0");      // Draw a dark line
        }
        lastSecStamp = scnNode[SNOTHOUR].GetFloatValue();
        cycleSkip = 20000;         // Check once every 20000 cycles
    }

    return true;
}



// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = 
// = = = = = = = = = = = = = =  SCREEN SAVER MANAGEMENT  = = = = = = = = = = = = = = = =
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = 

// Screen Saver management variables.
boolean  bDimmed = true;           // Current dimming state
uint32_t dimTimer = millis();      // Start time for dimming countdown


// This function manages the display timer, entering and exiting dimming mode.
// Call with True if intend to continue counting down to dim.
// Call with False if to wake up, or to reset the display timer.
// Return True if this call has cause dim mode to wake up.
bool ScreenSaver(tScreenSave action)
{
    if (bDimmed)
    {
        if (action == SCS_WAKE)           // Currently sleeping?
        {
            // Lights up again
            nexIo.SendAction("dim=" + String(g_Config.brightness));
            delay(500);         // Intentionally BLOCK everything to allow time to wake up
            dimTimer = millis();        // Restart timer
            bDimmed = false;

            return true;
        }
    }
    else
    {
        if (action == SCS_DIM ||
            (!IsNeverDim() && action == SCS_CONT))   // Currently keep counting down?
        {
            // Check for sleep timer
            if (action == SCS_DIM ||
                millis() > dimTimer + g_Config.screenDim * 60000)
            {
                nexIo.SendAction("dim=0");     // Time's up--lights out
                delay(500);     // Intentionally BLOCK everything to allow time to go dim
                bDimmed = true;
            }
        }
        else     // Received wake up signal while not in sleep,
        {        //  which means to reset the sleep timers
            dimTimer = snapTimerPage = snapTimerEnco = millis();
        }
    }
    return false;      // Was not a wake up event
}


// Return true if currently confgiured with no display dimming.
bool IsNeverDim(void)
{
    return g_Config.screenDim > DIMTIMEOUT_MAX;
}
