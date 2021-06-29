#!/usr/bin/env python3
import os
import os.path
if os.path.exists("./cmake/build"):
    print("./cmake/build exist")
else:
    print("create ./cmake/build ")
    os.system("mkdir -p cmake/build")
os.chdir("./cmake/build")
os.system("cmake -DCMAKE_PREFIX_PATH=$MY_INSTALL_DIR ../..")
os.system("make")