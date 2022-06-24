#ifndef __ERR_HAND_SDL_H__
#define __ERR_HAND_SDL_H__

#define ErrHandSDL_quit(msg) \
    { \
        printf("\n%s: %s", msg, SDL_GetError()); \
        SDL_Quit(); \
        return EXIT_FAILURE; \
    }
#define ErrHandSDL_quit_if_NULL(var, msg) if (var == NULL) ErrHandSDL_quit(msg)

#endif // __ERR_HAND_SDL_H__
