//
//  Texture.h
//  Pineapple
//
//  Created by Michael Patashnik on 3/24/16.
//
//

#ifndef __TEXTURE_H__
#define __TEXTURE_H__

/** The key for the tile tile texture in the asset manager */
#define FLOOR_TEXTURE     "floor"
/** The key for the floor top texture in the asset manager */
#define FLOOR_TOP_TEXTURE "floor_top"
/** The key for the platform texture in the asset manager */
#define PLATFORM_CENTER_TEXTURE "platform"
/** The key for the vertical platform texture in the asset manager */
#define PLATFORM_VERTICAL_CENTER_TEXTURE    "platform_center"
/** the key for the platform edge texture in the asset manager */
#define PLATFORM_EDGE_LEFT_TEXTURE   "platform_edge_left"
/** the key for the platform edge texture in the asset manager */
#define PLATFORM_EDGE_RIGHT_TEXTURE   "platform_edge_right"
/** The key for the win door texture in the asset manager */
#define GOAL_TEXTURE     "goal"
/** The key for the fridge door */
#define GOAL_DOOR_TEXTURE "goalDoor"
/** The key for the font for victory/failure messages in the asset manager */
#define MESSAGE_FONT     "retro"

/** The key for the splats 1 */
#define SPLAT_TEXTURE_1   "splat1"
/** The key for the splats 2 */
#define SPLAT_TEXTURE_2   "splat2"
/** The key for the splats 3 */
#define SPLAT_TEXTURE_3   "splat3"
/** The key for the splats 4 */
#define SPLAT_TEXTURE_4   "splat4"

/** The key for the font for level select buttons in the asset manager */
#define ELECTRIC_CIRCUS_FONT            "level_select_button_font"
#define ELECTRIC_CIRCUS_FONT_LOCATION  "fonts/ElectricCircus.ttf"

/** The texture for the character avatar */
#define PINEAPPLE_TEXTURE "pineapple"
/** The texture for the blender */
#define BLENDER_TEXTURE   "blender"
/** The texture for the first kid */
#define KID_TEXTURE_1     "pineapplet_bow"
/** The texture for the second kid */
#define KID_TEXTURE_2     "pineapplet_glasses"
/** The texture for the third kid */
#define KID_TEXTURE_3     "pineapplet_hat"
/** The texture for the fourth kid */
#define KID_TEXTURE_4     "pineapplet_pirate"

/** The texture for the spikes */
#define SPIKE_TEXTURE     "spikes_tex"
/** The texture for the jello */
#define JELLO_TEXTURE     "jello_tex"
/** The key for the spinner texture */
#define SPINNER_TEXTURE   "spinner"
/** The key for the red cup texture */
#define RED_CUP_TEXTURE	  "redcup"
/** The key for the blue cup texture */
#define BLUE_CUP_TEXTURE  "bluecup"
/** The key for the green cup texture */
#define GREEN_CUP_TEXTURE "greencup"
/** The key for the cupstack texture */
#define CUPSTACK_TEXTURE  "stackedcups"
/** The keys for the switch textures */
#define SWITCH_TEXTURE_BLUE   "switchBLUE"
#define SWITCH_TEXTURE_RED      "switchred"
#define SWITCH_TEXTURE_GREEN    "switchgreen"

#define SWITCH_REVERSE_TEXTURE_BLUE   "switchreverseBLUE"
#define SWITCH_REVERSE_TEXTURE_RED      "switchreversered"
#define SWITCH_REVERSE_TEXTURE_GREEN    "switchreversegreen"

/** The keys for the button texture */
#define BUTTON_TEXTURE_BLUE   "buttonBLUE"
#define BUTTON_TEXTURE_RED      "buttonred"
#define BUTTON_TEXTURE_GREEN    "buttongreen"
/** The keys for the movable platform textures */
#define LEFT_NUBBIN_TEXTURE_BLUE   "leftBLUE"
#define RIGHT_NUBBIN_TEXTURE_BLUE   "rightBLUE"
#define MIDDLE_TEXTURE_BLUE       "middleBLUE"
#define LEFT_NUBBIN_TEXTURE_RED        "leftred"
#define RIGHT_NUBBIN_TEXTURE_RED       "rightred"
#define MIDDLE_TEXTURE_RED          "middlered"
#define LEFT_NUBBIN_TEXTURE_GREEN      "leftgreen"
#define RIGHT_NUBBIN_TEXTURE_GREEN     "rightgreen"
#define MIDDLE_TEXTURE_GREEN        "middlegreen"

/** The key for the main background texture in the asset manager */
#define FRONT_BACKGROUND  "background_1"
/** The key for the further back background texture in the asset manager */
#define MIDDLE_BACKGROUND "background_2"
/** The key for the furthest back background texture in the asset manager */
#define BACK_BACKGROUND   "background_3"

/** The key for the pause screen overlay texture in the asset manager */
#define PAUSE_SCREEN_OVERLAY "pause_overlay"
#define CHUNKY_QUIVER        "chunky_quiver"

/** HUD */
#define TOP_BAR  "top_bar"
#define TOP_BLENDER "top_blender"
#define TOP_CHILD_BOW "top_child_bow"
#define TOP_CHILD_GLASSES "top_child_glasses"
#define TOP_CHILD_HAT "top_child_hat"
#define TOP_CHILD_PIRATE "top_child_pirate"
#define TOP_WILL "top_will"
#define STATUS_ONE         "one"
#define STATUS_TWO         "two"
#define STATUS_THREE       "three"
#define STATUS_FOUR        "four"
#define SLASH_FOUR         "slash_four"

/** The key for the loading screen background texture in the asset manager */
#define LOADING_BACKGROUND      "loading_background"

/** The key for the home screen background texture */
#define HOME_SCREEN_BACKGROUND "home_sceen_background"
/** The key for the play button on the home screen in its unpushed state */
#define HOME_SCREEN_PLAY_BUTTON_OFF "home_screen_play_button_off"
/** The key for the play button on the home screen in its unpushed state */
#define HOME_SCREEN_PLAY_BUTTON_ON "home_screen_play_button_on"
/** The filepath for the play button in its off state */
#define HOME_SCREEN_PLAY_BUTTON_OFF_FILEPATH  "textures/play_button.png"
/** The filepath for the play button in its on state */
#define HOME_SCREEN_PLAY_BUTTON_ON_FILEPATH  "textures/play_button_inverse.png"
/** The key for the will popup animation used on the homescreen */
#define HOME_SCREEN_WILL_ANIMATION  "home_screen_will_animation"

/** The key for the level select background texture in the asset manager */
#define LEVEL_SELECT_BACKGROUND "level_select_background"
/** The key for the unselected level button texture in the asset manager */
#define LEVEL_SELECT_LEVEL_BUTTON_OFF "level_select_level_button"
/** The key for the selected level button texture in the asset manager */
#define LEVEL_SELECT_LEVEL_BUTTON_ON "level_select_level_button_selected"
/** The filepath for the level select button in its off state */
#define LEVEL_SELECT_BUTTON_OFF_FILEPATH  "textures/buttons/level_button.png"
/** The filepath for the level select button in its on state */
#define LEVEL_SELECT_BUTTON_ON_FILEPATH  "textures/buttons/level_button_inverse.png"

/** The filepaths for the different score textures, used in level select */
#define LEVEL_SELECT_SCORE_FILEPATH_0 "textures/scores/0_score.png"
#define LEVEL_SELECT_SCORE_FILEPATH_1 "textures/scores/1_score.png"
#define LEVEL_SELECT_SCORE_FILEPATH_2 "textures/scores/2_score.png"
#define LEVEL_SELECT_SCORE_FILEPATH_3 "textures/scores/3_score.png"
#define LEVEL_SELECT_SCORE_FILEPATH_4 "textures/scores/4_score.png"

/** The filepaths for the different movement textures */
#define MOVE_LEFT_PRESSED "textures/left_right_move_view/pressed_arrow_left.png"
#define MOVE_LEFT_UNPRESSED "textures/left_right_move_view/unpressed_arrow_left.png"
#define MOVE_RIGHT_PRESSED "textures/left_right_move_view/pressed_arrow_right.png"
#define MOVE_RIGHT_UNPRESSED "textures/left_right_move_view/unpressed_arrow_right.png"

/** The key for the lose splash screen */
#define LOSE_SPLASH    "lose_splash"
/** The key for the image of the juice cup for the lose splash screen */
#define LOSE_JUICE_CUP "lose_juice_cup"
/** The key for the crying will animation for the lose splash screen */
#define LOSE_CRYING_WILL    "lose_crying_will"
/** The key for the win splash screen */
#define WIN_SPLASH    "win_splash"
/** The keys for the children animations on the win splash screen */
#define WIN_SPLASH_CHILD_1  "win_splash_child_1"
#define WIN_SPLASH_CHILD_2  "win_splash_child_2"
#define WIN_SPLASH_CHILD_3  "win_splash_child_3"
#define WIN_SPLASH_CHILD_4  "win_splash_child_4"
#define WIN_SPLASH_WILL     "win_splash_will"


/** Tutorial messages */
#define TUTORIAL_BLENDER_MESSAGE    "blender_message"
#define TUTORIAL_FRIDGE_MESSAGE     "fridge_message"
#define TUTORIAL_KNIVES_MESSAGE     "knives_message"
#define TUTORIAL_GROW_MESSAGE       "grow_message"
#define TUTORIAL_JUMP_MESSAGE       "jump_message"
#define TUTORIAL_MOVE_MESSAGE       "move_message"
#define TUTORIAL_SHRINK_MESSAGE     "shrink_message"
#define TUTORIAL_SWITCH_MESSAGE     "switch_message"
#define TUTORIAL_SMASH_MESSAGE      "smash_message"
#define TUTORIAL_JUMP_SIZE_MESSAGE "jump_size_message"

/** Tutorial images */
#define TUTORIAL_SHRINK_IMAGE   "shrink_image"
#define TUTORIAL_GROW_IMAGE     "grow_image"
#define TUTORIAL_JUMP_IMAGE     "jump_image"
#define TUTORIAL_DOOR_IMAGE     "door_image"
#define TUTORIAL_SWITCH_IMAGE   "switch_image"
#define TUTORIAL_POINTER_IMAGE  "pointer_image"
#define TUTORIAL_SMASH_IMAGE    "smash_image"
#define TUTORIAL_KNIVES_IMAGE   "knives_image"

/** Tutorial buttons */
#define TUTORIAL_BUTTON                 "tutorial_button"
#define TUTORIAL_BUTTON_FILEPATH        "textures/tutorial/tutorial_button.png"
#define TUTORIAL_BUTTON_ON              "tutorial_button_on"
#define TUTORIAL_BUTTON_ON_FILEPATH    "textures/tutorial/tutorial_button_inverse.png"

#endif /* Texture_h */
