// T.M.P.2 -- A Two-In-One Touchscreen Mini-Panel for MSFS2020 AND X-Plane11.
//
// Contain the call to initialization, and main loop--executing the three main "Tasks".
//
// Discman777    08/24/2021
// Version 1.08.25
// This freeware is provided "AS IS".  Non-commercial use, modification, distribution at your own risk.



#include <arduino.h>
#include <GlobalVars.h>
#include <Supports.h>
#include <Paging.h>
#include <SimExchange.h>
#include <Pages_MORE.h>


// Initialization
void setup()
{
    Serial.begin(XPLDIRECT_BAUDRATE);
    
    RetrieveNvConfig();          // Recall coinfig from flash memory
    SoftwareInit();              // Software component  initializations
    HardwareInit();              // MCU-related initializations

    // Start with displaying the "front page"
    OpenScreenPage(g_Profile[g_Config.curProf].scrn[0]);
}


// The Main Loop: Calling the Screen, Display tasks and the sim data fetcher.
// The Encoder and Screen Saver tasks are interrupt-driven.
void loop()
{
    // Launch data collector to fetch and/or refresh variables from the sim
    CollectorCommon();

    // Service a touch action from the user
    ScreenTask();

    // Refresh contents on the display with latest incoming data from the sim.
    // Each call will refresh data fetched from one screen node. 
    // Insert number below to keep refreshes x mS apart
    if (millis() > g_NextDispRunTime + 5)
    {
        DispTask();
    }

    // Periodic refresh of a list of necessary screen nodes in order for them to
    //  service multiple clients
    // Insert number below to keep revival every x mS
    if (millis() > g_NextRevivalTime + 250)
    {
        PeriodicRevival();
    }

#if !USE_ENCOD_INTRPT
    // Scan for rotary encoder action
    EncoderTask();
#endif
}


// Check for and process a Touch event.
void ScreenTask()
{
    // Got things to do only when user touches an item
    if (nexIo.FetchPacket())
    {
        // Look for an object to match for this new sim variable
        
        // Also take this opportunity to exit dimming mode
        if (!ScreenSaver(SCS_WAKE))
        {
            int16_t nodeIx = 0;

            while (nodeIx < page[g_CurPage].GetNumNodes())
            {
                int16_t scnInx = page[g_CurPage].GetInxToNode(nodeIx);

                // Check this action item's Nextion Op Index for a match
                if (scnNode[scnInx].GetIdFromNex() == nexIo.GetByte(INX_OP_ID))
                {
                    ExecActionForNode(scnInx);
                    break;
                }

                // Search next node
                nodeIx++;

            }
        }

        // Check if the Sidebar is touched.  Page to the destination page if so
        HandleSidebarPage();
    }
}


// Grab a packet from FS2020TA.  Then scan the screen nodes from the
//  current page, one node per call.  If the node is an input for sim value, see if any 
//  custom service should be rendered, applicable current value will be displayed.
void DispTask(void)
{
    int16_t scnInx = page[g_CurPage].GetInxToNode(g_NodeDisp);

    String sNexTag = scnNode[scnInx].GetTagToNex();
    
    // Check for match with From Sim ID, plus its index if exists
    if (scnNode[scnInx].IsInput() && scnNode[scnInx].DeltaCheck())
    {
        String sValue = "";
        // See whether custom Sim handler or no-frill handling?
        if (scnNode[scnInx].UseDispFunction())
        {
            // This node has a custom function to display sim data
            // Go execute the one for this sim client
            sValue = scnNode[scnInx].ExecuteDispFunc(scnInx);
        }
        else if (sNexTag != "")
        {
            // When there's no custom function, apply built-in formatting
            sValue = FormatNodeForDisplay(scnInx);
        }
        if (sNexTag != "" && sValue != "")
        {
            nexIo.SendAction(sNexTag, sValue);
        }
    } 

    // Continue to next node in next pass, wrap when necesary
    if (++g_NodeDisp >= page[g_CurPage].GetNumNodes())
    {
        g_NodeDisp = 0;
    }
    
    g_NextDispRunTime = millis();       // Timestamp time of recent run
}


// Encoder interrupt service function.
// Scan and process encoder and pushbutton actions.
void EncoderTask(void)
{
    // Scan the encoders one at a time
    for (int16_t scanPos = 0; scanPos < NUM_ENCODERS; scanPos++)
    {
        int16_t action;
        // First see if there's any rotary or pushbutton action
        if ((action = encoders[scanPos].Scan(true)) > ACT_NONE)
        {
            // Also take this opportunity to exit dimming mode
            if (ScreenSaver(SCS_WAKE))
            {
                break;
            }

            int16_t scnInx, opInx;
            uint16_t turnCnt;

            switch (action)
            {
                // Clockwise or Counter-clockwise turn
            case ACT_FAST_CCW:
            case ACT_FAST_CW:
            case ACT_TURN_CCW:
            case ACT_TURN_CW:
                // Turbo boost to burst 6 more detents
                turnCnt = (action == ACT_FAST_CCW || action == ACT_FAST_CW) ? 6 : 1;
                // Get the opcode index for the turn direction
                opInx = (action == ACT_TURN_CW || action == ACT_FAST_CW) ? 2 : 1;
                for (; turnCnt > 0; turnCnt--)
                {
                    // Obtain the screen node controlled with this encoder 
                    scnInx = page[g_CurPage].GetEncAttach(scanPos);
                    // Send the trigger event ID to the sim
                    NodeToSimCommon(scnInx, opInx);
                }
                break;

                // Quick-press
            case ACT_PRESS:
                // Obtain the screen node controlled by this pushbutton
                scnInx = page[g_CurPage].GetPushAttach(scanPos);
                ExecActionForNode(scnInx);
                break;

                // Long-Touch:
            case ACT_PRESS_LONG:
                // Obtain the screen node controlled by this pushbutton
                scnInx = page[g_CurPage].GetLongAttach(scanPos);
                if (scnInx != INVLD_SCRNOBJ)
                {
                    ExecActionForNode(scnInx);
                }
                break;

                default:
                    continue;
            }  // switch
        }
    }
}
