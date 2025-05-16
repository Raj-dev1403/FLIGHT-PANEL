// Functions to implement the input/output abstraction for the sims.
//
// Discman777    08/24/2021
// Version 1.08.25
// This freeware is provided “AS IS”.  Non-commercial use, modification, distribution at your own risk.


#include <arduino.h>
#include <GlobalVars.h>
#include <SimExchange.h>
#include <Supports.h>


// Send the operation as embedded in the screen node to the sim.  The code to send is
//  specified by an index (0,1,2) to point to the actual opcode holding in the node.
void NodeToSimCommon(int16_t scnInx, uint16_t opInx, int32_t operand)
{
    if (scnInx == INVLD_NODEINX)
    {
        return;
    }

    uint16_t opcode;

    // Which client?  System operations always c/o XP's
    if (IsXPlane() || scnNode[scnInx].IsSystemOp())
    {
        opcode = scnNode[scnInx].GetXpCmndHandle(opInx);
        xpdrIo.SendCmdAndData(opcode, operand, scnInx);
    }
    else         // FS2020
    {
        opcode = scnNode[scnInx].GetMsToSimId(opInx);
        MS_DirectToSim(scnInx, opcode, operand);
    }
}


// Send the directly-specified operation code and operand to FS2020.
// A legacy function necessary for certain FS2020 operations.
void MS_DirectToSim(int16_t scnInx, uint16_t opcode, int32_t operand)
{
    if (opcode > 0)
    {
        if (operand == 0x7FFFF)     // No operand
        {
            fstaIo.SendPacket(opcode);
        }
        else
        {
            fstaIo.SendPacket(opcode, operand);
        }
	}
}


// The collector function to read in a series of sim variables and deposit them
//  into the corresponding screen nodes.  This method of inputing sim variable is
//  what XP-Direct naturally do, as such, its collector is a simple call to its
//  interface function "xloop" to directly deposit into the nodes.
//  On the FS2020 front, it needed additional logics to do fetch the variable one
//  at a time from FS2020TA.
void CollectorCommon(void)
{
    if (IsXPlane())         // Which sim client?
    {
        xpdrIo.CollectVariables();
        RefreshConnectStat();
    }
    else
    {
        fstaIo.CollectVariables(90);
    }
}
