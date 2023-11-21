import './Data.js'
import './Piece.css'

export default function Piece({ piece }) {
  const { type, color } = piece;

  return <div className={`piece ${type} ${color}`} />;
}