#include <stdio.h>

#include "hpsocket/helper.h"
#include "pictl/pictl.h"

class CListenerImpl : public CTcpServerListener{

public:
	virtual EnHandleResult OnPrepareListen(ITcpServer* pSender, SOCKET soListen) override
	{
		TCHAR szAddress[50];
		int iAddressLen = sizeof(szAddress) / sizeof(TCHAR);
		USHORT usPort;

		pSender->GetListenAddress(szAddress, iAddressLen, usPort);

		return HR_OK;
	}

	virtual EnHandleResult OnAccept(ITcpServer* pSender, CONNID dwConnID, UINT_PTR soClient) override
	{
		BOOL bPass = TRUE;
		TCHAR szAddress[50];
		int iAddressLen = sizeof(szAddress) / sizeof(TCHAR);
		USHORT usPort;

		pSender->GetRemoteAddress(dwConnID, szAddress, iAddressLen, usPort);

		return bPass ? HR_OK : HR_ERROR;
	}

	virtual EnHandleResult OnHandShake(ITcpServer* pSender, CONNID dwConnID) override
	{
		return HR_OK;
	}

	virtual EnHandleResult OnReceive(ITcpServer* pSender, CONNID dwConnID, const BYTE* pData, int iLength) override
	{

		float y = 0.0;
		float x = 0.0;
		memcpy(&y, pData, 4);
		memcpy(&x, pData+4, 4);

		printf("dwConnID=%d iLength=%d y=%f x=%f\n", dwConnID, iLength, y, x);
		
		pi_run(y, x);

		return HR_OK;
	}

	virtual EnHandleResult OnSend(ITcpServer* pSender, CONNID dwConnID, const BYTE* pData, int iLength) override
	{
		return HR_OK;
	}

	virtual EnHandleResult OnClose(ITcpServer* pSender, CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode) override
	{

		return HR_OK;
	}

	virtual EnHandleResult OnShutdown(ITcpServer* pSender) override
	{
		return HR_OK;
	}

};

CListenerImpl g_listener;
CTcpPackServerPtr g_server(&g_listener);

#include <iostream>

int main(int argc, char* const argv[])
{
	if(0 != pi_init()) {
		printf("pictl init faild.\n");
	}

	g_server->SetMaxPackSize(0x7FF);
	g_server->SetPackHeaderFlag(0x169);
	g_server->SetKeepAliveTime(g_app_arg.keep_alive ? TCP_KEEPALIVE_TIME : 0);

	if(!g_server->Start(g_app_arg.bind_addr, g_app_arg.port)) {
		printf("hpsocket start faild.\n");
	}
	
	printf("hpsocket start succeed.\n");

	CString strLine;
	if(!std::getline(std::cin, strLine)) {
		PRINTLN();
		return -1;
	}

	return EXIT_CODE_OK;
}