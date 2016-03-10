//author : Russell Smith
//date   : January 2015
//adapted from code from Nils Krumnack

#include <RJigsawTools/RegionVarCalculator_lvlv.h>
#include <EventLoop/Worker.h>
#include <xAODRootAccess/TStore.h>

#include <RootCoreUtils/Assert.h>
#include <TSystem.h>
#include <cstdlib>
#include <fstream>
#include <gtest/gtest.h>

#include <RJigsawTools/strongErrorCheck.h>

#include <map>
#include <xAODMissingET/MissingET.h>
#include <xAODJet/JetContainer.h>

#if 0

struct RegionVarCalculator_lvlv_Test : testing::Test
{
  static const std::string root;
  //  xAOD::TStore * store;
  EL::Worker * worker;

  virtual void SetUp ()
  {
    // gSystem->MakeDirectory ((root + "").c_str());
    // std::ofstream ((root + "/file.root_17").c_str());
    // std::ofstream ((root + "/ignore").c_str());
    // gSystem->MakeDirectory ((root + "/sample").c_str());
    // std::ofstream ((root + "/sample/samplefile.root").c_str());
    // gSystem->MakeDirectory ((root + "/deepsample").c_str());
    // gSystem->MakeDirectory ((root + "/deepsample/subdir").c_str());
    // std::ofstream ((root + "/deepsample/subdir/deepfile.root").c_str());
    worker = new EL::Worker();
  }

  virtual void TearDown ()
  {
    delete worker; worker = nullptr;
  }

};
const std::string RegionVarCalculator_lvlv_Test::root = "test1";

TEST_F (RegionVarCalculator_lvlv_Test, calcTest)
{

  RegionVarCalculator_lvlv calc;
  calc.initialize(worker );

  std::map<std::string, double> mymap;
  xAOD::JetContainer jets;
  xAOD::MissingET met;

  std::map<std::string, std::vector<double> > myVecMap;

  EXPECT_TRUE( calc.calculate(mymap, myVecMap) == EL::StatusCode::SUCCESS ) ;//the first event is fine no matter what
}

TEST_F (RegionVarCalculator_lvlv_Test, initializeTwiceTest)
{
  RegionVarCalculator_lvlv calc;
  EXPECT_TRUE (  calc.initialize(worker) == EL::StatusCode::SUCCESS );
  EXPECT_FALSE(  calc.initialize(worker) == EL::StatusCode::SUCCESS );//can't call this twice
}

int main (int argc, char **argv)
{
  ::testing::InitGoogleTest (&argc, argv);
  return RUN_ALL_TESTS();
}
#endif 0
