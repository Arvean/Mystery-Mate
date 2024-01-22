#include "game.h"
#include "helper.hpp"
#include "crow.h"
#include "crow/middlewares/cors.h"
#include "crow/middlewares/cookie_parser.h"
#include <sstream>
#include <uuid/uuid.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

int main(int argc, char* argv[]) {

    // Enable CORS
    crow::App<crow::CORSHandler, crow::CookieParser> app;

    GamesDatabase gamesDatabase;
    Players players; 
    std::unique_ptr<Game> pGame;
    std::unique_ptr<Board> pBoard;
    std::unique_ptr<BoardRules> pBoardRules;
    std::shared_ptr<Player> pWhitePlayer;
    std::shared_ptr<Player> pBlackPlayer;

    // Customize CORS
    auto& cors = app.get_middleware<crow::CORSHandler>();

    cors.global() // Setting CORS policies for all routes
        .methods("GET"_method, "POST"_method, "PUT"_method, "DELETE"_method, "OPTIONS"_method)
        .headers("Content-Type","Upgrade-Insecure-Requests")
        .origin("http://localhost:3000") // Allowing your frontend's origin
        .allow_credentials();

    CROW_ROUTE(app, "/")
    ([]() {
        return "Check Access-Control-Allow-Methods header";
    });

    CROW_ROUTE(app, "/cors")
    ([]() {
        return "Check Access-Control-Allow-Origin header";
    });

    CROW_ROUTE(app, "/game/startNew")
    .methods("GET"_method)
    ([&app, &gamesDatabase, &players, &pWhitePlayer] (const crow::request& req){
        crow::response res;
        json status;

        try {
            std::string cookieStr = generatePlayerToken();
            std::cout << "Player ID Generated: " << cookieStr << std::endl;
            createPlayer(players, cookieStr, Color::WHITE, pWhitePlayer);
            
            const auto gameIDStr = generateGameID();
            std::cout << "Game ID Generated: " << gameIDStr << std::endl;
            createGame(gamesDatabase, gameIDStr);

            status["status"] = GameStateToInt(GameState::WAITING_FOR_OPPONENT);
            
            res.code = 200;
            res.body = status.dump();

            auto& ctx = app.get_context<crow::CookieParser>(req);
            // Store cookies with set_cookie
            ctx.set_cookie("gameID", gameIDStr)
            .path("/")
            .httponly();;

            ctx.set_cookie("playerID", cookieStr)
            .path("/")
            .httponly();
        }
        catch (const json::exception&) {
            res.code = 400;
            res.body = "Invalid JSON format.";
        }
        catch (const std::exception& e) {
            res.code = 500;
            res.body = "Internal server error";
        }
        res.set_header("Content-type", "application/json");
        return res;
    });


    CROW_ROUTE(app, "/game/join")
    .methods("POST"_method)
    ([&app, &gamesDatabase, &pGame, &pBoard, &pBoardRules, &players, &pWhitePlayer, &pBlackPlayer](const crow::request& req) {
        json status;
        try {
            auto jsonBody = json::parse(req.body);
            validateJsonFields(jsonBody, {"gameID"}); // Use a function to validate required fields

            auto gameID = jsonBody["gameID"].get<std::string>();

            validateGame(gamesDatabase, gameID);

            const auto player = generatePlayerToken();
            std::cout << "Second Player Token: " << player << std::endl;
            createPlayer(players, player, Color::BLACK, pBlackPlayer);

            pBoard = std::make_unique<Board>();
            pBoardRules = std::make_unique<BoardRules>();
            pGame = initializeGame(pWhitePlayer.get(), pBlackPlayer.get(), pBoard.get(), pBoardRules.get());

            gamesDatabase[gameID] = std::move(pGame);

            auto& retrievedGamePtr = gamesDatabase[gameID]; // gets the unique_ptr from the map

            if (!retrievedGamePtr) {
                throw std::runtime_error("Failed to retrieve the game from the database.");
            }

            auto& game = gamesDatabase[gameID];

            status["status"] = GameStateToInt(game->getGameState());

            crow::response response(200, status.dump());

            auto& ctx = app.get_context<crow::CookieParser>(req);

            ctx.set_cookie("gameID", gameID)
            .path("/")
            .httponly();

            ctx.set_cookie("playerID", player)
            .path("/")
            .httponly();

            response.set_header("Content-type", "application/json");
            return response;
        } catch(const json::exception& e) {
            crow::response response(400, "Invalid JSON format.");
            response.set_header("Content-type", "application/json");
            return response;
        } catch(const std::exception& e) {
            return createErrorResponse(e);
        }
    });

    CROW_ROUTE(app, "/game/player")
    .methods("GET"_method)
    ([&app, &gamesDatabase, &players](const crow::request& req) {
        json status;
        try {
            auto& ctx = app.get_context<crow::CookieParser>(req);
            // Read cookies with get_cookie
            auto gameID = ctx.get_cookie("gameID");
            auto playerID = ctx.get_cookie("playerID");
            
            auto player = findPlayer(players, playerID);

            status["playerColor"] = player->getColor();

            crow::response response(200, status.dump());
            response.set_header("Content-type", "application/json");
            return response;
        } catch(const nlohmann::json::exception& e) {
            crow::response response(400, "Invalid JSON format.");
            response.set_header("Content-type", "application/json");
            return response;
        } catch(const std::exception& e) {
            return createErrorResponse(e);
        }
    });

    CROW_ROUTE(app, "/game/select/horcrux")
    .methods("POST"_method)
    ([&app, &gamesDatabase, &players](const crow::request& req) {
        json status;

        try {
            auto& ctx = app.get_context<crow::CookieParser>(req);
            // Read cookies with get_cookie
            auto gameID = ctx.get_cookie("gameID");
            auto playerID = ctx.get_cookie("playerID");

            std::cout << "Received request body: " << req.body << std::endl;
            
            Player* pPlayer = findPlayer(players, playerID); // assuming you've created a function to find a player
            std::cout << "Player Color: " << int(pPlayer->getColor()) << std::endl;

            Game* pGame = findGame(gamesDatabase, gameID); // assuming you've created a function to find a game
            
            auto [file, rank] = parseFileAndRank(req.body);
            const IPiece* horcrux = pGame->getPieceFromPosition(Position(file, rank));
            if (horcrux) {
                int horcruxID = horcrux->getID();
                status["horcruxID"] = horcruxID;
                pPlayer->setHorcruxID(horcruxID);
                pGame->checkHorcruxSet();
            } else {
                throw std::runtime_error("Could not find piece on the selected square");
            }

            status["status"] = GameStateToInt(pGame->getGameState());
            
            crow::response response(200, status.dump());
            response.set_header("Content-type", "application/json");
            return response;
        } catch(const nlohmann::json::exception& e) {
            crow::response response(400, "Parse error: " + std::string(e.what()));
            response.set_header("Content-type", "application/json");
            return response;
        } catch(const std::exception& e) {
            return createErrorResponse(e);
        }
    });

    CROW_ROUTE(app, "/game/guess/horcrux")
    .methods("POST"_method)
    ([&app, &gamesDatabase, &pGame, &players](const crow::request& req) {
        json status;
        try {
            auto& ctx = app.get_context<crow::CookieParser>(req);
            // Read cookies with get_cookie
            auto gameID = ctx.get_cookie("gameID");
            auto playerID = ctx.get_cookie("playerID");

            // Split the body into 'from' and 'to' parts using semicolon as the delimiter
            std::string requestBody = req.body;

            // Use the parseFileAndRank function to extract 'from' and 'to' positions
            auto [squareFile, squareRank] = parseFileAndRank(requestBody);

            // Convert to Position objects
            Position from(squareFile, squareRank);

            Game* pGame = findGame(gamesDatabase, gameID); // Using the helper function to find a game
            Player* pPlayer = findPlayer(players, playerID); // Use a function to find a player

            const IPiece* pPiece = pGame->getBoard()->getSquare(from)->getPiece();

            // Perform the guess and update the status
            Player* pPlayerToCheck;
            pPlayer->getColor() == Color::WHITE ? pPlayerToCheck = pGame->blackPlayer : pPlayerToCheck = pGame->whitePlayer;
            bool guessCorrect = pGame->horcruxGuess(pPiece->getID(), pPlayer, pPlayerToCheck);

            status["guess"] = guessCorrect;
            status["status"] = GameStateToInt(pGame->getGameState());

            crow::response response(200, status.dump());
            response.set_header("Content-type", "application/json");
            return response;
        } catch(const nlohmann::json::exception& e) {
            crow::response response(400, "Invalid JSON format.");
            response.set_header("Content-type", "application/json");
            return response;
        } catch(const std::exception& e) {
            return createErrorResponse(e);
        }
    });


    CROW_ROUTE(app, "/game/get/horcruxStatus")
    .methods("GET"_method)
    ([&app, &gamesDatabase, &players](const crow::request& req) {
        json status;
        try {
            auto& ctx = app.get_context<crow::CookieParser>(req);
            // Read cookies with get_cookie
            auto gameID = ctx.get_cookie("gameID");
            auto playerID = ctx.get_cookie("playerID");

            Game* pGame = findGame(gamesDatabase, gameID); // Using the helper function to find a game
            Player* pPlayer = findPlayer(players, playerID); // Use a function to find a player

            if (pGame->whitePlayer->getHorcruxFound() || pGame->whitePlayer->getHasKingBeenCaptured()) {
                status["whiteHasBeenGuessed"] = true;
                status["whiteFoundID"] = pGame->whitePlayer->getHorcruxID();
            } else {
                status["whiteHasBeenGuessed"] = false;
            }

            if (pGame->blackPlayer->getHorcruxFound() || pGame->blackPlayer->getHasKingBeenCaptured()) {
                status["blackHasBeenGuessed"] = true;
                status["blackFoundID"] = pGame->blackPlayer->getHorcruxID();
            } else {
                status["blackHasBeenGuessed"] = false;
            }

            status["horcruxID"] = pPlayer->getHorcruxID();

            crow::response response(200, status.dump());
            response.set_header("Content-type", "application/json");
            return response;
        } catch(const std::exception& e) {
            return createErrorResponse(e);
        }
    });


    CROW_ROUTE(app, "/game/move")
    .methods("POST"_method)
    ([&app, &gamesDatabase, &players](const crow::request& req) {
        json status; 

        try {
            auto& ctx = app.get_context<crow::CookieParser>(req);
            // Read cookies with get_cookie
            auto gameID = ctx.get_cookie("gameID");
            auto playerID = ctx.get_cookie("playerID");

            // Split the body into 'from' and 'to' parts using semicolon as the delimiter
            std::string requestBody = req.body;
            size_t delimiterPos = requestBody.find(';');
            if (delimiterPos == std::string::npos) {
                throw std::runtime_error("Invalid move format.");
            }

            std::string fromText = requestBody.substr(0, delimiterPos);
            std::string toText = requestBody.substr(delimiterPos + 1);

            // Use the parseFileAndRank function to extract 'from' and 'to' positions
            auto [fromFile, fromRank] = parseFileAndRank(fromText);
            auto [toFile, toRank] = parseFileAndRank(toText);

            // Convert to Position objects
            Position from(fromFile, fromRank);
            Position to(toFile, toRank);

            Game* pGame = findGame(gamesDatabase, gameID); // Using the helper function to find a game
            Player* pPlayer = findPlayer(players, playerID); // Use a function to find a player

            const IPiece* pPiece = pGame->getBoard()->getSquare(from)->getPiece();
            pGame->movePiece(Move(pPiece, from, to), pPlayer);
            
            status["status"] = GameStateToInt(pGame->getGameState());
            
            crow::response response(200, status.dump());
            response.set_header("Content-type", "application/json");
            return response;
        } catch(const nlohmann::json::exception& e) {
            crow::response response(400, "Invalid JSON format.");
            response.set_header("Content-type", "application/json");
            return response;
        } catch(const std::exception& e) {
            return createErrorResponse(e);
        }
    });


    CROW_ROUTE(app, "/game/state")
    .methods("GET"_method)
    ([&app, &gamesDatabase](const crow::request& req) {
        try {
            auto& ctx = app.get_context<crow::CookieParser>(req);
            // Read cookies with get_cookie
            auto gameID = ctx.get_cookie("gameID");
            auto playerID = ctx.get_cookie("playerID");

            Game* pGame = findGame(gamesDatabase, gameID); // Using the helper function to find a game

            json status;

            if (!pGame) { // Game is found (no error thrown) not created yet (Not a full game yet)
                status["status"] = GameStateToInt(GameState::WAITING_FOR_OPPONENT);
            } else {
                status["status"] = GameStateToInt(pGame->getGameState());
            }
            
            crow::response response(200, status.dump());
            response.set_header("Content-type", "application/json");
            return response;
        } catch(const std::exception& e) {
            return createErrorResponse(e);
        }
    });


    CROW_ROUTE(app, "/game/gameID")
    .methods("GET"_method)
    ([&app, &gamesDatabase](const crow::request& req) {
        json status;
        try {
            auto& ctx = app.get_context<crow::CookieParser>(req);
            // Read cookies with get_cookie
            auto gameID = ctx.get_cookie("gameID");
            auto playerID = ctx.get_cookie("playerID");
            status["status"] = gameID;

            crow::response response(200, status.dump());
            response.set_header("Content-type", "application/json");
            return response;
        } catch(const std::exception& e) {
            return createErrorResponse(e);
        }
    });


CROW_ROUTE(app, "/game/board")
.methods("GET"_method)
([&app, &gamesDatabase, &players](const crow::request& req) {
    try {
        auto& ctx = app.get_context<crow::CookieParser>(req);
        auto gameID = ctx.get_cookie("gameID");
        auto playerID = ctx.get_cookie("playerID");

        Game* pGame = findGame(gamesDatabase, gameID);
        Player* pPlayer = findPlayer(players, playerID); // Using the helper function to find a player

        json status;
        json squaresJson;
        bool isBlack = (pPlayer && pPlayer->getColor() == Color::BLACK); // Assuming Color::BLACK is the enum for black

        for (const auto& [pos, square] : pGame->getBoard()->squares) {
            char file = pos.getFile();
            int rank = pos.getRank();

            json squareJson;
            squareJson["position"] = { {"file", std::string(1, file)}, {"rank", rank} };

            if (square->getPiece()) {
                squareJson["piece"]["id"] = square->getPiece()->getID();
                squareJson["piece"]["type"] = square->getPiece()->getType();
                squareJson["piece"]["color"] = square->getPiece()->getColor();
            }
            squaresJson.push_back(squareJson);
        }

        status["squares"] = squaresJson;

        crow::response response(200, status.dump());
        response.set_header("Content-type", "application/json");
        return response;
    } catch(const std::exception& e) {
        return createErrorResponse(e);
    }
});



    CROW_ROUTE(app, "/game/positions")
    .methods("POST"_method)
    ([&app, &gamesDatabase, &players](const crow::request& req) {
        json status;
        try {
            auto& ctx = app.get_context<crow::CookieParser>(req);
            // Read cookies with get_cookie
            auto gameID = ctx.get_cookie("gameID");
            auto playerID = ctx.get_cookie("playerID");
            
            Player* pPlayer = findPlayer(players, playerID); // Using the helper function to find a player
            if (!pPlayer) return crow::response(400, "Player not found.");
            
            Game* pGame = findGame(gamesDatabase, gameID); // Using the helper function to find a game

            auto [file, rank] = parseFileAndRank(req.body);
            const Position from = Position(file, rank);
            const IPiece* piece = pGame->getPieceFromPosition(from);
                        
            if (!piece || piece->getColor() != pPlayer->getColor()) {
                throw std::runtime_error("Invalid piece or player color does not match piece color");
            }
            
            json jsonObjs;
            for (const auto& pos : pGame->getAvailablePositions(const_cast<IPiece*>(piece), from)) {
                json jsonObj;
                jsonObj["rank"] = pos.getRank();
                jsonObj["file"] = std::string(1, pos.getFile());
                jsonObjs.push_back(jsonObj);
            }

            status["possiblePositions"] = jsonObjs;
            
            crow::response response(200, status.dump());
            response.set_header("Content-type", "application/json");
            return response;
        } catch(const nlohmann::json::exception& e) {
            crow::response response(400, "Invalid JSON format.");
            response.set_header("Content-type", "application/json");
            return response;
        } catch(const std::exception& e) {
            return createErrorResponse(e);
        }
    });


    CROW_ROUTE(app, "/game/result")
    .methods("GET"_method)
    ([&app, &gamesDatabase](const crow::request& req) {
        json status;
        
        try {
            auto& ctx = app.get_context<crow::CookieParser>(req);
            // Read cookies with get_cookie
            auto gameID = ctx.get_cookie("gameID");
            auto playerID = ctx.get_cookie("playerID");
            
            Game* pGame = findGame(gamesDatabase, gameID);
            
            status["status"] = GameEndToInt(pGame->getGameResult());

            crow::response response(200, status.dump());
            response.set_header("Content-type", "application/json");
            return response;
        } catch(const std::exception& e) {
            return createErrorResponse(e);
        }
    });

    CROW_ROUTE(app, "/game/isGameInProgress")
    .methods("GET"_method)
    ([&app, &gamesDatabase](const crow::request& req) {
        json status;
        try {
            auto& ctx = app.get_context<crow::CookieParser>(req);
            // Read cookies with get_cookie
            auto gameID = ctx.get_cookie("gameID");

            /* Expect an error if can't find game */
            Game* pGame = findGame(gamesDatabase, gameID); // Using the helper function to find a game

            // Return the game state
            status["isInProgress"] = true;
            return crow::response(200, status.dump());

        } catch(const std::exception& e) {
            status["isInProgress"] = false;
            return crow::response(200, status.dump());
        }
    });

    CROW_ROUTE(app, "/game/numberOfHorcruxGuessesLeft")
    .methods("GET"_method)
    ([&app, &gamesDatabase, &players](const crow::request& req) {
        json status;
        try {
            auto& ctx = app.get_context<crow::CookieParser>(req);
            // Read cookies with get_cookie
            auto gameID = ctx.get_cookie("gameID");
            auto playerID = ctx.get_cookie("playerID");

            Player* pPlayer = findPlayer(players, playerID); // Using the helper function to find a player

            status["horcruxGuessesLeft"] = pPlayer->getNumberOfHorcruxGuessesLeft();

            return crow::response(200, status.dump());
        } catch(const std::exception& e) {
            return createErrorResponse(e);
        }
    });

    CROW_ROUTE(app, "/game/end")
    .methods("GET"_method)
    ([&app, &gamesDatabase, &players](const crow::request& req) {
        try {
            auto& ctx = app.get_context<crow::CookieParser>(req);
            // Read cookies with get_cookie
            auto gameID = ctx.get_cookie("gameID");
            auto playerID = ctx.get_cookie("playerID");

            removePlayer(players, playerID);

            killGame(gamesDatabase, gameID);

            return crow::response(200);
        } catch(const std::exception& e) {
            return createErrorResponse(e);
        }
    });
    
    const char* port_str = std::getenv("PORT");
    int port = port_str ? std::stoi(port_str) : 8080;
    
    app.port(port)
       .multithreaded()
       .run();
};