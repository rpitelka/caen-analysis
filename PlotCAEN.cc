#include <RAT/DU/DSReader.hh>
#include <RAT/DS/Entry.hh>
#include <RAT/DS/Digitiser.hh>
#include <RAT/CAENBits.hh>
#include <TGraph.h>
#include <TCanvas.h>

#include <string>

/// Plot the CAEN trigger sums for a specified event
/// From rat/example/root
///
/// @param[in] fileName of the RAT::DS root file to analyse
/// @param[in] eventID to analyse
/// @return the canvas
TCanvas* PlotCAEN( const char* fileName, size_t eventID )
{
  RAT::DU::DSReader dsReader( fileName );
  const RAT::DS::Entry& rDS = dsReader.GetEntry( eventID );
  if( rDS.GetEVCount() == 0 ) // No events to plot
    return NULL;

  TCanvas* c1 = new TCanvas();
  const RAT::DS::Digitiser& digitiser = rDS.GetEV( 0 ).GetDigitiser();
  std::vector<UShort_t>ids = digitiser.GetIDs();
  // Calculate a good way to divide the canvas base upon the number of signals
  size_t size = ids.size();
  int y = size<4 ? 1 : 2;
  int x = ceil(float(size)/y);
  c1->Divide(x, y);

  for( size_t iWaveform = 0; iWaveform < ids.size(); iWaveform++ )
    {
      if( !digitiser.ExistsWaveform( ids[iWaveform] ) )
        continue;
      c1->cd( iWaveform + 1 );
      TGraph* graph = new TGraph();
      int id = ids[iWaveform];
      std::vector<UShort_t> waveform = digitiser.GetWaveform(id);

      for( size_t iSample = 0; iSample < waveform.size(); iSample++ )
      { graph->SetPoint( iSample, iSample, waveform.at( iSample ) ); }

      // Assign a title to the plot by parsing the ID.
      // The ID type*10+gain where type and gain are enumerated as specified
      // in CAENBits.hh
      std::string title;
      int type = id - (id%10); // Round to the nearest multiple of 10
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
