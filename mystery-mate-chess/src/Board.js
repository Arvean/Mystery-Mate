import Square from './Square.js'
import './Board.css'
import Chessboard from 'chessboardjsx';
import {PieceSymbolLookup, ColorLookup, Color} from './Data.js'
import React, { useEffect, useState } from 'react';

const Board = ({ board, player, onSquareClick, possiblePositions, horcruxsStatus, isGuessingHorcrux, onHorcruxClick }) => {
  const [highlightStyles, setHighlightStyles] = useState({});
  // Function to convert your board state to a position string for chessboardjsx
  const getPositionFromBoard = (board) => {
    let position = {};
  
    // Loop over each row and column, directly using the file and rank information
    for (const row of board) {
      for (const square of row) {
        if (square.piece) {
          // square.file and square.rank are directly used here
          const squareName = `${square.file}${square.rank}`;
          const pieceCode = `${square.piece.color.charAt(0).toLowerCase()}${PieceSymbolLookup[square.piece.type].toUpperCase()}`;
          position[squareName] = pieceCode;
        }
      }
    }
    
    return position;
  };
  

  useEffect(() => {
    const newHighlightStyles = {};
  
    board.forEach(row => {
      row.forEach(square => {
        const squareName = `${square.file}${square.rank}`;
        const isPossiblePosition = (possiblePositions || []).some(pos => pos.file === square.file && pos.rank === square.rank);
        const isWhiteHorcrux = square.piece && square.piece.id === horcruxsStatus.WHITE.id;
        const isBlackHorcrux = square.piece && square.piece.id === horcruxsStatus.BLACK.id;
        const isPlayerHocrux = square.piece && square.piece.id === horcruxsStatus.playerHorcruxID
        const isPlayerPiece = square.piece && (!horcruxsStatus.playerHorcruxID && square.piece.color === player.color);
        const isHorcruxGuessingOpponentPiece = isGuessingHorcrux && square.piece && (square.piece.color !== player.color);

        const opponentColor = player.color === Color.WHITE ? Color.BLACK : Color.WHITE;
        const isOpponentHorcruxGuessed = horcruxsStatus[opponentColor].hasBeenGuessed;
        const isSquarePieceOpponentHorcrux = square.piece && isOpponentHorcruxGuessed && square.piece.id === horcruxsStatus[opponentColor].id;
        
        if (isPossiblePosition || isWhiteHorcrux || isBlackHorcrux || isPlayerHocrux || isPlayerPiece || isHorcruxGuessingOpponentPiece || isSquarePieceOpponentHorcrux) {
          newHighlightStyles[squareName] = {
            backgroundColor: 'rgba(255, 235, 59, 0.8)',
            boxShadow: 'inset 0 0 0 2px gold',
          };
        }
      });
    });
  
    setHighlightStyles(newHighlightStyles);
  }, [board, player, possiblePositions, horcruxsStatus]);
  

  // chessboardjsx's onSquareClick prop uses square's name (like "e4")
  // So we need to translate that back to your game's logic (file/rank or x/y coordinates)
  const handleSquareClick = (squareName) => {
    // Extract file and rank from the squareName (e.g., "e4" -> file "e", rank 4)
    const file = squareName.charAt(0);
    const rank = parseInt(squareName.charAt(1), 10);
  
    // Find the piece on the clicked square by matching the file and rank in your board state
    const squareFromBoard = board
      .flat() // Flatten the 2D board array to make it easier to search
      .find(s => s.file === file && s.rank === rank);
  
    // If a piece is present on the square, we pass it along with the file and rank
    // If there's no piece on the square, the piece attribute will be undefined
    if (isGuessingHorcrux) {
      onHorcruxClick({ file, rank, piece: squareFromBoard?.piece });
    } else {
      onSquareClick({ file, rank, piece: squareFromBoard?.piece });
    }
  };
  

  return (
    <div className="board-wrapper"> {/* wrapper class, add your styles as needed */}
      <Chessboard
        position={getPositionFromBoard(board)}
        onSquareClick={handleSquareClick}
        squareStyles={highlightStyles}
        orientation={player.color.toLowerCase()}
        // pass other props as needed, e.g., boardStyle for size, orientation, etc.
      />
    </div>
  );
};

export default Board;
