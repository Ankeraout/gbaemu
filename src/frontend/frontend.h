#ifndef __FRONTEND_FRONTEND_H__
#define __FRONTEND_FRONTEND_H__

#include <stdint.h>

typedef void frontend_keypadHandler_t(void *parameter, bool *keys);

/**
 * @brief Closes the front-end.
 */
extern void frontend_close();

/**
 * @brief This function is called by the core whenever a new frame is ready to
 *        be displayed.
 * 
 * @param[in] buffer The buffer that contains the pixel data of the frame to be
 *                   displayed. This buffer is an array of pixels. Each pixel
 *                   is represented by an unsigned 32-bit integer.
 */
extern void frontend_frame(const uint32_t *buffer);

/**
 * @brief Initializes the front-end.
 * 
 * @returns An integer value that indicates whether the front-end was
 *          initialized correctly or not.
 * @retval 0 if the front-end was initialized correctly.
 * @retval Any other value if an error occurred.
 */
extern int frontend_init();

/**
 * @brief
 * 
 * @param[in] callback The function to call when a key is pressed or released.
 * @param[in] callbackParameter The parameter that will be passed to the
 *                              function.
 */
extern void frontend_setKeypadHandler(frontend_keypadHandler_t *callback, void *callbackParameter);

#endif
