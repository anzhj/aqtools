#include "authServer.h"
#include "wibudata.h"
#include <uv.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string>
#include <string.h>
#include <memory.h>
#include <time.h>
#include <iostream>
#include <openssl/md5.h>

uv_tcp_t   _server;
uv_tcp_t   _client;
uv_loop_t* _loop;

static void tinyweb_on_connection(uv_stream_t* server, int status);

static bool bHasExpired = false;

static void thread_entry(void* arg) {
	
	while (true)
	{
		if (bHasExpired = true)
		{
			system("./firewall.sh true");
		}
		else
		{
			system("./firewall.sh false");
		}

		sleep(10);
	}
}	

//start web server, linstening ip:port
//ip can be NULL or "", which means "0.0.0.0"
void tinyweb_start(uv_loop_t* loop, const char* ip, int port) {

	struct sockaddr_in addr;
	uv_ip4_addr((ip && ip[0]) ? ip : "0.0.0.0", port, &addr);
	_loop = loop;
	uv_tcp_init(_loop, &_server);
	uv_tcp_bind(&_server, (const struct sockaddr*) &addr, 0);
	uv_listen((uv_stream_t*)&_server, 8, tinyweb_on_connection);

	uv_thread_t tid;

	int nRtn = uv_thread_create(&tid, thread_entry, (void*)42);
	assert(nRtn == 0);
}

static void after_uv_close(uv_handle_t* handle) {
	free(handle); // uv_tcp_t* client, see tinyweb_on_connection()
}

static void after_uv_write(uv_write_t* w, int status) {
	if(w->data)
		free(w->data);
	uv_close((uv_handle_t*)w->handle, after_uv_close); // close client
	free(w);
}

static void write_uv_data(uv_stream_t* stream, const void* data, unsigned int len, int need_copy_data) {
	uv_buf_t buf;
	uv_write_t* w;
	void* newdata  = (void*)data;

	if(data == NULL || len == 0) return;
	if(len ==(unsigned int)-1)
		len = strlen((const char*)data);

	if(need_copy_data) {
		newdata = malloc(len);
		memcpy(newdata, data, len);
	}

	buf = uv_buf_init((char*)newdata, len);
	w = (uv_write_t*)malloc(sizeof(uv_write_t));
	w->data = need_copy_data ? newdata : NULL;
	uv_write(w, stream, &buf, 1, after_uv_write); // free w and w->data in after_uv_write()
}

char* format_http_respone(const char* status, const char* content_type, const void* content, int content_length) {
	int totalsize;
	char* respone;
	if(content_length < 0)
		content_length = content ? strlen((char*)content) : 0;
	totalsize = strlen(status) + strlen(content_type) + content_length + 128;
	respone = (char*) malloc(totalsize);
	sprintf(respone, "HTTP/1.1 %s\r\n"
                         "Content-Type:%s;charset=utf-8\r\n"
                         "Content-Length:%d\r\n\r\n"
                         "{\"ValidTime\":"
                         , status, content_type, 13+content_length);
	if(content) {
		memcpy(respone + strlen(respone), content, content_length);
	}
	return respone;
}

#if defined(WIN32)
        #define snprintf _snprintf
#endif

static void tinyweb_on_connection(uv_stream_t* server, int status) {
	assert(server == (uv_stream_t*)&_server);    

	int nRtn = 0;
	char content[1024];

	// Read expansion data.
	int nCharLen = 0;
	const char * iData = ReadData(&nCharLen, 10, 10726);

	// Analysis expiration time and md5 from expansion data.
	std::string strContext = iData;
	std::string strDate;
	std::string strMd5;

	size_t nPos = strContext.find(";");
	if (nPos == std::string::npos)
	{
		snprintf(content, sizeof(content), "\"Invalid expansion data.\"}\r\n");
	}
	else
	{	
		strDate = strContext.substr(0, nPos);
                strMd5 = strContext.substr(nPos + 1);

		// Get MAC
		char szMac[19];
                FILE * fp;
                char strCmd[128]="ifconfig | grep eth0 | awk '{print $5}'";

                if ((fp = popen(strCmd, "r")) == NULL)
                {
			snprintf(content, sizeof(content), "\"Get Mac Failed\"}\r\n");
                }
		else
		{
			if (fgets(szMac, sizeof(szMac)-1, fp) == NULL)
			{
				snprintf(content, sizeof(content), "\"Get Mac Failed\"}\r\n");
			}
			else
			{
				// Change Mac to Md5
				MD5_CTX ctx;
				unsigned char md[16];				
				char buf[33]={'\0'};
			        char tmp[33]={'\0'};

				int i;				

				MD5_Init(&ctx);
				MD5_Update(&ctx, szMac, strlen(szMac));
				MD5_Final(md, &ctx);
				for (i=0; i<16; i++)
        			{
			                sprintf(tmp, "%02x", md[i]);
			                strcat(buf, tmp);
			        }

				// Compare Mac'Md5 to strMd5
				if (strcmp(buf, strMd5.c_str()) != 0)
				{
					snprintf(content, sizeof(content), "\"PC had not been authorized.\"}\r\n");
				}
				else
				{
					if (strDate == "Error")
					{
						snprintf(content, sizeof(content), "\"%s\"}\r\n", strDate.c_str());
					}
					else
					{
						if (strDate.length() == 0)
						{
							snprintf(content, sizeof(content), "\"NeverExpired\"}\r\n");
						}
						else
						{
							struct tm date;

							sscanf(strDate.c_str(), "%d-%d-%d %d:%d:%d"
										, &date.tm_year, &date.tm_mon, &date.tm_mday
										, &date.tm_hour, &date.tm_min, &date.tm_sec);
							date.tm_year -= 1900;
							date.tm_mon -= 1;

							time_t expTime = mktime(&date);
							time_t now = time(NULL);

							date.tm_year += 1900;
							date.tm_mon += 1;
							if (expTime < 0)
							{
								snprintf(content, sizeof(content), "\"%d-%d-%d\"}\r\n"
												 , date.tm_year, date.tm_mon, date.tm_mday);
							}
							else if (difftime(now, expTime) <0)
							{
								snprintf(content, sizeof(content), "\"%d-%d-%d\"}\r\n"
												 , date.tm_year, date.tm_mon, date.tm_mday);
							}
							else
							{
								snprintf(content, sizeof(content), "\"%d-%d-%d\"}\r\n"
												 , date.tm_year, date.tm_mon, date.tm_mday);
								bHasExpired = true;
							}
						}
					}// End if(strDate == "Error")
				}// End if (strcmp(buf, strMd5.c_str()) != 0)
			}// End if (fgets(szMac, sizeof(szMac)-1, fp) == NULL)
		}// End if ((fp = popen(strCmd, "r")) == NULL)
	}// End if (nPos == std::string::npos)

	char* http_respone = format_http_respone("200 OK", "text/html", content, -1);

	if(status == 0) {
		uv_tcp_t* client = (uv_tcp_t*)malloc(sizeof(uv_tcp_t));
		uv_tcp_init(_loop, client);
		uv_accept((uv_stream_t*)&_server, (uv_stream_t*)client);
		write_uv_data((uv_stream_t*)client, http_respone, -1, 0);
		//close client after uv_write, and free it in after_uv_close()
	}
}
