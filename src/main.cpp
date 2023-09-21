#include "game.h"
#include "crow.h"
#include <uuid/uuid.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;


/* Helper Functions */

static int GameStateToInt(GameState state) {
    switch (state) {
        case GameState::WAITING_FOR_OPPONENT:
            return static_cast<int>(GameState::WAITING_FOR_OPPONENT);
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


static int GameEndToInt(GameEndType state) {
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


static std::string uuidToString(const uuid_t& uuid) {
    char buffer[37];
    uuid_unparse(uuid, buffer);
    return std::string(buffer);
}


static void generateUUID(uuid_t& uuid) {
    uuid_generate_random(uuid);
}


static void StrToUUID(std::string str, uuid_t gameID) {
    if (uuid_parse(str.c_str(), gameID) != 0) {
        std::runtime_error("Invalid UUID. Could not convert str to UUID");
    }
}


static std::string UUIDToStr(uuid_t& uuid) {
    char str[37]; // UUIDs are 36 characters + null terminator
    uuid_unparse(uuid, str);
    return std::string(str);
}


int main(int argc, char* argv[]) {

    crow::SimpleApp app;

    std::map<std::string, std::unique_ptr<Game>> gamesDatabase;
    std::unique_ptr<Game> pGame;
    std::unique_ptr<Board> pBoard;
    std::unique_ptr<BoardRules> pBoardRules;
    std::unique_ptr<Player> pPlayer1;
    std::unique_ptr<Player> pPlayer2;

    CROW_ROUTE(app, "/game/startNew")
    .methods("POST"_method)
    ([&gamesDatabase, &pPlayer1](const crow::request& req) {
        json status;

        try {
            auto jsonBody = json::parse(req.body);

            if(!jsonBody.contains("horcruxeID") || !jsonBody.contains("color")) {
                // Missing fields
                return crow::response(400, "Missing fields in request.");
            }

            int horcruxeID = jsonBody["horcruxeID"];
            Color color = static_cast<Color>(jsonBody["color"]);

            pPlayer1 = std::make_unique<Player>(horcruxeID, color);

            // Generate UUID and store in gamesDatabase
            uuid_t gameID;
            generateUUID(gameID);
            std::string gameIDStr = uuidToString(gameID);

            if (gamesDatabase.find(gameIDStr) == gamesDatabase.end()) {
                gamesDatabase[gameIDStr] = nullptr;
            } else {std::runtime_error("Error UUID. Game exists");}

            status["status"] = GameStateToInt(GameState::WAITING_FOR_OPPONENT);

            crow::response response(200, status.dump());
            response.set_header("Set-Cookie", "gameID=" + gameIDStr + "; Path=/; HttpOnly");
            return response;
            
        } catch(const json::exception& e) {
            // Error during JSON parsing
            return crow::response(400, "Invalid JSON format.");
        } catch(const std::exception& e) {
            status["status"] = "Internal server error";
            status["message"] = e.what();
            return crow::response(500, status.dump());
        }
    });


    CROW_ROUTE(app, "/game/join")
    .methods("POST"_method)
    ([&gamesDatabase, &pGame, &pBoard, &pBoardRules, &pPlayer1, &pPlayer2](const crow::request& req) {
        json status;

        try {
            auto jsonBody = json::parse(req.body);

            // Validate required fields
            if (!jsonBody.contains("horcruxeID") || !jsonBody.contains("color") || !jsonBody.contains("gameID")) {
                return crow::response(400, "Missing fields in request.");
            }

            int horcruxeID = jsonBody["horcruxeID"];
            Color color = static_cast<Color>(jsonBody["color"]);

            std::string gameID = jsonBody["gameID"];

            auto it = gamesDatabase.find(gameID);
            if (it == gamesDatabase.end() || it->second != nullptr) {
                return crow::response(400, "Game not found or game is not waiting for an opponent.");
            }

            // Create game
            pPlayer2 = std::make_unique<Player>(horcruxeID, color);
            pBoard = std::make_unique<Board>();
            pBoardRules = std::make_unique<BoardRules>();
            pGame = std::make_unique<Game>(*pPlayer1, *pPlayer2, pBoard.get(), pBoardRules.get());

            it->second = std::move(pGame);

            status["status"] = GameStateToInt(it->second->getGameState());

            crow::response response(200, status.dump());
            response.set_header("Set-Cookie", "gameID=" + gameID + "; Path=/; HttpOnly");
            return response;
            
        } catch(const json::exception& e) {
            return crow::response(400, "Invalid JSON format.");
        } catch(const std::exception& e) {
            status["status"] = "Internal server error";
            status["message"] = e.what();
            return crow::response(500, status.dump());
        }
    });


    CROW_ROUTE(app, "/game/move")
    .methods("POST"_method)
    ([&gamesDatabase, &pGame](const crow::request& req) {
        json status; 

        try {
            auto jsonBody = json::parse(req.body);

            std::string gameID = jsonBody["gameID"];

            Position from(
                jsonBody["from"]["file"].get<std::string>()[0],
                jsonBody["from"]["rank"].get<int>()
            );
            Position to(
                jsonBody["to"]["file"].get<std::string>()[0], 
                jsonBody["to"]["rank"].get<int>()
            );

            auto it = gamesDatabase.find(gameID);
            if (it == gamesDatabase.end() || !it->second) {
                throw std::runtime_error("Invalid game ID provided.");
            }

            Game* game = it->second.get();

            const IPiece* pPiece = game->getBoard()->getSquare(from)->getPiece();
            game->movePiece(Move(pPiece, from, to));

            status["status"] = GameStateToInt(it->second->getGameState());

            crow::response response(200, status.dump());
            response.set_header("Set-Cookie", "gameID=" + gameID + "; Path=/; HttpOnly");
            return response;

        } catch(const json::exception& e) {
            return crow::response(400, "Invalid JSON format.");
        } catch(const std::exception& e) {
            status["status"] = "Internal server error";
            status["message"] = e.what();
            return crow::response(500, status.dump());
        }
    });


    CROW_ROUTE(app, "/game/state/<string>")
    .methods("GET"_method)
    ([&gamesDatabase, &pGame](std::string gameID) {
        json status;

        auto it = gamesDatabase.find(gameID);
        if (it == gamesDatabase.end() || !it->second) {
            return crow::response(400, "Game not found or game is not waiting for an opponent.");
        }

        status["status"] = GameStateToInt(it->second->getGameState());

        crow::response response(200, status.dump());
        response.set_header("Set-Cookie", "gameID=" + gameID + "; Path=/; HttpOnly");
        return response;
    });


    CROW_ROUTE(app, "/game/checkGameOver/<string>")
    .methods("GET"_method)
    ([&gamesDatabase](std::string gameID) {
        // Check the game state for the given game_id
        // Return a JSON or response indicating if the game is over and, if so, the reason and the winner
        json status;

        auto it = gamesDatabase.find(gameID);
        if (it == gamesDatabase.end() || !it->second) {
            return crow::response(400, "Game not found or game is not waiting for an opponent.");
        }

        status["status"] = GameStateToInt(it->second->getGameState());

        crow::response response(200, status.dump());
        response.set_header("Set-Cookie", "gameID=" + gameID + "; Path=/; HttpOnly");
        return response;
    });


    CROW_ROUTE(app, "/game/currentTurn/<string>")
    .methods("GET"_method)
    ([&gamesDatabase](std::string gameID) {
        // Return whose turn it is for the given game_id
        json status;

        auto it = gamesDatabase.find(gameID);
        if (it == gamesDatabase.end() || !it->second) {
            return crow::response(400, "Game not found or game is not waiting for an opponent.");
        }

        status["status"] = GameStateToInt(it->second->getGameState());

        crow::response response(200, status.dump());
        response.set_header("Set-Cookie", "gameID=" + gameID + "; Path=/; HttpOnly");
        return response;
    });


    CROW_ROUTE(app, "/game/result/<string>")
    .methods("GET"_method)
    ([&gamesDatabase](std::string gameID) {
        // Return the result (winner, reason for win, etc.) for the given game_id
        json status;

        auto it = gamesDatabase.find(gameID);
        if (it == gamesDatabase.end() || !it->second) {
            return crow::response(400, "Game not found or game is not waiting for an opponent.");
        }

        status["status"] = GameEndToInt(it->second->getGameResult());

        crow::response response(200, status.dump());
        response.set_header("Set-Cookie", "gameID=" + gameID + "; Path=/; HttpOnly");
        return response;
    });
};