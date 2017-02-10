/* testClienttestServiceProxy.cpp
   Generated by gSOAP 2.8.8 from testClient.h

Copyright(C) 2000-2012, Robert van Engelen, Genivia Inc. All Rights Reserved.
The generated code is released under one of the following licenses:
1) GPL or 2) Genivia's license for commercial use.
This program is released under the GPL with the additional exemption that
compiling, linking, and/or using OpenSSL is allowed.
*/

#include "testClienttestServiceProxy.h"

testServiceProxy::testServiceProxy()
{	testServiceProxy_init(SOAP_IO_DEFAULT, SOAP_IO_DEFAULT);
}

testServiceProxy::testServiceProxy(const struct soap &_soap) : soap(_soap)
{ }

testServiceProxy::testServiceProxy(const char *url)
{	testServiceProxy_init(SOAP_IO_DEFAULT, SOAP_IO_DEFAULT);
	soap_endpoint = url;
}

testServiceProxy::testServiceProxy(soap_mode iomode)
{	testServiceProxy_init(iomode, iomode);
}

testServiceProxy::testServiceProxy(const char *url, soap_mode iomode)
{	testServiceProxy_init(iomode, iomode);
	soap_endpoint = url;
}

testServiceProxy::testServiceProxy(soap_mode imode, soap_mode omode)
{	testServiceProxy_init(imode, omode);
}

testServiceProxy::~testServiceProxy()
{ }

void testServiceProxy::testServiceProxy_init(soap_mode imode, soap_mode omode)
{	soap_imode(this, imode);
	soap_omode(this, omode);
	soap_endpoint = NULL;
	static const struct Namespace namespaces[] =
{
	{"SOAP-ENV", "http://schemas.xmlsoap.org/soap/envelope/", "http://www.w3.org/*/soap-envelope", NULL},
	{"SOAP-ENC", "http://schemas.xmlsoap.org/soap/encoding/", "http://www.w3.org/*/soap-encoding", NULL},
	{"xsi", "http://www.w3.org/2001/XMLSchema-instance", "http://www.w3.org/*/XMLSchema-instance", NULL},
	{"xsd", "http://www.w3.org/2001/XMLSchema", "http://www.w3.org/*/XMLSchema", NULL},
	{"ns1", "urn:testService", NULL, NULL},
	{NULL, NULL, NULL, NULL}
};
	soap_set_namespaces(this, namespaces);
}

void testServiceProxy::destroy()
{	soap_destroy(this);
	soap_end(this);
}

void testServiceProxy::reset()
{	destroy();
	soap_done(this);
	soap_init(this);
	testServiceProxy_init(SOAP_IO_DEFAULT, SOAP_IO_DEFAULT);
}

void testServiceProxy::soap_noheader()
{	this->header = NULL;
}

const SOAP_ENV__Header *testServiceProxy::soap_header()
{	return this->header;
}

const SOAP_ENV__Fault *testServiceProxy::soap_fault()
{	return this->fault;
}

const char *testServiceProxy::soap_fault_string()
{	return *soap_faultstring(this);
}

const char *testServiceProxy::soap_fault_detail()
{	return *soap_faultdetail(this);
}

int testServiceProxy::soap_close_socket()
{	return soap_closesock(this);
}

int testServiceProxy::soap_force_close_socket()
{	return soap_force_closesock(this);
}

void testServiceProxy::soap_print_fault(FILE *fd)
{	::soap_print_fault(this, fd);
}

#ifndef WITH_LEAN
#ifndef WITH_COMPAT
void testServiceProxy::soap_stream_fault(std::ostream& os)
{	::soap_stream_fault(this, os);
}
#endif

char *testServiceProxy::soap_sprint_fault(char *buf, size_t len)
{	return ::soap_sprint_fault(this, buf, len);
}
#endif

int testServiceProxy::RemoteConfig(const char *endpoint, const char *soap_action, _ns1__RemoteConfig *ns1__RemoteConfig, _ns1__ConfigRsp *ns1__ConfigRsp)
{	struct soap *soap = this;
	struct __ns1__RemoteConfig soap_tmp___ns1__RemoteConfig;
	if (endpoint)
		soap_endpoint = endpoint;
	if (!soap_endpoint)
		soap_endpoint = "http://localhost:6651/ws/testService";
	if (!soap_action)
		soap_action = "";
	soap->encodingStyle = NULL;
	soap_tmp___ns1__RemoteConfig.ns1__RemoteConfig = ns1__RemoteConfig;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___ns1__RemoteConfig(soap, &soap_tmp___ns1__RemoteConfig);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put___ns1__RemoteConfig(soap, &soap_tmp___ns1__RemoteConfig, "-ns1:RemoteConfig", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	}
	if (soap_end_count(soap))
		return soap->error;
	if (soap_connect(soap, soap_endpoint, soap_action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put___ns1__RemoteConfig(soap, &soap_tmp___ns1__RemoteConfig, "-ns1:RemoteConfig", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!ns1__ConfigRsp)
		return soap_closesock(soap);
	ns1__ConfigRsp->soap_default(soap);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	ns1__ConfigRsp->soap_get(soap, "ns1:ConfigRsp", "");
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}
/* End of client proxy code */