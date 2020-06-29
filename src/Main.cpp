#include <iostream>

#include <SDL2/SDL.h>
#undef main

#define SIZEX 1280
#define SIZEY 720
#define MAXITER 10000
#define LIMIT 10000

bool running = true;
Uint32 waitingTime = 50;
SDL_Window *window;
SDL_Renderer *renderer;

double coordRechts = 0.8;
double coordLinks  = -2.3;
double coordOben   = 1.0;
double coordUnten  = -1.0;

uint32_t tempScala[100];

void Point(int x, int y)
	{
	SDL_RenderDrawPoint(renderer, x, y);
	}

void CPoint(int x, int y, uint32_t color)
	{
	uint32_t r = ((color >> 24) & 0xff) / 255;
	uint32_t g = ((color >> 16) & 0xff) / 255;
	uint32_t b = ((color >> 8) & 0xff) / 255;
	uint32_t a = (color & 0xff) / 255;

	if (r == 1) r = 0xFF;
	else r = 0x00;

	if (g == 1) g = 0xFF;
	else g = 0x00;

	if (b == 1) b = 0xFF;
	else b = 0x00;

	if (a == 1) a = 0xFF;
	else a = 0x00;

	SDL_SetRenderDrawColor(renderer, r, g, b, a);
	SDL_RenderDrawPoint(renderer, x, y);
	}

void poll_events()
	{
	SDL_Event e;
	while (SDL_PollEvent(&e))
		{
		switch (e.type)
			{
			case SDL_QUIT:
				running = false;
				break;

			case SDL_KEYDOWN:
				switch (e.key.keysym.scancode)
					{
					case SDL_SCANCODE_ESCAPE:
						running = false;
						break;

					default:
						break;
					}
				break;

			default:
				break;
			}
		}
	}

int32_t gen_color(uint32_t x, uint32_t y)
	{
	double cReal = x * (coordRechts - coordLinks) / (double)SIZEX + coordLinks;
	double cImag = y * (coordOben - coordUnten) / (double)SIZEY + coordUnten;

	double check = 0.0;
	double zReal = 0.0;
	double zImag = 0.0;
	double altesZImag = 0.0;
	int32_t n = 0;

	for (n = 0; n < MAXITER; ++n)
		{
		zImag = 2 * zReal * zImag + cImag;
		zReal = zReal * zReal - altesZImag * altesZImag + cReal;

		check = zReal * zReal + zImag * zImag;
		if (check > LIMIT)
			{
			return n;
			}

		altesZImag = zImag;
		}

	return -1;
	}

void draw_field_map()
	{
	for (uint16_t y = 0; y < SIZEY; y++)
		{
		for (uint16_t x = 0; x < SIZEX; x++)
			{
			int32_t n = gen_color(x, y) % 100;

			if (-1 == n)
				{
				CPoint(x, y, 0x000000FF);
				}
			else
				{
				CPoint(x, y, tempScala[n]);
				}
			}
		}
	}

int main(int argc, char *argv[])
	{
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("AppleMan - by Can Karka", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SIZEX, SIZEY, 0);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	for (uint32_t c = 0; c < 100; ++c)
		{
		uint32_t f;
		uint8_t r, g, b, a;

		f = (uint32_t)(255.0 * c / 100.0);
		r = f;
		g = (f < 128) ? 2 * f : 512 - 2 * f;
		b = 255 - f;
		a = 255;

		uint32_t color = a << 24 | b << 16 | g << 8 | r;
		tempScala[c] = color;
		}

	while (running)
		{
		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
		SDL_RenderClear(renderer);

		draw_field_map();
		SDL_RenderPresent(renderer);

		poll_events();
		SDL_GL_SwapWindow(window);
		SDL_Delay(waitingTime);
		}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
	}