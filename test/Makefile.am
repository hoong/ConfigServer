bin_PROGRAMS = config_test

config_test_SOURCES = ../json_man.cpp \
	main.cpp 

config_test_LDFLAGS = -L../../service_engine -lservice_engine -L../../base -L../../third-party/jsoncpp/libs -lbase -ljsoncpp -lboost_log -lboost_thread  -lmysqlpp

#INCLUDES = -I../../service_engine -I../../base -I../../third-party/jsoncpp/include -I/usr/local/include -I/usr/include 
config_test_CPPFLAGS= -I../../service_engine -I../../base -I../../third-party/jsoncpp/include -I/usr/local/include -I/usr/include

DEFS += -DMYSQLPP_MYSQL_HEADERS_BURIED 

