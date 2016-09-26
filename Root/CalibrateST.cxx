#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>

#include <PATInterfaces/SystematicRegistry.h>

#include "SUSYTools/SUSYObjDef_xAOD.h"
#include "xAODEventInfo/EventInfo.h"
#include "xAODEventInfo/EventAuxInfo.h"

// Infrastructure include(s):
#include "xAODRootAccess/Init.h"
#include "xAODRootAccess/TEvent.h"

#include "xAODMissingET/MissingETContainer.h"
#include "xAODMissingET/MissingETAuxContainer.h"
#include "TauAnalysisTools/TauSmearingTool.h"
#include "TauAnalysisTools/ITauSmearingTool.h"

#include "FactoryTools/CalibrateST.h"
#include "FactoryTools/strongErrorCheck.h"

#include <boost/algorithm/string/replace.hpp>

// this is needed to distribute the algorithm to the workers
ClassImp(CalibrateST)

CalibrateST :: CalibrateST () :
systName(notSetString()),
m_objTool(nullptr)
{
  // Here you put any code for the base initialization of variables,
  // e.g. initialize all pointers to 0.  Note that you should only put
  // the most basic initialization here, since this method will be
  // called on both the submission and the worker node.  Most of your
  // initialization code will go into histInitialize() and
  // initialize().
}



EL::StatusCode CalibrateST :: setupJob (EL::Job& job)
{
  // Here you put code that sets up the job on the submission object
  // so that it is ready to work with your algorithm, e.g. you can
  // request the D3PDReader service or add output files.  Any code you
  // put here could instead also go into the submission script.  The
  // sole advantage of putting it here is that it gets automatically
  // activated/deactivated when you add/remove the algorithm from your
  // job, which may or may not be of value to you.
	return EL::StatusCode::SUCCESS;
}



EL::StatusCode CalibrateST :: histInitialize ()
{
  // Here you do everything that needs to be done at the very
  // beginning on each worker node, e.g. create histograms and output
  // trees.  This method gets called before any input files are
  // connected.
	return EL::StatusCode::SUCCESS;
}



EL::StatusCode CalibrateST :: fileExecute ()
{
  // Here you do everything that needs to be done exactly once for every
  // single file, e.g. collect a list of all lumi-blocks processed
	return EL::StatusCode::SUCCESS;
}



EL::StatusCode CalibrateST :: changeInput (bool firstFile)
{
  // Here you do everything you need to do when we change input files,
  // e.g. resetting branch addresses on trees.  If you are using
  // D3PDReader or a similar service this method is not needed.
	return EL::StatusCode::SUCCESS;
}



EL::StatusCode CalibrateST :: initialize ()
{
  // Here you do everything that you need to do after the first input
  // file has been connected and before the first event is processed,
  // e.g. create additional histograms based on which variables are
  // available in the input files.  You can also create all of your
  // histograms and trees in here, but be aware that this method
  // doesn't get called if no events are processed.  So any objects
  // you create here won't be available in the output if you have no
  // input events.

	xAOD::TEvent* event = wk()->xaodEvent();
	xAOD::TStore* store = wk()->xaodStore();

	const xAOD::EventInfo* eventInfo = 0;
	STRONG_CHECK(event->retrieve( eventInfo, "EventInfo"));

	// check if the event is data or MC
	// (many tools are applied either to data or MC)
	bool const isData = !(eventInfo->eventType( xAOD::EventInfo::IS_SIMULATION ));
	bool const isAtlfast = false;

	ST::ISUSYObjDef_xAODTool::DataSource datasource = (isData ? ST::ISUSYObjDef_xAODTool::Data : (isAtlfast ? ST::ISUSYObjDef_xAODTool::AtlfastII : ST::ISUSYObjDef_xAODTool::FullSim));
	if( systName == notSetString()) {
		ATH_MSG_ERROR( "you need to set the systematic string in your run script!");
		ATH_MSG_ERROR( "If you wish to run without systematics, set the systName to \"\"");
		return EL::StatusCode::FAILURE;
	}

	if( PRWLumiCalcFileNames == notSetString()) {
		ATH_MSG_ERROR( "you need to set the lumicalc file path in your run script!");
		return EL::StatusCode::FAILURE;
	}

	if( PRWConfigFileNames == notSetString()) {
		ATH_MSG_ERROR( "you need to set the pileup reweighting file path in your run script!");
		return EL::StatusCode::FAILURE;
	}


	m_objTool = new ST::SUSYObjDef_xAOD( "SUSYObjDef_xAOD" + systName );

	STRONG_CHECK( m_objTool->setProperty("DataSource", datasource)) ;
	if( SUSYToolsConfigFileName == notSetString() ) {
		STRONG_CHECK( m_objTool->setProperty("ConfigFile", "SUSYTools/SUSYTools_Default.conf") );
	} else {
		ATH_MSG_DEBUG( "analysisName set. Trying to open config file "<< SUSYToolsConfigFileName );
		STRONG_CHECK( m_objTool->setProperty("ConfigFile", SUSYToolsConfigFileName ) );
	}
	std::vector<std::string> PRWConfigFileNamesVec  ;
	std::vector<std::string> PRWLumiCalcFileNamesVec;

	PRWConfigFileNamesVec  .push_back(PRWConfigFileNames  ) ;
	PRWLumiCalcFileNamesVec.push_back(PRWLumiCalcFileNames) ;

	STRONG_CHECK( m_objTool->setProperty("PRWConfigFiles",   PRWConfigFileNamesVec     ) );
	STRONG_CHECK( m_objTool->setProperty("PRWLumiCalcFiles", PRWLumiCalcFileNamesVec   ) );

	m_objTool->msg().setLevel( this->msg().level());
	STRONG_CHECK( m_objTool->initialize());

	return EL::StatusCode::SUCCESS;
}



EL::StatusCode CalibrateST :: execute ()
{
	// Here you do everything that needs to be done on every single
	// events, e.g. read input variables, apply cuts, and fill
	// histograms and trees.  This is where most of your actual analysis
	// code will go.

	STRONG_CHECK( m_objTool->applySystematicVariation(systName));//apply the systematic variation

	xAOD::TStore * store = wk()->xaodStore();
	xAOD::TEvent * event = wk()->xaodEvent();

	//  store->print();
	store->clear();//We must clear the store when doing systematics so that we have a brand new setup.

	const xAOD::EventInfo* eventInfo(nullptr);
	STRONG_CHECK(event->retrieve( eventInfo, "EventInfo"));

	if( eventInfo->eventType( xAOD::EventInfo::IS_SIMULATION ) ) m_objTool->ApplyPRWTool();

	bool hasElectrons = event->contains<xAOD::ElectronContainer>( "Electrons" );
	bool hasPhotons = event->contains<xAOD::PhotonContainer>( "Photons" );
	bool hasMuons = event->contains<xAOD::MuonContainer>( "Muons" );
	bool hasTaus = event->contains<xAOD::TauJetContainer>( "TauJets" );

	// Get the nominal object containers from the event
	// Electrons
	xAOD::ElectronContainer* electrons_nominal(nullptr);
	xAOD::ShallowAuxContainer* electrons_nominal_aux(nullptr);
	if(hasElectrons) {
		STRONG_CHECK( m_objTool->GetElectrons(electrons_nominal, electrons_nominal_aux, true) );
	}

	// Photons
	xAOD::PhotonContainer* photons_nominal(nullptr);
	xAOD::ShallowAuxContainer* photons_nominal_aux(nullptr);
	if(hasPhotons) {
		STRONG_CHECK( m_objTool->GetPhotons(photons_nominal,photons_nominal_aux, true) );
	}

	// Muons
	xAOD::MuonContainer* muons_nominal(nullptr);
	xAOD::ShallowAuxContainer* muons_nominal_aux(nullptr);
	if(hasMuons) {
		STRONG_CHECK( m_objTool->GetMuons(muons_nominal, muons_nominal_aux, true) );
	}

	// Jets
	xAOD::JetContainer* jets_nominal(nullptr);
	xAOD::ShallowAuxContainer* jets_nominal_aux(nullptr);
	STRONG_CHECK( m_objTool->GetJets(jets_nominal, jets_nominal_aux, true) );

	// Fat Jets
	if(fatJetContainerName != notSetString() && event->contains<xAOD::JetContainer>( fatJetContainerName ) ){
		xAOD::JetContainer* fatjets_nominal(nullptr);
		xAOD::ShallowAuxContainer* fatjets_nominal_aux(nullptr);
		STRONG_CHECK( m_objTool->GetFatJets(fatjets_nominal, fatjets_nominal_aux, true, fatJetContainerName ) );
	}

	// Taus
	xAOD::TauJetContainer* taus_nominal(nullptr);
	xAOD::ShallowAuxContainer* taus_nominal_aux(nullptr);
	if(hasTaus) {
		STRONG_CHECK( m_objTool->GetTaus(taus_nominal,taus_nominal_aux) );
	}

	xAOD::MissingETContainer*    newMetContainer    = new xAOD::MissingETContainer();
	xAOD::MissingETAuxContainer* newMetAuxContainer = new xAOD::MissingETAuxContainer();
	newMetContainer->setStore(newMetAuxContainer);

	//todo this needs to be moved and calculated after selections
	if (hasElectrons && hasMuons && hasPhotons) STRONG_CHECK( m_objTool->GetMET(*newMetContainer,
						jets_nominal,
						electrons_nominal,
						muons_nominal,
						photons_nominal,
						nullptr, //taus_nominal,
						true,    //tst
						true,    //dojvt  cut
						nullptr  //no invisible particles in met
						));


	if (hasElectrons && hasMuons) STRONG_CHECK(  m_objTool->OverlapRemoval(electrons_nominal, muons_nominal, jets_nominal) );

	//everything other than MET is stored by default using the ST call with true
	STRONG_CHECK( store->record( newMetContainer    , "STCalibMET"    ) );//todo configurable if needed
	STRONG_CHECK( store->record( newMetAuxContainer , "STCalibMETAux.") );//todo configurable if needed


	// Also need to add trigger matching here

	/////////////////////////////////////////////////////////////////
	// Muons! - Figuring out year, getting SF
	//

	if(muTrig2015=="" ) muTrig2015 = "HLT_mu20_iloose_L1MU15_OR_HLT_mu50";
	if(muTrig2016=="" ) muTrig2016 = "HLT_mu26_ivarmedium";

	float muSF = 1.0;
	if ( eventInfo->eventType( xAOD::EventInfo::IS_SIMULATION ) && hasMuons){

		muSF = (float) m_objTool->GetTotalMuonSF(*muons_nominal,true,true, m_objTool->treatAsYear()==2015 ? muTrig2015: muTrig2016);
		//loop over electrons and attach trigger matching

		bool passTM = false;
		for (auto mu: *muons_nominal){
			passTM=false;
			passTM |= m_objTool->IsTrigMatched(mu, m_objTool->treatAsYear()==2015 ? 
				boost::replace_all_copy(muTrig2015, "_OR_", "") :
				boost::replace_all_copy(muTrig2016, "_OR_", "") );
			(mu)->auxdecor< int >( "passTM" ) = passTM;
		}
	}
	eventInfo->auxdecor<float>("muSF") = muSF ;

	//
	/////////////////////////////////////////////////////////////////


	/////////////////////////////////////////////////////////////////
	// Electrons! - Figuring out year, getting SF
	//

	if(elTrig2015=="") elTrig2015 = "HLT_e24_lhmedium_L1EM18VH";
	if(elTrig2016=="") elTrig2016 = "HLT_e26_lhtight_nod0_ivarloose";
	
	float elSF = 1.0;
	if ( eventInfo->eventType( xAOD::EventInfo::IS_SIMULATION ) && hasElectrons){
		elSF = (float) m_objTool->GetTotalElectronSF(*electrons_nominal);

		bool passTM = false;
		for (auto el: *electrons_nominal){
			passTM=false;
			passTM |= m_objTool->IsTrigMatched(el, m_objTool->treatAsYear()==2015 ? 
				boost::replace_all_copy(elTrig2015, "_OR_", "") :
				boost::replace_all_copy(elTrig2016, "_OR_", "") );
			(el)->auxdecor< int >( "passTM" ) = passTM;
		}
	}
	eventInfo->auxdecor<float>("elSF") = elSF ;

	//
	/////////////////////////////////////////////////////////////////


	/////////////////////////////////////////////////////////////////
	// B-Tagging! - Getting SF
	//

	float btagSF = 1.0;
	if ( eventInfo->eventType( xAOD::EventInfo::IS_SIMULATION ) ){
		btagSF = (float) m_objTool->BtagSF(jets_nominal);
	}
	eventInfo->auxdecor<float>("btagSF") = btagSF ;

	//
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	// Storing triggers and year to eventInfo
	//

	eventInfo->auxdecor<float>("year") =  m_objTool->treatAsYear();


	eventInfo->auxdecor<std::string>("muTrig2015") =  muTrig2015;
	eventInfo->auxdecor<std::string>("muTrig2016") =  muTrig2016;
	eventInfo->auxdecor<std::string>("elTrig2015") =  elTrig2015;
	eventInfo->auxdecor<std::string>("elTrig2016") =  elTrig2016;

	//
	/////////////////////////////////////////////////////////////////


	return EL::StatusCode::SUCCESS;
}



EL::StatusCode CalibrateST :: postExecute ()
{
	// Here you do everything that needs to be done after the main event
	// processing.  This is typically very rare, particularly in user
	// code.  It is mainly used in implementing the NTupleSvc.
	return EL::StatusCode::SUCCESS;
}



EL::StatusCode CalibrateST :: finalize ()
{
  // This method is the mirror image of initialize(), meaning it gets
  // called after the last event has been processed on the worker node
  // and allows you to finish up any objects you created in
  // initialize() before they are written to disk.  This is actually
  // fairly rare, since this happens separately for each worker node.
  // Most of the time you want to do your post-processing on the
  // submission node after all your histogram outputs have been
  // merged.  This is different from histFinalize() in that it only
  // gets called on worker nodes that processed input events.
	return EL::StatusCode::SUCCESS;
}



EL::StatusCode CalibrateST :: histFinalize ()
{
  // This method is the mirror image of histInitialize(), meaning it
  // gets called after the last event has been processed on the worker
  // node and allows you to finish up any objects you created in
  // histInitialize() before they are written to disk.  This is
  // actually fairly rare, since this happens separately for each
  // worker node.  Most of the time you want to do your
  // post-processing on the submission node after all your histogram
  // outputs have been merged.  This is different from finalize() in
  // that it gets called on all worker nodes regardless of whether
  // they processed input events.
	return EL::StatusCode::SUCCESS;
}
