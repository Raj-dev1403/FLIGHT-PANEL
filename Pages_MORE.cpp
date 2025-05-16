// Functions to implement the operations for:
//   More Page
//   Sim Options Page
//   Event Sender and Data Monitor test Pages (FS2020 only)
//
// Discman777    08/24/2021
// Version 1.08.25
// This freeware is provided “AS IS”.  Non-commercial use, modification, distribution at your own risk.


#include <EEPROM.h>
#include <GlobalVars.h>
#include <Supports.h>
#include <DefineNODEs.h>
#include <DefinePAGEs.h>>
#include <paging.h>
#include <Pages_MORE.h>
#include <SimExchange.h>


// - = - = - = - = - = - = -  Functions for the MORE Page  - = - = - = - = - = - = -

#define CLR_BTNUSER_BGND      RGB16(10, 20, 10)      // 21130
#define CLR_BTNMODAL_BGND     RGB16(8, 12, 8)        // 16776

// The MORE Page consists of 12 buttons for luanching other pages not shown on the
//  Sidebar.  This function takes the specification from the profile and paint
//  the labels and then assigns destination page numbers for each button. 
void PopulateMOREPage(void)
{
    uint8_t profl = g_Config.curProf;

    uint8_t btnNum = 1;    // Paging button numbered 1 to 12
    // Iterate the pages from 7 to the last
    for (uint8_t pgInx = SIDEBAR_ITEMS; pgInx < g_Profile[profl].numScreens; pgInx++)
    {
        // Program the button label into Nextion
        char sBtnTxt[30], sTitle[20];
        strcpy(sTitle, page[g_Profile[profl].scrn[pgInx]].GetTitle().c_str());
        sprintf(sBtnTxt, "B%02d.txt=\"%s\"", btnNum, sTitle);
        nexIo.SendAction(sBtnTxt);

        // Use different button background colors for non-user (modal) pages
        sprintf(sBtnTxt, "B%02d.bco", btnNum);
        uint16_t color = page[g_Profile[profl].scrn[pgInx]].IsModal() ?
                          CLR_BTNMODAL_BGND : CLR_BTNUSER_BGND;
        nexIo.SendAction(sBtnTxt, String(color));
        btnNum++;
    }

    // Link the logical page number assigned to every button
    btnNum = 1;
    // Iterate from the page beyond the Sidebar all the way to the last slot
    //  to nullify the page# entry in all the nodes
    for (uint8_t pgInx = SIDEBAR_ITEMS; pgInx < MAX_PAGES_PROF; pgInx++)
    {
        int8_t pgNum = -1;      // Presume no page exists, to nullify
        if (pgInx < g_Profile[profl].numScreens)
        {
            pgNum = g_Profile[profl].scrn[pgInx];     // Set target page number
        }
        scnNode[SNMORB01 + btnNum - 1].InitOperands(pgNum);
        btnNum++;
    }
}


// - = - = - = - = - = - = -  Functions for the SIM OPTIONS Page  - = - = - = - = - = - = -

// Service for selecting the two available values in Sim Options Page.
void SelectAllSimOptions(int16_t dummy)
{
    page[PG_SIMOPTIONS].SetEncoAttach(0, SNOTHOUR);      // Hour
    page[PG_SIMOPTIONS].SetEncoAttach(1, SNOTMINU);      // Minute
    page[PG_SIMOPTIONS].SetEncoAttach(2, SNOTACCE);      // Sim rate
    if (IsXPlane())
    {
        page[PG_SIMOPTIONS].SetEncoAttach(3, SNOTVOLM);  // Volume
        // Show the dot prompt for rotary #4
        nexIo.SendAction("Pvo.pic", String(IMG_ROTARYDOT + 3));
    }
    HighlightSelection();
}


// Two functions to display local time in Hour:Minute, input is in seconds.
String ShowHourMinute(int16_t scnInx)
{
    uint32_t curTime = scnNode[scnInx].GetFloatValue() / 60;
    char sBuf[8];
    sprintf(sBuf, "%2d:%02d", curTime / 60, curTime % 60);
    return sBuf;
}


String XP_ShowTimeAccel(int16_t scnInx)
{
    float curTime = scnNode[scnInx].GetFloatValue();
    char sBuf[8];
    sprintf(sBuf, "%4.1f", curTime);
    return sBuf;
}


// Show the X-Plane master volume as 0-12
String XP_ShowVolumeLevel(int16_t scnInx)
{
    float volume = scnNode[scnInx].GetFloatValue();
    char sBuf[8];
    sprintf(sBuf, "%2d%", (int) (volume * 10. + 0.5));
    return sBuf;
}


// - = - = - = - = - = -  Function for the EVENT SENDER Page  - = - = - = - = - = - = -


// This function is FS2020TA-Specific.
// For testing: Allow user to type in an operation (event) ID and its optional data, the
//  pad then sends the pair to the Disp Task to execute.
void DoTestPadPage(void)
{
    int16_t rubberbandHold = g_RubberbandIndex;  // Suspend during interactive operation
    g_RubberbandIndex = -1;

    nexIo.SendAction("page TESTPAD");
    delay(25);              // BLOCK to allow page change to settle

    static String sId = "39";
    static int16_t index = 0;
    static String sValue = "";

    nexIo.SendAction("Tev.txt", sId);    // Set up placeholder values
    nexIo.SendAction("Tix.txt", String(index));
    nexIo.SendAction("Tda.txt", sValue);

    // This infinite while handles inputs until the user finishes by touching the
    //  Enter or Cancel button
    bool bIdSelected = true;
    while (true)         // Loop until exit touch
    {
        if (nexIo.FetchPacket())     // Any key touch? 
        {
            if (!ScreenSaver(SCS_CONT))     // Wake up if necessary, then discard this touch
            {
                uint16_t scrnOp = nexIo.GetByte(INX_OP_ID);
                switch (scrnOp)
                {
                    case 0 ... 9:
                        if (bIdSelected && sId.length() < 5)
                        {
                            sId += scrnOp;
                        }
                        if (!bIdSelected && sValue.length() < 7)
                        {
                            sValue += scrnOp;
                        }
                        break;

                    case 0xA:             // Exit
                        g_RubberbandIndex = rubberbandHold;    // Restore rubber banding
                        OpenScreenPage(g_CurPage);     // Restore previous page
                        return;

                    case 0xC:             // Backspace
                        if (bIdSelected && sId.length() > 0)
                        {
                            sId.remove(sId.length() - 1);
                        }
                        else if (sValue.length() > 0)
                        {
                            sValue.remove(sValue.length() - 1);
                        }
                        break;

                    case 0xD:            // Send event or trigger command
                        if (sId.toInt() > 0)
                        {
                            if (IsFS2020())
                            {
                                MS_DirectToSim(0, sId.toInt(), sValue.toInt());
                            }
                            else
                            {
                                NodeToSimCommon(sId.toInt(), index, sValue.toInt());
                            }
                        }
                        break;

                    case 0x12:            // ID text select
                        bIdSelected = true;
                        nexIo.SendAction("Tev.bco=65535");    // Bright backgnd for Id field
                        nexIo.SendAction("Tda.bco=50712");    // Darker backgnd for Value field
                        break;

                    case 0x13:            // Index text touch
                        index++;
                        index %= 3;
                        break;

                    case 0x14:            // Value text select
                        bIdSelected = false;
                        nexIo.SendAction("Tev.bco=50712");
                        nexIo.SendAction("Tda.bco=65535");
                        break;
                }

                // Refresh values in entry boxes
                nexIo.SendAction("Tev.txt", sId);
                nexIo.SendAction("Tix.txt", String(index));
                nexIo.SendAction("Tda.txt", sValue);

            }  // if power mgt
        }  // if touch
        
    }  // while
}



// - = - = - = - = - = - = -  Function for the DATA MONITOR Page  - = - = - = - = - = - = -


// This function is FS2020TA-Specific.
// For testing: Allow user to enter a variable number (and optional index), the monitor
//  catches the corresponding data received from the Disp Task, and display it.
void DoDataMonitorPage(void)
{
    int16_t rubberbandHold = g_RubberbandIndex;  // Suspend during interactive operation
    g_RubberbandIndex = -1;

    nexIo.SendAction("page DATAMON");
    delay(25);              // BLOCK to allow page change to settle

    uint16_t dataCnt = 0;
    static String sVar = "193";     // Set up placeholder values
    static String sIndex = "-1";
    String sData;
    bool bPaused = false;

    nexIo.SendAction("Tva.txt", sVar);
    nexIo.SendAction("Tix.txt", sIndex);
    nexIo.SendAction("Tda.txt", sData = "");

    // First section of this infinite while handles inputs, second section displays the data
    bool bVarSelected = true;
    while (true)         // Loop until exit touch
    {
        if (nexIo.FetchPacket())     // Any key touch? 
        {
            if (!ScreenSaver(SCS_CONT))     // Wake up if necessary, then discard this touch
            {
                uint16_t scrnOp = nexIo.GetByte(INX_OP_ID);
                switch (scrnOp)
                {
                    case 0 ... 9:
                        if (bVarSelected && sVar.length() < 5)
                        {
                            sVar += scrnOp;
                        }
                        if (!bVarSelected && sIndex.length() < 2)
                        {
                            sIndex += scrnOp;
                        }
                        break;

                    case 0xF:
                        if (!bVarSelected && sIndex.length() < 2)
                        {
                            sIndex += "-";                            
                        }
                        break;

                    case 0xA:             // Exit
                        g_RubberbandIndex = rubberbandHold;    // Restore rubber banding
                        OpenScreenPage(g_CurPage);     // Restore previous page
                        return;

                    case 0xC:             // Backspace
                        if (bVarSelected && sVar.length() > 0)
                        {
                            sVar.remove(sVar.length() - 1);
                        }
                        else if (sIndex.length() > 0)
                        {
                            sIndex.remove(sIndex.length() - 1);
                        }
                        break;

                    case 0xD:            // Pause
                        bPaused = !bPaused;
                        break;
                        
                    case 0x12:            // Variable text select
                        bVarSelected = true;
                        nexIo.SendAction("Tva.bco=65535");    // Bright backgnd for Value field
                        nexIo.SendAction("Tix.bco=50712");    // Darker backgnd for Index field
                        break;

                    case 0x13:            // Index text select
                        bVarSelected = false;
                        nexIo.SendAction("Tva.bco=50712");
                        nexIo.SendAction("Tix.bco=65535");
                        break;

                    case 0x14:            // Clear count
                        dataCnt = 0;
                        break;
                }  // switch

                // Refresh entry values
                nexIo.SendAction("Tva.txt", sVar);
                nexIo.SendAction("Tix.txt", sIndex);

            }  // if power mgt
        }  // if touch

        if (!bPaused)
        {
            String sDataVal = "";
            if (IsFS2020())
            {
                // Next check for incoming data packet from FS2020TA
                if (fstaIo.FetchPacket())
                {
                    // If the variable and index fields must match the incomings
                    if (fstaIo.GetPacketId() == sVar)
                    {
                        if (fstaIo.GetPacketInx() == sIndex)
                        {
                            // Refresh values in entry boxes
                            sDataVal = fstaIo.GetPacketVal();
                        }
                    }
                }
            }
            else       // X-Plane
            {
                // Next check for incoming data packet from XP/Direct
                if (!bPaused && xpdrIo.CollectVariables())
                {
                    uint16_t scnInx = sVar.toInt();
                    if (scnInx < SN_TAIL)
                    {
                        if (scnNode[scnInx].IsValueInt())
                        {
                            sDataVal = String(scnNode[scnInx].GetIntValue());
                        }
                        else if (scnNode[scnInx].IsValueFloat())
                        {
                            sDataVal = String(scnNode[scnInx].GetFloatValue());
                        }
                        else if (scnNode[scnInx].IsValueCstr())
                        {
                            sDataVal = String(scnNode[scnInx].GetChrValue());
                        }
                    }
                }
            }

            // See if there's any result to display
            if (sDataVal != "")
            {
                // Refresh values in entry boxes
                nexIo.SendAction("Tda.txt", sDataVal);
                nexIo.SendAction("Tct.txt", String(++dataCnt));
            }
        }
    }  // while
}