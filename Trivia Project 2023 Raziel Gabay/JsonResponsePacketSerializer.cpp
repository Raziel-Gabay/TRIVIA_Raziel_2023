#include "JsonResponsePacketSerializer.h"



Buffer JsonResponsePacketSerializer::serializeResponse(ErrorResponse response)
{
    json responseJson = "{ \"message\":" + response.message + "}";
    std::string responseStr = responseJson.dump();
    return JsonResponsePacketSerializer::serializeResponse(responseStr, ERROR_CODE);
}


Buffer JsonResponsePacketSerializer::serializeResponse(LoginResponse response)
{
    json responseJson = "{ \"status\":" + std::to_string(response.status) + "}";
    std::string responseStr = responseJson.dump();
    return JsonResponsePacketSerializer::serializeResponse(responseStr, LOGIN_CODE);
}

Buffer JsonResponsePacketSerializer::serializeResponse(SignupResponse response)
{
    json responseJson = "{ \"status\":" + std::to_string(response.status) + "}";
    std::string responseStr = responseJson.dump();
    return JsonResponsePacketSerializer::serializeResponse(responseStr, SIGNUP_CODE);
}


Buffer JsonResponsePacketSerializer::serializeResponse(std::string response, char code)
{
    Buffer result;

    result.push_back(code);

    unsigned int msg_len = response.length();
    std::bitset<32> msg_len_binary(msg_len);  // Convert number to binary

    for (int i = 0; i < 32; i += 8)
    {
        std::bitset<8> group;
        for (int j = 0; j < 8; ++j) {
            group[j] = msg_len_binary[i + j];
        }
        int decimalValue = group.to_ulong();
        result.push_back(static_cast<char>(decimalValue));

    }
    std::reverse(result.begin()+ 1, result.end());

    for (auto iter = response.begin(); iter != response.end(); ++iter)
    {
        result.push_back(*iter);
    }

    return result;
}
