////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Package:    ZdcSimHitStudy
// Class:      ZdcSimHitStudy
//
/*
 Description:
 This code has been developed to be a check for the ZDC sim. In 2009, it was found that the ZDC Simulation was unrealistic and needed repair. The aim of this code is to show the user the input and output of a ZDC MinBias simulation.
 
 Implementation:
 First a MinBias simulation should be run, it could be pythia,hijin,or hydjet. This will output a .root file which should have information about recoGenParticles, and g4SimHits_ZDCHits. Use this .root file as the input into the cfg.py which is found in the main directory of this package. This output will be another .root file which is meant to be viewed in a TBrowser.
 
 */
//
// Original Author: Jaime Gomez (U. of Maryland) with SIGNIFICANT assistance of Dr. Jefferey Temple (U. of Maryland)
// Adapted from: E. Garcia-Solis' (CSU) original code
//
//         Created:  Summer 2012
//
/*
 Update: 
 The simulation has been updated to include the new reaction plane flow detector (RPD) that will be installed for the 2015 heavy-ion run.
 // Update Author: Will McBrayer (U. of Kansas) with assistance from Chris Ferraioli and Jaime Gomez (both of the U. of Maryland). 
 //        Updated:  Summer 2015
 */
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





#include "Validation/HcalHits/interface/ZdcSimHitStudy.h"
#include "DataFormats/HcalDetId/interface/HcalZDCDetId.h"

#include "FWCore/Utilities/interface/Exception.h"
#include "CLHEP/Units/GlobalSystemOfUnits.h"

ZdcSimHitStudy::ZdcSimHitStudy(const edm::ParameterSet& ps) {
    
    g4Label  = ps.getUntrackedParameter<std::string>("moduleLabel","g4SimHits");
    zdcHits = ps.getUntrackedParameter<std::string>("HitCollection","ZdcHits");
    outFile_ = ps.getUntrackedParameter<std::string>("outputFile", "zdcHitStudy.root");
    verbose_ = ps.getUntrackedParameter<bool>("Verbose", false);
    checkHit_= true;
    
    tok_gen_ = consumes<reco::GenParticleCollection>(edm::InputTag("genParticles"));
    tok_hits_ = consumes<edm::PCaloHitContainer>(edm::InputTag(g4Label,zdcHits));
    
    edm::LogInfo("ZdcSimHitStudy")
    //std::cout
    << "Module Label: " << g4Label << "   Hits: "
    << zdcHits << " / "<< checkHit_
    << "   Output: " << outFile_;
    
}

ZdcSimHitStudy::~ZdcSimHitStudy() {}

void ZdcSimHitStudy::bookHistograms(DQMStore::IBooker &ib, edm::Run const &run, edm::EventSetup const &es) {
    ib.setCurrentFolder("ZDCValidation");
//Histograms for Hits
////////////////////////////////////////////////////////////////////////////////////////////////////////
//# Below we are filling the histograms made in the .h file. The syntax is as follows:                 #
//# plot_code_name = ib.TypeofPlot[(1,2,3)-D,(F,I,D)]("Name as it will appear","Title",axis options);  #
//# They will be stored in the TFile subdirectory set by :    ib.setCurrentFolder("FolderIwant")       #
//# axis options are like (#ofbins,min,max)                                                            #
////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    
    if (checkHit_) {
        /////////////////////////1///////////////////////////
        ib.setCurrentFolder("ZDCValidation/ZdcSimHits");
        meAllZdcNHit_ = ib.book1D("ZDC Hits","Number of All Hits in ZDC",100,0.,100.);
        meAllZdcNHit_->setAxisTitle("Total Hits",1);
        meAllZdcNHit_->setAxisTitle("Counts",2);
        /////////////////////////2///////////////////////////
        ib.setCurrentFolder("ZDCValidation/ZdcSimHits/Excess_Info/Debug_Helper");
        meBadZdcDetHit_= ib.book1D("Hiits with the wrong Det","Hits with wrong Det in ZDC",100,0.,100.);
        meBadZdcDetHit_->setAxisTitle("Wrong Hits",1);
        meBadZdcDetHit_->setAxisTitle("Counts",2);
        /////////////////////////3///////////////////////////
        meBadZdcSecHit_= ib.book1D("Wrong Section Hits","Hits with wrong Section in ZDC",100,0.,100.);
        meBadZdcSecHit_->setAxisTitle("Hits in wrong section",1);
        meBadZdcSecHit_->setAxisTitle("Counts",2);
        /////////////////////////4///////////////////////////
        meBadZdcIdHit_ = ib.book1D("Wrong_ID_Hits","Hits with wrong ID in ZDC",100,0.,100.);
        meBadZdcIdHit_->setAxisTitle("Hits with wrong ID",1);
        meBadZdcIdHit_->setAxisTitle("Counts",2);
        /////////////////////////5///////////////////////////
        ib.setCurrentFolder("ZDCValidation/ZdcSimHits/Excess_Info/BasicHitInfo");
        meZdcNHitEM_   = ib.book1D("Hits in EM","Number of Hits in ZDC EM",100,0.,100.);
        meZdcNHitEM_->setAxisTitle("EM Hits",1);
        meZdcNHitEM_->setAxisTitle("Counts",2);
        /////////////////////////6///////////////////////////
        meZdcNHitHad_   = ib.book1D("Hits in HAD","Number of Hits in ZDC Had",100,0.,100.);
        meZdcNHitHad_->setAxisTitle("HAD Hits",1);
        meZdcNHitHad_->setAxisTitle("Counts",2);
        /////////////////////////7///////////////////////////
        meZdcNHitLum_   = ib.book1D("Hits in LUM","Number of Hits in ZDC Lum",100,0.,100.);
        meZdcNHitLum_->setAxisTitle("LUM Hits",1);
        meZdcNHitLum_->setAxisTitle("Counts",2);
        /////////////////////////8///////////////////////////
        meZdcDetectHit_= ib.book1D("Calo Detector ID","Calo Detector ID",50,0.,50.);
        meZdcDetectHit_->setAxisTitle("Detector Hits",1);
        meZdcDetectHit_->setAxisTitle("Counts",2);
        /////////////////////////9///////////////////////////
        meZdcSideHit_ = ib.book1D("ZDC Side","Side in ZDC",4,-2,2.);
        meZdcSideHit_->setAxisTitle("ZDC Side",1);
        meZdcSideHit_->setAxisTitle("Counts",2);
        /////////////////////////10///////////////////////////
        meZdcSectionHit_   = ib.book1D("ZDC Section","Section in ZDC",4,0.,4.);
        meZdcSectionHit_->setAxisTitle("ZDC Section",1);
        meZdcSectionHit_->setAxisTitle("Counts",2);
        /////////////////////////11///////////////////////////
        meZdcChannelHit_   = ib.book1D("ZDC Channel","Channel in ZDC",10,0.,10.);
        meZdcChannelHit_->setAxisTitle("ZDC Channel",1);
        meZdcChannelHit_->setAxisTitle("Counts",2);
        /////////////////////////12///////////////////////////
        ib.setCurrentFolder("ZDCValidation/ZdcSimHits/");
        meZdcEnergyHit_= ib.book1D("Hit Energy","Hits Energy",4000,0.,8000.);
        meZdcEnergyHit_->setAxisTitle("Counts",2);
        meZdcEnergyHit_->setAxisTitle("Energy (GeV)",1);
        /////////////////////////13///////////////////////////
        meZdcHadEnergyHit_= ib.book1D("Hit Energy HAD","Hits Energy in Had Section",4000,0.,8000.);
        meZdcHadEnergyHit_->setAxisTitle("Counts",2);
        meZdcHadEnergyHit_->setAxisTitle("Energy (GeV)",1);
        /////////////////////////14///////////////////////////
        meZdcEMEnergyHit_ = ib.book1D("Hit Energy EM","Hits Energy in EM Section",4000,0.,8000.);
        meZdcEMEnergyHit_->setAxisTitle("Counts",2);
        meZdcEMEnergyHit_->setAxisTitle("Energy (GeV)",1);
        /////////////////////////15///////////////////////////
        ib.setCurrentFolder("ZDCValidation/ZdcSimHits/Excess_Info/BasicHitInfo");
        meZdcTimeHit_  = ib.book1D("Time in ZDC","Time in ZDC",300,0.,600.);
        meZdcTimeHit_->setAxisTitle("Time (ns)",1);
        meZdcTimeHit_->setAxisTitle("Counts",2);
        /////////////////////////16///////////////////////////
        meZdcTimeWHit_ = ib.book1D("Energy Weighted Time in ZDC","Time in ZDC (E wtd)", 300,0.,600.);
        meZdcTimeWHit_->setAxisTitle("Time (ns)",1);
        meZdcTimeWHit_->setAxisTitle("Counts",2);
        /////////////////////////17///////////////////////////
        meZdc10Ene_ = ib.book1D("ZDC Log(E)","Log10Energy in ZDC", 140, -20., 20. );
        meZdc10Ene_->setAxisTitle("Log(E) (GeV)",1);
        meZdc10Ene_->setAxisTitle("Counts",2);
        /////////////////////////18///////////////////////////
        meZdcHadL10EneP_ = ib.bookProfile("Log(EHAD) vs Contribution","Log10Energy in Had ZDC vs Hit contribution", 140, -1., 20., 100, 0., 1. );
        meZdcHadL10EneP_->setAxisTitle("Log(EHAD) (GeV)",1);
        meZdcHadL10EneP_->setAxisTitle("Counts",2);
        /////////////////////////19///////////////////////////
        meZdcEML10EneP_ = ib.bookProfile("Log(EEM) vs Contribution","Log10Energy in EM ZDC vs Hit contribution", 140, -1., 20., 100, 0., 1. );
        meZdcEML10EneP_->setAxisTitle("Log(EEM) (GeV)",1);
        meZdcEML10EneP_->setAxisTitle("Counts",2);
        /////////////////////////20///////////////////////////
        ib.setCurrentFolder("ZDCValidation/ZdcSimHits");
        meZdcEHadCh_ = ib.book2D("ZDC EHAD vs Channel","ZDC Had Section Energy vs Channel", 4000, 0., 8000., 6, 0., 6. );
        meZdcEHadCh_->setAxisTitle("Hadronic Channel Number",2);
        meZdcEHadCh_->setAxisTitle("Energy (GeV)",1);
        /////////////////////////21///////////////////////////
        meZdcEEMCh_ = ib.book2D("ZDC EEM vs Channel","ZDC EM Section Energy vs Channel", 4000, 0., 8000., 6, 0., 6. );
        meZdcEEMCh_->setAxisTitle("EM Channel Number",2);
        meZdcEEMCh_->setAxisTitle("Energy (GeV)",1);
        /////////////////////////22///////////////////////////
        ib.setCurrentFolder("ZDCValidation/ZdcSimHits/Excess_Info/BasicHitInfo");
        meZdcETime_ = ib.book2D("E vs T","Hits ZDC Energy vs Time", 4000, 0., 8000., 300, 0., 600. );
        meZdcETime_->setAxisTitle("Energy (GeV)",1);
        meZdcETime_->setAxisTitle("Time (ns)",2);
        /////////////////////////23///////////////////////////
        ib.setCurrentFolder("ZDCValidation/ZdcSimHits/ENERGY_SUMS/Individual_Channels/NZDC");
        meZdcEneEmN1_  = ib.book1D("NZDC EM1 Energy","Energy EM module N1",4000,0.,8000.);
        meZdcEneEmN1_->setAxisTitle("Energy (GeV)",1);
        meZdcEneEmN1_->setAxisTitle("Counts",2);
        /////////////////////////24///////////////////////////
        meZdcEneEmN2_  = ib.book1D("NZDC EM2 Energy","Energy EM module N2",4000,0.,8000.);
        meZdcEneEmN2_->setAxisTitle("Energy (GeV)",1);
        meZdcEneEmN2_->setAxisTitle("Counts",2);
        /////////////////////////25///////////////////////////
        meZdcEneEmN3_  = ib.book1D("NZDC EM3 Energy","Energy EM module N3",4000,0.,8000.);
        meZdcEneEmN3_->setAxisTitle("Energy (GeV)",1);
        meZdcEneEmN3_->setAxisTitle("Counts",2);
        /////////////////////////26///////////////////////////
        meZdcEneEmN4_  = ib.book1D("NZDC EM4 Energy","Energy EM module N4",4000,0.,8000.);
        meZdcEneEmN4_->setAxisTitle("Energy (GeV)",1);
        meZdcEneEmN4_->setAxisTitle("Counts",2);
        /////////////////////////27///////////////////////////
        meZdcEneEmN5_  = ib.book1D("NZDC EM5 Energy","Energy EM module N5",4000,0.,8000.);
        meZdcEneEmN5_->setAxisTitle("Energy (GeV)",1);
        meZdcEneEmN5_->setAxisTitle("Counts",2);
        /////////////////////////28///////////////////////////
        meZdcEneHadN1_ = ib.book1D("NZDC HAD1 Energy","Energy HAD module N1",4000,0.,8000.);
        meZdcEneHadN1_->setAxisTitle("Energy (GeV)",1);
        meZdcEneHadN1_->setAxisTitle("Counts",2);
        /////////////////////////29///////////////////////////
        meZdcEneHadN2_ = ib.book1D("NZDC HAD2 Energy","Energy HAD module N2",4000,0.,8000.);
        meZdcEneHadN2_->setAxisTitle("Energy (GeV)",1);
        meZdcEneHadN2_->setAxisTitle("Counts",2);
        /////////////////////////30///////////////////////////
        meZdcEneHadN3_ = ib.book1D("NZDC HAD3 Energy","Energy HAD module N3",4000,0.,8000.);
        meZdcEneHadN3_->setAxisTitle("Energy (GeV)",1);
        meZdcEneHadN3_->setAxisTitle("Counts",2);
        /////////////////////////31///////////////////////////
        meZdcEneHadN4_ = ib.book1D("NZDC HAD4 Energy","Energy HAD module N4",4000,0.,8000.);
        meZdcEneHadN4_->setAxisTitle("Energy (GeV)",1);
        meZdcEneHadN4_->setAxisTitle("Counts",2);
        /////////////////////////32///////////////////////////
        ib.setCurrentFolder("ZDCValidation/ZdcSimHits/Excess_Info/Individual_ChannelvsTime/NZDC");
        meZdcEneTEmN1_ = ib.book2D("NZDC EM1 Energy vs Time","Energy EM mod N1 vs Time", 4000, 0., 8000., 300, 0., 600. );
        meZdcEneTEmN1_->setAxisTitle("Energy (GeV)",1);
        meZdcEneTEmN1_->setAxisTitle("Time (ns)",2);
        /////////////////////////33///////////////////////////
        meZdcEneTEmN2_ = ib.book2D("NZDC EM2 Energy vs Time","Energy EM mod N2 vs Time", 4000, 0., 8000., 300, 0., 600. );
        meZdcEneTEmN2_->setAxisTitle("Energy (GeV)",1);
        meZdcEneTEmN2_->setAxisTitle("Time (ns)",2);
        /////////////////////////34///////////////////////////
        meZdcEneTEmN3_ = ib.book2D("NZDC EM3 Energy vs Time","Energy EM mod N3 vs Time", 4000, 0., 8000., 300, 0., 600. );
        meZdcEneTEmN3_->setAxisTitle("Energy (GeV)",1);
        meZdcEneTEmN3_->setAxisTitle("Time (ns)",2);
        /////////////////////////35///////////////////////////
        meZdcEneTEmN4_ = ib.book2D("NZDC EM4 Energy vs Time","Energy EM mod N4 vs Time", 4000, 0., 8000., 300, 0., 600. );
        meZdcEneTEmN4_->setAxisTitle("Energy (GeV)",1);
        meZdcEneTEmN4_->setAxisTitle("Time (ns)",2);
        /////////////////////////36///////////////////////////
        meZdcEneTEmN5_ = ib.book2D("NZDC EM5 Energy vs Time","Energy EM mod N5 vs Time", 4000, 0., 8000., 300, 0., 600. );
        meZdcEneTEmN5_->setAxisTitle("Energy (GeV)",1);
        meZdcEneTEmN5_->setAxisTitle("Time (ns)",2);
        /////////////////////////37///////////////////////////
        meZdcEneTHadN1_ = ib.book2D("NZDC HAD1 Energy vs Time","Energy HAD mod N1 vs Time", 4000, 0., 8000., 300, 0., 600. );
        meZdcEneTHadN1_->setAxisTitle("Energy (GeV)",1);
        meZdcEneTHadN1_->setAxisTitle("Time (ns)",2);
        /////////////////////////38///////////////////////////
        meZdcEneTHadN2_ = ib.book2D("NZDC HAD2 Energy vs Time","Energy HAD mod N2 vs Time", 4000, 0., 8000., 300, 0., 600. );
        meZdcEneTHadN2_->setAxisTitle("Energy (GeV)",1);
        meZdcEneTHadN2_->setAxisTitle("Time (ns)",2);
        /////////////////////////39///////////////////////////
        meZdcEneTHadN3_ = ib.book2D("NZDC HAD3 Energy vs Time","Energy HAD mod N3 vs Time", 4000, 0., 8000., 300, 0., 600. );
        meZdcEneTHadN3_->setAxisTitle("Energy (GeV)",1);
        meZdcEneTHadN3_->setAxisTitle("Time (ns)",2);
        /////////////////////////40///////////////////////////
        meZdcEneTHadN4_ = ib.book2D("NZDC HAD4 Energy vs Time","Energy HAD mod N4 vs Time", 4000, 0., 8000., 300, 0., 600. );
        meZdcEneTHadN4_->setAxisTitle("Energy (GeV)",1);
        meZdcEneTHadN4_->setAxisTitle("Time (ns)",2);
        /////////////////////////41///////////////////////////
        ib.setCurrentFolder("ZDCValidation/ZdcSimHits/ENERGY_SUMS/NZDC");
        meZdcEneHadNTot_ = ib.book1D("NZDC EHAD","Total N-ZDC HAD Energy",4000,0.,4000.);
        meZdcEneHadNTot_->setAxisTitle("Counts",2);
        meZdcEneHadNTot_->setAxisTitle("Energy (GeV)",1);
        /////////////////////////42///////////////////////////
        meZdcEneEmNTot_ = ib.book1D("NZDC EEM","Total N-ZDC EM Energy",3000,0.,3000.);
        meZdcEneEmNTot_->setAxisTitle("Counts",2);
        meZdcEneEmNTot_->setAxisTitle("Energy (GeV)",1);
        /////////////////////////43///////////////////////////
        meZdcEneNTot_ = ib.book1D("NZDC ETOT","Total N-ZDC Energy ",7000,0.,7000.);
        meZdcEneNTot_->setAxisTitle("Counts",2);
        meZdcEneNTot_->setAxisTitle("Energy (GeV)",1);
        /////////////////////////44///////////////////////////
        ib.setCurrentFolder("ZDCValidation/ZdcSimHits/ENERGY_SUMS/Individual_Channels/PZDC");
        meZdcEneEmP1_ = ib.book1D("PZDC EM1 Energy","Energy EM module P1",3000,0.,3000.);
        meZdcEneEmP1_->setAxisTitle("Energy (GeV)",1);
        meZdcEneEmP1_->setAxisTitle("Counts",2);
        /////////////////////////45///////////////////////////
        meZdcEneEmP2_ = ib.book1D("PZDC EM2 Energy","Energy EM module P2",3000,0.,3000.);
        meZdcEneEmP2_->setAxisTitle("Energy (GeV)",1);
        meZdcEneEmP2_->setAxisTitle("Counts",2);
        /////////////////////////46///////////////////////////
        meZdcEneEmP3_ = ib.book1D("PZDC EM3 Energy","Energy EM module P3",3000,0.,3000.);
        meZdcEneEmP3_->setAxisTitle("Energy (GeV)",1);
        meZdcEneEmP3_->setAxisTitle("Counts",2);
        /////////////////////////47///////////////////////////
        meZdcEneEmP4_ = ib.book1D("PZDC EM4 Energy","Energy EM module P4",3000,0.,3000.);
        meZdcEneEmP4_->setAxisTitle("Energy (GeV)",1);
        meZdcEneEmP4_->setAxisTitle("Counts",2);
        /////////////////////////48///////////////////////////
        meZdcEneEmP5_ = ib.book1D("PZDC EM5 Energy","Energy EM module P5",3000,0.,3000.);
        meZdcEneEmP5_->setAxisTitle("Energy (GeV)",1);
        meZdcEneEmP5_->setAxisTitle("Counts",2);
        /////////////////////////49///////////////////////////
        meZdcEneHadP1_ = ib.book1D("PZDC HAD1 Energy","Energy HAD module P1",3000,0.,3000.);
        meZdcEneHadP1_->setAxisTitle("Energy (GeV)",1);
        meZdcEneHadP1_->setAxisTitle("Counts",2);
        /////////////////////////50///////////////////////////
        meZdcEneHadP2_ = ib.book1D("PZDC HAD2 Energy","Energy HAD module P2",3000,0.,3000.);
        meZdcEneHadP2_->setAxisTitle("Energy (GeV)",1);
        meZdcEneHadP2_->setAxisTitle("Counts",2);
        /////////////////////////51///////////////////////////
        meZdcEneHadP3_ = ib.book1D("PZDC HAD3 Energy","Energy HAD module P3",3000,0.,3000.);
        meZdcEneHadP3_->setAxisTitle("Energy (GeV)",1);
        meZdcEneHadP3_->setAxisTitle("Counts",2);
        /////////////////////////52///////////////////////////
        meZdcEneHadP4_ = ib.book1D("PZDC HAD4 Energy","Energy HAD module P4",3000,0.,3000.);
        meZdcEneHadP4_->setAxisTitle("Energy (GeV)",1);
        meZdcEneHadP4_->setAxisTitle("Counts",2);
        /////////////////////////53///////////////////////////
        ib.setCurrentFolder("ZDCValidation/ZdcSimHits/Excess_Info/Individual_ChannelvsTime/PZDC");
        meZdcEneTEmP1_ = ib.book2D("PZDC EM1 Energy vs Time","Energy EM mod P1 vs Time", 4000, 0., 8000., 300, 0., 600. );
        meZdcEneTEmP1_->setAxisTitle("Energy (GeV)",1);
        meZdcEneTEmP1_->setAxisTitle("Time (ns)",2);
        /////////////////////////54///////////////////////////
        meZdcEneTEmP2_ = ib.book2D("PZDC EM2 Energy vs Time","Energy EM mod P2 vs Time", 4000, 0., 8000., 300, 0., 600. );
        meZdcEneTEmP2_->setAxisTitle("Energy (GeV)",1);
        meZdcEneTEmP2_->setAxisTitle("Time (ns)",2);
        /////////////////////////55///////////////////////////
        meZdcEneTEmP3_ = ib.book2D("PZDC EM3 Energy vs Time","Energy EM mod P3 vs Time", 4000, 0., 8000., 300, 0., 600. );
        meZdcEneTEmP3_->setAxisTitle("Energy (GeV)",1);
        meZdcEneTEmP3_->setAxisTitle("Time (ns)",2);
        /////////////////////////56///////////////////////////
        meZdcEneTEmP4_ = ib.book2D("PZDC EM4 Energy vs Time","Energy EM mod P4 vs Time", 4000, 0., 8000., 300, 0., 600. );
        meZdcEneTEmP4_->setAxisTitle("Energy (GeV)",1);
        meZdcEneTEmP4_->setAxisTitle("Time (ns)",2);
        /////////////////////////57///////////////////////////
        meZdcEneTEmP5_ = ib.book2D("PZDC EM5 Energy vs Time","Energy EM mod P5 vs Time", 4000, 0., 8000., 300, 0., 600. );
        meZdcEneTEmP5_->setAxisTitle("Energy (GeV)",1);
        meZdcEneTEmP5_->setAxisTitle("Time (ns)",2);
        /////////////////////////58///////////////////////////
        meZdcEneTHadP1_ = ib.book2D("PZDC HAD1 Energy vs Time","Energy HAD mod P1 vs Time", 4000, 0., 8000., 300, 0., 600. );
        meZdcEneTHadP1_->setAxisTitle("Energy (GeV)",1);
        meZdcEneTHadP1_->setAxisTitle("Time (ns)",2);
        /////////////////////////59///////////////////////////
        meZdcEneTHadP2_ = ib.book2D("PZDC HAD2 Energy vs Time","Energy HAD mod P2 vs Time", 4000, 0., 8000., 300, 0., 600. );
        meZdcEneTHadP2_->setAxisTitle("Energy (GeV)",1);
        meZdcEneTHadP2_->setAxisTitle("Time (ns)",2);
        /////////////////////////60///////////////////////////
        meZdcEneTHadP3_ = ib.book2D("PZDC HAD3 Energy vs Time","Energy HAD mod P3 vs Time", 4000, 0., 8000., 300, 0., 600. );
        meZdcEneTHadP3_->setAxisTitle("Energy (GeV)",1);
        meZdcEneTHadP3_->setAxisTitle("Time (ns)",2);
        /////////////////////////61///////////////////////////
        meZdcEneTHadP4_ = ib.book2D("PZDC HAD4 Energy vs Time","Energy HAD mod P4 vs Time", 4000, 0., 8000., 300, 0., 600. );
        meZdcEneTHadP4_->setAxisTitle("Energy (GeV)",1);
        meZdcEneTHadP4_->setAxisTitle("Time (ns)",2);
        /////////////////////////62/////////////////////////////
        ib.setCurrentFolder("ZDCValidation/ZdcSimHits/ENERGY_SUMS/PZDC");
        meZdcEneHadPTot_ = ib.book1D("PZDC EHAD","Total P-ZDC HAD Energy",10000,0.,10000.);
        meZdcEneHadPTot_->setAxisTitle("Energy (GeV)",1);
        meZdcEneHadPTot_->setAxisTitle("Counts",2);
        /////////////////////////63///////////////////////////
        meZdcEneEmPTot_ = ib.book1D("PZDC EEM","Total P-ZDC EM Energy",10000,0.,10000.);
        meZdcEneEmPTot_->setAxisTitle("Energy (GeV)",1);
        meZdcEneEmPTot_->setAxisTitle("Counts",2);
        /////////////////////////64///////////////////////////
        meZdcEnePTot_ = ib.book1D("PZDC ETOT","Total P-ZDC Energy",10000,0.,10000.);
        meZdcEnePTot_->setAxisTitle("Energy (GeV)",1);
        meZdcEnePTot_->setAxisTitle("Counts",2);
        /////////////////////////65///////////////////////////
        ib.setCurrentFolder("ZDCValidation/ZdcSimHits/ENERGY_SUMS/NZDC");
        meZdcCorEEmNEHadN_= ib.book2D("NZDC EMvHAD","N-ZDC Energy EM vs HAD", 3000, 0., 3000.,3000, 0., 3000.);
        meZdcCorEEmNEHadN_->setAxisTitle("EM Energy (GeV)",1);
        meZdcCorEEmNEHadN_->setAxisTitle("HAD Energy (GeV)",2);
        /////////////////////////66///////////////////////////
        ib.setCurrentFolder("ZDCValidation/ZdcSimHits/ENERGY_SUMS/PZDC");
        meZdcCorEEmPEHadP_= ib.book2D("PZDC EMvHAD","P-ZDC Energy EM vs HAD", 3000, 0., 3000.,3000, 0., 3000.);
        meZdcCorEEmPEHadP_->setAxisTitle("EM Energy (GeV)",1);
        meZdcCorEEmPEHadP_->setAxisTitle("HAD Energy (GeV)",2);
        /////////////////////////67///////////////////////////
        ib.setCurrentFolder("ZDCValidation/ZdcSimHits/ENERGY_SUMS");
        meZdcCorEtotNEtotP_ = ib.book2D("PZDC vs NZDC","Energy N-ZDC vs P-ZDC", 3000, 0., 3000.,3000, 0., 3000.);
        meZdcCorEtotNEtotP_->setAxisTitle("N-ZDC Total Energy (GeV)",1);
        meZdcCorEtotNEtotP_->setAxisTitle("P-ZDC Total Energy (GeV)",2);
        /////////////////////////68///////////////////////////
        meZdcEneTot_ = ib.book1D("ETOT ZDCs","Total Energy ZDCs",3000,0.,3000.);
        meZdcEneTot_->setAxisTitle("Counts",2);
        meZdcEneTot_->setAxisTitle("Energy (GeV)",1);
        ///////////////////////////////////////////////////////////
        
        
        
        
        //////////////////New Plots////////////////////////////////
        
        ////////////////////// GenParticle Plots///////////////////////////////////////
        ib.setCurrentFolder("ZDCValidation/GenParticles/Forward");
        //////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////69/////////////////////////////////////////////
        genpart_Pi0F = ib.book2D("Pi0_Forward","Forward Generated Pi0s",200,7.5,13,100,-3.15,3.15);
        genpart_Pi0F->setAxisTitle("Eta",1);
        genpart_Pi0F->setAxisTitle("Phi (radians)",2);
        genpart_Pi0F->setAxisTitle("Energy (GeV)",3);
        genpart_Pi0F->getTH2F()->SetOption("lego2z,prof");
        genpart_Pi0F->getTH2F()->SetTitleOffset(1.4,"x");
        genpart_Pi0F->getTH2F()->SetTitleOffset(1.4,"y");
        
        genpart_Pi0F_counts = ib.book2D("Pi0_Forward_Counts","Number of Forward Generated Pi0s",200,7.5,13,100,-3.15,3.15);
        genpart_Pi0F_counts->setAxisTitle("#eta",1);
        genpart_Pi0F_counts->setAxisTitle("#phi (radians)",2);
        genpart_Pi0F_counts->setAxisTitle("Energy (GeV)",3);
        genpart_Pi0F_counts->getTH2F()->SetOption("lego2z,prof");
        genpart_Pi0F_counts->getTH2F()->SetTitleOffset(1.4,"x");
        genpart_Pi0F_counts->getTH2F()->SetTitleOffset(1.4,"y");
        
        /////////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////70/////////////////////////////////////////////
        genpart_NeutF = ib.book2D("Neutron_Forward","Forward Generated Neutrons",200,7.5,13,100,-3.15,3.15);
        genpart_NeutF->setAxisTitle("Eta",1);
        genpart_NeutF->setAxisTitle("Phi (radians)",2);
        genpart_NeutF->setAxisTitle("Energy (GeV)",3);
        genpart_NeutF->getTH2F()->SetOption("lego2z,prof");
        genpart_NeutF->getTH2F()->SetTitleOffset(1.4,"x");
        genpart_NeutF->getTH2F()->SetTitleOffset(1.4,"y");
        
        genpart_NeutF_counts = ib.book2D("Neutron_Forward_Counts","Number of Forward Generated Neutrons",200,7.5,13,100,-3.15,3.15);
        genpart_NeutF_counts->setAxisTitle("#eta",1);
        genpart_NeutF_counts->setAxisTitle("#phi (radians)",2);
        genpart_NeutF_counts->setAxisTitle("Energy (GeV)",3);
        genpart_NeutF_counts->getTH2F()->SetOption("lego2z,prof");
        genpart_NeutF_counts->getTH2F()->SetTitleOffset(1.4,"x");
        genpart_NeutF_counts->getTH2F()->SetTitleOffset(1.4,"y");
        
        /////////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////71/////////////////////////////////////////////
        genpart_GammaF = ib.book2D("Gamma_Forward","Forward Generated Gammas",200,7.5,13,100,-3.15,3.15);
        genpart_GammaF->setAxisTitle("Eta",1);
        genpart_GammaF->setAxisTitle("Phi (radians)",2);
        genpart_GammaF->setAxisTitle("Energy (GeV)",3);
        genpart_GammaF->getTH2F()->SetOption("lego2z,prof");
        genpart_GammaF->getTH2F()->SetTitleOffset(1.4,"x");
        genpart_GammaF->getTH2F()->SetTitleOffset(1.4,"y");
        
        genpart_GammaF_counts = ib.book2D("Gamma_Forward_Counts","Number of Forward Generated Gammas",200,7.5,13,100,-3.15,3.15);
        genpart_GammaF_counts->setAxisTitle("#eta",1);
        genpart_GammaF_counts->setAxisTitle("#phi (radians)",2);
        genpart_GammaF_counts->setAxisTitle("Energy (GeV)",3);
        genpart_GammaF_counts->getTH2F()->SetOption("lego2z,prof");
        genpart_GammaF_counts->getTH2F()->SetTitleOffset(1.4,"x");
        genpart_GammaF_counts->getTH2F()->SetTitleOffset(1.4,"y");
        
        ////////////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////////
        genpart_Pi0F_energydist = ib.book1D("Pi0_Forward_EDistribution","Gen-Level Forward Pi0 Energy",1500,0,1500);
        genpart_Pi0F_energydist->setAxisTitle("Energy (GeV)",1);
        genpart_Pi0F_energydist->setAxisTitle("Counts",2);
        ////////////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////////
        genpart_NeutF_energydist = ib.book1D("N_Forward_EDistribution","Gen-Level Forward Neutron Energy",1500,0,1500);
        genpart_NeutF_energydist->setAxisTitle("Energy (GeV)",1);
        genpart_NeutF_energydist->setAxisTitle("Counts",2);
        ///////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////
        genpart_GammaF_energydist = ib.book1D("Gamma_Forward_EDistribution","Gen-Level Fowarad Gamma Energy",1500,0,1500);
        genpart_GammaF_energydist->setAxisTitle("Energy (GeV)",1);
        genpart_GammaF_energydist->setAxisTitle("Counts",2);
        /////////////////////////////////////////////////////////////////////////////////
        ib.setCurrentFolder("ZDCValidation/GenParticles/Backward");
        ////////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////72/////////////////////////////////////////////
        genpart_Pi0B = ib.book2D("Pi0_Backward","Backward Generated Pi0s",1000,-13,-7.5,100,-3.15,3.15);
        genpart_Pi0B->setAxisTitle("Eta",1);
        genpart_Pi0B->setAxisTitle("Phi (radians)",2);
        genpart_Pi0B->setAxisTitle("Energy (GeV)",3);
        genpart_Pi0B->getTH2F()->SetOption("lego2z,prof");
        genpart_Pi0B->getTH2F()->SetTitleOffset(1.4,"x");
        genpart_Pi0B->getTH2F()->SetTitleOffset(1.4,"y");
        
        genpart_Pi0B_counts = ib.book2D("Pi0_Backwards_Counts","Number of Backward Generated Pi0s",200,-13,-7.5,100,-3.15,3.15);
        genpart_Pi0B_counts->setAxisTitle("#eta",1);
        genpart_Pi0B_counts->setAxisTitle("#phi (radians)",2);
        genpart_Pi0B_counts->setAxisTitle("Energy (GeV)",3);
        genpart_Pi0B_counts->getTH2F()->SetOption("lego2z,prof");
        genpart_Pi0B_counts->getTH2F()->SetTitleOffset(1.4,"x");
        genpart_Pi0B_counts->getTH2F()->SetTitleOffset(1.4,"y");
        
        /////////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////73/////////////////////////////////////////////
        genpart_NeutB = ib.book2D("Neutron_Backward","Backward Generated Neutrons",1000,-13,-7.5,100,-3.15,3.15);
        genpart_NeutB->setAxisTitle("Eta",1);
        genpart_NeutB->setAxisTitle("Phi (radians)",2);
        genpart_NeutB->setAxisTitle("Energy (GeV)",3);
        genpart_NeutB->getTH2F()->SetOption("lego2z,prof");
        genpart_NeutB->getTH2F()->SetTitleOffset(1.4,"x");
        genpart_NeutB->getTH2F()->SetTitleOffset(1.4,"y");
        
        genpart_NeutB_counts = ib.book2D("Neutron_Backwards_Counts","Number of Backward Generated Neutrons",200,-13,-7.5,100,-3.15,3.15);
        genpart_NeutB_counts->setAxisTitle("#eta",1);
        genpart_NeutB_counts->setAxisTitle("#phi (radians)",2);
        genpart_NeutB_counts->setAxisTitle("Energy (GeV)",3);
        genpart_NeutB_counts->getTH2F()->SetOption("lego2z,prof");
        genpart_NeutB_counts->getTH2F()->SetTitleOffset(1.4,"x");
        genpart_NeutB_counts->getTH2F()->SetTitleOffset(1.4,"y");
        
        
        ///////////////////////////////74/////////////////////////////////////////////
        genpart_GammaB = ib.book2D("Gamma_Backward","Backward Generated Gammas",1000,-13,-7.5,100,-3.15,3.15);
        genpart_GammaB->setAxisTitle("Eta",1);
        genpart_GammaB->setAxisTitle("Phi (radians)",2);
        genpart_GammaB->setAxisTitle("Energy (GeV)",3);
        genpart_GammaB->getTH2F()->SetOption("lego2z,prof");
        genpart_GammaB->getTH2F()->SetTitleOffset(1.4,"x");
        genpart_GammaB->getTH2F()->SetTitleOffset(1.4,"y");
        
        genpart_GammaB_counts = ib.book2D("Gamma_Backwards_Counts","Number of Backward Generated Gammas",200,-13,-7.5,100,-3.15,3.15);
        genpart_GammaB_counts->setAxisTitle("#eta",1);
        genpart_GammaB_counts->setAxisTitle("#phi (radians)",2);
        genpart_GammaB_counts->setAxisTitle("Energy (GeV)",3);
        genpart_GammaB_counts->getTH2F()->SetOption("lego2z,prof");
        genpart_GammaB_counts->getTH2F()->SetTitleOffset(1.4,"x");
        genpart_GammaB_counts->getTH2F()->SetTitleOffset(1.4,"y");
        
        
        /////////////////////////////////////////
        //# New plots for the RPD flow detector #
        /////////////////////////////////////////
        
        /////////////////////////75///////////////////////////
        ib.setCurrentFolder("ZDCValidation/ZdcSimHits/Excess_Info/BasicHitInfo");
        meZdcNHitRpd_   = ib.book1D("Hits in RPD","Number of Hits in ZDC Rpd",100,0.,100.);
        meZdcNHitRpd_->setAxisTitle("RPD Hits",1);
        meZdcNHitRpd_->setAxisTitle("Counts",2);
        /////////////////////////76///////////////////////////
        ib.setCurrentFolder("ZDCValidation/ZdcSimHits/");
        meZdcRpdEnergyHit_= ib.book1D("Hit Energy RPD","Hits Energy in Rpd Section",4000,0.,8000.);
        meZdcRpdEnergyHit_->setAxisTitle("Counts",2);
        meZdcRpdEnergyHit_->setAxisTitle("Energy (GeV)",1);
        /////////////////////////77///////////////////////////
        ib.setCurrentFolder("ZDCValidation/ZdcSimHits/Excess_Info/BasicHitInfo");
        meZdcRpdL10EneP_ = ib.bookProfile("Log(ERPD) vs Contribution","Log10Energy in Rpd ZDC vs Hit contribution", 140, -1., 20., 100, 0., 1. );
        meZdcRpdL10EneP_->setAxisTitle("Log(ERPD) (GeV)",1);
        meZdcRpdL10EneP_->setAxisTitle("Counts",2);
        /////////////////////////78///////////////////////////
        ib.setCurrentFolder("ZDCValidation/ZdcSimHits");
        meZdcERpdCh_ = ib.book2D("ZDC ERPD vs Channel","ZDC Rpd Section Energy vs Channel", 4000, 0., 8000., 17, 0., 17. );
        meZdcERpdCh_->setAxisTitle("RPD Channel Number",2);
        meZdcERpdCh_->setAxisTitle("Energy (GeV)",1);
        /////////////////////////79///////////////////////////
        ib.setCurrentFolder("ZDCValidation/ZdcSimHits/ENERGY_SUMS/Individual_Channels/NZDC");
        meZdcEneRpdN1_ = ib.book1D("NZDC RPD01 Energy","Energy RPD module N1",4000,0.,8000.);
        meZdcEneRpdN1_->setAxisTitle("Energy (GeV)",1);
        meZdcEneRpdN1_->setAxisTitle("Counts",2);
        /////////////////////////80///////////////////////////
        meZdcEneRpdN2_ = ib.book1D("NZDC RPD02 Energy","Energy RPD module N2",4000,0.,8000.);
        meZdcEneRpdN2_->setAxisTitle("Energy (GeV)",1);
        meZdcEneRpdN2_->setAxisTitle("Counts",2);
        /////////////////////////81///////////////////////////
        meZdcEneRpdN3_ = ib.book1D("NZDC RPD03 Energy","Energy RPD module N3",4000,0.,8000.);
        meZdcEneRpdN3_->setAxisTitle("Energy (GeV)",1);
        meZdcEneRpdN3_->setAxisTitle("Counts",2);
        /////////////////////////82///////////////////////////
        meZdcEneRpdN4_ = ib.book1D("NZDC RPD04 Energy","Energy RPD module N4",4000,0.,8000.);
        meZdcEneRpdN4_->setAxisTitle("Energy (GeV)",1);
        meZdcEneRpdN4_->setAxisTitle("Counts",2);
        /////////////////////////83///////////////////////////
        meZdcEneRpdN5_ = ib.book1D("NZDC RPD05 Energy","Energy RPD module N5",4000,0.,8000.);
        meZdcEneRpdN5_->setAxisTitle("Energy (GeV)",1);
        meZdcEneRpdN5_->setAxisTitle("Counts",2);
        /////////////////////////84///////////////////////////
        meZdcEneRpdN6_ = ib.book1D("NZDC RPD06 Energy","Energy RPD module N6",4000,0.,8000.);
        meZdcEneRpdN6_->setAxisTitle("Energy (GeV)",1);
        meZdcEneRpdN6_->setAxisTitle("Counts",2);
        /////////////////////////85///////////////////////////
        meZdcEneRpdN7_ = ib.book1D("NZDC RPD07 Energy","Energy RPD module N7",4000,0.,8000.);
        meZdcEneRpdN7_->setAxisTitle("Energy (GeV)",1);
        meZdcEneRpdN7_->setAxisTitle("Counts",2);
        /////////////////////////86///////////////////////////
        meZdcEneRpdN8_ = ib.book1D("NZDC RPD08 Energy","Energy RPD module N8",4000,0.,8000.);
        meZdcEneRpdN8_->setAxisTitle("Energy (GeV)",1);
        meZdcEneRpdN8_->setAxisTitle("Counts",2);
        /////////////////////////87///////////////////////////
        meZdcEneRpdN9_ = ib.book1D("NZDC RPD09 Energy","Energy RPD module N9",4000,0.,8000.);
        meZdcEneRpdN9_->setAxisTitle("Energy (GeV)",1);
        meZdcEneRpdN9_->setAxisTitle("Counts",2);
        /////////////////////////88///////////////////////////
        meZdcEneRpdN10_ = ib.book1D("NZDC RPD10 Energy","Energy RPD module N10",4000,0.,8000.);
        meZdcEneRpdN10_->setAxisTitle("Energy (GeV)",1);
        meZdcEneRpdN10_->setAxisTitle("Counts",2);
        /////////////////////////89///////////////////////////
        meZdcEneRpdN11_ = ib.book1D("NZDC RPD11 Energy","Energy RPD module N11",4000,0.,8000.);
        meZdcEneRpdN11_->setAxisTitle("Energy (GeV)",1);
        meZdcEneRpdN11_->setAxisTitle("Counts",2);
        /////////////////////////90///////////////////////////
        meZdcEneRpdN12_ = ib.book1D("NZDC RPD12 Energy","Energy RPD module N12",4000,0.,8000.);
        meZdcEneRpdN12_->setAxisTitle("Energy (GeV)",1);
        meZdcEneRpdN12_->setAxisTitle("Counts",2);
        /////////////////////////91///////////////////////////
        meZdcEneRpdN13_ = ib.book1D("NZDC RPD13 Energy","Energy RPD module N13",4000,0.,8000.);
        meZdcEneRpdN13_->setAxisTitle("Energy (GeV)",1);
        meZdcEneRpdN13_->setAxisTitle("Counts",2);
        /////////////////////////92///////////////////////////
        meZdcEneRpdN14_ = ib.book1D("NZDC RPD14 Energy","Energy RPD module N14",4000,0.,8000.);
        meZdcEneRpdN14_->setAxisTitle("Energy (GeV)",1);
        meZdcEneRpdN14_->setAxisTitle("Counts",2);
        /////////////////////////93///////////////////////////
        meZdcEneRpdN15_ = ib.book1D("NZDC RPD15 Energy","Energy RPD module N15",4000,0.,8000.);
        meZdcEneRpdN15_->setAxisTitle("Energy (GeV)",1);
        meZdcEneRpdN15_->setAxisTitle("Counts",2);
        /////////////////////////94///////////////////////////
        meZdcEneRpdN16_ = ib.book1D("NZDC RPD16 Energy","Energy RPD module N16",4000,0.,8000.);
        meZdcEneRpdN16_->setAxisTitle("Energy (GeV)",1);
        meZdcEneRpdN16_->setAxisTitle("Counts",2);
        /////////////////////////95///////////////////////////
        ib.setCurrentFolder("ZDCValidation/ZdcSimHits/Excess_Info/Individual_ChannelvsTime/NZDC");
        meZdcEneTRpdN1_ = ib.book2D("NZDC RPD01 Energy vs Time","Energy RPD mod N1 vs Time", 4000, 0., 8000., 300, 0., 600. );
        meZdcEneTRpdN1_->setAxisTitle("Energy (GeV)",1);
        meZdcEneTRpdN1_->setAxisTitle("Time (ns)",2);
        /////////////////////////96///////////////////////////
        meZdcEneTRpdN2_ = ib.book2D("NZDC RPD02 Energy vs Time","Energy RPD mod N2 vs Time", 4000, 0., 8000., 300, 0., 600. );
        meZdcEneTRpdN2_->setAxisTitle("Energy (GeV)",1);
        meZdcEneTRpdN2_->setAxisTitle("Time (ns)",2);
        /////////////////////////97///////////////////////////
        meZdcEneTRpdN3_ = ib.book2D("NZDC RPD03 Energy vs Time","Energy RPD mod N3 vs Time", 4000, 0., 8000., 300, 0., 600. );
        meZdcEneTRpdN3_->setAxisTitle("Energy (GeV)",1);
        meZdcEneTRpdN3_->setAxisTitle("Time (ns)",2);
        /////////////////////////98///////////////////////////
        meZdcEneTRpdN4_ = ib.book2D("NZDC RPD04 Energy vs Time","Energy RPD mod N4 vs Time", 4000, 0., 8000., 300, 0., 600. );
        meZdcEneTRpdN4_->setAxisTitle("Energy (GeV)",1);
        meZdcEneTRpdN4_->setAxisTitle("Time (ns)",2);
        /////////////////////////99///////////////////////////
        meZdcEneTRpdN5_ = ib.book2D("NZDC RPD05 Energy vs Time","Energy RPD mod N5 vs Time", 4000, 0., 8000., 300, 0., 600. );
        meZdcEneTRpdN5_->setAxisTitle("Energy (GeV)",1);
        meZdcEneTRpdN5_->setAxisTitle("Time (ns)",2);
        /////////////////////////100//////////////////////////
        meZdcEneTRpdN6_ = ib.book2D("NZDC RPD06 Energy vs Time","Energy RPD mod N6 vs Time", 4000, 0., 8000., 300, 0., 600. );
        meZdcEneTRpdN6_->setAxisTitle("Energy (GeV)",1);
        meZdcEneTRpdN6_->setAxisTitle("Time (ns)",2);
        /////////////////////////101//////////////////////////
        meZdcEneTRpdN7_ = ib.book2D("NZDC RPD07 Energy vs Time","Energy RPD mod N7 vs Time", 4000, 0., 8000., 300, 0., 600. );
        meZdcEneTRpdN7_->setAxisTitle("Energy (GeV)",1);
        meZdcEneTRpdN7_->setAxisTitle("Time (ns)",2);
        /////////////////////////102//////////////////////////
        meZdcEneTRpdN8_ = ib.book2D("NZDC RPD08 Energy vs Time","Energy RPD mod N8 vs Time", 4000, 0., 8000., 300, 0., 600. );
        meZdcEneTRpdN8_->setAxisTitle("Energy (GeV)",1);
        meZdcEneTRpdN8_->setAxisTitle("Time (ns)",2);
        /////////////////////////103//////////////////////////
        meZdcEneTRpdN9_ = ib.book2D("NZDC RPD09 Energy vs Time","Energy RPD mod N9 vs Time", 4000, 0., 8000., 300, 0., 600. );
        meZdcEneTRpdN9_->setAxisTitle("Energy (GeV)",1);
        meZdcEneTRpdN9_->setAxisTitle("Time (ns)",2);
        /////////////////////////104//////////////////////////
        meZdcEneTRpdN10_ = ib.book2D("NZDC RPD10 Energy vs Time","Energy RPD mod N10 vs Time", 4000, 0., 8000., 300, 0., 600. );
        meZdcEneTRpdN10_->setAxisTitle("Energy (GeV)",1);
        meZdcEneTRpdN10_->setAxisTitle("Time (ns)",2);
        /////////////////////////105//////////////////////////
        meZdcEneTRpdN11_ = ib.book2D("NZDC RPD11 Energy vs Time","Energy RPD mod N11 vs Time", 4000, 0., 8000., 300, 0., 600. );
        meZdcEneTRpdN11_->setAxisTitle("Energy (GeV)",1);
        meZdcEneTRpdN11_->setAxisTitle("Time (ns)",2);
        /////////////////////////106//////////////////////////
        meZdcEneTRpdN12_ = ib.book2D("NZDC RPD12 Energy vs Time","Energy RPD mod N12 vs Time", 4000, 0., 8000., 300, 0., 600. );
        meZdcEneTRpdN12_->setAxisTitle("Energy (GeV)",1);
        meZdcEneTRpdN12_->setAxisTitle("Time (ns)",2);
        /////////////////////////107//////////////////////////
        meZdcEneTRpdN13_ = ib.book2D("NZDC RPD13 Energy vs Time","Energy RPD mod N13 vs Time", 4000, 0., 8000., 300, 0., 600. );
        meZdcEneTRpdN13_->setAxisTitle("Energy (GeV)",1);
        meZdcEneTRpdN13_->setAxisTitle("Time (ns)",2);
        /////////////////////////108//////////////////////////
        meZdcEneTRpdN14_ = ib.book2D("NZDC RPD14 Energy vs Time","Energy RPD mod N14 vs Time", 4000, 0., 8000., 300, 0., 600. );
        meZdcEneTRpdN14_->setAxisTitle("Energy (GeV)",1);
        meZdcEneTRpdN14_->setAxisTitle("Time (ns)",2);
        /////////////////////////109//////////////////////////
        meZdcEneTRpdN15_ = ib.book2D("NZDC RPD15 Energy vs Time","Energy RPD mod N15 vs Time", 4000, 0., 8000., 300, 0., 600. );
        meZdcEneTRpdN15_->setAxisTitle("Energy (GeV)",1);
        meZdcEneTRpdN15_->setAxisTitle("Time (ns)",2);
        /////////////////////////110//////////////////////////
        meZdcEneTRpdN16_ = ib.book2D("NZDC RPD16 Energy vs Time","Energy RPD mod N16 vs Time", 4000, 0., 8000., 300, 0., 600. );
        meZdcEneTRpdN16_->setAxisTitle("Energy (GeV)",1);
        meZdcEneTRpdN16_->setAxisTitle("Time (ns)",2);
        /////////////////////////111///////////////////////////
        ib.setCurrentFolder("ZDCValidation/ZdcSimHits/ENERGY_SUMS/NZDC");
        meZdcEneRpdNTot_ = ib.book1D("NZDC ERPD","Total N-ZDC RPD Energy",4000,0.,4000.);
        meZdcEneRpdNTot_->setAxisTitle("Counts",2);
        meZdcEneRpdNTot_->setAxisTitle("Energy (GeV)",1);
        
        
        /////////////////////////112//////////////////////////
        ib.setCurrentFolder("ZDCValidation/ZdcSimHits/ENERGY_SUMS/Individual_Channels/PZDC");
        meZdcEneRpdP1_ = ib.book1D("PZDC RPD01 Energy","Energy RPD module N1",4000,0.,8000.);
        meZdcEneRpdP1_->setAxisTitle("Energy (GeV)",1);
        meZdcEneRpdP1_->setAxisTitle("Counts",2);
        /////////////////////////113//////////////////////////
        meZdcEneRpdP2_ = ib.book1D("PZDC RPD02 Energy","Energy RPD module N2",4000,0.,8000.);
        meZdcEneRpdP2_->setAxisTitle("Energy (GeV)",1);
        meZdcEneRpdP2_->setAxisTitle("Counts",2);
        /////////////////////////114//////////////////////////
        meZdcEneRpdP3_ = ib.book1D("PZDC RPD03 Energy","Energy RPD module N3",4000,0.,8000.);
        meZdcEneRpdP3_->setAxisTitle("Energy (GeV)",1);
        meZdcEneRpdP3_->setAxisTitle("Counts",2);
        /////////////////////////115//////////////////////////
        meZdcEneRpdP4_ = ib.book1D("PZDC RPD04 Energy","Energy RPD module N4",4000,0.,8000.);
        meZdcEneRpdP4_->setAxisTitle("Energy (GeV)",1);
        meZdcEneRpdP4_->setAxisTitle("Counts",2);
        /////////////////////////116//////////////////////////
        meZdcEneRpdP5_ = ib.book1D("PZDC RPD05 Energy","Energy RPD module N5",4000,0.,8000.);
        meZdcEneRpdP5_->setAxisTitle("Energy (GeV)",1);
        meZdcEneRpdP5_->setAxisTitle("Counts",2);
        /////////////////////////117//////////////////////////
        meZdcEneRpdP6_ = ib.book1D("PZDC RPD06 Energy","Energy RPD module N6",4000,0.,8000.);
        meZdcEneRpdP6_->setAxisTitle("Energy (GeV)",1);
        meZdcEneRpdP6_->setAxisTitle("Counts",2);
        /////////////////////////118//////////////////////////
        meZdcEneRpdP7_ = ib.book1D("PZDC RPD07 Energy","Energy RPD module N7",4000,0.,8000.);
        meZdcEneRpdP7_->setAxisTitle("Energy (GeV)",1);
        meZdcEneRpdP7_->setAxisTitle("Counts",2);
        /////////////////////////119//////////////////////////
        meZdcEneRpdP8_ = ib.book1D("PZDC RPD08 Energy","Energy RPD module N8",4000,0.,8000.);
        meZdcEneRpdP8_->setAxisTitle("Energy (GeV)",1);
        meZdcEneRpdP8_->setAxisTitle("Counts",2);
        /////////////////////////120//////////////////////////
        meZdcEneRpdP9_ = ib.book1D("PZDC RPD09 Energy","Energy RPD module N9",4000,0.,8000.);
        meZdcEneRpdP9_->setAxisTitle("Energy (GeV)",1);
        meZdcEneRpdP9_->setAxisTitle("Counts",2);
        /////////////////////////121//////////////////////////
        meZdcEneRpdP10_ = ib.book1D("PZDC RPD10 Energy","Energy RPD module N10",4000,0.,8000.);
        meZdcEneRpdP10_->setAxisTitle("Energy (GeV)",1);
        meZdcEneRpdP10_->setAxisTitle("Counts",2);
        /////////////////////////122//////////////////////////
        meZdcEneRpdP11_ = ib.book1D("PZDC RPD11 Energy","Energy RPD module N11",4000,0.,8000.);
        meZdcEneRpdP11_->setAxisTitle("Energy (GeV)",1);
        meZdcEneRpdP11_->setAxisTitle("Counts",2);
        /////////////////////////123//////////////////////////
        meZdcEneRpdP12_ = ib.book1D("PZDC RPD12 Energy","Energy RPD module N12",4000,0.,8000.);
        meZdcEneRpdP12_->setAxisTitle("Energy (GeV)",1);
        meZdcEneRpdP12_->setAxisTitle("Counts",2);
        /////////////////////////124//////////////////////////
        meZdcEneRpdP13_ = ib.book1D("PZDC RPD13 Energy","Energy RPD module N13",4000,0.,8000.);
        meZdcEneRpdP13_->setAxisTitle("Energy (GeV)",1);
        meZdcEneRpdP13_->setAxisTitle("Counts",2);
        /////////////////////////125//////////////////////////
        meZdcEneRpdP14_ = ib.book1D("PZDC RPD14 Energy","Energy RPD module N14",4000,0.,8000.);
        meZdcEneRpdP14_->setAxisTitle("Energy (GeV)",1);
        meZdcEneRpdP14_->setAxisTitle("Counts",2);
        /////////////////////////126//////////////////////////
        meZdcEneRpdP15_ = ib.book1D("PZDC RPD15 Energy","Energy RPD module N15",4000,0.,8000.);
        meZdcEneRpdP15_->setAxisTitle("Energy (GeV)",1);
        meZdcEneRpdP15_->setAxisTitle("Counts",2);
        /////////////////////////127//////////////////////////
        meZdcEneRpdP16_ = ib.book1D("PZDC RPD16 Energy","Energy RPD module N16",4000,0.,8000.);
        meZdcEneRpdP16_->setAxisTitle("Energy (GeV)",1);
        meZdcEneRpdP16_->setAxisTitle("Counts",2);
        /////////////////////////128//////////////////////////
        ib.setCurrentFolder("ZDCValidation/ZdcSimHits/Excess_Info/Individual_ChannelvsTime/PZDC");
        meZdcEneTRpdP1_ = ib.book2D("PZDC RPD01 Energy vs Time","Energy RPD mod P1 vs Time", 4000, 0., 8000., 300, 0., 600. );
        meZdcEneTRpdP1_->setAxisTitle("Energy (GeV)",1);
        meZdcEneTRpdP1_->setAxisTitle("Time (ns)",2);
        /////////////////////////129//////////////////////////
        meZdcEneTRpdP2_ = ib.book2D("PZDC RPD02 Energy vs Time","Energy RPD mod P2 vs Time", 4000, 0., 8000., 300, 0., 600. );
        meZdcEneTRpdP2_->setAxisTitle("Energy (GeV)",1);
        meZdcEneTRpdP2_->setAxisTitle("Time (ns)",2);
        /////////////////////////130//////////////////////////
        meZdcEneTRpdP3_ = ib.book2D("PZDC RPD03 Energy vs Time","Energy RPD mod P3 vs Time", 4000, 0., 8000., 300, 0., 600. );
        meZdcEneTRpdP3_->setAxisTitle("Energy (GeV)",1);
        meZdcEneTRpdP3_->setAxisTitle("Time (ns)",2);
        /////////////////////////131//////////////////////////
        meZdcEneTRpdP4_ = ib.book2D("PZDC RPD04 Energy vs Time","Energy RPD mod P4 vs Time", 4000, 0., 8000., 300, 0., 600. );
        meZdcEneTRpdP4_->setAxisTitle("Energy (GeV)",1);
        meZdcEneTRpdP4_->setAxisTitle("Time (ns)",2);
        /////////////////////////132//////////////////////////
        meZdcEneTRpdP5_ = ib.book2D("PZDC RPD05 Energy vs Time","Energy RPD mod P5 vs Time", 4000, 0., 8000., 300, 0., 600. );
        meZdcEneTRpdP5_->setAxisTitle("Energy (GeV)",1);
        meZdcEneTRpdP5_->setAxisTitle("Time (ns)",2);
        /////////////////////////133//////////////////////////
        meZdcEneTRpdP6_ = ib.book2D("PZDC RPD06 Energy vs Time","Energy RPD mod P6 vs Time", 4000, 0., 8000., 300, 0., 600. );
        meZdcEneTRpdP6_->setAxisTitle("Energy (GeV)",1);
        meZdcEneTRpdP6_->setAxisTitle("Time (ns)",2);
        /////////////////////////134//////////////////////////
        meZdcEneTRpdP7_ = ib.book2D("PZDC RPD07 Energy vs Time","Energy RPD mod P7 vs Time", 4000, 0., 8000., 300, 0., 600. );
        meZdcEneTRpdP7_->setAxisTitle("Energy (GeV)",1);
        meZdcEneTRpdP7_->setAxisTitle("Time (ns)",2);
        /////////////////////////135//////////////////////////
        meZdcEneTRpdP8_ = ib.book2D("PZDC RPD08 Energy vs Time","Energy RPD mod P8 vs Time", 4000, 0., 8000., 300, 0., 600. );
        meZdcEneTRpdP8_->setAxisTitle("Energy (GeV)",1);
        meZdcEneTRpdP8_->setAxisTitle("Time (ns)",2);
        /////////////////////////136//////////////////////////
        meZdcEneTRpdP9_ = ib.book2D("PZDC RPD09 Energy vs Time","Energy RPD mod P9 vs Time", 4000, 0., 8000., 300, 0., 600. );
        meZdcEneTRpdP9_->setAxisTitle("Energy (GeV)",1);
        meZdcEneTRpdP9_->setAxisTitle("Time (ns)",2);
        /////////////////////////137//////////////////////////
        meZdcEneTRpdP10_ = ib.book2D("PZDC RPD10 Energy vs Time","Energy RPD mod P10 vs Time", 4000, 0., 8000., 300, 0., 600. );
        meZdcEneTRpdP10_->setAxisTitle("Energy (GeV)",1);
        meZdcEneTRpdP10_->setAxisTitle("Time (ns)",2);
        /////////////////////////138//////////////////////////
        meZdcEneTRpdP11_ = ib.book2D("PZDC RPD11 Energy vs Time","Energy RPD mod P11 vs Time", 4000, 0., 8000., 300, 0., 600. );
        meZdcEneTRpdP11_->setAxisTitle("Energy (GeV)",1);
        meZdcEneTRpdP11_->setAxisTitle("Time (ns)",2);
        /////////////////////////139//////////////////////////
        meZdcEneTRpdP12_ = ib.book2D("PZDC RPD12 Energy vs Time","Energy RPD mod P12 vs Time", 4000, 0., 8000., 300, 0., 600. );
        meZdcEneTRpdP12_->setAxisTitle("Energy (GeV)",1);
        meZdcEneTRpdP12_->setAxisTitle("Time (ns)",2);
        /////////////////////////140//////////////////////////
        meZdcEneTRpdP13_ = ib.book2D("PZDC RPD13 Energy vs Time","Energy RPD mod P13 vs Time", 4000, 0., 8000., 300, 0., 600. );
        meZdcEneTRpdP13_->setAxisTitle("Energy (GeV)",1);
        meZdcEneTRpdP13_->setAxisTitle("Time (ns)",2);
        /////////////////////////141//////////////////////////
        meZdcEneTRpdP14_ = ib.book2D("PZDC RPD14 Energy vs Time","Energy RPD mod P14 vs Time", 4000, 0., 8000., 300, 0., 600. );
        meZdcEneTRpdP14_->setAxisTitle("Energy (GeV)",1);
        meZdcEneTRpdP14_->setAxisTitle("Time (ns)",2);
        /////////////////////////142//////////////////////////
        meZdcEneTRpdP15_ = ib.book2D("PZDC RPD15 Energy vs Time","Energy RPD mod P15 vs Time", 4000, 0., 8000., 300, 0., 600. );
        meZdcEneTRpdP15_->setAxisTitle("Energy (GeV)",1);
        meZdcEneTRpdP15_->setAxisTitle("Time (ns)",2);
        /////////////////////////143//////////////////////////
        meZdcEneTRpdP16_ = ib.book2D("PZDC RPD16 Energy vs Time","Energy RPD mod P16 vs Time", 4000, 0., 8000., 300, 0., 600. );
        meZdcEneTRpdP16_->setAxisTitle("Energy (GeV)",1);
        meZdcEneTRpdP16_->setAxisTitle("Time (ns)",2);
        /////////////////////////144///////////////////////////
        ib.setCurrentFolder("ZDCValidation/ZdcSimHits/ENERGY_SUMS/NZDC");
        meZdcEneRpdPTot_ = ib.book1D("PZDC ERPD","Total P-ZDC RPD Energy",4000,0.,4000.);
        meZdcEneRpdPTot_->setAxisTitle("Counts",2);
        meZdcEneRpdPTot_->setAxisTitle("Energy (GeV)",1);

        
        
        /////////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////GEN Particle Energy Distributions/////////////////////
        ///////////////////////////////////////////////////////////////////////////////////////////////
        genpart_Pi0B_energydist = ib.book1D("Pi0_Backward_EDistribution","Gen-Level Backward Pi0 Energy",1500,0,1500);
        genpart_Pi0B_energydist->setAxisTitle("Energy (GeV)",1);
        genpart_Pi0B_energydist->setAxisTitle("Counts",2);
        //////////////////////////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////////////////////////////////////
        genpart_NeutB_energydist = ib.book1D("N_Backward_EDistribution","Gen-Level Foward Neutron Energy",1500,0,1500);
        genpart_NeutB_energydist->setAxisTitle("Energy (GeV)",1);
        genpart_NeutB_energydist->setAxisTitle("Counts",2);
        ///////////////////////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////////////////////
        genpart_GammaB_energydist = ib.book1D("Gamma_Backward_EDistribution","Gen-Level Backward Gamma Energy",1500,0,1500);
        genpart_GammaB_energydist->setAxisTitle("Energy (GeV)",1);
        genpart_GammaB_energydist->setAxisTitle("Counts",2);
        /////////////////////////////////////////////////////////////////////////////////////////
        
    }
}

// let's see if this breaks anything
/*void ZdcSimHitStudy::endJob() {
 if (dbe_ && outFile_.size() > 0) dbe_->save(outFile_);
 }*/

//void ZdcSimHitStudy::analyze(const edm::Event& e, const edm::EventSetup& ) {
void ZdcSimHitStudy::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup ) {
    //////////NEW STUFF//////////////////////
    
    using namespace edm;
    bool gotGenParticles=true;
    
    
    Handle<reco::GenParticleCollection> genhandle;
    
    if (!(iEvent.getByToken(tok_gen_,genhandle)))
    {
        gotGenParticles=false; //this is the same kind of boolean except for the genparticles collection
    }
    if (!(genhandle.isValid()))
    {
        gotGenParticles=false;
    }
    
    
    //Handle<edm::PCaloHitContainer> zdcsimhandle;
    
    
    ////////////////////////////////////GEN PARTICLE HISTOS///////////////////////////////////
    
    
    if (gotGenParticles==true){ //if the boolean was able to find the leaf "genparticles" then do this
        for (reco::GenParticleCollection::const_iterator gen = genhandle->begin();
             gen!=genhandle->end();
             ++gen) //here we iterate over all generated particles
        {
            //         double energy=gen->energy();
            reco::GenParticle thisParticle = (reco::GenParticle)(*gen); //get the particle "gen" points to
            double energy_2= thisParticle.energy(); //here I grab some of the attributes of the generated particle....like its energy, its phi and its eta and what kind of particle it is
            double gen_phi = thisParticle.phi();
            double gen_eta = thisParticle.eta();
            int gen_id = thisParticle.pdgId();
            
            if (gen_id==111){ //here i require a pi0
                if (gen_eta>8.3){ //eta requirement
                    
/////////////////////////////////////////////////////////////////////////////////////
//# IMPORTANT     IMPORTANT         IMPORTANT            IMPORTANT                  #
//# The real eta of the ZDC is |eta| > 8.3, I have only changed it here to 3 because#
//# in the PG simulation the ZDC is at an eta of about 4.5-7, in the real GEANT the #
//# ZDC is in its appropriate place at the very foward region...please edit this if #
//# looking at MinBias data or the like                                             #
//#                                                                                 #
//# IMPORTANT     IMPORTANT      IMPORTANT       IMPORTANT                          #
/////////////////////////////////////////////////////////////////////////////////////
                    
                    
                    genpart_Pi0F->Fill(gen_eta,gen_phi,energy_2); //fill the lego plot
                    genpart_Pi0F_counts->Fill(gen_eta,gen_phi,1);
                    //genpart_Pi0F_profile->Fill(gen_eta,gen_phi,energy_2);
                    genpart_Pi0F_energydist->Fill(energy_2); //fill the 1D distribution
                    
                }
                if (gen_eta<-8.3){ //neg eta requirement
                    genpart_Pi0B->Fill(gen_eta,gen_phi,energy_2);
                    // genpart_Pi0B_profile->Fill(gen_eta,gen_phi,energy_2);
                    genpart_Pi0B_counts->Fill(gen_eta,gen_phi,1);
                    genpart_Pi0B_energydist->Fill(energy_2);
                }
            }
            if (gen_id==2112){ //require neutron
                if (gen_eta>8.3){
                    genpart_NeutF->Fill(gen_eta,gen_phi,energy_2);
                    //genpart_NeutF_profile->Fill(gen_eta,gen_phi,energy_2);
                    genpart_NeutF_counts->Fill(gen_eta,gen_phi,1);
                    genpart_NeutF_energydist->Fill(energy_2);
                }
                if (gen_eta<-8.3){
                    genpart_NeutB->Fill(gen_eta,gen_phi,energy_2);
                    //genpart_NeutB_profile->Fill(gen_eta,gen_phi,energy_2);
                    genpart_NeutB_counts->Fill(gen_eta,gen_phi,1);
                    genpart_NeutB_energydist->Fill(energy_2);
                }
            }
            
            if (gen_id==22){ //require gamma
                if (gen_eta>8.3){
                    genpart_GammaF->Fill(gen_eta,gen_phi,energy_2);
                    // genpart_GammaF_profile->Fill(gen_eta,gen_phi,energy_2);
                    genpart_GammaF_counts->Fill(gen_eta,gen_phi,1);
                    genpart_GammaF_energydist->Fill(energy_2);
                }
                if (gen_eta<-8.3){
                    genpart_GammaB->Fill(gen_eta,gen_phi,energy_2);
                    //genpart_GammaB_profile->Fill(gen_eta,gen_phi,energy_2);
                    genpart_GammaB_counts->Fill(gen_eta,gen_phi,1);
                    genpart_GammaB_energydist->Fill(energy_2);
                }
            }
            
        } //end of GEN loop
    }
    
    
    
    /////////////////////////////////////////////////////////////////////
    
    //Below is the old script which I will comment later
    
    
    
    LogDebug("ZdcSimHitStudy")
    //std::cout
    //std::cout
    << "Run = " << iEvent.id().run() << " Event = "
    << iEvent.id().event();
    /*    << "Run = " << e.id().run() << " Event = "
     << e.id().event();*/
    //std::cout<<std::endl;
    
    std::vector<PCaloHit>               caloHits;
    edm::Handle<edm::PCaloHitContainer> hitsZdc;
    
    bool getHits = false;
    if (checkHit_) {
        iEvent.getByToken(tok_hits_,hitsZdc);
        if (hitsZdc.isValid()) getHits = true;
    }
    
    LogDebug("ZdcSim") << "ZdcValidation: Input flags Hits " << getHits;
    
    if (getHits) {
        caloHits.insert(caloHits.end(),hitsZdc->begin(),hitsZdc->end());
        LogDebug("ZdcSimHitStudy")
        //std::cout
        << "ZdcValidation: Hit buffer "
        << caloHits.size();
        //<< std::endl;
        analyzeHits (caloHits);
    }
}

void ZdcSimHitStudy::analyzeHits(std::vector<PCaloHit>& hits){
    int nHit = hits.size();
    int nZdcEM = 0, nZdcHad = 0, nZdcLum = 0, nZdcRpd = 0;
    int nBad1=0, nBad2=0, nBad=0;
    std::vector<double> encontZdcEM(140, 0.);
    std::vector<double> encontZdcHad(140, 0.);
    std::vector<double> encontZdcRpd(140, 0.);
    double entotZdcEM  = 0;
    double entotZdcHad = 0;
    double entotZdcRpd = 0;
    
    enetotEmN  = 0.;
    enetotHadN = 0.;
    enetotRpdN = 0.;
    enetotN    = 0.;
    enetotEmP  = 0.;
    enetotHadP = 0.;
    enetotRpdP = 0.;
    enetotP    = 0.;
    enetot     = 0.;
    
    for (int i=0; i<nHit; i++) {
        double energy    = hits[i].energy();
        double log10en   = log10(energy);
        int log10i       = int( (log10en+10.)*10. );
        double time      = hits[i].time();
        unsigned int id_ = hits[i].id();
        HcalZDCDetId id  = HcalZDCDetId(id_);
        int det          = id.det();
        int side         = id.zside();
        int section      = id.section();
        if(section == HcalZDCDetId::RPD) std::cout<<"RPD HITS!! with specific channel ";
        int channel      = id.channel();
        
        std::cout
        << "energy           = " << hits[i].energy()  << "\n"
        << "time             = " << hits[i].time()    << "\n"
        << "id_              = " << std::hex << hits[i].id() << std::dec << "\n"
        << "HcalZDCDetId id  = " << HcalZDCDetId(id_) << "\n"
        << "det              = " << id.det()          << "\n"
        << "side             = " << id.zside()        << "\n"
        << "section          = " << id.section()      << "\n"
        << "channel          = " << id.channel()      << "\n"
        << std::endl;
        
        FillHitValHist(side,section,channel,energy,time);
        
        
        LogDebug("ZdcSimHitStudy");
        std::cout /*
        << "Hit[" << i << "] ID " << std::hex << id_
        << std::dec <<" DetID: "<<id
        << " Det "<< det << " side "<< side
        << " Section " << section
        << " channel "<< channel
        << " E " << energy
        << " time " << time << "\n"*/
        <<std::endl;
        
        if(det == 5) { // Check DetId.h
            if(section == HcalZDCDetId::EM)nZdcEM++;
            else if(section == HcalZDCDetId::RPD)nZdcRpd++;
            else if(section == HcalZDCDetId::HAD)nZdcHad++;
            else    { nBad++;  nBad2++;}
        } else    { nBad++;  nBad1++;}
        
        meZdcDetectHit_->Fill(double(det));
        if (det ==  5) {
            meZdcSideHit_->Fill(double(side));
            meZdcSectionHit_->Fill(double(section));
            meZdcChannelHit_->Fill(double(channel));
            meZdcEnergyHit_->Fill(energy);
            if(section == HcalZDCDetId::EM){
                meZdcEMEnergyHit_->Fill(energy);
                meZdcEEMCh_->Fill(energy,channel);
                if( log10i >=0 && log10i < 140 )encontZdcEM[log10i] += energy;
                entotZdcEM += energy;
            }
            if(section == HcalZDCDetId::RPD){
                meZdcRpdEnergyHit_->Fill(energy);
                meZdcERpdCh_->Fill(energy,channel);
                if( log10i >=0 && log10i <140 )encontZdcRpd[log10i] += energy;
                entotZdcRpd += energy;
            }
            if(section == HcalZDCDetId::HAD){
                meZdcHadEnergyHit_->Fill(energy);
                meZdcEHadCh_->Fill(energy,channel);
                if( log10i >=0 && log10i < 140 )encontZdcHad[log10i] += energy;
                entotZdcHad += energy;
            }
            meZdcTimeHit_->Fill(time);
            meZdcTimeWHit_->Fill(double(time),energy);
            meZdc10Ene_->Fill(log10en);
            meZdcETime_->Fill(energy, double(time));
        }
    }
    
    if( entotZdcEM  != 0 ) for( int i=0; i<140; i++ ) meZdcEML10EneP_->Fill( -10.+(float(i)+0.5)/10., encontZdcEM[i]/entotZdcEM);
    if( entotZdcHad != 0 ) for( int i=0; i<140; i++ ) meZdcHadL10EneP_->Fill( -10.+(float(i)+0.5)/10.,encontZdcHad[i]/entotZdcHad);
    if( entotZdcRpd != 0 ) for( int i=0; i<140; i++ ) meZdcRpdL10EneP_->Fill( -10.+(float(i)+0.5)/10., encontZdcRpd[i]/entotZdcRpd);
    
    std::cout << "Debugging:  "
    << " \nenetotEMP " << enetotEmP
    << " \nenetotHadP " << enetotHadP
    << " \nenetotRpdN " << enetotRpdN
    << " \nenetotRpdP " << enetotRpdP
    << std::endl;
    
    //** debug
    std::cout
    << "\nnZdcRpd = " << nZdcRpd
    << "\nenetotRpdN = " << enetotRpdN
    << "\nenetotRpdP = " << enetotRpdP
    << std::endl;
    //**
    
    if ( nHit>0) {
        meAllZdcNHit_->Fill(double(nHit));
        meBadZdcDetHit_->Fill(double(nBad1));
        meBadZdcSecHit_->Fill(double(nBad2));
        meBadZdcIdHit_->Fill(double(nBad));
        meZdcNHitEM_->Fill(double(nZdcEM));
        meZdcNHitRpd_->Fill(double(nZdcRpd));
        meZdcNHitHad_->Fill(double(nZdcHad));
        meZdcNHitLum_->Fill(double(nZdcLum));
        meZdcEnePTot_->Fill(enetotP);
        meZdcEneNTot_->Fill(enetotN);
        meZdcEneRpdNTot_->Fill(enetotRpdN); //replace once there's a ZDC-
        meZdcEneRpdPTot_->Fill(enetotRpdP);
        meZdcEneHadNTot_->Fill(enetotHadN);
        meZdcEneHadPTot_->Fill(enetotHadP);
        meZdcEneEmNTot_->Fill(enetotEmN);
        meZdcEneEmPTot_->Fill(enetotEmP);
        meZdcCorEEmNEHadN_->Fill(enetotEmN,enetotHadN);
        meZdcCorEEmPEHadP_->Fill(enetotEmP,enetotHadP);
        meZdcCorEtotNEtotP_->Fill(enetotN,enetotP);
        meZdcEneTot_->Fill(enetot);
    }
    LogDebug("HcalSimHitStudy");
    std::cout
    <<"HcalSimHitStudy::analyzeHits: Had " << nZdcHad
    << " EM "<< nZdcEM
    << " RPD " << nZdcRpd
    << " Bad " << nBad << " All " << nHit << "\n" 
    <<std::endl;
}

int ZdcSimHitStudy::FillHitValHist(int side,int section,int channel,double energy,double time){
    enetot += enetot;
    if(side == -1){
        enetotN += energy;
        if(section == HcalZDCDetId::EM){
            enetotEmN += energy;
            switch(channel){
                case 1 :
                    meZdcEneEmN1_->Fill(energy);
                    meZdcEneTEmN1_->Fill(energy,time);
                    break;
                case 2 :
                    meZdcEneEmN2_->Fill(energy);
                    meZdcEneTEmN2_->Fill(energy,time);
                    break;
                case 3 :
                    meZdcEneEmN3_->Fill(energy);
                    meZdcEneTEmN3_->Fill(energy,time);
                    break;
                case 4 :
                    meZdcEneEmN4_->Fill(energy);
                    meZdcEneTEmN4_->Fill(energy,time);
                    break;
                case 5 :
                    meZdcEneEmN4_->Fill(energy);
                    meZdcEneTEmN4_->Fill(energy,time);
                    break;
            }
        }
        if(section == HcalZDCDetId::RPD){
            enetotRpdN += energy;
            switch(channel){
                case 1 :
                    meZdcEneRpdN1_->Fill(energy);
                    meZdcEneTRpdN1_->Fill(energy,time);
                case 2 :
                    meZdcEneRpdN2_->Fill(energy);
                    meZdcEneTRpdN2_->Fill(energy,time);
                case 3 :
                    meZdcEneRpdN3_->Fill(energy);
                    meZdcEneTRpdN3_->Fill(energy,time);
                case 4 :
                    meZdcEneRpdN4_->Fill(energy);
                    meZdcEneTRpdN4_->Fill(energy,time);
                case 5 :
                    meZdcEneRpdN5_->Fill(energy);
                    meZdcEneTRpdN5_->Fill(energy,time);
                case 6 :
                    meZdcEneRpdN6_->Fill(energy);
                    meZdcEneTRpdN6_->Fill(energy,time);
                case 7 :
                    meZdcEneRpdN7_->Fill(energy);
                    meZdcEneTRpdN7_->Fill(energy,time);
                case 8 :
                    meZdcEneRpdN8_->Fill(energy);
                    meZdcEneTRpdN8_->Fill(energy,time);
                case 9 :
                    meZdcEneRpdN9_->Fill(energy);
                    meZdcEneTRpdN9_->Fill(energy,time);
                case 10 :
                    meZdcEneRpdN10_->Fill(energy);
                    meZdcEneTRpdN10_->Fill(energy,time);
                case 11 :
                    meZdcEneRpdN11_->Fill(energy);
                    meZdcEneTRpdN11_->Fill(energy,time);
                case 12 :
                    meZdcEneRpdN12_->Fill(energy);
                    meZdcEneTRpdN12_->Fill(energy,time);
                case 13 :
                    meZdcEneRpdN13_->Fill(energy);
                    meZdcEneTRpdN13_->Fill(energy,time);
                case 14 :
                    meZdcEneRpdN14_->Fill(energy);
                    meZdcEneTRpdN14_->Fill(energy,time);
                case 15 :
                    meZdcEneRpdN15_->Fill(energy);
                    meZdcEneTRpdN15_->Fill(energy,time);
                case 16 :
                    meZdcEneRpdN16_->Fill(energy);
                    meZdcEneTRpdN16_->Fill(energy,time);
            }
        }
        if(section == HcalZDCDetId::HAD){
            enetotHadN += energy;
            switch(channel){
                case 1 :
                    meZdcEneHadN1_->Fill(energy);
                    meZdcEneTHadN1_->Fill(energy,time);
                    break;
                case 2 :
                    meZdcEneHadN2_->Fill(energy);
                    meZdcEneTHadN2_->Fill(energy,time);
                    break;
                case 3 :
                    meZdcEneHadN3_->Fill(energy);
                    meZdcEneTHadN3_->Fill(energy,time);
                    break;
                case 4 :
                    meZdcEneHadN4_->Fill(energy);
                    meZdcEneTHadN4_->Fill(energy,time);
                    break;
            }
        }
    }
    if(side == 1){
        enetotP += energy;
        if(section == HcalZDCDetId::EM){
            enetotEmP += energy;
            switch(channel){
                case 1 :
                    meZdcEneEmP1_->Fill(energy);
                    meZdcEneTEmP1_->Fill(energy,time);
                    break;
                case 2 :
                    meZdcEneEmP2_->Fill(energy);
                    meZdcEneTEmP2_->Fill(energy,time);
                    break;
                case 3 :
                    meZdcEneEmP3_->Fill(energy);
                    meZdcEneTEmP3_->Fill(energy,time);
                    break;
                case 4 :
                    meZdcEneEmP4_->Fill(energy);
                    meZdcEneTEmP4_->Fill(energy,time);
                    break;
                case 5 :
                    meZdcEneEmP4_->Fill(energy);
                    meZdcEneTEmP4_->Fill(energy,time);
                    break;
            }
        }
        if(section == HcalZDCDetId::RPD){
            enetotRpdP += energy;
            switch(channel){
                case 1 :
                    meZdcEneRpdP1_->Fill(energy);
                    meZdcEneTRpdP1_->Fill(energy,time);
                case 2 :
                    meZdcEneRpdP2_->Fill(energy);
                    meZdcEneTRpdP2_->Fill(energy,time);
                case 3 :
                    meZdcEneRpdP3_->Fill(energy);
                    meZdcEneTRpdP3_->Fill(energy,time);
                case 4 :
                    meZdcEneRpdP4_->Fill(energy);
                    meZdcEneTRpdP4_->Fill(energy,time);
                case 5 :
                    meZdcEneRpdP5_->Fill(energy);
                    meZdcEneTRpdP5_->Fill(energy,time);
                case 6 :
                    meZdcEneRpdP6_->Fill(energy);
                    meZdcEneTRpdP6_->Fill(energy,time);
                case 7 :
                    meZdcEneRpdP7_->Fill(energy);
                    meZdcEneTRpdP7_->Fill(energy,time);
                case 8 :
                    meZdcEneRpdP8_->Fill(energy);
                    meZdcEneTRpdP8_->Fill(energy,time);
                case 9 :
                    meZdcEneRpdP9_->Fill(energy);
                    meZdcEneTRpdP9_->Fill(energy,time);
                case 10 :
                    meZdcEneRpdP10_->Fill(energy);
                    meZdcEneTRpdP10_->Fill(energy,time);
                case 11 :
                    meZdcEneRpdP11_->Fill(energy);
                    meZdcEneTRpdP11_->Fill(energy,time);
                case 12 :
                    meZdcEneRpdP12_->Fill(energy);
                    meZdcEneTRpdP12_->Fill(energy,time);
                case 13 :
                    meZdcEneRpdP13_->Fill(energy);
                    meZdcEneTRpdP13_->Fill(energy,time);
                case 14 :
                    meZdcEneRpdP14_->Fill(energy);
                    meZdcEneTRpdP14_->Fill(energy,time);
                case 15 :
                    meZdcEneRpdP15_->Fill(energy);
                    meZdcEneTRpdP15_->Fill(energy,time);
                case 16 :
                    meZdcEneRpdP16_->Fill(energy);
                    meZdcEneTRpdP16_->Fill(energy,time);
            }
        }
        if(section == HcalZDCDetId::HAD){
            enetotHadP += energy;
            switch(channel){
                case 1 :
                    meZdcEneHadP1_->Fill(energy);
                    meZdcEneTHadP1_->Fill(energy,time);
                    break;
                case 2 :
                    meZdcEneHadP2_->Fill(energy);
                    meZdcEneTHadP2_->Fill(energy,time);
                    break;
                case 3 :
                    meZdcEneHadP3_->Fill(energy);
                    meZdcEneTHadP3_->Fill(energy,time);
                    break;
                case 4 :
                    meZdcEneHadP4_->Fill(energy);
                    meZdcEneTHadP4_->Fill(energy,time);
                    break;
            }
        }
    }
    return 0;
}



void ZdcSimHitStudy::endRun(const edm::Run& run, const edm::EventSetup& c)
{
}


