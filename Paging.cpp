// Misc. functions for supporting paging and the Sidebar menu operations.
//
// Discman777    08/24/2021
// Version 1.08.25
// This freeware is provided “AS IS”.  Non-commercial use, modification, distribution at your own risk.


#include <arduino.h>
#include <GlobalVars.h>
#include <Supports.h>
#include <DefineNODEs.h>
#include <DefinePAGEs.h>>
#include <Paging.h>
#include <Page_RADIOS.h>
#include <Pages_MORE.h>
#include <Page_CONFIG.h>


// Change to a new screen page by page enum.  Handle any specific pre and post page
//  change requirements.
void OpenScreenPage(int16_t targetPage)
{
    ScreenSaver(SCS_CONT);         // Exit dimming mode if necessary

    // Do not allow page change if a profile selection is pending
    if (g_bProfileChanged || targetPage < 0)
    {
        return;
    }

    // When exiting Config or Situation page setting  triggers a save to the EEPROM
    if (g_bSaveRequired)
    {
        ConfigCommitSettings();
    }

    if (!page[targetPage].IsModal())    // Regular user page or Special non-data page?
    {
        noInterrupts();                 // Block encoders until page change complete

        g_CurPage = targetPage;
        page[g_CurPage].Invalidate();   // Force all page's value--force refresh
        g_NodeDisp = 0;                 // Reset display index at beginning
        g_bAtPrimaryEnco = true;        // Indicate no need to do encoder snapback

        // Direct the display to show the new page
        nexIo.SendAction("page " + page[g_CurPage].GetPhyPageName());
        // Intentionally BLOCK all activities to allow the page change to settle
        delay(25);

        // See if necessary to provide any page-specific setup for this target page
        page[g_CurPage].ExecuteEntryFunc();
        page[g_CurPage].ExecuteRotarySelectFunc();

        PopulateSidebar(g_CurPage);    // Go label the six Sidebar buttons
        
        interrupts();                   // Page change completed, interrupt okay
    }
    else            // Special modal page
    {
        switch (targetPage)    // Switch to the "modal" screen
        {
            case PG_TESTPAD:
                DoTestPadPage();
                break;
            case PG_DATAMON:
                DoDataMonitorPage();
                break;
        }
    }
}


// Sidebar paging handler.  Use Paging Op ID from Nextion for target page.
void HandleSidebarPage(void)
{
    uint16_t opId = nexIo.GetByte(INX_OP_ID);
    // Validate the Sidebar Nextion ID code
    if (opId >= SIDEBAR_NEX_0 && opId < (SIDEBAR_NEX_0 + SIDEBAR_ITEMS))
    {
        // First sort out rubber banding selection
        uint16_t requestInx = (opId & 0x7) - 1;   // Touched index

        // Use the Op ID from Nextion to index the screen node (0-5)
        // Touched on the current selected page?
        if (g_CurPage == scnNode[requestInx].GetMsToSimId(1))
        {
            // Touched on the current rubber banded page?
            if (requestInx == g_RubberbandIndex)
            {
                g_RubberbandIndex = -1;    // Remove the rubber band
            }
            else       // Not touched on the current rubberbanded
            {
                g_RubberbandIndex = requestInx;    // Set new rubber band
            }
            PopulateSidebar(g_CurPage);    // Refresh the labels
        }
        else     // It's a page change request
        {
            OpenScreenPage(scnNode[requestInx].GetMsToSimId(1));
        }
    }
}


#define CLR_SIDEMN_BGND      RGB16(10, 20, 10)      // 21130
#define CLR_SIDEMN_SELBGND   RGB16(16, 33, 19)      // 33843
#define CLR_SIDEMN_NORMAL    RGB16(30, 60, 27)      // 63387
#define CLR_SIDEMN_RUBBAND   RGB16(20, 63, 26)      // 43002

// Populate with actual title and the link for each page on the Sidebar buttons.
// Pass the page number to be highlighted as the selected page.
void PopulateSidebar(int16_t curPage)
{
    for (uint16_t sbInx = SNZBSB0;
           (sbInx < SNZBSB0 + SIDEBAR_ITEMS) && !page[g_CurPage].IsModal(); sbInx++)
    {
        // First, change the target page entry on the Sidebar to reflect the profile
        scnNode[sbInx].InitOperands(g_Profile[g_Config.curProf].scrn[sbInx]);

        uint16_t scanPage = scnNode[sbInx].GetMsToSimId(1);   // Index to page number
        String sTitle = page[scanPage].GetTitle();
        nexIo.SendAction("SB" + String(sbInx + 1) + ".txt", sTitle);

        // Together with the proper background color to highlight the current page
        //  and a diffeent text color for the rubber banded page title
        uint16_t colorTxt = CLR_SIDEMN_NORMAL;
        uint16_t colorBgnd = CLR_SIDEMN_BGND;
        if (sbInx == g_RubberbandIndex)
        {
            colorTxt = CLR_SIDEMN_RUBBAND;
        }
        if (scanPage == curPage)
        {
            colorBgnd = CLR_SIDEMN_SELBGND;
        }

        nexIo.SendAction("SB" + String(sbInx + 1) + ".pco", String(colorTxt));
        nexIo.SendAction("SB" + String(sbInx + 1) + ".bco", String(colorBgnd));
    }
}
