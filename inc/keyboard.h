#ifndef INCLUDE_KEYBOARD_H
#define INCLUDE_KEYBOARD_H


#define KBD_DATA_PORT           0x60
#define KBD_STATUS_PORT         0x64


/* Keycodes */

/* Section for pressed keys */
typedef enum {
        NO_KEY = 0,
        ESCAPE_PRESSED,
        F1_PRESSED,
        F2_PRESSED,
        F3_PRESSED,
        F4_PRESSED,
        F5_PRESSED,
        F6_PRESSED,
        F7_PRESSED,
        F8_PRESSED,
        F9_PRESSED,
        F10_PRESSED,
        F11_PRESSED,
        F12_PRESSED,
        BACK_TICK_PRESSED,
        ONE_PRESSED,
        TWO_PRESSED,
        THREE_PRESSED,
        FOUR_PRESSED,
        FIVE_PRESSED,
        SIX_PRESSED,
        SEVEN_PRESSED,
        EIGHT_PRESSED,
        NINE_PRESSED,
        ZERO_PRESSED,
        MINUS_PRESSED,
        EQUAL_PRESSED,
        BACKSPACE_PRESSED,
        TAB_PRESSED,
        Q_PRESSED,
        W_PRESSED,
        E_PRESSED,
        R_PRESSED,
        T_PRESSED,
        Y_PRESSED,
        U_PRESSED,
        I_PRESSED,
        O_PRESSED,
        P_PRESSED,
        L_SQ_BRACKET_PRESSED,          // Left square bracket pressed
        R_SQ_BRACKET_PRESSED,          // Right square bracket pressed
        BACKSLASH_PRESSED,
        CAPSLOCK_PRESSED,
        A_PRESSED,
        S_PRESSED,
        D_PRESSED,
        F_PRESSED,
        G_PRESSED,
        H_PRESSED,
        J_PRESSED,
        K_PRESSED,
        L_PRESSED,
        SEMI_COLON_PRESSED,
        SINGLE_QUOTE_PRESSED,
        ENTER_PRESSED,
        LEFT_SHIFT_PRESSED,
        Z_PRESSED,
        X_PRESSED,
        C_PRESSED,
        V_PRESSED,
        B_PRESSED,
        N_PRESSED,
        M_PRESSED,
        COMMA_PRESSED,
        PERIOD_PRESSED,
        FORWARD_SLASH_PRESSED,
        RIGHT_SHIFT_PRESSED,
        LEFT_CTRL_PRESSED,
        LEFT_ALT_PRESSED,
        SPACE_PRESSED,
        RIGHT_ALT_PRESSED,
        RIGHT_CTRL_PRESSED,
        KEYPAD_ONE_PRESSED,
        KEYPAD_TWO_PRESSED,
        KEYPAD_THREE_PRESSED,
        KEYPAD_FOUR_PRESSED,
        KEYPAD_FIVE_PRESSED,
        KEYPAD_SIX_PRESSED,
        KEYPAD_SEVEN_PRESSED,
        KEYPAD_EIGHT_PRESSED,
        KEYPAD_NINE_PRESSED,
        KEYPAD_ZERO_PRESSED,
        KEYPAD_ASTERISK_PRESSED,
        KEYPAD_MINUS_PRESSED,
        KEYPAD_PLUS_PRESSED,
        KEYPAD_PERIOD_PRESSED,
        KEYPAD_ENTER_PRESSED,
        KEYPAD_FORWARD_SLASH_PRESSED,
        NUMBERLOCK_PRESSED,
        SCROLLLOCK_PRESSED,
        UP_PRESSED,
        LEFT_PRESSED,
        RIGHT_PRESSED,
        DOWN_PRESSED,
        HOME_PRESSED,
        END_PRESSED,
        PAGE_UP_PRESSED,
        PAGE_DOWN_PRESSED,
        INSERT_PRESSED,
        DELETE_PRESSED,
        APPS_PRESSED,
        /*Windows/Command/Super/Meta key*/
        LEFT_GUI_PRESSED,
        RIGHT_GUI_PRESSED,
        /*Multimedia Keys*/
        PREVIOUS_TRACK_PRESSED,
        NEXT_TRACK_PRESSED,
        MUTE_PRESSED,
        CALCULATOR_PRESSED,
        ISO_EXTRA_PRESSED,
        STOP_PRESSED,
        VOLUME_DOWN_PRESSED,
        VOLUME_UP_PRESSED,
        PLAY_PAUSE_PRESSED,
        MY_COMPUTER_PRESSED,
        EMAIL_PRESSED,
        MEDIA_SELECT_PRESSED,
        WWW_HOME_PRESSED,
        WWW_SEARCH_PRESSED,
        WWW_FAVOURITES_PRESSED,
        WWW_REFRESH_PRESSED,
        WWW_STOP_PRESSED,
        WWW_FORWARD_PRESSED,
        WWW_BACK_PRESSED,
        /*ACPI*/
        POWER_PRESSED,
        SLEEP_PRESSED,
        WAKE_PRESSED,

        PRINT_SCREEN_PRESSED,
        PAUSE_PRESSED,


        /*Section for released keys*/
        ESCAPE_RELEASED,
        F1_RELEASED,
        F2_RELEASED,
        F3_RELEASED,
        F4_RELEASED,
        F5_RELEASED,
        F6_RELEASED,
        F7_RELEASED,
        F8_RELEASED,
        F9_RELEASED,
        F10_RELEASED,
        F11_RELEASED,
        F12_RELEASED,
        BACK_TICK_RELEASED,
        ONE_RELEASED,
        TWO_RELEASED,
        THREE_RELEASED,
        FOUR_RELEASED,
        FIVE_RELEASED,
        SIX_RELEASED,
        SEVEN_RELEASED,
        EIGHT_RELEASED,
        NINE_RELEASED,
        ZERO_RELEASED,
        MINUS_RELEASED,
        EQUAL_RELEASED,
        BACKSPACE_RELEASED,
        TAB_RELEASED,
        Q_RELEASED,
        W_RELEASED,
        E_RELEASED,
        R_RELEASED,
        T_RELEASED,
        Y_RELEASED,
        U_RELEASED,
        I_RELEASED,
        O_RELEASED,
        P_RELEASED,
        L_SQ_BRACKET_RELEASED,           //Left square bracket released
        R_SQ_BRACKET_RELEASED,           //Right square bracket released
        BACKSLASH_RELEASED,
        CAPSLOCK_RELEASED,
        A_RELEASED,
        S_RELEASED,
        D_RELEASED,
        F_RELEASED,
        G_RELEASED,
        H_RELEASED,
        J_RELEASED,
        K_RELEASED,
        L_RELEASED,
        SEMI_COLON_RELEASED,
        SINGLE_QUOTE_RELEASED,
        ENTER_RELEASED,
        LEFT_SHIFT_RELEASED,
        Z_RELEASED,
        X_RELEASED,
        C_RELEASED,
        V_RELEASED,
        B_RELEASED,
        N_RELEASED,
        M_RELEASED,
        COMMA_RELEASED,
        PERIOD_RELEASED,
        FORWARD_SLASH_RELEASED,
        RIGHT_SHIFT_RELEASED,
        LEFT_CTRL_RELEASED,
        LEFT_ALT_RELEASED,
        SPACE_RELEASED,
        RIGHT_ALT_RELEASED,
        RIGHT_CTRL_RELEASED,
        KEYPAD_ONE_RELEASED,
        KEYPAD_TWO_RELEASED,
        KEYPAD_THREE_RELEASED,
        KEYPAD_FOUR_RELEASED,
        KEYPAD_FIVE_RELEASED,
        KEYPAD_SIX_RELEASED,
        KEYPAD_SEVEN_RELEASED,
        KEYPAD_EIGHT_RELEASED,
        KEYPAD_NINE_RELEASED,
        KEYPAD_ZERO_RELEASED,
        KEYPAD_ASTERISK_RELEASED,
        KEYPAD_MINUS_RELEASED,
        KEYPAD_PLUS_RELEASED,
        KEYPAD_PERIOD_RELEASED,
        KEYPAD_ENTER_RELEASED,
        KEYPAD_FORWARD_SLASH_RELEASED,
        NUMBERLOCK_RELEASED,
        SCROLLLOCK_RELEASED,
        UP_RELEASED,
        LEFT_RELEASED,
        RIGHT_RELEASED,
        DOWN_RELEASED,
        HOME_RELEASED,
        END_RELEASED,
        PAGE_UP_RELEASED,
        PAGE_DOWN_RELEASED,
        INSERT_RELEASED,
        DELETE_RELEASED,
        APPS_RELEASED,
        /*Windows/Command/Super/Meta key*/
        LEFT_GUI_RELEASED,
        RIGHT_GUI_RELEASED,
        /*Multimedia Keys*/
        PREVIOUS_TRACK_RELEASED,
        NEXT_TRACK_RELEASED,
        MUTE_RELEASED,
        CALCULATOR_RELEASED,
        ISO_EXTRA_RELEASED,
        STOP_RELEASED,
        VOLUME_DOWN_RELEASED,
        VOLUME_UP_RELEASED,
        PLAY_PAUSE_RELEASED,
        MY_COMPUTER_RELEASED,
        EMAIL_RELEASED,
        MEDIA_SELECT_RELEASED,
        WWW_HOME_RELEASED,
        WWW_SEARCH_RELEASED,
        WWW_FAVOURITES_RELEASED,
        WWW_REFRESH_RELEASED,
        WWW_STOP_RELEASED,
        WWW_FORWARD_RELEASED,
        WWW_BACK_RELEASED,
        /*ACPI*/
        POWER_RELEASED,
        SLEEP_RELEASED,
        WAKE_RELEASED,

        PRINT_SCREEN_RELEASED,
        PAUSE_RELEASED,

        UNKNOWN_KEY = 255
} keycode_t;

/* init_ps2_kbd:
 *      Initialises the ps2 keyboard settings
 */
void init_ps2_kbd(void);


/* key_queue_get_event:
 *      Returns the next key event to be handled from the queue, returns 0 if empty
 *      !!!!! This should be the only key queue function that the kernal space can call
 *      Every other key queue function should remain private to this file as the kernel should not
 *      be modifying the queue directly
 *
 *      @return  The keycode of the next event in the queue
 *               Returns 0 if the queue is empty so nothing needs to be handled
 */
keycode_t key_queue_get_event(void);
        
/* handle_keycode:
 *      Handles key events calling from the kernel
 *      Temporary: Returns a string for their corresponding key
 *
 *      @param key  Keycode of the key that needs to be handled
 *
 *      @return A string message corresponds to its key
 */
const char *handle_keycode(keycode_t key);

/* irq_1_keyboard_interrupt_handler:
 *      Interrupt handler for IRQ1 the keyboard interrupt from PIC
 *
 *      Offset 0x21 from the IDT
 */
void irq_1_keyboard_interrupt_handler(void);

#endif /* INCLUDE_KEYBOARD_H */
