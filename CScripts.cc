#include <iostream>
#include <string>
#include <RAT/DU/DSReader.hh>
#include <RAT/DS/Entry.hh>

/// From Tereza
std::vector<UShort_t> GetCAENTrace(const RAT::DS::EV &triggeredEvent) {
    /*
     * Given a triggered SMELLIE event, obtain the MPU's CAEN trace.
     * If this is not possible, raise the RAT::DS::DataNotFound exception.
     */
    RAT::DS::Digitiser digitiser;
    try { digitiser = triggeredEvent.GetDigitiser(); }
    catch (RAT::DS::DataNotFound& e) { throw; }

    const int PMU_CAEN_channel = 3;
    return digitiser.GetWaveform(PMU_CAEN_channel);
}

/// Print first GTID of each subrun for a given runID
///
/// @param[in] runID to iterate through
void printGTIDs(UInt_t runID)
{
    for (int isubRunID = 0; isubRunID < 13; isubRunID++)
    {
        std::string subRunStr = std::to_string(isubRunID);
        if (isubRunID < 10) {subRunStr = "0" + subRunStr;}
        std::string iFileName = "/nfs/disk4/ratds_230515_230715_bronze/Analysis20_r0000" + std::to_string(runID) + "_s0" + subRunStr + "_p000.root";
        RAT::DU::DSReader dsReader(iFileName);
        UInt_t iGTID = dsReader.GetEntry(0).GetEV(0).GetGTID();
        std::cout << std::to_string(iGTID) << std::endl;
    }
}

// In window BiPos events for testing
// runID = 309622, GTID 11220168
// subRunID = 4, entry = 284290, EV = 0
// fileName = "/nfs/disk4/ratds_230515_230715_bronze/Analysis20_r0000309622_s004_p000.root"

// runID = 310365, GTID = 1834806
// subRunID = 6, entry = 130684, EV = 0
// fileName = "/nfs/disk4/ratds_230515_230715_bronze/Analysis20_r0000310365_s006_p000.root"

// runID = 309761, GTID = 16344385
// subRunID = 1, entry = 186566, EV = 0
// fileName = "/nfs/disk4/ratds_230515_230715_bronze/Analysis20_r0000309761_s001_p000.root"

// runID = 310329, GTID = 15765140
// subRunID = 4, entry = 77899, EV = 0
// fileName = "/nfs/disk4/ratds_230515_230715_bronze/Analysis20_r0000310329_s004_p000.root"

// runID = 310336, GTID = 82569
// subRunID = , entry = 29637, EV = 0
// fileName = "/nfs/disk4/ratds_230515_230715_bronze/Analysis20_r0000310336_s012_p000.root"

// runID = 309623, GTID = 1292310
// subRunID = 5, entry = 148530, EV = 0
// fileName = "/nfs/disk4/ratds_230515_230715_bronze/Analysis20_r0000309623_s005_p000.root"

// runID = 309624, GTID = 6235502
// subRunID = 2, entry = 423288, EV = 0
// fileName = "/nfs/disk4/ratds_230515_230715_bronze/Analysis20_r0000309624_s002_p000.root"

// runID = 309624, GTID = 6944773
// subRunID = 4, entry = 9080, EV = 0
// fileName = "/nfs/disk4/ratds_230515_230715_bronze/Analysis20_r0000309624_s004_p000.root"

// runID = 309624, GTID = 10035161
// subRunID = 9, entry = 287149, EV = 0
// fileName = "/nfs/disk4/ratds_230515_230715_bronze/Analysis20_r0000309624_s009_p000.root"

// runID = 309625, GTID = 13869355
// subRunID = 5, entry = 19546, EV = 0
// fileName = "/nfs/disk4/ratds_230515_230715_bronze/Analysis20_r0000309625_s005_p000.root"

// runID = 309743, GTID = 8385365 
// subRunID = , entry = 415724, EV = 0
// fileName = "/nfs/disk4/ratds_230515_230715_bronze/Analysis20_r0000309743_s008_p000.root"

// runID = 309745, GTID = 2505814 
// subRunID = 5, entry = 139206, EV = 0
// fileName = "/nfs/disk4/ratds_230515_230715_bronze/Analysis20_r0000309745_s005_p000.root"

// runID = 309753, GTID = 1961228 
// subRunID = 2, entry = 209101, EV = 0
// fileName = "/nfs/disk4/ratds_230515_230715_bronze/Analysis20_r0000309753_s002_p000.root"

// Out window BiPos events for testing
// runID = 309665, subRunID = 9, entry = 62028 and 62029, EV = 0, GTID = 10626139 and 10626140
// fileName ="/nfs/disk4/ratds_230515_230715_bronze/Analysis20_r0000309665_s009_p000.root"

// Simulated Bi214
// runID = 0, entry = 0, ev = 0, GTID = 0
// fileName = "/nfs/disk3/kroupa/caen_traces/Simulation_r22.root"
