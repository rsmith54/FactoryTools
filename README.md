## Getting Started

Once you have the RJigsawTools package with

```
git clone https://github.com/rsmith54/RJigsawTools.git
```

you'll still need a few dependencies. You'll need to get `Ext_RestFrames` and `CommonTools`.  `CommonTools` also depends on `QuickAna`.
First setup RootCore :

```
source RJigsawTools/util/setup.sh
```

Now we can checkout our dependencies using the following :

```
git clone https://github.com/lawrenceleejr/Ext_RestFrames.git
rc checkout_pkg atlasoff/PhysicsAnalysis/TopPhys/QuickAna/tags/QuickAna-00-00-94 QuickAna
rc checkout_pkg svn+ssh://svn.cern.ch/reps/atlasinst/Institutes/LBNL/AnalysisToolsRunII/CommonTools/tags/CommonTools-00-00-17 CommonTools
git clone https://github.com/UCATLAS/xAODAnaHelpers.git
```

And of course do a normal rc set up procedure

```
rc find_packages
rc compile
```

Then you can run a test with e.g.:

```
python RJigsawTools/util/run_lvlv.py  --doOverwrite --nevents 10 --verbosity debug --dataDir /afs/cern.ch/user/l/leejr/work/lvlv_datasets #[point to a location of a DxAOD]
```

PLEASE NOTE : The output will be in
```
submit_dir/data-TEST_OUTPUTNAME/*.root
```
by default when doing a test, where TEST_OUTPUTNAME is set in your submission script.  The file in submit_dir does not have your output trees!!!


You can run on the grid with

```
python RJigsawTools/util/run_lvlv.py  --doOverwrite --gridInputFile RJigsawTools/data/mc15_13TeV_25ns_SM_p2419.ds --gridTag 012516_lvlv
```

This default assumes your system username is the same as your grid/NICE name. If not, you can change it with --gridUser, and the --gridTag defaults to just the date. 



## Merging Step

Once you've run on the grid, download the tree output and the metadata output to some directory. Then you can use the new merging script to combine them with a new branch added that incorporates the cross section weighting. Simply run

```
python mergeOutput.py [path to datasets from grid]
```

And it will produce combined files containing the trees. One just needs to weight by the branch normweight. The assignment of dataset types is done in discoverInput.py where tags are added to samples based on their names. Then all of one kind of process are combined. 





