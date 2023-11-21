import { apiBaseUrl, ErrorStatus, GameState, GameEndType, PieceType, Color, piece, GameStateLookup, GameEndTypeLookup, ColorLookup} from './Data'
import Board from './Board.js'
import Menu from './Menu.js'
import PlayerInfo from './PlayerInfo.js'
import {whitePlayer, blackPlayer} from './Player.js'
import React, { useState, useEffect } from 'react';
import { Button, Card, Typography } from '@mui/material';
import styles from './Game.module.css';

export default function Game() {
    const [fromSquare, setFromSquare] = useState(null);
    const [toSquare, setToSquare] = useState(null);
    const [gameID, setGameID] = useState(null);
    const [board, setBoard] = useState(null);
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

    const [guessLeft, setGuessLeft] = useState(0);
    const [guessHorcruxState, setGuessHorcruxState] = useState(false);
    const [home, setHome] = useState(false);

    useEffect(() => {
        const fetchData = async () => {
            await fetchGameID();
            const fetchedGameState = await fetchGameState();
            console.log("GameState" + fetchedGameState);
            if (fetchedGameState !== GameState.WAITING_FOR_OPPONENT) {
                setHome(false);
                await fetchPlayer();
                await fetchBoard();
                await fetchHorcruxStatus();
            }
        };
        
        const intervalId = setInterval(fetchData, 2000); // Poll every 2 seconds
    
        return () => clearInterval(intervalId); // Cleanup on component unmount
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
        if (!response.ok) throw new Error('Network response was not ok');
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
        if (!response.ok) throw new Error('Network response was not ok');
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
        if (!response.ok) throw new Error("Network response was not ok");
        const data = await response.json();
        
        let updatedStatus = { ...horcruxsStatus, playerHorcruxID: data.horcruxID };
        
        if (data.hasBeenGuessed) {
            updatedStatus[player.color] = {
                hasBeenGuessed: true,
                id: data.foundID
            };
        }

        updatedStatus[player.color] = {
            id: data.horcruxID
        }

        setHorcruxsStatus(updatedStatus);
    }
    
    const fetchGameState = async () => {
        const response = await fetch(apiBaseUrl + '/state', {
            method: 'GET', 
            credentials: 'include',
        });
        if (!response.ok) throw new Error('Network response was not ok');
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
        if (!response.ok) throw new Error('Network response was not ok');
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
        if (!response.ok) throw new Error('Network response was not ok');
        const positionsData = await response.json(); // This is an object that contains the array
        
        if (positionsData && Array.isArray(positionsData.possiblePositions)) {
            // Pass the array directly to the state setter function
            setHighlightSquares(positionsData.possiblePositions);
        } else {
            // Handle the error or unexpected format
            console.error('Received data is not in the expected format:', positionsData);
        }
    }

    const guessHorcrux = async (pieceID) => {
        const res = await fetch(apiBaseUrl + '/guess/horcrux', {
            method: 'POST',
            credentials: 'include',
            headers: {
              'Content-Type': 'text/plain',
            },
            body: pieceID
        });

        if (!res.ok) throw new Error('Network response was not ok');
        const data = await res.json();
        if (data.status === ErrorStatus) console.error('Error:', data.message);


        if (data.guess) {
            setHorcruxsStatus({
                ...horcruxsStatus,
                [player.color]: {
                  hasBeenGuessed: true,
                  id: data.id
                }
            });
        }
    }

    const returnGameResult = async () => {
        const response = await fetch(apiBaseUrl + '/result', {
            method: 'GET', 
            credentials: 'include',
        });
        if (!response.ok) throw new Error('Network response was not ok');
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

    const cleanUpGame = async () => {
        const response = await fetch(apiBaseUrl + '/end', {
            method: 'GET', 
            credentials: 'include',
        });
        if (!response.ok) throw new Error('Network response was not ok');
    }

    const endGame = async () => {
        await setGameResult(returnGameResult());
        await cleanUpGame();
    }

    const disablePossiblePosHighlight = () => {
        setHighlightSquares(null);
    } 

    const handleSquareClick = async (square) => {
        if (square && gameState === player.moveType) {
            if (guessHorcruxState) {
                guessHorcrux(square.piece.id);
                fetchHorcruxStatus();
                setGuessHorcruxState(false);
            }
            if (square.piece && square.piece.color === player.color) {
                setFromSquare(square);
                disablePossiblePosHighlight();
                fetchPossiblePositions(square);
            } else if (fromSquare) {
                await sendMove(fromSquare, square);
                disablePossiblePosHighlight();
                setToSquare(square);

                await updateGame();

                setFromSquare(null);
            }
        }
    }

    const checkGameOver = async (gameState) => {
        if (gameState == GameState.ENDED) {endGame();}
    }

    const handleGuesshorcruxClick = async () => {
        if (gameState === player.moveType && guessLeft > 0) {
            setGuessHorcruxState(true);
            setGuessLeft(prevGuess => prevGuess - 1);
        }
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

        if (!res.ok) throw new Error('Network response was not ok');
        const data = await res.json();
        if (data.status === ErrorStatus) console.error('Error:', data.message);

        fetchGameState();
    };

    return (
        home ? <Menu/> :
            <div className="game">
            {gameState === GameState.WAITING_FOR_OPPONENT && (
                <>
                    <h1>Waiting for Opponent...</h1>
                    <p>Game ID: {gameID}</p>
                </>
            )}
            {gameState === GameState.CHOOSING_HORCRUX && board && (
                <>
                <h1>Select Your Horcrux</h1>
                <p>Click on one of your pieces to set it as your horcrux!</p>
                <Board board={board}
                       player={player}
                       onSquareClick={handleHorcruxChoice}
                       possiblePositions={highlightSquares} 
                       horcruxsStatus={horcruxsStatus}
                />
                <PlayerInfo guesses={guessLeft} color={player.color}/>
                </>
            )}
            
            {(gameState === GameState.WHITE_MOVE || gameState === GameState.BLACK_MOVE) && (
                <>
                <h1>{gameState === GameState.WHITE_MOVE ? "White's" : "Black's"} Turn</h1>
                <h1>Number of horcrux Guesses Left: {guessLeft}</h1>
                <button onClick={() => handleGuesshorcruxClick()}>Guess horcrux</button>
                <Board board={board} 
                        player={player}
                        onSquareClick={handleSquareClick}
                        possiblePositions={highlightSquares}
                        horcruxsStatus={horcruxsStatus}
                />
                <PlayerInfo name="Voldemort" guesses={guessLeft} />
                </>
            )}
            
            {gameState === GameState.ENDED && (
                <>
                <h1>Game Ended - {getGameResultMessage(gameResult)}</h1>
                <button onClick={() => setHome(true)}>Home </button> 
                </>
            )}
            </div>
        );
};