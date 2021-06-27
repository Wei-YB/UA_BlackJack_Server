## Build Procedure
1. Generate the necessary *.grpc.pb.h and *.pb.h files
```bash
# in UA_BlackJackServer/proxy/
mkdir -p protos/build
cd protos/build
cmake ..
make
```
2. Build the proxy module 
```bash
cd ../..
mkdir -p build
cd build
cmake ..
make
```
3. Build the unit tests
```bash
cd ../test
mkdir -p build
cd build
cmake ..
make
```