#include "view.hxx"

// Convenient type aliases:
using Color = ge211::Color;
using Sprite_set = ge211::Sprite_set;

// You can change this or even determine it some other way:
//static int const grid_size = 110;
static Color const dark_piece_color{0,0,0};
static Color const light_piece_color{255,255,255};
static Color const grid_color{0,115,0};
static Color const move_preview_color{255,0,0};
static Color const loser_grid_color{192,192,192};
static Color const winner_grid_color{255,215,0};
static ge211::Dims<int> const grid_dims{100,100};
static ge211::Dims<int> const move_preview_dims = grid_dims;
static ge211::Dims<int> const loser_grid_dims = grid_dims;
static ge211::Dims<int> const winner_grid_dims = grid_dims;
static int const piece_radius = 40;
static int const grid_buffer = 10;
//static ge211::Dims<int> const end_game_dims{1000,1000};

View::View(Model const& model)
        : model_(model)
        , light_piece_sprite(piece_radius, light_piece_color)
        , dark_piece_sprite(piece_radius, dark_piece_color)
        , grid_sprite(grid_dims, grid_color)
        , move_preview_sprite(move_preview_dims, move_preview_color)
        , loser_grid_sprite(loser_grid_dims, loser_grid_color)
        , winner_grid_sprite(winner_grid_dims, winner_grid_color)
        // You may want to add sprite initialization here
{ }

void View::draw(Sprite_set& set, ge211::Posn<int> mouse_pos)
{
    // TODO, PROBABLY
    //add_player_sprite_(set, model_.turn(), board_to_screen(posn), 2);


    //set.add_sprite(light_piece_sprite, posn, 2);

    for (auto pos: model_.board()) {
        //initializing grid
        int scale = grid_dims.width + grid_buffer;
        Position newPosn{scale*pos.x + grid_buffer,
                         scale*pos.y + grid_buffer};
        set.add_sprite(grid_sprite, newPosn, 1);

        //To indicate whose turn it is
        add_player_sprite_(set,
                           model_.turn(),
                           mouse_pos.down_right_by({-piece_radius,-piece_radius}),
                           4);

        //adding the player's piece to the grid
        add_player_sprite_(set, model_[pos], board_to_screen(pos), 3);

        //show move preview
        if (model_.find_move(screen_to_board(mouse_pos))) {
            for (auto pos_gain : model_.find_move(screen_to_board(mouse_pos))
            ->second) {
                int pos_gain_x = board_to_screen(pos_gain).x - grid_buffer;
                int pos_gain_y = board_to_screen(pos_gain).y - grid_buffer;
                set.add_sprite(move_preview_sprite, Position{pos_gain_x, pos_gain_y},
                               2);
            }
        }

        //game over
        if (model_.is_game_over()) {
            Player winner = model_.winner();
            for (auto final_pos : model_.board()) {
                int final_pos_x = board_to_screen(final_pos).x -
                                       grid_buffer;
                int final_pos_y = board_to_screen(final_pos).y -
                                       grid_buffer;
                if (model_[final_pos] != winner) {
                    set.add_sprite(loser_grid_sprite,
                                   Position{final_pos_x, final_pos_y},2);
                } else if (model_[final_pos] == winner) {
                    set.add_sprite(winner_grid_sprite,
                                   Position{final_pos_x, final_pos_y},2);
                }
                //text sprite saying who won (helper function that takes
                // in player
            }
        }


    }
}

View::Dimensions
View::initial_window_dimensions() const
{
    // You can change this if you want:
    int buffer = grid_buffer/model_.board().dimensions().width;
    /*ge211::Dims<int> newDims{model_.board().dimensions().width + buffer,
                             model_.board().dimensions().height + buffer};*/
    return (grid_dims.width + grid_buffer + buffer) * model_.board()
    .dimensions();
}

std::string
View::initial_window_title() const
{
    // You can change this if you want:
    return "Reversi";
}

// HELPER FUNCTIONS

void
View::add_player_sprite_(ge211::Sprite_set& sprites,
                         Player which,
                         ge211::Posn<int> physical,
                         int z_layer)
{
    if (which == Player::light) {
        sprites.add_sprite(light_piece_sprite, physical, z_layer);
    } else if (which == Player::dark) {
        sprites.add_sprite(dark_piece_sprite, physical, z_layer);
    }
}

View::Position
View::board_to_screen(Model::Position logical) const
{
    int buffer_x = grid_buffer * (logical.x + 1)
                   + (grid_dims.width/2 - piece_radius);
    int buffer_y = grid_buffer * (logical.y + 1)
                   + (grid_dims.height/2 - piece_radius);
    return View::Position(logical.x * grid_dims.width + buffer_x,
                          logical.y * grid_dims.height + buffer_y);
}

Model::Position
View::screen_to_board(View::Position physical) const {
    physical.x -= grid_dims.width/2;
    physical.y -= grid_dims.height/2;
    return View::Position((int) physical.x / grid_dims.width,
                          (int) physical.y / grid_dims.height);
}
