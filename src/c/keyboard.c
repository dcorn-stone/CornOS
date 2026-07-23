#include <stdint.h>
#include "keyboard.h"
#include "io.h"

#define KEYBOARD_EVENT_BUFFER 128

_Static_assert(ESCAPE_RELEASED == PAUSE_PRESSED + 1,
               "Pressed/released layout broken");

_Static_assert(UNKNOWN_KEY == 255,
               "Unknown key must remain 255");


// Yeah, I know this looks like a mess, sorry for leaving it like this,
// but doing something else only makes it more complicated

/* Scan code mapping for set 1 */
static const uint8_t set1_upper_pressed[] = {
        0, ESCAPE_PRESSED, ONE_PRESSED, TWO_PRESSED, THREE_PRESSED, FOUR_PRESSED,
        FIVE_PRESSED, SIX_PRESSED, SEVEN_PRESSED, EIGHT_PRESSED, NINE_PRESSED,
        ZERO_PRESSED, MINUS_PRESSED, EQUAL_PRESSED, BACKSPACE_PRESSED,
        TAB_PRESSED, Q_PRESSED, W_PRESSED, E_PRESSED, R_PRESSED, T_PRESSED,
        Y_PRESSED, U_PRESSED, I_PRESSED, O_PRESSED, P_PRESSED, L_SQ_BRACKET_PRESSED,
        R_SQ_BRACKET_PRESSED, ENTER_PRESSED, LEFT_CTRL_PRESSED, A_PRESSED,
        S_PRESSED, D_PRESSED, F_PRESSED, G_PRESSED, H_PRESSED, J_PRESSED,
        K_PRESSED, L_PRESSED, SEMI_COLON_PRESSED, SINGLE_QUOTE_PRESSED,
        BACK_TICK_PRESSED, LEFT_SHIFT_PRESSED, BACKSLASH_PRESSED, Z_PRESSED,
        X_PRESSED, C_PRESSED, V_PRESSED, B_PRESSED, N_PRESSED, M_PRESSED,
        COMMA_PRESSED, PERIOD_PRESSED, FORWARD_SLASH_PRESSED, RIGHT_SHIFT_PRESSED,
        KEYPAD_ASTERISK_PRESSED, LEFT_ALT_PRESSED, SPACE_PRESSED, CAPSLOCK_PRESSED,
        F1_PRESSED, F2_PRESSED, F3_PRESSED, F4_PRESSED, F5_PRESSED, F6_PRESSED,
        F7_PRESSED, F8_PRESSED, F9_PRESSED, F10_PRESSED, NUMBERLOCK_PRESSED,
        SCROLLLOCK_PRESSED, KEYPAD_SEVEN_PRESSED, KEYPAD_EIGHT_PRESSED,
        KEYPAD_NINE_PRESSED, KEYPAD_MINUS_PRESSED, KEYPAD_FOUR_PRESSED,
        KEYPAD_FIVE_PRESSED, KEYPAD_SIX_PRESSED, KEYPAD_PLUS_PRESSED,
        KEYPAD_ONE_PRESSED, KEYPAD_TWO_PRESSED, KEYPAD_THREE_PRESSED,
        KEYPAD_ZERO_PRESSED, KEYPAD_PERIOD_PRESSED, 0,0, ISO_EXTRA_PRESSED, F11_PRESSED,
        F12_PRESSED
};

static const uint8_t set1_upper_released[] = {
        ESCAPE_RELEASED, ONE_RELEASED, TWO_RELEASED, THREE_RELEASED, FOUR_RELEASED,
        FIVE_RELEASED, SIX_RELEASED, SEVEN_RELEASED, EIGHT_RELEASED, NINE_RELEASED,
        ZERO_RELEASED, MINUS_RELEASED, EQUAL_RELEASED, BACKSPACE_RELEASED,
        TAB_RELEASED, Q_RELEASED, W_RELEASED, E_RELEASED, R_RELEASED, T_RELEASED,
        Y_RELEASED, U_RELEASED, I_RELEASED, O_RELEASED, P_RELEASED, L_SQ_BRACKET_RELEASED,
        R_SQ_BRACKET_RELEASED, ENTER_RELEASED, LEFT_CTRL_RELEASED, A_RELEASED, 
        S_RELEASED, D_RELEASED, F_RELEASED, G_RELEASED, H_RELEASED, J_RELEASED,
        K_RELEASED, L_RELEASED, SEMI_COLON_RELEASED, SINGLE_QUOTE_RELEASED, 
        BACK_TICK_RELEASED, LEFT_SHIFT_RELEASED, BACKSLASH_RELEASED,
        Z_RELEASED, X_RELEASED, C_RELEASED, V_RELEASED, B_RELEASED, N_RELEASED,
        M_RELEASED, COMMA_RELEASED, PERIOD_RELEASED, FORWARD_SLASH_RELEASED,
        RIGHT_SHIFT_RELEASED, KEYPAD_ASTERISK_RELEASED, LEFT_ALT_RELEASED,
        SPACE_RELEASED, CAPSLOCK_RELEASED, F1_RELEASED, F2_RELEASED, F3_RELEASED,
        F4_RELEASED, F5_RELEASED, F6_RELEASED, F7_RELEASED, F8_RELEASED, F9_RELEASED,
        F10_RELEASED, NUMBERLOCK_RELEASED, SCROLLLOCK_RELEASED, KEYPAD_SEVEN_RELEASED,
        KEYPAD_EIGHT_RELEASED, KEYPAD_NINE_RELEASED, KEYPAD_MINUS_RELEASED,
        KEYPAD_FOUR_RELEASED, KEYPAD_FIVE_RELEASED, KEYPAD_SIX_RELEASED,
        KEYPAD_PLUS_RELEASED, KEYPAD_ONE_RELEASED, KEYPAD_TWO_RELEASED, KEYPAD_THREE_RELEASED,
        KEYPAD_ZERO_RELEASED, KEYPAD_PERIOD_RELEASED, 0,0, ISO_EXTRA_RELEASED, F11_RELEASED,
        F12_RELEASED
};

static const uint8_t set1_lower_pressed[] = {
        PREVIOUS_TRACK_PRESSED, 0,0,0,0,0,0,0,0, NEXT_TRACK_PRESSED, 0,0, KEYPAD_ENTER_PRESSED,
        RIGHT_CTRL_PRESSED, 0,0, MUTE_PRESSED, CALCULATOR_PRESSED, PLAY_PAUSE_PRESSED, 0,
        STOP_PRESSED, 0,0,0,0,0,0,0,0,0, VOLUME_DOWN_PRESSED, 0, VOLUME_UP_PRESSED, 0,
        WWW_HOME_PRESSED, 0,0, KEYPAD_FORWARD_SLASH_PRESSED, 0,0, RIGHT_ALT_PRESSED, 0,0,0,0,0,0,0,
        0,0,0,0,0,0,0, HOME_PRESSED, UP_PRESSED, PAGE_UP_PRESSED, 0, LEFT_PRESSED,
        0, RIGHT_PRESSED, 0, END_PRESSED, DOWN_PRESSED, PAGE_DOWN_PRESSED, INSERT_PRESSED,
        DELETE_PRESSED, 0,0,0,0,0,0,0, LEFT_GUI_PRESSED, RIGHT_GUI_PRESSED, APPS_PRESSED,
        POWER_PRESSED, SLEEP_PRESSED, 0,0,0, WAKE_PRESSED, 0, WWW_SEARCH_PRESSED, 
        WWW_FAVOURITES_PRESSED, WWW_REFRESH_PRESSED, WWW_STOP_PRESSED, WWW_FORWARD_PRESSED,
        WWW_BACK_PRESSED, MY_COMPUTER_PRESSED, EMAIL_PRESSED, MEDIA_SELECT_PRESSED,
};

static const uint8_t set1_lower_released[] = {
        PREVIOUS_TRACK_RELEASED, 0,0,0,0,0,0,0,0, NEXT_TRACK_RELEASED, 0,0, KEYPAD_ENTER_RELEASED,
        RIGHT_CTRL_RELEASED, 0,0, MUTE_RELEASED, CALCULATOR_RELEASED, PLAY_PAUSE_RELEASED, 0,
        STOP_RELEASED, 0,0,0,0,0,0,0,0,0, VOLUME_DOWN_RELEASED, 0, VOLUME_UP_RELEASED, 0,
        WWW_HOME_RELEASED, 0,0, KEYPAD_FORWARD_SLASH_RELEASED, 0,0, RIGHT_ALT_RELEASED, 0,0,0,0,0,0,0,
        0,0,0,0,0,0,0, HOME_RELEASED, UP_RELEASED, PAGE_UP_RELEASED, 0, LEFT_RELEASED,
        0, RIGHT_RELEASED, 0, END_RELEASED, DOWN_RELEASED, PAGE_DOWN_RELEASED, INSERT_RELEASED,
        DELETE_RELEASED, 0,0,0,0,0,0,0, LEFT_GUI_RELEASED, RIGHT_GUI_RELEASED, APPS_RELEASED,
        POWER_RELEASED, SLEEP_RELEASED, 0,0,0, WAKE_RELEASED, 0, WWW_SEARCH_RELEASED, 
        WWW_FAVOURITES_RELEASED, WWW_REFRESH_RELEASED, WWW_STOP_RELEASED, WWW_FORWARD_RELEASED,
        WWW_BACK_RELEASED, MY_COMPUTER_RELEASED, EMAIL_RELEASED, MEDIA_SELECT_RELEASED,
};




/* Scan code mapping for set 2 */
static const uint8_t set2_upper_pressed[] = {
        0, F9_PRESSED, 0, F5_PRESSED, F3_PRESSED, F1_PRESSED, F2_PRESSED, F12_PRESSED,
        0, F10_PRESSED, F8_PRESSED, F6_PRESSED, F4_PRESSED, TAB_PRESSED, BACK_TICK_PRESSED,
        0,0, LEFT_ALT_PRESSED, LEFT_SHIFT_PRESSED, 0, LEFT_CTRL_PRESSED, Q_PRESSED, ONE_PRESSED,
        0,0,0, Z_PRESSED, S_PRESSED, A_PRESSED, W_PRESSED, TWO_PRESSED, 0,0, C_PRESSED,
        X_PRESSED, D_PRESSED, E_PRESSED, FOUR_PRESSED, THREE_PRESSED, 0,0, SPACE_PRESSED,
        V_PRESSED, F_PRESSED, T_PRESSED, R_PRESSED, FIVE_PRESSED, 0,0, N_PRESSED, B_PRESSED,
        H_PRESSED, G_PRESSED, Y_PRESSED, SIX_PRESSED, 0,0,0, M_PRESSED, J_PRESSED,
        U_PRESSED, SEVEN_PRESSED, EIGHT_PRESSED, 0,0, COMMA_PRESSED, K_PRESSED, I_PRESSED,
        O_PRESSED, ZERO_PRESSED, NINE_PRESSED, 0,0, PERIOD_PRESSED, FORWARD_SLASH_PRESSED,
        L_PRESSED, SEMI_COLON_PRESSED, P_PRESSED, MINUS_PRESSED, 0,0,0, SINGLE_QUOTE_PRESSED,
        0, L_SQ_BRACKET_PRESSED, EQUAL_PRESSED, 0,0, CAPSLOCK_PRESSED, RIGHT_SHIFT_PRESSED,
        ENTER_PRESSED, R_SQ_BRACKET_PRESSED, 0, BACKSLASH_PRESSED, 0,0,0,0,0,0,0,0,
        BACKSPACE_PRESSED, 0,0, KEYPAD_ONE_PRESSED, 0, KEYPAD_FOUR_PRESSED,
        KEYPAD_SEVEN_PRESSED, 0,0,0, KEYPAD_ZERO_PRESSED, KEYPAD_PERIOD_PRESSED,
        KEYPAD_TWO_PRESSED, KEYPAD_FIVE_PRESSED, KEYPAD_SIX_PRESSED, KEYPAD_EIGHT_PRESSED,
        ESCAPE_PRESSED, NUMBERLOCK_PRESSED, F11_PRESSED, KEYPAD_PLUS_PRESSED,
        KEYPAD_THREE_PRESSED, KEYPAD_MINUS_PRESSED, KEYPAD_ASTERISK_RELEASED,
        KEYPAD_NINE_PRESSED, SCROLLLOCK_PRESSED, F7_PRESSED
};

static const uint8_t set2_lower_pressed[] = {
        WWW_SEARCH_PRESSED, RIGHT_ALT_PRESSED, 0,0, RIGHT_CTRL_PRESSED, PREVIOUS_TRACK_PRESSED,
        0,0, WWW_FAVOURITES_PRESSED, 0,0,0,0,0,0, LEFT_GUI_PRESSED, WWW_REFRESH_PRESSED,
        VOLUME_DOWN_PRESSED, 0, MUTE_PRESSED, 0,0,0, RIGHT_GUI_PRESSED, WWW_STOP_PRESSED,
        0,0, CALCULATOR_PRESSED, 0,0,0, APPS_PRESSED, WWW_FORWARD_PRESSED, 0,
        VOLUME_UP_PRESSED, 0, PLAY_PAUSE_PRESSED, 0,0, POWER_PRESSED, WWW_BACK_PRESSED,
        0, WWW_HOME_PRESSED, STOP_PRESSED, 0,0,0, SLEEP_PRESSED, MY_COMPUTER_PRESSED,
        0,0,0,0,0,0,0, EMAIL_PRESSED, 0, KEYPAD_FORWARD_SLASH_PRESSED, 0,0,
        NEXT_TRACK_PRESSED, 0,0, MEDIA_SELECT_PRESSED, 0,0,0,0,0,0,0,0,0, KEYPAD_ENTER_PRESSED,
        0,0,0, WAKE_PRESSED, 0,0,0,0,0,0,0,0,0,0, END_PRESSED, 0, LEFT_PRESSED,
        HOME_PRESSED, 0,0,0, INSERT_PRESSED, DELETE_PRESSED, DOWN_PRESSED, 0,
        RIGHT_PRESSED, UP_PRESSED, 0,0,0,0, PAGE_DOWN_PRESSED, 0,0, PAGE_UP_PRESSED
};

static const uint8_t set2_upper_released[] = {
        0, F9_RELEASED, 0, F5_RELEASED, F3_RELEASED, F1_RELEASED, F2_RELEASED, F12_RELEASED,
        0, F10_RELEASED, F8_RELEASED, F6_RELEASED, F4_RELEASED, TAB_RELEASED, BACK_TICK_RELEASED,
        0,0, LEFT_ALT_RELEASED, LEFT_SHIFT_RELEASED, 0, LEFT_CTRL_RELEASED, Q_RELEASED, ONE_RELEASED,
        0,0,0, Z_RELEASED, S_RELEASED, A_RELEASED, W_RELEASED, TWO_RELEASED, 0,0, C_RELEASED,
        X_RELEASED, D_RELEASED, E_RELEASED, FOUR_RELEASED, THREE_RELEASED, 0,0, SPACE_RELEASED,
        V_RELEASED, F_RELEASED, T_RELEASED, R_RELEASED, FIVE_RELEASED, 0,0, N_RELEASED, B_RELEASED,
        H_RELEASED, G_RELEASED, Y_RELEASED, SIX_RELEASED, 0,0,0, M_RELEASED, J_RELEASED,
        U_RELEASED, SEVEN_RELEASED, EIGHT_RELEASED, 0,0, COMMA_RELEASED, K_RELEASED, I_RELEASED,
        O_RELEASED, ZERO_RELEASED, NINE_RELEASED, 0,0, PERIOD_RELEASED, FORWARD_SLASH_RELEASED,
        L_RELEASED, SEMI_COLON_RELEASED, P_RELEASED, MINUS_RELEASED, 0,0,0, SINGLE_QUOTE_RELEASED,
        0, L_SQ_BRACKET_RELEASED, EQUAL_RELEASED, 0,0, CAPSLOCK_RELEASED, RIGHT_SHIFT_RELEASED,
        ENTER_RELEASED, R_SQ_BRACKET_RELEASED, 0, BACKSLASH_RELEASED, 0,0,0,0,0,0,0,0,
        BACKSPACE_RELEASED, 0,0, KEYPAD_ONE_RELEASED, 0, KEYPAD_FOUR_RELEASED,
        KEYPAD_SEVEN_RELEASED, 0,0,0, KEYPAD_ZERO_RELEASED, KEYPAD_PERIOD_RELEASED,
        KEYPAD_TWO_RELEASED, KEYPAD_FIVE_RELEASED, KEYPAD_SIX_RELEASED, KEYPAD_EIGHT_RELEASED,
        ESCAPE_RELEASED, NUMBERLOCK_RELEASED, F11_RELEASED, KEYPAD_PLUS_RELEASED,
        KEYPAD_THREE_RELEASED, KEYPAD_MINUS_RELEASED, KEYPAD_ASTERISK_RELEASED,
        KEYPAD_NINE_RELEASED, SCROLLLOCK_RELEASED, F7_RELEASED
};

static const uint8_t set2_lower_released[] = {
        WWW_SEARCH_RELEASED, RIGHT_ALT_RELEASED, 0,0, RIGHT_CTRL_RELEASED, PREVIOUS_TRACK_RELEASED,
        0,0, WWW_FAVOURITES_RELEASED, 0,0,0,0,0,0, LEFT_GUI_RELEASED, WWW_REFRESH_RELEASED,
        VOLUME_DOWN_RELEASED, 0, MUTE_RELEASED, 0,0,0, RIGHT_GUI_RELEASED, WWW_STOP_RELEASED,
        0,0, CALCULATOR_RELEASED, 0,0,0, APPS_RELEASED, WWW_FORWARD_RELEASED, 0,
        VOLUME_UP_RELEASED, 0, PLAY_PAUSE_RELEASED, 0,0, POWER_RELEASED, WWW_BACK_RELEASED,
        0, WWW_HOME_RELEASED, STOP_RELEASED, 0,0,0, SLEEP_RELEASED, MY_COMPUTER_RELEASED,
        0,0,0,0,0,0,0, EMAIL_RELEASED, 0, KEYPAD_FORWARD_SLASH_RELEASED, 0,0,
        NEXT_TRACK_RELEASED, 0,0, MEDIA_SELECT_RELEASED, 0,0,0,0,0,0,0,0,0, KEYPAD_ENTER_RELEASED,
        0,0,0, WAKE_RELEASED, 0,0,0,0,0,0,0,0,0,0, END_RELEASED, 0, LEFT_RELEASED,
        HOME_RELEASED, 0,0,0, INSERT_RELEASED, DELETE_RELEASED, DOWN_RELEASED, 0,
        RIGHT_RELEASED, UP_RELEASED, 0,0,0,0, PAGE_DOWN_RELEASED, 0,0, PAGE_UP_RELEASED
};
/* Ignore this piece of trash if you are noting modifying it */


/* read_scan_code:
 *      Read the scan code from the keyboard's data port
 *
 *      @return  The scan code from the keyboard
 */
uint8_t read_scan_code(void)
{
        return inb(KBD_DATA_PORT);
}

/* key_queue:
 *      keyboard event queue
 *      A struct containing a queue of key events that need to be handled
 */
static struct {
        keycode_t keycode[KEYBOARD_EVENT_BUFFER];
        volatile uint8_t head;
        volatile uint8_t rear;
}key_queue;

/* init_key_event_queue:
 *      Initialises the key event queue, resets the head and rear
 */
static void init_key_event_queue(void)
{
        key_queue.head = 0;
        key_queue.rear = 0;
}

/* is_key_event_queue_empty:
 *      Checks if the keyboard event queue is empty
 *
 *      @return  0 - if the queue is not empty
 *               1 - if the queue is empty
 */
static uint8_t is_key_event_queue_empty(void)
{
        return key_queue.rear == key_queue.head;
}

/* is_key_event_queue_full:
 *      Checks if the keyboard event queue is full
 *
 *      @return  0 - if the queue is not full
 *               1 - if the queue is full
 */
static uint8_t is_key_event_queue_full(void)
{
        return (key_queue.rear + 1) % KEYBOARD_EVENT_BUFFER == key_queue.head;
}

/* enqueue_key_event:
 *      Adds an event to the key event queue, abort if the queue if full
 *
 *      @param keycode  The keycode to add to the event queue
 */
static void enqueue_key_event(keycode_t keycode)
{
        if (is_key_event_queue_full())
                return;

        key_queue.keycode[key_queue.rear] = keycode;
        key_queue.rear = (key_queue.rear + 1) % KEYBOARD_EVENT_BUFFER;
        return;
}

/* key_queue_get_event:
 *      Returns the next key event to be handled from the queue, returns 0 if empty
 *      !!!!! This should be the only key queue function that the kernal space can call
 *      Every other key queue function should remain private to this file as the kernel should not
 *      be modifying the queue directly
 *
 *      @return  The keycode of the next event in the queue
 *               Returns 0 if the queue is empty so nothing needs to be handled
 */
keycode_t key_queue_get_event(void)
{
        if(is_key_event_queue_empty())
                return NO_KEY;

        keycode_t next_key = key_queue.keycode[key_queue.head];
        key_queue.head = (key_queue.head + 1) % KEYBOARD_EVENT_BUFFER;

        return next_key;
}

/* handle_keycode:
 *      Handles key events calling from the kernel
 *      Temporary: Returns a string for their corresponding key
 *
 *      @param key  Keycode of the key that needs to be handled
 *
 *      @return A string message corresponds to its key
 */
const char *handle_keycode(keycode_t key)
{
        char *message;       
/*****************************************************
 *                                                   *
 *                                                   *
 *                                                   *
 *                    DELETE THIS                    *
 *                                                   *
 *                                                   *
 *                                                   *
 *****************************************************/
        

        switch (key) {
            /* Escape */
            case ESCAPE_PRESSED: message = "Escape pressed\n"; break;
            case ESCAPE_RELEASED: message = "Escape released\n"; break;

            /* Function keys */
            case F1_PRESSED: message = "F1 pressed\n"; break;
            case F1_RELEASED: message = "F1 released\n"; break;
            case F2_PRESSED: message = "F2 pressed\n"; break;
            case F2_RELEASED: message = "F2 released\n"; break;
            case F3_PRESSED: message = "F3 pressed\n"; break;
            case F3_RELEASED: message = "F3 released\n"; break;
            case F4_PRESSED: message = "F4 pressed\n"; break;
            case F4_RELEASED: message = "F4 released\n"; break;
            case F5_PRESSED: message = "F5 pressed\n"; break;
            case F5_RELEASED: message = "F5 released\n"; break;
            case F6_PRESSED: message = "F6 pressed\n"; break;
            case F6_RELEASED: message = "F6 released\n"; break;
            case F7_PRESSED: message = "F7 pressed\n"; break;
            case F7_RELEASED: message = "F7 released\n"; break;
            case F8_PRESSED: message = "F8 pressed\n"; break;
            case F8_RELEASED: message = "F8 released\n"; break;
            case F9_PRESSED: message = "F9 pressed\n"; break;
            case F9_RELEASED: message = "F9 released\n"; break;
            case F10_PRESSED: message = "F10 pressed\n"; break;
            case F10_RELEASED: message = "F10 released\n"; break;
            case F11_PRESSED: message = "F11 pressed\n"; break;
            case F11_RELEASED: message = "F11 released\n"; break;
            case F12_PRESSED: message = "F12 pressed\n"; break;
            case F12_RELEASED: message = "F12 released\n"; break;
                /* Number row */
            case BACK_TICK_PRESSED: message = "` pressed\n"; break;
            case BACK_TICK_RELEASED: message = "` released\n"; break;

            case ONE_PRESSED: message = "1 pressed\n"; break;
            case ONE_RELEASED: message = "1 released\n"; break;
            case TWO_PRESSED: message = "2 pressed\n"; break;
            case TWO_RELEASED: message = "2 released\n"; break;
            case THREE_PRESSED: message = "3 pressed\n"; break;
            case THREE_RELEASED: message = "3 released\n"; break;
            case FOUR_PRESSED: message = "4 pressed\n"; break;
            case FOUR_RELEASED: message = "4 released\n"; break;
            case FIVE_PRESSED: message = "5 pressed\n"; break;
            case FIVE_RELEASED: message = "5 released\n"; break;
            case SIX_PRESSED: message = "6 pressed\n"; break;
            case SIX_RELEASED: message = "6 released\n"; break;
            case SEVEN_PRESSED: message = "7 pressed\n"; break;
            case SEVEN_RELEASED: message = "7 released\n"; break;
            case EIGHT_PRESSED: message = "8 pressed\n"; break;
            case EIGHT_RELEASED: message = "8 released\n"; break;
            case NINE_PRESSED: message = "9 pressed\n"; break;
            case NINE_RELEASED: message = "9 released\n"; break;
            case ZERO_PRESSED: message = "0 pressed\n"; break;
            case ZERO_RELEASED: message = "0 released\n"; break;

            case MINUS_PRESSED: message = "- pressed\n"; break;
            case MINUS_RELEASED: message = "- released\n"; break;

            case EQUAL_PRESSED: message = "= pressed\n"; break;
            case EQUAL_RELEASED: message = "= released\n"; break;

            /* Editing keys */
            case BACKSPACE_PRESSED: message = "Backspace pressed\n"; break;
            case BACKSPACE_RELEASED: message = "Backspace released\n"; break;

            case TAB_PRESSED: message = "Tab pressed\n"; break;
            case TAB_RELEASED: message = "Tab released\n"; break;

            case CAPSLOCK_PRESSED: message = "Caps Lock pressed\n"; break;
            case CAPSLOCK_RELEASED: message = "Caps Lock released\n"; break;

            case ENTER_PRESSED: message = "Enter pressed\n"; break;
            case ENTER_RELEASED: message = "Enter released\n"; break;

            case SPACE_PRESSED: message = "Space pressed\n"; break;
            case SPACE_RELEASED: message = "Space released\n"; break;

            /* Modifiers */
            case LEFT_SHIFT_PRESSED: message = "Left Shift pressed\n"; break;
            case LEFT_SHIFT_RELEASED: message = "Left Shift released\n"; break;

            case RIGHT_SHIFT_PRESSED: message = "Right Shift pressed\n"; break;
            case RIGHT_SHIFT_RELEASED: message = "Right Shift released\n"; break;

            case LEFT_CTRL_PRESSED: message = "Left Ctrl pressed\n"; break;
            case LEFT_CTRL_RELEASED: message = "Left Ctrl released\n"; break;

            case RIGHT_CTRL_PRESSED: message = "Right Ctrl pressed\n"; break;
            case RIGHT_CTRL_RELEASED: message = "Right Ctrl released\n"; break;

            case LEFT_ALT_PRESSED: message = "Left Alt pressed\n"; break;
            case LEFT_ALT_RELEASED: message = "Left Alt released\n"; break;

            case RIGHT_ALT_PRESSED: message = "Right Alt pressed\n"; break;
            case RIGHT_ALT_RELEASED: message = "Right Alt released\n"; break;

            /* Alphabet */
            case A_PRESSED: message = "A pressed\n"; break;
            case A_RELEASED: message = "A released\n"; break;
            case B_PRESSED: message = "B pressed\n"; break;
            case B_RELEASED: message = "B released\n"; break;
            case C_PRESSED: message = "C pressed\n"; break;
            case C_RELEASED: message = "C released\n"; break;
            case D_PRESSED: message = "D pressed\n"; break;
            case D_RELEASED: message = "D released\n"; break;
            case E_PRESSED: message = "E pressed\n"; break;
            case E_RELEASED: message = "E released\n"; break;
            case F_PRESSED: message = "F pressed\n"; break;
            case F_RELEASED: message = "F released\n"; break;
            case G_PRESSED: message = "G pressed\n"; break;
            case G_RELEASED: message = "G released\n"; break;
            case H_PRESSED: message = "H pressed\n"; break;
            case H_RELEASED: message = "H released\n"; break;
            case I_PRESSED: message = "I pressed\n"; break;
            case I_RELEASED: message = "I released\n"; break;
            case J_PRESSED: message = "J pressed\n"; break;
            case J_RELEASED: message = "J released\n"; break;
            case K_PRESSED: message = "K pressed\n"; break;
            case K_RELEASED: message = "K released\n"; break;
            case L_PRESSED: message = "L pressed\n"; break;
            case L_RELEASED: message = "L released\n"; break;
            case M_PRESSED: message = "M pressed\n"; break;
            case M_RELEASED: message = "M released\n"; break;
            case N_PRESSED: message = "N pressed\n"; break;
            case N_RELEASED: message = "N released\n"; break;
            case O_PRESSED: message = "O pressed\n"; break;
            case O_RELEASED: message = "O released\n"; break;
            case P_PRESSED: message = "P pressed\n"; break;
            case P_RELEASED: message = "P released\n"; break;
            case Q_PRESSED: message = "Q pressed\n"; break;
            case Q_RELEASED: message = "Q released\n"; break;
            case R_PRESSED: message = "R pressed\n"; break;
            case R_RELEASED: message = "R released\n"; break;
            case S_PRESSED: message = "S pressed\n"; break;
            case S_RELEASED: message = "S released\n"; break;
            case T_PRESSED: message = "T pressed\n"; break;
            case T_RELEASED: message = "T released\n"; break;
            case U_PRESSED: message = "U pressed\n"; break;
            case U_RELEASED: message = "U released\n"; break;
            case V_PRESSED: message = "V pressed\n"; break;
            case V_RELEASED: message = "V released\n"; break;
            case W_PRESSED: message = "W pressed\n"; break;
            case W_RELEASED: message = "W released\n"; break;
            case X_PRESSED: message = "X pressed\n"; break;
            case X_RELEASED: message = "X released\n"; break;
            case Y_PRESSED: message = "Y pressed\n"; break;
            case Y_RELEASED: message = "Y released\n"; break;
            case Z_PRESSED: message = "Z pressed\n"; break;
            case Z_RELEASED: message = "Z released\n"; break;

            /* Punctuation */
            case L_SQ_BRACKET_PRESSED: message = "[ pressed\n"; break;
            case L_SQ_BRACKET_RELEASED: message = "[ released\n"; break;

            case R_SQ_BRACKET_PRESSED: message = "] pressed\n"; break;
            case R_SQ_BRACKET_RELEASED: message = "] released\n"; break;

            case BACKSLASH_PRESSED: message = "\\ pressed\n"; break;
            case BACKSLASH_RELEASED: message = "\\ released\n"; break;

            case SEMI_COLON_PRESSED: message = "; pressed\n"; break;
            case SEMI_COLON_RELEASED: message = "; released\n"; break;

            case SINGLE_QUOTE_PRESSED: message = "' pressed\n"; break;
            case SINGLE_QUOTE_RELEASED: message = "' released\n"; break;

            case COMMA_PRESSED: message = ", pressed\n"; break;
            case COMMA_RELEASED: message = ", released\n"; break;

            case PERIOD_PRESSED: message = ". pressed\n"; break;
            case PERIOD_RELEASED: message = ". released\n"; break;

            case FORWARD_SLASH_PRESSED: message = "/ pressed\n"; break;
            case FORWARD_SLASH_RELEASED: message = "/ released\n"; break;

            /* Remaining keys continue in Part 2... */

         /* Print Screen / Pause */
            case PRINT_SCREEN_PRESSED: message = "Print Screen pressed\n"; break;
            case PRINT_SCREEN_RELEASED: message = "Print Screen released\n"; break;

            case PAUSE_PRESSED: message = "Pause pressed\n"; break;
            case PAUSE_RELEASED: message = "Pause released\n"; break;

            /* Navigation cluster */
            case INSERT_PRESSED: message = "Insert pressed\n"; break;
            case INSERT_RELEASED: message = "Insert released\n"; break;

            case DELETE_PRESSED: message = "Delete pressed\n"; break;
            case DELETE_RELEASED: message = "Delete released\n"; break;

            case HOME_PRESSED: message = "Home pressed\n"; break;
            case HOME_RELEASED: message = "Home released\n"; break;

            case END_PRESSED: message = "End pressed\n"; break;
            case END_RELEASED: message = "End released\n"; break;

            case PAGE_UP_PRESSED: message = "Page Up pressed\n"; break;
            case PAGE_UP_RELEASED: message = "Page Up released\n"; break;

            case PAGE_DOWN_PRESSED: message = "Page Down pressed\n"; break;
            case PAGE_DOWN_RELEASED: message = "Page Down released\n"; break;


            default:
                message = "Unknown key\n";
                break;
        }

/*****************************************************
 *                                                   *
 *                                                   *
 *                                                   *
 *                    DELETE THIS                    *
 *                                                   *
 *                                                   *
 *                                                   *
 *****************************************************/


        return message;
}

/* keyboard_state:
 *      The state container when decoding scan codes
 */
typedef enum {
    NORMAL,
    EXTENDED,
    PRTSC_PRESSED,
    PRTSC_RELEASED,
    PAUSE
} keyboard_state_t;

static keyboard_state_t keyboard_state = NORMAL;

/* count:
 *      Tracking how many bytes of scan code is consumed
 */
static uint8_t sequence_index = 0;


/* scan code sequence for set 1 */
static const uint8_t set1_sequence_prtsc_p[] = {0xE0, 0x37};

static const uint8_t set1_sequence_prtsc_r[] = {0xE0, 0xAA};

static const uint8_t set1_sequence_pause[] = {0x1D, 0x45, 0xE1, 0x9D, 0xC5};

/* convert_scan_code_set1:
 *      Subroutine from the irq1 handler, converts scancodes
 *      into system defined keycodes for set1
 *
 *      @return  The keycode of the scan code
 *              returns UNKNOWN_KEY if the scan code is unmapped
 *              returns NO_KEY if the conversion is incomplete yet
 *
 */
keycode_t scan_code_set1_decode(uint8_t scan_code)
{
        switch (keyboard_state) {
                
                // Case for normal keys
                case NORMAL:
                        if (scan_code == 0xE1){
                                sequence_index = 0;
                                keyboard_state = PAUSE;
                        } else if (scan_code == 0xE0){
                                sequence_index = 0;
                                keyboard_state = EXTENDED;
                        } else {
                                if (scan_code <= 0x58)
                                        return set1_upper_pressed[scan_code];
                                else if (scan_code >= 0x81 && scan_code <= 0xD8)
                                        return set1_upper_released[scan_code - 0x81];
                                else
                                        return UNKNOWN_KEY;
                        }
                        break;

                // Case for extended keys
                case EXTENDED:
                        if (scan_code == 0x2A){
                                sequence_index = 0;
                                keyboard_state = PRTSC_PRESSED;
                        } else if (scan_code == 0xB7){
                                sequence_index = 0;
                                keyboard_state = PRTSC_RELEASED;
                        } else {
                                keyboard_state = NORMAL;
                                // Case for extended keys pressed
                                if (scan_code >= 0x10 && scan_code <= 0x6D)
                                        return set1_lower_pressed[scan_code - 0x10];
                                // Case for extended keys released
                                else if (scan_code >= 0x90 && scan_code <= 0xED)
                                        return set1_lower_released[scan_code - 0x90];
                                else
                                        return UNKNOWN_KEY;
                        }
                        break;

                // Case for PrintScreen pressed
                case PRTSC_PRESSED:
                        if (scan_code == set1_sequence_prtsc_p[sequence_index])
                                sequence_index++;
                        else{
                                sequence_index = 0;
                                keyboard_state = NORMAL;
                                return UNKNOWN_KEY;
                        }
                        if (sequence_index == sizeof(set1_sequence_prtsc_p)){
                                sequence_index = 0;
                                keyboard_state = NORMAL;
                                return PRINT_SCREEN_PRESSED;
                        }
                        break;
                
                // Case for PrintScreen released
                case PRTSC_RELEASED:
                        if (scan_code == set1_sequence_prtsc_r[sequence_index])
                                sequence_index++;
                        else{
                                sequence_index = 0;
                                keyboard_state = NORMAL;
                                return UNKNOWN_KEY;
                        }
                        if (sequence_index == sizeof(set1_sequence_prtsc_r)){
                                sequence_index = 0;
                                keyboard_state = NORMAL;
                                return PRINT_SCREEN_RELEASED;
                        }
                        break;

                // Case for Pause pressed
                case PAUSE:
                        if (scan_code == set1_sequence_pause[sequence_index])
                                sequence_index++;
                        else{
                                sequence_index = 0;
                                keyboard_state = NORMAL;
                                return UNKNOWN_KEY;
                        }
                        if (sequence_index == sizeof(set1_sequence_pause)){
                                sequence_index = 0;
                                keyboard_state = NORMAL;
                                return PAUSE_PRESSED;
                        }
                        break;
        }
        return NO_KEY;
}

/* convert_scan_code_set2:
 *      Subroutine from the irq1 handler, reads scancodes and
 *      converts them into system defined keycodes for set2
 *
 *      @return  The keycode of the scancode read
 */
keycode_t convert_scan_code_set2(void)
{
        uint8_t scan_code = read_scan_code();
        switch (scan_code) {
                
                // Case for Pause key
                case 0xE1:
                        scan_code = read_scan_code();
                        if (scan_code != 0x14)
                                return UNKNOWN_KEY;

                        scan_code = read_scan_code();
                        if (scan_code != 0x77)
                                return UNKNOWN_KEY;

                        scan_code = read_scan_code();
                        if (scan_code != 0xE1)
                                return UNKNOWN_KEY;

                        scan_code = read_scan_code();
                        if (scan_code != 0xF0)
                                return UNKNOWN_KEY;

                        scan_code = read_scan_code();
                        if (scan_code != 0x14)
                                return UNKNOWN_KEY;

                        scan_code = read_scan_code();
                        if (scan_code != 0xF0)
                                return UNKNOWN_KEY;

                        scan_code = read_scan_code();
                        if (scan_code != 0x77)
                                return UNKNOWN_KEY;

                        return PAUSE_PRESSED;
                
                // Case for extended keys
                case 0xE0:

                        scan_code = read_scan_code();

                        if (scan_code != 0xF0)
                                goto pressed;
                               
                        // Case for extended keys released
                        scan_code = read_scan_code();

                        // Case for PrintScreen released
                        if (scan_code == 0x7C){
                                scan_code = read_scan_code();
                                if (scan_code != 0xE0)
                                        return UNKNOWN_KEY;

                                scan_code = read_scan_code();
                                if (scan_code != 0xF0)
                                        return UNKNOWN_KEY;

                                scan_code = read_scan_code();
                                if (scan_code != 0x12)
                                        return UNKNOWN_KEY;

                                return PRINT_SCREEN_RELEASED;
                        }
                        // Case for extended keys released
                        if (scan_code >= 0x10 && scan_code <= 0x7D)
                                return set2_lower_released[scan_code - 0x10];

pressed:
                        // Case for PrintScreen pressed
                        if (scan_code == 0x12){
                                scan_code = read_scan_code();
                                if (scan_code != 0xE0)
                                        return UNKNOWN_KEY;

                                scan_code = read_scan_code();
                                if (scan_code != 0x7C)
                                        return UNKNOWN_KEY;

                                return PRINT_SCREEN_PRESSED;
                        }

                        // Case for extended keys pressed
                        if (scan_code >= 0x10 && scan_code <= 0x7D)
                                return set2_lower_pressed[scan_code - 0x10];
                        
                        return UNKNOWN_KEY;

                // Case for normal keys
                default:

                        // Case for normal keys released
                        if (scan_code == 0xF0){
                                scan_code = read_scan_code();
                                if (scan_code <= 0x83)
                                        return set2_upper_released[scan_code];
                        }

                        // Case for normal keys pressed
                        if (scan_code <= 0x83)
                                return set2_upper_pressed[scan_code];

                        return UNKNOWN_KEY;
        }
}

/* init_ps2_kbd:
 *      Initialises the ps2 keyboard settings
 */
void init_ps2_kbd(void)
{
        init_key_event_queue();
}

/* irq_1_keyboard_interrupt_handler:
 *      Interrupt handler for IRQ1 the keyboard interrupt from PIC
 *
 *      Offset 0x21 from the IDT
 */
void irq_1_keyboard_interrupt_handler(void)
{
        uint8_t scan_code = read_scan_code();
        keycode_t key = scan_code_set1_decode(scan_code);

        if (key != NO_KEY)
                enqueue_key_event(key);

}


