#!/usr/bin/env python3
import os
import os.path
if os.path.exists("./cmake/build"):
    print("./cmake/build exist")
else:
    print("create ./cmake/build ")
    os.system("mkdir -p cmake/build")

if os.path.exists("./grpc_cpp"):
    print("./grpc_cpp exist")
else:
    print("create ./grpc_cpp ")
    os.system("mkdir -p grpc_cpp")

if os.path.exists("./grpc_h"):
    print("./grpc_h exist")
else:
    print("create ./grpc_h ")
    os.system("mkdir -p grpc_h")    

if not os.path.exists("./grpc"):
    os.system("git clone --recurse-submodules -b v1.38.0 https://github.com/grpc/grpc")
os.chdir("./cmake/build")
# os.system("cmake -DCMAKE_PREFIX_PATH=$MY_INSTALL_DIR ../..")
os.system("cmake  -DGRPC_AS_SUBMODULE=ON ../..")
os.system("make")

os.system("mv ./UA_BlackJack.grpc.pb.cc ../../grpc_cpp")
os.system("mv ./UA_BlackJack.pb.cc ../../grpc_cpp")
os.system("mv ./UA_BlackJack.grpc.pb.h ../../grpc_h")
os.system("mv ./UA_BlackJack.pb.h ../../grpc_h")