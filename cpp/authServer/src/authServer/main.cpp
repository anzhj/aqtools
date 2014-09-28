#include "authServer.h"
#include "init.h"
#include <stdio.h>
#include <iostream>
#include <openssl/md5.h>
#include <string>
#include <string.h>

int main(int argc, char* argv[]) 
{
	//daemon(0, 0);
	
	if (argc != 2)
	{
		std::cout << "Incorrect number of parameters." << std::endl;
		return -1;
	}

	//Get mac
	FILE * fp;
        char strCmd[1024];
	char szMac[19];
        snprintf(strCmd, sizeof(strCmd), "ifconfig | grep %s | awk '{print $5}'", argv[1]);

        if ((fp = popen(strCmd, "r")) == NULL)
        {
		std::cout << "Get mac failed" << std::endl;
		return -1;
        }
        else
        {
                if (fgets(szMac, sizeof(szMac)-1, fp) == NULL)
                {
			std::cout << "Get mac failed" << std::endl;
			return -1;
                }
        }
	fclose(fp);

	//Convert mac to md5
	MD5_CTX ctx;
	unsigned char md[16];
	char buf[33]={'\0'};
	char tmp[33]={'\0'};

	MD5_Init(&ctx);
	MD5_Update(&ctx, szMac, strlen(szMac));
	MD5_Final(md, &ctx);
	for (int i=0; i<16; i++)
	{
		sprintf(tmp, "%02x", md[i]);
		strcat(buf, tmp);
	}

	init_daemon();

	tinyweb_start(uv_default_loop(), "127.0.0.1", 8080, buf);
	uv_run(uv_default_loop(), UV_RUN_DEFAULT);  
} 

