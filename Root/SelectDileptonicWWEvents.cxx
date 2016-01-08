#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>
#include <RJigsawTools/SelectDileptonicWWEvents.h>

#include <AsgTools/MsgStream.h>
#include <AsgTools/MsgStreamMacros.h>

#include "xAODRootAccess/Init.h"
#include "xAODRootAccess/TEvent.h"
#include "xAODRootAccess/TStore.h"

#include "SUSYTools/SUSYObjDef_xAOD.h"

#include <RJigsawTools/strongErrorCheck.h>

#include "xAODParticleEvent/ParticleContainer.h"
#include "xAODParticleEvent/ParticleAuxContainer.h"


// this is needed to distribute the algorithm to the workers
ClassImp(SelectDileptonicWWEvents)



SelectDileptonicWWEvents :: SelectDileptonicWWEvents ()
{
  // Here you put any code for the base initialization of variables,
  // e.g. initialize all pointers to 0.  Note that you should only put
  // the most basic initialization here, since this method will be
  // called on both the submission and the worker node.  Most of your
  // initialization code will go into histInitialize() and
  // initialize().
}



EL::StatusCode SelectDileptonicWWEvents :: setupJob (EL::Job& job)
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



EL::StatusCode SelectDileptonicWWEvents :: histInitialize ()
{
  // Here you do everything that needs to be done at the very
  // beginning on each worker node, e.g. create histograms and output
  // trees.  This method gets called before any input files are
  // connected.
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode SelectDileptonicWWEvents :: fileExecute ()
{
  // Here you do everything that needs to be done exactly once for every
  // single file, e.g. collect a list of all lumi-blocks processed
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode SelectDileptonicWWEvents :: changeInput (bool firstFile)
{
  // Here you do everything you need to do when we change input files,
  // e.g. resetting branch addresses on trees.  If you are using
  // D3PDReader or a similar service this method is not needed.
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode SelectDileptonicWWEvents :: initialize ()
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



EL::StatusCode SelectDileptonicWWEvents :: execute ()
{
  // Here you do everything that needs to be done on every single
  // events, e.g. read input variables, apply cuts, and fill
  // histograms and trees.  This is where most of your actual analysis
  // code will go.

  //todo add some preselection here!

  xAOD::TStore * store = wk()->xaodStore();


  const xAOD::EventInfo* eventInfo = 0;
  STRONG_CHECK(store->retrieve( eventInfo, "EventInfo"));



  xAOD::IParticleContainer * selectedLeptons = new xAOD::IParticleContainer();

  xAOD::ParticleContainer * myparticles = new xAOD::ParticleContainer();
  xAOD::ParticleAuxContainer * myparticlesaux = new xAOD::ParticleAuxContainer();
  myparticles->setStore(myparticlesaux);



  // If the event didn't pass the preselection alg, don't bother doing anything with it...
  std::string regionName =  eventInfo->auxdecor< std::string >("regionName");
  ATH_MSG_DEBUG("Preselected?: " << regionName  );

  if( regionName == "" ) return EL::StatusCode::SUCCESS;



  xAOD::MuonContainer* muons_nominal(nullptr);
  STRONG_CHECK(store->retrieve(muons_nominal, "STCalibMuons"));

  xAOD::ElectronContainer* electrons_nominal(nullptr);
  STRONG_CHECK(store->retrieve(electrons_nominal, "STCalibElectrons"));

  for (const auto& mu : *muons_nominal) {
    if ((int)mu->auxdata<char>("baseline") == 0) continue;
    if ((int)mu->auxdata<char>("passOR") != 1) continue;
    if ((int)mu->auxdata<char>("signal") != 1) continue;
    // If I've gotten this far, I have a signal, isolated, beautiful muon
    selectedLeptons->push_back(mu);
  }

  for (const auto& el : *electrons_nominal) {
    if ((int)el->auxdata<char>("baseline") == 0) continue;
    if ((int)el->auxdata<char>("passOR") != 1) continue;
    if ((int)el->auxdata<char>("signal") != 1) continue;
    // If I've gotten this far, I have a signal, isolated, beautiful el
    selectedLeptons->push_back(el);
  }

  int nLeptons = selectedLeptons->size();

  ATH_MSG_DEBUG("Number of Selected Leptons: " << nLeptons  );

  //Let's just categorize from here maybe? But if we want different CRs in different algs,
  // then we'd need to play with something in the store a little more smartly

  if(nLeptons==2){
    regionName = "SR";
  } else if(nLeptons==1) {
    regionName = "CR1L";
  } else if(nLeptons==0) {
    regionName = "CR0L";
  }

  //Here we should add the particles that we want in the calculation to myparticles
  for( const auto& mylepton: *selectedLeptons){
    xAOD::Particle *tmpparticle = new xAOD::Particle;
    myparticles->push_back(tmpparticle  );
    tmpparticle->setP4( mylepton->p4() );
    ATH_MSG_DEBUG("particle pT: " << tmpparticle->pt()  );
  }

  ATH_MSG_DEBUG("Event falls in region: " << regionName  );


  ATH_MSG_DEBUG("Writing particle container for calculator to store");
  STRONG_CHECK( store->record( myparticles    , "myparticles"    ) );//todo configurable if needed
  STRONG_CHECK( store->record( myparticlesaux    , "myparticlesaux."    ) );//todo configurable if needed


  eventInfo->auxdecor< std::string >("regionName") = regionName ;
  ATH_MSG_DEBUG("Writing to eventInfo decoration: " <<  eventInfo->auxdecor< std::string >("regionName")   );

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode SelectDileptonicWWEvents :: postExecute ()
{
  // Here you do everything that needs to be done after the main event
  // processing.  This is typically very rare, particularly in user
  // code.  It is mainly used in implementing the NTupleSvc.
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode SelectDileptonicWWEvents :: finalize ()
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



EL::StatusCode SelectDileptonicWWEvents :: histFinalize ()
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
