#include "game.h"
#include "helper.hpp"
#include "crow.h"
#include "crow/middlewares/cors.h"
#include "crow/middlewares/cookie_parser.h"
#include <sstream>
#include <uuid/uuid.h>
#include <nlohmann/json.hpp>

#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

using json = nlohmann::json;

int main(int argc, char* argv[]) {

    sql::mysql::MySQL_Driver* driver;
    std::unique_ptr<sql::Connection> con;

    try {
        driver = sql::mysql::get_mysql_driver_instance();
        con.reset(driver->connect("tcp://127.0.0.1:3306", "root", "my_secret_pw"));
        con->setSchema("mystery_mate_database");
    } catch (sql::SQLException& e) {
        std::cerr << "Error connecting to MySQL: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    // Enable CORS
    crow::App<crow::CORSHandler, crow::CookieParser> app;

    // Customize CORS
    auto& cors = app.get_middleware<crow::CORSHandler>();

    cors.global() // Setting CORS policies for all routes
        .methods("GET"_method, "POST"_method, "PUT"_method, "DELETE"_method, "OPTIONS"_method)
        .headers("Content-Type","Upgrade-Insecure-Requests")
        .origin("http://localhost:3000")
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
    ([&app, &con] (const crow::request& req) {
        crow::response res;
        json status;

        try {
            std::string playerToken = generatePlayerToken();
            std::cout << "Player ID Generated: " << playerToken << std::endl;
            createPlayer(con.get(), playerToken, Color::WHITE);

            std::string gameID = generateGameID();
            std::cout << "Game ID Generated: " << gameID << std::endl;
            createGame(con.get(), gameID);

            status["status"] = GameStateToInt(GameState::WAITING_FOR_OPPONENT);

            res.code = 200;
            res.body = status.dump();

            auto& ctx = app.get_context<crow::CookieParser>(req);
            ctx.set_cookie("gameID", gameID).path("/").httponly();
            ctx.set_cookie("playerID", playerToken).path("/").httponly();
        }
        catch (const json::exception& e) {
            res.code = 400;
            res.body = "Invalid JSON format: " + std::string(e.what());
        }
        catch (const sql::SQLException& e) {
            res.code = 500;
            res.body = "Database error: " + std::string(e.what());
        }
        catch (const std::exception& e) {
            res.code = 500;
            res.body = "Internal server error: " + std::string(e.what());
        }

        res.set_header("Content-type", "application/json");
        return res;
    });

    CROW_ROUTE(app, "/game/join")
    .methods("POST"_method)
    ([&app, &con](const crow::request& req) {
        json status;
        try {
            auto jsonBody = json::parse(req.body);
            if (!validateJsonFields(jsonBody, {"gameID"})) {
                throw std::runtime_error("Missing required fields");
            }

            auto gameID = jsonBody["gameID"].get<std::string>();
            if (!validateGame(con.get(), gameID)) {
                throw std::runtime_error("Invalid or non-existent game ID");
            }

            const auto playerToken = generatePlayerToken();
            auto pBlackPlayer = createPlayer(con.get(), playerToken, Color::BLACK);

            // Load white player and other game state data from the database
            auto pWhitePlayer = loadPlayerForGame(con.get(), gameID);  // This method needs to be defined
            auto pGame = initializeGame(pWhitePlayer, pBlackPlayer);

            updateGameState(con.get(), gameID, pGame->getGameState());

            status["status"] = GameStateToInt(pGame->getGameState());

            crow::response response(200, status.dump());
            auto& ctx = app.get_context<crow::CookieParser>(req);
            ctx.set_cookie("gameID", gameID).path("/").httponly();
            ctx.set_cookie("playerID", playerToken).path("/").httponly();

            response.set_header("Content-type", "application/json");
            return response;
        } catch(const json::exception& e) {
            crow::response response(400, "Invalid JSON format: " + std::string(e.what()));
            response.set_header("Content-type", "application/json");
            return response;
        } catch(const sql::SQLException& e) {
            crow::response response(500, "Database error: " + std::string(e.what()));
            response.set_header("Content-type", "application/json");
            return response;
        } catch(const std::exception& e) {
            return createErrorResponse(e);
        }
    });

    CROW_ROUTE(app, "/game/select/horcrux")
    .methods("POST"_method)
    ([&app, &con](const crow::request& req) {
        json status;

        try {
            auto& ctx = app.get_context<crow::CookieParser>(req);
            // Read cookies with get_cookie
            auto gameID = ctx.get_cookie("gameID");
            auto playerID = ctx.get_cookie("playerID");

            std::cout << "Received request body: " << req.body << std::endl;
            
            Player* pPlayer = findPlayer(con.get(), playerID);
            std::cout << "Player Color: " << int(pPlayer->getColor()) << std::endl;

            Game* pGame = findGame(con.get(), gameID);
            
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
    ([&app, &con](const crow::request& req) {
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

            Game* pGame = findGame(con.get(), gameID); // Using the helper function to find a game
            Player* pPlayer = findPlayer(con.get(), playerID); // Use a function to find a player

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

    CROW_ROUTE(app, "/game/select/horcrux")
    .methods("POST"_method)
    ([&app, &con](const crow::request& req) {
        json status;

        try {
            auto& ctx = app.get_context<crow::CookieParser>(req);
            auto gameID = ctx.get_cookie("gameID");
            auto playerID = ctx.get_cookie("playerID");

            auto pPlayer = findPlayer(con.get(), playerID);
            auto pGame = findGame(con.get(), gameID);

            auto [file, rank] = parseFileAndRank(req.body);
            const IPiece* horcrux = pGame->getPieceFromPosition(Position(file, rank));
            if (horcrux) {
                int horcruxID = horcrux->getID();
                status["horcruxID"] = horcruxID;
                updatePlayerHorcrux(con.get(), playerID, horcruxID);
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

    CROW_ROUTE(app, "/game/move")
    .methods("POST"_method)
    ([&app, &con](const crow::request& req) {
        json status; 

        try {
            auto& ctx = app.get_context<crow::CookieParser>(req);
            auto gameID = ctx.get_cookie("gameID");
            auto playerID = ctx.get_cookie("playerID");

            std::string requestBody = req.body;
            size_t delimiterPos = requestBody.find(';');
            if (delimiterPos == std::string::npos) {
                throw std::runtime_error("Invalid move format.");
            }

            std::string fromText = requestBody.substr(0, delimiterPos);
            std::string toText = requestBody.substr(delimiterPos + 1);

            auto [fromFile, fromRank] = parseFileAndRank(fromText);
            auto [toFile, toRank] = parseFileAndRank(toText);

            Position from(fromFile, fromRank);
            Position to(toFile, toRank);

            auto pGame = findGame(con.get(), gameID);
            auto pPlayer = findPlayer(con.get(), playerID);

            pGame->movePiece(Move(from, to, pPlayer->getId())); // Move method needs player ID and positions
            
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
    ([&app, &con](const crow::request& req) {
        json status;

        try {
            auto& ctx = app.get_context<crow::CookieParser>(req);
            auto gameID = ctx.get_cookie("gameID");

            auto pGame = findGame(con.get(), gameID);

            if (!pGame) {
                // If the game is not found, we assume it's waiting for an opponent to join
                status["status"] = GameStateToInt(GameState::WAITING_FOR_OPPONENT);
            } else {
                // If the game is found, report its current state
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
    ([&app](const crow::request& req) {
        json status;
        try {
            auto& ctx = app.get_context<crow::CookieParser>(req);
            auto gameID = ctx.get_cookie("gameID");

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
    ([&app, &con](const crow::request& req) {
        try {
            auto& ctx = app.get_context<crow::CookieParser>(req);
            auto gameID = ctx.get_cookie("gameID");
            auto playerID = ctx.get_cookie("playerID");

            auto pGame = findGame(con.get(), gameID);
            auto pPlayer = findPlayer(con.get(), playerID);

            json status;
            json squaresJson;

            bool isBlack = (pPlayer && pPlayer->getColor() == Color::BLACK);

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
    ([&app, &con](const crow::request& req) {
        json status;
        try {
            auto& ctx = app.get_context<crow::CookieParser>(req);
            auto gameID = ctx.get_cookie("gameID");
            auto playerID = ctx.get_cookie("playerID");

            auto pPlayer = findPlayer(con.get(), playerID);
            if (!pPlayer) 
                return crow::response(400, "Player not found.");

            auto pGame = findGame(con.get(), gameID);
            if (!pGame)
                return crow::response(400, "Game not found.");

            auto [file, rank] = parseFileAndRank(req.body);
            Position from(file, rank);
            const IPiece* piece = pGame->getPieceFromPosition(from);

            if (!piece || piece->getColor() != pPlayer->getColor()) {
                return crow::response(400, "Invalid piece or player color does not match piece color.");
            }

            json jsonObjs;
            for (const auto& pos : pGame->getAvailablePositions(piece, from)) {
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
            crow::response response(400, "Invalid JSON format: " + std::string(e.what()));
            response.set_header("Content-type", "application/json");
            return response;
        } catch(const std::exception& e) {
            return createErrorResponse(e);
        }
    });

    CROW_ROUTE(app, "/game/result")
    .methods("GET"_method)
    ([&app, &con](const crow::request& req) {
        json status;

        try {
            auto& ctx = app.get_context<crow::CookieParser>(req);
            auto gameID = ctx.get_cookie("gameID");

            auto pGame = findGame(con.get(), gameID);
            if (!pGame) 
                return crow::response(404, "Game not found");

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
    ([&app, &con](const crow::request& req) {
        json status;
        try {
            auto& ctx = app.get_context<crow::CookieParser>(req);
            auto gameID = ctx.get_cookie("gameID");

            auto pGame = findGame(con.get(), gameID);
            status["isInProgress"] = !pGame ? false : true;

            return crow::response(200, status.dump());

        } catch(const std::exception& e) {
            status["isInProgress"] = false;
            return crow::response(200, status.dump());
        }
    });

    CROW_ROUTE(app, "/game/numberOfHorcruxGuessesLeft")
    .methods("GET"_method)
    ([&app, &con](const crow::request& req) {
        json status;
        try {
            auto& ctx = app.get_context<crow::CookieParser>(req);
            auto playerID = ctx.get_cookie("playerID");

            auto pPlayer = findPlayer(con.get(), playerID);
            if (!pPlayer)
                return crow::response(404, "Player not found");

            status["horcruxGuessesLeft"] = pPlayer->getNumberOfHorcruxGuessesLeft();

            return crow::response(200, status.dump());
        } catch(const std::exception& e) {
            return createErrorResponse(e);
        }
    });

    CROW_ROUTE(app, "/game/end")
    .methods("GET"_method)
    ([&app, &con](const crow::request& req) {
        try {
            auto& ctx = app.get_context<crow::CookieParser>(req);
            auto gameID = ctx.get_cookie("gameID");
            auto playerID = ctx.get_cookie("playerID");

            removePlayer(con.get(), playerID);
            killGame(con.get(), gameID);

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