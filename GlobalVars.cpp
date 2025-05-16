// Global variable definitions.
//
// Discman777    08/24/2021
// Version 1.08.25
// This freeware is provided “AS IS”.  Non-commercial use, modification, distribution at your own risk.


#include <GlobalVars.h>
#include <CEncoderG3.h>
#include <CNextionIo.h>
#include <XPLDirect.h>



// Instantiate the encoder objects

//                                   1:DT CK  SW
CEncoderG3 encoders[NUM_ENCODERS] = { {26, 25, 27},
//                                   2:DT CK  SW  3:DT  CK  SW   4:DT  CK  SW
                                      {18, 19, 5}, {32, 35, 33}, {39, 36, 34} };

// Object for a One-second timer.
hw_timer_t* timerSec = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;


// The global object for interfacing with the Nextion touchscreen.
CNextionIo nexIo;


// The global object for interfacing with the FS2020TA software (the sim).
CMsFsIo fstaIo;


// X-Plane Direct object.
XPLDirect XPDplugin(&Serial);
CXpDrIo xpdrIo;


// Screen node pool.  Generally, there is one of these nodes assigned for each
//  object on the Nextion display.  It encapsulates the info needed to interface
//  with the sim.
CScreenNode scnNode[MAX_NODES];


// Holding information pertained to screen pages.
CScreenPage page[MAX_PAGES];


// Global page management variables.
int16_t g_CurPage;              // Currently active page


// Display Task variables.
int16_t g_NodeDisp = 0;           // Tracking node index to be display
uint32_t g_NextDispRunTime = 0;   // Timestamp used for scheduling next display
uint32_t g_NextRevivalTime = 0;   // Timestamp used for scheduling next revival

// Holding the 0-based Sidebar index of the rubber banded page.
int16_t g_RubberbandIndex = -1;          // Set to no rubber band set
// Remember whether the primary items are currently in encoder's focus.
bool g_bAtPrimaryEnco = true;


// Available Profiles
tProfile g_Profile[MAX_PROFS];


// Store all non-volatile config parameters.
tConfig g_Config;


// Reset the level of re-initialization required after a Config change.
uint16_t g_bProfileChanged = false;
uint16_t g_bSaveRequired   = false;


// Global variables holding current status/values sampled from the sim.
// Used for servicing various screen values and button states.
tSimState g_Stat;



// Global (external) function declarations

// Test for MSFS or X-Plane.
bool IsXPlane(void)
{
    return g_Profile[g_Config.curProf].client == SMC_XPLN;
}


bool IsFS2020(void)
{
    return g_Profile[g_Config.curProf].client == SMC_MSFS;
}
