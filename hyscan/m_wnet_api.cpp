#include "m_wnet_api.h"


WNET_API::WNET_API(string& serverName)
{
}


WNET_API::~WNET_API()
{
	this->closeConnection();
}

void WNET_API::openConnect(){
	NETRESOURCE netResource;
	ZeroMemory(&netResource, sizeof(NETRESOURCE));
	string fmtRemoteName;
	
	fmtRemoteName.append("\\\\");
	fmtRemoteName.append(this->serverName);
	fmtRemoteName.append("\\c$");
	
	cout << "[+] Calling WNetAddConnection2 with\n" << endl;
	cout << "[+] RemoteName: " << this->serverName.c_str() << endl;
	
	netResource.dwType = RESOURCETYPE_ANY;
	netResource.lpLocalName = NULL;
	netResource.lpRemoteName = (LPSTR)fmtRemoteName.c_str();
	netResource.lpProvider = NULL;
	
	DWORD dwRet;
	dwRet = WNetAddConnection2(&netResource, NULL, NULL, CONNECT_TEMPORARY);
}

void WNET_API::closeConnection(){
	// WNetCancelConnection2()
}

void WNET_API::enumLoggedUser(wstring serverName){
	NET_API_STATUS nStatus;
	LPSESSION_INFO_10 pBuf = NULL;
	LPSESSION_INFO_10 pTmpBuf;
	DWORD dwLevel = 10;
	DWORD dwPrefMaxLen = MAX_PREFERRED_LENGTH;
	DWORD dwEntriesRead = 0;
	DWORD dwTotalEntries = 0;
	DWORD dwResumeHandle = 0;
	DWORD i;
	DWORD dwTotalCount = 0;
	//LPTSTR pszServerName = NULL;
	do // begin do
	{
		nStatus = NetSessionEnum((LPWSTR)serverName.c_str(),
			NULL,
			NULL,
			dwLevel,
			(LPBYTE*)&pBuf,
			dwPrefMaxLen,
			&dwEntriesRead,
			&dwTotalEntries,
			&dwResumeHandle);
		
		if ((nStatus == NERR_Success) || (nStatus == ERROR_MORE_DATA))
		{
			if ((pTmpBuf = pBuf) != NULL)
			{
				//
				// Loop through the entries.
				//
				for (i = 0; (i < dwEntriesRead); i++)
				{

					if (pTmpBuf == NULL)
					{
						fprintf(stderr, "An access violation has occurred\n");
						break;
					}
					//
					// Print the retrieved data. 
					//

					//sprintf(url, "%ws -> %ws\n", pTmpBuf->sesi10_cname,pTmpBuf->sesi10_username);

					SYSTEMTIME sys;
					GetLocalTime(&sys);
					char current_time[64] = { NULL };
					sprintf(current_time, "%4d-%02d-%02d %02d:%02d:%02d ", sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond);




					printf("[%s]  [%ws]  [%ws]  [%ws]\n", current_time, serverName, pTmpBuf->sesi10_cname, pTmpBuf->sesi10_username);
					/*
					wprintf(L"\n\tClient: %s\n", pTmpBuf->sesi10_cname);
					wprintf(L"\tUser:   %s\n", pTmpBuf->sesi10_username);
					printf("\tActive: %d\n", pTmpBuf->sesi10_time);
					printf("\tIdle:   %d\n", pTmpBuf->sesi10_idle_time);
					*/
					pTmpBuf++;
					dwTotalCount++;
				}
			}
		}
		//
		// Otherwise, indicate a system error.
		//
		else
			fprintf(stderr, "A system error has occurred: %d\n", nStatus);
		//
		// Free the allocated memory.
		//
		if (pBuf != NULL)
		{
			delete pBuf;
			pBuf = NULL;
		}
	}
	// 
	// Continue to call NetSessionEnum while 
	//  there are more entries. 
	// 
	while (nStatus == ERROR_MORE_DATA); // end do

	// Check again for an allocated buffer.
	//
	if (pBuf != NULL)
		delete pBuf;
}