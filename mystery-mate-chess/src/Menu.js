import './Menu.css'
import { apiBaseUrl, ErrorStatus, GameState, GameEndType, PieceType, Color, piece, GameStateLookup } from './Data'
import JoinGame from './JoinGame.js'
import Game from './Game.js'
import React, { useState, useEffect } from 'react';

export default function Menu() {
    const [isGameInProgress, setIsGameInProgress] = useState(false);
    const [isJoining, setIsJoining] = useState(false);

    // Prevents game from resetting if the browser refreshes
    useEffect(() => {
        const fetchIsGameInProgreass = async () => {
            const response = await fetch(apiBaseUrl + '/isGameInProgress', {
                method: 'GET', 
                credentials: 'include',
            });
            if (!response.ok) throw new Error('Network response was not ok');
            const data = await response.json();
            setIsGameInProgress(data.isInProgress);
        }
        fetchIsGameInProgreass();
    }, [isGameInProgress]);

    const startNewGame = async () => {
        try {
            const res = await fetch(apiBaseUrl + '/startNew', {
                method: 'GET',
                credentials: 'include', // to include cookies for the originating domain
            });

            if (!res.ok) throw new Error('Network response was not ok');
            const data = await res.json();
            if (data.status == ErrorStatus) throw new Error(data.message);

            const respGameState = GameStateLookup[data.status];

            if (respGameState != GameState.WAITING_FOR_OPPONENT) throw new Error('Game is not waiting for opponent');
            console.log('Joined game:', data);

            setIsGameInProgress(true);
        
        } catch (error) {
            console.error('There was a problem with the fetch operation:', error);
        }
    }

    return (
        <>
            {!isGameInProgress && !isJoining && (
                <div className="menu">
                    <button onClick={() => { setIsJoining(false); startNewGame(); }}>Start New Game</button>
                    <button onClick={() => { setIsJoining(true); }}>Join Game</button>
                </div>
            )}
            {isJoining && <JoinGame/>}
            {isGameInProgress && <Game/>}
        </>
    );
}
