#!/usr/bin/env python3
import os
import os.path
cwd = os.getcwd()

os.system("./build.sh")
os.system("mkdir -p build/game")
os.system("mkdir -p build/lobby")
os.system("mkdir -p build/proxy")
os.system("mkdir -p build/social")
# # compile GameService
# os.system("mkdir -p cmake/build")
# os.system("pushd cmake/build")
# os.system("cmake -DCMAKE_PREFIX_PATH=$MY_INSTALL_DIR ../..")
# os.system("make -j")


