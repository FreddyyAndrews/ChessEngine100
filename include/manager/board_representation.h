#ifndef BOARD_REPRESENTATION_H
#define BOARD_REPRESENTATION_H

#include <string>
#include <vector>

class BoardRepresentation
{
public:
    // Constructors
    BoardRepresentation();                                // Initialize an empty or standard chess board
    explicit BoardRepresentation(const std::string &fen); // Initialize from a FEN string

    // Methods to handle FEN strings
    void input_fen_position(const std::string &fen); // Load a position from a FEN string
    std::string output_fen_position() const;         // Output the current position as a FEN string

    // Methods to handle moves
    std::vector<std::string> list_next_legal_moves() const; // Generate all legal moves from the current position
    bool make_move(const std::string &move);                // Apply a move (in UCI notation)

    // Methods for specific game states
    bool is_checkmate() const;                                // Check if the current player is in checkmate
    bool is_stalemate() const;                                // Check if the current player is in stalemate
    bool is_insufficient_material() const;                    // Check if the game is a draw due to insufficient material
    bool is_draw_by_repetition() const;                       // Check if the game is a draw by repetition
    bool is_draw_by_fifty_moves() const;                      // Check if the game is a draw by the fifty-move rule

    // Utility methods
    void reset();
    // Reset the board to the initial position
    void print_board() const; // Print the board for debugging purposes

private:
    // Bitboards for piece types
    uint64_t white_pawns, white_knights, white_bishops, white_rooks, white_queens, white_king;
    uint64_t black_pawns, black_knights, black_bishops, black_rooks, black_queens, black_king;

    // Castling rights and turn information
    bool white_can_castle_kingside, white_can_castle_queenside;
    bool black_can_castle_kingside, black_can_castle_queenside;
    bool white_to_move; // True if it's white's turn, false for black

    // En passant square
    int en_passant_square; // -1 if no en passant is available

    int halfmove_clock;  // Fifty-move rule counter
    int fullmove_number; // Number of full moves

    // Move history for undoing moves
    struct Move
    {
        std::string uci_move;
        uint64_t white_pieces_before, black_pieces_before;
        bool white_to_move_before;
        // Other details can be added here for more advanced move undoing (castling rights, en passant, etc.)
    };
    std::vector<Move> move_history;

    // Helper methods for legal move generation and game status checks
    void update_attacking_defending_squares();                                           // Update bitboards of attacking/defending squares
    bool is_legal_move(const std::string &move) const;                                   // Check if a move is legal
    void apply_move(const std::string &move);                                            // Apply the move to the bitboards
    int square_to_bit_index(int rank, int file);                                         // Convert square coordinates to bit index
    std::string bitboard_to_fen(uint64_t bitboard, int rank, int file);                  // Convert a bitboard to FEN string
    wchar_t get_piece_at_square(int square) const;                                       // Get the piece at a square
    bool remove_piece_from_square(int square, char &moving_piece, bool &is_white_piece); // Remove a piece from a square
    void handle_captures_and_castling_rights(int to_square, bool is_white_piece);
    bool remove_white_piece_from_square(int square);
    bool remove_black_piece_from_square(int square);
    void handle_pawn_promotion(uint64_t to_bit, char promotion_piece, bool is_white_piece);
    void handle_regular_move(int from_square, int to_square, char moving_piece, bool is_white_piece,
                             bool &is_castling_move, bool &is_double_pawn_push);
    void update_en_passant_square(char moving_piece, int from_square, int to_square, bool is_double_pawn_push);
    int algebraic_to_square(const std::string &square_str) const;
    std::string square_to_algebraic(int square) const;
    void remove_opponent_pawn_from_square(int square, bool is_white_piece);
};

#endif // BOARD_REPRESENTATION_H
