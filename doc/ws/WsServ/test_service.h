//File: test_service.h
//gsoap test service name: testService
//gsoap test service namespace: urn:testService
//gsoap test service location: http://localhost:6651/ws/testService

#include <string>
#include <vector>
#import "stlvector.h"

class test__ResultInfo
{
	int resultCode;
	std::string resultString;
};

class test__ConfigReq
{ 
    std::string configXml; 
};

class test__ConfigRsp
{
	test__ResultInfo resultInfo;
};

int test__RemoteConfig(test__ConfigReq req, test__ConfigRsp& rsp);

