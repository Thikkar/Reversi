#include "model.hxx"
#include <catch.hxx>

using namespace ge211;

// These pass with the starter code and should continue
// to pass.
TEST_CASE("Passes with starter code")
{
    Model m(8, 8);
    CHECK( m.board() == Model::Rectangle{0, 0, 8, 8} );
    CHECK_FALSE( m.is_game_over() );
    CHECK( m.turn() == Player::dark );
    CHECK( m.winner() == Player::neither );
    CHECK( m[{0, 0}] == Player::neither );
    CHECK( m.find_move({0, 0}) == nullptr );
    CHECK_THROWS_AS( m.play_move({0, 0}), Client_logic_error );
}

// This fails with the starter code, but should pass.
TEST_CASE("Fails with starter code")
{
    Model m(6);
    CHECK( m.find_move({2, 2}) );
}

// This fails with the starter code, but should pass.
TEST_CASE("Throws with starter code")
{
    Model m(4);
    m.play_move({1, 1});
}

// YOU NEED MANY MORE TESTS!

struct Test_access
{
    Model& model ;
    Board& board ()
    {
        return model.board_ ;
    }

    Move_map
    get_next_moves()
    {
        return model.next_moves_;
    }

    Position_set
    evaluate_position(ge211::Posn<int> pos)
    {
        return model.evaluate_position_(pos);
    }

    bool advance_turn()
    {
        return model.advance_turn_();
    }

    Position_set
    find_flips ( Model::Position p,
                 Model::Dimensions d )
    {
        return model.find_flips_(p, d);
    }
};

TEST_CASE("Gameplay Pre-Center Fill")
{
    Model m(4,4);
    Test_access t{m};
    //t.advance_turn();

    CHECK( m.turn() == Player::dark );
    Move_map initMap{};
    initMap[ge211::Posn<int>{1,1}] = Position_set{{1,1}};
    initMap[ge211::Posn<int>{2,1}] = Position_set{{2,1}};
    initMap[ge211::Posn<int>{1,2}] = Position_set{{1,2}};
    initMap[ge211::Posn<int>{2,2}] = Position_set{{2,2}};

    CHECK( t.get_next_moves() == initMap );
    m.play_move({1, 1});

    CHECK( t.evaluate_position(ge211::Posn<int>{0,0}) == Position_set{} );
    //try to play same piece
    CHECK( t.evaluate_position(ge211::Posn<int>{1,1}) == Position_set{} );
    //out of bounds case
    CHECK_THROWS_AS( t.evaluate_position(ge211::Posn<int>{10,10}),
                     Client_logic_error );
    CHECK( t.board()[{1,1}] == Player::dark );
    CHECK( m.turn() == Player::light );
    initMap.clear();
    initMap[ge211::Posn<int>{2,1}] = Position_set{{2,1}};
    initMap[ge211::Posn<int>{1,2}] = Position_set{{1,2}};
    initMap[ge211::Posn<int>{2,2}] = Position_set{{2,2}};
    CHECK( t.get_next_moves() == initMap );
    Position_set f1 = t.find_flips({1,2}, {0,1});
    Position_set f2 = t.find_flips({1,2}, {0,-1});
    CHECK( f1.empty() );
    CHECK( f2.empty() );

    m.play_move({1,2});
    CHECK( t.board()[{1,2}] == Player::light );
    CHECK( t.board().count_player(Player::light) == 1 );
    CHECK(t.advance_turn());
    initMap.clear();
    initMap[ge211::Posn<int>{2,1}] = Position_set{{2,1}};
    initMap[ge211::Posn<int>{2,2}] = Position_set{{2,2}};
    CHECK( t.get_next_moves() == initMap );

    m.play_move({2,1});
    initMap.clear();
    initMap[ge211::Posn<int>{2,2}] = Position_set{{2,2}};
    CHECK( t.get_next_moves() == initMap );


    m.play_move({2,2});
    CHECK( t.board().count_player(Player::light) == 2 );
    CHECK( t.board().count_player(Player::dark) == 2 );
}

TEST_CASE("Gameplay Post-Center Fill")
{
    Model m(4, 4);
    Test_access t {m};

    m.play_move({1, 1}); //dark
    m.play_move({1, 2}); //light
    m.play_move({2, 1}); //dark
    m.play_move({2, 2}); //light
    CHECK( m.turn() == Player::dark );

    Move_map mmap{};
    mmap[ge211::Posn<int>{0,3}] = Position_set{{0,3}, {1,2}};
    mmap[ge211::Posn<int>{1,3}] = Position_set{{1,3}, {1,2}};
    mmap[ge211::Posn<int>{2,3}] = Position_set{{2,3}, {2,2}};
    mmap[ge211::Posn<int>{3,3}] = Position_set{{3,3}, {2,2}};

    CHECK( t.evaluate_position(ge211::Posn<int>{0,0}) == Position_set{} );
    CHECK( t.get_next_moves() == mmap);

    CHECK( t.board()[{2,2}] == Player::light );
    m.play_move({3, 3}); //dark
    CHECK( t.board()[{2,2}] == Player::dark );

    CHECK( t.board()[{2,1}] == Player::dark );
    m.play_move({3, 0}); //light
    CHECK( t.board()[{2,1}] == Player::light );

    CHECK( t.board()[{1,2}] == Player::light );
    m.play_move({0, 2}); //dark
    CHECK( t.board()[{1,2}] == Player::dark );

    CHECK( t.board()[{2,2}] == Player::dark );
    m.play_move({2, 3}); //light
    CHECK( t.board()[{2,2}] == Player::light );

    CHECK( t.board()[{2,1}] == Player::light );
    m.play_move({3, 1}); //dark
    CHECK( t.board()[{2,1}] == Player::dark );

    CHECK( t.board()[{1,1}] == Player::dark );
    m.play_move({0, 0}); //light
    CHECK( t.board()[{1,1}] == Player::light );

    CHECK( t.board()[{2,2}] == Player::light );
    m.play_move({3, 2}); //dark
    CHECK( t.board()[{2,2}] == Player::dark );
    CHECK( m.turn() == Player::light );

    CHECK( t.board()[{1,2}] == Player::dark );
    m.play_move({1, 3}); //light
    CHECK( t.board()[{1,2}] == Player::light );
    CHECK( m.turn() == Player::dark );

    CHECK( t.board()[{1,2}] == Player::light );
    CHECK( t.board()[{1,3}] == Player::light );
    CHECK( t.board()[{2,3}] == Player::light );
    m.play_move({0, 3}); //dark
    CHECK( t.board()[{1,2}] == Player::dark );
    CHECK( t.board()[{1,3}] == Player::dark );
    CHECK( t.board()[{2,3}] == Player::dark );
    CHECK( m.turn() == Player::dark );


}

TEST_CASE("Winner player")
{
    Model m(4, 4);

    m.play_move({1, 1}); //dark
    m.play_move({1, 2}); //light
    m.play_move({2, 1}); //dark
    m.play_move({2, 2}); //light
    m.play_move({3, 3}); //dark
    m.play_move({3, 0}); //light
    m.play_move({0, 2}); //dark
    m.play_move({2, 3}); //light
    m.play_move({3, 1}); //dark
    m.play_move({0, 0}); //light
    m.play_move({3, 2}); //dark
    m.play_move({1, 3}); //light
    m.play_move({0, 3}); //dark

    CHECK ( m.winner() == Player::neither);
    CHECK_FALSE ( m.is_game_over() );

    m.play_move({0, 1}); //dark
    CHECK ( m.winner() == Player::dark);
    CHECK ( m.is_game_over() );

}

TEST_CASE("Winner tie (2x2)")
{
    Model m(2, 2);

    m.play_move({0, 0}); //dark
    m.play_move({0, 1}); //light
    m.play_move({1, 0}); //dark
    m.play_move({1, 1}); //light

    CHECK ( m.winner() == Player::neither);
    CHECK ( m.is_game_over() );

}