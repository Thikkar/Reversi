#include "model.hxx"

using namespace ge211;

Model::Model(int size)
        : Model(size, size)
{ }

Model::Model(int width, int height)
        : board_({width, height})
{
    // TODO: initialize `next_moves_` to `turn_`'s available moves
    compute_next_moves_();
}

Model::Rectangle
Model::board() const
{
    return board_.all_positions();
}

Player
Model::operator[](Position pos) const
{
    return board_[pos];
}

Move const *
Model::find_move(Position pos) const
{
    auto i = next_moves_.find(pos);

    if (i == next_moves_.end()) {
        return nullptr;
    } else {
        return &*i;
    }
}

void
Model::play_move(Position pos)
{
    if (is_game_over()) {
        throw Client_logic_error("Model::play_move: game over");
    }

    Move const *movep = find_move(pos);
    if (!movep) {
        throw Client_logic_error("Model::play_move: no such move");
    }

    // TODO: actually execute the move, advance the turn, refill
    // `next_moves_`, etc.
    really_play_move_(*movep);
}

//
// BELOW ARE *OPTIONAL* HELPER FUNCTIONS
//

Position_set
Model::find_flips_(Position current, Dimensions dir) const
{
    // TODO OR NOT TODO: OPTIONAL HELPER

    Player curr = turn_;
    Player other = other_player(turn_);
    Position_set pset{};

    //current += dir;

    /*current += dir;
    if (!board_.good_position(current)) {
        return Position_set{};
    }
    curr = board_[current];*/

    while(true) {
        current += dir;
        if (!board_.good_position(current)) {
            return Position_set{};
        }
        if (board_[current] == other) {
            Position_set temp{current};
            //pset[current] = true;
            pset |= temp;
        }
        if (board_[current] == curr) {
            return pset;
        }
        if (board_[current] == Player::neither) {
            return Position_set{};
        }

    }

    /*while(curr == other) {
        current += dir;
        if (!board_.good_position(current)) {
            return Position_set{};
        }

        Position_set temp{current};
        pset |= temp;
        current += dir;
        curr = board_[current];
        if (curr == Player::neither) {
            return Position_set{};
        }

    }

    return pset;*/
}

Position_set
Model::evaluate_position_(Position pos) const
{
    // TODO OR NOT TODO: OPTIONAL HELPER

    if (board_[pos] != Player::neither) {
        return Position_set{};
    }

    Position_set all_gains{};

    for (auto dir : Board::all_directions()) {
        Position_set temp = find_flips_(pos, dir);
        all_gains |= temp;
    }

    if (all_gains.empty()) {
        return Position_set{};
    }

    return all_gains | Position_set{pos};
}

void
Model::compute_next_moves_()
{
    // TODO OR NOT TODO: OPTIONAL HELPER
    next_moves_.clear();

    //Position posn{1,1};
    //posn.x = board_.center_positions().x;

    for (auto pos : board_.center_positions()) {
        if (board_[pos] == Player::neither) {
            next_moves_[pos] = {pos};
        }
    }

    if (!next_moves_.empty()) {
        return;
    }

    for (auto pos : board_.all_positions()) {
        Position_set temp = evaluate_position_(pos);
        if (temp.empty()) {
            continue;
        }
        next_moves_[pos] = temp;
    }
}

bool
Model::advance_turn_()
{
    // TODO OR NOT TODO: OPTIONAL HELPER
    turn_ = other_player(turn_);
    compute_next_moves_();
    return !next_moves_.empty();
}

void
Model::set_game_over_()
{
    // TODO OR NOT TODO: OPTIONAL HELPER
    turn_ = Player::neither;
    size_t dark_count = board_.count_player(Player::dark);
    size_t light_count = board_.count_player(Player::light);

    if (dark_count > light_count) {
        winner_ = Player::dark;
    } else if (light_count > dark_count) {
        winner_ = Player::light;
    } else {
        winner_ = Player::neither;
    }
}

void
Model::really_play_move_(Move move)
{
    // TODO OR NOT TODO: OPTIONAL HELPER
    /*Position_set all_gains = Position_set{move.first}|Position_set{move
                                                                    .second};*/
    board_.set_all(move.second, turn_);

    if (!advance_turn_()) {
        if (!advance_turn_()) {
            set_game_over_();
        }
    }
}
