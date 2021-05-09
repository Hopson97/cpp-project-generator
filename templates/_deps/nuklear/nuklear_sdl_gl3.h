/*
 * Nuklear - 1.32.0 - public domain
 * no warrenty implied; use at your own risk.
 * authored from 2015-2016 by Micha Mettke
 */
#pragma once
#include <SDL2/SDL.h>

#include "nuklear_def.h"

NK_API struct nk_context*   nk_sdl_init(SDL_Window *win);
NK_API void                 nk_sdl_font_stash_begin(struct nk_font_atlas **atlas);
NK_API void                 nk_sdl_font_stash_end(void);
NK_API int                  nk_sdl_handle_event(SDL_Event *evt);
NK_API void                 nk_sdl_render(enum nk_anti_aliasing , int max_vertex_buffer, int max_element_buffer);
NK_API void                 nk_sdl_shutdown(void);
NK_API void                 nk_sdl_device_destroy(void);
NK_API void                 nk_sdl_device_create(void);

// demo stuff
enum nk_theme { THEME_BLACK, THEME_WHITE, THEME_RED, THEME_BLUE, THEME_DARK };
int nk_overview(struct nk_context* ctx);
int nk_node_editor(struct nk_context* ctx);
void nk_calculator(struct nk_context* ctx);
void nk_set_style(struct nk_context* ctx, enum nk_theme theme);
