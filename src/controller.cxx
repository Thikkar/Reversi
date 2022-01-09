#include "controller.hxx"

Controller::Controller(int size)
        : Controller(size, size)
{ }

Controller::Controller(int width, int height)
        : model_(width, height),
          view_(model_),
          mouse_posn(0,0)
{ }

void
Controller::draw(ge211::Sprite_set& sprites)
{
    view_.draw(sprites, mouse_posn);
}

View::Dimensions
Controller::initial_window_dimensions() const
{
    return view_.initial_window_dimensions();
}

std::string
Controller::initial_window_title() const
{
    return view_.initial_window_title();
}

void
Controller::on_key(ge211::Key key)
{
    if (key == ge211::Key::code('q')) {
        quit();
    }
}

/*void
Controller::on_mouse_up(ge211::Mouse_button, ge211::Posn<int> pos)
{
    // TODO: your code here
    model_.play_move(pos);
}*/

/*void
Controller::on_mouse_down(ge211::Sprite_set& sprites, ge211::Mouse_button mouse_button,
                          ge211::Posn<int> pos)
{
    // TODO: your code here
    model_.play_move(pos);
    view_.draw(sprites, pos);
    //on_mouse_down(mouse_button, pos);
}*/

void
Controller::on_mouse_down(ge211::Mouse_button, ge211::Posn<int> pos)
{
    //mouse_posn = pos;
    if (model_.find_move(view_.screen_to_board(pos))) {
        model_.play_move(view_.screen_to_board(pos));
    }
}

void
Controller::on_mouse_move(ge211::Posn<int> pos)
{
    // TODO: your code here
    mouse_posn = pos;
    /*if (model_.find_move(view_.screen_to_board(pos))) {

    }*/
}


