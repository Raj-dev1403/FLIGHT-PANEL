// Implementation to handle packet I/O operations specific to FS2020TA:
//  Input of data packet from FS2020.
//  Output of event from MCU to FS2020.
//
// Specifically for use with the FS2020-To-Arduino utility.
//  See https://github.com/Seahawk240/Microsoft-FS2020-To-Arduino
//
// Discman777    08/24/2021
// Version 1.08.25
// This freeware is provided “AS IS”.  Non-commercial use, modification, distribution at your own risk.


#include <CMsFsIo.h>
#include <GlobalVars.h>



// Accept input stream from FS2020-To-Arduino.  It detects the start (@) and end ($)
//  of packet, then parse the packet for the individual components.
// Return true if an input packet has been received and qualified.
//                    m_sBuf = "@76/-1=4123$"

bool CMsFsIo::FetchPacket(void)
{
     // Keep checking, grab a byte as long as one is available, and buffer it.
    // Return at once if no byte available--this function call is non-blocking.
    while (Serial.available() > 0)
    {
         char inByte = Serial.read();

        if (inByte == '@')    // Scan for the Start char
        {
            m_CharCnt = 0;    // Prepare to receive
            m_sBuf = "";
        }

        if (m_CharCnt >= 0)   // Currently accepting input packet?
        {
            if (m_CharCnt < MAX_INSTREAM)
            {
                m_sBuf += String(inByte);
                if (inByte == '$')
                {
                    return true;      // Indicate successful packet
                }
                else
                {
                    m_CharCnt++;       // Packet not yet terminated, continue input
                }
            }
            else      // Exceeded max stream limit
            {
                m_CharCnt = -1;    // Abort and reset state machine
            }
        }
    }

    return false;     // No packet ready
}


// Single out and return the ID substring in the packet.
String CMsFsIo::GetPacketId(void)
{
    uint16_t inxEnd = m_sBuf.indexOf('/', 2);
    if (inxEnd >= 2)
    {
        return m_sBuf.substring(1, inxEnd);
    }
    return "";
}


// Single out and return the Index substring in the packet.
String CMsFsIo::GetPacketInx(void)
{
    uint16_t inxSta = m_sBuf.indexOf('/', 2) + 1;
    uint16_t inxEnd = m_sBuf.indexOf('=', 3);
    if (inxSta < inxEnd)
    {
        return m_sBuf.substring(inxSta, inxEnd);
    }
    return "";
}    


// Single out and return the Value substring in the packet.
String CMsFsIo::GetPacketVal(void)
{
    uint16_t inxSta = m_sBuf.indexOf('=', 3) + 1;
    uint16_t inxEnd = m_sBuf.indexOf('$', 4);
    if (inxSta < inxEnd)
    {
        return m_sBuf.substring(inxSta, inxEnd);
    }
    return "";
}


// Send the given ID, without value, out to FS2020-To-Arduino.
void CMsFsIo::SendPacket(uint16_t pacId)
{
    SendPacket("@" + String(pacId) + "/$");
}


// Send the given ID and numberic Value pair out to FS2020-To-Arduino.
void CMsFsIo::SendPacket(uint16_t pacId, int32_t pacVal)
{
    SendPacket("@" + String(pacId) + "/" + String(pacVal) + "$");
}



// This is the sim variable Collector.  It parses input stream from FS2020TA
//  and populates the designated screen node with the value.  
// Return the number of sim variables processed.
uint16_t CMsFsIo::CollectVariables(uint16_t quota)
{
    uint16_t totalRead = 0;

    while (FetchPacket())
    {
        bool bValidVar = false;       // Flag for variable is found and saved
        uint16_t fromSimId = GetNumericId();    // Recieved an ID from sim

        // Look for an object to match for this received variable ID
        for (uint16_t pge = 0; (!page[pge].IsModal() && !bValidVar); pge++)
        {
            int16_t nodeIx = 0;

            while (nodeIx < page[pge].GetNumNodes())
            {
                int16_t scnInx = page[pge].GetInxToNode(nodeIx);

                String sValue = "";
                // Check for match with From Sim ID, plus its index if exists
                if (scnNode[scnInx].IsInput() &&
                    scnNode[scnInx].GetIdFromSim() == fromSimId &&
                    scnNode[scnInx].GetInxFromSim() == GetNumericIndex())
                {
                    sValue = GetPacketVal();

                    if (scnNode[scnInx].IsValueInt())
                    {
                        scnNode[scnInx].SetValue((int32_t) GetNumericVal());
                    }
                    else if (scnNode[scnInx].IsValueFloat())
                    {
                        scnNode[scnInx].SetValue(sValue.toFloat());
                    }

                    totalRead++;
                    bValidVar = true;     // Singal this var's been saved, next node
                }

                nodeIx++;       // Next screen item
            }
        }

        if (--quota == 0)       // Within variables per call quota?
        {
            break;
        }
    }

    return totalRead;
}