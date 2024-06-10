#include "game.h"
#include "crow.h"
#include <sstream>
#include <uuid/uuid.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

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

std::string generatePlayerToken() {
    uuid_t cookie;
    generateUUID(cookie);
    return uuidToString(cookie);
}

void createGame(sql::Connection* con, const std::string& gameID) {
    try {
        std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement("INSERT INTO games(id, state) VALUES (?, ?)"));
        pstmt->setString(1, gameID);
        pstmt->setInt(2, static_cast<int>(GameState::WAITING_FOR_OPPONENT));
        pstmt->execute();
    } catch (sql::SQLException& e) {
        std::cerr << "Error creating game: " << e.what() << std::endl;
        throw;
    }
}

void createPlayer(sql::Connection* con, const std::string& playerID, Color color) {
    try {
        std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement("INSERT INTO players(id, color) VALUES (?, ?)"));
        pstmt->setString(1, playerID);
        pstmt->setInt(2, static_cast<int>(color));
        pstmt->execute();
    } catch (sql::SQLException& e) {
        std::cerr << "Error creating player: " << e.what() << std::endl;
        throw;
    }
}

void updateGameState(sql::Connection* con, const std::string& gameID, GameState state) {
    try {
        std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement("UPDATE games SET state = ? WHERE id = ?"));
        pstmt->setInt(1, static_cast<int>(state));
        pstmt->setString(2, gameID);
        pstmt->execute();
    } catch (sql::SQLException& e) {
        std::cerr << "Error updating game state: " << e.what() << std::endl;
        throw;
    }
}

std::string generateGameID() {
    uuid_t gameID;
    generateUUID(gameID);
    return uuidToString(gameID);
}

bool validateGame(sql::Connection* con, const std::string& gameID) {
    try {
        std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement("SELECT COUNT(*) AS count FROM games WHERE id = ?"));
        pstmt->setString(1, gameID);
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        if (res->next()) {
            return res->getInt("count") > 0;
        }
        return false;
    } catch (sql::SQLException& e) {
        std::cerr << "Error validating game: " << e.what() << std::endl;
        throw;
    }
}

void validateJsonFields(const json& j, const std::initializer_list<std::string>& fields) {
    for (const auto& field : fields) {
        if (!j.contains(field)) {
            throw std::runtime_error("Missing field in JSON: " + field);
        }
    }
}

std::shared_ptr<Player> findPlayer(sql::Connection* con, const std::string& playerID) {
    try {
        std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement("SELECT * FROM players WHERE id = ?"));
        pstmt->setString(1, playerID);
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        if (res->next()) {
            // Construct player object from database data
            return std::make_shared<Player>(res);
        } else {
            throw std::runtime_error("Player not found");
        }
    } catch (sql::SQLException& e) {
        std::cerr << "Error finding player: " << e.what() << std::endl;
        throw;
    }
}

std::shared_ptr<Game> findGame(sql::Connection* con, const std::string& gameID) {
    try {
        std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement("SELECT * FROM games WHERE id = ?"));
        pstmt->setString(1, gameID);
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        if (res->next()) {
            // Construct game from database result
            return std::make_shared<Game>(res);
        } else {
            throw std::runtime_error("Game not found");
        }
    } catch (sql::SQLException& e) {
        std::cerr << "Error finding game: " << e.what() << std::endl;
        throw;
    }
}

std::unique_ptr<Game> initializeGame(std::shared_ptr<Player> whitePlayer, std::shared_ptr<Player> blackPlayer) {
    // Assume this method now initializes a game object based on player info and possibly existing game states.
    auto pGame = std::make_unique<Game>(whitePlayer, blackPlayer);
    pGame->startGame();  // This method should be designed to handle game initialization logic.
    return pGame;
}

void removePlayer(sql::Connection* con, const std::string& playerID) {
    try {
        std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement("DELETE FROM players WHERE id = ?"));
        pstmt->setString(1, playerID);
        pstmt->execute();
    } catch (sql::SQLException& e) {
        std::cerr << "Error removing player: " << e.what() << std::endl;
        throw;
    }
}

void killGame(sql::Connection* con, const std::string& gameID) {
    try {
        std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement("DELETE FROM games WHERE id = ?"));
        pstmt->setString(1, gameID);
        pstmt->execute();
    } catch (sql::SQLException& e) {
        std::cerr << "Error killing game: " << e.what() << std::endl;
        throw;
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

void updatePlayerHorcrux(sql::Connection* con, const std::string& playerID, int horcruxID) {
    try {
        std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement("UPDATE players SET horcrux_id = ? WHERE id = ?"));
        pstmt->setInt(1, horcruxID);
        pstmt->setString(2, playerID);
        pstmt->execute();
    } catch (sql::SQLException& e) {
        std::cerr << "Error updating player's horcrux: " << e.what() << std::endl;
        throw;
    }
}