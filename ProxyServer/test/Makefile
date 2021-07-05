all : testlibco overAllTest
.PHONY : all

VPATH = ../src ../include/grpc
INC = -I /include -I /usr/local/include/google/protobuf -I /usr/local/include/libco \
	  -I ../include/grpc -I ../include
LIB = -L /lib -L /usr/lib -L /usr/local/lib -L ../lib
LD = -pthread -lcolib -lprotobuf -ldl  -lgrpc++ -lgrpc++_reflection
SRC = overAllTest.cpp ../include/grpc/UA_BlackJack.pb.cc ../include/grpc/UA_BlackJack.grpc.pb.cc

overAllTest : overAllTest.cpp
	g++ -o overAllTest ${SRC} ${INC} ${LIB} ${LD} 

testlibco : testlibco.cpp
	g++ -o testlibco testlibco.cpp -I /usr/local/include/libco -L /usr/local/lib -L /lib -pthread -lcolib -ldl

testLogLevel : testLogLevel.cpp
	g++ -o testLogLevel testLogLevel.cpp -I /usr/local/include -L /usr/local/lib -lspdlog

.PHONY : clean
clean : 
	rm -f testlibco testLogLevel overAllTest *.o