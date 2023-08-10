#include <iostream>
#include <fstream>
#include <string>
#include <RAT/DU/DSReader.hh>
#include <RAT/DS/Entry.hh>
#include <RAT/DS/Digitiser.hh>
#include <RAT/CAENBits.hh>

struct BiPos
{
    std::string fileName;
    size_t run;
    size_t subRun;
    size_t entry;
    size_t EV;
    size_t GTID;
};

BiPos GetBiPos(const size_t &runID, const size_t &GTID)
{
    BiPos result;
    result.run = runID;
    result.GTID = GTID;
    const std::string directory = "/nfs/disk4/ratds_230515_230715_bronze/";
    for (int subRunID = 0; subRunID < 13; subRunID++)
    {
        std::string subRunStr = std::to_string(subRunID);
        if (subRunID < 10)
        {
            subRunStr = "0" + subRunStr;
        }
        std::string fileName = directory + "Analysis20_r0000" + std::to_string(runID) + "_s0" + subRunStr + "_p000.root";
        std::cout << fileName << std::endl;
        std::ifstream file(fileName);
        if (file.good())
        {
            RAT::DU::DSReader dsReader(fileName);
            for (size_t iEntry = 0; iEntry < dsReader.GetEntryCount(); iEntry++)
            {
                const RAT::DS::Entry &rDS = dsReader.GetEntry(iEntry);
                for (size_t iEV = 0; iEV < rDS.GetEVCount(); iEV++)
                {
                    size_t retrievedID = rDS.GetEV(iEV).GetGTID();
                    if (retrievedID == GTID)
                    {
                        result.fileName = fileName;
                        std::cout << "subRun = " << std::to_string(subRunID) << std::endl;
                        result.subRun = subRunID;
                        std::cout << "Entry = " << std::to_string(iEntry) << std::endl;
                        result.entry = iEntry;
                        std::cout << "EV = " << std::to_string(iEV) << std::endl;
                        result.EV = iEV;
                        return result;
                    }
                }
            }
        }
    }
    std::cout << "GTID not found" << std::endl;
    return result;
}

// run = 309622, subRun = 4, entry = 284290, EV = 0, GTID 11220168
// fileName = "/nfs/disk4/ratds_230515_230715_bronze/Analysis20_r0000309622_s004_p000.root"

// run = 310365, subRun = 6, entry = 130684, EV = 0, GTID = 1834806
// fileName = "/nfs/disk4/ratds_230515_230715_bronze/Analysis20_r0000310365_s006_p000.root"

/// Plot the CAEN trigger sums for a tagged BiPos event
/// Modified from rat/example/root/PlotCAEN.cc
///
/// @param[in] fileName of the RAT::DS root file to analyse
/// @param[in] entry to analyse
/// @return the canvas
TCanvas* PlotBiPos(BiPos input)
{
    RAT::DU::DSReader dsReader(input.fileName);
    const RAT::DS::Entry &rDS = dsReader.GetEntry(input.entry);
    if (rDS.GetEVCount() == 0) // No events to plot
        return NULL;

    TCanvas *c1 = new TCanvas();
    const RAT::DS::Digitiser &digitiser = rDS.GetEV(input.EV).GetDigitiser();
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