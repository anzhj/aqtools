#include "authServer.h"
#include "init.h"

int main() 
{
    //daemon(0, 0);

    init_daemon();
  
    tinyweb_start(uv_default_loop(), "127.0.0.1", 8080);  
    uv_run(uv_default_loop(), UV_RUN_DEFAULT);  
} 

