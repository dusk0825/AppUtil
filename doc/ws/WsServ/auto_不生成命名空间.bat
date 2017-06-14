CD ..
soapcpp2.exe -x -i -Iimport -dWsServ -p testService WsServ/test_service.h

CD WsServ
DEL testService.h
DEL testService.cpp
DEL testProxy.h
DEL testProxy.cpp

REN testServicetestServiceService.h testService.h
REN testServicetestServiceService.cpp testService.cpp
REN testServicetestServiceProxy.h testProxy.h
REN testServicetestServiceProxy.cpp testProxy.cpp

pause