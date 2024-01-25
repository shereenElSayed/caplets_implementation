This is an implementation for the Caplets paper: https://ieeexplore.ieee.org/document/9709087

# Installation
For Centos8: run install.sh

It is going to be installed a library to be added to your CMAKE projects
Once it is installed, just add these lines into your new project's CmakeLists.txt file

'''
set(caplets_DIR "/usr/local/lib/cmake/caplets")
find_package(caplets REQUIRED)
'''

Please note if you choose a different prefix for the installation, use it for caplets_DIR. The default installation is "/usr/local/lib/cmake/caplets"
