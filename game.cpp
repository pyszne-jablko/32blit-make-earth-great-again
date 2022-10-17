#include "game.hpp"
#include "assets.hpp"

using namespace blit;

/**
 * \brief Position of player (top left corner of sprite).
 */
static Point player {8, 48};

static char player_dir = 'R';

constexpr std::size_t player_w = 16;
constexpr std::size_t player_h = 16;

/**
 * \brief Draw player sprite on screen with correct orientation.
 */
void draw_player()
{
    if(player_dir == 'L') {
        //screen.sprite(Point(4, 0), player); // Another way
        //screen.sprite(Point(5, 0), player + Point(8, 0));
        //screen.sprite(Point(4, 1), player + Point(0, 8));
        //screen.sprite(Point(5, 1), player + Point(8, 8));
        screen.sprite(Rect(4, 0, 2, 2), player);
    }
    else { // Direction 'R' or unknown
        //screen.sprite(Point(2, 0), player); // Another way
        //screen.sprite(Point(3, 0), player + Point(8, 0));
        //screen.sprite(Point(2, 1), player + Point(0, 8));
        //screen.sprite(Point(3, 1), player + Point(8, 8));
        screen.sprite(Rect(2, 0, 2, 2), player);
    }
    
}

/**
 * \brief Setup game.
 */
void init() {
    // ScreenMode::hires -> 320x240 (32blit DevKit)
    // ScreenMode::lores -> 160x120
    set_screen_mode(ScreenMode::hires);

    screen.sprites = Surface::load(asset_sprite);
}

/**
 * \brief This function is called to perform rendering of the game. time is the 
 *        amount if milliseconds elapsed since the start of your game
 */
void render(uint32_t time) {
    // white rgb -> 255, 255, 255
    // black rgb -> 0, 0, 0

    // clear the screen -- screen is a reference to the frame buffer and can be used to draw all things with the 32blit
    screen.pen = Pen(50, 50, 50); // almost black
    screen.clear();

    screen.alpha = 255;
    screen.mask = nullptr;

    screen.pen = Pen(0, 200, 0);
    screen.text("Make Earth Great Again", minimal_font, Point(5, 5));

    // Draw player sprite
    draw_player();
}

/**
 * \brief This is called to update your game state. time is the 
 *        amount if milliseconds elapsed since the start of your game
 */
void update(uint32_t time) {
    // 20Hz
    static uint32_t dpad_prev_time { 0 };
    if ((time - dpad_prev_time) > 50) { // 20Hz
        // Joystick calculations, because `joystick` is `Vec2` type.
        auto jx = static_cast<int>(joystick.x * 16);
        // auto jy = static_cast<int>(joystick.y * 16);

        if((buttons & Button::DPAD_RIGHT) or (jx > 4)) {
            player_dir = 'R';
            player.x += 8;
        }
        if((buttons & Button::DPAD_LEFT) or (jx < -4)) {
            player_dir = 'L';
            player.x -= 8;
        }

        // Min Max validation
        if(player.x < 0) {
            player.x = 0;
        }
        if(player.x > 128) {
            player.x = 128;
        }

        dpad_prev_time = time;
    }
}