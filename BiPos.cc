#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <RAT/DU/DSReader.hh>
#include <RAT/DS/Entry.hh>
#include <RAT/DS/Digitiser.hh>
#include <RAT/CAENBits.hh>

class BiPos
{
    public:
        BiPos(UInt_t irunID = 0, UInt_t iGTID = 0)
        {
            runID = irunID;
            GTID = iGTID;
        }

        UInt_t GetRunID() const {return runID;}
        void SetRunID(const UInt_t irunID) {runID = irunID;}

        UInt_t GetEntry() const {return entry;}
        void SetEntry(const UInt_t iEntry) {entry = iEntry;}

        UInt_t GetEV() const {return EV;}
        void SetEV(const UInt_t iEV) {EV = iEV;}

        UInt_t GetGTID() const {return GTID;}
        void SetGTID(const UInt_t iGTID) {GTID = iGTID;}

        std::string GetFileName() const {return fileName;}
        void SetFileName(const std::string ifileName) {fileName = ifileName;}

        int ReadFile(const std::string ifileName);
        
        void FindLocation();

        TCanvas* PlotWaveforms();

    protected:
        std::string directory = "/nfs/disk4/ratds_230515_230715_bronze/";
    private:
        UInt_t runID;
        UInt_t entry;
        UInt_t EV;
        UInt_t GTID;
        std::string fileName;
};

/// Read a file to find a BiPos event by its GTID
///
/// @param[in] fileName of the RAT::DS root file to read
/// @return int 1 if matching GTID found, 0 if not
int BiPos::ReadFile(const std::string ifileName)
{
    RAT::DU::DSReader dsReader(ifileName);
    for (size_t iEntry = 0; iEntry < dsReader.GetEntryCount(); iEntry++)
    {
        const RAT::DS::Entry &rDS = dsReader.GetEntry(iEntry);
        for (size_t iEV = 0; iEV < rDS.GetEVCount(); iEV++)
        {
            size_t retrievedID = rDS.GetEV(iEV).GetGTID();
            if (retrievedID == GTID)
            {
                fileName = ifileName;
                std::cout << "Entry = " << std::to_string(iEntry) << std::endl;
                entry = iEntry;
                std::cout << "EV = " << std::to_string(iEV) << std::endl;
                EV = iEV;
                return 1;
            }
        }
    }
    return 0;
}

/// Locate the file containing a tagged BiPos event using its runID and GTID
void BiPos::FindLocation()
{
    for (int isubRunID = 0; isubRunID < 13; isubRunID++)
    {
        std::string subRunStr = std::to_string(isubRunID);
        if (isubRunID < 10)
        {
            subRunStr = "0" + subRunStr;
        }
        std::string ifileName = directory + "Analysis20_r0000"+ std::to_string(runID) + "_s0" + subRunStr + "_p000.root";
        std::ifstream file(ifileName);
        if (file.good())
        {
            std::cout << ifileName << std::endl;
            int result = ReadFile(ifileName);
            if (result == 1) {return;}
        }
    }
    std::cout << "GTID not found. Please set valid runID and GTID." << std::endl;
}

/// Plot the CAEN trigger sums for a tagged BiPos event
/// Modified from rat/example/root/PlotCAEN.cc
///
/// @return the canvas
TCanvas* BiPos::PlotWaveforms()
{
    RAT::DU::DSReader dsReader(fileName);
    const RAT::DS::Entry &rDS = dsReader.GetEntry(entry);
    if (rDS.GetEVCount() == 0) // No events to plot
        return NULL;

    TCanvas *c1 = new TCanvas();
    const RAT::DS::Digitiser &digitiser = rDS.GetEV(EV).GetDigitiser();
    std::vector<UShort_t> ids = digitiser.GetIDs();
    // BiPos ids: 4, 10, 20, 40 -> Delayed N20, N100L, N20, ESUMH
    // Swap N20 with Delayed N20 for plotting purposes
    std::swap(ids[0], ids[2]);
    // Calculate a good way to divide the canvas base upon the number of signals
    size_t size = ids.size();
    int y = size < 4 ? 1 : 2;
    int x = ceil(float(size) / y);
    c1->Divide(x, y);

    for (size_t iWaveform = 0; iWaveform < ids.size(); iWaveform++)
    {
        if (!digitiser.ExistsWaveform(ids[iWaveform]))
            continue;
        c1->cd(iWaveform + 1);
        TGraph *graph = new TGraph();
        int id = ids[iWaveform];
        std::vector<UShort_t> waveform = digitiser.GetWaveform(id);

        for (size_t iSample = 0; iSample < waveform.size(); iSample++)
        {
            graph->SetPoint(iSample, iSample, waveform.at(iSample));
        }

        // Assign a title to the plot by parsing the ID.
        // The ID type*10+gain where type and gain are enumerated as specified
        // in CAENBits.hh
        std::string title;
        // int type = id - (id % 10); // Round to the nearest multiple of 10
        if(id == RAT::NH100Lo) {title = "N100L";}
        else if(id == RAT::NH20Lo) {title = "N20";}
        // else if(type == RAT::ESLoLo) {title = "ESUML";}
        else if(id == RAT::ESHiLo) {title = "ESUMH";}
        // else if(type == RAT::OWLNLo) {title = "OWLN";}
        // else if(type == RAT::OWLELoLo) {title = "OWLEL";}
        // else if(type == RAT::OWLEHiLo) {title = "OWLEH";}
        else if(id == 4) {title = "Delayed N20";}
        else {title = "Unknown";}

        graph->SetTitle(title.c_str());
        graph->Draw("AL*");
    }
    c1->cd();
    return c1;
}

// In window BiPos events for testing
// runID = 309622, subRunID = 4, entry = 284290, EV = 0, GTID 11220168
// fileName = "/nfs/disk4/ratds_230515_230715_bronze/Analysis20_r0000309622_s004_p000.root"

// runID = 310365, subRunID = 6, entry = 130684, EV = 0, GTID = 1834806
// fileName = "/nfs/disk4/ratds_230515_230715_bronze/Analysis20_r0000310365_s006_p000.root"

// Out window BiPos events for testing
// runID = 309665, subRunID = 9, entry = 62028 and 62029, EV = 0, GTID = 10626139 and 10626140
// fileName ="/nfs/disk4/ratds_230515_230715_bronze/Analysis20_r0000309665_s009_p000.root"
