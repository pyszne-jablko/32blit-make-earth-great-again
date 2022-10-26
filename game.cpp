#include "game.hpp"
#include "assets.hpp"

using namespace blit;

static char player_dir = 'R';

static bool g_draw_btn_x {false};
static bool g_draw_btn_y {false};
static bool g_draw_btn_a {false};
static bool g_draw_btn_b {false};
static bool g_draw_btn_menu {false};
static bool g_draw_btn_home {false};

constexpr std::size_t player_w = 16;
constexpr std::size_t player_h = 16;

/* ********** GLOBALS ********** */

TileMap* environment; //!< Tail map

std::string local_error_string {}; //!< String with information about error, if empty then no error.

static Point player {8, 48}; //!< Position of player (top left corner of sprite).

/**
 * \brief Draw player sprite on screen with correct orientation.
 */
void draw_player() {
    if(player_dir == 'L') {
        screen.sprite(Rect(4, 0, 2, 2), player);
    }
    else { // Direction 'R' or unknown
        screen.sprite(Rect(2, 0, 2, 2), player);
    }
    
}

/**
 * \brief Draw a background of dialog "window".
 * \param rect [in] Start point and width and height. All dimensions in pixels.
 */
void draw_dialog(Rect rect) {
    const Point local_max {rect.x + rect.w, rect.y + rect.h};

    // Draw corners
    screen.sprite(Point(10, 0), Point(100, 100));
    screen.sprite(Point(11, 0), Point(100 + 8, 100));
    screen.sprite(Point(11, 0), Point(100 + 16, 100));
    screen.sprite(Point(11, 0), Point(100 + 24, 100));
    screen.sprite(Point(10, 0), Point(100 + 32, 100), SpriteTransform::R90);

    screen.sprite(Point(10, 0), Point(100, 108), SpriteTransform::R270);
    screen.sprite(Point(11, 0), Point(100 + 8, 108), SpriteTransform::R180);
    screen.sprite(Point(11, 0), Point(100 + 16, 108), SpriteTransform::R180);
    screen.sprite(Point(11, 0), Point(100 + 24, 108), SpriteTransform::R180);
    screen.sprite(Point(10, 0), Point(100 + 32, 108), SpriteTransform::R180);
}

/**
 * \brief Setup game.
 */
void init() {
    // ScreenMode::hires -> 320x240 (32blit DevKit)
    // ScreenMode::lores -> 160x120
    set_screen_mode(ScreenMode::hires);

    // Load sprite
    screen.sprites = Surface::load(asset_sprite);

    // Load map
    environment = TileMap::load_tmx(map_1, screen.sprites);
    if(nullptr == environment) {
        local_error_string = "Can't load Tile Map";
    }
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

    // Draw map
    environment->draw(&screen, Rect(0, 0, 320, 240)); // It looks like "Rect" width and height are in pixels, not sprites.

    // Draw text with game name
    screen.pen = Pen(0, 200, 0);
    screen.text("Make Earth Great Again", minimal_font, Point(5, 5));

    Point txt_pos = Point(10, 20);
    // Draw test with error (if needed)
    if(local_error_string.length() != 0) {
        screen.pen = Pen(200, 0, 0);
        screen.text(local_error_string.c_str(), minimal_font, txt_pos);
        txt_pos.y += 10;
    }

    screen.pen = Pen(200, 0, 0);
    if(g_draw_btn_x) {
        screen.text("X button pressed", minimal_font, txt_pos);
        txt_pos.y += 10;
    }
    if(g_draw_btn_y) {
        screen.text("Y button pressed", minimal_font, txt_pos);
        txt_pos.y += 10;
    }
    if(g_draw_btn_a) {
        screen.text("A button pressed", minimal_font, txt_pos);
        txt_pos.y += 10;
    }
    if(g_draw_btn_b) {
        screen.text("B button pressed", minimal_font, txt_pos);
        txt_pos.y += 10;
    }
    if(g_draw_btn_menu) {
        screen.text("MENU button pressed", minimal_font, txt_pos);
        txt_pos.y += 10;
        draw_dialog(Rect(100, 100, 80, 40));
    }
    if(g_draw_btn_home) {
        screen.text("HOME button pressed", minimal_font, txt_pos);
        txt_pos.y += 10;
    }

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
        auto jy = static_cast<int>(joystick.y * 16);

        if((buttons & Button::DPAD_RIGHT) or (jx > 4)) {
            player_dir = 'R';
            player.x += 8;
        }
        if((buttons & Button::DPAD_LEFT) or (jx < -4)) {
            player_dir = 'L';
            player.x -= 8;
        }
        if((buttons & Button::DPAD_UP) or (jy > 4)) {
            player_dir = 'R';
            player.y -= 8;
        }
        if((buttons & Button::DPAD_DOWN) or (jy < -4)) {
            player_dir = 'L';
            player.y += 8;
        }

        if(buttons & Button::X)
        {
            g_draw_btn_x = true;
        }
        else {
            g_draw_btn_x = false;
        }
        if(buttons & Button::Y)
        {
            g_draw_btn_y = true;
        }
        else {
            g_draw_btn_y = false;
        }
        if(buttons & Button::A)
        {
            g_draw_btn_a = true;
        }
        else {
            g_draw_btn_a = false;
        }
        if(buttons & Button::B)
        {
            g_draw_btn_b = true;
        }
        else {
            g_draw_btn_b = false;
        }
        if(buttons & Button::MENU)
        {
            g_draw_btn_menu = true;
        }
        else {
            g_draw_btn_menu = false;
        }
        if(buttons & Button::HOME)
        {
            g_draw_btn_home = true;
        }
        else {
            g_draw_btn_home = false;
        }

        // Min Max validation
        if(player.x < 0) {
            player.x = 0;
        }
        int max_x = 320 - player_w;
        if(player.x > max_x) {
            player.x = max_x;
        }
        if(player.y < 0) {
            player.y = 0;
        }
        int max_y = 240 - player_h;
        if(player.y > max_y) {
            player.y = max_y;
        }

        // Save time as previous
        dpad_prev_time = time;
    }
}