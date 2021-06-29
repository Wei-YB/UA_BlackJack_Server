#!/usr/bin/env python3
import os
import os.path
os.chdir("./3rd/grpc")
os.system("./BuildProtobufToRpc.py")
os.chdir("../..")
if os.path.exists("./build"):
    print("./build exist")
else:
    print("create ./build ")
    os.system("mkdir -p build")
os.chdir("./build")
os.system("cmake ..")
os.system("make -j8")
os.system("cp ./Main ..")