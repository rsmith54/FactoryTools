## Getting Started

Once you have the RJigsawTools package with

```
git clone https://github.com/rsmith54/RJigsawTools.git
```

you'll still need a few dependencies. You'll need to get `Ext_RestFrames` and `CommonTools`. We are using a newer version of SUSYTools than is currently in the release, while depending on a package not in the release we are using yet (todo update when not true anymore).
First setup RootCore :

```
source RJigsawTools/util/setup.sh
```

Now we can checkout our dependencies using the following :

```
git clone https://github.com/lawrenceleejr/Ext_RestFrames.git; cd Ext_RestFrames; git checkout v1.6; git checkout -b v1.6; cd ..
rc checkout_pkg atlasoff/PhysicsAnalysis/SUSYPhys/SUSYTools/tags/SUSYTools-00-07-42 SUSYTools
rc checkout_pkg svn+ssh://svn.cern.ch/reps/atlasinst/Institutes/LBNL/AnalysisToolsRunII/CommonTools/tags/CommonTools-00-00-18 CommonTools
git clone https://github.com/UCATLAS/xAODAnaHelpers.git; cd xAODAnaHelpers; git checkout 686427881df80c5c7ab783a77c0628adee35b77a; git checkout -b localRJbranch; cd ..;
rc checkout_pkg atlasoff/Reconstruction/Jet/JetJvtEfficiency/tags/JetJvtEfficiency-00-00-03 JetJvtEfficiency
```

And of course do a normal rc set up procedure

```
rc find_packages
rc compile
```

Then you can run a test with e.g.:

```
python RJigsawTools/util/run_lvlv.py  --doOverwrite --nevents 10 --verbosity debug --inputDS /afs/cern.ch/work/l/larry/public/lvlv_datasets/
```

(This points to a public directory so this should run for anyone on afs.)

The inputDS option is smart.  You can give supply a local directory, txt file with a list of grid datasets, or a pattern which matches a grid pattern.  If you use the grid options, obviously you need panda stuff setup, which is setup by the setup script.

PLEASE NOTE : The output will be in
```
submit_dir/data-TEST_OUTPUTNAME/*.root
```
by default when doing a test, where TEST_OUTPUTNAME is set in your submission script.  The file in submit_dir does not have your output trees!!!


You can run on the grid with

```
python RJigsawTools/util/run_lvlv.py  --doOverwrite --driver grid --inputDS RJigsawTools/data/mc15_13TeV_25ns_SM_p2419.ds --gridTag 012516_lvlv
```

This default assumes your system username is the same as your grid/NICE name. If not, you can change it with --gridUser, and the --gridTag defaults to just the date.

## Merging Step

Once you've run on the grid, download the tree output and the metadata output to some directory. Then you can use the new merging script to combine them with a new branch added that incorporates the cross section weighting. Simply run

```
python mergeOutput.py --inDir [path to datasets from grid]
```

And it will produce combined files containing the trees. One just needs to weight by the branch normweight. The assignment of dataset types is done in discoverInput.py where tags are added to samples based on their names. Then all of one kind of process are combined.
