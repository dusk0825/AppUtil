DEL vagClientSerivce.nsmap
CD ..
wsdl2h.exe -tWsClient/cms.typemap -o testClient.h WsClient/testService.wsdl
soapcpp2.exe -x -i -ptestClient -C -dWsClient testClient.h

pause

