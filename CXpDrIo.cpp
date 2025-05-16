// Class to handle packet I/O operations specific to X-Plane Direct:
//  Input of data packets from X-Plane Direct.
//  Output of event from MCU to X-Plane Direct's datarefs.
//
// Specifically designed for use with the X-Plane Direct interface.
//  See https://www.patreon.com/curiosityworkshop
//
// Discman777    08/24/2021
// Version 1.08.25
// This freeware is provided “AS IS”.  Non-commercial use, modification, distribution at your own risk.


#include <GlobalVars.h>
#include <SimExchange.h>
#include <DefineNODEs.h>
#include <DefinePROFILEs.h>
#include <Page_CONFIG.h>


// Constructor
CXpDrIo::CXpDrIo()
{
    uint16_t m_CycleSkip = 1;            // Initialize connection sttaus variables
    float m_LastSecStamp = 0;

    Serial.begin(XPLDIRECT_BAUDRATE);      // Start serial interface to XPLDirect
    XPDplugin.begin("TMP2");               // Send a texual identifier
};


// X-Plane sim variable collector.
// This will cause the new values to be deposited into the designated sreen nodes.
int CXpDrIo::CollectVariables(void)
{
    return XPDplugin.xloop();
}


// Attach X-Plane Commands to handles and Datarefs into variables.
// Handles are for issuing command trigger, and variables are for dataref modification.
//  by touch event or encoder movement.
void CXpDrIo::AttachXpExtras(void)
{
    cmd_TimeDnFast = XPDplugin.registerCommand("sim/operation/time_down_lots");
    cmd_TimeUpFast = XPDplugin.registerCommand("sim/operation/time_up_lots");

    // Aditional datarefs and commands which do not belong in the screen nodes.
    if (IsZiboProfile())
    {
        // Mapping for the panel brightness, 4 elements
        for (uint16_t inx = 0; inx <= 3; inx++)
        {
            XPDplugin.registerDataRef("laminar/B738/electric/panel_brightness",
                XPL_READWRITE, 150, 0.1, &zbPanelBrightness[inx], inx);
        }

        // Mapping for the interior/flood brightness, 4 elements
        for (uint16_t inx = 6; inx <= 9; inx++)
        {
            XPDplugin.registerDataRef("sim/cockpit2/switches/generic_lights_switch",
                XPL_READWRITE, 150, 0.1, &zbFloodBrightness[inx], inx);
        }
        // One more for circuit breaker
        XPDplugin.registerDataRef("sim/cockpit2/switches/generic_lights_switch",
            XPL_READWRITE, 150, 0.1, &zbFloodBrightness[12], 12);

        // Mapping for the display unit brightness, first 6 elements
        for (uint16_t inx = 0; inx <= 5; inx++)
        {
            XPDplugin.registerDataRef("laminar/B738/electric/instrument_brightness",
                XPL_READWRITE, 150, 0.1, &zbDisplayBrightness[inx], inx);
        }
        // One more for the FMCs
        for (uint16_t inx = 10; inx <= 11; inx++)
        {
            XPDplugin.registerDataRef("laminar/B738/electric/instrument_brightness",
                XPL_READWRITE, 150, 0.1, &zbDisplayBrightness[inx], inx);
        }
    }

    if (IsToLissProfile())
    {
        XPDplugin.registerDataRef("AirbusFBW/OHPBrightnessLevel", XPL_READWRITE,
            150, 0.1, &tlPanelBrightness[0]);
        XPDplugin.registerDataRef("AirbusFBW/PanelBrightnessLevel", XPL_READWRITE,
            150, 0.1, &tlPanelBrightness[1]);

        XPDplugin.registerDataRef("AirbusFBW/PanelFloodBrightnessLevel", XPL_READWRITE,
            150, 0.1, &tlFloodBrightness[0]);
        XPDplugin.registerDataRef("AirbusFBW/PedestalFloodBrightnessLevel", XPL_READWRITE,
            150, 0.1, &tlFloodBrightness[1]);

        // Mapping for the interior and flood brightness, 8 elements
        for (uint16_t inx = 0; inx <= 7; inx++)
        {
            XPDplugin.registerDataRef("AirbusFBW/DUBrightness", XPL_READWRITE,
                150, 0.1, &tlDisplayBrightness[inx], inx);
        }
    }
}


// From the given Operation Index (which actually is FS2020TA's event), map it
//  into an corresponding X-Plane operation, that can be a dataref value update
//  or triggering a command.
// These cases are for handling those operations that simply send an event code to
//  the sim to set/inc/dec without resorting to sophistication custom functions.
void CXpDrIo::SendCmdAndData(uint16_t opcode, int32_t operand, int16_t scnInx)
{
    if (opcode == 0)
    {
        return;
    }

    int32_t iValue;
    int32_t delta;
    float fValue;

    // Execute XP Command with various triggering mechanisms, or update XP Dataref
    if (scnNode[scnInx].IsSystemOp())
    {
        HandleConfigInput(opcode);
    }
    else if (scnNode[scnInx].IsSendDirect())
    {
        XPDplugin.commandTrigger(opcode);
    }
    else if (scnNode[scnInx].IsCmdStart())
    {
        // Use the command Start option as long as the touch is pressed and held
        XPDplugin.commandStart(opcode);
    }
    else if (scnNode[scnInx].IsCmdEnd())
    {
        // Use the command End option when the touch is released
        XPDplugin.commandEnd(opcode);
    }
    else if (scnNode[scnInx].IsDatarefUpdate())
    {
        // Simply write the given operand to the node's dataref
        scnNode[scnInx].SetValue(operand);
    }

    // Execute XP Dateref or XP Command code translated from MSFS
    else if (opcode < XP_SENDIDTO_DATAREF_START)
    {
        switch (opcode)
        {
            case OD_ApSpeed_Dec:
            case OD_ApSpeed_Inc:
                iValue = scnNode[scnInx].GetFloatValue();
                iValue += opcode == OD_ApSpeed_Inc ? 1 : -1;

                iValue = max(iValue, 100);
                scnNode[scnInx].SetValue((float)iValue);
                break;

            case OD_ApHeading_Dec:
            case OD_ApCourse1_Dec:
            case OD_ApCourse2_Dec:
                if ((iValue = scnNode[scnInx].GetIntValue() - 1) < 0)
                {
                    iValue += 360;
                }
                scnNode[scnInx].SetValue(iValue);
                break;

            case OD_ApHeading_Inc:
            case OD_ApCourse1_Inc:
            case OD_ApCourse2_Inc:
                iValue = (scnNode[scnInx].GetIntValue() + 1) % 360;
                scnNode[scnInx].SetValue(iValue);
                break;

            case OD_ApAltitude_Dec:
            case OD_ApAltitude_Inc:
                iValue = scnNode[scnInx].GetIntValue();
                delta = 100;
                if (iValue >= 18000)
                {
                    delta = 1000;
                }
                else if (iValue >= 10000)
                {
                    delta = 500;
                }

                iValue += (opcode == OD_ApAltitude_Inc) ? delta : -delta;

                iValue -= iValue % 100;
                iValue = min(iValue, 44000);
                iValue = max(iValue, -500);
                scnNode[scnInx].SetValue(iValue);
                break;

            case OD_ApVS_Dec:
            case OD_ApVS_Inc:
                iValue = scnNode[scnInx].GetIntValue();
                iValue += opcode == OD_ApVS_Inc ? 100 : -100;
                iValue -= iValue % 100;
                scnNode[scnInx].SetValue(iValue);

                break;

            case OC_TimeDnFast:
                XPDplugin.commandTrigger(cmd_TimeDnFast, 13);
                break;

            case OC_TimeUpFast:
                XPDplugin.commandTrigger(cmd_TimeUpFast, 13);
                break;

            case OD_Accelerate_Dec:
            case OD_Accelerate_Inc:
                fValue = scnNode[scnInx].GetFloatValue();
                fValue += (opcode == OD_Accelerate_Inc) ? 0.5 : -0.5;
                fValue = max(fValue, float(1));
                fValue = min(fValue, float(16));
                scnNode[scnInx].SetValue((float) (fValue));
                break;

        }  // switch
    }

    // Execute a XP-specific dataref modification opcode
    else if (opcode >= XP_SENDIDTO_DATAREF_START)
    {
        switch (opcode)
        {
            case OD_XpndrMode_Dec:
            case OD_XpndrMode_Inc:
                iValue = scnNode[scnInx].GetIntValue();
                iValue += (opcode == OD_XpndrMode_Inc) ? 1 : -1;
                iValue = max(iValue, 0);
                iValue = min(iValue, 4);
                scnNode[scnInx].SetValue(iValue);
                break;

            case OD_Volume_Dec:
            case OD_Volume_Inc:
                fValue = scnNode[scnInx].GetFloatValue();
                fValue += (opcode == OD_Volume_Inc) ? 0.1 : -0.1;
                fValue = max(fValue, float(0));
                fValue = min(fValue, float(1.1));
                scnNode[scnInx].SetValue(fValue);
                break;

            case OD_ZbLightFmc_Dec:
            case OD_ZbLightFmc_Inc:
                fValue = (opcode == OD_ZbLightFmc_Dec) ? -0.1 : 0.1;
                AdjustBrightness(&zbDisplayBrightness[10], fValue);
                AdjustBrightness(&zbDisplayBrightness[11], fValue);
                break;

            case OD_ZbLightPanel_Dec:
            case OD_ZbLightPanel_Inc:
                fValue = (opcode == OD_ZbLightPanel_Dec) ? -0.1 : 0.1;
                for (uint16_t inx = 0; inx <= 3; inx++)
                {
                    AdjustBrightness(&zbPanelBrightness[inx], fValue);
                }
                break;

            case OD_ZbLightFlood_Dec:
            case OD_ZbLightFlood_Inc:
                fValue = (opcode == OD_ZbLightFlood_Dec) ? -0.1 : 0.1;
                for (uint16_t inx = 6; inx <= 9; inx++)
                {
                    AdjustBrightness(&zbFloodBrightness[inx], fValue);
                }
                AdjustBrightness(&zbFloodBrightness[12], fValue);
                break;

            case OD_ZbLightDispl_Dec:
            case OD_ZbLightDispl_Inc:
                fValue = (opcode == OD_ZbLightDispl_Dec) ? -0.1 : 0.1;
                for (uint16_t inx = 0; inx <= 6; inx++)
                {
                    AdjustBrightness(&zbDisplayBrightness[inx], fValue);
                }
                break;

            case OD_TlLightPanel_Dec:
            case OD_TlLightPanel_Inc:
                fValue = (opcode == OD_TlLightPanel_Dec) ? -0.1 : 0.1;
                AdjustBrightness(&tlPanelBrightness[0], fValue);
                AdjustBrightness(&tlPanelBrightness[1], fValue);
                break;

            case OD_TlLightFlood_Dec:
            case OD_TlLightFlood_Inc:
                fValue = (opcode == OD_TlLightFlood_Dec) ? -0.1 : 0.1;
                AdjustBrightness(&tlFloodBrightness[0], fValue);
                AdjustBrightness(&tlFloodBrightness[1], fValue);
                break;

            case OD_TlLightDispl_Dec:
            case OD_TlLightDispl_Inc:
                fValue = (opcode == OD_TlLightDispl_Dec) ? -0.1 : 0.1;
                for (uint16_t inx = 0; inx <= 5; inx++)
                {
                    AdjustBrightness(&tlDisplayBrightness[inx], fValue);
                }
                break;

            case OD_TlLightFmc_Dec:
            case OD_TlLightFmc_Inc:
                fValue = (opcode == OD_TlLightFmc_Dec) ? -0.1 : 0.1;
                AdjustBrightness(&tlDisplayBrightness[6], fValue);
                AdjustBrightness(&tlDisplayBrightness[7], fValue);
                break;

            case OD_TlWiperLeft_Dec:
            case OD_TlWiperLeft_Inc:
                IncDecNodeContent(scnInx, (opcode == OD_TlWiperLeft_Inc), 2, 0);
                break;

            case OD_TlWiperRight_Dec:
            case OD_TlWiperRight_Inc:
                IncDecNodeContent(scnInx, (opcode == OD_TlWiperRight_Inc), 2, 0);
                break;

            case OD_TlLandElev_Dec:
            case OD_TlLandElev_Inc:
                float value = scnNode[scnInx].GetFloatValue();
                value += (opcode == OD_TlLandElev_Dec) ? -0.5 : 0.5;
                scnNode[scnInx].SetValue(value);
                break;

        }  // switch
    }
}


// Increment/decrement the integer value in the node.  Apply the
//  specified up/down parameter and range constraints.
// Return modified value.
int32_t CXpDrIo::IncDecNodeContent(int16_t scnInx, bool bUp, int32_t maxV, int32_t minV)
{
    int32_t iValue = scnNode[scnInx].GetIntValue();
    iValue += bUp ? 1 : -1;
    iValue = max(iValue, minV);
    iValue = min(iValue, maxV);
    scnNode[scnInx].SetValue(iValue);

    return iValue;
}


// Increment/decrement brightness level for the specified entry and by the amount.
// Return modified brightness value.
float CXpDrIo::AdjustBrightness(float* pSetting, float offset)
{
    float value = *pSetting + offset;
    value = max(value, float(0));
    value = min(value, float(1));

    return *pSetting = value;
}
