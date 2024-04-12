#ifndef MAIN_MESSAGES_H_
#define MAIN_MESSAGES_H_

// Messages sent to main task.
typedef enum {
    MAIN_MSG_BUTTON_PRESSED,
} main_msg_type_t;

typedef struct {
    main_msg_type_t type;
} main_msg_t;

// Messages sent to camera task.
typedef enum {
    CAM_MSG_START,
    CAM_MSG_STOP,
} cam_msg_type_t;

typedef struct {
    cam_msg_type_t type;
} cam_msg_t;

// Messages sent to led task.
typedef enum {
    LED_MSG_START,
} led_msg_type_t;

typedef struct {
    led_msg_type_t type;
} led_msg_t;;

#endif /* MAIN_MESSAGES_H_ */
