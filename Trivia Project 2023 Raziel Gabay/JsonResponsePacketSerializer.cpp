#include "JsonResponsePacketSerializer.h"



Buffer JsonResponsePacketSerializer::serializeResponse(ErrorResponse response)
{

    json responseJson;
    responseJson["message"] = response.message;
    std::string responseStr = responseJson.dump();
    return JsonResponsePacketSerializer::serializeResponse(responseStr, ERROR_CODE);
}


Buffer JsonResponsePacketSerializer::serializeResponse(LoginResponse response)
{
    json responseJson;
    responseJson["status"] = response.status;
    std::string responseStr = responseJson.dump();
    return JsonResponsePacketSerializer::serializeResponse(responseStr, LOGIN_CODE);
}

Buffer JsonResponsePacketSerializer::serializeResponse(SignupResponse response)
{
    json responseJson;
    responseJson["status"] = response.status;
    std::string responseStr = responseJson.dump();
    return JsonResponsePacketSerializer::serializeResponse(responseStr, SIGNUP_CODE);
}

Buffer JsonResponsePacketSerializer::serializeResponse(LogoutResponse response)
{
    json responseJson;
    responseJson["status"] = response.status;
    std::string responseStr = responseJson.dump();
    return JsonResponsePacketSerializer::serializeResponse(responseStr, LOGOUT_CODE);
}

Buffer JsonResponsePacketSerializer::serializeResponse(GetRoomsResponse response)
{
    json responseJson;
    responseJson["status"] = response.status;
    //responseJson["Rooms"]
    json roomsJson = json::array();
    for (auto it = response.rooms.begin(); it != response.rooms.end(); ++it)
    {
        json roomJson;
        roomJson["id"] = it->id;
        roomJson["name"] = it->name;
        roomJson["maxPlayers"] = it->maxPlayers;
        roomJson["numOfQuestionsInGame"] = it->numOfQuestionsInGame;
        roomJson["timePerQuestion"] = it->timePerQuestion;
        roomJson["isActive"] = it->isActive;
        roomsJson.push_back(roomJson);
    }
    responseJson["Rooms"] = roomsJson;

    std::string responseStr = responseJson.dump();
    return JsonResponsePacketSerializer::serializeResponse(responseStr, GET_ROOMS_CODE);
}   

Buffer JsonResponsePacketSerializer::serializeResponse(GetPlayersInRoomResponse response)
{
    json responseJson;
    json playersJson = json::array();
    for (auto it = response.players.begin(); it != response.players.end(); ++it)
    {
        playersJson.push_back(*it);
    }
    responseJson["PlayersInRoom"] = playersJson;
    std::string responseStr = responseJson.dump();
    return JsonResponsePacketSerializer::serializeResponse(responseStr, GET_PLAYERS_IN_ROOM_CODE);
}

Buffer JsonResponsePacketSerializer::serializeResponse(GetHighScoreResponse response)
{
    json responseJson;
    responseJson["status"] = response.status;
    json highscoresJson = {};
    for (auto it = response.statistics.begin(); it != response.statistics.end(); it += 2)
    {
        highscoresJson[*it] = *(it + 1);
    }
    responseJson["HighScores"] = highscoresJson;
    std::string responseStr = responseJson.dump();
    return JsonResponsePacketSerializer::serializeResponse(responseStr, GET_HIGHSCORE_CODE);
}

Buffer JsonResponsePacketSerializer::serializeResponse(GetPersonalStatsResponse response)
{
    json responseJson;
    responseJson["status"] = response.status;
    json personalStatsJson = json::array();
    for (auto it = response.statistics.begin(); it != response.statistics.end(); ++it)
    {
        personalStatsJson.push_back(*it);
    }
    responseJson["UserStatistics"] = personalStatsJson;
    std::string responseStr = responseJson.dump();
    return JsonResponsePacketSerializer::serializeResponse(responseStr, GET_PERSONAL_STATS_CODE);
}

Buffer JsonResponsePacketSerializer::serializeResponse(JoinRoomResponse response)
{
    json responseJson;
    responseJson["status"] = response.status;
    std::string responseStr = responseJson.dump();
    return JsonResponsePacketSerializer::serializeResponse(responseStr, JOIN_ROOM_CODE);
}

Buffer JsonResponsePacketSerializer::serializeResponse(CreateRoomResponse response)
{
    json responseJson;
    responseJson["status"] = response.status;
    std::string responseStr = responseJson.dump();
    return JsonResponsePacketSerializer::serializeResponse(responseStr, CREATE_ROOM_CODE);
}

Buffer JsonResponsePacketSerializer::serializeResponse(CloseRoomResponse response)
{
    json responseJson;
    responseJson["status"] = response.status;
    std::string responseStr = responseJson.dump();
    return JsonResponsePacketSerializer::serializeResponse(responseStr, CLOSE_ROOM_CODE);
}

Buffer JsonResponsePacketSerializer::serializeResponse(StartGameResponse response)
{
    json responseJson;
    responseJson["status"] = response.status;
    std::string responseStr = responseJson.dump();
    return JsonResponsePacketSerializer::serializeResponse(responseStr, START_GAME_CODE);
}

Buffer JsonResponsePacketSerializer::serializeResponse(GetRoomStateResponse response)
{
    json responseJson;
    responseJson["status"] = response.status;
    responseJson["hasGameBegun"] = response.hasGameBegun;
    json playersJson = json::array();
    for (auto it = response.players.begin(); it != response.players.end(); ++it)
    {
        playersJson.push_back(*it);
    }
    responseJson["players"] = playersJson;
    responseJson["questionCount"] = response.questionCount;
    responseJson["answerTimeout"] = response.answerTimeout;
    std::string responseStr = responseJson.dump();
    return JsonResponsePacketSerializer::serializeResponse(responseStr, GET_ROOM_STATE_CODE);
}

Buffer JsonResponsePacketSerializer::serializeResponse(LeaveRoomResponse response)
{
    json responseJson;
    responseJson["status"] = response.status;
    std::string responseStr = responseJson.dump();
    return JsonResponsePacketSerializer::serializeResponse(responseStr, LEAVE_ROOM_CODE);
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
