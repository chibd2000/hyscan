#include "s_web_scanner.h"


s_web_scanner::s_web_scanner()
{
	
}


s_web_scanner::~s_web_scanner()
{

}


void s_web_scanner::scan(string& url, string& port){
	HttpRequest httpRequest;
	WORD wPort = atoi(port.c_str());
	httpRequest.request(url, wPort);
}
