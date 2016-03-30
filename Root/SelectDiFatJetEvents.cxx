#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>
#include <RJigsawTools/SelectDiFatJetEvents.h>

#include <AsgTools/MsgStream.h>
#include <AsgTools/MsgStreamMacros.h>

#include "xAODRootAccess/Init.h"
#include "xAODRootAccess/TEvent.h"
#include "xAODRootAccess/TStore.h"

#include "SUSYTools/SUSYObjDef_xAOD.h"

#include <RJigsawTools/strongErrorCheck.h>

#include "xAODBase/IParticleContainer.h"
#include "xAODParticleEvent/ParticleAuxContainer.h"
#include <utility>

// this is needed to distribute the algorithm to the workers
ClassImp(SelectDiFatJetEvents)



SelectDiFatJetEvents :: SelectDiFatJetEvents ()
{
  // Here you put any code for the base initialization of variables,
  // e.g. initialize all pointers to 0.  Note that you should only put
  // the most basic initialization here, since this method will be
  // called on both the submission and the worker node.  Most of your
  // initialization code will go into histInitialize() and
  // initialize().
}



EL::StatusCode SelectDiFatJetEvents :: setupJob (EL::Job& job)
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



EL::StatusCode SelectDiFatJetEvents :: histInitialize ()
{
  // Here you do everything that needs to be done at the very
  // beginning on each worker node, e.g. create histograms and output
  // trees.  This method gets called before any input files are
  // connected.
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode SelectDiFatJetEvents :: fileExecute ()
{
  // Here you do everything that needs to be done exactly once for every
  // single file, e.g. collect a list of all lumi-blocks processed
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode SelectDiFatJetEvents :: changeInput (bool firstFile)
{
  // Here you do everything you need to do when we change input files,
  // e.g. resetting branch addresses on trees.  If you are using
  // D3PDReader or a similar service this method is not needed.
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode SelectDiFatJetEvents :: initialize ()
{
  // Here you do everything that you need to do after the first input
  // file has been connected and before the first event is processed,
  // e.g. create additional histograms based on which variables are
  // available in the input files.  You can also create all of your
  // histograms and trees in here, but be aware that this method
  // doesn't get called if no events are processed.  So any objects
  // you create here won't be available in the output if you have no
  // input events.
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode SelectDiFatJetEvents :: execute ()
{
  // Here you do everything that needs to be done on every single
  // events, e.g. read input variables, apply cuts, and fill
  // histograms and trees.  This is where most of your actual analysis
  // code will go.

  //todo add some preselection here!

  xAOD::TStore * store = wk()->xaodStore();
  xAOD::TEvent* event = wk()->xaodEvent();

  const xAOD::EventInfo* eventInfo = 0;
  STRONG_CHECK(event->retrieve( eventInfo, "EventInfo"));

  // If the event didn't pass the preselection alg, don't bother doing anything with it...
  std::string preselectedRegionName =  eventInfo->auxdecor< std::string >("regionName");
  ATH_MSG_DEBUG("Preselected?: " << preselectedRegionName  );

  if( preselectedRegionName == "" ) return EL::StatusCode::SUCCESS;

  std::pair<xAOD::IParticleContainer* , xAOD::ParticleAuxContainer*> selectedJets   ( new xAOD::IParticleContainer(SG::VIEW_ELEMENTS) , nullptr);
  selectedJets.first->setStore(selectedJets.second);
  std::pair<xAOD::IParticleContainer* , xAOD::ParticleAuxContainer *> selectedRJigsawParticles(new xAOD::IParticleContainer(SG::VIEW_ELEMENTS), nullptr);
  selectedRJigsawParticles.first->setStore(selectedRJigsawParticles.second);

  STRONG_CHECK( store->record( selectedJets.first     , "selectedJets"    ) );//todo configurable if needed
  STRONG_CHECK( store->record( selectedJets.second    , "selectedJetsAux."    ) );//todo configurable if needed

  STRONG_CHECK( store->record( selectedRJigsawParticles.first  , "myparticles"    ) );//todo configurable if needed
  STRONG_CHECK( store->record( selectedRJigsawParticles.second , "myparticlesaux."    ) );//todo configurable if needed


  xAOD::JetContainer* jets_nominal(nullptr);
  STRONG_CHECK(store->retrieve(jets_nominal, "STCalibCamKt12LCTopoJets"));

  for (const auto& jet : *jets_nominal) {
    if ((int)jet->auxdata<char>("baseline") == 0) continue;
    if ((int)jet->auxdata<char>("passOR") != 1) continue;
    if ((int)jet->auxdata<char>("signal") != 1) continue;
    // If I've gotten this far, I have a signal, isolated, beautiful jet
    ATH_MSG_VERBOSE( "jet pt : " << jet->pt() );

    selectedJets.first->push_back( jet  );
  }

  //Let's just categorize from here maybe? But if we want different CRs in different algs,
  // then we'd need to play with something in the store a little more smartly

  std::string regionName = "";

  auto  getRegionName = [](int nFatJets){ std::string regionName = "";
					  if(nFatJets >= 2){regionName = "SR";}
					  if(nFatJets == 1){regionName = "CR1J";}
					  if(nFatJets == 0){regionName = "CR0J";}
					  return regionName;};//todo >2 leptons in the SR? pretty rare though

  int nFatJets = selectedJets.first->size();


  ATH_MSG_DEBUG("Event falls in region: " << getRegionName( nFatJets)  );

  ATH_MSG_DEBUG("Writing particle container for calculator to store");

  eventInfo->auxdecor< std::string >("regionName") = getRegionName( nFatJets) ;
  ATH_MSG_DEBUG("Writing to eventInfo decoration: " <<  eventInfo->auxdecor< std::string >("regionName")   );

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode SelectDiFatJetEvents :: postExecute ()
{
  // Here you do everything that needs to be done after the main event
  // processing.  This is typically very rare, particularly in user
  // code.  It is mainly used in implementing the NTupleSvc.
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode SelectDiFatJetEvents :: finalize ()
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



EL::StatusCode SelectDiFatJetEvents :: histFinalize ()
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
