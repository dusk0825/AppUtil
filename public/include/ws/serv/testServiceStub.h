/* testServiceStub.h
   Generated by gSOAP 2.8.8 from WsServ/test_service.h

Copyright(C) 2000-2012, Robert van Engelen, Genivia Inc. All Rights Reserved.
The generated code is released under one of the following licenses:
1) GPL or 2) Genivia's license for commercial use.
This program is released under the GPL with the additional exemption that
compiling, linking, and/or using OpenSSL is allowed.
*/

#ifndef testServiceStub_H
#define testServiceStub_H
#include <string>
#include <vector>
#include <vector>
#ifndef WITH_NOGLOBAL
#define WITH_NOGLOBAL
#endif
#include "ws/stdsoap2.h"
#if GSOAP_VERSION != 20808
# error "GSOAP VERSION MISMATCH IN GENERATED CODE: PLEASE REINSTALL PACKAGE"
#endif


namespace testService {

/******************************************************************************\
 *                                                                            *
 * Enumerations                                                               *
 *                                                                            *
\******************************************************************************/


/******************************************************************************\
 *                                                                            *
 * Types with Custom Serializers                                              *
 *                                                                            *
\******************************************************************************/


/******************************************************************************\
 *                                                                            *
 * Classes and Structs                                                        *
 *                                                                            *
\******************************************************************************/


#if 0 /* volatile type: do not declare here, declared elsewhere */

#endif

#if 0 /* volatile type: do not declare here, declared elsewhere */

#endif

#ifndef SOAP_TYPE_testService_test__ResultInfo
#define SOAP_TYPE_testService_test__ResultInfo (8)
/* test:ResultInfo */
class SOAP_CMAC test__ResultInfo
{
public:
	int resultCode;	/* required element of type xsd:int */
	std::string resultString;	/* required element of type xsd:string */
public:
	virtual int soap_type() const { return 8; } /* = unique id SOAP_TYPE_testService_test__ResultInfo */
	virtual void soap_default(struct soap*);
	virtual void soap_serialize(struct soap*) const;
	virtual int soap_put(struct soap*, const char*, const char*) const;
	virtual int soap_out(struct soap*, const char*, int, const char*) const;
	virtual void *soap_get(struct soap*, const char*, const char*);
	virtual void *soap_in(struct soap*, const char*, const char*);
	         test__ResultInfo() { test__ResultInfo::soap_default(NULL); }
	virtual ~test__ResultInfo() { }
};
#endif

#ifndef SOAP_TYPE_testService_test__ConfigReq
#define SOAP_TYPE_testService_test__ConfigReq (10)
/* test:ConfigReq */
class SOAP_CMAC test__ConfigReq
{
public:
	std::string configXml;	/* required element of type xsd:string */
public:
	virtual int soap_type() const { return 10; } /* = unique id SOAP_TYPE_testService_test__ConfigReq */
	virtual void soap_default(struct soap*);
	virtual void soap_serialize(struct soap*) const;
	virtual int soap_put(struct soap*, const char*, const char*) const;
	virtual int soap_out(struct soap*, const char*, int, const char*) const;
	virtual void *soap_get(struct soap*, const char*, const char*);
	virtual void *soap_in(struct soap*, const char*, const char*);
	         test__ConfigReq() { test__ConfigReq::soap_default(NULL); }
	virtual ~test__ConfigReq() { }
};
#endif

#ifndef SOAP_TYPE_testService_test__ConfigRsp
#define SOAP_TYPE_testService_test__ConfigRsp (11)
/* test:ConfigRsp */
class SOAP_CMAC test__ConfigRsp
{
public:
	test__ResultInfo resultInfo;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* required element of type test:ResultInfo */
public:
	virtual int soap_type() const { return 11; } /* = unique id SOAP_TYPE_testService_test__ConfigRsp */
	virtual void soap_default(struct soap*);
	virtual void soap_serialize(struct soap*) const;
	virtual int soap_put(struct soap*, const char*, const char*) const;
	virtual int soap_out(struct soap*, const char*, int, const char*) const;
	virtual void *soap_get(struct soap*, const char*, const char*);
	virtual void *soap_in(struct soap*, const char*, const char*);
	         test__ConfigRsp() { test__ConfigRsp::soap_default(NULL); }
	virtual ~test__ConfigRsp() { }
};
#endif

#ifndef SOAP_TYPE_testService_test__RemoteConfig
#define SOAP_TYPE_testService_test__RemoteConfig (14)
/* test:RemoteConfig */
struct test__RemoteConfig
{
public:
	test__ConfigReq req;	/* required element of type test:ConfigReq */
};
#endif

#ifndef WITH_NOGLOBAL

#ifndef SOAP_TYPE_testService_SOAP_ENV__Header
#define SOAP_TYPE_testService_SOAP_ENV__Header (15)
/* SOAP Header: */
struct SOAP_ENV__Header
{
#ifdef WITH_NOEMPTYSTRUCT
private:
	char dummy;	/* dummy member to enable compilation */
#endif
};
#endif

#endif

#ifndef WITH_NOGLOBAL

#ifndef SOAP_TYPE_testService_SOAP_ENV__Code
#define SOAP_TYPE_testService_SOAP_ENV__Code (16)
/* SOAP Fault Code: */
struct SOAP_ENV__Code
{
public:
	char *SOAP_ENV__Value;	/* optional element of type xsd:QName */
	struct SOAP_ENV__Code *SOAP_ENV__Subcode;	/* optional element of type SOAP-ENV:Code */
};
#endif

#endif

#ifndef WITH_NOGLOBAL

#ifndef SOAP_TYPE_testService_SOAP_ENV__Detail
#define SOAP_TYPE_testService_SOAP_ENV__Detail (18)
/* SOAP-ENV:Detail */
struct SOAP_ENV__Detail
{
public:
	char *__any;
	int __type;	/* any type of element <fault> (defined below) */
	void *fault;	/* transient */
};
#endif

#endif

#ifndef WITH_NOGLOBAL

#ifndef SOAP_TYPE_testService_SOAP_ENV__Reason
#define SOAP_TYPE_testService_SOAP_ENV__Reason (21)
/* SOAP-ENV:Reason */
struct SOAP_ENV__Reason
{
public:
	char *SOAP_ENV__Text;	/* optional element of type xsd:string */
};
#endif

#endif

#ifndef WITH_NOGLOBAL

#ifndef SOAP_TYPE_testService_SOAP_ENV__Fault
#define SOAP_TYPE_testService_SOAP_ENV__Fault (22)
/* SOAP Fault: */
struct SOAP_ENV__Fault
{
public:
	char *faultcode;	/* optional element of type xsd:QName */
	char *faultstring;	/* optional element of type xsd:string */
	char *faultactor;	/* optional element of type xsd:string */
	struct SOAP_ENV__Detail *detail;	/* optional element of type SOAP-ENV:Detail */
	struct SOAP_ENV__Code *SOAP_ENV__Code;	/* optional element of type SOAP-ENV:Code */
	struct SOAP_ENV__Reason *SOAP_ENV__Reason;	/* optional element of type SOAP-ENV:Reason */
	char *SOAP_ENV__Node;	/* optional element of type xsd:string */
	char *SOAP_ENV__Role;	/* optional element of type xsd:string */
	struct SOAP_ENV__Detail *SOAP_ENV__Detail;	/* optional element of type SOAP-ENV:Detail */
};
#endif

#endif

/******************************************************************************\
 *                                                                            *
 * Typedefs                                                                   *
 *                                                                            *
\******************************************************************************/

#ifndef SOAP_TYPE_testService__QName
#define SOAP_TYPE_testService__QName (5)
typedef char *_QName;
#endif

#ifndef SOAP_TYPE_testService__XML
#define SOAP_TYPE_testService__XML (6)
typedef char *_XML;
#endif


/******************************************************************************\
 *                                                                            *
 * Externals                                                                  *
 *                                                                            *
\******************************************************************************/


} // namespace testService


#endif

/* End of testServiceStub.h */