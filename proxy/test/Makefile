CXX = g++
CPPFLAGS += `pkg-config --cflags protobuf grpc`
CXXFLAGS += -std=c++11
LDFLAGS += -L/usr/local/lib `pkg-config --libs protobuf grpc++`\
			-pthread\
			-Wl,--no-as-needed -lgrpc++_reflection -Wl,--as-needed\
			-ldl 
PROTOC = protoc
PROTOS_PATH = ./include/protobuf
CPPS_PATH = ./src
GRPC_CPP_PLUGIN = grpc_cpp_plugin
GRPC_PLUGIN_PATH ?= `which $(GRPC_CPP_PLUGIN)`

INCPATH = /include/google/protobuf
LDPATH = /usr/local/lib

all : 

# proxy : proxy.o ./protobuf/client_proxy.pb.o
# 	g++ -o proxy proxy.o ./protobuf/client_proxy.pb.o -I $(INCPATH) -L $(LDPATH) -lprotobuf

client : client.o ../include/grpc/common.pb.o
	g++ -o client client.o ../include/grpc/common.pb.o -I $(INCPATH) -L $(LDPATH) -lprotobuf

../include/grpc/common.pb.o : ../include/grpc/common.pb.cc ../include/grpc/common.pb.h
	g++ -o ../include/grpc/common.pb.o -c ../include/grpc/common.pb.cc -I $(INCPATH) -L $(LDPATH) -lprotobuf

# %.grpc.pb.cc: %.proto
# 	$(PROTOC) -I $(PROTOS_PATH) --grpc_out=$(PROTOS_PATH) --plugin=protoc-gen-grpc=$(GRPC_PLUGIN_PATH) $<

# %.pb.cc: %.proto
# 	$(PROTOC) -I $(PROTOS_PATH) --cpp_out=$(CPPS_PATH) $<
	
# proxy.o : proxy.cpp
# 	g++ -c proxy.cpp -I $(INCPATH) -L $(LDPATH) -lprotobuf

client.o : client.cpp
	g++ -o client.o -c client.cpp -I $(INCPATH) -L $(LDPATH) -lprotobuf

.PHONY : clean
clean : 
	rm -rf client ../include/grpc/common.pb.o client.o