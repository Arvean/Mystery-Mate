import Piece from './Piece.js'
import './Board.css'
import React, { useState, useEffect } from 'react';


export default function Square({ highlight, square, onSquareClick }) {
  return (
    <div 
        className={`square ${highlight ? 'highlight' : ''}`} 
        onClick={() => onSquareClick(square)}
        role="button"
        tabIndex={0}
    >
        {square.piece && <Piece piece={square.piece} />}
    </div>

  );
}