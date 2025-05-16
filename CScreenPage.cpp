// Implementation of the Screen Page methods.
//
// Discman777    08/24/2021
// Version 1.08.25
// This freeware is provided “AS IS”.  Non-commercial use, modification, distribution at your own risk.


// Screen Page is a data structure to collect screen nodes on a display page.
// A Screen Page will associate with one Nextion page.  However, one Nextion page can be
//  linked with multiple Screen Pages.  You may think of Screen Pages as logical pages, 
//  while a physical page is a page defined in Nextion's domain.
// A group of Screen Pages together is called a "Profile".  For example, a profile can be
//  defined for FS2020 aircraft.



#include <arduino.h>
#include <GlobalVars.h>
#include <CScreenPage.h>


// Constructor
CScreenPage::CScreenPage()
{
    // Set up defaults
    m_ColorNormal = CLR_TEXT_STANDARD;
    m_Type = PTY_USER;
    m_NumNodes = 0;
    m_EntryFunc = NULL;
    m_EncoSelectFunc = NULL;

    // Clear all selections, since no node has been selected at the start
    for (uint16_t enc = 0; enc < NUM_ENCODERS; enc++)
    {
        SetEncoAttach(enc, INVLD_SCRNOBJ);
        SetPushAttach(enc, INVLD_SCRNOBJ);
        SetLongAttach(enc, INVLD_SCRNOBJ);
    }
};


// Add a series of screen nodes from N through M into the domain of this page.
// Caution: Make sure N and M cover the ENTIRE range of nodes intneded for the page!
// Return total number added.
uint16_t CScreenPage::AddNodes(uint16_t nodeN, uint16_t nodeM)
{
    uint16_t count = 0;
    while (nodeN + count <= nodeM)
    {
        AddNode(nodeN + count++);
    }
    return count;
}


// Add a single screen node into the domain of this page.
// Return true if added.
bool CScreenPage::AddNode(int16_t ndInx)
{
    if (m_NumNodes < MAX_NODES_PAGE)
    {
        m_Node[m_NumNodes++] = ndInx;
        return true;
    }
    g_Stat.sErrorMsg = ERR_PAGEOVERFLOW;   // Need to increase MAX_NODES_PAGE
    return false;
}


// Copy each attribute from the specified page to this page.  Used for building
//  a page using an existing page as its template.
void CScreenPage::CloneFrom(uint16_t fromPage)
{
    m_sTitle = page[fromPage].m_sTitle;
    m_sNexPageName = page[fromPage].m_sNexPageName;
    m_Type = page[fromPage].m_Type;

    m_EntryFunc = page[fromPage].m_EntryFunc;
    m_EncoSelectFunc = page[fromPage].m_EncoSelectFunc;

    m_ColorNormal = page[fromPage].m_ColorNormal;

    m_NumNodes = page[fromPage].m_NumNodes;
    for (uint16_t inx = 0; inx < m_NumNodes; inx++)
    {
        m_Node[inx] = page[fromPage].m_Node[inx];
    }

    for (uint16_t inx = 0; inx < NUM_ENCODERS; inx++)
    {
        m_EncAttach[inx]  = page[fromPage].m_EncAttach[inx];
        m_PushAttach[inx] = page[fromPage].m_PushAttach[inx];
    }
}


// Launch the function (if available) to initialize the page's entry requirement.
void CScreenPage::ExecuteEntryFunc(void)
{
    if (m_EntryFunc != NULL)
    {
        (*m_EntryFunc)();
    }
}


// Launch the function (if available) to make the primary rotary assignment for the page.
void CScreenPage::ExecuteRotarySelectFunc(void)
{
    if (m_EncoSelectFunc != NULL)
    {
        (*m_EncoSelectFunc)(0);
    }
}


// MarK all nodes on the page to force a refresh by the Display Task at the
//  next opportunity.
void CScreenPage::Invalidate(void)
{
    uint16_t nodeIx = 0;

    while (nodeIx < GetNumNodes())     // Work through all nodes on this page
    {
        int16_t scnInx = GetInxToNode(nodeIx);
        scnNode[scnInx].InvalidateValue();
        nodeIx++;
    }
}
