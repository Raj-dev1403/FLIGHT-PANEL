// The function to initialize the contents for all the screen nodes.
//
// Discman777    08/24/2021
// Version 1.08.25
// This freeware is provided “AS IS”.  Non-commercial use, modification, distribution at your own risk.


#include <arduino.h>
#include <GlobalVars.h>
#include <Supports.h>
#include <DefineNODEs.h>
#include <DefinePROFILEs.h>
#include <Pages_AP_NAV.h>
#include <Page_RADIOS.h>
#include <Pages_DEV_SIT.h>
#include <Page_CONFIG.h>
#include <XPLDirect.h>



void DefineAllNodes(void)
{
    // The Screen Node's attributes (some parameters optional):
    //  1. Nextion Operation ID, Command label to program Nextion
    //  2. Nextion object Type (bitmask)
    //  3. Id Read from Sim, index, Id Write to Sim
    //  4. Additional parameters such as Ids to sim for increment/decrement
    //  5. Handler function to respond to a sim input
    //  6. Handler function to respond to a touch action
    //  7. Current value of the sim item
    //  8. Data type of the value
    //  9. A control string to format numeric value
    //  10. The previous value to check for value changed

    // Methods to initialize above member attributes:
    //  1. InitNexCon(m_IdFromNex, m_sTagToNex)
    //  2. InitNexType(m_Type)
    //  3. InitSimIds(m_IdFromSim, m_InxFromSim, m_MsToSimId[0])
    //  4. InitOperands(m_MsToSimId[1] & [2])
    //  5. InitDispFuncMS(m_DispFunc_)
    //  6. InitTouchFuncMS(m_TouchFunc_)
    //  7. SetValue(m_Value...)
    //  8. SetSetVarType(m_VarType)
    //  9. InitFmtCtrl(m_sFmtCtrl)
    //  10. Previous value used a DeltaCheck function 


    // Blanket pre-initialization of the screen nodes to default characteristics.
    for (uint16_t sInx = 0; sInx < MAX_NODES; sInx++)
    {
        scnNode[sInx].InitNexType(0);
        scnNode[sInx].SetVarType(VT_INT);
        scnNode[sInx].InitFmtCtrl("");
    }


// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
// SIDEBAR.
// Six nodes are reserved to contain the six Sidebar Menu items.
// The Sidebar technically isn't a page.  It doesn't get displayed as a screen page.

    // The following 6 nodes determine which pages get assigned to the side menu.
    scnNode[SNZBSB0].InitNexCon(0x81);
    scnNode[SNZBSB0].InitNexType(AMK_PAGING);
    // Its destination pages will get initialized (into Op1) as the Sidebar is built in runtime.

    scnNode[SNZBSB1].InitNexCon(0x82);
    scnNode[SNZBSB1].InitNexType(AMK_PAGING);

    scnNode[SNZBSB2].InitNexCon(0x83);
    scnNode[SNZBSB2].InitNexType(AMK_PAGING);

    scnNode[SNZBSB3].InitNexCon(0x84);
    scnNode[SNZBSB3].InitNexType(AMK_PAGING);

    scnNode[SNZBSB4].InitNexCon(0x85);
    scnNode[SNZBSB4].InitNexType(AMK_PAGING);

    scnNode[SNZBSB5].InitNexCon(0x86);
    scnNode[SNZBSB5].InitNexType(AMK_PAGING);


// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
// PAGE: AUTOPILOT.
// Autopilot controls.

    if (IsGenericProfile())         //MSFS & X-Plane Generic-only
    {
        scnNode[SNATSPED].InitNexCon(0x12, "Tsp.txt");                // SPD text
        scnNode[SNATSPED].InitNexType(AMK_INPUT | AMK_SELECT);
        scnNode[SNATSPED].InitSimIds(74, -1, 122);                      // FromSimID, CmdToSim
        scnNode[SNATSPED].InitOperands(OD_ApSpeed_Dec, OD_ApSpeed_Inc);
        scnNode[SNATSPED].AttachSimFloat("sim/cockpit/autopilot/airspeed", XPL_READWRITE, 100, 0.01);
        scnNode[SNATSPED].InitTouchFuncAll(&SelectApPrimary);
        scnNode[SNATSPED].InitFmtCtrl("%3d");

        scnNode[SNATHDNG].InitNexCon(0, "Thd.txt");                   // HDG text
        scnNode[SNATHDNG].InitNexType(AMK_INPUT | AMK_SELECT);
        scnNode[SNATHDNG].InitSimIds(86, -1, 431);
        scnNode[SNATHDNG].InitOperands(OD_ApHeading_Dec, OD_ApHeading_Inc);
        scnNode[SNATHDNG].AttachSimInt("sim/cockpit/autopilot/heading_mag");
        scnNode[SNATHDNG].InitFmtCtrl("%03d");

        scnNode[SNATALTI].InitNexCon(0, "Tal.txt");                   // ALT text
        scnNode[SNATALTI].InitNexType(AMK_INPUT | AMK_SELECT);
        scnNode[SNATALTI].InitSimIds(76, -1, 64);
        scnNode[SNATALTI].InitOperands(OD_ApAltitude_Dec, OD_ApAltitude_Inc);
        scnNode[SNATALTI].AttachSimInt("sim/cockpit/autopilot/altitude");
        scnNode[SNATALTI].InitFmtCtrl("%5d");

        scnNode[SNATVSPD].InitNexCon(0, "Tvs.txt");                   // VS text
        scnNode[SNATVSPD].InitNexType(AMK_INPUT | AMK_SELECT);
        scnNode[SNATVSPD].InitSimIds(101, -1, 126);
        scnNode[SNATVSPD].InitOperands(OD_ApVS_Dec, OD_ApVS_Inc);
        scnNode[SNATVSPD].AttachSimInt("sim/cockpit/autopilot/vertical_velocity");
        scnNode[SNATVSPD].InitFmtCtrl("%+d");

        scnNode[SNATCRS1].InitNexCon(0x16, "Tc1.txt");                // Course 1 text
        scnNode[SNATCRS1].InitNexType(AMK_INPUT | AMK_SELECT);
        scnNode[SNATCRS1].InitSimIds(519, 1, 973);                      // FromSimID, index, CmdToSim
        scnNode[SNATCRS1].InitOperands(OD_ApCourse1_Dec, OD_ApCourse1_Inc);
        scnNode[SNATCRS1].AttachSimInt("sim/cockpit/radios/nav1_obs_degm");
        scnNode[SNATCRS1].InitTouchFuncAll(&SelectApCourse);
        scnNode[SNATCRS1].InitFmtCtrl("%03d");

        scnNode[SNATCRS2].InitNexCon(0, "Tc2.txt");                   // Course 2 text
        scnNode[SNATCRS2].InitNexType(AMK_INPUT | AMK_SELECT);
        scnNode[SNATCRS2].InitSimIds(519, 2, 976);
        scnNode[SNATCRS2].InitOperands(OD_ApCourse2_Dec, OD_ApCourse2_Inc);
        scnNode[SNATCRS2].AttachSimInt("sim/cockpit/radios/nav2_obs_degm");
        scnNode[SNATCRS2].InitFmtCtrl("%03d");

        scnNode[SNATMAXB].InitNexCon(0, "Tmb.txt");                   // Max bank text
        scnNode[SNATMAXB].InitNexType(AMK_INPUT | AMK_SELECT);
        scnNode[SNATMAXB].InitSimIds(91);
        scnNode[SNATMAXB].InitOperands(89, 90);                         // Dec, Inc
        scnNode[SNATMAXB].AttachSimInt("sim/cockpit2/autopilot/bank_angle_mode");
        scnNode[SNATMAXB].InitDispFuncXP(&ShowMaxBank_ZB);
        scnNode[SNATMAXB].InitFmtCtrl("%2d");

        scnNode[SNATABRK].InitNexCon(0, "Tab.txt");                   // Autobrake text
        scnNode[SNATABRK].InitNexType(AMK_INPUT | AMK_SELECT | AMK_SENDDIRECT);
        scnNode[SNATABRK].InitSimIds(72);
        scnNode[SNATABRK].InitOperands(241, 444);                       // Dec, Inc
        scnNode[SNATABRK].AttachSimInt("sim/cockpit2/switches/auto_brake_level");
        scnNode[SNATABRK].AttachCommand("sim/flight_controls/brakes_dn_auto", 1);
        scnNode[SNATABRK].AttachCommand("sim/flight_controls/brakes_up_auto", 2);
        scnNode[SNATABRK].InitFmtCtrl("%d");

        scnNode[SNABAUTO].InitNexCon(4, "Pap.pic");                   // AP button
        scnNode[SNABAUTO].InitNexType(AMK_INPUT | AMK_SENDDIRECT);
        scnNode[SNABAUTO].InitSimIds(90, -1, 88);
        scnNode[SNABAUTO].InitOperands(IMG_APAP);                       // Pic ID
        scnNode[SNABAUTO].AttachSimInt("sim/cockpit2/autopilot/autopilot_on_or_cws", XPL_READ, 200);
        scnNode[SNABAUTO].AttachCommand("sim/autopilot/servos_toggle", 0);
        scnNode[SNABAUTO].InitTouchFuncAll(&ToggleBinarySwitch);

        scnNode[SNABSPED].InitNexCon(5, "Psp.pic");                   // SPD button
        scnNode[SNABSPED].InitNexType(AMK_INPUT | AMK_SENDDIRECT);
        scnNode[SNABSPED].InitSimIds(75, -1, 149);
        scnNode[SNABSPED].InitOperands(IMG_APSPD);
        scnNode[SNABSPED].AttachSimInt("sim/cockpit2/autopilot/autothrottle_on", XPL_READ, 200);
        scnNode[SNABSPED].AttachCommand("sim/autopilot/autothrottle", 0);
        scnNode[SNABSPED].InitTouchFuncAll(&ToggleBinarySwitch);

        scnNode[SNABHDNG].InitNexCon(6, "Phd.pic");                   // HDG button
        scnNode[SNABHDNG].InitNexType(AMK_INPUT | AMK_SENDDIRECT);
        scnNode[SNABHDNG].InitSimIds(87, -1, 106);
        scnNode[SNABHDNG].InitOperands(IMG_APHDG);
        scnNode[SNABHDNG].AttachSimInt("sim/cockpit2/autopilot/heading_mode", XPL_READ, 200);
        scnNode[SNABHDNG].AttachCommand("sim/autopilot/heading", 0);
        scnNode[SNABHDNG].InitTouchFuncAll(&ToggleBinarySwitch);
        scnNode[SNABHDNG].InitDispFuncXP(&XP_ShowHeadingBtn);

        scnNode[SNABALTI].InitNexCon(7, "Pal.pic");                   // ALT button
        scnNode[SNABALTI].InitNexType(AMK_INPUT | AMK_SENDDIRECT);
        scnNode[SNABALTI].InitSimIds(77, -1, 102);
        scnNode[SNABALTI].InitOperands(IMG_APALT);
        scnNode[SNABALTI].AttachSimInt("sim/cockpit2/autopilot/altitude_hold_status", XPL_READ, 200);
        scnNode[SNABALTI].AttachCommand("sim/autopilot/altitude_hold", 0);
        scnNode[SNABALTI].InitTouchFuncAll(&ToggleBinarySwitch);
        scnNode[SNABALTI].InitDispFuncXP(&ShowTruncatedBinaryBtn);

        scnNode[SNABVSPD].InitNexCon(8, "Pvs.pic");                   // VS button
        scnNode[SNABVSPD].InitNexType(AMK_INPUT | AMK_SENDDIRECT);
        scnNode[SNABVSPD].InitSimIds(102, -1, 123);
        scnNode[SNABVSPD].InitOperands(IMG_APVS);
        scnNode[SNABVSPD].AttachSimInt("sim/cockpit2/autopilot/vvi_status", XPL_READ, 200);
        scnNode[SNABVSPD].AttachCommand("sim/autopilot/alt_vs", 0);
        scnNode[SNABVSPD].InitTouchFuncAll(&ToggleBinarySwitch);
        scnNode[SNABVSPD].InitDispFuncXP(&ShowTruncatedBinaryBtn);
        
        scnNode[SNABSPSY].InitNexCon(0x25);                           // SPD-sync Long-Touch
        scnNode[SNABSPSY].InitNexType(AMK_SENDDIRECT);
        scnNode[SNABSPSY].InitSimIds(0, -1, 55);
        scnNode[SNABSPSY].AttachCommand("sim/autopilot/airspeed_sync", 0);

        scnNode[SNABHDSY].InitNexCon(0x26);                           // Hdg-sync Long-Touch
        scnNode[SNABHDSY].InitNexType(AMK_SENDDIRECT);
        scnNode[SNABHDSY].InitOperands(106, 75);
        scnNode[SNABHDSY].AttachCommand("sim/autopilot/heading_sync", 0);
        scnNode[SNABHDSY].InitTouchFuncMS(&MS_SetHeadingSync);

        scnNode[SNABALSY].InitNexCon(0x27);                           // Alt-sync Long-Touch
        scnNode[SNABALSY].InitNexType(AMK_SENDDIRECT);
        scnNode[SNABALSY].InitSimIds(0, -1, 59);
        scnNode[SNABALSY].AttachCommand("sim/autopilot/altitude_sync", 0);

        scnNode[SNABVSSY].InitNexCon(0x28);                           // VS-sync Long-Touch
        scnNode[SNABVSSY].InitNexType(AMK_SENDDIRECT);
        scnNode[SNABVSSY].InitSimIds(0, -1, 126);
        scnNode[SNABVSSY].AttachCommand("sim/autopilot/vertical_speed_sync", 0);
        scnNode[SNABVSSY].InitTouchFuncMS(&MS_SyncVertSpeed);

        scnNode[SNABFLDR].InitNexCon(9, "Pfd.pic");                   // FD button
        scnNode[SNABFLDR].InitNexType(AMK_INPUT | AMK_DATAUPDATE);
        scnNode[SNABFLDR].InitSimIds(82, -1, 873);
        scnNode[SNABFLDR].InitOperands(IMG_APFD);                       // Pic ID
        scnNode[SNABFLDR].AttachSimInt("sim/cockpit2/autopilot/flight_director_mode");
        scnNode[SNABFLDR].InitTouchFuncXP(&ToggleBinarySwitch);
        scnNode[SNABFLDR].InitDispFuncXP(&XP_Show01Button);

        scnNode[SNABHNAV].InitNexCon(0xA, "Pnv.pic");                 // NAV button
        scnNode[SNABHNAV].InitNexType(AMK_INPUT | AMK_SENDDIRECT);
        scnNode[SNABHNAV].InitSimIds(94, -1, 96);
        scnNode[SNABHNAV].InitOperands(IMG_APNAV);                      // Pic ID
        scnNode[SNABHNAV].AttachSimInt("sim/cockpit2/autopilot/nav_status", XPL_READ, 200);
        scnNode[SNABHNAV].AttachCommand("sim/autopilot/NAV", 0);
        scnNode[SNABHNAV].InitDispFuncXP(&XP_Show01Button);

        scnNode[SNABYAWD].InitNexCon(0xC, "Pyd.pic");                 // YD button
        scnNode[SNABYAWD].InitNexType(AMK_INPUT | AMK_DATAUPDATE);
        scnNode[SNABYAWD].InitSimIds(104, -1, 993);
        scnNode[SNABYAWD].InitOperands(IMG_APYD);                       // Pic ID
        scnNode[SNABYAWD].AttachSimInt("sim/cockpit2/switches/yaw_damper_on");
        scnNode[SNABYAWD].InitTouchFuncXP(&ToggleBinarySwitch);

        scnNode[SNABAPPR].InitNexCon(0xD, "Ppr.pic");                 // APPR button
        scnNode[SNABAPPR].InitNexType(AMK_INPUT | AMK_SENDDIRECT);
        scnNode[SNABAPPR].InitSimIds(78, -1, 66);
        scnNode[SNABAPPR].InitOperands(IMG_APAPPR);                     // Pic ID
        scnNode[SNABAPPR].AttachSimInt("sim/cockpit2/autopilot/approach_status", XPL_READ, 200);
        scnNode[SNABAPPR].AttachCommand("sim/autopilot/approach", 0);
        scnNode[SNABAPPR].InitDispFuncXP(&ShowTruncatedBinaryBtn);

        scnNode[SNABNVSR].InitNexCon(0xE, "Pgp.pic");                 // GPS button
        scnNode[SNABNVSR].InitNexType(AMK_INPUT);
        scnNode[SNABNVSR].InitSimIds(0, -1, 879);
        scnNode[SNABNVSR].InitOperands(IMG_APGPS, 95);                // Pic ID, code for select NAV1 or 2
        scnNode[SNABNVSR].InitTouchFuncXP(&XP_CycleGpsNav12Sw);       // This is a tristate button,
        scnNode[SNABNVSR].InitDispFuncXP(&XP_ShowGpsNav12Btn);        //  working in conjunction with
        scnNode[SNABNVSR].InitTouchFuncMS(&MS_CycleGpsNav12Sw);       //  info from the NAV button 
        scnNode[SNABNVSR].AttachSimInt("sim/cockpit/switches/HSI_selector");

        scnNode[SNABVNAV].InitNexCon(0xB, "Pvn.pic");                 // VNAV button
        scnNode[SNABVNAV].InitNexType(AMK_INPUT | AMK_SENDDIRECT);
        scnNode[SNABVNAV].InitOperands(IMG_APVNAV);                     // Pic ID
        scnNode[SNABVNAV].AttachSimInt("sim/cockpit2/autopilot/vnav_status", XPL_READ, 400);
        scnNode[SNABVNAV].AttachCommand("sim/autopilot/vnav", 0);

        scnNode[SNABFLCH].InitNexCon(0xF, "Pfl.pic");                 // FLC button
        scnNode[SNABFLCH].InitNexType(AMK_INPUT | AMK_SENDDIRECT);
        scnNode[SNABFLCH].InitOperands(IMG_APFLC);                      // Pic ID
        scnNode[SNABFLCH].AttachCommand("sim/autopilot/level_change", 0);

        scnNode[SNABNGPS].InitNexCon(0);                              // Input only for GPS selection
        scnNode[SNABNGPS].InitNexType(AMK_INPUT);             // Used by below to gather input
        scnNode[SNABNGPS].InitSimIds(369);
        scnNode[SNABNGPS].InitDispFuncMS(&MS_ShowGpsNav12Btn);

        scnNode[SNABNV12].InitNexCon(0);                              // Input only for MSFS NAV1/NAV2
        scnNode[SNABNV12].InitNexType(AMK_INPUT);               
        scnNode[SNABNV12].InitSimIds(92);
        scnNode[SNABNV12].InitDispFuncMS(&MS_ShowGpsNav12Btn);
    }


// Autopilot controls for the Zibo profile.  XP-only.

    if (IsZiboProfile())         // Zibo-only
    {
        scnNode[SNZTSPED].InitNexCon(1, "Tsp.txt");                    // SPD text
        scnNode[SNZTSPED].InitNexType(AMK_INPUT | AMK_SELECT);
        scnNode[SNZTSPED].InitOperands(OD_ApSpeed_Dec, OD_ApSpeed_Inc);
        scnNode[SNZTSPED].AttachSimFloat("laminar/B738/autopilot/mcp_speed_dial_kts_mach", XPL_READWRITE, 100, 0.01);
        scnNode[SNZTSPED].InitTouchFuncAll(&SelectApPrimary);
        scnNode[SNZTSPED].InitDispFuncXP(&ShowAutoSpeed_ZB);

        scnNode[SNZTHDNG].InitNexCon(1, "Thd.txt");                    // HDG text
        scnNode[SNZTHDNG].InitNexType(AMK_INPUT | AMK_SELECT);
        scnNode[SNZTHDNG].InitOperands(OD_ApHeading_Dec, OD_ApHeading_Inc);
        scnNode[SNZTHDNG].AttachSimInt("laminar/B738/autopilot/mcp_hdg_dial");
        scnNode[SNZTHDNG].InitTouchFuncAll(&SelectApPrimary);
        scnNode[SNZTHDNG].InitFmtCtrl("%03d");

        scnNode[SNZTALTI].InitNexCon(1, "Tal.txt");                    // ALT text
        scnNode[SNZTALTI].InitNexType(AMK_INPUT | AMK_SELECT);
        scnNode[SNZTALTI].InitOperands(OD_ApAltitude_Dec, OD_ApAltitude_Inc);
        scnNode[SNZTALTI].AttachSimInt("laminar/B738/autopilot/mcp_alt_dial");
        scnNode[SNZTALTI].InitTouchFuncAll(&SelectApPrimary);
        scnNode[SNZTALTI].InitFmtCtrl("%5d");

        scnNode[SNZTVSPD].InitNexCon(1, "Tvs.txt");                    // VS text
        scnNode[SNZTVSPD].InitNexType(AMK_INPUT | AMK_SELECT);
        scnNode[SNZTVSPD].InitOperands(OD_ApVS_Dec, OD_ApVS_Inc);
        scnNode[SNZTVSPD].AttachSimInt("sim/cockpit2/autopilot/vvi_dial_fpm");
        scnNode[SNZTVSPD].InitTouchFuncAll(&SelectApPrimary);
        scnNode[SNZTVSPD].InitFmtCtrl("%+d");

        scnNode[SNZTCRS1].InitNexCon(2, "Tc1.txt");                    // Course1 text
        scnNode[SNZTCRS1].InitNexType(AMK_INPUT | AMK_SELECT);
        scnNode[SNZTCRS1].InitOperands(OD_ApCourse1_Dec, OD_ApCourse1_Inc);
        scnNode[SNZTCRS1].AttachSimInt("laminar/B738/autopilot/course_pilot");
        scnNode[SNZTCRS1].InitTouchFuncAll(&SelectApCourse);
        scnNode[SNZTCRS1].InitFmtCtrl("%03d");

        scnNode[SNZTCRS2].InitNexCon(2, "Tc2.txt");                    // Course2 text
        scnNode[SNZTCRS2].InitNexType(AMK_INPUT | AMK_SELECT);
        scnNode[SNZTCRS2].InitOperands(OD_ApCourse2_Dec, OD_ApCourse2_Inc);
        scnNode[SNZTCRS2].AttachSimInt("laminar/B738/autopilot/course_copilot");
        scnNode[SNZTCRS2].InitTouchFuncAll(&SelectApCourse);
        scnNode[SNZTCRS2].InitFmtCtrl("%03d");

        scnNode[SNZTMAXB].InitNexCon(2, "Tmb.txt");                    // Max bank text
        scnNode[SNZTMAXB].InitNexType(AMK_INPUT | AMK_SELECT | AMK_SENDDIRECT);
        scnNode[SNZTMAXB].AttachSimInt("laminar/B738/autopilot/bank_angle_pos", XPL_READ, 120);
        scnNode[SNZTMAXB].AttachCommand("laminar/B738/autopilot/bank_angle_dn", 1);
        scnNode[SNZTMAXB].AttachCommand("laminar/B738/autopilot/bank_angle_up", 2);
        scnNode[SNZTMAXB].InitTouchFuncAll(&SelectApCourse);
        scnNode[SNZTMAXB].InitDispFuncXP(&ShowMaxBank_ZB);

        scnNode[SNZTABRK].InitNexCon(2, "Tab.txt");                    // Autobrake text
        scnNode[SNZTABRK].InitNexType(AMK_INPUT | AMK_SELECT | AMK_SENDDIRECT);
        scnNode[SNZTABRK].AttachSimInt("laminar/B738/autobrake/autobrake_pos", XPL_READ, 120);
        scnNode[SNZTABRK].AttachCommand("laminar/B738/knob/autobrake_dn", 1);
        scnNode[SNZTABRK].AttachCommand("laminar/B738/knob/autobrake_up", 2);
        scnNode[SNZTABRK].InitTouchFuncAll(&SelectApCourse);
        scnNode[SNZTABRK].InitDispFuncXP(&ShowAutobrake_ZB);

        scnNode[SNZBCMDA].InitNexCon(0x10, "Pa1.pic");                // CMD A button
        scnNode[SNZBCMDA].InitNexType(AMK_INPUT | AMK_SENDDIRECT);
        scnNode[SNZBCMDA].InitOperands(IMG_APLITE0);                      // Pic ID
        scnNode[SNZBCMDA].AttachSimInt("laminar/B738/autopilot/cmd_a_status", XPL_READ, 200);
        scnNode[SNZBCMDA].AttachCommand("laminar/B738/autopilot/cmd_a_press", 0);

        scnNode[SNZBCMDB].InitNexCon(0x11, "Pa2.pic");                // CMD B button
        scnNode[SNZBCMDB].InitNexType(AMK_INPUT | AMK_SENDDIRECT);
        scnNode[SNZBCMDB].InitOperands(IMG_APLITE0);                      // Pic ID
        scnNode[SNZBCMDB].AttachSimInt("laminar/B738/autopilot/cmd_b_status", XPL_READ, 200);
        scnNode[SNZBCMDB].AttachCommand("laminar/B738/autopilot/cmd_b_press", 0);

        scnNode[SNZBATAR].InitNexCon(9, "Pat.pic");                   // AT Arm switch
        scnNode[SNZBATAR].InitNexType(AMK_INPUT | AMK_SENDDIRECT);
        scnNode[SNZBATAR].InitOperands(IMG_APLITE0);                        // Pic ID
        scnNode[SNZBATAR].AttachSimInt("laminar/B738/autopilot/autothrottle_arm_pos", XPL_READ, 200);
        scnNode[SNZBATAR].AttachCommand("laminar/B738/autopilot/autothrottle_arm_toggle", 0);

        scnNode[SNZBVNAV].InitNexCon(0xA, "Pvn.pic");                 // VNAV pressed
        scnNode[SNZBVNAV].InitNexType(AMK_INPUT | AMK_SENDDIRECT);
        scnNode[SNZBVNAV].InitOperands(IMG_APLITE0);
        scnNode[SNZBVNAV].AttachSimInt("laminar/B738/autopilot/vnav_status1", XPL_READ, 250);
        scnNode[SNZBVNAV].AttachCommand("laminar/B738/autopilot/vnav_press", 0);

        scnNode[SNZBVORL].InitNexCon(0xC, "Pvo.pic");                 // VOR/LOC button
        scnNode[SNZBVORL].InitNexType(AMK_INPUT | AMK_SENDDIRECT);
        scnNode[SNZBVORL].InitOperands(IMG_APLITE0);
        scnNode[SNZBVORL].AttachSimInt("laminar/B738/autopilot/vorloc_status", XPL_READ, 200);
        scnNode[SNZBVORL].AttachCommand("laminar/B738/autopilot/vorloc_press", 0);

        scnNode[SNZBLNAV].InitNexCon(0xD, "Pln.pic");                 // LNAV button
        scnNode[SNZBLNAV].InitNexType(AMK_INPUT | AMK_SENDDIRECT);
        scnNode[SNZBLNAV].InitOperands(IMG_APLITE0);
        scnNode[SNZBLNAV].AttachSimInt("laminar/B738/autopilot/lnav_status", XPL_READ, 250);
        scnNode[SNZBLNAV].AttachCommand("laminar/B738/autopilot/lnav_press", 0);

        scnNode[SNZBATN1].InitNexCon(0x13, "Pn1.pic");                // N1 button
        scnNode[SNZBATN1].InitNexType(AMK_INPUT | AMK_SENDDIRECT);
        scnNode[SNZBATN1].InitOperands(IMG_APLITE0);
        scnNode[SNZBATN1].AttachSimInt("laminar/B738/autopilot/n1_status", XPL_READ, 250);
        scnNode[SNZBATN1].AttachCommand("laminar/B738/autopilot/n1_press", 0);

        scnNode[SNZBLVLC].InitNexCon(0x14, "Plv.pic");                // LvlChg button
        scnNode[SNZBLVLC].InitNexType(AMK_INPUT | AMK_SENDDIRECT);
        scnNode[SNZBLVLC].InitOperands(IMG_APLITE0);
        scnNode[SNZBLVLC].AttachSimInt("laminar/B738/autopilot/lvl_chg_status", XPL_READ, 250);
        scnNode[SNZBLVLC].AttachCommand("laminar/B738/autopilot/lvl_chg_press", 0);

        scnNode[SNZBAPPR].InitNexCon(0x15, "Ppp.pic");                // Appr button
        scnNode[SNZBAPPR].InitNexType(AMK_INPUT | AMK_SENDDIRECT);
        scnNode[SNZBAPPR].InitOperands(IMG_APLITE0);
        scnNode[SNZBAPPR].AttachSimInt("laminar/B738/autopilot/app_status", XPL_READ, 250);
        scnNode[SNZBAPPR].AttachCommand("laminar/B738/autopilot/app_press", 0);

        scnNode[SNZBAINV].InitNexCon(0x17, "Pai.pic");                // Alt Intv button
        scnNode[SNZBAINV].InitNexType(AMK_SENDDIRECT);
        scnNode[SNZBAINV].AttachCommand("laminar/B738/autopilot/alt_interv", 0);

        scnNode[SNZBFDR1].InitNexCon(0x24, "Pf1.pic");                // Flt director1 button
        scnNode[SNZBFDR1].InitNexType(AMK_INPUT | AMK_SENDDIRECT);
        scnNode[SNZBFDR1].InitOperands(IMG_APLITE0);
        scnNode[SNZBFDR1].AttachSimInt("laminar/B738/autopilot/flight_director_pos", XPL_READ, 200);
        scnNode[SNZBFDR1].AttachCommand("laminar/B738/autopilot/flight_director_toggle", 0);

        scnNode[SNZBFDR2].InitNexCon(0x2A, "Pf2.pic");                // Flt director2 button
        scnNode[SNZBFDR2].InitNexType(AMK_INPUT | AMK_SENDDIRECT);
        scnNode[SNZBFDR2].InitOperands(IMG_APLITE0);
        scnNode[SNZBFDR2].AttachSimInt("laminar/B738/autopilot/flight_director_fo_pos", XPL_READ, 200);
        scnNode[SNZBFDR2].AttachCommand("laminar/B738/autopilot/flight_director_fo_toggle", 0);

        scnNode[SNZBSPD1].InitNexCon(0x25, "Psp.pic");                // Speed pressed
        scnNode[SNZBSPD1].InitNexType(AMK_INPUT | AMK_XPPRESSED);
        scnNode[SNZBSPD1].InitOperands(IMG_APLITE0);
        scnNode[SNZBSPD1].AttachSimInt("laminar/B738/autopilot/speed_status1", XPL_READ, 200);
        scnNode[SNZBSPD1].AttachCommand("laminar/B738/autopilot/speed_press", 0);

        scnNode[SNZBSPD0].InitNexCon(0x26);                           // Speed released
        scnNode[SNZBSPD0].InitNexType(AMK_INPUT | AMK_XPRELEASED);
        scnNode[SNZBSPD0].AttachCommand("laminar/B738/autopilot/speed_press", 0);

        scnNode[SNZBHDNG].InitNexCon(0x27, "Phd.pic");                // Heading button
        scnNode[SNZBHDNG].InitNexType(AMK_INPUT | AMK_SENDDIRECT);
        scnNode[SNZBHDNG].InitOperands(IMG_APLITE0);
        scnNode[SNZBHDNG].AttachSimInt("sim/cockpit2/autopilot/heading_status", XPL_READ, 200);
        scnNode[SNZBHDNG].AttachCommand("laminar/B738/autopilot/hdg_sel_press", 0);
        scnNode[SNZBHDNG].InitDispFuncXP(&XP_Show01Button);

        scnNode[SNZBALTI].InitNexCon(0x28, "Pal.pic");                // AltHold button
        scnNode[SNZBALTI].InitNexType(AMK_INPUT | AMK_SENDDIRECT);
        scnNode[SNZBALTI].InitOperands(IMG_APLITE0);
        scnNode[SNZBALTI].AttachSimInt("laminar/B738/autopilot/alt_hld_status", XPL_READ, 200);
        scnNode[SNZBALTI].AttachCommand("laminar/B738/autopilot/alt_hld_press", 0);

        scnNode[SNZBVSPD].InitNexCon(0x29, "Pvs.pic");                // VS button
        scnNode[SNZBVSPD].InitNexType(AMK_INPUT | AMK_SENDDIRECT);
        scnNode[SNZBVSPD].InitOperands(IMG_APLITE0);
        scnNode[SNZBVSPD].AttachSimInt("laminar/B738/autopilot/vs_status", XPL_READ, 200);
        scnNode[SNZBVSPD].AttachCommand("laminar/B738/autopilot/vs_press", 0);

        scnNode[SNZBSINV].InitNexCon(0x2F, "Psi.pic");                // Spd Intv button
        scnNode[SNZBSINV].InitNexType(AMK_SENDDIRECT);
        scnNode[SNZBSINV].AttachCommand("laminar/B738/autopilot/spd_interv", 0);

        scnNode[SNZBRAP1].InitNexCon(0x1A, "Pra.pic");                // Reset AP discon pressed
        scnNode[SNZBRAP1].InitNexType(AMK_INPUT | AMK_XPPRESSED);
        scnNode[SNZBRAP1].InitOperands(IMG_RSTAP_WARNS);
        scnNode[SNZBRAP1].AttachSimInt("laminar/B738/annunciator/ap_disconnect", XPL_READ, 255);
        scnNode[SNZBRAP1].AttachCommand("laminar/B738/push_button/ap_light_pilot", 0);
        scnNode[SNZBRAP1].InitDispFuncXP(&ShowApWarnLights_ZB);

        scnNode[SNZBRAP0].InitNexCon(0x1B, "Pra.pic");                // Reset AP released
        scnNode[SNZBRAP0].InitNexType(AMK_INPUT | AMK_XPRELEASED);    //  the Pressed node INDEX above
        scnNode[SNZBRAP0].AttachSimInt("laminar/B738/annunciator/ap_warn1", XPL_READ, 255);
        scnNode[SNZBRAP0].AttachCommand("laminar/B738/push_button/ap_light_pilot", 0);
        scnNode[SNZBRAP0].InitDispFuncXP(&ShowApWarnLights_ZB);

        scnNode[SNZBRAT1].InitNexCon(0x1C, "Prt.pic");                // Reset AT discon pressed
        scnNode[SNZBRAT1].InitNexType(AMK_INPUT | AMK_XPPRESSED);
        scnNode[SNZBRAT1].InitOperands(IMG_RSTAT_WARNS);
        scnNode[SNZBRAT1].AttachSimInt("laminar/B738/annunciator/at_disconnect", XPL_READ, 255);
        scnNode[SNZBRAT1].AttachCommand("laminar/B738/push_button/at_light_pilot", 0);
        scnNode[SNZBRAT1].InitDispFuncXP(&ShowApWarnLights_ZB);

        scnNode[SNZBRAT0].InitNexCon(0x1D, "Prt.pic");                // Reset AT released
        scnNode[SNZBRAT0].InitNexType(AMK_INPUT | AMK_XPRELEASED);    //  the Pressed node INDEX above
        scnNode[SNZBRAT0].AttachSimInt("laminar/B738/annunciator/at_fms_warn1", XPL_READ, 255);
        scnNode[SNZBRAT0].AttachCommand("laminar/B738/push_button/at_light_pilot", 0);
        scnNode[SNZBRAT0].InitDispFuncXP(&ShowApWarnLights_ZB);

        scnNode[SNZBRSFM].InitNexCon(0x1E, "Prf.pic");                // Reset FMC warn
        scnNode[SNZBRSFM].InitNexType(AMK_INPUT | AMK_SENDDIRECT);
        scnNode[SNZBRSFM].InitOperands(IMG_RSTFM_WARNS);
        scnNode[SNZBRSFM].AttachSimInt("laminar/B738/annunciator/at_fms_disconnect1", XPL_READ, 400);
        scnNode[SNZBRSFM].AttachCommand("laminar/B738/push_button/fms_light_pilot", 0);
    }


// Autopilot controls for the ToLiss profile.  XP-only.

    if (IsToLissProfile())         // ToLiss-only
    {
        scnNode[SNTMSELH].InitNexCon(1);                              // AP row select hotspot
        scnNode[SNTMSELH].InitNexType(AMK_SELECT);
        scnNode[SNTMSELH].InitTouchFuncXP(&SelectApPrimary);

        scnNode[SNZTSPED].InitNexCon(0, "Tsp.txt");                   // SPD text
        scnNode[SNZTSPED].InitNexType(AMK_INPUT | AMK_SELECT);
        scnNode[SNZTSPED].InitOperands(OD_ApSpeed_Dec, OD_ApSpeed_Inc);
        scnNode[SNZTSPED].AttachSimFloat("sim/cockpit2/autopilot/airspeed_dial_kts_mach", XPL_READWRITE, 100, 0.01);
        scnNode[SNZTSPED].InitDispFuncXP(&ShowAutoSpeed_ZB);
 
        scnNode[SNZTHDNG].InitNexCon(0, "Thd.txt");                   // HDG text
        scnNode[SNZTHDNG].InitNexType(AMK_INPUT | AMK_SELECT);
        scnNode[SNZTHDNG].InitOperands(OD_ApHeading_Dec, OD_ApHeading_Inc);
        scnNode[SNZTHDNG].AttachSimInt("sim/cockpit2/autopilot/heading_dial_deg_mag_pilot");
        scnNode[SNZTHDNG].InitFmtCtrl("%03d");

        scnNode[SNZTALTI].InitNexCon(0, "Tal.txt");                   // ALT text
        scnNode[SNZTALTI].InitNexType(AMK_INPUT | AMK_SELECT);
        scnNode[SNZTALTI].InitOperands(OD_ApAltitude_Dec, OD_ApAltitude_Inc);
        scnNode[SNZTALTI].AttachSimInt("sim/cockpit2/autopilot/altitude_dial_ft");
        scnNode[SNZTALTI].InitFmtCtrl("%5d");

        scnNode[SNZTVSPD].InitNexCon(0, "Tvs.txt");                   // VS text
        scnNode[SNZTVSPD].InitNexType(AMK_INPUT | AMK_SELECT | AMK_SENDDIRECT);
        scnNode[SNZTVSPD].AttachCommand("AirbusFBW/PushVSSel", 0);
        scnNode[SNZTVSPD].AttachCommand("sim/autopilot/vertical_speed_down", 1);
        scnNode[SNZTVSPD].AttachCommand("sim/autopilot/vertical_speed_up", 2);
        scnNode[SNZTVSPD].AttachSimInt("sim/cockpit2/autopilot/vvi_dial_fpm");
        scnNode[SNZTVSPD].InitFmtCtrl("%+d");

        scnNode[SNABSPED].InitNexCon(0);                              // SPD managed encoder button
        scnNode[SNABSPED].InitNexType(AMK_INPUT | AMK_SENDDIRECT);
        scnNode[SNABSPED].AttachCommand("AirbusFBW/PushSPDSel", 0);

        scnNode[SNABSPSY].InitNexCon(0);                              // SPD Sel Long-Touch
        scnNode[SNABSPSY].InitNexType(AMK_SENDDIRECT);
        scnNode[SNABSPSY].AttachCommand("AirbusFBW/PullSPDSel", 0);

        scnNode[SNABHDNG].InitNexCon(0);                              // Hdg managed encoder push
        scnNode[SNABHDNG].InitNexType(AMK_INPUT | AMK_SENDDIRECT);
        scnNode[SNABHDNG].AttachCommand("AirbusFBW/PushHDGSel", 0);

        scnNode[SNABHDSY].InitNexCon(0);                              // Hdg Sel Long-Touch
        scnNode[SNABHDSY].InitNexType(AMK_SENDDIRECT);
        scnNode[SNABHDSY].AttachCommand("AirbusFBW/PullHDGSel", 0);

        scnNode[SNABALTI].InitNexCon(0);                              // Alt managed encoder push
        scnNode[SNABALTI].InitNexType(AMK_INPUT | AMK_SENDDIRECT);
        scnNode[SNABALTI].AttachCommand("AirbusFBW/PushAltitude", 0);

        scnNode[SNABALSY].InitNexCon(0);                              // Alt Sel Long-Touch
        scnNode[SNABALSY].InitNexType(AMK_SENDDIRECT);
        scnNode[SNABALSY].AttachCommand("AirbusFBW/PullAltitude", 0);

        scnNode[SNABVSSY].InitNexCon(0);                              // VS Sel Long-Touch
        scnNode[SNABVSSY].InitNexType(AMK_SENDDIRECT);
        scnNode[SNABVSSY].AttachCommand("AirbusFBW/PullVSSel", 0);

        scnNode[SNZBFDR1].InitNexCon(0xB, "Pf1.pic");                 // Flt director1 button
        scnNode[SNZBFDR1].InitNexType(AMK_INPUT | AMK_DATAUPDATE);
        scnNode[SNZBFDR1].InitOperands(IMG_APLITE0);
        scnNode[SNZBFDR1].AttachSimInt("AirbusFBW/FD1Engage", XPL_READWRITE, 200);
        scnNode[SNZBFDR1].InitTouchFuncXP(&ToggleBinarySwitch);

        scnNode[SNZBATAR].InitNexCon(0xC, "Pat.pic");                 // A/THR button
        scnNode[SNZBATAR].InitNexType(AMK_INPUT | AMK_SENDDIRECT);
        scnNode[SNZBATAR].InitOperands(IMG_APLITE0);
        scnNode[SNZBATAR].AttachCommand("AirbusFBW/ATHRbutton", 0);
        scnNode[SNZBATAR].AttachSimInt("AirbusFBW/ATHRmode", XPL_READ, 250);
        scnNode[SNZBATAR].InitDispFuncXP(&ShowAutoThrMode_TL);

        scnNode[SNZBCMDA].InitNexCon(0xD, "Pa1.pic");                 // AP1 button
        scnNode[SNZBCMDA].InitNexType(AMK_INPUT | AMK_DATAUPDATE);
        scnNode[SNZBCMDA].InitOperands(IMG_APLITE0);
        scnNode[SNZBCMDA].AttachSimInt("AirbusFBW/AP1Engage", XPL_READWRITE, 200);
        scnNode[SNZBCMDA].InitTouchFuncXP(&ToggleBinarySwitch);

        scnNode[SNZBCMDB].InitNexCon(0xE, "Pa2.pic");                 // AP2 button
        scnNode[SNZBCMDB].InitNexType(AMK_INPUT | AMK_DATAUPDATE);
        scnNode[SNZBCMDB].InitOperands(IMG_APLITE0);
        scnNode[SNZBCMDB].AttachSimInt("AirbusFBW/AP2Engage", XPL_READWRITE, 200);
        scnNode[SNZBCMDB].InitTouchFuncXP(&ToggleBinarySwitch);

        scnNode[SNZBFDR2].InitNexCon(0xF, "Pf2.pic");                 // Flt director2 button
        scnNode[SNZBFDR2].InitNexType(AMK_INPUT | AMK_DATAUPDATE);
        scnNode[SNZBFDR2].InitOperands(IMG_APLITE0);
        scnNode[SNZBFDR2].AttachSimInt("AirbusFBW/FD2Engage", XPL_READWRITE, 200);
        scnNode[SNZBFDR2].InitTouchFuncXP(&ToggleBinarySwitch);

        scnNode[SNTBLSON].InitNexCon(0x14, "Pls.pic");                // LS button
        scnNode[SNTBLSON].InitNexType(AMK_INPUT | AMK_DATAUPDATE);
        scnNode[SNTBLSON].InitOperands(IMG_APLITE0);
        scnNode[SNTBLSON].AttachSimInt("AirbusFBW/ILSonCapt", XPL_READWRITE, 200);
        scnNode[SNTBLSON].InitTouchFuncXP(&ToggleBinarySwitch);

        scnNode[SNZBVORL].InitNexCon(0x15, "Plo.pic");                // LOC button
        scnNode[SNZBVORL].InitNexType(AMK_INPUT | AMK_SENDDIRECT);
        scnNode[SNZBVORL].InitOperands(IMG_APLITE0);
        scnNode[SNZBVORL].AttachCommand("AirbusFBW/LOCbutton", 0);
        scnNode[SNZBVORL].AttachSimInt("AirbusFBW/LOCilluminated", XPL_READ, 200);

        scnNode[SNZBLVLC].InitNexCon(0x16, "Pex.pic");                // EXPED button
        scnNode[SNZBLVLC].InitNexType(AMK_INPUT | AMK_SENDDIRECT);
        scnNode[SNZBLVLC].InitOperands(IMG_APLITE0);
        scnNode[SNZBLVLC].AttachCommand("AirbusFBW/EXPEDbutton", 0);
        scnNode[SNZBLVLC].AttachSimInt("AirbusFBW/ATHRmode2", XPL_READ, 200);
        scnNode[SNZBLVLC].InitDispFuncXP(&ShowAutoThrMode_TL);

        scnNode[SNZBAPPR].InitNexCon(0x17, "Pap.pic");                // APPRS button
        scnNode[SNZBAPPR].InitNexType(AMK_INPUT | AMK_SENDDIRECT);
        scnNode[SNZBAPPR].InitOperands(IMG_APLITE0);
        scnNode[SNZBAPPR].AttachCommand("AirbusFBW/APPRbutton", 0);
        scnNode[SNZBAPPR].AttachSimInt("AirbusFBW/APPRilluminated", XPL_READ, 200);

        scnNode[SNTBWARN].InitNexCon(0xA, "Pmw.pic");                 // Clear warn button
        scnNode[SNTBWARN].InitNexType(AMK_INPUT | AMK_SENDDIRECT);
        scnNode[SNTBWARN].InitOperands(IMG_TL_MSTWARN0);
        scnNode[SNTBWARN].AttachSimInt("AirbusFBW/MasterWarn", XPL_READ, 250);
        scnNode[SNTBWARN].AttachCommand("sim/annunciator/clear_master_warning", 0);

        scnNode[SNTBCAUT].InitNexCon(0x13, "Pmc.pic");                // Clear caution button
        scnNode[SNTBCAUT].InitNexType(AMK_INPUT | AMK_SENDDIRECT);
        scnNode[SNTBCAUT].InitOperands(IMG_TL_MSTCAUT0);
        scnNode[SNTBCAUT].AttachSimInt("AirbusFBW/MasterCaut", XPL_READ, 250);
        scnNode[SNTBCAUT].AttachCommand("sim/annunciator/clear_master_caution", 0);

        scnNode[SNTBGPWS].InitNexCon(0x1F, "Pgp.pic");                // Clear GPWS button
        scnNode[SNTBGPWS].InitNexType(AMK_INPUT | AMK_SENDDIRECT);
        scnNode[SNTBGPWS].InitOperands(IMG_TL_GPWS0);
        scnNode[SNTBGPWS].AttachSimInt("AirbusFBW/fmod/spkr/gpwsCont", XPL_READ, 250);
        scnNode[SNTBGPWS].AttachCommand("AirbusFBW/GPWSMute", 0);
        scnNode[SNTBGPWS].InitDispFuncXP(&ShowGpwsLight_TL);

        scnNode[SNTMSELL].InitNexCon(6);                              // EFIS row select hotspot
        scnNode[SNTMSELL].InitNexType(AMK_SELECT);
        scnNode[SNTMSELL].InitTouchFuncXP(&SelectEFIS_TL);

        scnNode[SNZKMAPM].InitNexCon(0, "Tem.txt");                   // EFIS mode text
        scnNode[SNZKMAPM].InitNexType(AMK_INPUT | AMK_SELECT | AMK_SENDDIRECT);
        scnNode[SNZKMAPM].AttachCommand("sim/instruments/EFIS_mode_dn", 1);
        scnNode[SNZKMAPM].AttachCommand("sim/instruments/EFIS_mode_up", 2);
        scnNode[SNZKMAPM].AttachSimInt("AirbusFBW/NDmodeCapt", XPL_READ, 250);
        scnNode[SNZKMAPM].InitDispFuncXP(&ShowEfisMode_ZB);
        scnNode[SNZKMAPM].InitFmtCtrl("%1d");

        scnNode[SNZKRANG].InitNexCon(0, "Tra.txt");                   // Map range text
        scnNode[SNZKRANG].InitNexType(AMK_INPUT | AMK_SELECT | AMK_SENDDIRECT);
        scnNode[SNZKRANG].AttachCommand("sim/instruments/map_zoom_in", 1);
        scnNode[SNZKRANG].AttachCommand("sim/instruments/map_zoom_out", 2);
        scnNode[SNZKRANG].AttachSimInt("AirbusFBW/NDrangeCapt", XPL_READ, 250);
        scnNode[SNZKRANG].InitDispFuncXP(&ShowMapRange_TL);
        scnNode[SNZKRANG].InitFmtCtrl("%3d");

        scnNode[SNZTBARO].InitNexCon(0, "Tba.txt");                   // Barometer text
        scnNode[SNZTBARO].InitNexType(AMK_INPUT | AMK_SELECT | AMK_SENDDIRECT);
        scnNode[SNZTBARO].AttachSimFloat("sim/cockpit/misc/barometer_setting", XPL_READWRITE, 100, 0.01);
        scnNode[SNZTBARO].AttachCommand("toliss_airbus/capt_baro_push", 0);
        scnNode[SNZTBARO].AttachCommand("sim/instruments/barometer_down", 1);
        scnNode[SNZTBARO].AttachCommand("sim/instruments/barometer_up", 2);
        scnNode[SNZTBARO].InitDispFuncXP(&ShowBarometerText);

        scnNode[SNTBBAPL].InitNexCon(0);                              // Baro Long-press
        scnNode[SNTBBAPL].InitNexType(AMK_SENDDIRECT);
        scnNode[SNTBBAPL].AttachCommand("toliss_airbus/capt_baro_pull", 0);

        scnNode[SNTBABLO].InitNexCon(0x2B, "Pb1.pic");                // Autobrake Lo
        scnNode[SNTBABLO].InitNexType(AMK_INPUT | AMK_SENDDIRECT);
        scnNode[SNTBABLO].InitOperands(IMG_TL_BLULITE0);
        scnNode[SNTBABLO].AttachSimInt("AirbusFBW/AutoBrkLo", XPL_READ, 250);
        scnNode[SNTBABLO].AttachCommand("AirbusFBW/AbrkLo", 0);

        scnNode[SNTBABMI].InitNexCon(0x2C, "Pb2.pic");                // Autobrake Med
        scnNode[SNTBABMI].InitNexType(AMK_INPUT | AMK_SENDDIRECT);
        scnNode[SNTBABMI].InitOperands(IMG_TL_BLULITE0);
        scnNode[SNTBABMI].AttachSimInt("AirbusFBW/AutoBrkMed", XPL_READ, 250);
        scnNode[SNTBABMI].AttachCommand("AirbusFBW/AbrkMed", 0);

        scnNode[SNTBABMX].InitNexCon(0x2D, "Pb3.pic");                // Autobrake Max
        scnNode[SNTBABMX].InitNexType(AMK_INPUT | AMK_SENDDIRECT);
        scnNode[SNTBABMX].InitOperands(IMG_TL_BLULITE0);
        scnNode[SNTBABMX].AttachSimInt("AirbusFBW/AutoBrkMax", XPL_READ, 250);
        scnNode[SNTBABMX].AttachCommand("AirbusFBW/AbrkMax", 0);

        scnNode[SNTBCSTR].InitNexCon(0x20, "Pcs.pic");                // CSTR button
        scnNode[SNTBCSTR].InitNexType(AMK_INPUT | AMK_DATAUPDATE);
        scnNode[SNTBCSTR].InitOperands(IMG_TL_NDLITE0);
        scnNode[SNTBCSTR].AttachSimInt("AirbusFBW/NDShowCSTRCapt", XPL_READWRITE, 270);
        scnNode[SNTBCSTR].InitTouchFuncXP(&ToggleBinarySwitch);

        scnNode[SNTBWYPT].InitNexCon(0x21, "Pwp.pic");                // WPT button
        scnNode[SNTBWYPT].InitNexType(AMK_INPUT | AMK_DATAUPDATE);
        scnNode[SNTBWYPT].InitOperands(IMG_TL_NDLITE0);
        scnNode[SNTBWYPT].AttachSimInt("AirbusFBW/NDShowWPTCapt", XPL_READWRITE, 270);
        scnNode[SNTBWYPT].InitTouchFuncXP(&ToggleBinarySwitch);

        scnNode[SNTBVORD].InitNexCon(0x22, "Pvo.pic");                // VORD button
        scnNode[SNTBVORD].InitNexType(AMK_INPUT | AMK_DATAUPDATE);
        scnNode[SNTBVORD].InitOperands(IMG_TL_NDLITE0);
        scnNode[SNTBVORD].AttachSimInt("AirbusFBW/NDShowVORDCapt", XPL_READWRITE, 270);
        scnNode[SNTBVORD].InitTouchFuncXP(&ToggleBinarySwitch);

        scnNode[SNTBNDBN].InitNexCon(0x23, "Pnd.pic");                // NDB button
        scnNode[SNTBNDBN].InitNexType(AMK_INPUT | AMK_DATAUPDATE);
        scnNode[SNTBNDBN].InitOperands(IMG_TL_NDLITE0);
        scnNode[SNTBNDBN].AttachSimInt("AirbusFBW/NDShowNDBCapt", XPL_READWRITE, 270);
        scnNode[SNTBNDBN].InitTouchFuncXP(&ToggleBinarySwitch);

        scnNode[SNTBARPT].InitNexCon(0x24, "Par.pic");                // ARPT button
        scnNode[SNTBARPT].InitNexType(AMK_INPUT | AMK_DATAUPDATE);
        scnNode[SNTBARPT].InitOperands(IMG_TL_NDLITE0);
        scnNode[SNTBARPT].AttachSimInt("AirbusFBW/NDShowARPTCapt", XPL_READWRITE, 270);
        scnNode[SNTBARPT].InitTouchFuncXP(&ToggleBinarySwitch);

        scnNode[SNTBHTRK].InitNexCon(0x18);                          // Hdg/Trk button
        scnNode[SNTBHTRK].InitNexType(AMK_INPUT | AMK_DATAUPDATE);
        scnNode[SNTBHTRK].AttachSimInt("AirbusFBW/HDGTRKmode", XPL_READWRITE, 400);
        scnNode[SNTBHTRK].InitTouchFuncXP(&ToggleBinarySwitch);

        scnNode[SNZKBAIN].InitNexCon(0x25);                            // Hg/hPa button
        scnNode[SNZKBAIN].InitNexType(AMK_INPUT | AMK_DATAUPDATE);
        scnNode[SNZKBAIN].AttachSimInt("AirbusFBW/BaroUnitCapt", XPL_READWRITE, 400);
        scnNode[SNZKBAIN].InitTouchFuncXP(&ToggleBinarySwitch);

        scnNode[SNTBTERR].InitNexCon(0x2A, "Pte.pic");                // Terrain button
        scnNode[SNTBTERR].InitNexType(AMK_INPUT | AMK_SENDDIRECT);
        scnNode[SNTBTERR].InitOperands(IMG_TL_BLULITE0);
        scnNode[SNTBTERR].AttachCommand("toliss_airbus/dispcommands/TerrOnND1Toggle", 0);
        scnNode[SNTBTERR].AttachSimInt("AirbusFBW/TerrainSelectedND1", XPL_READ, 400);
}


// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
// PAGE: NAVIGATE
// Zibo Navigation Display

    // Navigation Display controls for the Zibo profile.  XP-only.
    if (IsZiboProfile())           // Zibo-only
    {
        scnNode[SNZKMAPM].InitNexCon(0xC, "Tmm.txt");                 // Map mode text
        scnNode[SNZKMAPM].InitNexType(AMK_INPUT | AMK_SENDDIRECT);
        scnNode[SNZKMAPM].AttachSimInt("laminar/B738/EFIS_control/capt/map_mode_pos", XPL_READ, 150);
        scnNode[SNZKMAPM].AttachCommand("laminar/B738/EFIS_control/capt/map_mode_dn", 1);
        scnNode[SNZKMAPM].AttachCommand("laminar/B738/EFIS_control/capt/map_mode_up", 2);
        scnNode[SNZKMAPM].InitDispFuncXP(&ShowMapMode_ZB);

        scnNode[SNZKRANG].InitNexCon(0xD, "Tra.txt");                 // Range text
        scnNode[SNZKRANG].InitNexType(AMK_INPUT | AMK_SENDDIRECT);
        scnNode[SNZKRANG].AttachSimInt("laminar/B738/EFIS/capt/map_range", XPL_READ, 120);
        scnNode[SNZKRANG].AttachCommand("laminar/B738/EFIS_control/capt/map_range_dn", 1);
        scnNode[SNZKRANG].AttachCommand("laminar/B738/EFIS_control/capt/map_range_up", 2);
        scnNode[SNZKRANG].InitDispFuncXP(&ShowNdRange_ZB);

        scnNode[SNZKMIRA].InitNexCon(0xA);                            // Radio button
        scnNode[SNZKMIRA].InitNexType(AMK_SENDDIRECT);
        scnNode[SNZKMIRA].AttachCommand("laminar/B738/EFIS_control/cpt/minimums_up", 0);

        scnNode[SNZKMIBA].InitNexCon(0xB);                            // Baro button
        scnNode[SNZKMIBA].InitNexType(AMK_SENDDIRECT);
        scnNode[SNZKMIBA].AttachCommand("laminar/B738/EFIS_control/cpt/minimums_dn", 0);

        scnNode[SNZKMRST].InitNexCon(2, "Trs.txt");                   // RST button, Minimum text
        scnNode[SNZKMRST].InitNexType(AMK_INPUT | AMK_SENDDIRECT);
        scnNode[SNZKMRST].AttachSimInt("laminar/B738/pfd/dh_pilot", XPL_READ, 100);
        scnNode[SNZKMRST].AttachCommand("laminar/B738/EFIS_control/capt/push_button/rst_press", 0);
        scnNode[SNZKMRST].AttachCommand("laminar/B738/pfd/dh_pilot_dn", 1);
        scnNode[SNZKMRST].AttachCommand("laminar/B738/pfd/dh_pilot_up", 2);
        scnNode[SNZKMRST].InitFmtCtrl("%d");

        scnNode[SNZKMCTR].InitNexCon(3);                              // CTR button
        scnNode[SNZKMCTR].InitNexType(AMK_SENDDIRECT);
        scnNode[SNZKMCTR].AttachCommand("laminar/B738/EFIS_control/capt/push_button/ctr_press", 0);

        scnNode[SNZKRTFC].InitNexCon(4);                              // TFC button
        scnNode[SNZKRTFC].InitNexType(AMK_SENDDIRECT);
        scnNode[SNZKRTFC].AttachCommand("laminar/B738/EFIS_control/capt/push_button/tfc_press", 0);

        scnNode[SNZKBSTD].InitNexCon(5);                              // STD button
        scnNode[SNZKBSTD].InitNexType(AMK_SENDDIRECT);  // Button only, rotary shared from Navigate page
        scnNode[SNZKBSTD].AttachCommand("laminar/B738/EFIS_control/capt/push_button/std_press", 0);

        scnNode[SNZKBAIN].InitNexCon(0xE);                            // Hg button
        scnNode[SNZKBAIN].InitNexType(AMK_SENDDIRECT);
        scnNode[SNZKBAIN].AttachCommand("laminar/B738/EFIS_control/capt/baro_in_hpa_dn", 0);

        scnNode[SNZKBAHP].InitNexCon(0xF);                            // HPA button
        scnNode[SNZKBAHP].InitNexType(AMK_SENDDIRECT);
        scnNode[SNZKBAHP].AttachCommand("laminar/B738/EFIS_control/capt/baro_in_hpa_up", 0);

        scnNode[SNZBNSKN].InitNexCon(1);                              // Option array
        scnNode[SNZBNSKN].InitNexType(AMK_SENDDIRECT);                //  of 7 elements
        scnNode[SNZBNSKN].InitSimIds(0, 7);
        scnNode[SNZBNSKN].SetValue((int32_t) &cmd_ZB_Softkey);
        scnNode[SNZBNSKN].InitTouchFuncXP(&XP_SendOpcodeFromArray);
        // The command attribute of this node will receive the actual command handle during runtime
        cmd_ZB_Softkey[0] = XPDplugin.registerCommand("laminar/B738/EFIS_control/capt/push_button/wxr_press");
        cmd_ZB_Softkey[1] = XPDplugin.registerCommand("laminar/B738/EFIS_control/capt/push_button/sta_press");
        cmd_ZB_Softkey[2] = XPDplugin.registerCommand("laminar/B738/EFIS_control/capt/push_button/wpt_press");
        cmd_ZB_Softkey[3] = XPDplugin.registerCommand("laminar/B738/EFIS_control/capt/push_button/arpt_press");
        cmd_ZB_Softkey[4] = XPDplugin.registerCommand("laminar/B738/EFIS_control/capt/push_button/data_press");
        cmd_ZB_Softkey[5] = XPDplugin.registerCommand("laminar/B738/EFIS_control/capt/push_button/pos_press");
        cmd_ZB_Softkey[6] = XPDplugin.registerCommand("laminar/B738/EFIS_control/capt/push_button/terr_press");
    }


// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
// PAGE: RADIOS
// Communication frequency setting.

    // The Active and its Standby frequency screen objects must be grouped as follow
    scnNode[SNCTCOM1].InitNexCon(1, "Tc1.txt");                       // COM1 Freq text
    scnNode[SNCTCOM1].InitNexType(AMK_INPUT);
    scnNode[SNCTCOM1].InitSimIds(145, 1, 211);                          // Value, inx, Set
    scnNode[SNCTCOM1].InitOperands(216, 217);                           // Set Stby, Swap
    scnNode[SNCTCOM1].InitFmtCtrl("%7.3f");
    scnNode[SNCTCOM1].InitDispFuncXP(&XP_ShowComNavFreq);
    scnNode[SNCTCOM1].InitTouchFuncAll(&SwapFreqSelStdby);
    scnNode[SNCTCOM1].AttachSimFloat("sim/cockpit2/radios/actuators/com1_frequency_hz_833", XPL_READWRITE, 100, 0.001);

    scnNode[SNCTCOM2].InitNexCon(2, "Tc2.txt");                       // COM2 Freq text
    scnNode[SNCTCOM2].InitNexType(AMK_INPUT);
    scnNode[SNCTCOM2].InitSimIds(145, 2, 223);                          // Value, inx, Set
    scnNode[SNCTCOM2].InitOperands(227, 224);                           // Set Stby, Swap
    scnNode[SNCTCOM2].InitFmtCtrl("%7.3f");
    scnNode[SNCTCOM2].InitDispFuncXP(&XP_ShowComNavFreq);
    scnNode[SNCTCOM2].InitTouchFuncAll(&SwapFreqSelStdby);
    scnNode[SNCTCOM2].AttachSimFloat("sim/cockpit2/radios/actuators/com2_frequency_hz_833", XPL_READWRITE, 100, 0.001);

    scnNode[SNCTNAV1].InitNexCon(3, "Tn1.txt");                       // NAV1 Freq text
    scnNode[SNCTNAV1].InitNexType(AMK_INPUT);
    scnNode[SNCTNAV1].InitSimIds(502, 1, 565);                          // Value, inx, Set
    scnNode[SNCTNAV1].InitOperands(569, 566);                           // Set Stby, Swap
    scnNode[SNCTNAV1].InitFmtCtrl("%6.2f");
    scnNode[SNCTNAV1].InitDispFuncXP(&XP_ShowComNavFreq);
    scnNode[SNCTNAV1].InitTouchFuncAll(&SwapFreqSelStdby);
    scnNode[SNCTNAV1].AttachSimFloat("sim/cockpit/radios/nav1_freq_hz", XPL_READWRITE, 100, 0.01);

    scnNode[SNCTNAV2].InitNexCon(4, "Tn2.txt");                       // NAV2 Freq text
    scnNode[SNCTNAV2].InitNexType(AMK_INPUT);
    scnNode[SNCTNAV2].InitSimIds(502, 2, 574);                          // Value, inx, Set
    scnNode[SNCTNAV2].InitOperands(578, 575);                           // Set Stby, Swap
    scnNode[SNCTNAV2].InitFmtCtrl("%6.2f");
    scnNode[SNCTNAV2].InitDispFuncXP(&XP_ShowComNavFreq);
    scnNode[SNCTNAV2].InitTouchFuncAll(&SwapFreqSelStdby);
    scnNode[SNCTNAV2].AttachSimFloat("sim/cockpit/radios/nav2_freq_hz", XPL_READWRITE, 100, 0.01);

    scnNode[SNCTSTBY].InitNexCon(9, "T5.txt");                        // Standby Pool
    scnNode[SNCTSTBY].InitNexType(0);
    scnNode[SNCTSTBY].InitOperands(0, FRQ_COM);                         // Freq type
    scnNode[SNCTSTBY].InitTouchFuncAll(&StdbyFreqTouched);              // All standby touches come here,
    scnNode[SNCTSTBY].SetVarType(VT_FLOAT);                             //  long-touch will be determined
    scnNode[SNCTSTBY].InitFmtCtrl("%7.3f");                             //  in the function call

    scnNode[SNCTXPCO].InitNexCon(5, "Txp.txt");                       // XPDR Code text
    scnNode[SNCTXPCO].InitNexType(AMK_INPUT);
    scnNode[SNCTXPCO].InitSimIds(721, 1, 989);                          // Value, Set
    scnNode[SNCTXPCO].InitOperands(0, FRQ_XPDR);                        // Freq type
    scnNode[SNCTXPCO].InitTouchFuncAll(&KeypadInput);
    scnNode[SNCTXPCO].AttachSimInt("sim/cockpit2/radios/actuators/transponder_code", XPL_READWRITE);
    scnNode[SNCTXPCO].InitFmtCtrl("%4d");
    if (IsToLissProfile())       // Input inoperative for ToLiss, output okay
    {
        scnNode[SNCTXPCO].InitTouchFuncAll(NULL);
    }

    if (IsXPlane())
    {
        scnNode[SNCTXPMO].InitNexCon(7, "Txc.txt");                   // XPDR Mode text XP-only
        scnNode[SNCTXPMO].InitNexType(AMK_INPUT);
        scnNode[SNCTXPMO].InitDispFuncXP(&XP_ShowXponderMode);
        if (IsZiboProfile())
        {
            scnNode[SNCTXPMO].InitNexType(AMK_INPUT | AMK_SENDDIRECT);
            scnNode[SNCTXPMO].AttachCommand("laminar/B738/knob/transponder_mode_dn", 1);
            scnNode[SNCTXPMO].AttachCommand("laminar/B738/knob/transponder_mode_up", 2);
            scnNode[SNCTXPMO].AttachSimInt("laminar/B738/knob/transponder_pos");
        }
        else if (IsToLissProfile())       // Input inoperative for ToLiss, output okay
        {
            scnNode[SNCTXPMO].AttachSimInt("ckpt/transponder/mode/anim");
        }
        else        // XP Generic
        {
            scnNode[SNCTXPMO].InitOperands(OD_XpndrMode_Dec, OD_XpndrMode_Inc);
            scnNode[SNCTXPMO].AttachSimInt("sim/cockpit2/radios/actuators/transponder_mode");
        }

        scnNode[SNCBIDNT].InitNexCon(8, "Pid.pic");                   // Ident XP-only
        scnNode[SNCBIDNT].InitNexType(AMK_INPUT | AMK_SENDDIRECT);
        scnNode[SNCBIDNT].InitOperands(IMG_IDENT);
        scnNode[SNCBIDNT].AttachSimInt("sim/cockpit/radios/transponder_light", XPL_READ, 500);
        scnNode[SNCBIDNT].AttachCommand("sim/transponder/transponder_ident", 0);

        // Applicable for all X-Plane
        scnNode[SNZBXMAP].InitNexCon(0x34);                           // X-Plane's moving map
        scnNode[SNZBXMAP].InitNexType(AMK_SENDDIRECT);
        scnNode[SNZBXMAP].AttachCommand("sim/map/show_current", 0);
    }

    scnNode[SNCTADF1].InitNexCon(6, "Tad.txt");                       // ADF text
    scnNode[SNCTADF1].InitNexType(AMK_INPUT | AMK_DATAUPDATE);
    scnNode[SNCTADF1].InitSimIds(7, 1, 15);                             // Value, Set
    scnNode[SNCTADF1].InitOperands(CKC_ADFSWAP, FRQ_ADF);               // Keyboard bind, freq type
    scnNode[SNCTADF1].InitDispFuncMS(&MS_ShowAdfFreq);
    scnNode[SNCTADF1].InitTouchFuncAll(&KeypadInput);
    scnNode[SNCTADF1].AttachSimInt("sim/cockpit2/radios/actuators/adf1_frequency_hz", XPL_READWRITE, 500);
    scnNode[SNCTADF1].SetVarType(IsXPlane() ? VT_INT : VT_FLOAT);
    scnNode[SNCTADF1].InitFmtCtrl("%4d");

    if (!IsZiboProfile())
    {
        if (IsToLissProfile())       // ToLiss-only
        {
            scnNode[SNCBCOM1].InitNexCon(0xC, "Pc1.pic");                 // COM1 button
            scnNode[SNCBCOM1].InitNexType(AMK_INPUT | AMK_DATAUPDATE);
            scnNode[SNCBCOM1].InitOperands(IMG_COM1);                       // Pic ID
            scnNode[SNCBCOM1].AttachSimInt("sim/cockpit2/radios/actuators/audio_selection_com1", XPL_READWRITE, 150);
            scnNode[SNCBCOM1].InitTouchFuncXP(&ToggleBinarySwitch);

            scnNode[SNCBCOM2].InitNexCon(0xD, "Pc2.pic");                 // COM2 button
            scnNode[SNCBCOM2].InitNexType(AMK_INPUT | AMK_DATAUPDATE);
            scnNode[SNCBCOM2].InitOperands(IMG_COM2);                       // Pic ID
            scnNode[SNCBCOM2].AttachSimInt("sim/cockpit2/radios/actuators/audio_selection_com2", XPL_READWRITE, 150);
            scnNode[SNCBCOM2].InitTouchFuncXP(&ToggleBinarySwitch);
        }
        else         // FS2020 or X-Plane generic
        {
            scnNode[SNCBCOM1].InitNexCon(0xC, "Pc1.pic");                 // COM1 button
            scnNode[SNCBCOM1].InitNexType(AMK_INPUT | AMK_SENDDIRECT);
            scnNode[SNCBCOM1].InitSimIds(152, 1, 218);
            scnNode[SNCBCOM1].InitOperands(IMG_COM1);                       // Pic ID
            scnNode[SNCBCOM1].AttachSimInt("sim/cockpit2/radios/actuators/audio_selection_com1", XPL_READ, 250);
            scnNode[SNCBCOM1].AttachCommand("sim/audio_panel/select_audio_com1", 0);

            scnNode[SNCBCOM2].InitNexCon(0xD, "Pc2.pic");                 // COM2 button
            scnNode[SNCBCOM2].InitNexType(AMK_INPUT | AMK_SENDDIRECT);
            scnNode[SNCBCOM2].InitSimIds(152, 2, 228);
            scnNode[SNCBCOM2].InitOperands(IMG_COM2);                       // Pic ID
            scnNode[SNCBCOM2].AttachSimInt("sim/cockpit2/radios/actuators/audio_selection_com2", XPL_READ, 250);
            scnNode[SNCBCOM2].AttachCommand("sim/audio_panel/select_audio_com2", 0);
        }

        if (IsXPlane())    // All X-Plane
        {
            scnNode[SNCBNV1X].InitNexCon(0xE, "Pn1.pic");             // NAV1 button--XP
            scnNode[SNCBNV1X].InitNexType(AMK_INPUT | AMK_SENDDIRECT);
            scnNode[SNCBNV1X].InitOperands(IMG_NAV1);                   // Pic ID
            scnNode[SNCBNV1X].AttachSimInt("sim/cockpit2/radios/actuators/audio_selection_nav1", XPL_READ, 250);
            scnNode[SNCBNV1X].AttachCommand("sim/audio_panel/select_audio_nav1", 0);

            scnNode[SNCBNV2X].InitNexCon(0xF, "Pn2.pic");             // NAV2 button--XP
            scnNode[SNCBNV2X].InitNexType(AMK_INPUT | AMK_SENDDIRECT);
            scnNode[SNCBNV2X].InitOperands(IMG_NAV2);                   // Pic ID
            scnNode[SNCBNV2X].AttachSimInt("sim/cockpit2/radios/actuators/audio_selection_nav2", XPL_READ, 250);
            scnNode[SNCBNV2X].AttachCommand("sim/audio_panel/select_audio_nav2", 0);

            scnNode[SNCBMRKR].InitNexCon(0x10, "Pmk.pic");                // Marker button
            scnNode[SNCBMRKR].InitNexType(AMK_INPUT | AMK_SENDDIRECT);
            scnNode[SNCBMRKR].InitSimIds(494, -1, 511);
            scnNode[SNCBMRKR].InitOperands(IMG_MRKR);                       // Pic ID
            scnNode[SNCBMRKR].AttachSimInt("sim/cockpit2/radios/actuators/audio_marker_enabled", XPL_READ, 250);
            scnNode[SNCBMRKR].AttachCommand("sim/annunciator/marker_beacon_mute_or_off", 0);
        }
        else         // FS2020
        {   // Sim interface unable to provide status of these buttons
            scnNode[SNCBNV1M].InitNexCon(0xE);                        // NAV1 button--MSFS
            scnNode[SNCBNV1M].InitSimIds(0, -1, 693);                   // A single-pic button
            scnNode[SNCBNV1M].InitTouchFuncXP(&ToggleBinarySwitch);

            scnNode[SNCBNV2M].InitNexCon(0xF);                        // NAV2 button--MSFS
            scnNode[SNCBNV2M].InitSimIds(0, -1, 697);                   // A single-pic button
            scnNode[SNCBNV2M].InitTouchFuncXP(&ToggleBinarySwitch);
        }
     }
    else             // Zibo-only
    {
        scnNode[SNCBCOM1].InitNexCon(0xC, "Pc1.pic");                 // COM1 button
        scnNode[SNCBCOM1].InitNexType(AMK_INPUT | AMK_SENDDIRECT);
        scnNode[SNCBCOM1].InitOperands(IMG_COM1);
        scnNode[SNCBCOM1].AttachSimInt("laminar/B738/comm/audio_sel_com1_on", XPL_READ, 250);
        scnNode[SNCBCOM1].AttachCommand("laminar/B738/comm/com1_audio_sel", 0);

        scnNode[SNCBCOM2].InitNexCon(0xD, "Pc2.pic");                 // COM2 button
        scnNode[SNCBCOM2].InitNexType(AMK_INPUT | AMK_SENDDIRECT);
        scnNode[SNCBCOM2].InitOperands(IMG_COM2);
        scnNode[SNCBCOM2].AttachSimInt("laminar/B738/comm/audio_sel_com2_on", XPL_READ, 250);
        scnNode[SNCBCOM2].AttachCommand("laminar/B738/comm/com2_audio_sel", 0);

        scnNode[SNCBNV1X].InitNexCon(0xE, "Pn1.pic");                 // NAV1 button
        scnNode[SNCBNV1X].InitNexType(AMK_INPUT | AMK_SENDDIRECT);
        scnNode[SNCBNV1X].InitOperands(IMG_NAV1);
        scnNode[SNCBNV1X].AttachSimInt("laminar/B738/comm/audio_sel_nav1_on", XPL_READ, 250);
        scnNode[SNCBNV1X].AttachCommand("laminar/B738/comm/nav1_audio_sel", 0);

        scnNode[SNCBNV2X].InitNexCon(0xF, "Pn2.pic");                 // NAV2 button
        scnNode[SNCBNV2X].InitNexType(AMK_INPUT | AMK_SENDDIRECT);
        scnNode[SNCBNV2X].InitOperands(IMG_NAV2);
        scnNode[SNCBNV2X].AttachSimInt("laminar/B738/comm/audio_sel_nav2_on", XPL_READ, 250);
        scnNode[SNCBNV2X].AttachCommand("laminar/B738/comm/nav2_audio_sel", 0);

        scnNode[SNCBMRKR].InitNexCon(0x10, "Pmk.pic");                // Marker button
        scnNode[SNCBMRKR].InitNexType(AMK_INPUT | AMK_SENDDIRECT);
        scnNode[SNCBMRKR].InitOperands(IMG_MRKR);                       // Pic ID
        scnNode[SNCBMRKR].AttachSimInt("laminar/B738/comm/audio_sel_mark_on", XPL_READ, 250);
        scnNode[SNCBMRKR].AttachCommand("laminar/B738/comm/mark_audio_sel", 0);
    }


// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
// PAGE: DEVICES
// Aircraft system controls.    

    if (IsGenericProfile())
    {
        scnNode[SNDBANTI].InitNexCon(1, "Pai.pic");                   // Anti-Ice switch
        scnNode[SNDBANTI].InitNexType(AMK_INPUT | AMK_DATAUPDATE);
        scnNode[SNDBANTI].InitSimIds(330, 1, 48);
        scnNode[SNDBANTI].InitOperands(IMG_ANTIICE);                    // Pic ID
        scnNode[SNDBANTI].AttachSimInt("sim/cockpit2/ice/ice_surfce_heat_on");
        scnNode[SNDBANTI].InitTouchFuncXP(&ToggleBinarySwitch);

        scnNode[SNDBSTRI].InitNexCon(2, "Pst.pic");                   // Structural de-ice switch
        scnNode[SNDBSTRI].InitNexType(AMK_INPUT | AMK_DATAUPDATE);
        scnNode[SNDBSTRI].InitSimIds(693, -1, 907);
        scnNode[SNDBSTRI].InitOperands(IMG_STRUCTICE);                  // Pic ID
        scnNode[SNDBSTRI].AttachSimInt("sim/cockpit2/ice/ice_window_heat_on");
        scnNode[SNDBSTRI].InitTouchFuncXP(&ToggleBinarySwitch);

        scnNode[SNDBPROI].InitNexCon(3, "Ppr.pic");                   // Prop de-ice switch
        scnNode[SNDBPROI].InitNexType(AMK_INPUT | AMK_DATAUPDATE);
        scnNode[SNDBPROI].InitSimIds(578, 1, 896);
        scnNode[SNDBPROI].InitOperands(IMG_PROPICE);                    // Pic ID
        scnNode[SNDBPROI].AttachSimInt("sim/cockpit2/ice/ice_prop_heat_on");
        scnNode[SNDBPROI].InitTouchFuncXP(&ToggleBinarySwitch);

        scnNode[SNDBPIHT].InitNexCon(4, "Ppi.pic");                   // Pitot heat switch
        scnNode[SNDBPIHT].InitNexType(AMK_INPUT | AMK_DATAUPDATE);
        scnNode[SNDBPIHT].InitSimIds(555, -1, 622);
        scnNode[SNDBPIHT].InitOperands(IMG_PITOT);                      // Pic ID
        scnNode[SNDBPIHT].AttachSimInt("sim/cockpit2/ice/ice_pitot_heat_on_pilot");
        scnNode[SNDBPIHT].InitTouchFuncXP(&ToggleBinarySwitch);

        scnNode[SNDBLPNL].InitNexCon(5, "Ppa.pic");                   // Panel light switch
        scnNode[SNDBLPNL].InitNexType(AMK_INPUT | AMK_DATAUPDATE);
        scnNode[SNDBLPNL].InitSimIds(470, -1, 613);
        scnNode[SNDBLPNL].InitOperands(IMG_LTPANEL);                    // Pic ID
        scnNode[SNDBLPNL].AttachSimInt("sim/cockpit/electrical/instrument_brightness");
        scnNode[SNDBLPNL].InitTouchFuncXP(&ToggleBinarySwitch);

        scnNode[SNDBLTXI].InitNexCon(6, "Ptx.pic");                   // Taxi light switch
        scnNode[SNDBLTXI].InitNexType(AMK_INPUT | AMK_DATAUPDATE);
        scnNode[SNDBLTXI].InitSimIds(477, -1, 910);
        scnNode[SNDBLTXI].InitOperands(IMG_LTTAXI);                     // Pic ID
        scnNode[SNDBLTXI].AttachSimInt("sim/cockpit2/switches/taxi_light_on");
        scnNode[SNDBLTXI].InitTouchFuncXP(&ToggleBinarySwitch);

        scnNode[SNDBLANC].InitNexCon(7, "Pac.pic");                   // Anti-Collision (Beacon) switch
        scnNode[SNDBLANC].InitNexType(AMK_INPUT | AMK_DATAUPDATE);
        scnNode[SNDBLANC].InitSimIds(457, -1, 854);
        scnNode[SNDBLANC].InitOperands(IMG_LTANTICOL);                  // Pic ID
        scnNode[SNDBLANC].AttachSimInt("sim/cockpit2/switches/beacon_on");
        scnNode[SNDBLANC].InitTouchFuncXP(&ToggleBinarySwitch);

        scnNode[SNDBLPUL].InitNexCon(8, "Ppu.pic");                   // Pulse light switch
        scnNode[SNDBLPUL].InitNexType(AMK_INPUT | AMK_DATAUPDATE);
        scnNode[SNDBLPUL].InitSimIds(475, -1, 789);
        scnNode[SNDBLPUL].InitOperands(IMG_LTPULSE);                    // Pic ID
        scnNode[SNDBLPUL].AttachSimInt("sim/cockpit2/switches/strobe_lights_on");
        scnNode[SNDBLPUL].InitTouchFuncXP(&ToggleBinarySwitch);

        scnNode[SNDBLNAV].InitNexCon(9, "Pnv.pic");                   // Nav light switch
        scnNode[SNDBLNAV].InitNexType(AMK_INPUT | AMK_DATAUPDATE);
        scnNode[SNDBLNAV].InitSimIds(467, -1, 889);
        scnNode[SNDBLNAV].InitOperands(IMG_LTNAV);                      // Pic ID
        scnNode[SNDBLNAV].AttachSimInt("sim/cockpit/electrical/nav_lights_on");
        scnNode[SNDBLNAV].InitTouchFuncXP(&ToggleBinarySwitch);

        scnNode[SNDBLLDG].InitNexCon(0xA, "Pla.pic");                 // Landing light switch
        scnNode[SNDBLLDG].InitNexType(AMK_INPUT | AMK_DATAUPDATE);
        scnNode[SNDBLLDG].InitSimIds(463, -1, 466);
        scnNode[SNDBLLDG].InitOperands(IMG_LTLAND);                     // Pic ID
        scnNode[SNDBLLDG].AttachSimInt("sim/cockpit2/switches/landing_lights_on", XPL_READWRITE, 200);
        scnNode[SNDBLLDG].InitTouchFuncXP(&ToggleBinarySwitch);

        scnNode[SNDBBATT].InitNexCon(0xB, "Pma.pic");                 // Battery main switch
        scnNode[SNDBBATT].InitNexType(AMK_INPUT | AMK_DATAUPDATE);
        scnNode[SNDBBATT].InitSimIds(186, -1, 886);
        scnNode[SNDBBATT].InitOperands(IMG_BATMAIN);                    // Pic ID
        scnNode[SNDBBATT].AttachSimInt("sim/cockpit/electrical/battery_on");
        scnNode[SNDBBATT].InitTouchFuncXP(&ToggleBinarySwitch);

        scnNode[SNDBALTR].InitNexCon(0xC, "Pal.pic");                 // Alternator main switch
        scnNode[SNDBALTR].InitNexType(AMK_INPUT | AMK_SENDDIRECT);
        scnNode[SNDBALTR].InitSimIds(344, 1, 885);
        scnNode[SNDBALTR].InitOperands(IMG_ALTMAIN);                    // Pic ID
        scnNode[SNDBALTR].AttachSimInt("sim/cockpit2/electrical/generator_on", XPL_READ);
        scnNode[SNDBALTR].AttachCommand("sim/electrical/generator_1_toggle", 0);

        scnNode[SNDBAVIO].InitNexCon(0xD, "Pav.pic");                 // Avionic power switch
        scnNode[SNDBAVIO].InitNexType(AMK_INPUT | AMK_DATAUPDATE);
        scnNode[SNDBAVIO].InitSimIds(108, -1, 154);
        scnNode[SNDBAVIO].InitOperands(IMG_AVIONIC);                    // Pic ID
        scnNode[SNDBAVIO].AttachSimInt("sim/cockpit2/switches/avionics_power_on");
        scnNode[SNDBAVIO].InitTouchFuncAll(&ToggleBinarySwitch);

        scnNode[SNDBIGN1].InitNexCon(0xE, "Pi1.pic");                 // Ignition1 switch. MS-only
        scnNode[SNDBIGN1].InitNexType(AMK_INPUT);
        scnNode[SNDBIGN1].InitSimIds(732, 1, 925);
        scnNode[SNDBIGN1].InitOperands(IMG_IGNITN1);                    // Pic ID

        scnNode[SNDBIGN2].InitNexCon(0xF, "Pi2.pic");                 // Ignition2 switch. MS-only
        scnNode[SNDBIGN2].InitNexType(AMK_INPUT);
        scnNode[SNDBIGN2].InitSimIds(732, 2, 925);
        scnNode[SNDBIGN2].InitOperands(IMG_IGNITN2);                    // Pic ID

        scnNode[SNDBSTR1].InitNexCon(0x10, "Ps1.pic");                // Starter1 press
        scnNode[SNDBSTR1].InitNexType(AMK_INPUT | AMK_XPPRESSED);
        scnNode[SNDBSTR1].InitSimIds(354, 1, 902);
        scnNode[SNDBSTR1].InitOperands(IMG_STARTR1);                    // Pic ID
        scnNode[SNDBSTR1].AttachSimInt("sim/flightmodel2/engines/starter_is_running", XPL_READ, 300, 0);
        scnNode[SNDBSTR1].AttachCommand("sim/ignition/ignition_up_1", 0);

        scnNode[SNDBST1X].InitNexCon(0x11);                           // Starter1 release. XP-only
        scnNode[SNDBST1X].InitNexType(AMK_XPRELEASED);
        scnNode[SNDBST1X].AttachCommand("sim/ignition/ignition_up_1", 0);

        scnNode[SNDKKEY1].InitNexCon(0);                              // Starter1 turnkey
        scnNode[SNDKKEY1].InitNexType(AMK_SELECT | AMK_SENDDIRECT);
        scnNode[SNDKKEY1].InitOperands(477, 478);
        scnNode[SNDKKEY1].AttachCommand("sim/ignition/ignition_down_1", 1);
        scnNode[SNDKKEY1].AttachCommand("sim/ignition/ignition_up_1", 2);

        scnNode[SNDBSTR2].InitNexCon(0x12, "Ps2.pic");                // Starter2 switch
        scnNode[SNDBSTR2].InitNexType(AMK_INPUT | AMK_XPPRESSED);
        scnNode[SNDBSTR2].InitSimIds(354, 2, 903);
        scnNode[SNDBSTR2].InitOperands(IMG_STARTR2);                    // Pic ID
        scnNode[SNDBSTR2].AttachSimInt("sim/flightmodel2/engines/starter_is_running", XPL_READ, 300, 1);
        scnNode[SNDBSTR2].AttachCommand("sim/ignition/ignition_up_2", 0);

        scnNode[SNDBST2X].InitNexCon(0x13);                           // Starter2 release. XP-only
        scnNode[SNDBST2X].InitNexType(AMK_XPRELEASED);
        scnNode[SNDBST2X].AttachCommand("sim/ignition/ignition_up_2", 0);

        scnNode[SNDKKEY2].InitNexCon(0);                              // Starter2 turnkey
        scnNode[SNDKKEY2].InitNexType(AMK_SELECT | AMK_SENDDIRECT);
        scnNode[SNDKKEY2].InitOperands(485, 486);
        scnNode[SNDKKEY2].AttachCommand("sim/ignition/ignition_down_2", 1);
        scnNode[SNDKKEY2].AttachCommand("sim/ignition/ignition_up_2", 2);
    }


// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
// PAGE: ZIBO PANELs
// For the Zibo-only Ice-Protection & Lights Panel page.

    if (IsZiboProfile())
    {
        scnNode[SNZBMCAU].InitNexCon(5, "Pmc.pic");                   // Master caution
        scnNode[SNZBMCAU].InitNexType(AMK_INPUT | AMK_SENDDIRECT);
        scnNode[SNZBMCAU].InitOperands(IMG_MSTCAUTION);
        scnNode[SNZBMCAU].AttachSimInt("laminar/B738/annunciator/master_caution_light", XPL_READ, 200);
        scnNode[SNZBMCAU].AttachCommand("laminar/B738/push_button/master_caution1", 0);

        scnNode[SNZKPANL].InitNexCon(1, "Tli.txt");                   // Panel brightness
        scnNode[SNZKPANL].InitNexType(AMK_SELECT);
        scnNode[SNZKPANL].InitOperands(OD_ZbLightPanel_Dec, OD_ZbLightPanel_Inc);
        scnNode[SNZKPANL].InitTouchFuncXP(&SelectPanelLights);

        scnNode[SNZKINTL].InitNexCon(1);                              // Interior/flood brightness
        scnNode[SNZKINTL].InitNexType(AMK_SELECT);
        scnNode[SNZKINTL].InitOperands(OD_ZbLightFlood_Dec, OD_ZbLightFlood_Inc);

        scnNode[SNZKDISP].InitNexCon(1);                              // Display brightness
        scnNode[SNZKDISP].InitNexType(AMK_SELECT);
        scnNode[SNZKDISP].InitOperands(OD_ZbLightDispl_Dec, OD_ZbLightDispl_Inc);

        scnNode[SNZKFMC1].InitNexCon(1);                              // FMC brightness
        scnNode[SNZKFMC1].InitNexType(AMK_SELECT);
        scnNode[SNZKFMC1].InitOperands(OD_ZbLightFmc_Dec, OD_ZbLightFmc_Inc);

        scnNode[SNZBPHTL].InitNexCon(0x10, "Ppl.pic");                // Probe heat L
        scnNode[SNZBPHTL].InitNexType(AMK_INPUT | AMK_SENDDIRECT);
        scnNode[SNZBPHTL].InitOperands(IMG_SWGREEN0);
        scnNode[SNZBPHTL].AttachSimInt("laminar/B738/toggle_switch/capt_probes_pos", XPL_READ, 120);
        scnNode[SNZBPHTL].AttachCommand("laminar/B738/toggle_switch/capt_probes_pos", 0);

        scnNode[SNZBPHTR].InitNexCon(0x11, "Ppr.pic");                // Probe heat R
        scnNode[SNZBPHTR].InitNexType(AMK_INPUT | AMK_SENDDIRECT);
        scnNode[SNZBPHTR].InitOperands(IMG_SWGREEN0);
        scnNode[SNZBPHTR].AttachSimInt("laminar/B738/toggle_switch/fo_probes_pos", XPL_READ, 120);
        scnNode[SNZBPHTR].AttachCommand("laminar/B738/toggle_switch/fo_probes_pos", 0);

        scnNode[SNZBWICE].InitNexCon(0x12, "Pwa.pic");                // Wing anti-ice
        scnNode[SNZBWICE].InitNexType(AMK_INPUT | AMK_SENDDIRECT);
        scnNode[SNZBWICE].InitOperands(IMG_SWGREEN0);
        scnNode[SNZBWICE].AttachSimInt("laminar/B738/antiice_sw", XPL_READ, 120, 2);
        scnNode[SNZBWICE].AttachCommand("laminar/B738/toggle_switch/wing_heat", 0);

        scnNode[SNZBEIC1].InitNexCon(0x14, "Pe1.pic");                // Engine1 anti-ice
        scnNode[SNZBEIC1].InitNexType(AMK_INPUT | AMK_SENDDIRECT);
        scnNode[SNZBEIC1].InitOperands(IMG_SWGREEN0);
        scnNode[SNZBEIC1].AttachSimInt("laminar/B738/antiice_sw", XPL_READ, 120, 0);
        scnNode[SNZBEIC1].AttachCommand("laminar/B738/toggle_switch/eng1_heat", 0);

        scnNode[SNZBEIC2].InitNexCon(0x15, "Pe2.pic");                // Engine2 anti-ice
        scnNode[SNZBEIC2].InitNexType(AMK_INPUT | AMK_SENDDIRECT);
        scnNode[SNZBEIC2].InitOperands(IMG_SWGREEN0);
        scnNode[SNZBEIC2].AttachSimInt("laminar/B738/antiice_sw", XPL_READ, 120, 1);
        scnNode[SNZBEIC2].AttachCommand("laminar/B738/toggle_switch/eng2_heat", 0);

        scnNode[SNZBLOGO].InitNexCon(0x17, "Plo.pic");               // Logo light
        scnNode[SNZBLOGO].InitNexType(AMK_INPUT | AMK_SENDDIRECT);
        scnNode[SNZBLOGO].InitOperands(IMG_SWGREEN0);
        scnNode[SNZBLOGO].AttachSimInt("laminar/B738/toggle_switch/logo_light", XPL_READ, 120);
        scnNode[SNZBLOGO].AttachCommand("laminar/B738/switch/logo_light_toggle", 0);

        scnNode[SNZBSTRB].InitNexCon(0x18, "Pst.pic");               // Position light
        scnNode[SNZBSTRB].InitNexType(AMK_INPUT | AMK_SENDDIRECT);
        scnNode[SNZBSTRB].InitOperands(IMG_SWGREEN0);
        scnNode[SNZBSTRB].AttachSimInt("laminar/B738/toggle_switch/position_light_pos", XPL_READ, 120);
        scnNode[SNZBSTRB].AttachCommand("laminar/B738/toggle_switch/position_light_off", 1);
        scnNode[SNZBSTRB].AttachCommand("laminar/B738/toggle_switch/position_light_strobe", 2);
        scnNode[SNZBSTRB].InitTouchFuncXP(&XP_TogglePositionLight);
        scnNode[SNZBSTRB].InitDispFuncXP(&ShowTruncatedBinaryBtn);

        scnNode[SNZBANCO].InitNexCon(0x19, "Pac.pic");               // Anti-collision light
        scnNode[SNZBANCO].InitNexType(AMK_INPUT | AMK_SENDDIRECT);
        scnNode[SNZBANCO].InitOperands(IMG_SWGREEN0);
        scnNode[SNZBANCO].AttachSimInt("sim/cockpit2/switches/beacon_on", XPL_READ, 120);
        scnNode[SNZBANCO].AttachCommand("sim/lights/beacon_lights_toggle", 0);

        scnNode[SNZBWING].InitNexCon(0x1A, "Pwi.pic");               // Wing light
        scnNode[SNZBWING].InitNexType(AMK_INPUT | AMK_SENDDIRECT);
        scnNode[SNZBWING].InitOperands(IMG_SWGREEN0);
        scnNode[SNZBWING].AttachSimInt("sim/cockpit2/switches/generic_lights_switch", XPL_READ, 120, 0);
        scnNode[SNZBWING].AttachCommand("laminar/B738/switch/wing_light_toggle", 0);

        scnNode[SNZBWHEL].InitNexCon(0x1B, "Pww.pic");               // Wheel well light
        scnNode[SNZBWHEL].InitNexType(AMK_INPUT | AMK_SENDDIRECT);
        scnNode[SNZBWHEL].InitOperands(IMG_SWGREEN0);
        scnNode[SNZBWHEL].AttachSimInt("sim/cockpit2/switches/generic_lights_switch", XPL_READ, 120, 5);
        scnNode[SNZBWHEL].AttachCommand("laminar/B738/switch/wheel_light_toggle", 0);

        scnNode[SNZBLNDL].InitNexCon(0x1E, "Pll.pic");               // Landing light L
        scnNode[SNZBLNDL].InitNexType(AMK_INPUT | AMK_SENDDIRECT);
        scnNode[SNZBLNDL].InitOperands(IMG_SWGREEN0);
        scnNode[SNZBLNDL].AttachSimInt("laminar/B738/switch/land_lights_left_pos", XPL_READ, 120, 0);
        scnNode[SNZBLNDL].AttachCommand("laminar/B738/switch/land_lights_left", 0);

        scnNode[SNZBLNDR].InitNexCon(0x1F, "Plr.pic");               // Landing light R
        scnNode[SNZBLNDR].InitNexType(AMK_INPUT | AMK_SENDDIRECT);
        scnNode[SNZBLNDR].InitOperands(IMG_SWGREEN0);
        scnNode[SNZBLNDR].AttachSimInt("laminar/B738/switch/land_lights_right_pos", XPL_READ, 120, 3);
        scnNode[SNZBLNDR].AttachCommand("laminar/B738/switch/land_lights_right", 0);

        scnNode[SNZBRWYL].InitNexCon(0x20, "Prl.pic");               // Rwy turnoff light R
        scnNode[SNZBRWYL].InitNexType(AMK_INPUT | AMK_SENDDIRECT);
        scnNode[SNZBRWYL].InitOperands(IMG_SWGREEN0);
        scnNode[SNZBRWYL].AttachSimInt("laminar/B738/toggle_switch/rwy_light_left", XPL_READ, 120);
        scnNode[SNZBRWYL].AttachCommand("laminar/B738/switch/rwy_light_left_toggle", 0);

        scnNode[SNZBRWYR].InitNexCon(0x21, "Prr.pic");               // Rwy turnoff light R
        scnNode[SNZBRWYR].InitNexType(AMK_INPUT | AMK_SENDDIRECT);
        scnNode[SNZBRWYR].InitOperands(IMG_SWGREEN0);
        scnNode[SNZBRWYR].AttachSimInt("laminar/B738/toggle_switch/rwy_light_right", XPL_READ, 120);
        scnNode[SNZBRWYR].AttachCommand("laminar/B738/switch/rwy_light_right_toggle", 0);

        scnNode[SNZBTAXI].InitNexCon(0x22, "Pta.pic");               // Taxi light
        scnNode[SNZBTAXI].InitNexType(AMK_INPUT | AMK_SENDDIRECT);
        scnNode[SNZBTAXI].InitOperands(IMG_SWGREEN0);
        scnNode[SNZBTAXI].AttachSimInt("laminar/B738/toggle_switch/taxi_light_brightness_pos", XPL_READ, 120);
        scnNode[SNZBTAXI].AttachCommand("laminar/B738/toggle_switch/taxi_light_brigh_toggle", 0);
        scnNode[SNZBTAXI].InitDispFuncXP(&ShowTruncatedBinaryBtn);

        scnNode[SNZKWIPL].InitNexCon(2, "Twp.txt");                  // Wiper group. Left wiper
        scnNode[SNZKWIPL].InitNexType(AMK_SELECT | AMK_SENDDIRECT);
        scnNode[SNZKWIPL].AttachCommand("laminar/B738/knob/left_wiper_dn", 1);
        scnNode[SNZKWIPL].AttachCommand("laminar/B738/knob/left_wiper_up", 2);
        scnNode[SNZKWIPL].InitTouchFuncXP(&SelectPanelWiper);

        scnNode[SNZKWIPR].InitNexCon(2);                             // Right wiper
        scnNode[SNZKWIPR].InitNexType(AMK_SELECT | AMK_SENDDIRECT);
        scnNode[SNZKWIPR].AttachCommand("laminar/B738/knob/right_wiper_dn", 1);
        scnNode[SNZKWIPR].AttachCommand("laminar/B738/knob/right_wiper_up", 2);

        scnNode[SNZKFLTA].InitNexCon(2, "Tcp.txt");                  // Cabin Flt alt
        scnNode[SNZKFLTA].InitNexType(AMK_SELECT | AMK_SENDDIRECT);
        scnNode[SNZKFLTA].AttachCommand("laminar/B738/knob/flt_alt_press_dn", 1);
        scnNode[SNZKFLTA].AttachCommand("laminar/B738/knob/flt_alt_press_up", 2);

        scnNode[SNZKLNDA].InitNexCon(2);                             // Cabin Land alt
        scnNode[SNZKLNDA].InitNexType(AMK_SELECT | AMK_SENDDIRECT);
        scnNode[SNZKLNDA].AttachCommand("laminar/B738/knob/land_alt_press_dn", 1);
        scnNode[SNZKLNDA].AttachCommand("laminar/B738/knob/land_alt_press_up", 2);

// For the Zibo-only - Aux Panel - Window Heat controls.

        scnNode[SNZBWHLS].InitNexCon(0x10, "Pls.pic");               // Window heat L/Side
        scnNode[SNZBWHLS].InitNexType(AMK_INPUT | AMK_SENDDIRECT);
        scnNode[SNZBWHLS].InitOperands(IMG_SWGREEN0);
        scnNode[SNZBWHLS].AttachSimInt("laminar/B738/ice/window_heat_l_side_pos", XPL_READ, 120);
        scnNode[SNZBWHLS].AttachCommand("laminar/B738/toggle_switch/window_heat_l_side", 0);

        scnNode[SNZBWHLF].InitNexCon(0x11, "Plf.pic");               // Window heat L/Fwd
        scnNode[SNZBWHLF].InitNexType(AMK_INPUT | AMK_SENDDIRECT);
        scnNode[SNZBWHLF].InitOperands(IMG_SWGREEN0);
        scnNode[SNZBWHLF].AttachSimInt("laminar/B738/ice/window_heat_l_fwd_pos", XPL_READ, 120);
        scnNode[SNZBWHLF].AttachCommand("laminar/B738/toggle_switch/window_heat_l_fwd", 0);
       
        scnNode[SNZBWHRF].InitNexCon(0x12, "Prf.pic");               // Window heat R/Fwd
        scnNode[SNZBWHRF].InitNexType(AMK_INPUT | AMK_SENDDIRECT);
        scnNode[SNZBWHRF].InitOperands(IMG_SWGREEN0);
        scnNode[SNZBWHRF].AttachSimInt("laminar/B738/ice/window_heat_r_fwd_pos", XPL_READ, 120);
        scnNode[SNZBWHRF].AttachCommand("laminar/B738/toggle_switch/window_heat_r_fwd", 0);

        scnNode[SNZBWHRS].InitNexCon(0x13, "Prs.pic");               // Window heat R/Side
        scnNode[SNZBWHRS].InitNexType(AMK_INPUT | AMK_SENDDIRECT);
        scnNode[SNZBWHRS].InitOperands(IMG_SWGREEN0);
        scnNode[SNZBWHRS].AttachSimInt("laminar/B738/ice/window_heat_r_side_pos", XPL_READ, 120);
        scnNode[SNZBWHRS].AttachCommand("laminar/B738/toggle_switch/window_heat_r_side", 0);

        // SNZBWHRF, SNZBWHRS,
    }

// PAGE: TOLISS PANEL
// For the ToLiss-only Anti-Ice & Lights Panel page.

    if (IsToLissProfile())
    {
        scnNode[SNZKPANL].InitNexCon(1, "Tli.txt");                   // Panel brightness
        scnNode[SNZKPANL].InitNexType(AMK_SELECT);
        scnNode[SNZKPANL].InitOperands(OD_TlLightPanel_Dec, OD_TlLightPanel_Inc);
        scnNode[SNZKPANL].InitTouchFuncXP(&SelectPanelLights);

        scnNode[SNZKINTL].InitNexCon(1);                              // Interior/flood brightness
        scnNode[SNZKINTL].InitNexType(AMK_SELECT);
        scnNode[SNZKINTL].InitOperands(OD_TlLightFlood_Dec, OD_TlLightFlood_Inc);

        scnNode[SNZKDISP].InitNexCon(1);                              // Display brightness
        scnNode[SNZKDISP].InitNexType(AMK_SELECT);
        scnNode[SNZKDISP].InitOperands(OD_TlLightDispl_Dec, OD_TlLightDispl_Inc);

        scnNode[SNZKFMC1].InitNexCon(1);                              // MCDU brightness
        scnNode[SNZKFMC1].InitNexType(AMK_SELECT);
        scnNode[SNZKFMC1].InitOperands(OD_TlLightFmc_Dec, OD_TlLightFmc_Inc);

        scnNode[SNZBPHTL].InitNexCon(0xC, "Ppw.pic");                 // Probe/ Window heat
        scnNode[SNZBPHTL].InitNexType(AMK_INPUT | AMK_DATAUPDATE);
        scnNode[SNZBPHTL].InitOperands(IMG_SWCOBALT0);
        scnNode[SNZBPHTL].AttachSimInt("AirbusFBW/ProbeHeatSwitch", XPL_READWRITE, 130);
        scnNode[SNZBPHTL].InitTouchFuncAll(&ToggleBinarySwitch);

        scnNode[SNZBWICE].InitNexCon(8, "Phw.pic");                   // Wing anti-ice
        scnNode[SNZBWICE].InitNexType(AMK_INPUT | AMK_SENDDIRECT);
        scnNode[SNZBWICE].InitOperands(IMG_SWCOBALT0);
        scnNode[SNZBWICE].AttachSimInt("sim/cockpit/switches/anti_ice_surf_heat", XPL_READ, 130);
        scnNode[SNZBWICE].AttachCommand("toliss_airbus/antiicecommands/WingToggle", 0);

        scnNode[SNZBEIC1].InitNexCon(0xA, "Pe1.pic");                 // Engine1 anti-ice
        scnNode[SNZBEIC1].InitNexType(AMK_INPUT | AMK_SENDDIRECT);
        scnNode[SNZBEIC1].InitOperands(IMG_SWCOBALT0);
        scnNode[SNZBEIC1].AttachSimInt("AirbusFBW/ENG1AISwitch", XPL_READ, 130);
        scnNode[SNZBEIC1].AttachCommand("toliss_airbus/antiicecommands/ENG1Toggle", 0);

        scnNode[SNZBEIC2].InitNexCon(0xB, "Pe2.pic");                 // Engine2 anti-ice
        scnNode[SNZBEIC2].InitNexType(AMK_INPUT | AMK_SENDDIRECT);
        scnNode[SNZBEIC2].InitOperands(IMG_SWCOBALT0);
        scnNode[SNZBEIC2].AttachSimInt("AirbusFBW/ENG2AISwitch", XPL_READ, 130);
        scnNode[SNZBEIC2].AttachCommand("toliss_airbus/antiicecommands/ENG2Toggle", 0);

        scnNode[SNZBLOGO].InitNexCon(0x13, "Pnl.pic");                // Logo light
        scnNode[SNZBLOGO].InitNexType(AMK_INPUT | AMK_DATAUPDATE);
        scnNode[SNZBLOGO].InitOperands(IMG_SWCOBALT2);
        scnNode[SNZBLOGO].AttachSimInt("AirbusFBW/OHPLightSwitches", XPL_READWRITE, 130, 2);
        scnNode[SNZBLOGO].InitTouchFuncXP(&StepTristateSwitch);

        scnNode[SNZBSTRB].InitNexCon(0x10, "Pst.pic");                // Strobe light
        scnNode[SNZBSTRB].InitNexType(AMK_INPUT | AMK_DATAUPDATE);
        scnNode[SNZBSTRB].InitOperands(IMG_SWCOBALT2);
        scnNode[SNZBSTRB].AttachSimInt("AirbusFBW/OHPLightSwitches", XPL_READWRITE, 130, 7);
        scnNode[SNZBSTRB].InitTouchFuncXP(&StepTristateSwitch);

        scnNode[SNZBANCO].InitNexCon(0x11, "Pbe.pic");                // Beacon light
        scnNode[SNZBANCO].InitNexType(AMK_INPUT | AMK_DATAUPDATE);
        scnNode[SNZBANCO].InitOperands(IMG_SWCOBALT0);
        scnNode[SNZBANCO].AttachSimInt("AirbusFBW/OHPLightSwitches", XPL_READWRITE, 130, 0);
        scnNode[SNZBANCO].InitTouchFuncXP(&ToggleBinarySwitch);

        scnNode[SNZBWING].InitNexCon(0x12, "Pwi.pic");                // Wing light
        scnNode[SNZBWING].InitNexType(AMK_INPUT | AMK_DATAUPDATE);
        scnNode[SNZBWING].InitOperands(IMG_SWCOBALT0);
        scnNode[SNZBWING].AttachSimInt("AirbusFBW/OHPLightSwitches", XPL_READWRITE, 130, 1);
        scnNode[SNZBWING].InitTouchFuncAll(&ToggleBinarySwitch);

        scnNode[SNZBWHEL].InitNexCon(0x1C, "Psb.pic");                // Seat belts light
        scnNode[SNZBWHEL].InitNexType(AMK_INPUT | AMK_DATAUPDATE);
        scnNode[SNZBWHEL].InitOperands(IMG_SWCOBALT0);
        scnNode[SNZBWHEL].AttachSimInt("AirbusFBW/OHPLightSwitches", XPL_READWRITE, 130, 11);
        scnNode[SNZBWHEL].InitTouchFuncAll(&ToggleBinarySwitch);

        scnNode[SNZBLNDL].InitNexCon(0x19, "Pll.pic");                // Landing light L
        scnNode[SNZBLNDL].InitNexType(AMK_INPUT | AMK_DATAUPDATE);
        scnNode[SNZBLNDL].InitOperands(IMG_SWCOBALT2);
        scnNode[SNZBLNDL].AttachSimInt("AirbusFBW/OHPLightSwitches", XPL_READWRITE, 130, 4);
        scnNode[SNZBLNDL].InitTouchFuncAll(&StepTristateSwitch);

        scnNode[SNZBLNDR].InitNexCon(0x1A, "Plr.pic");                // Landing light R
        scnNode[SNZBLNDR].InitNexType(AMK_INPUT | AMK_DATAUPDATE);
        scnNode[SNZBLNDR].InitOperands(IMG_SWCOBALT2);
        scnNode[SNZBLNDR].AttachSimInt("AirbusFBW/OHPLightSwitches", XPL_READWRITE, 130, 5);
        scnNode[SNZBLNDR].InitTouchFuncAll(&StepTristateSwitch);

        scnNode[SNZBRWYL].InitNexCon(0x18, "Prt.pic");                // Rwy turnoff light
        scnNode[SNZBRWYL].InitNexType(AMK_INPUT | AMK_DATAUPDATE);
        scnNode[SNZBRWYL].InitOperands(IMG_SWCOBALT0);
        scnNode[SNZBRWYL].AttachSimInt("AirbusFBW/OHPLightSwitches", XPL_READWRITE, 130, 6);
        scnNode[SNZBRWYL].InitTouchFuncAll(&ToggleBinarySwitch);

        scnNode[SNZBRWYR].InitNexCon(0x14, "Pdo.pic");                // Dome light
        scnNode[SNZBRWYR].InitNexType(AMK_INPUT | AMK_DATAUPDATE);
        scnNode[SNZBRWYR].InitOperands(IMG_SWCOBALT2);
        scnNode[SNZBRWYR].AttachSimInt("AirbusFBW/OHPLightSwitches", XPL_READWRITE, 130, 8);
        scnNode[SNZBRWYR].InitTouchFuncXP(&StepTristateSwitch);

        scnNode[SNZBTAXI].InitNexCon(0x1B, "Pno.pic");                // Nose/Taxi light
        scnNode[SNZBTAXI].InitNexType(AMK_INPUT | AMK_DATAUPDATE);
        scnNode[SNZBTAXI].InitOperands(IMG_SWCOBALT2);
        scnNode[SNZBTAXI].AttachSimInt("AirbusFBW/OHPLightSwitches", XPL_READWRITE, 130, 3);
        scnNode[SNZBTAXI].InitTouchFuncXP(&StepTristateSwitch);

        scnNode[SNZKWIPL].InitNexCon(2, "Twp.txt");                   // Wiper group. Left wiper
        scnNode[SNZKWIPL].InitNexType(AMK_SELECT);
        scnNode[SNZKWIPL].InitOperands(OD_TlWiperLeft_Dec, OD_TlWiperLeft_Inc);
        scnNode[SNZKWIPL].AttachSimInt("AirbusFBW/LeftWiperSwitch", XPL_READWRITE, 250);
        scnNode[SNZKWIPL].InitTouchFuncXP(&SelectPanelWiper);

        scnNode[SNZKWIPR].InitNexCon(2);                              // Right wiper
        scnNode[SNZKWIPR].InitNexType(AMK_SELECT);
        scnNode[SNZKWIPR].InitOperands(OD_TlWiperRight_Dec, OD_TlWiperRight_Inc);
        scnNode[SNZKWIPR].AttachSimInt("AirbusFBW/RightWiperSwitch", XPL_READWRITE, 250);

        scnNode[SNZKLNDA].InitNexCon(2);                              // Landing elev
        scnNode[SNZKLNDA].InitNexType(AMK_SELECT);
        scnNode[SNZKLNDA].AttachSimFloat("AirbusFBW/LandElev", XPL_READWRITE, 200, .1);
        scnNode[SNZKLNDA].InitOperands(OD_TlLandElev_Dec, OD_TlLandElev_Inc);
    }


// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
// PAGE: SITUATION
// Situation awareness display.

    scnNode[SNSTACDR].InitNexCon(0, "Zwn.val");                       // A/C heading text for wind vector display
    scnNode[SNSTACDR].InitNexType(AMK_INPUT);
    scnNode[SNSTACDR].InitSimIds(561);
    scnNode[SNSTACDR].InitDispFuncAll(&PlotWindComponents);
    scnNode[SNSTACDR].AttachSimFloat("sim/cockpit2/gauges/indicators/heading_electric_deg_mag_pilot", XPL_READ, 1000, 1.);

    scnNode[SNSTWVEL].InitNexCon(0);                                  // A passive node which collects wind
    scnNode[SNSTWVEL].InitNexType(AMK_INPUT);                         //  velocity input but does not display data
    scnNode[SNSTWVEL].InitSimIds(50);
    scnNode[SNSTWVEL].AttachSimFloat("sim/cockpit2/gauges/indicators/wind_speed_kts", XPL_READ, 1000, 0.5);

    scnNode[SNSTWDIR].InitNexCon(0);                                  // A passive node which collects wind
    scnNode[SNSTWDIR].InitNexType(AMK_INPUT);                         //  direction input but does not display data
    scnNode[SNSTWDIR].InitSimIds(49);
    scnNode[SNSTWDIR].AttachSimFloat("sim/cockpit2/gauges/indicators/wind_heading_deg_mag", XPL_READ, 1000, 1.);

    scnNode[SNSTVSPD].InitNexCon(0xA, "Ttd.txt");                     // VS/touchdown select text
    scnNode[SNSTVSPD].InitNexType(AMK_INPUT);                         // Disp function called from A/C altitude
    scnNode[SNSTVSPD].InitSimIds(763);
    scnNode[SNSTVSPD].InitTouchFuncAll(&ToggleVsTdownText);
    scnNode[SNSTVSPD].AttachSimFloat("sim/flightmodel/position/vh_ind_fpm2", XPL_READ, 300, 2.);

    scnNode[SNSTNFLP].InitNexCon(0, "Vfd.val");                       // Number of flaps detents text
    scnNode[SNSTNFLP].InitNexType(AMK_INPUT);
    scnNode[SNSTNFLP].InitSimIds(249);
    scnNode[SNSTNFLP].AttachSimInt("sim/aircraft/controls/acf_flap_detents", XPL_READ, 2000);

    scnNode[SNSTFLAP].InitNexCon(0, "Sfl.val");                       // Flaps text
    scnNode[SNSTFLAP].InitNexType(AMK_INPUT);
    scnNode[SNSTFLAP].InitSimIds(248);
    scnNode[SNSTFLAP].InitDispFuncAll(&DrawOnSliderScale);
    scnNode[SNSTFLAP].AttachSimFloat("sim/cockpit2/controls/flap_handle_deploy_ratio", XPL_READ, 200, 0.05);

    scnNode[SNSTTRIM].InitNexCon(0, "Str.val");                       // Trim text
    scnNode[SNSTTRIM].InitNexType(AMK_INPUT);
    scnNode[SNSTTRIM].InitSimIds(193);
    scnNode[SNSTTRIM].InitDispFuncAll(&DrawElevTrimOnScale);
    scnNode[SNSTTRIM].AttachSimFloat("sim/cockpit2/controls/elevator_trim", XPL_READ, 230, 0.01);

    scnNode[SNSTALTI].InitNexCon(0, "Tal.txt");                       // Alt indicator text
    scnNode[SNSTALTI].InitNexType(AMK_INPUT);
    scnNode[SNSTALTI].InitSimIds(558);
    scnNode[SNSTALTI].InitDispFuncAll(&ShowSelectAltitude);
    scnNode[SNSTALTI].AttachSimFloat("sim/flightmodel/position/elevation", XPL_READ, 700, 1.);

    scnNode[SNSTRALT].InitNexCon(0, "Tra.txt");                       // Radar Alt text
    scnNode[SNSTRALT].InitNexType(AMK_INPUT);
    scnNode[SNSTRALT].InitSimIds(595);
    scnNode[SNSTRALT].AttachSimFloat("sim/cockpit2/gauges/indicators/radio_altimeter_height_ft_pilot", XPL_READ, 300, 1.);

    scnNode[SNSTWPEV].InitNexCon(9);                                  // GPS waypoint elevation text
    scnNode[SNSTWPEV].InitNexType(AMK_INPUT);
    scnNode[SNSTWPEV].InitSimIds(397, -1);
    scnNode[SNSTWPEV].InitTouchFuncAll(&ToggleElevationText);

    scnNode[SNSTELEV].InitNexCon(0, "Tel.txt");                       // Elevation text
    scnNode[SNSTELEV].InitNexType(AMK_INPUT);
    scnNode[SNSTELEV].InitSimIds(410);
    scnNode[SNSTELEV].SetVarType(VT_FLOAT);
    scnNode[SNSTELEV].InitDispFuncMS(&MS_ShowSelectElevation);

    scnNode[SNSTGERF].InitNexCon(0, "Sgf.hig");                       // Front gear text
    scnNode[SNSTGERF].InitNexType(AMK_INPUT);
    scnNode[SNSTGERF].InitSimIds(320, 0);
    scnNode[SNSTGERF].InitDispFuncAll(&ShowGearIndicator);
    scnNode[SNSTGERF].AttachSimFloat("sim/flightmodel2/gear/deploy_ratio", XPL_READ, 800, 0.05, 0);

    scnNode[SNSTGERL].InitNexCon(0, "Sgl.hig");                       // Left gear text
    scnNode[SNSTGERL].InitNexType(AMK_INPUT);
    scnNode[SNSTGERL].InitSimIds(320, 1);
    scnNode[SNSTGERL].InitDispFuncAll(&ShowGearIndicator);
    scnNode[SNSTGERL].AttachSimFloat("sim/flightmodel2/gear/deploy_ratio", XPL_READ, 800, 0.05, 1);

    scnNode[SNSTGERR].InitNexCon(0, "Sgr.hig");                       // Right gear text
    scnNode[SNSTGERR].InitNexType(AMK_INPUT);
    scnNode[SNSTGERR].InitSimIds(320, 2);
    scnNode[SNSTGERR].InitDispFuncAll(&ShowGearIndicator);
    scnNode[SNSTGERR].AttachSimFloat("sim/flightmodel2/gear/deploy_ratio", XPL_READ, 800, 0.05, 2);

    scnNode[SNSTRUDR].InitNexCon(0, "Sru.val");                       // Rudder trim text
    scnNode[SNSTRUDR].InitNexType(AMK_INPUT | AMK_SELECT | AMK_SENDDIRECT);
    scnNode[SNSTRUDR].InitSimIds(659, -1);
    scnNode[SNSTRUDR].InitOperands(722, 723);
    scnNode[SNSTRUDR].AttachCommand("sim/flight_controls/rudder_trim_left", 1);
    scnNode[SNSTRUDR].AttachCommand("sim/flight_controls/rudder_trim_right", 2);
    scnNode[SNSTRUDR].InitDispFuncAll(&ShowTrimIndicatorMini);
    scnNode[SNSTRUDR].AttachSimFloat("sim/cockpit2/controls/rudder_trim", XPL_READ, 800, 0.007);

    scnNode[SNSTAILN].InitNexCon(0, "Sai.val");                       // Aileron trim text
    scnNode[SNSTAILN].InitNexType(AMK_INPUT | AMK_SELECT | AMK_SENDDIRECT);
    scnNode[SNSTAILN].InitSimIds(31, -1);
    scnNode[SNSTAILN].InitOperands(34, 35);
    scnNode[SNSTAILN].AttachCommand("sim/flight_controls/aileron_trim_left", 1);
    scnNode[SNSTAILN].AttachCommand("sim/flight_controls/aileron_trim_right", 2);
    scnNode[SNSTAILN].InitDispFuncAll(&ShowTrimIndicatorMini);
    scnNode[SNSTAILN].AttachSimFloat("sim/cockpit2/controls/aileron_trim", XPL_READ, 800, 0.007);

    scnNode[SNSTWICE].InitNexCon(0, "Tic.pco");                       // Ice text
    scnNode[SNSTWICE].InitNexType(AMK_INPUT);
    scnNode[SNSTWICE].InitSimIds(694);
    scnNode[SNSTWICE].InitDispFuncAll(&ShowIcingColor);
    scnNode[SNSTWICE].AttachSimFloat("sim/cockpit2/annunciators/ice", XPL_READ, 500, 1.);

    scnNode[SNSTREVR].InitNexCon(0, "Trv.pco");                       // Reverser text
    scnNode[SNSTREVR].InitNexType(AMK_INPUT);
    scnNode[SNSTREVR].InitSimIds(744, 1);
    scnNode[SNSTREVR].InitDispFuncAll(&ShowReverserText);
    scnNode[SNSTREVR].AttachSimFloat("sim/flightmodel2/engines/thrust_reverser_deploy_ratio", XPL_READ, 350, 0.1, 0);

    scnNode[SNSTPARK].InitNexCon(0, "Tpb.pco");                       // Parking brake text
    scnNode[SNSTPARK].InitNexType(AMK_INPUT);
    scnNode[SNSTPARK].InitSimIds(117);
    scnNode[SNSTPARK].InitDispFuncAll(&ShowParkingBrakesText);
    scnNode[SNSTPARK].AttachSimInt("sim/flightmodel/controls/parkbrake", XPL_READ, 200);

    scnNode[SNSTGSPD].InitNexCon(0xB, "Tgs.txt");                     // GS/IAS toggle text
    scnNode[SNSTGSPD].InitNexType(AMK_INPUT);
    scnNode[SNSTGSPD].InitSimIds(411);
    scnNode[SNSTGSPD].InitOperands(411, 37);                            // GS, IAS
    scnNode[SNSTGSPD].InitTouchFuncAll(&ToggleGsIasText);
    scnNode[SNSTGSPD].InitDispFuncAll(&ShowSpeedText);
    scnNode[SNSTGSPD].AttachSimFloat("sim/flightmodel2/position/groundspeed", XPL_READ, 200, 0.4);

    scnNode[SNSTIASP].InitNexCon(0);                                  // IAS text capture only
    scnNode[SNSTIASP].InitNexType(AMK_INPUT);                         // Displayed via GS node
    scnNode[SNSTIASP].InitSimIds(37);
    scnNode[SNSTIASP].AttachSimFloat("sim/flightmodel/position/indicated_airspeed", XPL_READ, 200, 0.7);

    scnNode[SNSTDMES].InitNexCon(1, "Tss.txt");                       // Current NAV selection text
    scnNode[SNSTDMES].InitTouchFuncAll(&CycleNavSelection);           // Hold no vlaue, display 1/2

    scnNode[SNSTDME1].InitNexCon(0, "Tdm.txt");                       // DME1 distance text
    scnNode[SNSTDME1].InitNexType(AMK_INPUT);
    scnNode[SNSTDME1].InitSimIds(507, 1);                               // Value, SNSTDME1
    scnNode[SNSTDME1].InitDispFuncAll(&ShowDmeDistanceText);
    scnNode[SNSTDME1].AttachSimFloat("sim/cockpit/radios/nav1_dme_dist_m", XPL_READ, 1000, 0.05);

    scnNode[SNSTDME2].InitNexCon(0, "Tdm.txt");                       // DME2 distance text
    scnNode[SNSTDME2].InitNexType(AMK_INPUT);
    scnNode[SNSTDME2].InitSimIds(507, 2);                               // Value, SNSTDME2
    scnNode[SNSTDME2].InitDispFuncAll(&ShowDmeDistanceText);
    scnNode[SNSTDME2].AttachSimFloat("sim/cockpit/radios/nav2_dme_dist_m", XPL_READ, 1000, 0.05);

    scnNode[SNSTRATF].InitNexCon(2);                                  // Radial choice text
    scnNode[SNSTRATF].InitTouchFuncAll(&ToggleToFromRadial);          // Hold no vlaue

    scnNode[SNSTRAD1].InitNexCon(0, "Tfr.txt");                       // NAV1 Radial text
    scnNode[SNSTRAD1].InitNexType(AMK_INPUT);
    scnNode[SNSTRAD1].InitSimIds(521, 1);                               // Value, SNSTRAD1
    scnNode[SNSTRAD1].InitDispFuncAll(&ShowToRadialText);
    scnNode[SNSTRAD1].AttachSimFloat("sim/cockpit2/radios/indicators/nav1_bearing_deg_mag", XPL_READ, 1000, 0.5);

    scnNode[SNSTRAD2].InitNexCon(0, "Tfr.txt");                       // NAV2 Radial text
    scnNode[SNSTRAD2].InitNexType(AMK_INPUT);
    scnNode[SNSTRAD2].InitSimIds(521, 2);                               // Value, SNSTRAD2
    scnNode[SNSTRAD2].InitDispFuncAll(&ShowToRadialText);
    scnNode[SNSTRAD2].AttachSimFloat("sim/cockpit2/radios/indicators/nav2_bearing_deg_mag", XPL_READ, 1000, 0.5);

    scnNode[SNSPRBER].InitNexCon(3, "Prb.pic");                       // Relative bearing indicator
    scnNode[SNSPRBER].InitOperands(IMG_DIRARROW);
    scnNode[SNSPRBER].InitTouchFuncAll(&ToggleNavCompass);

    if (IsZiboProfile())         // Zibo-only Situation
    {
        scnNode[SNSTRPM1].InitNexCon(0, "Z1.val");                    // Engine1 gauge value
        scnNode[SNSTRPM1].InitNexType(AMK_INPUT);
        scnNode[SNSTRPM1].InitDispFuncXP(&XP_ShowEngineGauge);
        scnNode[SNSTRPM1].AttachSimFloat("laminar/B738/engine/indicators/N1_percent_1", XPL_READ, 70, 1.);

        scnNode[SNSTRPM2].InitNexCon(0, "Z2.val");                    // Engine2 gauge value
        scnNode[SNSTRPM2].InitNexType(AMK_INPUT);
        scnNode[SNSTRPM2].InitDispFuncXP(&XP_ShowEngineGauge);
        scnNode[SNSTRPM2].AttachSimFloat("laminar/B738/engine/indicators/N1_percent_2", XPL_READ, 70, 1.);

        scnNode[SNSTRETG].InitNexCon(0x12);                           // Gears toggle
        scnNode[SNSTRETG].InitNexType(AMK_DATAUPDATE);
        scnNode[SNSTRETG].AttachSimInt("laminar/B738/controls/gear_handle_down", XPL_WRITE, 500);
        scnNode[SNSTRETG].InitTouchFuncXP(&ToggleBinarySwitch);

        scnNode[SNSTSBRK].InitNexCon(0, "Ssb.val");                   // Speedbrakes text
        scnNode[SNSTSBRK].InitNexType(AMK_INPUT);
        scnNode[SNSTSBRK].InitDispFuncXP(&ZB_ShowSpeedbrakes);
        scnNode[SNSTSBRK].AttachSimFloat("sim/flightmodel2/wing/speedbrake1_deg", XPL_READ, 280, 10., 0);

        scnNode[SNSTSBRA].InitNexCon(0, "Tsb.pco");                   // Speedbrakes armed text
        scnNode[SNSTSBRA].InitNexType(AMK_INPUT);
        scnNode[SNSTSBRA].InitDispFuncXP(&ShowSpeedbrakesArmedLabel);
        scnNode[SNSTSBRA].AttachSimFloat("sim/cockpit2/controls/speedbrake_ratio", XPL_READ, 400);

        scnNode[SNSTDIST].InitNexCon(0, "Tdm.txt");                   // LNAV distance text
        scnNode[SNSTDIST].InitNexType(AMK_INPUT);
        scnNode[SNSTDIST].AttachSimFloat("laminar/B738/fms/lnav_dist_next", XPL_READ, 1000, 0.05);

        scnNode[SNSTBEAR].InitNexCon(0, "Tfr.txt");                   // GPS bearing text
        scnNode[SNSTBEAR].InitNexType(AMK_INPUT);
        scnNode[SNSTBEAR].AttachSimFloat("laminar/B738/fms/gps_course_degtm", XPL_READ, 1000, 1.);

        // Also included by the Navigate page
        scnNode[SNZTBARO].InitNexCon(0x11, "Tba.txt");                // Barometer text
        scnNode[SNZTBARO].InitNexType(AMK_INPUT | AMK_SENDDIRECT);
        scnNode[SNZTBARO].AttachSimFloat("sim/cockpit/misc/barometer_setting", XPL_READ, 120, 0.005);
        scnNode[SNZTBARO].AttachCommand("laminar/B738/EFIS_control/capt/push_button/std_press", 0);
        scnNode[SNZTBARO].AttachCommand("laminar/B738/pilot/barometer_down", 1);
        scnNode[SNZTBARO].AttachCommand("laminar/B738/pilot/barometer_up", 2);
        scnNode[SNZTBARO].InitDispFuncXP(&ShowBarometerText);
    }
    else          // FS2020 or non-Zibo Situation
    {
        scnNode[SNSTTYPE].InitNexCon(0);                              // Engine type
        scnNode[SNSTTYPE].InitNexType(AMK_INPUT);
        scnNode[SNSTTYPE].InitSimIds(234, -1);
        scnNode[SNSTTYPE].AttachSimInt("sim/aircraft/prop/acf_en_type", XPL_READ, 2000);

        scnNode[SNSTRPM1].InitNexCon(0, "Z1.val");                    // Engine1 gauge value
        scnNode[SNSTRPM1].InitNexType(AMK_INPUT);
        scnNode[SNSTRPM1].InitSimIds(224, 1);
        scnNode[SNSTRPM1].InitDispFuncMS(&MS_ShowEngineGauge);
        scnNode[SNSTRPM1].InitDispFuncXP(&XP_ShowEngineGauge);
        scnNode[SNSTRPM1].AttachSimFloat("sim/cockpit2/engine/indicators/engine_speed_rpm", XPL_READ, 70, 12., 0);
        // Coupled nodes--must follow above in the .H file
        scnNode[SNSTLIM1].InitNexCon(0);                              // Engine1 RPM limit value
        scnNode[SNSTLIM1].InitNexType(AMK_INPUT);                     // Hold value, not on screen
        scnNode[SNSTLIM1].AttachSimFloat("sim/aircraft/prop/acf_des_rpm_prp", XPL_READ, 2000, 10., 0);

        scnNode[SNSTRPM2].InitNexCon(0, "Z2.val");                    // Engine2 gauge value
        scnNode[SNSTRPM2].InitNexType(AMK_INPUT);
        scnNode[SNSTRPM2].InitSimIds(224, 2);
        scnNode[SNSTRPM2].InitDispFuncMS(&MS_ShowEngineGauge);
        scnNode[SNSTRPM2].InitDispFuncXP(&XP_ShowEngineGauge);
        scnNode[SNSTRPM2].AttachSimFloat("sim/cockpit2/engine/indicators/engine_speed_rpm", XPL_READ, 70, 12., 1);
        // Coupled nodes--must follow above in the .H file
        scnNode[SNSTLIM2].InitNexCon(0);                              // Engine2 RPM limit value
        scnNode[SNSTLIM2].InitNexType(AMK_INPUT);                     // Hold value, not on screen
        scnNode[SNSTLIM2].AttachSimFloat("sim/aircraft/prop/acf_des_rpm_prp", XPL_READ, 2000, 10., 1);

        scnNode[SNSTN1P1].InitNexCon(0);                              // Engine1 N1
        scnNode[SNSTN1P1].InitNexType(AMK_INPUT);
        scnNode[SNSTN1P1].InitSimIds(216, 1);
        scnNode[SNSTN1P1].AttachSimFloat("sim/cockpit2/engine/indicators/N1_percent", XPL_READ, 80, 3., 0);

        scnNode[SNSTN1P2].InitNexCon(0);                              // Engine2 N1
        scnNode[SNSTN1P2].InitNexType(AMK_INPUT);
        scnNode[SNSTN1P2].InitSimIds(216, 2);
        scnNode[SNSTN1P2].AttachSimFloat("sim/cockpit2/engine/indicators/N1_percent", XPL_READ, 80, 3., 1);

        scnNode[SNSTRETG].InitNexCon(0x12);                               // Gears retractable?
        scnNode[SNSTRETG].InitNexType(AMK_INPUT | AMK_SENDDIRECT);
        scnNode[SNSTRETG].InitSimIds(439, -1, 399);
        scnNode[SNSTRETG].AttachSimInt("sim/aircraft/gear/acf_gear_retract", XPL_READ, 5000);
        scnNode[SNSTRETG].AttachCommand("sim/flight_controls/landing_gear_toggle", 0);

        scnNode[SNSTSBAV].InitNexCon(0);                              // Speedbrake available?
        scnNode[SNSTSBAV].InitNexType(AMK_INPUT);                     // Fetch for its value only
        scnNode[SNSTSBAV].InitSimIds(679);                            // MSFS-only

        scnNode[SNSTSBRK].InitNexCon(0, "Ssb.val");                   // Speedbrakes text
        scnNode[SNSTSBRK].InitNexType(AMK_INPUT);
        scnNode[SNSTSBRK].InitSimIds(681);
        scnNode[SNSTSBRK].AttachSimFloat("sim/cockpit2/controls/speedbrake_ratio", XPL_READ, 250, 0.1);
        scnNode[SNSTSBRK].InitDispFuncAll(&PlotSpeedbrakesPercent);

        scnNode[SNSTSBRA].InitNexCon(0, "Tsb.pco");                   // Speedbrakes armed text
        scnNode[SNSTSBRA].InitNexType(AMK_INPUT);
        scnNode[SNSTSBRA].InitSimIds(680);
        scnNode[SNSTSBRA].InitDispFuncAll(&ShowSpeedbrakesArmedLabel);
        scnNode[SNSTSBRA].AttachSimFloat("sim/cockpit2/controls/speedbrake_ratio", XPL_READ, 900, 0.1);

        scnNode[SNSTDIST].InitNexCon(0, "Tdm.txt");                   // GPS distance text
        scnNode[SNSTDIST].InitNexType(AMK_INPUT);
        scnNode[SNSTDIST].InitSimIds(394, -1);
        scnNode[SNSTDIST].InitDispFuncAll(&ShowDmeDistanceText);
        scnNode[SNSTDIST].AttachSimFloat("sim/cockpit/radios/gps_dme_dist_m", XPL_READ, 1000, 0.05);

        scnNode[SNSTBEAR].InitNexCon(0, "Tfr.txt");                   // GPS bearing text
        scnNode[SNSTBEAR].InitNexType(AMK_INPUT);
        scnNode[SNSTBEAR].InitSimIds(391, -1);
        scnNode[SNSTBEAR].InitDispFuncAll(&ShowToRadialText);
        scnNode[SNSTBEAR].AttachSimFloat("sim/cockpit2/radios/indicators/gps_bearing_deg_mag", XPL_READ, 1000, 1.);

        scnNode[SNSTOVSP].InitNexCon(0, "Tgs.bco");                   // Overspeed text
        scnNode[SNSTOVSP].InitNexType(AMK_INPUT);
        scnNode[SNSTOVSP].InitSimIds(532, -1);

        scnNode[SNSTBARO].InitNexCon(0x11, "Tba.txt");                // Barometer text
        scnNode[SNSTBARO].InitNexType(AMK_INPUT | AMK_SENDDIRECT);
        scnNode[SNSTBARO].InitSimIds(448, -1, 187);                          // Value, Sync
        scnNode[SNSTBARO].InitOperands(455, 456);
        scnNode[SNSTBARO].AttachSimFloat("sim/cockpit/misc/barometer_setting", XPL_READWRITE, 100, 0.01);
        scnNode[SNSTBARO].AttachCommand("sim/instruments/barometer_2992", 0);
        scnNode[SNSTBARO].AttachCommand("sim/instruments/barometer_down", 1);
        scnNode[SNSTBARO].AttachCommand("sim/instruments/barometer_up", 2);
        scnNode[SNSTBARO].InitDispFuncAll(&ShowBarometerText);
    }


// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
// PAGE: GPS
// Buttons and knobs for the X-Plane G1000 GPS 

    if (IsXPlane() && !IsZiboProfile())
    {
        scnNode[SNG1BPNS].InitNexCon(0x10, "Ppn.pic");                // Pan select
        scnNode[SNG1BPNS].InitOperands(IMG_GPSPANLR);                 // The arrow image
        scnNode[SNG1BPNS].InitTouchFuncXP(&XP_FlipPanDirect);

        scnNode[SNG1KPLR].InitNexCon(0x11);                           // Pan L/R
        scnNode[SNG1KPLR].InitNexType(AMK_SENDDIRECT);
        scnNode[SNG1KPLR].AttachCommand("sim/GPS/g1000n3_pan_push", 0);
        scnNode[SNG1KPLR].AttachCommand("sim/GPS/gcu478/pan_left", 1);
        scnNode[SNG1KPLR].AttachCommand("sim/GPS/gcu478/pan_right", 2);

        scnNode[SNG1KPUD].InitNexCon(0x12);                           // Pan U/D
        scnNode[SNG1KPUD].InitNexType(AMK_SENDDIRECT);
        scnNode[SNG1KPUD].AttachCommand("sim/GPS/gcu478/pan_down", 1);
        scnNode[SNG1KPUD].AttachCommand("sim/GPS/gcu478/pan_up", 2);

        scnNode[SNG1KRNG].InitNexCon(0x13);                           // Range
        scnNode[SNG1KRNG].InitNexType(AMK_SENDDIRECT);
        scnNode[SNG1KRNG].AttachCommand("sim/GPS/gcu478/range_down", 1);
        scnNode[SNG1KRNG].AttachCommand("sim/GPS/gcu478/range_up", 2);

        scnNode[SNG1KFOU].InitNexCon(0x14);                           // FMS outter
        scnNode[SNG1KFOU].InitNexType(AMK_SENDDIRECT);
        scnNode[SNG1KFOU].AttachCommand("sim/GPS/gcu478/outer_down", 1);
        scnNode[SNG1KFOU].AttachCommand("sim/GPS/gcu478/outer_up", 2);

        scnNode[SNG1KFIN].InitNexCon(0x15);                           // FMS inner
        scnNode[SNG1KFIN].InitNexType(AMK_SENDDIRECT);
        scnNode[SNG1KFIN].AttachCommand("sim/GPS/gcu478/cursor", 0);
        scnNode[SNG1KFIN].AttachCommand("sim/GPS/gcu478/inner_down", 1);
        scnNode[SNG1KFIN].AttachCommand("sim/GPS/gcu478/inner_up", 2);

        scnNode[SNG1BSKN].InitNexCon(0x18);                           // Softkey array
        scnNode[SNG1BSKN].InitNexType(AMK_SENDDIRECT);                //  of 12 elements
        scnNode[SNG1BSKN].InitSimIds(0, 12);
        scnNode[SNG1BSKN].SetValue((int32_t) &cmd_1000Softkey);
        scnNode[SNG1BSKN].InitTouchFuncXP(&XP_SendOpcodeFromArray);
        cmd_1000Softkey[0]  = XPDplugin.registerCommand("sim/GPS/g1000n3_softkey1");
        cmd_1000Softkey[1]  = XPDplugin.registerCommand("sim/GPS/g1000n3_softkey2");
        cmd_1000Softkey[2]  = XPDplugin.registerCommand("sim/GPS/g1000n3_softkey3");
        cmd_1000Softkey[3]  = XPDplugin.registerCommand("sim/GPS/g1000n3_softkey4");
        cmd_1000Softkey[4]  = XPDplugin.registerCommand("sim/GPS/g1000n3_softkey5");
        cmd_1000Softkey[5]  = XPDplugin.registerCommand("sim/GPS/g1000n3_softkey6");
        cmd_1000Softkey[6]  = XPDplugin.registerCommand("sim/GPS/g1000n3_softkey7");
        cmd_1000Softkey[7]  = XPDplugin.registerCommand("sim/GPS/g1000n3_softkey8");
        cmd_1000Softkey[8]  = XPDplugin.registerCommand("sim/GPS/g1000n3_softkey9");
        cmd_1000Softkey[9]  = XPDplugin.registerCommand("sim/GPS/g1000n3_softkey10");
        cmd_1000Softkey[10] = XPDplugin.registerCommand("sim/GPS/g1000n3_softkey11");
        cmd_1000Softkey[11] = XPDplugin.registerCommand("sim/GPS/g1000n3_softkey12");

        scnNode[SNG1BKP1].InitNexCon(2);                              // Keypad pressed
        scnNode[SNG1BKP1].InitNexType(AMK_XPPRESSED);                 //  of 6 elements
        scnNode[SNG1BKP1].InitSimIds(0, 6);
        scnNode[SNG1BKP1].SetValue((int32_t) &cmd_1000Keypad);
        scnNode[SNG1BKP1].InitTouchFuncXP(&XP_SendOpcodeFromArray);
        cmd_1000Keypad[0] = XPDplugin.registerCommand("sim/GPS/gcu478/direct");
        cmd_1000Keypad[1] = XPDplugin.registerCommand("sim/GPS/gcu478/menu");
        cmd_1000Keypad[2] = XPDplugin.registerCommand("sim/GPS/gcu478/fpl");
        cmd_1000Keypad[3] = XPDplugin.registerCommand("sim/GPS/gcu478/proc");
        cmd_1000Keypad[4] = XPDplugin.registerCommand("sim/GPS/gcu478/clr");
        cmd_1000Keypad[5] = XPDplugin.registerCommand("sim/GPS/gcu478/ent");

        scnNode[SNG1BKP0].InitNexCon(3);                              // Keypad released
        scnNode[SNG1BKP0].InitNexType(AMK_XPRELEASED);                //  of 6 elements
        scnNode[SNG1BKP0].InitSimIds(0, 6);
        scnNode[SNG1BKP0].SetValue((int32_t)&cmd_1000Keypad);
        scnNode[SNG1BKP0].InitTouchFuncXP(&XP_SendOpcodeFromArray);


        // = = Buttons and knobs for the X-Plane G530 GPS = =

        scnNode[SNGB1SEL].InitNexCon(0x31);                           // G1000 select
        scnNode[SNGB1SEL].InitTouchFuncXP(&XP_FlipGpsControls);

        scnNode[SNGB5SEL].InitNexCon(0x32, "Bg5.pco");                // G530 select
        scnNode[SNGB5SEL].InitTouchFuncXP(&XP_FlipGpsControls);
                                                                      
        scnNode[SNGB4SEL].InitNexCon(0x33, "Bg4.pco");                // G430 select
        scnNode[SNGB4SEL].InitTouchFuncXP(&XP_FlipGpsControls);

        scnNode[SNG5KCOO].InitNexCon(0x11);                           // Freq coarse
        scnNode[SNG5KCOO].InitNexType(AMK_SENDDIRECT);
        scnNode[SNG5KCOO].SetValue((int32_t) &cmd_530Coarse);
        cmd_530Coarse[1] = XPDplugin.registerCommand("sim/GPS/g430n1_coarse_down");
        cmd_530Coarse[2] = XPDplugin.registerCommand("sim/GPS/g430n1_coarse_up");
        cmd_530Coarse[4] = XPDplugin.registerCommand("sim/GPS/g430n2_coarse_down");
        cmd_530Coarse[5] = XPDplugin.registerCommand("sim/GPS/g430n2_coarse_up");

        scnNode[SNG5KCOI].InitNexCon(0x12);                           // Freq fine
        scnNode[SNG5KCOI].InitNexType(AMK_SENDDIRECT);
        scnNode[SNG5KCOI].SetValue((int32_t) &cmd_530Fine);
        cmd_530Fine[0] = XPDplugin.registerCommand("sim/GPS/g430n1_nav_com_tog");
        cmd_530Fine[1] = XPDplugin.registerCommand("sim/GPS/g430n1_fine_down");
        cmd_530Fine[2] = XPDplugin.registerCommand("sim/GPS/g430n1_fine_up");
        cmd_530Fine[3] = XPDplugin.registerCommand("sim/GPS/g430n2_nav_com_tog");
        cmd_530Fine[4] = XPDplugin.registerCommand("sim/GPS/g430n2_fine_down");
        cmd_530Fine[5] = XPDplugin.registerCommand("sim/GPS/g430n2_fine_up");

        scnNode[SNG5KGPO].InitNexCon(0x13);                           // GPS outter
        scnNode[SNG5KGPO].InitNexType(AMK_SENDDIRECT);
        scnNode[SNG5KGPO].SetValue((int32_t) &cmd_530Outter);
        cmd_530Outter[1] = XPDplugin.registerCommand("sim/GPS/g430n1_chapter_dn");
        cmd_530Outter[2] = XPDplugin.registerCommand("sim/GPS/g430n1_chapter_up");
        cmd_530Outter[4] = XPDplugin.registerCommand("sim/GPS/g430n2_chapter_dn");
        cmd_530Outter[5] = XPDplugin.registerCommand("sim/GPS/g430n2_chapter_up");

        scnNode[SNG5KGPI].InitNexCon(0x14);                           // GPS inner
        scnNode[SNG5KGPI].InitNexType(AMK_SENDDIRECT);
        scnNode[SNG5KGPI].SetValue((int32_t)&cmd_530Inner);
        cmd_530Inner[0] = XPDplugin.registerCommand("sim/GPS/g430n1_cursor");
        cmd_530Inner[1] = XPDplugin.registerCommand("sim/GPS/g430n1_page_dn");
        cmd_530Inner[2] = XPDplugin.registerCommand("sim/GPS/g430n1_page_up");
        cmd_530Inner[3] = XPDplugin.registerCommand("sim/GPS/g430n2_cursor");
        cmd_530Inner[4] = XPDplugin.registerCommand("sim/GPS/g430n2_page_dn");
        cmd_530Inner[5] = XPDplugin.registerCommand("sim/GPS/g430n2_page_up");

        scnNode[SNG5BSKN].InitNexCon(0x18);                           // Softkey array
        scnNode[SNG5BSKN].InitNexType(AMK_SENDDIRECT);                //  of 6 elements
        scnNode[SNG5BSKN].InitSimIds(1, 6);
        scnNode[SNG5BSKN].SetValue((int32_t) &cmd_530Softkey);
        scnNode[SNG5BSKN].InitTouchFuncXP(&XP_SendOpcodeFromArray);
        cmd_530Softkey[0] = XPDplugin.registerCommand("sim/GPS/g430n1_cdi");
        cmd_530Softkey[1] = XPDplugin.registerCommand("sim/GPS/g430n1_obs");
        cmd_530Softkey[2] = XPDplugin.registerCommand("sim/GPS/g430n1_msg");
        cmd_530Softkey[3] = XPDplugin.registerCommand("sim/GPS/g430n1_fpl");
        cmd_530Softkey[4] = XPDplugin.registerCommand("sim/GPS/g430n1_vnav");
        cmd_530Softkey[5] = XPDplugin.registerCommand("sim/GPS/g430n1_proc");
        cmd_530Softkey[6] = XPDplugin.registerCommand("sim/GPS/g430n2_cdi");
        cmd_530Softkey[7] = XPDplugin.registerCommand("sim/GPS/g430n2_obs");
        cmd_530Softkey[8] = XPDplugin.registerCommand("sim/GPS/g430n2_msg");
        cmd_530Softkey[9] = XPDplugin.registerCommand("sim/GPS/g430n2_fpl");
        cmd_530Softkey[10] = XPDplugin.registerCommand("sim/GPS/g430n2_vnav");
        cmd_530Softkey[11] = XPDplugin.registerCommand("sim/GPS/g430n2_proc");

        scnNode[SNG5BKP1].InitNexCon(0x20);                           // Keypad pressed
        scnNode[SNG5BKP1].InitNexType(AMK_XPPRESSED);                 //  of 4 elements
        scnNode[SNG5BKP1].InitSimIds(1, 4);
        scnNode[SNG5BKP1].SetValue((int32_t) &cmd_530Keypad);
        scnNode[SNG5BKP1].InitTouchFuncXP(&XP_SendOpcodeFromArray);
        cmd_530Keypad[0] = XPDplugin.registerCommand("sim/GPS/g430n1_direct");
        cmd_530Keypad[1] = XPDplugin.registerCommand("sim/GPS/g430n1_menu");
        cmd_530Keypad[2] = XPDplugin.registerCommand("sim/GPS/g430n1_clr");
        cmd_530Keypad[3] = XPDplugin.registerCommand("sim/GPS/g430n1_ent");
        cmd_530Keypad[4] = XPDplugin.registerCommand("sim/GPS/g430n2_direct");
        cmd_530Keypad[5] = XPDplugin.registerCommand("sim/GPS/g430n2_menu");
        cmd_530Keypad[6] = XPDplugin.registerCommand("sim/GPS/g430n2_clr");
        cmd_530Keypad[7] = XPDplugin.registerCommand("sim/GPS/g430n2_ent");

        scnNode[SNG5BKP0].InitNexCon(0x21);                           // Keypad released
        scnNode[SNG5BKP0].InitNexType(AMK_XPRELEASED);                //  of 4 elements
        scnNode[SNG5BKP0].InitSimIds(1, 4);
        scnNode[SNG5BKP0].SetValue((int32_t) &cmd_530Keypad);
        scnNode[SNG5BKP0].InitTouchFuncXP(&XP_SendOpcodeFromArray);

        scnNode[SNG5BRG1].InitNexCon(0x28);                           // Range key array
        scnNode[SNG5BRG1].InitNexType(AMK_XPPRESSED);                 //  of 4 elements
        scnNode[SNG5BRG1].InitSimIds(1, 4);
        scnNode[SNG5BRG1].SetValue((int32_t) &cmd_530KeyRange);
        scnNode[SNG5BRG1].InitTouchFuncXP(&XP_SendOpcodeFromArray);
        cmd_530KeyRange[0] = XPDplugin.registerCommand("sim/GPS/g430n1_com_ff");
        cmd_530KeyRange[1] = XPDplugin.registerCommand("sim/GPS/g430n1_nav_ff");
        cmd_530KeyRange[2] = XPDplugin.registerCommand("sim/GPS/g430n1_zoom_in");
        cmd_530KeyRange[3] = XPDplugin.registerCommand("sim/GPS/g430n1_zoom_out");
        cmd_530KeyRange[4] = XPDplugin.registerCommand("sim/GPS/g430n2_com_ff");
        cmd_530KeyRange[5] = XPDplugin.registerCommand("sim/GPS/g430n2_nav_ff");
        cmd_530KeyRange[6] = XPDplugin.registerCommand("sim/GPS/g430n2_zoom_in");
        cmd_530KeyRange[7] = XPDplugin.registerCommand("sim/GPS/g430n2_zoom_out");

        scnNode[SNG5BRG0].InitNexCon(0x29);                           // Range key released
        scnNode[SNG5BRG0].InitNexType(AMK_XPRELEASED);                //  of 4 elements
        scnNode[SNG5BRG0].InitSimIds(1, 4);
        scnNode[SNG5BRG0].SetValue((int32_t)&cmd_530KeyRange);
        scnNode[SNG5BRG0].InitTouchFuncXP(&XP_SendOpcodeFromArray);
    }


// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
// PAGE: MORE
// Contain additional control and diagnostic pages.

    scnNode[SNMORB01].InitNexCon(1);
    scnNode[SNMORB01].InitNexType(AMK_PAGING);
    scnNode[SNMORB02].InitNexCon(2);
    scnNode[SNMORB02].InitNexType(AMK_PAGING);
    scnNode[SNMORB03].InitNexCon(3);
    scnNode[SNMORB03].InitNexType(AMK_PAGING);
    scnNode[SNMORB04].InitNexCon(4);
    scnNode[SNMORB04].InitNexType(AMK_PAGING);
    scnNode[SNMORB05].InitNexCon(5);
    scnNode[SNMORB05].InitNexType(AMK_PAGING);
    scnNode[SNMORB06].InitNexCon(6);
    scnNode[SNMORB06].InitNexType(AMK_PAGING);
    scnNode[SNMORB07].InitNexCon(7);
    scnNode[SNMORB07].InitNexType(AMK_PAGING);
    scnNode[SNMORB08].InitNexCon(8);
    scnNode[SNMORB08].InitNexType(AMK_PAGING);
    scnNode[SNMORB09].InitNexCon(9);
    scnNode[SNMORB09].InitNexType(AMK_PAGING);
    scnNode[SNMORB10].InitNexCon(10);
    scnNode[SNMORB10].InitNexType(AMK_PAGING);
    scnNode[SNMORB11].InitNexCon(11);
    scnNode[SNMORB11].InitNexType(AMK_PAGING);
    scnNode[SNMORB12].InitNexCon(12);
    scnNode[SNMORB12].InitNexType(AMK_PAGING);


// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
// PAGE: SIM OPTIONS
// Simulator options.

    scnNode[SNOTHOUR].InitNexCon(1, "Thr.txt");                       // Sim Hour text
    scnNode[SNOTHOUR].InitNexType(AMK_INPUT | AMK_SELECT);
    scnNode[SNOTHOUR].InitSimIds(486);                                  // Value
    scnNode[SNOTHOUR].InitOperands(OC_TimeDnFast, OC_TimeUpFast);
    scnNode[SNOTHOUR].InitDispFuncAll(&ShowHourMinute);
    scnNode[SNOTHOUR].AttachSimFloat("sim/time/local_time_sec", XPL_READ, 100, 1.);

    scnNode[SNOTMINU].InitNexCon(0, "");                              // Sim Minute text
    scnNode[SNOTMINU].InitNexType(AMK_INPUT | AMK_SENDDIRECT);        // This item coupled to above
    scnNode[SNOTMINU].InitOperands(201, 202);
    scnNode[SNOTMINU].AttachCommand("sim/operation/time_down_lots", 1);
    scnNode[SNOTMINU].AttachCommand("sim/operation/time_up_lots", 2);

    scnNode[SNOTACCE].InitNexCon(0, "Tsr.txt");                       // Sim rate text
    scnNode[SNOTACCE].InitNexType(AMK_INPUT | AMK_SELECT);
    scnNode[SNOTACCE].InitSimIds(670);
    scnNode[SNOTACCE].InitOperands(OD_Accelerate_Dec, OD_Accelerate_Inc);
    scnNode[SNOTACCE].AttachSimFloat("sim/time/ground_speed_flt", XPL_READWRITE, 120, 0.1);
    scnNode[SNOTACCE].InitDispFuncXP(&XP_ShowTimeAccel);
    scnNode[SNOTACCE].InitFmtCtrl("%4.1f");

    if (IsXPlane())
    {
        scnNode[SNOTVOLM].InitNexCon(0, "Tvo.txt");                   // Volume
        scnNode[SNOTVOLM].InitNexType(AMK_INPUT | AMK_SELECT);
        scnNode[SNOTVOLM].InitOperands(OD_Volume_Dec, OD_Volume_Inc);
        scnNode[SNOTVOLM].AttachSimFloat("sim/operation/sound/master_volume_ratio", XPL_READWRITE, 100, 0);
        scnNode[SNOTVOLM].InitDispFuncXP(&XP_ShowVolumeLevel);
    }
 

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
// PAGE: CONFIG
// T.M.P. system configuration.

    scnNode[SNFTBRIG].InitNexCon(0, "Tsb.txt");                       // Brightness text
    scnNode[SNFTBRIG].InitNexType(AMK_INPUT | AMK_SYSTEM | AMK_SELECT);
    scnNode[SNFTBRIG].AttachCommand(LBC_SHW_BRIGHTNESS, 0);
    scnNode[SNFTBRIG].InitOperands(LBC_DEC_BRIGHTNESS, LBC_INC_BRIGHTNESS);

    scnNode[SNFTDIMM].InitNexCon(2, "Tsd.txt");                       // Screen dim text
    scnNode[SNFTDIMM].InitNexType(AMK_INPUT| AMK_SYSTEM | AMK_SELECT);
    scnNode[SNFTDIMM].AttachCommand(LBC_SHW_SCREENDIM, 0);
    scnNode[SNFTDIMM].InitOperands(LBC_DEC_SCREENDIM, LBC_INC_SCREENDIM);
    scnNode[SNFTDIMM].InitTouchFuncAll(&DisplayDimNow);               // Dim display now

    scnNode[SNFTBQNH].InitNexCon(0, "Tdu.txt");                       // Display units text
    scnNode[SNFTBQNH].InitNexType(AMK_INPUT| AMK_SYSTEM | AMK_SELECT);
    scnNode[SNFTBQNH].AttachCommand(LBC_SHW_DISPUNIT, 0);
    scnNode[SNFTBQNH].InitOperands(LBC_DEC_DISPUNIT, LBC_INC_DISPUNIT);

    scnNode[SNFBPROF].InitNexCon(LBC_NXOP_PROFILE, "Pcl.pic");        // Profile
    scnNode[SNFBPROF].InitNexType(AMK_INPUT | AMK_SYSTEM);
    scnNode[SNFBPROF].AttachCommand(LBC_SHW_PROFILE, 0);
    scnNode[SNFBPROF].InitOperands(IMG_PROFILE);
    scnNode[SNFBPROF].InitTouchFuncAll(&CycleProfile);

    scnNode[SNFBRSET].InitNexCon(5);                                  // Restart
    scnNode[SNFBRSET].InitTouchFuncAll(&MiniPanelRestart);

    scnNode[SNFBACPT].InitNexCon(6, "Bav.pco");                       // Activate
    scnNode[SNFBACPT].InitTouchFuncAll(&ActivProfile);

    if (IsXPlane())
    {
        scnNode[SNFTICAO].InitNexCon(0, "Tac.txt");                       // A/C ICAO
        scnNode[SNFTICAO].InitNexType(AMK_INPUT);
        scnNode[SNFTICAO].AttachSimString("sim/aircraft/view/acf_ICAO", 500);
        scnNode[SNFTICAO].InitFmtCtrl("%s");
    }
}
