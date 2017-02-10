/* testClientH.h
   Generated by gSOAP 2.8.8 from testClient.h

Copyright(C) 2000-2012, Robert van Engelen, Genivia Inc. All Rights Reserved.
The generated code is released under one of the following licenses:
1) GPL or 2) Genivia's license for commercial use.
This program is released under the GPL with the additional exemption that
compiling, linking, and/or using OpenSSL is allowed.
*/

#ifndef testClientH_H
#define testClientH_H
#include "testClientStub.h"
#ifndef WITH_NOIDREF

#ifdef __cplusplus
extern "C" {
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_markelement(struct soap*, const void*, int);
SOAP_FMAC3 int SOAP_FMAC4 soap_putelement(struct soap*, const void*, const char*, int, int);
SOAP_FMAC3 void *SOAP_FMAC4 soap_getelement(struct soap*, int*);

#ifdef __cplusplus
}
#endif
SOAP_FMAC3 int SOAP_FMAC4 soap_putindependent(struct soap*);
SOAP_FMAC3 int SOAP_FMAC4 soap_getindependent(struct soap*);
#endif
SOAP_FMAC3 int SOAP_FMAC4 soap_ignore_element(struct soap*);

SOAP_FMAC3 void * SOAP_FMAC4 testClient_instantiate(struct soap*, int, const char*, const char*, size_t*);
SOAP_FMAC3 int SOAP_FMAC4 testClient_fdelete(struct soap_clist*);
SOAP_FMAC3 void* SOAP_FMAC4 soap_class_id_enter(struct soap*, const char*, void*, int, size_t, const char*, const char*);

SOAP_FMAC3 void* SOAP_FMAC4 soap_container_id_forward(struct soap*, const char*, void*, size_t, int, int, size_t, unsigned int);

SOAP_FMAC3 void SOAP_FMAC4 testClient_container_insert(struct soap*, int, int, void*, size_t, const void*, size_t);

#ifndef SOAP_TYPE_byte
#define SOAP_TYPE_byte (3)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default_byte(struct soap*, char *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_byte(struct soap*, const char*, int, const char *, const char*);
SOAP_FMAC3 char * SOAP_FMAC4 soap_in_byte(struct soap*, const char*, char *, const char*);

#ifndef soap_write_byte
#define soap_write_byte(soap, data) ( soap_begin_send(soap) || soap_put_byte(soap, data, "byte", NULL) || soap_end_send(soap) )
#endif


SOAP_FMAC3 int SOAP_FMAC4 soap_put_byte(struct soap*, const char *, const char*, const char*);

#ifndef soap_read_byte
#define soap_read_byte(soap, data) ( soap_begin_recv(soap) || !soap_get_byte(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 char * SOAP_FMAC4 soap_get_byte(struct soap*, char *, const char*, const char*);

#ifndef SOAP_TYPE_int
#define SOAP_TYPE_int (1)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default_int(struct soap*, int *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_int(struct soap*, const char*, int, const int *, const char*);
SOAP_FMAC3 int * SOAP_FMAC4 soap_in_int(struct soap*, const char*, int *, const char*);

#ifndef soap_write_int
#define soap_write_int(soap, data) ( soap_begin_send(soap) || soap_put_int(soap, data, "int", NULL) || soap_end_send(soap) )
#endif


SOAP_FMAC3 int SOAP_FMAC4 soap_put_int(struct soap*, const int *, const char*, const char*);

#ifndef soap_read_int
#define soap_read_int(soap, data) ( soap_begin_recv(soap) || !soap_get_int(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 int * SOAP_FMAC4 soap_get_int(struct soap*, int *, const char*, const char*);

#ifndef SOAP_TYPE_std__string
#define SOAP_TYPE_std__string (12)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default_std__string(struct soap*, std::string *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_std__string(struct soap*, const std::string *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_std__string(struct soap*, const char*, int, const std::string*, const char*);
SOAP_FMAC3 std::string * SOAP_FMAC4 soap_in_std__string(struct soap*, const char*, std::string*, const char*);

#ifndef soap_write_std__string
#define soap_write_std__string(soap, data) ( soap_begin_send(soap) || ((data)->soap_serialize(soap), 0) || (data)->soap_put(soap, "string", NULL) || soap_end_send(soap) )
#endif


SOAP_FMAC3 int SOAP_FMAC4 soap_put_std__string(struct soap*, const std::string *, const char*, const char*);

#ifndef soap_read_std__string
#define soap_read_std__string(soap, data) ( soap_begin_recv(soap) || !soap_get_std__string(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 std::string * SOAP_FMAC4 soap_get_std__string(struct soap*, std::string *, const char*, const char*);

#define soap_new_std__string(soap, n) soap_instantiate_std__string(soap, n, NULL, NULL, NULL)


#define soap_delete_std__string(soap, p) soap_delete(soap, p)

SOAP_FMAC1 std::string * SOAP_FMAC2 soap_instantiate_std__string(struct soap*, int, const char*, const char*, size_t*);
SOAP_FMAC3 void SOAP_FMAC4 soap_copy_std__string(struct soap*, int, int, void*, size_t, const void*, size_t);

#ifndef SOAP_TYPE__ns1__ConfigRsp
#define SOAP_TYPE__ns1__ConfigRsp (11)
#endif

SOAP_FMAC3 int SOAP_FMAC4 soap_out__ns1__ConfigRsp(struct soap*, const char*, int, const _ns1__ConfigRsp *, const char*);
SOAP_FMAC3 _ns1__ConfigRsp * SOAP_FMAC4 soap_in__ns1__ConfigRsp(struct soap*, const char*, _ns1__ConfigRsp *, const char*);

#ifndef soap_write__ns1__ConfigRsp
#define soap_write__ns1__ConfigRsp(soap, data) ( soap_begin_send(soap) || ((data)->soap_serialize(soap), 0) || (data)->soap_put(soap, "ns1:ConfigRsp", NULL) || soap_end_send(soap) )
#endif


#ifndef soap_read__ns1__ConfigRsp
#define soap_read__ns1__ConfigRsp(soap, data) ( soap_begin_recv(soap) || !soap_get__ns1__ConfigRsp(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 _ns1__ConfigRsp * SOAP_FMAC4 soap_get__ns1__ConfigRsp(struct soap*, _ns1__ConfigRsp *, const char*, const char*);

#define soap_new__ns1__ConfigRsp(soap, n) soap_instantiate__ns1__ConfigRsp(soap, n, NULL, NULL, NULL)


#define soap_delete__ns1__ConfigRsp(soap, p) soap_delete(soap, p)

SOAP_FMAC1 _ns1__ConfigRsp * SOAP_FMAC2 soap_instantiate__ns1__ConfigRsp(struct soap*, int, const char*, const char*, size_t*);
SOAP_FMAC3 void SOAP_FMAC4 soap_copy__ns1__ConfigRsp(struct soap*, int, int, void*, size_t, const void*, size_t);

#ifndef SOAP_TYPE__ns1__RemoteConfig
#define SOAP_TYPE__ns1__RemoteConfig (10)
#endif

SOAP_FMAC3 int SOAP_FMAC4 soap_out__ns1__RemoteConfig(struct soap*, const char*, int, const _ns1__RemoteConfig *, const char*);
SOAP_FMAC3 _ns1__RemoteConfig * SOAP_FMAC4 soap_in__ns1__RemoteConfig(struct soap*, const char*, _ns1__RemoteConfig *, const char*);

#ifndef soap_write__ns1__RemoteConfig
#define soap_write__ns1__RemoteConfig(soap, data) ( soap_begin_send(soap) || ((data)->soap_serialize(soap), 0) || (data)->soap_put(soap, "ns1:RemoteConfig", NULL) || soap_end_send(soap) )
#endif


#ifndef soap_read__ns1__RemoteConfig
#define soap_read__ns1__RemoteConfig(soap, data) ( soap_begin_recv(soap) || !soap_get__ns1__RemoteConfig(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 _ns1__RemoteConfig * SOAP_FMAC4 soap_get__ns1__RemoteConfig(struct soap*, _ns1__RemoteConfig *, const char*, const char*);

#define soap_new__ns1__RemoteConfig(soap, n) soap_instantiate__ns1__RemoteConfig(soap, n, NULL, NULL, NULL)


#define soap_delete__ns1__RemoteConfig(soap, p) soap_delete(soap, p)

SOAP_FMAC1 _ns1__RemoteConfig * SOAP_FMAC2 soap_instantiate__ns1__RemoteConfig(struct soap*, int, const char*, const char*, size_t*);
SOAP_FMAC3 void SOAP_FMAC4 soap_copy__ns1__RemoteConfig(struct soap*, int, int, void*, size_t, const void*, size_t);

#ifndef SOAP_TYPE_ns1__ConfigReq
#define SOAP_TYPE_ns1__ConfigReq (9)
#endif

SOAP_FMAC3 int SOAP_FMAC4 soap_out_ns1__ConfigReq(struct soap*, const char*, int, const ns1__ConfigReq *, const char*);
SOAP_FMAC3 ns1__ConfigReq * SOAP_FMAC4 soap_in_ns1__ConfigReq(struct soap*, const char*, ns1__ConfigReq *, const char*);

#ifndef soap_write_ns1__ConfigReq
#define soap_write_ns1__ConfigReq(soap, data) ( soap_begin_send(soap) || ((data)->soap_serialize(soap), 0) || (data)->soap_put(soap, "ns1:ConfigReq", NULL) || soap_end_send(soap) )
#endif


#ifndef soap_read_ns1__ConfigReq
#define soap_read_ns1__ConfigReq(soap, data) ( soap_begin_recv(soap) || !soap_get_ns1__ConfigReq(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 ns1__ConfigReq * SOAP_FMAC4 soap_get_ns1__ConfigReq(struct soap*, ns1__ConfigReq *, const char*, const char*);

#define soap_new_ns1__ConfigReq(soap, n) soap_instantiate_ns1__ConfigReq(soap, n, NULL, NULL, NULL)


#define soap_delete_ns1__ConfigReq(soap, p) soap_delete(soap, p)

SOAP_FMAC1 ns1__ConfigReq * SOAP_FMAC2 soap_instantiate_ns1__ConfigReq(struct soap*, int, const char*, const char*, size_t*);
SOAP_FMAC3 void SOAP_FMAC4 soap_copy_ns1__ConfigReq(struct soap*, int, int, void*, size_t, const void*, size_t);

#ifndef SOAP_TYPE_ns1__ResultInfo
#define SOAP_TYPE_ns1__ResultInfo (8)
#endif

SOAP_FMAC3 int SOAP_FMAC4 soap_out_ns1__ResultInfo(struct soap*, const char*, int, const ns1__ResultInfo *, const char*);
SOAP_FMAC3 ns1__ResultInfo * SOAP_FMAC4 soap_in_ns1__ResultInfo(struct soap*, const char*, ns1__ResultInfo *, const char*);

#ifndef soap_write_ns1__ResultInfo
#define soap_write_ns1__ResultInfo(soap, data) ( soap_begin_send(soap) || ((data)->soap_serialize(soap), 0) || (data)->soap_put(soap, "ns1:ResultInfo", NULL) || soap_end_send(soap) )
#endif


#ifndef soap_read_ns1__ResultInfo
#define soap_read_ns1__ResultInfo(soap, data) ( soap_begin_recv(soap) || !soap_get_ns1__ResultInfo(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 ns1__ResultInfo * SOAP_FMAC4 soap_get_ns1__ResultInfo(struct soap*, ns1__ResultInfo *, const char*, const char*);

#define soap_new_ns1__ResultInfo(soap, n) soap_instantiate_ns1__ResultInfo(soap, n, NULL, NULL, NULL)


#define soap_delete_ns1__ResultInfo(soap, p) soap_delete(soap, p)

SOAP_FMAC1 ns1__ResultInfo * SOAP_FMAC2 soap_instantiate_ns1__ResultInfo(struct soap*, int, const char*, const char*, size_t*);
SOAP_FMAC3 void SOAP_FMAC4 soap_copy_ns1__ResultInfo(struct soap*, int, int, void*, size_t, const void*, size_t);

#ifndef WITH_NOGLOBAL

#ifndef SOAP_TYPE_SOAP_ENV__Fault
#define SOAP_TYPE_SOAP_ENV__Fault (27)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default_SOAP_ENV__Fault(struct soap*, struct SOAP_ENV__Fault *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_SOAP_ENV__Fault(struct soap*, const struct SOAP_ENV__Fault *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_SOAP_ENV__Fault(struct soap*, const char*, int, const struct SOAP_ENV__Fault *, const char*);
SOAP_FMAC3 struct SOAP_ENV__Fault * SOAP_FMAC4 soap_in_SOAP_ENV__Fault(struct soap*, const char*, struct SOAP_ENV__Fault *, const char*);

#ifndef soap_write_SOAP_ENV__Fault
#define soap_write_SOAP_ENV__Fault(soap, data) ( soap_begin_send(soap) || (soap_serialize_SOAP_ENV__Fault(soap, data), 0) || soap_put_SOAP_ENV__Fault(soap, data, "SOAP-ENV:Fault", NULL) || soap_end_send(soap) )
#endif


SOAP_FMAC3 int SOAP_FMAC4 soap_put_SOAP_ENV__Fault(struct soap*, const struct SOAP_ENV__Fault *, const char*, const char*);

#ifndef soap_read_SOAP_ENV__Fault
#define soap_read_SOAP_ENV__Fault(soap, data) ( soap_begin_recv(soap) || !soap_get_SOAP_ENV__Fault(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct SOAP_ENV__Fault * SOAP_FMAC4 soap_get_SOAP_ENV__Fault(struct soap*, struct SOAP_ENV__Fault *, const char*, const char*);

#define soap_new_SOAP_ENV__Fault(soap, n) soap_instantiate_SOAP_ENV__Fault(soap, n, NULL, NULL, NULL)


#define soap_delete_SOAP_ENV__Fault(soap, p) soap_delete(soap, p)

SOAP_FMAC1 struct SOAP_ENV__Fault * SOAP_FMAC2 soap_instantiate_SOAP_ENV__Fault(struct soap*, int, const char*, const char*, size_t*);
SOAP_FMAC3 void SOAP_FMAC4 soap_copy_SOAP_ENV__Fault(struct soap*, int, int, void*, size_t, const void*, size_t);

#endif

#ifndef WITH_NOGLOBAL

#ifndef SOAP_TYPE_SOAP_ENV__Reason
#define SOAP_TYPE_SOAP_ENV__Reason (26)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default_SOAP_ENV__Reason(struct soap*, struct SOAP_ENV__Reason *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_SOAP_ENV__Reason(struct soap*, const struct SOAP_ENV__Reason *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_SOAP_ENV__Reason(struct soap*, const char*, int, const struct SOAP_ENV__Reason *, const char*);
SOAP_FMAC3 struct SOAP_ENV__Reason * SOAP_FMAC4 soap_in_SOAP_ENV__Reason(struct soap*, const char*, struct SOAP_ENV__Reason *, const char*);

#ifndef soap_write_SOAP_ENV__Reason
#define soap_write_SOAP_ENV__Reason(soap, data) ( soap_begin_send(soap) || (soap_serialize_SOAP_ENV__Reason(soap, data), 0) || soap_put_SOAP_ENV__Reason(soap, data, "SOAP-ENV:Reason", NULL) || soap_end_send(soap) )
#endif


SOAP_FMAC3 int SOAP_FMAC4 soap_put_SOAP_ENV__Reason(struct soap*, const struct SOAP_ENV__Reason *, const char*, const char*);

#ifndef soap_read_SOAP_ENV__Reason
#define soap_read_SOAP_ENV__Reason(soap, data) ( soap_begin_recv(soap) || !soap_get_SOAP_ENV__Reason(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct SOAP_ENV__Reason * SOAP_FMAC4 soap_get_SOAP_ENV__Reason(struct soap*, struct SOAP_ENV__Reason *, const char*, const char*);

#define soap_new_SOAP_ENV__Reason(soap, n) soap_instantiate_SOAP_ENV__Reason(soap, n, NULL, NULL, NULL)


#define soap_delete_SOAP_ENV__Reason(soap, p) soap_delete(soap, p)

SOAP_FMAC1 struct SOAP_ENV__Reason * SOAP_FMAC2 soap_instantiate_SOAP_ENV__Reason(struct soap*, int, const char*, const char*, size_t*);
SOAP_FMAC3 void SOAP_FMAC4 soap_copy_SOAP_ENV__Reason(struct soap*, int, int, void*, size_t, const void*, size_t);

#endif

#ifndef WITH_NOGLOBAL

#ifndef SOAP_TYPE_SOAP_ENV__Detail
#define SOAP_TYPE_SOAP_ENV__Detail (23)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default_SOAP_ENV__Detail(struct soap*, struct SOAP_ENV__Detail *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_SOAP_ENV__Detail(struct soap*, const struct SOAP_ENV__Detail *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_SOAP_ENV__Detail(struct soap*, const char*, int, const struct SOAP_ENV__Detail *, const char*);
SOAP_FMAC3 struct SOAP_ENV__Detail * SOAP_FMAC4 soap_in_SOAP_ENV__Detail(struct soap*, const char*, struct SOAP_ENV__Detail *, const char*);

#ifndef soap_write_SOAP_ENV__Detail
#define soap_write_SOAP_ENV__Detail(soap, data) ( soap_begin_send(soap) || (soap_serialize_SOAP_ENV__Detail(soap, data), 0) || soap_put_SOAP_ENV__Detail(soap, data, "SOAP-ENV:Detail", NULL) || soap_end_send(soap) )
#endif


SOAP_FMAC3 int SOAP_FMAC4 soap_put_SOAP_ENV__Detail(struct soap*, const struct SOAP_ENV__Detail *, const char*, const char*);

#ifndef soap_read_SOAP_ENV__Detail
#define soap_read_SOAP_ENV__Detail(soap, data) ( soap_begin_recv(soap) || !soap_get_SOAP_ENV__Detail(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct SOAP_ENV__Detail * SOAP_FMAC4 soap_get_SOAP_ENV__Detail(struct soap*, struct SOAP_ENV__Detail *, const char*, const char*);

#define soap_new_SOAP_ENV__Detail(soap, n) soap_instantiate_SOAP_ENV__Detail(soap, n, NULL, NULL, NULL)


#define soap_delete_SOAP_ENV__Detail(soap, p) soap_delete(soap, p)

SOAP_FMAC1 struct SOAP_ENV__Detail * SOAP_FMAC2 soap_instantiate_SOAP_ENV__Detail(struct soap*, int, const char*, const char*, size_t*);
SOAP_FMAC3 void SOAP_FMAC4 soap_copy_SOAP_ENV__Detail(struct soap*, int, int, void*, size_t, const void*, size_t);

#endif

#ifndef WITH_NOGLOBAL

#ifndef SOAP_TYPE_SOAP_ENV__Code
#define SOAP_TYPE_SOAP_ENV__Code (21)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default_SOAP_ENV__Code(struct soap*, struct SOAP_ENV__Code *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_SOAP_ENV__Code(struct soap*, const struct SOAP_ENV__Code *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_SOAP_ENV__Code(struct soap*, const char*, int, const struct SOAP_ENV__Code *, const char*);
SOAP_FMAC3 struct SOAP_ENV__Code * SOAP_FMAC4 soap_in_SOAP_ENV__Code(struct soap*, const char*, struct SOAP_ENV__Code *, const char*);

#ifndef soap_write_SOAP_ENV__Code
#define soap_write_SOAP_ENV__Code(soap, data) ( soap_begin_send(soap) || (soap_serialize_SOAP_ENV__Code(soap, data), 0) || soap_put_SOAP_ENV__Code(soap, data, "SOAP-ENV:Code", NULL) || soap_end_send(soap) )
#endif


SOAP_FMAC3 int SOAP_FMAC4 soap_put_SOAP_ENV__Code(struct soap*, const struct SOAP_ENV__Code *, const char*, const char*);

#ifndef soap_read_SOAP_ENV__Code
#define soap_read_SOAP_ENV__Code(soap, data) ( soap_begin_recv(soap) || !soap_get_SOAP_ENV__Code(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct SOAP_ENV__Code * SOAP_FMAC4 soap_get_SOAP_ENV__Code(struct soap*, struct SOAP_ENV__Code *, const char*, const char*);

#define soap_new_SOAP_ENV__Code(soap, n) soap_instantiate_SOAP_ENV__Code(soap, n, NULL, NULL, NULL)


#define soap_delete_SOAP_ENV__Code(soap, p) soap_delete(soap, p)

SOAP_FMAC1 struct SOAP_ENV__Code * SOAP_FMAC2 soap_instantiate_SOAP_ENV__Code(struct soap*, int, const char*, const char*, size_t*);
SOAP_FMAC3 void SOAP_FMAC4 soap_copy_SOAP_ENV__Code(struct soap*, int, int, void*, size_t, const void*, size_t);

#endif

#ifndef WITH_NOGLOBAL

#ifndef SOAP_TYPE_SOAP_ENV__Header
#define SOAP_TYPE_SOAP_ENV__Header (20)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default_SOAP_ENV__Header(struct soap*, struct SOAP_ENV__Header *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_SOAP_ENV__Header(struct soap*, const struct SOAP_ENV__Header *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_SOAP_ENV__Header(struct soap*, const char*, int, const struct SOAP_ENV__Header *, const char*);
SOAP_FMAC3 struct SOAP_ENV__Header * SOAP_FMAC4 soap_in_SOAP_ENV__Header(struct soap*, const char*, struct SOAP_ENV__Header *, const char*);

#ifndef soap_write_SOAP_ENV__Header
#define soap_write_SOAP_ENV__Header(soap, data) ( soap_begin_send(soap) || (soap_serialize_SOAP_ENV__Header(soap, data), 0) || soap_put_SOAP_ENV__Header(soap, data, "SOAP-ENV:Header", NULL) || soap_end_send(soap) )
#endif


SOAP_FMAC3 int SOAP_FMAC4 soap_put_SOAP_ENV__Header(struct soap*, const struct SOAP_ENV__Header *, const char*, const char*);

#ifndef soap_read_SOAP_ENV__Header
#define soap_read_SOAP_ENV__Header(soap, data) ( soap_begin_recv(soap) || !soap_get_SOAP_ENV__Header(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct SOAP_ENV__Header * SOAP_FMAC4 soap_get_SOAP_ENV__Header(struct soap*, struct SOAP_ENV__Header *, const char*, const char*);

#define soap_new_SOAP_ENV__Header(soap, n) soap_instantiate_SOAP_ENV__Header(soap, n, NULL, NULL, NULL)


#define soap_delete_SOAP_ENV__Header(soap, p) soap_delete(soap, p)

SOAP_FMAC1 struct SOAP_ENV__Header * SOAP_FMAC2 soap_instantiate_SOAP_ENV__Header(struct soap*, int, const char*, const char*, size_t*);
SOAP_FMAC3 void SOAP_FMAC4 soap_copy_SOAP_ENV__Header(struct soap*, int, int, void*, size_t, const void*, size_t);

#endif

#ifndef SOAP_TYPE___ns1__RemoteConfig
#define SOAP_TYPE___ns1__RemoteConfig (19)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default___ns1__RemoteConfig(struct soap*, struct __ns1__RemoteConfig *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize___ns1__RemoteConfig(struct soap*, const struct __ns1__RemoteConfig *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out___ns1__RemoteConfig(struct soap*, const char*, int, const struct __ns1__RemoteConfig *, const char*);
SOAP_FMAC3 struct __ns1__RemoteConfig * SOAP_FMAC4 soap_in___ns1__RemoteConfig(struct soap*, const char*, struct __ns1__RemoteConfig *, const char*);

#ifndef soap_write___ns1__RemoteConfig
#define soap_write___ns1__RemoteConfig(soap, data) ( soap_begin_send(soap) || (soap_serialize___ns1__RemoteConfig(soap, data), 0) || soap_put___ns1__RemoteConfig(soap, data, "-ns1:RemoteConfig", NULL) || soap_end_send(soap) )
#endif


SOAP_FMAC3 int SOAP_FMAC4 soap_put___ns1__RemoteConfig(struct soap*, const struct __ns1__RemoteConfig *, const char*, const char*);

#ifndef soap_read___ns1__RemoteConfig
#define soap_read___ns1__RemoteConfig(soap, data) ( soap_begin_recv(soap) || !soap_get___ns1__RemoteConfig(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct __ns1__RemoteConfig * SOAP_FMAC4 soap_get___ns1__RemoteConfig(struct soap*, struct __ns1__RemoteConfig *, const char*, const char*);

#define soap_new___ns1__RemoteConfig(soap, n) soap_instantiate___ns1__RemoteConfig(soap, n, NULL, NULL, NULL)


#define soap_delete___ns1__RemoteConfig(soap, p) soap_delete(soap, p)

SOAP_FMAC1 struct __ns1__RemoteConfig * SOAP_FMAC2 soap_instantiate___ns1__RemoteConfig(struct soap*, int, const char*, const char*, size_t*);
SOAP_FMAC3 void SOAP_FMAC4 soap_copy___ns1__RemoteConfig(struct soap*, int, int, void*, size_t, const void*, size_t);

#ifndef WITH_NOGLOBAL

#ifndef SOAP_TYPE_PointerToSOAP_ENV__Reason
#define SOAP_TYPE_PointerToSOAP_ENV__Reason (29)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerToSOAP_ENV__Reason(struct soap*, struct SOAP_ENV__Reason *const*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerToSOAP_ENV__Reason(struct soap*, const char *, int, struct SOAP_ENV__Reason *const*, const char *);
SOAP_FMAC3 struct SOAP_ENV__Reason ** SOAP_FMAC4 soap_in_PointerToSOAP_ENV__Reason(struct soap*, const char*, struct SOAP_ENV__Reason **, const char*);

#ifndef soap_write_PointerToSOAP_ENV__Reason
#define soap_write_PointerToSOAP_ENV__Reason(soap, data) ( soap_begin_send(soap) || (soap_serialize_PointerToSOAP_ENV__Reason(soap, data), 0) || soap_put_PointerToSOAP_ENV__Reason(soap, data, "SOAP-ENV:Reason", NULL) || soap_end_send(soap) )
#endif

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerToSOAP_ENV__Reason(struct soap*, struct SOAP_ENV__Reason *const*, const char*, const char*);

#ifndef soap_read_PointerToSOAP_ENV__Reason
#define soap_read_PointerToSOAP_ENV__Reason(soap, data) ( soap_begin_recv(soap) || !soap_get_PointerToSOAP_ENV__Reason(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct SOAP_ENV__Reason ** SOAP_FMAC4 soap_get_PointerToSOAP_ENV__Reason(struct soap*, struct SOAP_ENV__Reason **, const char*, const char*);

#endif

#ifndef WITH_NOGLOBAL

#ifndef SOAP_TYPE_PointerToSOAP_ENV__Detail
#define SOAP_TYPE_PointerToSOAP_ENV__Detail (28)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerToSOAP_ENV__Detail(struct soap*, struct SOAP_ENV__Detail *const*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerToSOAP_ENV__Detail(struct soap*, const char *, int, struct SOAP_ENV__Detail *const*, const char *);
SOAP_FMAC3 struct SOAP_ENV__Detail ** SOAP_FMAC4 soap_in_PointerToSOAP_ENV__Detail(struct soap*, const char*, struct SOAP_ENV__Detail **, const char*);

#ifndef soap_write_PointerToSOAP_ENV__Detail
#define soap_write_PointerToSOAP_ENV__Detail(soap, data) ( soap_begin_send(soap) || (soap_serialize_PointerToSOAP_ENV__Detail(soap, data), 0) || soap_put_PointerToSOAP_ENV__Detail(soap, data, "SOAP-ENV:Detail", NULL) || soap_end_send(soap) )
#endif

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerToSOAP_ENV__Detail(struct soap*, struct SOAP_ENV__Detail *const*, const char*, const char*);

#ifndef soap_read_PointerToSOAP_ENV__Detail
#define soap_read_PointerToSOAP_ENV__Detail(soap, data) ( soap_begin_recv(soap) || !soap_get_PointerToSOAP_ENV__Detail(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct SOAP_ENV__Detail ** SOAP_FMAC4 soap_get_PointerToSOAP_ENV__Detail(struct soap*, struct SOAP_ENV__Detail **, const char*, const char*);

#endif

#ifndef WITH_NOGLOBAL

#ifndef SOAP_TYPE_PointerToSOAP_ENV__Code
#define SOAP_TYPE_PointerToSOAP_ENV__Code (22)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerToSOAP_ENV__Code(struct soap*, struct SOAP_ENV__Code *const*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerToSOAP_ENV__Code(struct soap*, const char *, int, struct SOAP_ENV__Code *const*, const char *);
SOAP_FMAC3 struct SOAP_ENV__Code ** SOAP_FMAC4 soap_in_PointerToSOAP_ENV__Code(struct soap*, const char*, struct SOAP_ENV__Code **, const char*);

#ifndef soap_write_PointerToSOAP_ENV__Code
#define soap_write_PointerToSOAP_ENV__Code(soap, data) ( soap_begin_send(soap) || (soap_serialize_PointerToSOAP_ENV__Code(soap, data), 0) || soap_put_PointerToSOAP_ENV__Code(soap, data, "SOAP-ENV:Code", NULL) || soap_end_send(soap) )
#endif

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerToSOAP_ENV__Code(struct soap*, struct SOAP_ENV__Code *const*, const char*, const char*);

#ifndef soap_read_PointerToSOAP_ENV__Code
#define soap_read_PointerToSOAP_ENV__Code(soap, data) ( soap_begin_recv(soap) || !soap_get_PointerToSOAP_ENV__Code(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct SOAP_ENV__Code ** SOAP_FMAC4 soap_get_PointerToSOAP_ENV__Code(struct soap*, struct SOAP_ENV__Code **, const char*, const char*);

#endif

#ifndef SOAP_TYPE_PointerTo_ns1__ConfigRsp
#define SOAP_TYPE_PointerTo_ns1__ConfigRsp (17)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_ns1__ConfigRsp(struct soap*, _ns1__ConfigRsp *const*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_ns1__ConfigRsp(struct soap*, const char *, int, _ns1__ConfigRsp *const*, const char *);
SOAP_FMAC3 _ns1__ConfigRsp ** SOAP_FMAC4 soap_in_PointerTo_ns1__ConfigRsp(struct soap*, const char*, _ns1__ConfigRsp **, const char*);

#ifndef soap_write_PointerTo_ns1__ConfigRsp
#define soap_write_PointerTo_ns1__ConfigRsp(soap, data) ( soap_begin_send(soap) || (soap_serialize_PointerTo_ns1__ConfigRsp(soap, data), 0) || soap_put_PointerTo_ns1__ConfigRsp(soap, data, "ns1:ConfigRsp", NULL) || soap_end_send(soap) )
#endif

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_ns1__ConfigRsp(struct soap*, _ns1__ConfigRsp *const*, const char*, const char*);

#ifndef soap_read_PointerTo_ns1__ConfigRsp
#define soap_read_PointerTo_ns1__ConfigRsp(soap, data) ( soap_begin_recv(soap) || !soap_get_PointerTo_ns1__ConfigRsp(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 _ns1__ConfigRsp ** SOAP_FMAC4 soap_get_PointerTo_ns1__ConfigRsp(struct soap*, _ns1__ConfigRsp **, const char*, const char*);

#ifndef SOAP_TYPE_PointerTo_ns1__RemoteConfig
#define SOAP_TYPE_PointerTo_ns1__RemoteConfig (16)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_ns1__RemoteConfig(struct soap*, _ns1__RemoteConfig *const*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_ns1__RemoteConfig(struct soap*, const char *, int, _ns1__RemoteConfig *const*, const char *);
SOAP_FMAC3 _ns1__RemoteConfig ** SOAP_FMAC4 soap_in_PointerTo_ns1__RemoteConfig(struct soap*, const char*, _ns1__RemoteConfig **, const char*);

#ifndef soap_write_PointerTo_ns1__RemoteConfig
#define soap_write_PointerTo_ns1__RemoteConfig(soap, data) ( soap_begin_send(soap) || (soap_serialize_PointerTo_ns1__RemoteConfig(soap, data), 0) || soap_put_PointerTo_ns1__RemoteConfig(soap, data, "ns1:RemoteConfig", NULL) || soap_end_send(soap) )
#endif

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_ns1__RemoteConfig(struct soap*, _ns1__RemoteConfig *const*, const char*, const char*);

#ifndef soap_read_PointerTo_ns1__RemoteConfig
#define soap_read_PointerTo_ns1__RemoteConfig(soap, data) ( soap_begin_recv(soap) || !soap_get_PointerTo_ns1__RemoteConfig(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 _ns1__RemoteConfig ** SOAP_FMAC4 soap_get_PointerTo_ns1__RemoteConfig(struct soap*, _ns1__RemoteConfig **, const char*, const char*);

#ifndef SOAP_TYPE_PointerTons1__ResultInfo
#define SOAP_TYPE_PointerTons1__ResultInfo (15)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTons1__ResultInfo(struct soap*, ns1__ResultInfo *const*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTons1__ResultInfo(struct soap*, const char *, int, ns1__ResultInfo *const*, const char *);
SOAP_FMAC3 ns1__ResultInfo ** SOAP_FMAC4 soap_in_PointerTons1__ResultInfo(struct soap*, const char*, ns1__ResultInfo **, const char*);

#ifndef soap_write_PointerTons1__ResultInfo
#define soap_write_PointerTons1__ResultInfo(soap, data) ( soap_begin_send(soap) || (soap_serialize_PointerTons1__ResultInfo(soap, data), 0) || soap_put_PointerTons1__ResultInfo(soap, data, "ns1:ResultInfo", NULL) || soap_end_send(soap) )
#endif

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTons1__ResultInfo(struct soap*, ns1__ResultInfo *const*, const char*, const char*);

#ifndef soap_read_PointerTons1__ResultInfo
#define soap_read_PointerTons1__ResultInfo(soap, data) ( soap_begin_recv(soap) || !soap_get_PointerTons1__ResultInfo(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 ns1__ResultInfo ** SOAP_FMAC4 soap_get_PointerTons1__ResultInfo(struct soap*, ns1__ResultInfo **, const char*, const char*);

#ifndef SOAP_TYPE_PointerTons1__ConfigReq
#define SOAP_TYPE_PointerTons1__ConfigReq (14)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTons1__ConfigReq(struct soap*, ns1__ConfigReq *const*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTons1__ConfigReq(struct soap*, const char *, int, ns1__ConfigReq *const*, const char *);
SOAP_FMAC3 ns1__ConfigReq ** SOAP_FMAC4 soap_in_PointerTons1__ConfigReq(struct soap*, const char*, ns1__ConfigReq **, const char*);

#ifndef soap_write_PointerTons1__ConfigReq
#define soap_write_PointerTons1__ConfigReq(soap, data) ( soap_begin_send(soap) || (soap_serialize_PointerTons1__ConfigReq(soap, data), 0) || soap_put_PointerTons1__ConfigReq(soap, data, "ns1:ConfigReq", NULL) || soap_end_send(soap) )
#endif

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTons1__ConfigReq(struct soap*, ns1__ConfigReq *const*, const char*, const char*);

#ifndef soap_read_PointerTons1__ConfigReq
#define soap_read_PointerTons1__ConfigReq(soap, data) ( soap_begin_recv(soap) || !soap_get_PointerTons1__ConfigReq(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 ns1__ConfigReq ** SOAP_FMAC4 soap_get_PointerTons1__ConfigReq(struct soap*, ns1__ConfigReq **, const char*, const char*);

#ifndef SOAP_TYPE__QName
#define SOAP_TYPE__QName (5)
#endif

#define soap_default__QName(soap, a) soap_default_string(soap, a)


#define soap_serialize__QName(soap, a) soap_serialize_string(soap, a)

SOAP_FMAC3 int SOAP_FMAC4 soap_out__QName(struct soap*, const char*, int, char*const*, const char*);
SOAP_FMAC3 char * * SOAP_FMAC4 soap_in__QName(struct soap*, const char*, char **, const char*);

#ifndef soap_write__QName
#define soap_write__QName(soap, data) ( soap_begin_send(soap) || (soap_serialize__QName(soap, data), 0) || soap_put__QName(soap, data, "byte", NULL) || soap_end_send(soap) )
#endif

SOAP_FMAC3 int SOAP_FMAC4 soap_put__QName(struct soap*, char *const*, const char*, const char*);

#ifndef soap_read__QName
#define soap_read__QName(soap, data) ( soap_begin_recv(soap) || !soap_get__QName(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 char ** SOAP_FMAC4 soap_get__QName(struct soap*, char **, const char*, const char*);

#ifndef SOAP_TYPE_string
#define SOAP_TYPE_string (4)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default_string(struct soap*, char **);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_string(struct soap*, char *const*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_string(struct soap*, const char*, int, char*const*, const char*);
SOAP_FMAC3 char * * SOAP_FMAC4 soap_in_string(struct soap*, const char*, char **, const char*);

#ifndef soap_write_string
#define soap_write_string(soap, data) ( soap_begin_send(soap) || (soap_serialize_string(soap, data), 0) || soap_put_string(soap, data, "byte", NULL) || soap_end_send(soap) )
#endif

SOAP_FMAC3 int SOAP_FMAC4 soap_put_string(struct soap*, char *const*, const char*, const char*);

#ifndef soap_read_string
#define soap_read_string(soap, data) ( soap_begin_recv(soap) || !soap_get_string(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 char ** SOAP_FMAC4 soap_get_string(struct soap*, char **, const char*, const char*);

#endif

/* End of testClientH.h */
