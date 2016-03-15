#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>
#include <RJigsawTools/PreselectDiFatJetEvents.h>

#include <AsgTools/MsgStream.h>
#include <AsgTools/MsgStreamMacros.h>

#include "xAODRootAccess/Init.h"
#include "xAODRootAccess/TEvent.h"
#include "xAODRootAccess/TStore.h"

#include <TSystem.h>

#include "SUSYTools/SUSYObjDef_xAOD.h"

#include <RJigsawTools/strongErrorCheck.h>



// this is needed to distribute the algorithm to the workers
ClassImp(PreselectDiFatJetEvents)



PreselectDiFatJetEvents :: PreselectDiFatJetEvents ()
{
  // Here you put any code for the base initialization of variables,
  // e.g. initialize all pointers to 0.  Note that you should only put
  // the most basic initialization here, since this method will be
  // called on both the submission and the worker node.  Most of your
  // initialization code will go into histInitialize() and
  // initialize().
}



EL::StatusCode PreselectDiFatJetEvents :: setupJob (EL::Job& job)
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



EL::StatusCode PreselectDiFatJetEvents :: histInitialize ()
{
  // Here you do everything that needs to be done at the very
  // beginning on each worker node, e.g. create histograms and output
  // trees.  This method gets called before any input files are
  // connected.
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode PreselectDiFatJetEvents :: fileExecute ()
{
  // Here you do everything that needs to be done exactly once for every
  // single file, e.g. collect a list of all lumi-blocks processed
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode PreselectDiFatJetEvents :: changeInput (bool firstFile)
{
  // Here you do everything you need to do when we change input files,
  // e.g. resetting branch addresses on trees.  If you are using
  // D3PDReader or a similar service this method is not needed.
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode PreselectDiFatJetEvents :: initialize ()
{
  // Here you do everything that you need to do after the first input
  // file has been connected and before the first event is processed,
  // e.g. create additional histograms based on which variables are
  // available in the input files.  You can also create all of your
  // histograms and trees in here, but be aware that this method
  // doesn't get called if no events are processed.  So any objects
  // you create here won't be available in the output if you have no
  // input events.




  // // GRL
  // m_grl = new GoodRunsListSelectionTool("GoodRunsListSelectionTool");
  // // Make this python-configurable! /LL
  // std::string GRLFilePath = "$ROOTCOREBIN/data/SUSYTools/GRL/Moriond2016/data15_13TeV.periodAllYear_DetStatus-v73-pro19-08_DQDefects-00-01-02_PHYS_StandardGRL_All_Good_25ns.xml";
  // std::vector<std::string> vecStringGRL;
  // vecStringGRL.push_back(GRLFilePath);
  // STRONG_CHECK(m_grl->setProperty( "GoodRunsListVec", vecStringGRL));

  // // Make this python-configurable! /LL
  // STRONG_CHECK(m_grl->setProperty("PassThrough", false)); // if true (default) will ignore result of GRL and will just pass all events
  // STRONG_CHECK(m_grl->initialize());


  return EL::StatusCode::SUCCESS;
}



EL::StatusCode PreselectDiFatJetEvents :: execute ()
{
  // Here you do everything that needs to be done on every single
  // events, e.g. read input variables, apply cuts, and fill
  // histograms and trees.  This is where most of your actual analysis
  // code will go.

  //todo add some preselection here!
  xAOD::TEvent* event = wk()->xaodEvent();
  xAOD::TStore * store = wk()->xaodStore();

  const xAOD::EventInfo* eventInfo = 0;
  STRONG_CHECK(event->retrieve( eventInfo, "EventInfo"));
  eventInfo->auxdecor< std::string >("regionName") = "Preselected";


  if(eventInfo->auxdecor< std::string >("regionName") == "")  wk()->skipEvent();

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode PreselectDiFatJetEvents :: postExecute ()
{
  // Here you do everything that needs to be done after the main event
  // processing.  This is typically very rare, particularly in user
  // code.  It is mainly used in implementing the NTupleSvc.
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode PreselectDiFatJetEvents :: finalize ()
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


  // GRL
  // if (m_grl) {
  //   delete m_grl;
  //   m_grl = 0;
  // }


  return EL::StatusCode::SUCCESS;
}



EL::StatusCode PreselectDiFatJetEvents :: histFinalize ()
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
