// Functions to implement the operations for:
//   Device Config Page
//
// Discman777    08/24/2021
// Version 1.08.25
// This freeware is provided “AS IS”.  Non-commercial use, modification, distribution at your own risk.


#include <EEPROM.h>
#include <GlobalVars.h>
#include <Supports.h>
#include <DefineNODEs.h>>
#include <DefinePAGEs.h>>
#include <paging.h>
#include <Page_RADIOS.h>
#include <Page_CONFIG.h>
#include <SimExchange.h>


// Config data storage byte locations in EEPROM.
#define ECFG_BRIGHTNESS      0
#define ECFG_SCREENDIM       1
#define ECFG_DISPUNITS       2
#define ECFG_PROFILE         3
#define ECFG_Reserved0       4
#define ECFG_Reserved1       5
#define ECFG_Reserved2       6
#define ECFG_Reserved3       7
#define ECFG_SITU_GS_IAS     8
#define ECFG_SITU_TDOWN      9

#define ECFG_SDTBYFREQS      16       // Span 6x4=24 bytes. Radio page's frequencies
#define ECFG_CONFIG_END      (ECFG_SDTBYFREQS + sizeof(float) * NUM_OF_STDBYFREQS)


// - = - = - = - = - = - = -  Functions for the CONFIG Page  - = - = - = - = - = - = -


// Service for selecting the three available values in the Config Page.
void SelectAllConfig(int16_t dummy)
{
    page[PG_CONFIG].SetEncoAttach(0, SNFTBRIG);       // Brightness
    page[PG_CONFIG].SetEncoAttach(1, SNFTDIMM);       // Screen dim
    page[PG_CONFIG].SetEncoAttach(2, INVLD_NODEINX);
    page[PG_CONFIG].SetEncoAttach(3, SNFTBQNH);       // Display units

    page[PG_CONFIG].SetPushAttach(1, SNFTDIMM);       // Dim now
    HighlightSelection();
}


// Service to perform any setup requirement when entering this page each time.
void EntrySetup_Config(void)
{
    // Push the current values to be displayed
    NodeToSimCommon(SNFTBRIG, 0);
    NodeToSimCommon(SNFTDIMM, 0);
    NodeToSimCommon(SNFTBQNH, 0);
    NodeToSimCommon(SNFBPROF, 0);
}


// Recall settings from flash into working display copies.
void RetrieveNvConfig(void)
{
    EEPROM.begin(ECFG_CONFIG_END);
    g_Config.brightness = EEPROM.readByte(ECFG_BRIGHTNESS);
    InputEnforcement(ECFG_BRIGHTNESS);
    nexIo.SendAction("dim=" + String(g_Config.brightness));
    g_Config.screenDim = EEPROM.readByte(ECFG_SCREENDIM);
    InputEnforcement(ECFG_SCREENDIM);
    g_Config.dispUnits = EEPROM.readByte(ECFG_DISPUNITS);
    InputEnforcement(ECFG_DISPUNITS);
    g_Config.curProf = EEPROM.readByte(ECFG_PROFILE);
    InputEnforcement(ECFG_PROFILE);
    g_Stat.bCurGsDisp = EEPROM.readByte(ECFG_SITU_GS_IAS);
    g_Stat.bTouchdownDisp = EEPROM.readByte(ECFG_SITU_TDOWN);

    // Read the standby frequencies
    for (uint16_t fInx = 0; fInx < NUM_OF_STDBYFREQS; fInx++)
    {
        g_Config.stbyFreqs[fInx] = EEPROM.readFloat(ECFG_SDTBYFREQS + fInx * sizeof(float));
        // Check if the retrieval is successful and freq is within valid range
        if (isnan(g_Config.stbyFreqs[fInx]) || g_Config.stbyFreqs[fInx] < 108. || g_Config.stbyFreqs[fInx] >= 137.)
        {
            g_Config.stbyFreqs[fInx] = 123.45;      // Apply a valid default if failed the check
        }

        // Call the Radios page to enter frequency to it
        FreqConfigToPage(fInx);
    }

    g_bSaveRequired = false;
}


// This function processes the increment and decrement encoder events and modifies
//  the parameters.  This is esentially the command processor for the system opcodes
//  used by configuration.  The modified values will be flushed to flash only when
//  the user exits the Config Page (i.e. by entering a new page).
void HandleConfigInput(uint16_t opcode)
{
    // Point to the first screen node for the Config Page.  Hardcoded below, node entries
    //  must be defined in order.
    int16_t scnInx;
    String sBuf;

    switch (opcode)
    {
        // Actions from the Brightness adjustment rotary
        // CCW turn
        case LBC_DEC_BRIGHTNESS:
            g_Config.brightness -= 4;

        // CW turn
        case LBC_INC_BRIGHTNESS:
            if (opcode == LBC_INC_BRIGHTNESS)
            {
                g_Config.brightness += 4;
            }
            // Implement the change
            InputEnforcement(ECFG_BRIGHTNESS);
            nexIo.SendAction("dim=" + String(g_Config.brightness));

        // Push the new value into the node
        case LBC_SHW_BRIGHTNESS:
            sBuf = String(g_Config.brightness) + "%";
            nexIo.SendAction(scnNode[SNFTBRIG].GetTagToNex(), sBuf);
            break;

        // Actions from the Display Timeout adjustment rotary
        // CCW turn
        case LBC_DEC_SCREENDIM:
            g_Config.screenDim -= (g_Config.screenDim > 20) ? 10 : 1;

        // CW turn
        case LBC_INC_SCREENDIM:
            if (opcode == LBC_INC_SCREENDIM)
            {
                g_Config.screenDim += (g_Config.screenDim >= 20) ? 10 : 1;
            }
            
            InputEnforcement(ECFG_SCREENDIM);

        // Push the new value into the node
        case LBC_SHW_SCREENDIM:
            sBuf = IsNeverDim() ? "  never" : String(g_Config.screenDim) + " mins";
            nexIo.SendAction(scnNode[SNFTDIMM].GetTagToNex(), sBuf);
            break;

        // Actions from the Display Units selection rotary
        // CCW turn
        case LBC_DEC_DISPUNIT:
            g_Config.dispUnits -= (g_Config.dispUnits > 0) ? 1 : 0;

        // CW turn
        case LBC_INC_DISPUNIT:
            if (opcode == LBC_INC_DISPUNIT)
            {
                g_Config.dispUnits += (g_Config.dispUnits < 1) ? 1 : 0;
            }

        // Push the new value into the node
        case LBC_SHW_DISPUNIT:
            sBuf = g_Config.dispUnits == 0 ? "inHg" : "hPa";
            nexIo.SendAction(scnNode[SNFTBQNH].GetTagToNex(), sBuf);
            break;

        // Push the Profile index into the node
        case LBC_SHW_PROFILE:
            scnNode[SNFBPROF].SetValue(g_Config.curProf);
            break;

    }  // switch
    
    g_bSaveRequired = true;
}


// Dim the display immediately.
void DisplayDimNow(int16_t scnInx)
{
    ScreenSaver(SCS_DIM);
}


// Cycle through the profile selection.
//  This is called from the function pointer from the screen node.
void CycleProfile(int16_t scnInx)
{
    if (++g_Config.curProf >= MAX_PROFS)    // Wrap if out of range
    { 
        g_Config.curProf = 0;
    }
    scnNode[scnInx].SetValue(g_Config.curProf);    // Save back for the display

    // Highlight the "Accept & Restart" button to be used
    nexIo.SendAction(scnNode[SNFBACPT].GetTagToNex(), String(CLR_TEXT_STANDARD));

    // Signal a software re-initialzation will be required
    g_bProfileChanged = true;
}


// The following function enfores the config parameters are within acceptable limits.
void InputEnforcement(uint16_t paramId)
{
    switch (paramId)
    {
        case ECFG_BRIGHTNESS:
            g_Config.brightness = max(g_Config.brightness, (uint8_t) 4);
            g_Config.brightness = min(g_Config.brightness, (uint8_t) 100);
            break;

        case ECFG_SCREENDIM:
            g_Config.screenDim = max(g_Config.screenDim, (uint8_t) 1);
            g_Config.screenDim = min(g_Config.screenDim, (uint8_t) (DIMTIMEOUT_MAX + 10));
            break;

        case ECFG_DISPUNITS:
            g_Config.dispUnits = (g_Config.dispUnits > 1) ? 1 : g_Config.dispUnits;
            break;

        case ECFG_PROFILE:
            g_Config.curProf = min(g_Config.curProf, (uint8_t) (MAX_PROFS - 1));
            break;
    }
}


// Called when page exits to save current settings into the flash.
void ConfigCommitSettings(void)
{
    EEPROM.writeByte(ECFG_BRIGHTNESS, g_Config.brightness);
    EEPROM.writeByte(ECFG_SCREENDIM, g_Config.screenDim);
    EEPROM.writeByte(ECFG_DISPUNITS, g_Config.dispUnits);
    EEPROM.writeByte(ECFG_PROFILE, g_Config.curProf);
    EEPROM.writeByte(ECFG_SITU_GS_IAS, g_Stat.bCurGsDisp);
    EEPROM.writeByte(ECFG_SITU_TDOWN, g_Stat.bTouchdownDisp);

    // Write each standby frequency to the flash
    for (uint16_t fInx = 0; fInx < NUM_OF_STDBYFREQS; fInx++)
    {
        // Call the Radios page to copy the frequency to the COnfig structure
        FreqPageToConfig(fInx);
        EEPROM.writeFloat(ECFG_SDTBYFREQS + fInx * sizeof(float), g_Config.stbyFreqs[fInx]);
    }

    EEPROM.commit();
    g_bSaveRequired = false;
}


// Issue a Software Restart to the MCU
void MiniPanelRestart(int16_t dummy)
{
    ESP.restart();
}


// Accept the selection changes and restart, as required after a Profile change.
void ActivProfile(int16_t dummy)
{
    if (g_bProfileChanged)
    {
        ConfigCommitSettings();
        MiniPanelRestart(0);
    }
}
