/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.cc to edit this template
 */

/* 
 * File:   SSDBPool.cpp
 * Author: pnovack
 * 
 * Created on October 4, 2022, 7:47 AM
 */

#include "SSDBPool.h"
using namespace std;

SSDBPool::SSDBPool() {
    
    redisReply *reply;
    struct timeval timeout = { 1, 500000 }; // 1.5 seconds
    for(int idx = 0;idx < CONNECTIONS;idx++){
        c[idx] = redisConnectWithTimeout("127.0.0.1", 8888,timeout); 

    }
    
}
SSDBPool::SSDBPool(std::string ssdb_ip_address,int ssdb_port) {
    
    redisReply *reply;
    struct timeval timeout = { 1, 500000 }; // 1.5 seconds
    for(int idx = 0;idx < CONNECTIONS;idx++){
        c[idx] = redisConnectWithTimeout(ssdb_ip_address.c_str(), ssdb_port,timeout); 

    }
}


SSDBPool::SSDBPool(const SSDBPool& orig) {
}
redisContext* SSDBPool::getConn(){
    std::lock_guard<std::mutex> guard(conn_arr_mut);
    connCounter++;
    return c[connCounter % CONNECTIONS];
}
std::string SSDBPool::del(std::string key){
    std::string ret = "";
    return ret;
}

std::string SSDBPool::get(std::string key){
    std::string req = "GET " + key;
    redisReply *reply;
    char *buf;
    size_t sz;
    std::string ret = "";
    redisContext *c = getConn();
    reply = (redisReply *) redisCommand(c,req.c_str());
    sz = snprintf(NULL, 0, "%s", reply->str);
    buf = (char *)malloc(sz + 1); /* make sure you check for != NULL in real code */
    snprintf(buf, sz+1, "%s", reply->str);
    freeReplyObject(reply);
    ret = buf;
    free(buf);
    std::replace( ret.begin(), ret.end(), '_', ' ');
    return ret;
}
std::string SSDBPool::set(std::string key,std::string val){
    std::replace( val.begin(), val.end(), ' ', '_');
    std::string req = "SET " + key + " " + val;
    redisReply *reply;
    char *buf;
    size_t sz;
    std::string ret = "";
    redisContext *c = getConn();
    reply = (redisReply *) redisCommand(c,req.c_str());
    sz = snprintf(NULL, 0, "%s", reply->str);
    buf = (char *)malloc(sz + 1); /* make sure you check for != NULL in real code */
    snprintf(buf, sz+1, "%s", reply->str);
    freeReplyObject(reply);
    ret = buf;
    free(buf);
    return ret;
}

std::string SSDBPool::incr(std::string key){

    std::string req = "INCR " + key;
    redisReply *reply;
    char *buf;
    size_t sz;
    std::string ret = "";
    redisContext *c = getConn();
    reply = (redisReply *) redisCommand(c,req.c_str());
    ret = std::to_string(reply->integer);
    return ret;
}
std::string SSDBPool::scan(std::string start,std::string end, int count){

    std::string req = "scan " + start + " " + end + std::to_string(count);
    redisReply *reply;
    char *buf;
    size_t sz;
    std::string ret = "";
    redisContext *c = getConn();
    reply = (redisReply *) redisCommand(c,req.c_str());
    ret = std::to_string(reply->integer);
    return ret;
}

SSDBPool::~SSDBPool() {
}

