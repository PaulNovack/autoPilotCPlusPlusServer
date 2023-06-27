#include <string>
#include <vector>
#include <mutex>
#include <hiredis.h>
#include <iostream>
#include "globals.h"
#include <algorithm>
#ifndef SSDBPOOL_H
#define SSDBPOOL_H

const int CONNECTIONS = 100;

class SSDBPool {
public:
    SSDBPool();
    SSDBPool(std::string ssdb_ip_address,int ssdb_port);
    SSDBPool(const SSDBPool& orig);
    virtual ~SSDBPool();
    redisContext *getConn();
    redisContext *c[CONNECTIONS];
    std::string del(std::string key);
    std::string get(std::string key);
    std::string incr(std::string key);
    std::string set(std::string key,std::string val);
    std::string  scan(std::string start,std::string end, int count);
    std::vector<std::string> scan(std::string start, std::string end);
private:
    int64_t connCounter = 0LL;
    std::mutex conn_arr_mut;
};

#endif /* SSDBPOOL_H */

