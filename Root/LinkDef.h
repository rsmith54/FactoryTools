#include <FactoryTools/CalculateRegionVars.h>

#include <FactoryTools/CalculateRJigsawVariables.h>


#include <FactoryTools/PreselectDileptonicWWEvents.h>
#include <FactoryTools/SelectDileptonicWWEvents.h>
#include <FactoryTools/PostselectDileptonicWWEvents.h>

#include <FactoryTools/PreselectTwoLeptonEvents.h>
#include <FactoryTools/SelectTwoLeptonEvents.h>
#include <FactoryTools/PostselectTwoLeptonEvents.h>

//#include <FactoryTools/PreselectDileptonicWWEvents.h>
#include <FactoryTools/SelectZeroLeptonEvents.h>
//#include <FactoryTools/PostselectDileptonicWWEvents.h>


#include <FactoryTools/PreselectDiFatJetEvents.h>
#include <FactoryTools/SelectDiFatJetEvents.h>

#include <FactoryTools/SelectNixonResolvedEvents.h>

#include <FactoryTools/RegionVarCalculator.h>
#include <FactoryTools/RegionVarCalculator_lvlv.h>
#include <FactoryTools/RegionVarCalculator_zl.h>
#include <FactoryTools/RegionVarCalculator_tls.h>
#include <FactoryTools/RegionVarCalculator_b4j.h>
#include <FactoryTools/RegionVarCalculator_nr.h>

#include <FactoryTools/RJigsawCalculator.h>
#include <FactoryTools/RJigsawCalculator_compressed.h>
#include <FactoryTools/RJigsawCalculator_lvlv.h>
#include <FactoryTools/RJigsawCalculator_zl.h>
#include <FactoryTools/RJigsawCalculator_tls.h>

#include <FactoryTools/WriteOutputNtuple.h>
#include <FactoryTools/CalibrateST.h>
#include <FactoryTools/MCEventVeto.h>
#include <FactoryTools/MCEventVetoHelper.h>

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ nestedclass;

#endif

#ifdef __CINT__
#pragma link C++ class CalibrateST+;
#endif

#ifdef __CINT__
#pragma link C++ class MCEventVeto+;
#endif

#ifdef __CINT__
#pragma link C++ class MCEventVetoHelper+;
#endif


#ifdef __CINT__
#pragma link C++ class RJigsawCalculator+;
#endif

#ifdef __CINT__
#pragma link C++ class RJigsawCalculator_lvlv+;
#endif

#ifdef __CINT__
#pragma link C++ class RJigsawCalculator_compressed+;
#endif

#ifdef __CINT__
#pragma link C++ class RJigsawCalculator_zl+;
#endif

#ifdef __CINT__
#pragma link C++ class RJigsawCalculator_tls+;
#endif

#ifdef __CINT__
#pragma link C++ class RegionVarCalculator+;
#endif

#ifdef __CINT__
#pragma link C++ class RegionVarCalculator_lvlv+;
#endif

#ifdef __CINT__
#pragma link C++ class RegionVarCalculator_zl+;
#endif

#ifdef __CINT__
#pragma link C++ class RegionVarCalculator_tls+;
#endif

#ifdef __CINT__
#pragma link C++ class RegionVarCalculator_b4j+;
#endif

#ifdef __CINT__
#pragma link C++ class RegionVarCalculator_nr+;
#endif

#ifdef __CINT__
#pragma link C++ class SelectZeroLeptonEvents+;
#endif

#ifdef __CINT__
#pragma link C++ class PreselectDileptonicWWEvents+;
#endif

#ifdef __CINT__
#pragma link C++ class SelectDileptonicWWEvents+;
#endif

#ifdef __CINT__
#pragma link C++ class PreselectDiFatJetEvents+;
#endif

#ifdef __CINT__
#pragma link C++ class SelectDiFatJetEvents+;
#endif

#ifdef __CINT__
#pragma link C++ class PostselectDileptonicWWEvents+;
#endif

#ifdef __CINT__
#pragma link C++ class PreselectTwoLeptonEvents+;
#endif

#ifdef __CINT__
#pragma link C++ class SelectTwoLeptonEvents+;
#endif

#ifdef __CINT__
#pragma link C++ class PostselectTwoLeptonEvents+;
#endif

#ifdef __CINT__
#pragma link C++ class CalculateRJigsawVariables+;
#endif

#ifdef __CINT__
#pragma link C++ class WriteOutputNtuple+;
#endif

#ifdef __CINT__
#pragma link C++ class CalculateRegionVars+;
#endif

#ifdef __CINT__
#pragma link C++ class SelectNixonResolvedEvents+;
#endif
