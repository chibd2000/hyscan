#include "m_http_api.h"


m_http_api::m_http_api()
{
}


m_http_api::~m_http_api()
{
}

string m_http_api::getHttpTitle(string& text){
	// 处理多个标签从上到下
	smatch matchResult;
	string::const_iterator iterStart = text.begin();
	string::const_iterator iterEnd = text.end();
	// title标签
	if (regex_search(iterStart, iterEnd, matchResult, regex("<title>(.*)</title>"), regex_constants::format_first_only)){
		//cout << matchResult[1] << endl;
		return matchResult[1];
	}
	// h1
	if (regex_search(iterStart, iterEnd, matchResult, regex("<h1>(.*)</h1>"), regex_constants::format_first_only)){
		//cout << matchResult[1] << endl;

		return matchResult[1];
	}
	// h2
	if (regex_search(iterStart, iterEnd, matchResult, regex("<h2>(.*)</h2>"), regex_constants::format_first_only)){
		//cout << matchResult[1] << endl;

		return matchResult[1];
	}
	// h3
	if (regex_search(iterStart, iterEnd, matchResult, regex("<h3>(.*)</h3>"), regex_constants::format_first_only)){
		//cout << matchResult[1] << endl;

		return matchResult[1];
	}
	// h4
	if (regex_search(iterStart, iterEnd, matchResult, regex("<meta name=\"description\" content=\"(.*)\">"), regex_constants::format_first_only)){
		//cout << matchResult[1] << endl;

		return matchResult[1];
	}
	return "";
}
/*
HttpRequest m_http_api::requestInit(){
	HttpRequest a;
	return a;
}*/


void HttpRequest::request(string& url, WORD port){
	HINTERNET hSession = NULL;
	HINTERNET hConnect = NULL;
	HINTERNET hRequest = NULL;
	m_http_api httpApi;
	setlocale(0, "");

	//1. 初始化一个WinHTTP-session句柄，参数1为此句柄的名称
	hSession = WinHttpOpen(L"Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/65.0.3325.181 Safari/537.36", NULL, NULL, NULL, NULL);
	if (hSession == NULL) {
#ifdef DEBUG
		cout << "Error:Open session failed: " << GetLastError() << endl;
#else
#endif
		return;
	}
	// 超时时间设置
	WinHttpSetTimeouts(hRequest, 5000, 5000, 0, 0);

	//2. 通过上述句柄连接到服务器，需要指定服务器IP和端口号。若连接成功，返回的hConnect句柄不为NULL
	hConnect = WinHttpConnect(hSession, string2Wstring(url).c_str(), (INTERNET_PORT)port, 0);
	if (hConnect == NULL) {
#ifdef DEBUG
		cout << "Error:Connect failed: " << GetLastError() << endl;
#else
#endif
		return;
	}

	//3. 通过hConnect句柄创建一个hRequest句柄，用于发送数据与读取从服务器返回的数据。
	if (port == 443){
		// use SSL if applicable (HTTPS)
		hRequest = WinHttpOpenRequest(hConnect, L"Get", NULL, NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);
		// hRequest = WinHttpOpenRequest(hConnect, L"Post", L"getServiceInfo", NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, 0);
	}
	else{
		hRequest = WinHttpOpenRequest(hConnect, L"Get", NULL, NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, 0);
	}

	
	// 可选证书
	//BOOL fRet = WinHttpSetOption(hRequest, WINHTTP_OPTION_CLIENT_CERT_CONTEXT, WINHTTP_NO_CLIENT_CERT_CONTEXT, 0);

	//其中参数2表示请求方式，此处为Post；参数3:给定Post的具体地址，如这里的具体地址为http://192.168.50.112/getServiceInfo
	if (hRequest == NULL) {
#ifdef DEBUG
		cout << "Error:OpenRequest failed: " << GetLastError() << endl;
#else
#endif
		return;
	}

	BOOL bResults;

	// header add
	// bResults = WinHttpAddRequestHeaders(hRequest, L"", (ULONG)-1L, WINHTTP_ADDREQ_FLAG_ADD);

	// 向服务器发送post数据
	//string data = "This is my data to be sent";
	//const void *ss = (const char *)data.c_str();

	// POST 
	//bResults = WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, const_cast<void*>(ss), data.length(), data.length(), 0);

	// GET
	bResults = WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0);

	if (!bResults){
#ifdef DEBUG
		cout << "Error:SendRequest failed: " << GetLastError() << endl;
#else
#endif
		return;
	}
	else{
		//（3） 发送请求成功则准备接受服务器的response。
		// 注意：在使用 WinHttpQueryDataAvailable和WinHttpReadData前必须使用WinHttpReceiveResponse才能access服务器返回的数据
		bResults = WinHttpReceiveResponse(hRequest, NULL);
	}

	//4-2. 获取服务器返回数据的header信息。这一步我用来获取返回数据的数据类型。

	DWORD dwSize = 0;
	DWORD dwMaxSize = 0;
	/*
	PWCHAR lpHeaderBuffer = NULL;
	BOOL bUtf8 = FALSE;
	// 获取响应包的header头
	if (bResults)
	{
		// 获取header的长度
		WinHttpQueryHeaders(hRequest, WINHTTP_QUERY_RAW_HEADERS_CRLF,
			WINHTTP_HEADER_NAME_BY_INDEX, NULL,
			&dwSize, WINHTTP_NO_HEADER_INDEX);

		// 根据header的长度为buffer申请内存空间
		if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
		{
			lpHeaderBuffer = new WCHAR[dwSize / sizeof(WCHAR)];

			if (lpHeaderBuffer != NULL){
				// 使用WinHttpQueryHeaders获取header信息
				bResults = WinHttpQueryHeaders(hRequest,
					WINHTTP_QUERY_RAW_HEADERS_CRLF,
					WINHTTP_HEADER_NAME_BY_INDEX,
					lpHeaderBuffer, &dwSize,
					WINHTTP_NO_HEADER_INDEX);
				if (wcsstr(lpHeaderBuffer, L"charset=utf-8") != NULL){
					bUtf8 = TRUE;
				}
			}
		}
	}*/
	


	//解析上述header信息会发现服务器返回数据的charset为uft-8。这意味着后面需要对获取到的raw data进行宽字符转换。一开始由于没有意识到需要进行转换所以得到的数据都是乱码。
	//出现乱码的原因是：HTTP在传输过程中是二值的，它并没有text或者是unicode的概念。HTTP使用7bit的ASCII码作为HTTP headers，但是内容是任意的二值数据，需要根据header中指定的编码方式来描述它（通常是Content-Type header）.
	//因此当你接收到原始的HTTP数据时，先将其保存到char[] buffer中，然后利用WinHttpQueryHearders()获取HTTP头，得到内容的Content-Type,这样你就知道数据到底是啥类型的了，是ASCII还是Unicode或者其他。
	//一旦你知道了具体的编码方式，你就可以通过MultiByteToWideChar()将其转换成合适编码的字符，存入wchar_t[]中。

	// 获取服务器返回数据
	LPSTR pszOutBuffer = NULL;
	DWORD dwDownloaded = 0;         //实际收取的字符数
	/*
	if (bResults)
	{
		do
		{
			//(1) 获取返回数据的大小（以字节为单位）
			dwSize = 0;
			if (!WinHttpQueryDataAvailable(hRequest, &dwSize)){
#ifdef DEBUG
				cout << "Error：WinHttpQueryDataAvailable failed：" << GetLastError() << endl;
#else
#endif
				break;
			}
			if (!dwSize)break;  //数据大小为0                

			//(2) 根据返回数据的长度为buffer申请内存空间
			pszOutBuffer = new char[dwSize + 1];
			if (!pszOutBuffer){
#ifdef DEBUG
				cout << "Out of memory." << endl;
#else
#endif
				break;
			}
			ZeroMemory(pszOutBuffer, dwSize + 1);       //将buffer置0

			//(3) 通过WinHttpReadData读取服务器的返回数据
			if (!WinHttpReadData(hRequest, pszOutBuffer, dwSize, &dwDownloaded)){
#ifdef DEBUG
				cout << "Error：WinHttpQueryDataAvailable failed：" << GetLastError() << endl;
#else
#endif
			}
			if (!dwDownloaded)
				break;

			string title = httpApi.getHttpTitle(string(pszOutBuffer));
			cout << "[+] " << url << " - " << utf82Gbk(title.c_str()) << endl;
			delete[] pszOutBuffer;

		} while (dwSize > 0);

	}*/

	if (bResults)
	{
		//(1) 获取返回数据的大小（以字节为单位）
		dwSize = 0;
		dwMaxSize = 1024;
		if (!WinHttpQueryDataAvailable(hRequest, &dwSize)){
#ifdef DEBUG
			cout << "Error：WinHttpQueryDataAvailable failed：" << GetLastError() << endl;
#else
#endif
			return;
		}
		if (!dwSize)return;  //数据大小为0   

		dwSize = dwSize > dwMaxSize ? dwMaxSize : dwSize;

		//(2) 根据返回数据的长度为buffer申请内存空间
		pszOutBuffer = new char[dwSize + 1];
		if (!pszOutBuffer){
#ifdef DEBUG
			cout << "Out of memory." << endl;
#else
#endif
			return;
		}
		ZeroMemory(pszOutBuffer, dwSize + 1);       //将buffer置0

		//(3) 通过WinHttpReadData读取服务器的返回数据
		if (!WinHttpReadData(hRequest, pszOutBuffer, dwSize, &dwDownloaded)){
#ifdef DEBUG
			cout << "Error：WinHttpQueryDataAvailable failed：" << GetLastError() << endl;
#else
#endif
		}
		if (!dwDownloaded)
			return;
		string title = httpApi.getHttpTitle(string(pszOutBuffer, dwSize));
		printf("[+] %s:%d - %s \n", url.data(), port, utf82Gbk(title).data());

		delete[] pszOutBuffer;

	}


	//5. 依次关闭request，connect，session句柄
	if (hRequest) WinHttpCloseHandle(hRequest);
	if (hConnect) WinHttpCloseHandle(hConnect);
	if (hSession) WinHttpCloseHandle(hSession);
}