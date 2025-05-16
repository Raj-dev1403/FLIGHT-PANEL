// Implementation of the Screen Node methods.
//
// Discman777    08/24/2021
// Version 1.08.25
// This freeware is provided “AS IS”.  Non-commercial use, modification, distribution at your own risk.


// Screen Node is a data structure to represent elements on a screen page.
// An item on the Nextion Page is called a screen object.  For example, a Text Box.
// When one is touched, a message is sent from Nextion to the MCU with an operation
//  ID attached to identify the origin object.
// The Screen Node is used to describe the actions needed to take place when the item is touched,
//  or when data it associated with has been received from the sim.
// There is usually a one-to-one relationship between a screen object and a node.



#include <arduino.h>
#include <GlobalVars.h>
#include <CScreenNode.h>


// Attach a X-Plane dataref to the node's value storage.
// Basic integer version.
void CScreenNode::AttachSimInt(char* sVar, int32_t mode, uint16_t update)
{
    XPDplugin.registerDataRef(sVar, mode, update, 0, (long int*) &m_Value.iVal);
}


// Integer Array version.
void CScreenNode::AttachSimInt(char* sVar, int32_t mode, uint16_t update, uint16_t index)
{
    XPDplugin.registerDataRef(sVar, mode, update, 0, (long int*) &m_Value.iVal, index);
}


// Simple Float, specify any mode and update freq.
void CScreenNode::AttachSimFloat(char* sVar, int32_t mode, uint16_t update, float divider)
{
    XPDplugin.registerDataRef(sVar, mode, update, divider, (float*) &m_Value.fVal);
    SetVarType(VT_FLOAT);
}


// Float Array, specify any mode and update freq.
void CScreenNode::AttachSimFloat(char* sVar, int32_t mode, uint16_t update, float divider, uint16_t index)
{
    XPDplugin.registerDataRef(sVar, mode, update, divider, (float*) &m_Value.fVal, index);
    SetVarType(VT_FLOAT);
}


// Simple String, read-only, specify update freq.
void CScreenNode::AttachSimString(char* sVar, uint16_t update)
{
    XPDplugin.registerDataRef(sVar, XPL_READ, update, (char*) &m_cValue);
    SetVarType(VT_CSTR);
}


// Embed a X-Plane Command to the node's value storage.  Position in the node specified by index,
//  one of three handles belonging to this node.
// Return the handle for the registered command.
int16_t CScreenNode::AttachCommand(char* sCmd, uint16_t inx)
{
    return m_XpHandle[inx] = XPDplugin.registerCommand(sCmd);
}

// The following version stuffs a handle directly instead of a named XP command (String).
// This is used by keypad array processor after it has extracted the actual comman handle.
void CScreenNode::AttachCommand(int16_t handle, uint16_t inx)
{
    m_XpHandle[inx] = handle;
}


// Mark the previous value with a bogus number to initiate a refresh by the Disp Task.
void CScreenNode::InvalidateValue(void)
{
    if (IsValueInt())
    {
        m_PrevVal.iVal = micros();
    }
    else
    {
        m_PrevVal.fVal = micros();
    }
}


// Accessors for test and updating if the node's value has changed since the
//  last call.  This is used for preventing unnecessay update of the display
//   (to reduce dataflow to Nextion).
// Return True if a new (changed) value is present in this node.
bool CScreenNode::DeltaCheck(void)
{
    if (IsValueInt())
    {
        if (GetIntValue() != m_PrevVal.iVal)
        {
            m_PrevVal.iVal = GetIntValue();
            return true;
        }
    }
    else if (IsValueFloat())
    {
        // Check for delta bigger than a set magnitude
        if (fabs(GetFloatValue() - m_PrevVal.fVal) > 0.0001)
        {
            m_PrevVal.fVal = GetFloatValue();
            return true;
        }
    }
    else if (IsValueCstr())
    {
        if (GetChrValue() != m_PrevValChr)
        {
            strcpy(m_PrevValChr, GetChrValue().c_str());
            return true;
        }
    }

    return false;
}


// Has an available custom display function?
bool CScreenNode::UseDispFunction(void)
{
    return IsXPlane() ? m_DispFuncXP != NULL : m_DispFuncMS != NULL;
}


// Has an available custom touch function?
bool CScreenNode::UseTouchFunction(void)
{
    return IsXPlane() ? m_TouchFuncXP != NULL : m_TouchFuncMS != NULL;
}


// Launch the custom responder to Display event based on client.
String CScreenNode::ExecuteDispFunc(uint16_t inx)
{
    return IsXPlane() ? (*m_DispFuncXP)(inx) : (*m_DispFuncMS)(inx);
}


// Launch the custom responder to Touch event based on client.
void CScreenNode::ExecuteTouchFunc(uint16_t inx)
{
    return IsXPlane() ? (*m_TouchFuncXP)(inx) : (*m_TouchFuncMS)(inx);
}
