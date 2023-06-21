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

Buffer JsonResponsePacketSerializer::serializeResponse(LogoutResponse response)
{
    json responseJson = "{ \"status\":" + std::to_string(response.status) + "}";
    std::string responseStr = responseJson.dump();
    return JsonResponsePacketSerializer::serializeResponse(responseStr, LOGOUT_CODE);
}

Buffer JsonResponsePacketSerializer::serializeResponse(GetRoomsResponse response)
{
    std::string responseStr = "{ \"status\":" + std::to_string(response.status) + ",\"Rooms\":[";
    for (auto it = response.rooms.begin(); it != response.rooms.end(); ++it)
    {
        responseStr += "{\"id\":" + std::to_string(it->id) + ",\"name\":\"" + it->name
            + "\",\"maxPlayers\":" + std::to_string(it->maxPlayers) + ",\"numOfQuestionsInGame\":" + std::to_string(it->numOfQuestionsInGame)
            + ",\"timePerQuestion\":" + std::to_string(it->timePerQuestion) + ",\"isActive\":" + std::to_string(it->isActive) + "},";
    }
    responseStr.pop_back();
    responseStr += "]}";
    json responseJson = responseStr;
    responseStr = responseJson.dump();
    return JsonResponsePacketSerializer::serializeResponse(responseStr, GET_ROOMS_CODE);
}   

Buffer JsonResponsePacketSerializer::serializeResponse(GetPlayersInRoomResponse response)
{
    std::string responseStr = "{\"PlayersInRoom\":[";
    for (auto it = response.players.begin(); it != response.players.end(); ++it)
    {
        responseStr += "\"" + *it + "\",";
    }
    responseStr.pop_back();
    responseStr += "]}";
    json responseJson = responseStr;
    responseStr = responseJson.dump();
    return JsonResponsePacketSerializer::serializeResponse(responseStr, GET_PLAYERS_IN_ROOM_CODE);
}

Buffer JsonResponsePacketSerializer::serializeResponse(GetHighScoreResponse response)
{
    std::string responseStr = "{ \"status\":" + std::to_string(response.status) + ",\"HighScores\":<";
    for (auto it = response.statistics.begin(); it != response.statistics.end(); ++it)
    {
        responseStr += "\"" + *it + "\",";
    }
    responseStr.pop_back();
    responseStr += ">}";
    json responseJson = responseStr;
    responseStr = responseJson.dump();
    return JsonResponsePacketSerializer::serializeResponse(responseStr, GET_HIGHSCORE_CODE);
}

Buffer JsonResponsePacketSerializer::serializeResponse(GetPersonalStatsResponse response)
{
    std::string responseStr = "{ \"status\":" + std::to_string(response.status) + ",\"UserStatistics\":<";
    for (auto it = response.statistics.begin(); it != response.statistics.end(); ++it)
    {
        responseStr += "\"" + *it + "\",";
    }
    responseStr.pop_back();
    responseStr += ">}";
    json responseJson = responseStr;
    responseStr = responseJson.dump();
    return JsonResponsePacketSerializer::serializeResponse(responseStr, GET_PERSONAL_STATS_CODE);
}

Buffer JsonResponsePacketSerializer::serializeResponse(JoinRoomResponse response)
{
    json responseJson = "{ \"status\":" + std::to_string(response.status) + "}";
    std::string responseStr = responseJson.dump();
    return JsonResponsePacketSerializer::serializeResponse(responseStr, JOIN_ROOM_CODE);
}

Buffer JsonResponsePacketSerializer::serializeResponse(CreateRoomResponse response)
{
    json responseJson = "{ \"status\":" + std::to_string(response.status) + "}";
    std::string responseStr = responseJson.dump();
    return JsonResponsePacketSerializer::serializeResponse(responseStr, CREATE_ROOM_CODE);
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
