import React from 'react';
import {Color} from './Data.js';

const PlayerInfo = ({ guesses, color }) => {
    const imageUrl = color === Color.WHITE ? './voldemort.jpeg' : './voldemort_snake.jpg';
    const name = color === Color.WHITE ? 'Mordemort' : 'Lordervort';
    //<img src={imageUrl} alt="Voldemort" width="10" />
  
    return (
      <div className="player-info">
        <h2>{name}</h2>
        <p>Number of Guesses: {guesses}</p>
      </div>
    );
  };
  

export default PlayerInfo;
