# setupATLAS # or equivalent on your machine

ls -l /cvmfs/atlas.cern.ch/repo
pwd
source ~/.bashrc  || echo ignore alrb
lsetup root || echo ignore alrb
rcSetup -r || echo ignore alrb
rcSetup Base,2.4.X,rel_0 || echo ignore alrb
  # - rc find_packages