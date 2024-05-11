// Make sure you use the US international layout in every device.
// On Linux, use US, intl., with dead keys.
enum layout_names {
    XQC,
    XQC2,
    QWERTY,
    SYM,
    NUM,
    NAV,
    FUN,
    GAM,
    ACC,
    ACC2,
};

// Used to extract the basic tapping keycode from a dual-role key.
// Example: GET_TAP_KC(MT(MOD_RSFT, KC_E)) == KC_E
#define GET_TAP_KC(dual_role_key) (dual_role_key & 0xFF)
#define REPEAT LT(SYM, KC_NO)

#include QMK_KEYBOARD_H
// #include "keymap_us_international.h"
// #include "sendstring_us_international.h"
#include "features/caps_word.h"

// Macros definitions for accented characters.
// This is necessary because not every accented character is available with AltGr.
enum custom_keycodes {
    NH = SAFE_RANGE,
    HN,
    A_TILDE,
    O_TILDE,
    A_CIRC,
    E_CIRC,
    O_CIRC,
    A_GRV,
    A_ACUTE,
    E_ACUTE,
    I_ACUTE,
    O_ACUTE,
    U_ACUTE,
    C_ACUTE,
    CIRC,
    QUOT,
    DQUO,
    LH,
    TILDE,
    GRV,
};

#include "g/keymap_combo.h"

uint16_t last_keycode = KC_NO;
uint8_t last_modifier = 0;
uint16_t key_timer = 1000;
uint8_t mod_state;
uint8_t oneshot_mod_state;

void process_repeat_key(uint16_t keycode, const keyrecord_t *record) {
    if ((keycode != REPEAT) || (record->tap.count == 0)) {
        // Early return when holding down a pure layer key
        // to retain modifiers
        switch (keycode) {
            case QK_DEF_LAYER ... QK_DEF_LAYER_MAX:
            case QK_MOMENTARY ... QK_MOMENTARY_MAX:
            case QK_LAYER_MOD ... QK_LAYER_MOD_MAX:
            case QK_ONE_SHOT_LAYER ... QK_ONE_SHOT_LAYER_MAX:
            case QK_TOGGLE_LAYER ... QK_TOGGLE_LAYER_MAX:
            case QK_TO ... QK_TO_MAX:
            case QK_LAYER_TAP_TOGGLE ... QK_LAYER_TAP_TOGGLE_MAX:
                return;
        }
        if (record->event.pressed) {
            key_timer = timer_read();
        }
        last_modifier = oneshot_mod_state > mod_state ? oneshot_mod_state : mod_state;
        switch (keycode) {
            case QK_LAYER_TAP ... QK_LAYER_TAP_MAX:
            case QK_MOD_TAP ... QK_MOD_TAP_MAX:
                if (record->event.pressed) {
                    last_keycode = GET_TAP_KC(keycode);
                }
                break;
            default:
                if (record->event.pressed) {
                    last_keycode = keycode;
                }
                break;
        }
    } else { // keycode == REPEAT
	if(((last_keycode < KC_A || last_keycode > KC_Z) && (last_keycode != NH || last_keycode != HN)) || timer_elapsed(key_timer) > 500) {
	    add_oneshot_mods(MOD_BIT(KC_LSFT));
        } else {
		if (record->event.pressed) {
		    register_mods(last_modifier);
		    register_code16(last_keycode);
		} else {
		    unregister_code16(last_keycode);
		    unregister_mods(last_modifier);
		}

        }
    }
}


// Enable caps word idle timeout
void matrix_scan_user(void) {
  caps_word_task();
}

#define PRENASAL(code) ((KC_A <= (code)) && ((code) <= KC_Z) && (code != KC_T) && (code != KC_S) && (code != KC_C) && (code != KC_P) && (code != KC_G) && (code != KC_W) && (code != KC_X) && (code != KC_L) && (code != KC_N) && (code != KC_H))

uint16_t predecessor_key = KC_NO;
uint16_t adaptive_hn_key = KC_NO;
uint16_t adaptive_nh_key = KC_NO;

static uint16_t prenasal_proximity;
static uint16_t mod_timer;
bool hn_mod = false;

bool prenasal(void) {
	return (predecessor_key == KC_A) && (timer_elapsed(prenasal_proximity) < MIN_PROXIMITY) && !(get_mods() & ~MOD_MASK_SHIFT & MOD_MASK_CAG);
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // Process caps word
    if (!process_caps_word(keycode, record)) { return false; }

    process_repeat_key(keycode, record);
    if ((keycode == REPEAT) && (record->tap.count != 0)) {
	return false;
    }
    // It's important to update the mod variables *after* calling process_repeat_key, or else
    // only a single modifier from the previous key is repeated (e.g. Ctrl+Shift+T then Repeat produces Shift+T)

    mod_state = get_mods();
    oneshot_mod_state = get_oneshot_mods();

    switch (keycode) {
        case A_TILDE:
            if (record->event.pressed) {
                if ((mod_state || oneshot_mod_state) && MOD_MASK_SHIFT) {
                    clear_mods();
                    clear_oneshot_mods();
                    register_code(KC_LSFT);
                    tap_code(KC_GRV);
                    tap_code(KC_A);
                    unregister_code(KC_LSFT);
                    set_mods(mod_state);
                } else {
                    clear_mods();
                    register_code(KC_LSFT);
                    tap_code(KC_GRV);
                    unregister_code(KC_LSFT);
                    tap_code(KC_A);
                    set_mods(mod_state);
                }
            }
            break;
        case O_TILDE:
            if (record->event.pressed) {
                if ((mod_state || oneshot_mod_state) && MOD_MASK_SHIFT) {
                    clear_mods();
                    clear_oneshot_mods();
                    register_code(KC_LSFT);
                    tap_code(KC_GRV);
                    tap_code(KC_O);
                    unregister_code(KC_LSFT);
                    set_mods(mod_state);
                } else {
                    clear_mods();
                    register_code(KC_LSFT);
                    tap_code(KC_GRV);
                    unregister_code(KC_LSFT);
                    tap_code(KC_O);
                    set_mods(mod_state);
                }
            }
            break;
        case A_CIRC:
            if (record->event.pressed) {
                if ((mod_state || oneshot_mod_state) && MOD_MASK_SHIFT) {
                    clear_mods();
                    clear_oneshot_mods();
                    register_code(KC_LSFT);
                    tap_code(KC_6);
                    tap_code(KC_A);
                    unregister_code(KC_LSFT);
                    set_mods(mod_state);
                } else {
                    clear_mods();
                    register_code(KC_LSFT);
                    tap_code(KC_6);
                    unregister_code(KC_LSFT);
                    tap_code(KC_A);
                    set_mods(mod_state);
                }
            }
            break;
        case E_CIRC:
            if (record->event.pressed) {
                if ((mod_state || oneshot_mod_state) && MOD_MASK_SHIFT) {
                    clear_mods();
                    clear_oneshot_mods();
                    register_code(KC_LSFT);
                    tap_code(KC_6);
                    tap_code(KC_E);
                    unregister_code(KC_LSFT);
                    set_mods(mod_state);
                } else {
                    clear_mods();
                    register_code(KC_LSFT);
                    tap_code(KC_6);
                    unregister_code(KC_LSFT);
                    tap_code(KC_E);
                    set_mods(mod_state);
                }
            }
            break;
        case O_CIRC:
            if (record->event.pressed) {
                if ((mod_state || oneshot_mod_state) && MOD_MASK_SHIFT) {
                    clear_mods();
                    clear_oneshot_mods();
                    register_code(KC_LSFT);
                    tap_code(KC_6);
                    tap_code(KC_O);
                    unregister_code(KC_LSFT);
                    set_mods(mod_state);
                } else {
                    clear_mods();
                    register_code(KC_LSFT);
                    tap_code(KC_6);
                    unregister_code(KC_LSFT);
                    tap_code(KC_O);
                    set_mods(mod_state);
                }
            }
            break;
        case A_GRV:
            if (record->event.pressed) {
                if ((mod_state || oneshot_mod_state) && MOD_MASK_SHIFT) {
                    clear_mods();
                    clear_oneshot_mods();
                    tap_code(KC_GRV);
                    register_code(KC_LSFT);
                    tap_code(KC_A);
                    unregister_code(KC_LSFT);
                    set_mods(mod_state);
                } else {
                    clear_mods();
                    tap_code(KC_GRV);
                    tap_code(KC_A);
                    set_mods(mod_state);
                }
            }
            break;
        case CIRC:
            if (record->event.pressed) {
                clear_mods();
                register_code(KC_LSFT);
                tap_code(KC_6);
                unregister_code(KC_LSFT);
                tap_code(KC_SPC);
                set_mods(mod_state);
            }
            break;
        case QUOT:
            if (record->event.pressed) {
                tap_code(KC_QUOT);
                tap_code(KC_SPC);
            }
            break;
        case DQUO:
            if (record->event.pressed) {
                clear_mods();
                register_code(KC_LSFT);
                tap_code(KC_QUOT);
                unregister_code(KC_LSFT);
                tap_code(KC_SPC);
                set_mods(mod_state);
            }
            break;
        case A_ACUTE:
            if (record->event.pressed) {
                if ((mod_state || oneshot_mod_state) && MOD_MASK_SHIFT) {
                    clear_mods();
                    clear_oneshot_mods();
                    tap_code(KC_QUOT);
                    register_code(KC_LSFT);
                    tap_code(KC_A);
                    unregister_code(KC_LSFT);
                    set_mods(mod_state);
                } else {
                    clear_mods();
                    tap_code(KC_QUOT);
                    tap_code(KC_A);
                    set_mods(mod_state);
                }
            }
            break;
        case E_ACUTE:
            if (record->event.pressed) {
                if ((mod_state || oneshot_mod_state) && MOD_MASK_SHIFT) {
                    clear_mods();
                    clear_oneshot_mods();
                    tap_code(KC_QUOT);
                    register_code(KC_LSFT);
                    tap_code(KC_E);
                    unregister_code(KC_LSFT);
                    set_mods(mod_state);
                } else {
                    clear_mods();
                    tap_code(KC_QUOT);
                    tap_code(KC_E);
                    set_mods(mod_state);
                }
            }
            break;
        case I_ACUTE:
            if (record->event.pressed) {
                if ((mod_state || oneshot_mod_state) && MOD_MASK_SHIFT) {
                    clear_mods();
                    clear_oneshot_mods();
                    tap_code(KC_QUOT);
                    register_code(KC_LSFT);
                    tap_code(KC_I);
                    unregister_code(KC_LSFT);
                    set_mods(mod_state);
                } else {
                    clear_mods();
                    tap_code(KC_QUOT);
                    tap_code(KC_I);
                    set_mods(mod_state);
                }
            }
            break;
        case O_ACUTE:
            if (record->event.pressed) {
                if ((mod_state || oneshot_mod_state) && MOD_MASK_SHIFT) {
                    clear_mods();
                    clear_oneshot_mods();
                    tap_code(KC_QUOT);
                    register_code(KC_LSFT);
                    tap_code(KC_O);
                    unregister_code(KC_LSFT);
                    set_mods(mod_state);
                } else {
                    clear_mods();
                    tap_code(KC_QUOT);
                    tap_code(KC_O);
                    set_mods(mod_state);
                }
            }
            break;
        case U_ACUTE:
            if (record->event.pressed) {
                if ((mod_state || oneshot_mod_state) && MOD_MASK_SHIFT) {
                    clear_mods();
                    clear_oneshot_mods();
                    tap_code(KC_QUOT);
                    register_code(KC_LSFT);
                    tap_code(KC_U);
                    unregister_code(KC_LSFT);
                    set_mods(mod_state);
                } else {
                    clear_mods();
                    tap_code(KC_QUOT);
                    tap_code(KC_U);
                    set_mods(mod_state);
                }
            }
            break;
        case C_ACUTE:
            if (record->event.pressed) {
                register_code(KC_RALT);
                tap_code(KC_COMM);
                unregister_code(KC_RALT);
            }
            break;
        case LH:
	    if (record->event.pressed) {
		send_string("lh");
	    }
	    break;
	case TILDE:
	    if (record->event.pressed) {
		send_string("~ ");
	    }
	    break;
	case GRV:
	    if (record->event.pressed) {
		send_string("` ");
	    }
	    break;
	case HN:
	    if (record->event.pressed) {
	        mod_timer = timer_read();
		keycode = prenasal()?KC_N:KC_H;	
		adaptive_hn_key = keycode;
		hn_mod = true;
            }
	    else {
		if (hn_mod) {
			if (timer_elapsed(mod_timer) < TAPPING_TERM) {
			    tap_code(adaptive_hn_key);
			}
			else {
			    unregister_mods(MOD_BIT(KC_LGUI));
			}
			hn_mod = false;
	        }
	    }
	    break;
	case NH:
            if (record->event.pressed) {
		keycode = prenasal()?KC_H:KC_N;	
		adaptive_nh_key = keycode;
		register_code(adaptive_nh_key);
	    }
	    else {
		unregister_code(adaptive_nh_key);
	    }
	    break;
        }
	if (!KEYCODE_IS_MOD(keycode) && record->event.pressed) {
		if (keycode != adaptive_hn_key) {
			if (hn_mod && timer_elapsed(mod_timer) >= TAPPING_TERM) {
			    register_mods(MOD_BIT(KC_LGUI));
			}
			if (hn_mod && timer_elapsed(mod_timer) < TAPPING_TERM) {
			    tap_code(adaptive_hn_key);
			    hn_mod = false;
			}
		}
		if(PRENASAL(GET_TAP_KC(keycode)) || (keycode >= A_TILDE && keycode <= U_ACUTE)) {
		        predecessor_key = KC_A;
			prenasal_proximity = timer_read();
		}
		else {
			predecessor_key = keycode;
		}
	}
    return true;
}

// Combos shouldn't trigger on the gaming layer.
bool combo_should_trigger(uint16_t combo_index, combo_t *combo, uint16_t keycode, keyrecord_t *record) {
    if (layer_state_is(GAM)) {
	return false;
    }
    return true;
}

// Enable permissive hold only for some specific keys that are lesser prone to accidental activation
bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
//        case LSFT_T(KC_S):
//            // Immediately select the hold action when another key is tapped.
//            return true;
//        case RSFT_T(KC_E):
//            return true;
//        case LCTL_T(KC_Z):
//            return true;
//        case LCTL_T(KC_Q):
//            return true;
        case LT(NUM, KC_SPC):
            return true;
        case LT(NAV, KC_TAB):
            return true;
        case REPEAT:
            return true;
//        case LT(SYM, KC_E):
//            return true;
        default:
            // Do not select the hold action when another key is tapped.
            return false;
    }
}

/*
Base alphas - Canary mod (QLIMBY)
Base alphas - Canary
Base alphas - QWERTY
Symbols and access to other layers
Numbers and additional symbols
Navigation layer (arrow keys and mouse control)
Function keys
Gaming layer (Overwatch)
Accented characters
*/
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[XQC] = LAYOUT(
		LCTL_T(KC_B), KC_L, KC_C, KC_M, LT(FUN, KC_F),				LT(FUN, KC_J), KC_Q, KC_U, KC_K, KC_BSPC,
		HN, LALT_T(KC_R), LSFT_T(KC_S), LCTL_T(KC_T), KC_G,		KC_P, RCTL_T(KC_I), RSFT_T(KC_A), LALT_T(KC_E), RGUI_T(KC_O),
	       	QUOT, RALT_T(KC_Z), KC_V, KC_D, KC_X,				KC_Y, NH, KC_W, RALT_T(KC_COMM), LT(NUM, KC_DOT),
	       				LT(NAV, KC_TAB), LT(NUM, KC_SPC),			REPEAT, OSL(ACC)
	),
	[XQC2] = LAYOUT(
		KC_B, KC_L, KC_C, KC_M, LT(FUN, KC_F),				LT(FUN, KC_J), KC_Q, KC_U, KC_K, KC_BSPC,
		HN, KC_R, KC_S, KC_T, KC_G,		KC_P, KC_I, KC_A, KC_E, KC_O,
	       	QUOT, KC_Z, KC_V, KC_D, KC_X,				KC_Y, NH, KC_W, KC_COMM, KC_DOT,
	       				LT(NAV, KC_TAB), LT(NUM, KC_SPC),			REPEAT, OSL(ACC)
	),
//	[QLIMBY] = LAYOUT(
//		QUOT, KC_L, KC_M, KC_P, LT(FUN, KC_Q),				LT(FUN, KC_Z), KC_F, KC_O, KC_U, KC_BSPC,
//		LGUI_T(KC_C), LALT_T(KC_R), LSFT_T(KC_S), LCTL_T(KC_T), KC_W,		KC_Y, RCTL_T(KC_N), RSFT_T(KC_E), LALT_T(KC_I), RGUI_T(KC_H),
//	       	LCTL_T(KC_J), RALT_T(KC_X), KC_G, KC_D, KC_K,				KC_SLSH, KC_B, KC_COMM, RALT_T(KC_DOT), LT(NUM, KC_V),
//	       				LT(NAV, KC_TAB), LT(NUM, KC_SPC),			LT(SYM, KC_A), OSL(ACC)
//	),
//	[COLEMAK] = LAYOUT(
//		KC_Q, KC_W, KC_F, KC_P, KC_B,						KC_J, KC_L, KC_U, KC_Y, KC_BSPC,
//		LGUI_T(KC_A), LALT_T(KC_R), LSFT_T(KC_S), LCTL_T(KC_T), KC_G,		KC_M, RCTL_T(KC_N), RSFT_T(KC_E), LALT_T(KC_I), RGUI_T(KC_O),
//	       	LCTL_T(KC_Z), RALT_T(KC_X), KC_C, KC_D, KC_V,				KC_K, KC_H, KC_COMM, RALT_T(KC_DOT), LCTL_T(KC_SLSH),
//	       				LT(NAV, KC_TAB), LT(NUM, KC_SPC),			REPEAT, OSL(ACC)
//	),
	[QWERTY] = LAYOUT(
		KC_Q, KC_W, KC_E, KC_R, KC_T,						KC_Y, KC_U, KC_I, KC_O, KC_P,
		LGUI_T(KC_A), LALT_T(KC_S), LSFT_T(KC_D), LCTL_T(KC_F), KC_G,		KC_H, RCTL_T(KC_J), RSFT_T(KC_K), LALT_T(KC_L), RGUI_T(KC_SCLN),
	       	LCTL_T(KC_Z), RALT_T(KC_X), KC_C, KC_V, KC_B,				KC_N, KC_M, KC_COMM, RALT_T(KC_DOT), LCTL_T(KC_SLSH),
	       				LT(NAV, KC_TAB), LT(NUM, KC_SPC),			REPEAT, OSL(ACC)
	),
	[SYM] = LAYOUT(
		KC_ESC, KC_EXLM, KC_AT, KC_HASH, KC_DLR,		KC_PSCR, KC_LCBR, KC_RCBR, QUOT, KC_DEL,
		KC_TAB, KC_MINS, KC_PLUS, KC_EQL, KC_SCLN,		DQUO, KC_LPRN, KC_RPRN, KC_COLN, KC_ENT,
		TG(NUM), KC_GRV, KC_ASTR, KC_QUOT, KC_LGUI,		KC_BSLS, KC_LBRC, KC_RBRC, TG(GAM), TG(NAV),
					KC_LSFT, KC_SPC,		KC_TRNS, KC_LALT
	),
	[NUM] = LAYOUT(
		KC_ESC, KC_PERC, KC_CIRC, KC_AMPR, KC_LCBR, 			KC_UNDS, KC_7, KC_8, KC_9, KC_BSPC,
		KC_TAB, KC_KP_MINUS, KC_KP_PLUS, KC_EQL, KC_RCBR,		KC_PDOT, KC_4, KC_5, KC_6, KC_PENT,
		KC_COLN, KC_SLSH, KC_ASTR, TILDE, GRV,				KC_COMM, KC_1, KC_2, KC_3, TG(NAV),
					OSM(MOD_LSFT), TG(NUM),			KC_0, KC_SPC
	),
	[NAV] = LAYOUT(
		KC_ESC, KC_WH_L, KC_MS_U, KC_WH_R, KC_BTN3,		KC_HOME, LCTL(LSFT(KC_TAB)), LCTL(KC_TAB), KC_END, LCTL(KC_W),
		KC_TAB, KC_MS_L, KC_MS_D, KC_MS_R, KC_PGUP,		KC_LEFT, KC_DOWN, KC_UP, KC_RGHT, KC_ENT,
		TG(FUN), KC_BTN2, KC_WH_U, KC_WH_D, KC_PGDN,		KC_BTN1, KC_ACL0, KC_ACL1, KC_ACL2, KC_NO,
					KC_TRNS, TG(NAV),		KC_LCTL, KC_LSFT
	),
	[FUN] = LAYOUT(
		KC_ESC, DM_REC1, DM_PLY1, DF(XQC), KC_MUTE,			KC_SCRL, KC_F7, KC_F8, KC_F9, KC_F10,
		KC_CAPS, KC_LALT, DF(XQC2), DF(QWERTY), KC_VOLU,		KC_PAUS, KC_F4, KC_F5, KC_F6, KC_F11,
		KC_TRNS, KC_MPRV, KC_MNXT, KC_MPLY, KC_VOLD,			KC_INS, KC_F1, KC_F2, KC_F3, KC_F12,
					KC_TRNS, TG(FUN),			KC_TRNS, KC_TRNS
	),
	[GAM] = LAYOUT(
		KC_TAB, KC_Q, KC_W, KC_E, KC_R,		KC_Y, KC_U, KC_I, KC_O, KC_P,
		KC_LSFT, KC_A, KC_S, KC_D, KC_F,	KC_H, KC_J, KC_K, KC_L, KC_SCLN,
		KC_LCTL, KC_Z, KC_X, KC_C, KC_V,	KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH,
				KC_M, KC_SPC,		TG(GAM), KC_ESC
	),
//	[ACC] = LAYOUT(
//		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,		KC_TRNS, KC_TRNS, U_ACUTE, KC_TRNS, LCTL(KC_BSPC),
//		A_ACUTE, A_GRV, O_CIRC, A_CIRC, KC_TRNS,		KC_TRNS, KC_TRNS, E_ACUTE, I_ACUTE, O_ACUTE,
//		KC_TRNS, E_CIRC, C_ACUTE, A_TILDE, O_TILDE,		KC_TRNS, KC_TRNS, RALT(KC_COMM), KC_RALT, RALT(KC_SLSH),
//						KC_TRNS, KC_TRNS,		KC_TRNS, KC_TRNS
//	),
	[ACC] = LAYOUT(
		KC_TRNS, A_TILDE, C_ACUTE, O_TILDE, KC_TRNS,		KC_TRNS, KC_TRNS, U_ACUTE, KC_TRNS, LCTL(KC_BSPC),
		RALT(KC_N), E_CIRC, A_CIRC, A_GRV, KC_TRNS,		KC_TRNS, I_ACUTE, A_ACUTE, E_ACUTE, O_ACUTE,
		KC_TRNS, KC_TRNS, O_CIRC, KC_TRNS, KC_TRNS,		KC_TRNS, KC_TRNS, RALT(KC_COMM), KC_RALT, RALT(KC_SLSH),
						KC_TRNS, KC_TRNS,		KC_TRNS, KC_TRNS
	),
	[ACC2] = LAYOUT(
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,		KC_TRNS, KC_TRNS, O_ACUTE, U_ACUTE, LCTL(KC_BSPC),
		C_ACUTE, A_TILDE, O_TILDE, A_CIRC, KC_TRNS,		KC_TRNS, KC_TRNS, E_ACUTE, I_ACUTE, A_ACUTE,
		KC_TRNS, E_CIRC, O_CIRC, A_GRV, KC_TRNS,		KC_TRNS, KC_TRNS, RALT(KC_COMM), KC_RALT, RALT(KC_SLSH),
						KC_TRNS, KC_TRNS,		KC_TRNS, KC_TRNS
	),
};
