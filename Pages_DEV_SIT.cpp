// Functions to implement the operations for:
//   Device and Panel control Pages
//   Situation Page
//
// Discman777    08/24/2021
// Version 1.08.25
// This freeware is provided “AS IS”.  Non-commercial use, modification, distribution at your own risk.


#include <arduino.h>
#include <GlobalVars.h>
#include <Supports.h>
#include <DefineNODEs.h>
#include <DefinePAGEs.h>>
#include <DefinePROFILEs.h>>
#include <Paging.h>
#include <SimExchange.h>
#include <Pages_DEV_SIT.h>
#include <Pages_AP_NAV.h>


// - = - = - = - = - = - = -  Functions for ZIBO PANEL Page  - = - = - = - = - = - = -
// Custom functions for the Devices and Panel control features
//
// Select the default group in the generic Devices Page.
void SelectDevicesPrimary(int16_t dummy)
{
    page[PG_MS_DEVICES].SetEncoAttach(0, SNDKKEY1);     // Starter1 key
    page[PG_MS_DEVICES].SetEncoAttach(1, SNDKKEY2);     // Starter2 key
    page[PG_MS_DEVICES].SetEncoAttach(2, INVLD_NODEINX);
    page[PG_MS_DEVICES].SetEncoAttach(3, INVLD_NODEINX);

    page[PG_XP_DEVICES].SetEncoAttach(0, SNDKKEY1);     // Starter1 key
    page[PG_XP_DEVICES].SetEncoAttach(1, SNDKKEY2);     // Starter2 key
    page[PG_XP_DEVICES].SetEncoAttach(2, INVLD_NODEINX);
    page[PG_XP_DEVICES].SetEncoAttach(3, INVLD_NODEINX);
}


// Select the alternating groups in the Zibo Panel Page.
void SelectPanelLights(int16_t dummy)
{
    page[PG_ZB_PANEL1].SetEncoAttach(0, SNZKPANL);     // Light brightness group
    page[PG_ZB_PANEL1].SetEncoAttach(1, SNZKINTL);
    page[PG_ZB_PANEL1].SetEncoAttach(2, SNZKDISP);
    page[PG_ZB_PANEL1].SetEncoAttach(3, SNZKFMC1);

    page[PG_TL_PANEL].SetEncoAttach(0, SNZKPANL);      // For ToLiss
    page[PG_TL_PANEL].SetEncoAttach(1, SNZKINTL);
    page[PG_TL_PANEL].SetEncoAttach(2, SNZKDISP);
    page[PG_TL_PANEL].SetEncoAttach(3, SNZKFMC1);
    HighlightSelection();
    g_bAtPrimaryEnco = true;        // Signal encoder snapback not needed
}


// Secondary group
void SelectPanelWiper(int16_t dummy)
{
    page[PG_ZB_PANEL1].SetEncoAttach(0, SNZKWIPL);     // Overhead items group
    page[PG_ZB_PANEL1].SetEncoAttach(1, SNZKWIPR);
    page[PG_ZB_PANEL1].SetEncoAttach(2, SNZKFLTA);
    page[PG_ZB_PANEL1].SetEncoAttach(3, SNZKLNDA);
    
    page[PG_TL_PANEL].SetEncoAttach(0, SNZKWIPL);      // For ToLiss
    page[PG_TL_PANEL].SetEncoAttach(1, SNZKWIPR);
    page[PG_TL_PANEL].SetEncoAttach(2, SNZKLNDA);
    page[PG_TL_PANEL].SetEncoAttach(3, INVLD_NODEINX);
    HighlightSelection();
    g_bAtPrimaryEnco = false;        // Signal encoder snapback pending
}


// - = - = - = - = - = - = -  Functions for SITUATION Page  - = - = - = - = - = - = -
// Custom functions for the Situation Awareness features.

#define NUM_SCALE_DETS    15       // Number of detents in the graphic vertical scale
#define CLR_TEXT_LABEL    RGB16(15, 47, 23)   // 32247
#define CLR_WARNING_NONE  RGB16(12, 25, 12)   // 25388
#define CLR_ICING_LOW     RGB16(31, 63, 16)   // 65520


// Set up the screen items when entering the Situation page.
void EntrySetup_Situation(void)
{
}


// Service for selecting the applicable values in Situation Page.
void SelectAllSituation(int16_t dummy)
{
    page[PG_SITUATION].SetEncoAttach(0, SNSTBARO);       // Barometer setting
    page[PG_SITUATION].SetEncoAttach(1, SNSTRUDR);       // Rudder trim
    if (!IsToLissProfile())
    {
        page[PG_SITUATION].SetEncoAttach(2, SNSTAILN);   // Aileron trim
    }
    else
    {
        // Hide the dot prompt for rotary #3
        nexIo.SendAction("Pai.pic", String(IMG_ROTARYDOT + 5));
    }

    page[PG_ZB_SITU].SetEncoAttach(0, SNZTBARO);         // Barometer setting
    page[PG_ZB_SITU].SetEncoAttach(1, SNSTRUDR);         // Rudder trim
    page[PG_ZB_SITU].SetEncoAttach(2, SNSTAILN);         // Aileron trim
}


// Toggle the Position light between strobe and off.
void XP_TogglePositionLight(int16_t scnInx)
{
    // Switch the strobe light based on current state
    if (scnNode[scnInx].GetIntValue() == 0)
    {
        NodeToSimCommon(scnInx, 2);
    }
    else
    {
        NodeToSimCommon(scnInx, 1);
    }
}



// This function takes care of the graphic relative wind display.  It fetches the
//  sim data to perform the calculations, and display the headwind and crosswind at the
//  proper positions.  The directional arrow is rendered employing 16 different images.
String PlotWindComponents(int16_t scnInx)
{
    // Fetch the data from the sim to calculate the wind components
    float velocity = scnNode[SNSTWVEL].GetFloatValue();
    
    float acHeading = scnNode[scnInx].GetFloatValue();;
    float windDir = scnNode[SNSTWDIR].GetFloatValue();

    // Conversion to radians
    acHeading /= 57.296;
    windDir /= 57.296;

    // Calculate relative wind angle, headwind and crosswind
    float windAngle = windDir - acHeading;
    float windComponent = velocity * cos(windAngle);
    int16_t headWind = windComponent + ((windComponent > 0) ? 0.5 : -0.5);
    windComponent = velocity * sin(windAngle);
    int16_t crossWind = windComponent + ((windComponent > 0) ? 0.5 : -0.5);

    // First, display the headwind compontents
    String sTxtId1 = "Tn.txt";
    String sTxtId2 = "Ts.txt";
    if (headWind >= 0)
    {
        nexIo.SendAction(sTxtId1, String(headWind));
        nexIo.SendAction(sTxtId2, "");
    }
    else
    {
        nexIo.SendAction(sTxtId2, String(-headWind));
        nexIo.SendAction(sTxtId1, "");
    }

    // Next, display the crosswind compontents
    sTxtId1 = "Te.txt";
    sTxtId2 = "Tw.txt";
    if (crossWind >= 0)
    {
        nexIo.SendAction(sTxtId1, String(crossWind));
        nexIo.SendAction(sTxtId2, "");
    }
    else
    {
        nexIo.SendAction(sTxtId2, String(-crossWind));
        nexIo.SendAction(sTxtId1, "");
    }

    windAngle = int(windAngle * 57.296);
    // Offset the angle for plotting.
    // For the Nextion's dail, 0-degree is at 9 o'clock, 180-degree at 3 o'clock
    windAngle += 90.;
    if (windAngle > 360.)     // Make sure it's between 0-360
    {
        windAngle -= 360.;
    }
    else if (windAngle < 0.)
    {
        windAngle += 360.;
    }

    return String(int(windAngle));
}


// Push the image to depict the speedbrakes current position.
// Share the same logic as the flaps scale.
String PlotSpeedbrakesPercent(int16_t scnInx)
{
    if (scnNode[SNSTSBAV].GetIntValue() || IsXPlane())
    {
        return DrawOnSliderScale(scnInx);
    }
    return "";
}


// Speedbrakes for Zibo is a number from 0 to 52 degrees.  Massage that number
//  into 100 thr 0 for the Nextion slider.
String ZB_ShowSpeedbrakes(int16_t scnInx)
{
    float slider = scnNode[scnInx].GetFloatValue() / 52. * 100.;
    slider = 100. - slider;           // Reverse vertically
    return String((int) slider);
}


// Plot the position of the control surface using a Nextion Slider.
String DrawOnSliderScale(int16_t scnInx)
{
    // Fetch the position data from the sim and scale it to the slider
    // Map sim output of 0. thr 1. to 100 thr 0
    float slider = scnNode[scnInx].GetFloatValue() * 100.;
    slider = 100. - slider;           // Reverse vertically
    return String((int) slider);
}


// plot the position of elevator trim onto the slider in Nextion.
String DrawElevTrimOnScale(int16_t scnInx)
{
    // Fetch the position data from the sim and scale it to the slider
    // Map -1. thr 1. to 100 thr 0
    float trim = scnNode[scnInx].GetFloatValue();
    bool bNegative = trim < 0.;
    trim = abs(trim);

    // Use a two-tier scale to provide finer resolution near center
    if (trim < 0.1)   // The first 10% gets 150% boost of units on slider
    {
        trim *= 2.5;
    }
    else
    {
        trim = 0.25 + (trim - 0.1) * (0.75 / 0.9);
    }
    if (bNegative)     // Relocate negative values to upper half (54-100)
    {
        trim = trim * 46. + 54.;
    }
    else
    {
        trim = (1. - trim) * 54.;
    }
    return String((int) trim);
}


// Touch event from GS/IAS readout, toggle current selection and update the label.
void ToggleGsIasText(int16_t scnInx)
{
    g_Stat.bCurGsDisp = !g_Stat.bCurGsDisp;

    // To force the display to update the number
    scnNode[SNSTGSPD].InvalidateValue();
    scnNode[SNSTIASP].InvalidateValue();

    g_bSaveRequired = true;         // EEPROM save required
}


// Show the Ground Speed or IAS based on current selection.
String ShowSpeedText(int16_t scnInx)
{
    float speed;
    if (g_Stat.bCurGsDisp)
    {
        // Fetch the ground speed (in m/s)
        speed = scnNode[scnInx].GetFloatValue();
        speed *= 1.944;        // Convert to knots
    }
    else
    {
        // Fetch the IAS (in knots)
        speed = scnNode[SNSTIASP].GetFloatValue();
    }
    return String(int(speed + 0.5));
}


// Touch event to select between Terain/GPS Elevation, 
//  toggle current selection and update the label.
void ToggleElevationText(int16_t scnInx)
{
    if (IsFS2020())
    {
        g_Stat.bWpElevation = !g_Stat.bWpElevation;
        scnNode[SNSTELEV].InvalidateValue();
    }
}


// Show the terrain elevation.  This value is available from the sim for FS2020.
// For X-Plane it needs to be calculated.  This function also serves the GPS
//  waypoint elevation display.
String MS_ShowSelectElevation(int16_t scnInx)
{
    String sElevation;
    // Fetch either the GPS waypoint elevation or terrain elevation
    if (g_Stat.bWpElevation)
    {
        sElevation = "-";
        if (g_Stat.bSituaNavValid && scnNode[SNSTWPEV].GetIntValue() != 0)
        {
            sElevation = String(scnNode[SNSTWPEV].GetIntValue());
        }
    }
    else       // Terrain elevation in float
    {
        sElevation = String(int(scnNode[scnInx].GetFloatValue()));
    }

    return sElevation;
}


// The Radar Altimeter requires a custom function because it is displayed only while
//  below 2500 feet.  Above that, display the aircraft altitude.
// Called from aircraft altitude node.
String ShowSelectAltitude(int16_t scnInx)
{
    String sLabel = " ";
    uint16_t altColor = CLR_TEXT_AZURE;

    // Fetch the radar altimeter reading data from the sim 
    uint16_t altitude = scnNode[SNSTRALT].GetFloatValue() + 0.5;
    if (altitude < 2500)
    {
        sLabel = "RAD";
        altColor = CLR_TEXT_STANDARD;
    }
    else
    {
        // Fetch the aircraft elevation
        altitude = scnNode[scnInx].GetFloatValue() + 0.5;
        altitude *= IsXPlane() ? 3.28 : 1.;     // Meter to feet
    }

    // Need to calculate and display ground elevation for X-Plane
    if (IsXPlane())
    {
        // Fetch the aircraft altitude data from the sim (meters)
        int32_t elevation = scnNode[SNSTALTI].GetFloatValue() * 3.28;
        // Minus the radar altitude
        elevation -= scnNode[SNSTRALT].GetFloatValue();
        nexIo.SendAction(scnNode[SNSTELEV].GetTagToNex(), String(elevation));
    }

    // Fetch the radar altimeter reading data from the sim 
    String sOutput = String(altitude);
    String sTxtId = scnNode[SNSTRALT].GetTagToNex();
    nexIo.SendAction(sTxtId, sOutput);

    // Display the estimated time to touchdown based on radar altimeter
    CalcAndShowTimeTDown(SNSTVSPD);

    // Select the color for the number based on type of altitude
    sTxtId.replace(".txt", ".pco");
    nexIo.SendAction(sTxtId, String(altColor));

    // Update the label to accompany the number
    return sLabel;
}


// Animate gears are up, down or in transition, with yellow and green colors.
#define GEAR_SCALE_HEIGHT      13
String ShowGearIndicator(int16_t scnInx)
{
    uint16_t level = 1;     // Presumed as retracted

    String sColorTag = scnNode[scnInx].GetTagToNex();
    sColorTag.replace(".hig", ".pco");
    nexIo.SendAction(sColorTag, String(CLR_GRAY_DARK));

    float gear;
    // Non-retractable displays three greens
    if (IsZiboProfile() || IsToLissProfile() ||
        scnNode[SNSTRETG].GetIntValue())      // Retractable?
    {
        gear = scnNode[scnInx].GetFloatValue();
    }
    else
    {
        gear = 1.;     // Fixed gears--all greens
    }

    if (gear >= 0.99)         // Down & locked
    {
        nexIo.SendAction(sColorTag, String(CLR_GREEN));
        level = GEAR_SCALE_HEIGHT;
    }
    else if (gear > 0.01)     // In transition
    {
        nexIo.SendAction(sColorTag, String(CLR_YELLOW));
        level = gear * (GEAR_SCALE_HEIGHT - 3) + 3.5;
    }
    else         // Retracted
    {
        level = 1;
    }

    return String(level);
}


// Show the value on the mini Rudder/Aileron trim scale with saturation.
#define TRIM_SCALE_SIZE      37
String ShowTrimIndicatorMini(int16_t scnInx)
{
    int16_t scalePos = TRIM_SCALE_SIZE / 2 + 2;   // 0-position
    // Fetch the trim value.  Between -1.0 to +1.0
    float trim = scnNode[scnInx].GetFloatValue();

    // Value will be plotted on a scale of 37 dots across, 1 to 37 with
    //  19 at the center.  Mapping the scale with 0.018 per dot.
    // To emphasize non-zero setting, anything 0.007 off-center gets a
    //  2-dot bump away from the center position.  Also saturate the scale

    if (trim > 0.007)
    {
        scalePos += trim / 0.018;
        scalePos += 2;
        scalePos = min(scalePos, (int16_t) TRIM_SCALE_SIZE);
    }
    else if (trim < -0.007)
    {
        scalePos += trim / 0.018;
        scalePos -= 2;
        scalePos = max(scalePos, (int16_t) 1);
    }

    return String(scalePos);
}


// Apply colors to Icing warning text box.
String ShowIcingColor(int16_t scnInx)
{
    // Fetch the Structural Ice percentage from the sim
    float icing = scnNode[scnInx].GetFloatValue();
    uint16_t color = CLR_WARNING_NONE;
    if (icing > 0.6)
    {
        color = CLR_SELECT_STANDARD;
    }
    else
    {
        if (icing > 0.1)
        {
            color = CLR_ICING_LOW;
        }
    }
    
    return String(color);
}


// Highlight Speedbrakes text to show armed state.
String ShowSpeedbrakesArmedLabel(int16_t scnInx)
{
    uint16_t color = CLR_BGND_STANDARD;    // Presume speedbrake is unavailable, hide text
    bool bSbArmed;
    if (IsFS2020())       // FS2020
    {
        if (scnNode[SNSTSBAV].GetIntValue())     // Spedbrake available?
        {
            // Fetch the spoilers armed state from the sim
            bSbArmed = scnNode[scnInx].GetFloatValue() == 1.;
            // Indicate current state via text color (red=armed, blue=not armed)
            color = bSbArmed ? CLR_SELECT_STANDARD : CLR_TEXT_LABEL;
        }
        return String(color);
    }

    // X-Plane
    if (IsZiboProfile())
    {
        bSbArmed = scnNode[scnInx].GetFloatValue() == -0.5;
    }
    else
    {
        // For XP, negative value indicates in armed mode
        bSbArmed = scnNode[scnInx].GetFloatValue() < -0.1;
    }

    // Indicate current state via text color (red=armed, blue=not armed)
    color = bSbArmed ? CLR_SELECT_STANDARD : CLR_TEXT_LABEL;
    return String(color);
}
    
    
// Highlight Reverser text box to show engaged state.
String ShowReverserText(int16_t scnInx)
{
    // Fetch the reverser state from the sim
    float reverThres = IsXPlane() ? 0.01 : 1.;    // 1% threshold
    bool bReverser = scnNode[scnInx].GetFloatValue() > reverThres;
    // Indicate state via choice of text color
    uint16_t color = bReverser ? CLR_SELECT_STANDARD : CLR_WARNING_NONE;

    return String(color);
}


// Highlight Parking Brakes text box to show engaged state.
String ShowParkingBrakesText(int16_t scnInx)
{
    // Fetch the parking brakes state from the sim
    int16_t parkingB = scnNode[scnInx].GetIntValue();
    // Indicate state via choice of text color
    uint16_t color = (parkingB == 1) ? CLR_SELECT_STANDARD : CLR_WARNING_NONE;

    return String(color);
}


// Select the next nav unit to show on the display.
void CycleNavSelection(int16_t scnInx)
{
    // Flip the NAV linked to the currently Situation screen
    g_Stat.curNavDisplay++;       // 0=GPS, 1,2=DME1/2
    g_Stat.curNavDisplay %= 3;

    // Force the node display to refresh to avoid posting bogus values
    scnNode[SNSTDME1].InvalidateValue();
    scnNode[SNSTDME2].InvalidateValue();
    scnNode[SNSTDIST].InvalidateValue();    // GPS

    // Blank out the bearing indicator
    BlankDirectionArrow();
}


// Show the DME distance from the previously selected NAV.
String ShowDmeDistanceText(int16_t scnInx)
{
    const int16_t inxLookup[3] = { SNSTDIST, SNSTDME1, SNSTDME2 };

    char sRetBuf[8] = "-";

    // Fetch the DME distance from the sim
    float distance = scnNode[inxLookup[g_Stat.curNavDisplay]].GetFloatValue();

    // Is the number legit?
    g_Stat.bSituaNavValid = distance > 0.05;
    if (g_Stat.bSituaNavValid)
    {
        if (distance >= 100. || IsZiboProfile())
        {                     // 0.2 is a kludge for LNAV
            sprintf(sRetBuf, "%#5.0f", (distance + 0.2));
        }
        else
        {
            sprintf(sRetBuf, "%5.1f", distance);
        }
    }
    // Update the ETE display
    CalcAndDisplayEte(distance);

    return sRetBuf;
}


// From the given distance, calculate the ETE based on current ground speed.
// Called only when there's a valid distance.
void CalcAndDisplayEte(float distance)
{
    char sEteBuf[8] = "-";

    float groundSpeed = scnNode[SNSTGSPD].GetFloatValue();
    if (groundSpeed > 1.)
    {
        float ete = distance / groundSpeed / 1.944;  // From m/s
        if (g_Stat.bSituaNavValid && ete > 0.001 && ete < 3.33)
        {
            int16_t minutes = ete * 60;
            sprintf(sEteBuf, "%d:%02d", minutes, int(ete * 3600 - (minutes * 60) + 0.5));
        }
    }
    nexIo.SendAction("Tet.txt", sEteBuf);
}


// Toggle display between Vertical Speed and Estimated Touchdown Time.
void ToggleVsTdownText(int16_t scnInx)
{
    g_Stat.bTouchdownDisp = !g_Stat.bTouchdownDisp;

    g_bSaveRequired = true;         // EEPROM save required
}


// Calculate and display estimated time to ground level.
// Originated from aircraft altitude node.
void CalcAndShowTimeTDown(int16_t scnInx)
{
    char sVinfo[8] = "^";

    // Fetch the altitude of the waypoint
    float vertSpeed = scnNode[scnInx].GetFloatValue();
    if (g_Stat.bTouchdownDisp)     // Show touchdown estimate
    {
        if (vertSpeed < -1.)          // Valid only if decending
        {
            float delta = scnNode[SNSTRALT].GetFloatValue();
            delta = delta / -vertSpeed;
            uint16_t minutes = delta;
            if (minutes < 60)
            {
                sprintf(sVinfo, "%2d:%02d", minutes, int((delta - minutes) * 60));
            }
        }
    }
    else       // Show vertical speed 
    {
        if (abs(vertSpeed) < 10000.)
        {
            sprintf(sVinfo, "%+d", int(vertSpeed));
        }
        else
        {
            sprintf(sVinfo, "%+4.1f", vertSpeed / 1000.);
        }
    }
    
    nexIo.SendAction(scnNode[scnInx].GetTagToNex(), sVinfo);
}


// Toggle between To/From for the showing of NAV radial.
void ToggleToFromRadial(int16_t scnInx)
{
    g_Stat.bToRadialDisp = !g_Stat.bToRadialDisp;
}


// Toggle between navigation bearing and compass for showing the direction arrow.
void ToggleNavCompass(int16_t scnInx)
{
    g_Stat.bNavCompDisp = !g_Stat.bNavCompDisp;
    // Blank out the bearing indicator
    BlankDirectionArrow();
}


// Show the To/From Radial for the previously selected NAV.
// Originated from the GPS bearing and the NAV radial nodes.
String ShowToRadialText(int16_t scnInx)
{
    const int16_t inxLookup[3] = { SNSTBEAR, SNSTRAD1, SNSTRAD2 };
    // Flag to convert to the appropriate To/From radial reading
    // For FS2020, NAV radials come in as From, for XP it's To
    bool bConvert = g_Stat.bToRadialDisp;
    if (IsXPlane())        // XP dataref is To, invert the logic
    {
        bConvert = !bConvert;
    }
    else if (g_Stat.curNavDisplay == 0)   // FS2020's GPS comes in as To
    {
        bConvert = !bConvert;     // Invert it
    }

    uint16_t dispRadial;
    char sRetBuf[6] = "-";

    if (g_Stat.bSituaNavValid)
    {
        // Fetch the From radial from the sim
        float radial = scnNode[inxLookup[g_Stat.curNavDisplay]].GetFloatValue();

        if (radial < 0.)
        {
            radial += 360;
        }
        dispRadial = radial + 0.5;

        dispRadial = (dispRadial + (bConvert ? 180 : 0)) % 360;
        sprintf(sRetBuf, "%03d", dispRadial);

        // Show Relative bearing directional arrow
        if (g_Stat.bNavCompDisp)
        {
            scnNode[SNSPRBER].SetValue((float)dispRadial);
            PlotBearingDir(SNSPRBER);
        }
    }
    return  sRetBuf;
}


// Convert (if applicable) and display the Kohlsman setting.
String ShowBarometerText(int16_t scnInx)
{
    // Fetch the current reading from the sim
    float pressure = scnNode[scnInx].GetFloatValue();
    char sRetBuf[8] = "";
    if (g_Config.dispUnits == 0)       // Convert according to config
    {
        sprintf(sRetBuf, "%5.2f", pressure);
    }
    else        // To millibars
    {
        sprintf(sRetBuf, "%d", int( pressure * 33.864 + 0.5));
    }
    
    return sRetBuf;
}


#define MS_ENG_TYPE_JET       1
#define MS_ENG_TYPE_TURBO     5

// Plot the gauge for the engine percentage.  This one function serves both engines.
String MS_ShowEngineGauge(int16_t scnInx)
{
    if (scnNode[SNSTTYPE].GetIntValue() == MS_ENG_TYPE_JET ||   // Jet engines?
        scnNode[SNSTTYPE].GetIntValue() == MS_ENG_TYPE_TURBO)
    {
        // Point to the current N1 percentage from the sim
        scnInx += 2;
    }

    // Fetch the current RPM percentage from the sim
    return String(ShowGaugeCommon(scnNode[scnInx].GetFloatValue()));
}


// Plot the gauge for the engine percentage.  This one function serves both engines.
String XP_ShowEngineGauge(int16_t scnInx)
{
    // Fetch the current RPM percentage from the sim
    float percent;

    if (IsZiboProfile())
    {
        percent = scnNode[scnInx].GetFloatValue();
    }
    else
    {
        // Use function to lookup aircraft-specific result
        percent = scnNode[scnInx + 2].GetFloatValue();
    }

    return String(ShowGaugeCommon(percent));
}


// Calculate the position (angle) of the needle for displaying the given percentage.
String ShowGaugeCommon(float percent)
{
    percent = min(percent, (float) 105.);

    // Map the given percentage (0.-100.) onto 300-212 degrees span on the gauge
    //  (0 degree being at 9 o'clock position).
    // Thus, Gauge Min @ 300, Max @ 212; Range of movement = (360-300+1) + 212 = 273 degrees
    //  I.e., map 0-10000 onto 0-271
    int16_t needlePos = percent / (100. / 272.);   // Scale from 0 to 272

    needlePos -= 360 - 300;        // Slide the 0 value position to the gauge's lowest end
    if (needlePos < 0)
    {
        needlePos += 360;
    }

    return String(needlePos);
}
