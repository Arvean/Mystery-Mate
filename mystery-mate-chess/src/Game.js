import { apiBaseUrl, ErrorStatus, GameState, GameEndType, PieceType, Color, piece, GameStateLookup, GameEndTypeLookup, ColorLookup} from './Data'
import Board from './Board.js'
import Menu from './Menu.js'
import PlayerInfo from './PlayerInfo.js'
import {whitePlayer, blackPlayer} from './Player.js'
import React, { useState, useEffect, useRef } from 'react';
import { Container, Grid, Button, Loader, Modal, Header} from 'semantic-ui-react';
import './Background.css';


export default function Game() {
    const [fromSquare, setFromSquare] = useState(null);
    const [gameID, setGameID] = useState(null);
    const [board, setBoard] = useState([]);
    const [player, setPlayer] = useState(whitePlayer);
    const [gameState, setGameState] = useState(null);
    const [gameResult, setGameResult] = useState(null);
    const [highlightSquares, setHighlightSquares] = useState([]);
    const [horcruxsStatus, setHorcruxsStatus] = useState({
        WHITE: {
          hasBeenGuessed: false,
          id: null
        },
        BLACK: {
          hasBeenGuessed: false,
          id: null
        },
        playerHorcruxID: null // Set during choose horcrux inital turn
      });
    
    const [isGuessingHorcrux, setIsGuessingHorcrux] = useState(false);
    const [horcruxGuessesLeft, setHorcruxGuessLeft] = useState(0);
    const [home, setHome] = useState(false);

    const intervalRef = useRef();

    useEffect(() => {
        const fetchData = async () => {
            await fetchGameID();
            const fetchedGameState = await fetchGameState();
            console.log("GameState: " + fetchedGameState);
            if (fetchedGameState !== GameState.WAITING_FOR_OPPONENT && fetchedGameState !== GameState.ENDED) {
                setHome(false);
                await fetchBoard();
                await fetchPlayer();
                await fetchNumberOfHorcruxGuessesLeft();
                await fetchHorcruxStatus();
            }
        };
    
        intervalRef.current = setInterval(fetchData, 2000); // Poll every 2 seconds
    
        let timeoutRef;
        if (gameState === GameState.ENDED) {
            checkGameOver();
            clearInterval(intervalRef.current);
            timeoutRef = setTimeout(killGame, 10000); // Delay killGame for 10 seconds
        }
    
        return () => {
            clearInterval(intervalRef.current); // Cleanup interval
            if (timeoutRef) {
                clearTimeout(timeoutRef); // Cleanup timeout
            }
        };
    }, [gameState]);
    
    
    const transformBoard = (squaresJson) => {
        const board = Array(8).fill(null).map(() => Array(8).fill(null));
    
        squaresJson.forEach((squareJson) => {
            let { file, rank } = squareJson.position;
    
            const row = 8 - rank;
            const col = file.charCodeAt(0) - 'a'.charCodeAt(0);
    
            const piece = squareJson.piece 
                ? { id: squareJson.piece.id, type: squareJson.piece.type, color: ColorLookup[squareJson.piece.color] }
                : null;
    
            board[row][col] = { file, rank, piece };
        });
        return board;
    };
    
    const updateGame = async ()  => {
        await fetchBoard();
        const gameState = await fetchGameState();
        checkGameOver(gameState);
    }

    const fetchBoard = async () => {
        const response = await fetch(apiBaseUrl + '/board', {
            method: 'GET', 
            credentials: 'include',
        });
        if (!response.ok) console.log('Network response was not ok');
        const data = await response.json();
        const transformedBoard = transformBoard(data.squares);
        setBoard(transformedBoard);
        return transformedBoard;
    }

    const fetchPlayer = async () => {
        const response = await fetch(apiBaseUrl + '/player', {
            method: 'GET', 
            credentials: 'include',
        });
        if (!response.ok) console.log('Network response was not ok');
        const data = await response.json();
        const color = ColorLookup[data.playerColor];
        color === Color.WHITE ? 
                              setPlayer(whitePlayer) : 
                              setPlayer(blackPlayer);
    }

    const fetchHorcruxStatus = async () => {
        const response = await fetch(apiBaseUrl + '/get/horcruxStatus', {
            method: 'GET', 
            credentials: 'include',
        });
        if (!response.ok) console.log("Network response was not ok");
        const data = await response.json();
                
        setHorcruxsStatus({
            WHITE: {
                hasBeenGuessed: data.whiteHasBeenGuessed,
                id: data.whiteHasBeenGuessed ? data.whiteFoundID : null
            },
            BLACK: {
                hasBeenGuessed: data.blackHasBeenGuessed,
                id: data.blackHasBeenGuessed ? data.blackFoundID : null
            },
            playerHorcruxID: data.horcruxID
        });
    }
    
    const fetchGameState = async () => {
        const response = await fetch(apiBaseUrl + '/state', {
            method: 'GET', 
            credentials: 'include',
        });
        if (!response.ok) console.log('Network response was not ok');
        const data = await response.json();
        if (gameState !== GameStateLookup[data.status]) {
            setGameState(GameStateLookup[data.status]);
        }
        return GameStateLookup[data.status];
    }

    const fetchGameID = async () => {
        const response = await fetch(apiBaseUrl + '/gameID', {
            method: 'GET', 
            credentials: 'include',
        });
        if (!response.ok) console.log('Network response was not ok');
        const data = await response.json();
        setGameID(data.status);
    }

    const fetchPossiblePositions = async (square) => {
        const bodyText = square.file + ',' + square.rank; // Comma-separated values
        const response = await fetch(apiBaseUrl + '/positions', {
            method: 'POST',
            credentials: 'include',
            headers: {
              'Content-Type': 'text/plain',
            },
            body: bodyText,
        });
        if (!response.ok) console.log('Network response was not ok');
        const positionsData = await response.json(); // This is an object that contains the array
        
        if (positionsData && Array.isArray(positionsData.possiblePositions)) {
            // Pass the array directly to the state setter function
            setHighlightSquares(positionsData.possiblePositions);
        } else {
            // Handle the error or unexpected format
            console.error('Received data is not in the expected format:', positionsData);
        }
    }

    const guessHorcrux = async (square) => {
        const bodyText = square.file + ',' + square.rank; // Comma-separated values

        const res = await fetch(apiBaseUrl + '/guess/horcrux', {
            method: 'POST',
            credentials: 'include',
            headers: {
              'Content-Type': 'text/plain',
            },
            body: bodyText
        });

        if (!res.ok) console.log('Network response was not ok');
        const data = await res.json();
        if (data.status === ErrorStatus) console.error('Error:', data.message);
    }

    const fetchNumberOfHorcruxGuessesLeft = async () => {
        const response = await fetch(apiBaseUrl + '/numberOfHorcruxGuessesLeft', {
            method: 'GET', 
            credentials: 'include',
        });
        if (!response.ok) console.log('Network response was not ok');
        const data = await response.json();
        setHorcruxGuessLeft(data.horcruxGuessesLeft);
        return data.horcruxGuessesLeft;
    }

    const fetchGameResult = async () => {
        const response = await fetch(apiBaseUrl + '/result', {
            method: 'GET', 
            credentials: 'include',
        });
        if (!response.ok) console.log('Network response was not ok');
        const data = await response.json();
        return GameEndTypeLookup[data.status];
    }

    const sendMove = async (from, to) => {
        const fromBodyText = from.file + ',' + from.rank; // Comma-separated values
        const toBodyText = to.file + ',' + to.rank; // Comma-separated values
        const bodyText = fromBodyText + ';' + toBodyText; // Use semicolon to separate from and to
        const res = await fetch(apiBaseUrl + '/move', {
            method: 'POST',
            credentials: 'include',
            headers: {
              'Content-Type': 'text/plain',
            },
            body: bodyText,
        });

        const data = await res.json();
        if (data.status === ErrorStatus) {
            console.error('Invalid Move:', data.message);
            return false;
        }
        console.log('Move was successful:', data);
        return true;
    }

    const killGame = async () => {
        const response = await fetch(apiBaseUrl + '/end', {
            method: 'GET', 
            credentials: 'include',
        });
        if (!response.ok) console.log('Network response was not ok');
    }

    const disablePossiblePosHighlight = () => {
        setHighlightSquares(null);
    } 

    const handleSquareClick = async (square) => {
        if (square && gameState === player.moveType) {
            if (square.piece && square.piece.color === player.color) {
                setFromSquare(square);
                disablePossiblePosHighlight();
                fetchPossiblePositions(square);
            } else if (fromSquare) {
                await sendMove(fromSquare, square);
                disablePossiblePosHighlight();

                await updateGame();

                setFromSquare(null);
            }
        }
    }

    const checkGameOver = async (gameState) => {
        if (gameState == GameState.ENDED) {
            await setGameResult(fetchGameResult());
        }
    }

    const handleGuessHorcruxButton = () => {
        const guessesLeft = fetchNumberOfHorcruxGuessesLeft();
        if (guessesLeft) {
            setIsGuessingHorcrux(true);
        } else {
            console.log("No more horcrux guesses left");
        }
    }

    const handleOnHorcruxClick = async (square) => {
        await guessHorcrux(square);
        setIsGuessingHorcrux(false);
    }

    const getGameResultMessage = (result) =>  {
        switch(result) {
            case GameEndType.WHITE_WIN: return "White Wins!";
            case GameEndType.BLACK_WIN: return "Black Wins!";
            case GameEndType.STALEMATE: return "Stalemate!";
            case GameEndType.DRAW: return "Draw!";
            default: return "";
        }
    }

    const handleHorcruxChoice = async (square) => {
        const bodyText = square.file + ',' + square.rank; // Comma-separated values
        const res = await fetch(apiBaseUrl + '/select/horcrux', {
            method: 'POST',
            credentials: 'include',
            headers: {
              'Content-Type': 'text/plain',
            },
            body: bodyText,
        });

        if (!res.ok) console.log('Network response was not ok');
        const data = await res.json();
        if (data.status === ErrorStatus) console.error('Error:', data.message);

        fetchGameState();
    };

    const handleQuitGame = async () => {
        killGame(); // Check why we can't make 'await'
        clearInterval(intervalRef.current);
        setHome(true);
    }

    const opponentColor = player.color === Color.WHITE ? Color.BLACK : Color.WHITE;
    const isOpponentHorcruxGuessed = horcruxsStatus[opponentColor].hasBeenGuessed;

    return (
        home ? <Menu /> :
            <div className="harry-potter-background">
                <Container>
                    <Grid>
                        <Grid.Row centered>
                            <Grid.Column>

                                {gameState === GameState.WAITING_FOR_OPPONENT && (
                                    <>
                                        <Loader active inline='centered'>Waiting for Opponent...</Loader>
                                        <p>Game ID: {gameID}</p>
                                    </>
                                )}

                                {gameState === GameState.CHOOSING_HORCRUX && board && (
                                    <Modal 
                                    open={true} 
                                    centered={true}
                                    style={{ background: 'rgba(255, 255, 255, 0.8)', width: '60%' }}
                                    >
                                        <Header>Select Your Horcrux</Header>
                                        <Modal.Content>
                                            <p>Click on one of your pieces to set it as your horcrux!</p>
                                            <Board board={board}
                                                player={player}
                                                onSquareClick={handleHorcruxChoice}
                                                possiblePositions={highlightSquares} 
                                                horcruxsStatus={horcruxsStatus}
                                            />
                                        </Modal.Content>
                                        <Modal.Actions>
                                            <PlayerInfo guesses={horcruxGuessesLeft} color={player.color}/>
                                        </Modal.Actions>
                                    </Modal>
                                )}

                                {(gameState === GameState.WHITE_MOVE || gameState === GameState.BLACK_MOVE) && (
                                    <Modal 
                                    className="my-custom-modal"
                                    open={true} 
                                    centered={true}
                                    dimmer={true}
                                    style={{ background: 'rgba(128, 0, 128, 0.8)', width: '40%' }}
                                  >
                                    <Modal.Content>
                                        <Header as='h1' style={{ color: '#4a4a4a', fontFamily: 'YourCustomFont, sans-serif' }}>
                                            {gameState === GameState.WHITE_MOVE ? "White's" : "Black's"} Turn
                                        </Header>
                                        <Board board={board} 
                                                player={player}
                                                onSquareClick={handleSquareClick}
                                                possiblePositions={highlightSquares}
                                                horcruxsStatus={horcruxsStatus}
                                                isGuessingHorcrux={isGuessingHorcrux}
                                                onHorcruxClick={handleOnHorcruxClick}
                                        />
                                        <Grid>
                                        <div style={{ marginTop: '10px' }}> {/* Add spacing between the buttons */}
                                        </div>
                                            <Grid.Row>
                                                <Grid.Column width={5} verticalAlign="middle">
                                                    <PlayerInfo name="Voldemort" guesses={horcruxGuessesLeft} />
                                                </Grid.Column>
                                                <Grid.Column width={6} textAlign="center">
                                                    {!isOpponentHorcruxGuessed && (
                                                    <Button color='blue' onClick={() => handleGuessHorcruxButton()}>Guess Horcrux</Button>
                                                    )}
                                                </Grid.Column>
                                                <Grid.Column width={5} textAlign="right">
                                                    {!isOpponentHorcruxGuessed && (
                                                    <Button color='red' onClick={() => handleQuitGame()}>Quit Game</Button>
                                                    )}
                                                </Grid.Column>
                                            </Grid.Row>
                                        </Grid>
                                        </Modal.Content>
                                    </Modal>
                                )}

                                {gameState === GameState.ENDED && (
                                    <div> {/* Enclosing the elements in a div */}
                                    <Header as='h1'>Game Ended - {getGameResultMessage(gameResult)}</Header>
                                    <Button color='red' onClick={() => handleQuitGame()}>Quit Game</Button>
                                    </div>
                                )}
                            </Grid.Column>
                        </Grid.Row>
                    </Grid>
                </Container>
            </div>
    );
};