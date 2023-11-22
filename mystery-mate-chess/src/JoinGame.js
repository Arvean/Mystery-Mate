import './JoinGame.css'
import { apiBaseUrl, ErrorStatus, GameState, GameEndType, PieceType, Color, piece } from './Data'
import { Button, Form, InputGroup } from 'react-bootstrap'; // Import React Bootstrap components
import Game from './Game.js'
import React, { useState, useEffect } from 'react';


export default function JoinGame() {
    const [joinedGame, setJoinedGame] = useState(false);
    const [gameIdToJoin, setGameIdToJoin] = useState("");
    
    const joinGame = async () => {
        try {
            const response = await fetch(apiBaseUrl + '/join', {
                method: 'POST',
                credentials: 'include', // to include cookies for the originating domain
                headers: {
                  'Content-Type': 'text/plain',
                },
                body: JSON.stringify({
                    gameID: gameIdToJoin
                }),
            });

            if (!response.ok) throw new Error('Network response was not ok');
            const data = await response.json();
            console.log('Joined game:', data);

            if (data.status == ErrorStatus) throw new Error(data.message);
            if (data.status != GameState.WHITE_MOVE) throw new Error('Game is not waiting for opponent');

            joinedGame = true;
        } catch (error) {
            console.error('There was a problem with the fetch operation:', error);
        }
    }

    const handleJoinButtonClick = () => {
        setJoinedGame(true);
        joinGame();
    }
    
    return (
        joinedGame ? <Game /> : (
            <div>
                <InputGroup className="mb-3">
                    <Form.Control
                        placeholder="Game ID"
                        value={gameIdToJoin}
                        onChange={(e) => setGameIdToJoin(e.target.value)}
                    />
                </InputGroup>
                <Button onClick={handleJoinButtonClick} variant="primary">Join</Button>
            </div>
        )
    );
  }