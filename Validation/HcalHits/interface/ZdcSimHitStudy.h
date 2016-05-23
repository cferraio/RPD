////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Package:    ZdcSimHitStudy
// Class:      ZdcSimHitStudy
//
/*
 Description:
 This code has been developed to be a check for the ZDC sim. In 2009, it was found that the ZDC Simulation was unrealistic and needed repair. The aim of this code is to show the user the input and output of a ZDC MinBias simulation.
 
 Implementation:
 First a MinBias simulation should be run, it could be pythia,hijin,or hydjet. This will output a .root file which should have information about recoGenParticles, hcalunsuppresseddigis, and g4SimHits_ZDCHits. Use this .root file as the input into the cfg.py which is found in the main directory of this package. This output will be another .root file which is meant to be viewed in a TBrowser.
 
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



#ifndef SimG4CMS_ZdcSimHitStudy_H
#define SimG4CMS_ZdcSimHitStudy_H

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "DQMServices/Core/interface/DQMEDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DQMServices/Core/interface/MonitorElement.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "SimDataFormats/CaloHit/interface/PCaloHit.h"
#include "SimDataFormats/CaloHit/interface/PCaloHitContainer.h"

#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"


#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <memory>

class ZdcSimHitStudy: public DQMEDAnalyzer{
public:
    
    ZdcSimHitStudy(const edm::ParameterSet& ps);
    ~ZdcSimHitStudy();
    
protected:
    
    void endRun(const edm::Run& run, const edm::EventSetup& c);
    virtual void bookHistograms(DQMStore::IBooker &, edm::Run const &, edm::EventSetup const &);
    
    void analyze  (const edm::Event& e, const edm::EventSetup& c);
    void analyzeHits  (std::vector<PCaloHit> &);
    int FillHitValHist (int side,int section,int channel,double energy,double time);
    
private:
    double enetotEmN, enetotHadN, enetotRpdN, enetotN;
    double enetotEmP, enetotHadP, enetotRpdP, enetotP;
    double enetot;
    
    /////////////////////////////////////////
    //#   Below all the monitoring elements #
    //# are simply the plots "code names"  #
    //# they will be filled in the .cc file #
    /////////////////////////////////////////
    
    
    
    std::string    g4Label, zdcHits, outFile_;
    edm::EDGetTokenT<reco::GenParticleCollection> tok_gen_;
    edm::EDGetTokenT<edm::PCaloHitContainer> tok_hits_;
    bool           verbose_, checkHit_;
    
    MonitorElement *meAllZdcNHit_, *meBadZdcDetHit_, *meBadZdcSecHit_, *meBadZdcIdHit_;
    MonitorElement *meZdcNHit_,*meZdcDetectHit_,*meZdcSideHit_,*meZdcETime_;
    MonitorElement *meZdcNHitEM_,*meZdcNHitHad_,*meZdcNHitLum_,*meZdc10Ene_;
    MonitorElement *meZdcSectionHit_,*meZdcChannelHit_,*meZdcEnergyHit_,*meZdcTimeWHit_;
    MonitorElement *meZdcHadEnergyHit_, *meZdcEMEnergyHit_, *meZdcTimeHit_, *meZdcHadL10EneP_;
    MonitorElement *meZdc10EneP_, *meZdcEHadCh_, *meZdcEEMCh_,*meZdcEML10EneP_;
    MonitorElement *meZdcEneEmN1_,*meZdcEneEmN2_,*meZdcEneEmN3_,*meZdcEneEmN4_,*meZdcEneEmN5_;
    MonitorElement *meZdcEneHadN1_,*meZdcEneHadN2_,*meZdcEneHadN3_,*meZdcEneHadN4_;
    MonitorElement *meZdcEneTEmN1_,*meZdcEneTEmN2_,*meZdcEneTEmN3_,*meZdcEneTEmN4_,*meZdcEneTEmN5_;
    MonitorElement *meZdcEneTHadN1_,*meZdcEneTHadN2_,*meZdcEneTHadN3_,*meZdcEneTHadN4_;
    MonitorElement *meZdcEneHadNTot_,*meZdcEneEmNTot_,*meZdcEneNTot_;
    MonitorElement *meZdcCorEEmNEHadN_;
    MonitorElement *meZdcEneEmP1_,*meZdcEneEmP2_,*meZdcEneEmP3_,*meZdcEneEmP4_,*meZdcEneEmP5_;
    MonitorElement *meZdcEneHadP1_,*meZdcEneHadP2_,*meZdcEneHadP3_,*meZdcEneHadP4_;
    MonitorElement *meZdcEneTEmP1_,*meZdcEneTEmP2_,*meZdcEneTEmP3_,*meZdcEneTEmP4_,*meZdcEneTEmP5_;
    MonitorElement *meZdcEneTHadP1_,*meZdcEneTHadP2_,*meZdcEneTHadP3_,*meZdcEneTHadP4_;
    MonitorElement *meZdcEneHadPTot_,*meZdcEneEmPTot_, *meZdcEnePTot_;
    MonitorElement *meZdcCorEEmPEHadP_,*meZdcCorEtotNEtotP_,*meZdcEneTot_;
    
    ///////////////////New Plots/////////////
    
    ////////GenParticle Plots///////
    MonitorElement* genpart_Pi0F;
    MonitorElement* genpart_Pi0F_energydist;
    MonitorElement* genpart_Pi0B;
    MonitorElement* genpart_Pi0B_energydist;
    MonitorElement* genpart_NeutF;
    MonitorElement* genpart_NeutF_energydist;
    MonitorElement* genpart_NeutB;
    MonitorElement* genpart_NeutB_energydist;
    MonitorElement* genpart_GammaF;
    MonitorElement* genpart_GammaF_energydist;
    MonitorElement* genpart_GammaB;
    MonitorElement* genpart_GammaB_energydist;
    //////////////////////////////
    
    
    //N counts plots
    MonitorElement* genpart_Pi0F_counts;
    MonitorElement* genpart_Pi0B_counts;
    MonitorElement* genpart_NeutF_counts;
    MonitorElement* genpart_NeutB_counts;
    MonitorElement* genpart_GammaF_counts;
    MonitorElement* genpart_GammaB_counts;
    
    /////////////////////////////////////////
    //# New plots for the RPD flow detector #
    /////////////////////////////////////////
    
    MonitorElement         *meZdcNHitRpd_;
    MonitorElement         *meZdcRpdEnergyHit_, *meZdcRpdL10EneP_;
    MonitorElement         *meZdcERpdCh_;
    MonitorElement         *meZdcEneRpdN1_,  *meZdcEneRpdN2_,  *meZdcEneRpdN3_,
        *meZdcEneRpdN4_,   *meZdcEneRpdN5_,  *meZdcEneRpdN6_,  *meZdcEneRpdN7_,
        *meZdcEneRpdN8_,   *meZdcEneRpdN9_,  *meZdcEneRpdN10_, *meZdcEneRpdN11_,
        *meZdcEneRpdN12_,  *meZdcEneRpdN13_, *meZdcEneRpdN14_, *meZdcEneRpdN15_,
        *meZdcEneRpdN16_;
    MonitorElement         *meZdcEneTRpdN1_,  *meZdcEneTRpdN2_,  *meZdcEneTRpdN3_,
        *meZdcEneTRpdN4_,  *meZdcEneTRpdN5_,  *meZdcEneTRpdN6_,  *meZdcEneTRpdN7_,
        *meZdcEneTRpdN8_,  *meZdcEneTRpdN9_,  *meZdcEneTRpdN10_, *meZdcEneTRpdN11_,
        *meZdcEneTRpdN12_, *meZdcEneTRpdN13_, *meZdcEneTRpdN14_, *meZdcEneTRpdN15_,
        *meZdcEneTRpdN16_;
    MonitorElement         *meZdcEneRpdNTot_;
    MonitorElement         *meZdcCorEEmNERpdN_;
    MonitorElement         *meZdcEneRpdP1_,  *meZdcEneRpdP2_,  *meZdcEneRpdP3_,
        *meZdcEneRpdP4_,   *meZdcEneRpdP5_,  *meZdcEneRpdP6_,  *meZdcEneRpdP7_,
        *meZdcEneRpdP8_,   *meZdcEneRpdP9_,  *meZdcEneRpdP10_, *meZdcEneRpdP11_,
        *meZdcEneRpdP12_,  *meZdcEneRpdP13_, *meZdcEneRpdP14_, *meZdcEneRpdP15_,
        *meZdcEneRpdP16_;
    MonitorElement         *meZdcEneTRpdP1_,  *meZdcEneTRpdP2_,  *meZdcEneTRpdP3_,
        *meZdcEneTRpdP4_,  *meZdcEneTRpdP5_,  *meZdcEneTRpdP6_,  *meZdcEneTRpdP7_,
        *meZdcEneTRpdP8_,  *meZdcEneTRpdP9_,  *meZdcEneTRpdP10_, *meZdcEneTRpdP11_,
        *meZdcEneTRpdP12_, *meZdcEneTRpdP13_, *meZdcEneTRpdP14_, *meZdcEneTRpdP15_,
        *meZdcEneTRpdP16_;
    MonitorElement         *meZdcEneRpdPTot_;
    MonitorElement         *meZdcCorEEmPERpdP_;
};

#endif
