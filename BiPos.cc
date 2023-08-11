#include <iostream>
#include <fstream>
#include <string>
#include <RAT/DU/DSReader.hh>
#include <RAT/DS/Entry.hh>
#include <RAT/DS/Digitiser.hh>
#include <RAT/CAENBits.hh>

class BiPos
{
    public:
        BiPos(size_t irunID = 0, size_t iGTID = 0)
        {
            runID = irunID;
            GTID = iGTID;
        }

        size_t GetRunID() const {return runID;}
        void SetRunID(const size_t irunID) {runID = irunID;}

        size_t GetSubRunID() const {return subRunID;}
        void SetSubRunID(const size_t isubRunID) {subRunID = isubRunID;}

        size_t GetEntry() const {return entry;}
        void SetEntry(const size_t iEntry) {entry = iEntry;}

        size_t GetEV() const {return EV;}
        void SetEV(const size_t iEV) {EV = iEV;}

        size_t GetGTID() const {return GTID;}
        void SetGTID(const size_t iGTID) {GTID = iGTID;}

        std::string GetFileName() const {return fileName;}
        void SetFileName(const std::string iFileName) {fileName = iFileName;}

        void FindLocation();

        TCanvas* PlotWaveforms();

    protected:
        std::string directory = "/nfs/disk4/ratds_230515_230715_bronze/";
    private:
        size_t runID;
        size_t subRunID;
        size_t entry;
        size_t EV;
        size_t GTID;
        std::string fileName;
};

// run = 309622, subRun = 4, entry = 284290, EV = 0, GTID 11220168
// fileName = "/nfs/disk4/ratds_230515_230715_bronze/Analysis20_r0000309622_s004_p000.root"

// run = 310365, subRun = 6, entry = 130684, EV = 0, GTID = 1834806
// fileName = "/nfs/disk4/ratds_230515_230715_bronze/Analysis20_r0000310365_s006_p000.root"


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
        std::string ifileName = directory + "Analysis20_r0000" + std::to_string(runID) + "_s0" + subRunStr + "_p000.root";
        std::cout << ifileName << std::endl;
        std::ifstream file(ifileName);
        if (file.good())
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
                        std::cout << "subRunID = " << std::to_string(isubRunID) << std::endl;
                        subRunID = isubRunID;
                        std::cout << "Entry = " << std::to_string(iEntry) << std::endl;
                        entry = iEntry;
                        std::cout << "EV = " << std::to_string(iEV) << std::endl;
                        EV = iEV;
                        return;
                    }
                }
            }
        }
    }
    std::cout << "GTID not found. Please set runID and GTID." << std::endl;
}

/// Plot the CAEN trigger sums for a tagged BiPos event
/// Modified from rat/example/root/PlotCAEN.cc
///
/// @return the canvas
void BiPos::PlotWaveforms()
{
    RAT::DU::DSReader dsReader(fileName);
    const RAT::DS::Entry &rDS = dsReader.GetEntry(entry);
    if (rDS.GetEVCount() == 0) // No events to plot
        return NULL;

    TCanvas *c1 = new TCanvas();
    const RAT::DS::Digitiser &digitiser = rDS.GetEV(EV).GetDigitiser();
    std::vector<UShort_t> ids = digitiser.GetIDs();
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
        int type = id - (id % 10); // Round to the nearest multiple of 10
        if(type == RAT::NH100Lo) {title = "N100";}
        else if(type == RAT::NH20Lo) {title = "N20";}
        else if(type == RAT::ESLoLo) {title = "ESUML";}
        else if(type == RAT::ESHiLo) {title = "ESUMH";}
        else if(type == RAT::OWLNLo) {title = "OWLN";}
        else if(type == RAT::OWLELoLo) {title = "OWLEL";}
        else if(type == RAT::OWLEHiLo) {title = "OWLEH";}
        else {title = "Unknown";}

        graph->SetTitle(title.c_str());
        graph->Draw("AL*");
    }
    c1->cd();
    return c1;
}