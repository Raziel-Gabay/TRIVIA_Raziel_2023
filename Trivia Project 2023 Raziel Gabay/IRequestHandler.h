#pragma once
#include <ctime>
#include <vector>
#include "json.hpp"
#include "JsonResponsePacketSerializer.h"

using json = nlohmann::json;
typedef std::vector<unsigned char> Buffer;

struct RequestInfo;
struct RequestResult;

class IRequestHandler
{
public:
    virtual bool isRequestRelevant(const RequestInfo& requestInfo) = 0;
    virtual RequestResult handleRequest(const RequestInfo& requestInfo) = 0;
    virtual ~IRequestHandler() {}
};

struct RequestResult
{
    Buffer response;
    IRequestHandler* newHandler;
};

struct RequestInfo
{
    int id;
    time_t receivalTime;
    Buffer buffer;
};