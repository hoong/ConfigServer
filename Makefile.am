bin_PROGRAMS = config_server

config_server_SOURCES = db_mysql.cpp \
	json_man.cpp \
	main.cpp \
	method_config_get.cpp \
	method_config_update.cpp \
	method_server_acquire.cpp \
	method_status_query.cpp \
	method_status_refresh.cpp \
	method_status_register.cpp \
	rpc/config_service_acceptor.cpp \
	rpc/config_service_handler.cpp \
	proto/config_server.pb.cc \
	service_config.cpp 

config_server_LDFLAGS = -L../service_engine -lservice_engine -L../base -L../third-party/jsoncpp/libs -lbase -ljson -lboost_log -lboost_thread  -lmysqlpp

config_server_CPPFLAGS = -I../service_engine -I../base -I../third-party/jsoncpp/include -I/usr/local/include -I/usr/include 



DEFS += -DMYSQLPP_MYSQL_HEADERS_BURIED 

