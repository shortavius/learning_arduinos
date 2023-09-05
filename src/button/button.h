#if !defined(BUTTON_H)
#define BUTTON_H

typedef enum
{
    BUTTON_STATE_OPEN,
    BUTTON_STATE_CLOSED,
    BUTTON_STATE_MAX
} button_states;

void button_switch_cfg(void);
button_states button_state_get_now(int button);
button_states button_state_get(int button);
void button_action(void);

#endif // BUTTON_H
