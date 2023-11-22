#include "helper.hpp"

/* Helper Function Definitions */

enum class MoveStatus {
    INVALID,
    VALID
};

int GameStateToInt(GameState state) {
    std::cout << "Game State: " << int(state) << std::endl;
    switch (state) {
        case GameState::WAITING_FOR_OPPONENT:
            return static_cast<int>(GameState::WAITING_FOR_OPPONENT);
        case GameState::CHOOSING_HORCRUX:
            return static_cast<int>(GameState::CHOOSING_HORCRUX);
        case GameState::WHITE_MOVE:
            return static_cast<int>(GameState::WHITE_MOVE);
        case GameState::BLACK_MOVE:
            return static_cast<int>(GameState::BLACK_MOVE);
        case GameState::ENDED:
            return static_cast<int>(GameState::ENDED);
        default:
            throw std::runtime_error("Can't stringify. Not a valid GameState");
    }
};


int GameEndToInt(GameEndType state) {
    switch (state) {
        case GameEndType::WHITE_WIN:
            return static_cast<int>(GameEndType::WHITE_WIN);
        case GameEndType::BLACK_WIN:
            return static_cast<int>(GameEndType::BLACK_WIN);
        case GameEndType::DRAW:
            return static_cast<int>(GameEndType::DRAW);
        case GameEndType::STALEMATE:
            return static_cast<int>(GameEndType::STALEMATE);
        default:
            throw std::runtime_error("Can't stringify. Not a valid GameState");
    }
};

std::string uuidToString(const uuid_t& uuid) {
    char buffer[37];
    uuid_unparse(uuid, buffer);
    return std::string(buffer);
}


void generateUUID(uuid_t& uuid) {
    uuid_generate_random(uuid);
}


void StrToUUID(std::string str, uuid_t gameID) {
    if (uuid_parse(str.c_str(), gameID) != 0) {
        throw std::runtime_error("Invalid UUID. Could not convert str to UUID");
    }
}

/*
std::unordered_map<std::string, std::string> parseCookies(const std::string& cookie_header) {
    std::unordered_map<std::string, std::string> cookies;
    std::istringstream cookie_stream(cookie_header);
    std::string pair;
    while (std::getline(cookie_stream, pair, ';')) {
        size_t pos = pair.find('=');
        if (pos != std::string::npos) {
            std::string name = pair.substr(0, pos);
            std::string value = pair.substr(pos + 1);
            
            name.erase(name.begin(), std::find_if(name.begin(), name.end(), [](int ch) { return !std::isspace(ch); }));
            value.erase(value.begin(), std::find_if(value.begin(), value.end(), [](int ch) { return !std::isspace(ch); }));
            
            cookies[name] = value;
        }
    }
    return cookies;
}
*/

std::string getGameID(std::unordered_map<std::string, std::string>& cookies) {
    auto it = cookies.find("gameID");
    if(it != cookies.end())
        return it->second; // note that it->second is used to get the value associated with the key "gameID"
    else
        throw std::runtime_error("Game ID not found in cookies");
}


std::string getPlayerID(std::unordered_map<std::string, std::string>& cookies) {
    auto it = cookies.find("playerID");
    if(it != cookies.end())
        return it->second; 
    else
        throw std::runtime_error("Player ID not found in cookies");
}


std::string generatePlayerToken() {
    uuid_t cookie;
    generateUUID(cookie);
    return uuidToString(cookie);
}


void createPlayer(Players& players, const std::string& cookieStr, Color color, std::shared_ptr<Player>& pPlayer) {
    pPlayer = std::make_shared<Player>(color);
    if (!players.emplace(cookieStr, pPlayer).second)
        throw std::runtime_error("Error UUID. Player exists");
}


std::string generateGameID() {
    uuid_t gameID;
    generateUUID(gameID);
    return uuidToString(gameID);
}


void createGame(GamesDatabase& gamesDatabase, const std::string& gameIDStr) {
    if (!gamesDatabase.emplace(gameIDStr, nullptr).second)
        throw std::runtime_error("Error UUID. Game exists");
}

void validateGame(const GamesDatabase& gamesDatabase, const std::string& gameID) {
    // Original validation logic
    auto it = gamesDatabase.find(gameID);
    if (it == gamesDatabase.end() || it->second != nullptr)
        throw std::runtime_error("Game not found or game is not waiting for an opponent.");
}



void validateJsonFields(const json& j, const std::initializer_list<std::string>& fields) {
    for (const auto& field : fields) {
        if (!j.contains(field)) {
            throw std::runtime_error("Missing field in JSON: " + field);
        }
    }
}


Player* findPlayer(const Players& players, const std::string& playerID) {
    auto it = players.find(playerID);
    if (it == players.end()) throw std::runtime_error("Player not found.");
    std::cout << "Player Color: " << int(it->second->getColor()) << std::endl;
    return it->second.get();
}


Game* findGame(const GamesDatabase& gamesDatabase, const std::string& gameID) {
    auto it = gamesDatabase.find(gameID);
    if (it == gamesDatabase.end()) throw std::runtime_error("Game not found or not in a joinable state.");
    return it->second.get();
}


std::unique_ptr<Game> initializeGame(Player* whitePlayer, Player* blackPlayer, Board* board, BoardRules* boardRules) {
    auto pGame = std::make_unique<Game>(whitePlayer, blackPlayer, board, boardRules);
    pGame->startGame();
    return pGame;
}

void killGame(GamesDatabase& gamesDatabase, const std::string& gameID) {
    auto it = gamesDatabase.find(gameID);
    if (it != gamesDatabase.end()) {
        gamesDatabase.erase(it);
    }
}

void removePlayer(Players& players, std::string& playerID) {
    auto it = players.find(playerID);
    if (it != players.end()) {
        players.erase(it);
    }
}

std::pair<char, int> parseFileAndRank(const std::string& input) {
    std::istringstream iss(input);
    std::string fileStr;
    std::string rankStr;
    std::getline(iss, fileStr, ',');
    std::getline(iss, rankStr);

    if (fileStr.empty() || rankStr.empty()) {
        throw std::invalid_argument("Invalid input string");
    }

    char file = fileStr.length() == 1 ? fileStr[0] : throw std::invalid_argument("Invalid file character");
    int rank = std::stoi(rankStr); // This might throw std::invalid_argument if conversion fails

    return {file, rank};
}

crow::response createErrorResponse(const std::exception& e) {
    json status;
    status["status"] = ERROR_STATUS;
    status["message"] = e.what();
    crow::response response(500, status.dump());
    response.set_header("Content-type", "application/json");
    //addCorsHeader(response);
    return response;
}