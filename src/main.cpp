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
        throw std::runtime_error("Invalid UUID. Could not convert str to UUID");
    }
}


static std::string UUIDToStr(uuid_t& uuid) {
    char str[37]; // UUIDs are 36 characters + null terminator
    uuid_unparse(uuid, str);
    return std::string(str);
}


int main(int argc, char* argv[]) {

    crow::SimpleApp app;

    std::unordered_map<std::string, std::unique_ptr<Game>> gamesDatabase;
    std::unordered_map<std::string, std::unique_ptr<Player>> players; 
    std::unique_ptr<Game> pGame;
    std::unique_ptr<Board> pBoard;
    std::unique_ptr<BoardRules> pBoardRules;
    std::unique_ptr<Player> pWhitePlayer;
    std::unique_ptr<Player> pBlackPlayer;

    CROW_ROUTE(app, "/game/startNew")
    .methods("POST"_method)
    ([&gamesDatabase, &players, &pWhitePlayer](const crow::request& req) {
        json status;

        try {
            auto jsonBody = json::parse(req.body);

            uuid_t cookie;
            generateUUID(cookie);
            std::string cookieStr = uuidToString(cookie); 

            pWhitePlayer = std::make_unique<Player>(Color::WHITE);

            if (players.find(cookieStr) == players.end()) {
                players[cookieStr] = std::move(pWhitePlayer);
            } else {throw std::runtime_error("Error UUID. Player exists");}

            uuid_t gameID;
            generateUUID(gameID);
            std::string gameIDStr = uuidToString(gameID);

            if (gamesDatabase.find(gameIDStr) == gamesDatabase.end()) {
                gamesDatabase[gameIDStr] = nullptr;
            } else {throw std::runtime_error("Error UUID. Game exists");}

            status["status"] = GameStateToInt(GameState::WAITING_FOR_OPPONENT);

            crow::response response(200, status.dump());
            response.set_header("Set-Cookie", "gameID=" + gameIDStr + "; Path=/; HttpOnly");
            response.add_header("Set-Cookie", "PlayerToken=" + cookieStr + "; Path=/; HttpOnly");
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
    ([&gamesDatabase, &pGame, &pBoard, &pBoardRules, &players, &pWhitePlayer, &pBlackPlayer](const crow::request& req) {
        json status;

        try {
            auto jsonBody = json::parse(req.body);

            if (!jsonBody.contains("gameID")) {
                return crow::response(400, "Missing fields in request.");
            }
            const std::string gameID = jsonBody["gameID"];

            auto it = gamesDatabase.find(gameID);
            if (it == gamesDatabase.end() || it->second != nullptr) {
                return crow::response(400, "Game not found or game is not waiting for an opponent.");
            }

            pBlackPlayer = std::make_unique<Player>(Color::BLACK);

            uuid_t cookie;
            generateUUID(cookie);
            std::string cookieStr = uuidToString(cookie); 

            if (players.find(cookieStr) == players.end()) {
                players[cookieStr] = std::move(pBlackPlayer);
            } else {throw std::runtime_error("Error UUID. Player exists");}


            pBoard = std::make_unique<Board>();
            pBoardRules = std::make_unique<BoardRules>();
            pGame = std::make_unique<Game>(pWhitePlayer.get(), pBlackPlayer.get(), pBoard.get(), pBoardRules.get());

            it->second = std::move(pGame);

            status["status"] = GameStateToInt(it->second->getGameState());

            crow::response response(200, status.dump());
            response.set_header("Set-Cookie", "gameID=" + gameID + "; Path=/; HttpOnly");
            response.add_header("Set-Cookie", "PlayerToken=" + cookieStr + "; Path=/; HttpOnly");
            return response;
            
        } catch(const json::exception& e) {
            return crow::response(400, "Invalid JSON format.");
        } catch(const std::exception& e) {
            status["status"] = "Internal server error";
            status["message"] = e.what();
            return crow::response(500, status.dump());
        }
    });

    CROW_ROUTE(app, "/game/select/horcruxe")
    .methods("POST"_method)
    ([&gamesDatabase, &players](const crow::request& req) {
        // Need to consider edge case if horcruxe ID is never received
        json status;

        try {
            auto jsonBody = json::parse(req.body);

            // Validate required fields
            if (!jsonBody.contains("horcruxeID") 
            || !jsonBody.contains("cookie")
            || !jsonBody.contains("gameID")) {
                return crow::response(400, "Missing fields in request.");
            }

            const int horcruxeID = jsonBody["horcruxeID"];
            const std::string cookie = jsonBody["cookie"];
            const std::string gameID = jsonBody["gameID"];

            auto p_it = players.find(cookie);
            if (p_it == players.end()) {
                return crow::response(400, "Player not found.");
            }

            Player* pPlayer = p_it->second.get();

            auto it = gamesDatabase.find(gameID);
            if (it == gamesDatabase.end() || it->second != nullptr) {
                return crow::response(400, "Game not found or game is not waiting for an opponent.");
            }

            pPlayer->setHorcruxeID(horcruxeID);

            status["status"] = GameStateToInt(it->second->getGameState());

            crow::response response(200, status.dump());
            response.set_header("Set-Cookie", "gameID=" + gameID + "; Path=/; HttpOnly");
            response.add_header("Set-Cookie", "PlayerToken=" + cookie + "; Path=/; HttpOnly");

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


    CROW_ROUTE(app, "/game/guess/horcruxe")
    .methods("POST"_method)
    ([&gamesDatabase, &pGame, &players](const crow::request& req) {
        // Need to consider edge case if horcruxe ID is never received
        json status;

        try {
            auto jsonBody = json::parse(req.body);

            // Validate required fields
            if (!jsonBody.contains("horcruxeID") 
            || !jsonBody.contains("cookie")
            || !jsonBody.contains("gameID")) {
                return crow::response(400, "Missing fields in request.");
            }

            const int horcruxeID = jsonBody["horcruxeID"];
            const std::string cookie = jsonBody["cookie"];
            const std::string gameID = jsonBody["gameID"];

            auto p_it = players.find(cookie);
            if (p_it == players.end()) {
                return crow::response(400, "Player not found.");
            }

            Player* pPlayer = p_it->second.get();

            auto it = gamesDatabase.find(gameID);
            if (it == gamesDatabase.end() || it->second != nullptr) {
                return crow::response(400, "Game not found or game is not waiting for an opponent.");
            }

            pGame->horcruxeGuess(horcruxeID, pPlayer);
            
            status["status"] = GameStateToInt(it->second->getGameState());

            crow::response response(200, status.dump());
            response.set_header("Set-Cookie", "gameID=" + gameID + "; Path=/; HttpOnly");
            response.add_header("Set-Cookie", "PlayerToken=" + cookie + "; Path=/; HttpOnly");

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



    CROW_ROUTE(app, "/game/move")
    .methods("POST"_method)
    ([&gamesDatabase, &players](const crow::request& req) {
        json status; 

        try {
            auto jsonBody = json::parse(req.body);

            if ((!jsonBody.contains("gameID") 
            || !jsonBody.contains("cookie"))) {
                return crow::response(400, "Missing fields in request.");
            }

            const std::string cookie = jsonBody["cookie"];
            const std::string gameID = jsonBody["gameID"];

            auto p_it = players.find(cookie);
            if (p_it == players.end()) {
                return crow::response(400, "Player not found.");
            }

            Player* pPlayer = p_it->second.get();

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
            game->movePiece(Move(pPiece, from, to), pPlayer);

            status["status"] = GameStateToInt(it->second->getGameState());

            crow::response response(200, status.dump());
            response.set_header("Set-Cookie", "gameID=" + gameID + "; Path=/; HttpOnly");
            response.add_header("Set-Cookie", "PlayerToken=" + cookie + "; Path=/; HttpOnly");
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
    ([&gamesDatabase](std::string gameID) {
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


    CROW_ROUTE(app, "/game/board/<string>")
    .methods("GET"_method)
    ([&gamesDatabase, &pGame](std::string gameID) {
        json status;

        auto it = gamesDatabase.find(gameID);
        if (it == gamesDatabase.end() || !it->second) {
            return crow::response(400, "Game not found or game is not waiting for an opponent.");
        }

        json squaresJson;
        for (const auto& [pos, square] : pGame->getBoard()->squares) {
            json squareJson;
            squareJson["position"] = { {"file", pos.getFile()}, {"rank", pos.getRank()} };
            
            if(square->getPiece()) {
                squareJson["piece_id"] = square->getPiece()->getID();
            }
            squaresJson.push_back(squareJson);
        }

        status["squares"] = squaresJson;

        crow::response response(200, status.dump());
        response.set_header("Set-Cookie", "gameID=" + gameID + "; Path=/; HttpOnly");
        return response;
    });


    CROW_ROUTE(app, "/game/positions")
    .methods("POST"_method)
    ([&gamesDatabase, &pGame, &players](const crow::request& req) {
        json status;

        try {
            auto jsonBody = json::parse(req.body);

            if (!jsonBody.contains("gameID") 
            || !jsonBody.contains("cookie")) {
                return crow::response(400, "Missing fields in request.");
            }

            const std::string gameID = jsonBody["gameID"];
            const std::string cookie = jsonBody["cookie"];

            auto p_it = players.find(cookie);
            if (p_it == players.end()) {
                return crow::response(400, "Player not found.");
            }

            Player* pPlayer = p_it->second.get();

            auto it = gamesDatabase.find(gameID);
            if (it == gamesDatabase.end() || !it->second) {
                return crow::response(400, "Game not found or game is not waiting for an opponent.");
            }

            int pieceID = jsonBody["pieceID"];
            const IPiece* pPiece {pGame->getPieceFromID(pieceID)};

            if (pPiece->getColor() != pPlayer->getColor()) {
                throw std::runtime_error("Player color does not match piece color");
            }
            
            std::unordered_set<Position> position = it->second->getAvailablePositions(const_cast<IPiece*>(pPiece));
            
            json jsonObjs;
            for (const auto& pos : position) {
                json jsonObj;
                jsonObj["rank"] = pos.getRank();
                jsonObj["file"] = pos.getFile();
                jsonObjs.push_back(jsonObj);
            }

            status["possiblePositions"] = jsonObjs;

            crow::response response(200, status.dump());
            response.set_header("Set-Cookie", "gameID=" + gameID + "; Path=/; HttpOnly");
            response.add_header("Set-Cookie", "PlayerToken=" + cookie + "; Path=/; HttpOnly");
            return response;

        } catch(const json::exception& e) {
            return crow::response(400, "Invalid JSON format.");
        } catch(const std::exception& e) {
            status["status"] = "Internal server error";
            status["message"] = e.what();
            return crow::response(500, status.dump());
        }
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

    const char* port_str = std::getenv("PORT");
    int port = port_str ? std::stoi(port_str) : 8080;
    
    app.port(port)
       .multithreaded()
       .run();
};