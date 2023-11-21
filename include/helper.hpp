#include "game.h"
#include "crow.h"
#include <sstream>
#include <uuid/uuid.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

typedef std::unordered_map<std::string, std::shared_ptr<Player>> Players;
typedef std::unordered_map<std::string, std::unique_ptr<Game>> GamesDatabase;

/* Helper Function Declarations */
int GameStateToInt(GameState state);
int GameEndToInt(GameEndType state);
std::string uuidToString(const uuid_t& uuid);
void generateUUID(uuid_t& uuid);
void StrToUUID(std::string str, uuid_t gameID);
std::string UUIDToStr(uuid_t& uuid);
std::string getGameID(std::unordered_map<std::string, std::string>& cookies);
std::string getPlayerID(std::unordered_map<std::string, std::string>& cookies);
std::string generatePlayerToken();
std::unordered_map<std::string, std::string> parseCookies(const std::string& cookie_header);
void createPlayer(Players& players, const std::string& cookieStr, Color color, std::shared_ptr<Player>& pPlayer);
std::string generateGameID();
void createGame(GamesDatabase& gamesDatabase, const std::string& gameIDStr);
void setCookies(crow::response& response, const std::string& gameIDStr, const std::string& playerStr);
std::string getGameID(const std::unordered_map<std::string, std::string>& cookies);
void validateGame(const GamesDatabase& gamesDatabase, const std::string& gameID);
std::unique_ptr<Game> initializeGame(Player* whitePlayer, Player* blackPlayer, Board* board, BoardRules* boardRules);
crow::response startNewGame(
    GamesDatabase& gamesDatabase,
    Players& players,
    std::shared_ptr<Player>& pWhitePlayer,
    crow::response& response);
crow::response joinGame(
    GamesDatabase& gamesDatabase,
    Players& players, 
    std::unique_ptr<Game>& pGame,
    std::unique_ptr<Board>& pBoard,
    std::unique_ptr<BoardRules>& pBoardRules,
    std::shared_ptr<Player>& pWhitePlayer,
    std::shared_ptr<Player>& pBlackPlayer,
    const crow::request& req);
void killGame(
    GamesDatabase& gamesDatabase, 
    const std::string& gameID
);
void removePlayer(Players& players, std::string& playerCookie);
crow::response createJoinResponse(const std::string& gameID, const std::string& player, GameState gameState);
crow::response createErrorResponse(const std::exception& e);
void validateJsonFields(const json& j, const std::initializer_list<std::string>& fields);
Player* findPlayer(const Players& players, const std::string& playerID);
Game* findGame(const GamesDatabase& gamesDatabase, const std::string& gameID);
std::pair<char, int> parseFileAndRank(const std::string& input);
void addCorsHeader(crow::response& res);

constexpr int ERROR_STATUS = -1;