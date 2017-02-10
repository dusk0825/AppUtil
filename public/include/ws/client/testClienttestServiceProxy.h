/* testClienttestServiceProxy.h
   Generated by gSOAP 2.8.8 from testClient.h

Copyright(C) 2000-2012, Robert van Engelen, Genivia Inc. All Rights Reserved.
The generated code is released under one of the following licenses:
1) GPL or 2) Genivia's license for commercial use.
This program is released under the GPL with the additional exemption that
compiling, linking, and/or using OpenSSL is allowed.
*/

#ifndef testClienttestServiceProxy_H
#define testClienttestServiceProxy_H
#include "testClientH.h"

class SOAP_CMAC testServiceProxy : public soap
{ public:
	/// Endpoint URL of service 'testServiceProxy' (change as needed)
	const char *soap_endpoint;
	/// Constructor
	testServiceProxy();
	/// Construct from another engine state
	testServiceProxy(const struct soap&);
	/// Constructor with endpoint URL
	testServiceProxy(const char *url);
	/// Constructor with engine input+output mode control
	testServiceProxy(soap_mode iomode);
	/// Constructor with URL and input+output mode control
	testServiceProxy(const char *url, soap_mode iomode);
	/// Constructor with engine input and output mode control
	testServiceProxy(soap_mode imode, soap_mode omode);
	/// Destructor frees deserialized data
	virtual	~testServiceProxy();
	/// Initializer used by constructors
	virtual	void testServiceProxy_init(soap_mode imode, soap_mode omode);
	/// Delete all deserialized data (uses soap_destroy and soap_end)
	virtual	void destroy();
	/// Delete all deserialized data and reset to default
	virtual	void reset();
	/// Disables and removes SOAP Header from message
	virtual	void soap_noheader();
	/// Get SOAP Header structure (NULL when absent)
	virtual	const SOAP_ENV__Header *soap_header();
	/// Get SOAP Fault structure (NULL when absent)
	virtual	const SOAP_ENV__Fault *soap_fault();
	/// Get SOAP Fault string (NULL when absent)
	virtual	const char *soap_fault_string();
	/// Get SOAP Fault detail as string (NULL when absent)
	virtual	const char *soap_fault_detail();
	/// Close connection (normally automatic, except for send_X ops)
	virtual	int soap_close_socket();
	/// Force close connection (can kill a thread blocked on IO)
	virtual	int soap_force_close_socket();
	/// Print fault
	virtual	void soap_print_fault(FILE*);
#ifndef WITH_LEAN
	/// Print fault to stream
#ifndef WITH_COMPAT
	virtual	void soap_stream_fault(std::ostream&);
#endif

	/// Put fault into buffer
	virtual	char *soap_sprint_fault(char *buf, size_t len);
#endif

	/// Web service operation 'RemoteConfig' (returns error code or SOAP_OK)
	virtual	int RemoteConfig(_ns1__RemoteConfig *ns1__RemoteConfig, _ns1__ConfigRsp *ns1__ConfigRsp) { return RemoteConfig(NULL, NULL, ns1__RemoteConfig, ns1__ConfigRsp); }
	virtual	int RemoteConfig(const char *endpoint, const char *soap_action, _ns1__RemoteConfig *ns1__RemoteConfig, _ns1__ConfigRsp *ns1__ConfigRsp);
};
#endif
