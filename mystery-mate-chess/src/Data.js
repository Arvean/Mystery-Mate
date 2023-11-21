//export const apiBaseUrl = 'https://mystery-mate-09f0bde019c8.herokuapp.com/game';
export const apiBaseUrl = "http://localhost:8080/game";
export const ErrorStatus = -1;
export const GameStateLookup = {
    0: 'WAITING_FOR_OPPONENT',
    1: 'CHOOSING_HORCRUX',
    2: 'WHITE_MOVE',
    3: 'BLACK_MOVE',
    4: 'ENDED'
};

export const GameState = {
    WAITING_FOR_OPPONENT: 'WAITING_FOR_OPPONENT',
    CHOOSING_HORCRUX: 'CHOOSING_HORCRUX',
    WHITE_MOVE: 'WHITE_MOVE',
    BLACK_MOVE: 'BLACK_MOVE',
    ENDED: 'ENDED'
};

export const GameEndTypeLookup = {
    0: 'WHITE_WIN',
    1: 'BLACK_WIN',
    2: 'DRAW',
    3: 'STALEMATE'
}

export const GameEndType = {
    WHITE_WIN: 'WHITE_WIN',
    BLACK_WIN: 'BLACK_WIN',
    DRAW: 'DRAW',
    STALEMATE: 'STALEMATE',
};

export const ColorLookup = {
    0: 'WHITE',
    1: 'BLACK'
};

export const Color = {
    WHITE: 'WHITE',
    BLACK: 'BLACK'
};

export const PieceTypeLookup = {
    0: 'MOCK',
    1: 'PAWN',
    2: 'BISHOP',
    3: 'KNIGHT',
    4: 'ROOK',
    5: 'QUEEN',
    6: 'KING'
}

export const PieceType = {
    MOCK: 0,
    PAWN: 1,
    BISHOP: 2,
    KNIGHT: 3,
    ROOK: 4,
    QUEEN: 5,
    KING: 6
};

export const PieceSymbolLookup = {
    1: 'p',
    2: 'b',
    3: 'n',
    4: 'r',
    5: 'q',
    6: 'k'
};
  
export const piece = 
{
    whitePawn: {
      type: PieceType.PAWN,
      color: ColorLookup[Color.WHITE],
      id: null
    },
    whiteRook: {
        type: PieceType.ROOK,
        color: ColorLookup[Color.WHITE],
        id: null
    },
    whiteKnight: {
        type: PieceType.KNIGHT,
        color: ColorLookup[Color.WHITE],
        id: null
    },
    whiteBishop: {
        type: PieceType.BISHOP,
        color: ColorLookup[Color.WHITE],
        id: null
    },
    whiteQueen: {
        type: PieceType.QUEEN,
        color: ColorLookup[Color.WHITE],
        id: null
    },
    whiteKing: {
        type: PieceType.KING,
        color: ColorLookup[Color.WHITE],
        id: null
    },

    blackPawn: {
        type: PieceType.PAWN,
        color: ColorLookup[Color.BLACK],
        id: null
    },
    blackRook: {
        type: PieceType.ROOK,
        color: ColorLookup[Color.BLACK],
        id: null
    },
    blackKnight: {
        type: PieceType.KNIGHT,
        color: ColorLookup[Color.BLACK],
        id: null
    },
    blackBishop: {
        type: PieceType.BISHOP,
        color: ColorLookup[Color.BLACK],
        id: null
    },
    blackQueen: {
        type: PieceType.QUEEN,
        color: ColorLookup[Color.BLACK],
        id: null
    },
    blackKing: {
        type: PieceType.KING,
        color: ColorLookup[Color.BLACK],
        id: null
    }
};
  