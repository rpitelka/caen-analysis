#include <iostream>
#include <string>

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
// runID = 309622, subRunID = 4, entry = 284290, EV = 0, GTID 11220168
// fileName = "/nfs/disk4/ratds_230515_230715_bronze/Analysis20_r0000309622_s004_p000.root"

// runID = 310365, subRunID = 6, entry = 130684, EV = 0, GTID = 1834806
// fileName = "/nfs/disk4/ratds_230515_230715_bronze/Analysis20_r0000310365_s006_p000.root"

// Out window BiPos events for testing
// runID = 309665, subRunID = 9, entry = 62028 and 62029, EV = 0, GTID = 10626139 and 10626140
// fileName ="/nfs/disk4/ratds_230515_230715_bronze/Analysis20_r0000309665_s009_p000.root"
