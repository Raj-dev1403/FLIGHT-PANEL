// Functions to implement the operations for the Radios Page.
//
// Discman777    08/24/2021
// Version 1.08.25
// This freeware is provided “AS IS”.  Non-commercial use, modification, distribution at your own risk.


#include <arduino.h>
#include <GlobalVars.h>
#include <Supports.h>
#include <DefineNODEs.h>>
#include <DefinePAGEs.h>>
#include <DefinePROFILEs.h>>
#include <Paging.h>
#include <Page_RADIOS.h>
#include <SimExchange.h>


// - = - = - = - = - = - = -  Functions for RADIOS Page  - = - = - = - = - = - = -
//

#define CLR_FREQ_COM     RGB16(6, 62, 10)    // 14282
#define CLR_FREQ_NAV     RGB16(20, 55, 27)   // 42747


// Define the acceptable ranges of the COM and NAV frequenies
const uint16_t freqBandFloor[2]   = { 118, 108 };
const uint16_t freqBandCeiling[2] = { 136, 117 };

// Labels for the Nextion text fields
const String sSbyScnLabel[NUM_OF_STDBYFREQS] = { "Ts1", "Ts2", "Ts3", "Ts4", "Ts5", "Ts6" };


// Operation ID for current selection in the stdby frequency pool
int16_t stdbyFreqSel = -1;
        
String stdbyFreqTxt[NUM_OF_STDBYFREQS] = { "118.325", "121.175",  // Some hard default values
                                           "130.010", "136.900",
                                           "115.800", "114.100" };

// Required number of character (string length) for each of the aboves
const uint8_t freqNumDigits[4] = { 7, 7, 3, 4 };


// X-Plane datarefs for writing standby frequencies.
long int dRef_SbyFreqCom1, dRef_SbyFreqCom2, dRef_SbyFreqNav1, dRef_SbyFreqNav2;

// Attach X-Plane datarefs for XP-Direct to write to sim's standby frequencies.
void InitRadiosPage(void)
{
    XPDplugin.registerDataRef("sim/cockpit2/radios/actuators/com1_standby_frequency_hz_833", XPL_WRITE, 10, 0, &dRef_SbyFreqCom1);
    XPDplugin.registerDataRef("sim/cockpit2/radios/actuators/com2_standby_frequency_hz_833", XPL_WRITE, 10, 0, &dRef_SbyFreqCom2);
    XPDplugin.registerDataRef("sim/cockpit2/radios/actuators/nav1_standby_frequency_hz", XPL_WRITE, 10, 0, &dRef_SbyFreqNav1);
    XPDplugin.registerDataRef("sim/cockpit2/radios/actuators/nav2_standby_frequency_hz", XPL_WRITE, 10, 0, &dRef_SbyFreqNav2);
}


// Service for selecting the applicable values in Radios Page.
void SelectAllRadios(int16_t dummy)
{
    // ToLiss' cutomized transponder UI uses incompatible datarefs, it can't
    //  be supported
    if (!IsToLissProfile())
    {
        page[PG_XP_RADIOS].SetEncoAttach(3, SNCTXPMO);     // Transponder mode setting
        // Show the dot prompt for rotary #4
        nexIo.SendAction("Pxc.pic", String(IMG_ROTARYDOT + 3));
    }
}


// Handler for touching one of the standby frequency entries.
// Distinguish long-touch (operand >= 6) for launching keypad entry.
//  else a short-touch for selecting one of the items from the pool.
void StdbyFreqTouched(int16_t scnInx)
{
    uint16_t touchOperand = nexIo.GetByte(INX_OP_ID2);
    stdbyFreqSel = touchOperand % NUM_OF_STDBYFREQS;       // Select this standby

    if (touchOperand >= NUM_OF_STDBYFREQS)
    {
        KeypadInput(stdbyFreqSel);       // Long-touch opens the keypad to accept input
    }
    else
    {
        RefreshStdbySelection();         // Short-touch selects one of the Standbys
    }
}


// Handler for accepting a user entry from the on-screen keypad.
// This function blocks loop execution.
void KeypadInput(int16_t scnInx)
{
    int16_t rubberbandHold = g_RubberbandIndex;  // Suspend during keypad operation
    g_RubberbandIndex = -1;

    nexIo.SendAction("page FREQKEYPAD");
    uint16_t tuneType = scnNode[scnInx].GetMsToSimId(2);    // Freq category

    // Initialize the value in entry box.  Pre-enter a 1 digit for COM/NAV
    String sInStr = "";
#if false                // Automatically fill in leading '1'
    sInStr = (tuneType == FRQ_COM || tuneType == FRQ_NAV) ? "1" : "";
#endif
    nexIo.SendAction("Tin.txt", sInStr);

    while (true)         // Loop until exit touch
    {
        if (nexIo.FetchPacket())     // Any key touch? 
        {
            if (!ScreenSaver(SCS_CONT))     // Wake up if necessary, then discard this touch
            {
                uint16_t scnOp = nexIo.GetByte(INX_OP_ID);

                switch (scnOp)
                {
                    case 0 ... 9:
                        if (sInStr.length() < freqNumDigits[FRQ_COM])
                        {
                            String sTestValue = sInStr + String(scnOp);
                            uint16_t testNewValue = sTestValue.toInt();

                            switch (tuneType)
                            {
                                case FRQ_COM:
                                case FRQ_NAV:
                                    // Enforce freq's whole number portion is between 108. and 136.
                                    if ( (sInStr.length() == 0 && testNewValue == 1) ||
                                         (sInStr.length() == 1 && testNewValue >= 10 && testNewValue <= 13) ||
                                         (testNewValue >= freqBandFloor[FRQ_NAV] && testNewValue <= freqBandCeiling[FRQ_COM]) )
                                    {
                                        sInStr = sTestValue;
                                        if (sInStr.length() == 3)     // Auto-append '.'
                                        {
                                            sInStr += ".";
                                        }
                                    }
                                    break;

                                case FRQ_ADF:
                                    // Enforce freq to between 190 and 1750
                                    if ( (sInStr.length() == 0 && testNewValue > 0) ||
                                         (sInStr.length() == 1 && testNewValue != 18) ||
                                         (sInStr.length() == 2 && testNewValue <= 190) ||
                                         (testNewValue >= 190 && testNewValue <= 1750) )
                                    {
                                        sInStr = sTestValue;
                                    }
                                    break;

                                case FRQ_XPDR:
                                    if (sInStr.length() < freqNumDigits[tuneType] && scnOp <= 7)
                                    {
                                        sInStr = sTestValue;
                                    }
                                    break;
                            }
        
                        }
                        break;

                    case 0xD:           // Enter/Accept
                        if (scnOp == 0xD)
                        {
                            switch (tuneType)
                            {
                                case FRQ_COM:
                                case FRQ_NAV:
                                    // Enforce that all three whole digits plus '.' present
                                    if (sInStr.length() < 4)
                                    {
                                        continue;
                                    }
                                    while (sInStr.length() < freqNumDigits[tuneType])
                                    {
                                        sInStr += "0";    // Right fill with '0's
                                    }
                                    stdbyFreqTxt[scnInx] = sInStr;
                                    
                                    g_bSaveRequired = true;   // Writeback to flash is required
                                    break;

                                case FRQ_ADF:
                                    if (sInStr.length() < freqNumDigits[tuneType])
                                    {
                                        continue;
                                    }
                                    SetAdfFrequency(scnInx, sInStr);
                                    break;
                                
                                case FRQ_XPDR:
                                    if (sInStr.length() < freqNumDigits[tuneType])
                                    {
                                        continue;
                                    }
                                    // Send event ID and frequency to sim
                                    SetXponderCode(scnInx, sInStr);
                                    break;
                            }
                        }

                    case 0xA:           // Cancel-X
                        g_RubberbandIndex = rubberbandHold;    // Restore rubber banding
                        OpenScreenPage(g_CurPage);      // Restore previous page
                        return;

                    case 0xC:           // Backspace
                        if (sInStr.length() > 0)
                        {
                            if (sInStr.endsWith(".") || sInStr.length() > freqNumDigits[FRQ_COM] - 1)
                            {
                                sInStr.remove(sInStr.length() - 1);   // Back over the '.'
                            }
                            sInStr.remove(sInStr.length() - 1);
                        }
                        break;

                }  // switch

                // Refresh value in entry box
                nexIo.SendAction("Tin.txt", sInStr);

            }  // if power mgt
        }  // if touch

    }  // while endless
}


// Display active COM and NAV frequency for X-Plane.
String XP_ShowComNavFreq(int16_t scnInx)
{
    // Fetch the frequency and divide to get to float
    float freq = scnNode[scnInx].GetFloatValue();
    char sFreq[10];
    if (scnInx == SNCTCOM1 || scnInx == SNCTCOM2)
    {
        sprintf(sFreq, "%7.3f", freq / 1000.);
    }
    else
    {
        sprintf(sFreq, "%6.2f", (float) freq / 100.);
    }
    return String(sFreq);
}


// Execute a swap between a selected standby frequency with the given target screen index.
// Make X-Plane mimic the above as much as possible.
void SwapFreqSelStdby(int16_t scnInx)
{
    if (stdbyFreqSel >= 0)
    {
        uint16_t stdbyFreqWhole = stdbyFreqTxt[stdbyFreqSel].toInt();
        // The op IDs 1 & 2 on this screen are COMs, items 3 & 4 are NAVs
        // Qualify the acceptable freq range
        uint8_t  targetFreqType = (scnNode[scnInx].GetIdFromNex() < 3) ? FRQ_COM : FRQ_NAV;
        if (stdbyFreqWhole >= freqBandFloor[targetFreqType] &&
            stdbyFreqWhole <= freqBandCeiling[targetFreqType])
        {
            if (IsXPlane())     // X-Plane stores freq as integer
            {
                // For X-Plane, the procedure to put a value into an active frequencies is: 
                //   1. Fetch and hold the selected standby freq 
                //   2. Fetch and hold the selected active freq
                //   3. Send the value from #1 to the sim's active freq
                //   If #2 is a valid value...
                //   4. Read the active freq and save it in the source standby slot
                //   5. Send the value from #2 to the sim's standby freqs.
                float divider = (scnInx == SNCTCOM1 || scnInx == SNCTCOM2) ? 1000. : 100.;
                uint32_t newFreq = (uint32_t)(stdbyFreqTxt[stdbyFreqSel].toFloat() * divider + 0.5);
                String sOldActive = String(scnNode[scnInx].GetFloatValue() / divider);
                scnNode[scnInx].SetValue((float) newFreq);

                if (sOldActive.length() > 4)    // Minimum length to be valid
                {
                    stdbyFreqTxt[stdbyFreqSel] = sOldActive;
                    switch (scnInx)
                    {
                    case SNCTCOM1:
                        dRef_SbyFreqCom1 = (uint32_t)(sOldActive.toFloat() * 1000. + 0.5);
                        break;
                    case SNCTCOM2:
                        dRef_SbyFreqCom2 = (uint32_t)(sOldActive.toFloat() * 1000. + 0.5);
                        break;
                    case SNCTNAV1:
                        dRef_SbyFreqNav1 = (uint32_t)(sOldActive.toFloat() * 100. + 0.5);
                        break;
                    case SNCTNAV2:
                        dRef_SbyFreqNav2 = (uint32_t)(sOldActive.toFloat() * 100. + 0.5);
                        break;
                    }
                }
            }
            else
            {
                // For MSFS, the only doable procedure to put a value into an active frequencies is: 
                //   1. Write the selected freq from the Standby Pool into the chosen sim's standby freq
                //   2. Fetch and hold the selected active freq
                //   3. Do a swap between the sim's active and the sim's standby freqs.
                //   If #2 is a valid value...
                //   4. Copy the old active freq to the selected slot of the Standby Pool

                NodeToSimCommon(scnInx, 1, TextToBCD16_7f(stdbyFreqTxt[stdbyFreqSel]));
                float oldActive = scnNode[scnInx].GetFloatValue();
                // Send the swap event to sim
                NodeToSimCommon(scnInx, 2);
                if (oldActive > 0.)
                {
                    stdbyFreqTxt[stdbyFreqSel] = oldActive;
                }
            }

            RefreshStdbySelection();
        }
    }
}


// Display the column of six standby frequencies and highlight the selected one (if avail).
// The Radios Page manages the Standby Freqs objects, i.e. does not go through the Disp Task
//  for displaying them.
void RefreshStdbySelection(void)
{
    uint16_t color;

    // Walk through each standby frequency display
    for (uint16_t inx = 0; inx < NUM_OF_STDBYFREQS; inx++)
    {
        float freq = stdbyFreqTxt[inx].toFloat();
        // See if this one is in COM or NAV range
        uint16_t range = 999;     // Default None
        if (freq >= freqBandFloor[0])
        {
            range = FRQ_COM;
        }
        else if (freq >= freqBandFloor[1])
        {
            range = FRQ_NAV;
        }

        char sTemp[8];
        sprintf(sTemp, "%7.3f", freq);    // Zero-fill the number
        stdbyFreqTxt[inx] = sTemp;

        nexIo.SendAction(sSbyScnLabel[inx] + ".txt", stdbyFreqTxt[inx]);
        // Color code the frequency text
        color = CLR_TEXT_STANDARD;
        // Check to see if this screen object has been selected?
        if (inx == stdbyFreqSel)
        {
            color = CLR_SELECT_STANDARD;
        }
        else if (range == FRQ_COM)
        {
            color = CLR_FREQ_COM;
        }
        else if (range == FRQ_NAV)
        {
            color = CLR_FREQ_NAV;
        }

        // Set the text color to reflect selected state
        nexIo.SendAction(sSbyScnLabel[inx] + ".pco", String(color));
    }
}


// Convert a String (text) of a frequency to a 16-bit BCD for sim consumption.
// Return 4 digits of BCD in 16 bits.  0 if conversion failed.
uint16_t TextToBCD16_7f(String sFreq)
{
    return TextToBCD16_4d(sFreq.substring(1, 6));
}


// Convert a String (up to 4 chars) to a 16-bit BCD for sim consumption.
// Return 4 digits of BCD in 16 bits.  0 if conversion failed.
uint16_t TextToBCD16_4d(String sBCD)
{
    uint16_t numBCD = 0;
    for (uint16_t inx = 0; inx < sBCD.length(); inx++)
    {
        uint8_t curChar = sBCD.charAt(inx);
        if (curChar >= '0' && curChar <= '9')
        {
            numBCD <<= 4;
            numBCD += curChar - '0';
        }
    }
    return numBCD;
}


// Called by the flash read function to transfer a standby frequency to the Radios page.
void FreqConfigToPage(uint16_t fInx)
{
    
    stdbyFreqTxt[fInx] = String(g_Config.stbyFreqs[fInx]);
}


// Called by the flash read function to transfer a standby frequency to the Config structure.
void FreqPageToConfig(uint16_t fInx)
{
    g_Config.stbyFreqs[fInx] = stdbyFreqTxt[fInx].toFloat();
}


// Send the transponder code to the sim.
void SetXponderCode(int16_t scnInx, String sFreq)
{
    if (IsXPlane())
    {
        scnNode[scnInx].SetValue((uint16_t)sFreq.toInt());
    }
    else
    {
        NodeToSimCommon(scnInx, 0, TextToBCD16_4d(sFreq));
    }
}


// Translate and show the transponder mode enum in text.
String XP_ShowXponderMode(int16_t scnInx)
{
    const String sGenericLabels[6] = { "OFF", "STDBY", "ON", "ALT", "TEST", "?" };
    const String sZiboLabels[6] = { "TEST", "STDBY", "ALTOFF", "ALT ON", "TA", "TA/RA" };
    const String sToLissLabels[6] = { "TA/RA", "TA", "XPNDR", "ALTOFF", "STBY", "?" };

    if (scnNode[scnInx].GetIntValue() < 6)
    {
        if (IsZiboProfile())
        {
            return sZiboLabels[scnNode[scnInx].GetIntValue()];
        }
        else if (IsToLissProfile())
        {
            return sToLissLabels[scnNode[scnInx].GetIntValue()];
        }
        return sGenericLabels[scnNode[scnInx].GetIntValue()];
    }
    return "?";
}


// Send the frequency for active ADF1 to the sim.
void SetAdfFrequency(int16_t scnInx, String sFreq)
{
    if (IsXPlane())
    {
        scnNode[scnInx].SetValue((uint16_t) sFreq.toInt());
    }
    else
    {
        // Send event ID and frequency to sim
        NodeToSimCommon(scnInx, 0, TextToBCD16_4d(sFreq));
        // For ADF, need to swap this standby with the Active 
        NodeToSimCommon(scnInx, 1);
    }
}


// Display ADF frequency.
String MS_ShowAdfFreq(int16_t scnInx)
{
    // Fetch the frequency from the sim
    float adfFreq = scnNode[scnInx].GetFloatValue() / 0x10000;
    char sBuf[6];
    sprintf(sBuf, "%4X", (uint16_t) (adfFreq + 0.5));

    return String(sBuf);
}




// - = - = - = - = - = - = -  Functions for XP GPS Pages  - = - = - = - = - = - = -
//

#define CLR_UNIT_SEL      RGB16(30, 62, 17)   // 63441
#define CLR_UNIT_UNSEL    RGB16(3, 41, 5)     // 7461


// Service for selecting the applicable encoder assignments in X-Plane GPS Pages.
void SelectAllGPS(int16_t dummy)
{
    page[PG_G1000].SetEncoAttach(0, SNG1KPLR);     // Pan
    page[PG_G1000].SetEncoAttach(1, SNG1KRNG);     // Range
    page[PG_G1000].SetEncoAttach(2, SNG1KFOU);     // Outter FMS
    page[PG_G1000].SetEncoAttach(3, SNG1KFIN);     // Inner FMS

    page[PG_G1000].SetPushAttach(0, SNG1KPLR);     // Push Pan
    page[PG_G1000].SetPushAttach(3, SNG1KFIN);     // Push CRSR

    page[PG_G530].SetEncoAttach(0, SNG5KCOO);      // Outter COM/VLOC
    page[PG_G530].SetEncoAttach(1, SNG5KCOI);      // Inner COM/VLOC
    page[PG_G530].SetEncoAttach(2, SNG5KGPO);      // Outter GPS
    page[PG_G530].SetEncoAttach(3, SNG5KGPI);      // Inner GPS

    page[PG_G530].SetPushAttach(1, SNG5KCOI);      // Push C/V
    page[PG_G530].SetPushAttach(3, SNG5KGPI);      // Push CRSR
}


// Set up XP commands for either the 530 or 430 GPS rotary knobs.
void SetupGpsRotaryCmds(void)
{
    // For unit2 (GPS430), skip first 3 elements of the array holding commands
    uint16_t ofst = g_Stat.curGpsCtrl == 1 ? 3 : 0;
 
    int16_t* pCmdHandle = (int16_t*) scnNode[SNG5KCOO].GetIntValue();
    scnNode[SNG5KCOO].AttachCommand(*(pCmdHandle + ofst + 1), 1);
    scnNode[SNG5KCOO].AttachCommand(*(pCmdHandle + ofst + 2), 2);

    pCmdHandle = (int16_t*)scnNode[SNG5KCOI].GetIntValue();
    scnNode[SNG5KCOI].AttachCommand(*(pCmdHandle + ofst), 0);
    scnNode[SNG5KCOI].AttachCommand(*(pCmdHandle + ofst + 1), 1);
    scnNode[SNG5KCOI].AttachCommand(*(pCmdHandle + ofst + 2), 2);

    pCmdHandle = (int16_t*)scnNode[SNG5KGPO].GetIntValue();
    scnNode[SNG5KGPO].AttachCommand(*(pCmdHandle + ofst + 1), 1);
    scnNode[SNG5KGPO].AttachCommand(*(pCmdHandle + ofst + 2), 2);

    pCmdHandle = (int16_t*)scnNode[SNG5KGPI].GetIntValue();
    scnNode[SNG5KGPI].AttachCommand(*(pCmdHandle + ofst), 0);
    scnNode[SNG5KGPI].AttachCommand(*(pCmdHandle + ofst + 1), 1);
    scnNode[SNG5KGPI].AttachCommand(*(pCmdHandle + ofst + 2), 2);

    // Update the label color on the screen
    nexIo.SendAction(scnNode[SNGB5SEL].GetTagToNex(),
                     String(g_Stat.curGpsCtrl == 0 ? CLR_UNIT_SEL : CLR_UNIT_UNSEL));
    nexIo.SendAction(scnNode[SNGB4SEL].GetTagToNex(),
                     String(g_Stat.curGpsCtrl == 1 ? CLR_UNIT_SEL : CLR_UNIT_UNSEL));
}


// Called when entering X-Plane's GPS page, will determine to use G1000 or G530 page.
void XP_GpsPageEntry(void)
{
    if ((g_Stat.curGpsCtrl & 0x2) == 0)      // G1000 bit not set?
    {
        OpenScreenPage(PG_G530);     // G530 page with exisitng unit selection
    }
}


// Select the GPS page to apply page and XP commands for G1000 or 530
//  or the 430 GPS buttons.
void XP_FlipGpsControls(int16_t scnInx)
{
    if (g_CurPage == PG_G1000)     // Currently on G1000 page?
    {
        if (scnInx == SNGB5SEL)    // Touched "G530..", change page
        {
            g_Stat.curGpsCtrl &= 0x1;    // Clear bit1
            OpenScreenPage(PG_G530);     // G530 page with exisitng unit selection
        }
    }
    else         // Currently on G530/430 page
    {
        if (scnInx == SNGB1SEL)    // Touched G1000, change page
        {
            g_Stat.curGpsCtrl |= 0x2;    // Set bit1
            OpenScreenPage(PG_G1000);
        }
        else       // Touched G530.., swap between G530/430
        {
            g_Stat.curGpsCtrl = scnInx == SNGB4SEL ? 1 : 0;
            // Update the command handles held in the nodes
            SetupGpsRotaryCmds();
        }
    }
}


// Use one rotary encoder for either Pan Left/Right or Pan Up/Down.  By switching
//  its assignment to the alternate Nextion ID numbers. Each ID holds a different
//  XP command, thus switching Nextion IDs of the encoder changes it function.
void XP_FlipPanDirect(int16_t scnInx)
{
    String sNexTag = scnNode[scnInx].GetTagToNex();
    uint16_t picId = scnNode[scnInx].GetMsToSimId(1);

    // Check which Nextion ID is currently attached to Encoder0
    // If it's the ID for Left/Right, switch it to ID Up/Down
    if (page[PG_G1000].GetEncAttach(0) == SNG1KPLR)
    {
        page[PG_G1000].SetEncoAttach(0, SNG1KPUD);
        nexIo.SendAction(sNexTag, String(picId + 1));
    }
    else     // Otherwise, switch attachment node from Up/Dn to L/R
    {
        page[PG_G1000].SetEncoAttach(0, SNG1KPLR);
        nexIo.SendAction(sNexTag, String(picId));
    }
}
