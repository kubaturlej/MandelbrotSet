#include <iostream>
#include <memory>
#include <thread>
#include <vector>
#include <complex>
#include <chrono>

#include "SDLEnvironment.h"
#include "Pixel.h"


const int WIDTH = 800;
const int HEIGHT = 800;

long double xMove = 0;
long double yMove = 0;
long double zoom = 0.3;
long double maxIteration = 100;
long double N1 = 0.5;
long double N2 = 0.5;
auto sdl = std::make_unique<SDLEnvironment>();


void moveCamera()
{
    SDL_Event event = sdl->getEvent();
    SDL_PollEvent(&event);

    long double delta = 0.30;
 
    long double moveStep = 0.5;
    long double zoomStep = 3.0;

    const Uint8* keystate = sdl->getKeystate();
    if (keystate[SDL_SCANCODE_LEFT]) 
    {
        xMove = xMove + (moveStep / zoom * delta);
    }
    else if (keystate[SDL_SCANCODE_RIGHT]) 
    {
        xMove = xMove - (moveStep / zoom * delta);
    }
    else if (keystate[SDL_SCANCODE_UP]) 
    {
        yMove = yMove + (moveStep / zoom * delta);
    }
    else if (keystate[SDL_SCANCODE_DOWN]) 
    {
        yMove = yMove - (moveStep / zoom * delta);
    }
    else if (keystate[SDL_SCANCODE_KP_PLUS]) 
    {
        zoom = zoom + (moveStep * zoom * delta);
        maxIteration = maxIteration + zoomStep * delta;
    }
    else if (keystate[SDL_SCANCODE_KP_MINUS] && (zoom - (moveStep * zoom * delta)) > 0.3) 
    {
        zoom = zoom - (moveStep * zoom * delta);
        maxIteration = maxIteration - zoomStep * delta;
    }
    else if (keystate[SDL_SCANCODE_RSHIFT] && N1 + 0.1 < 1)
    {
        N1 += 0.1;
    }
    else if (keystate[SDL_SCANCODE_LSHIFT] && N1 - 0.1 > 0.1)
    {
        N1 -= 0.1;
    }
    else if (keystate[SDL_SCANCODE_RCTRL] && N2 + 0.1 < 1)
    {
        N2 += 0.1;
    }
    else if (keystate[SDL_SCANCODE_LCTRL] && N2 - 0.1 > 0.1)
    {
        N2 -= 0.1;
    }
}



void thread_main(uint32_t id, uint32_t thread_count, Pixel t[WIDTH][HEIGHT])
{
    int i;

    for (int x = id; x < WIDTH; x += thread_count) {
        for (int y = 0; y < HEIGHT; y++) {

            std::complex<long double> p((((long double)x - WIDTH / 2) / (N1 * WIDTH * zoom)) - xMove, (((long double)y - HEIGHT / 2) / (N2 * HEIGHT * zoom)) - yMove);

            std::complex<long double> z(0.0f, 0.0f);

            i = 0;

            do
            {
                z = z * z + p;
                i++;
            } while (z.real() * z.real() + z.imag() * z.imag() < 4.0L && i < 60);

            if (i >= 60)
            {
                // In the set
                t[x][y].R = 0;
                t[x][y].G = 0;
                t[x][y].B = 255;
            }
            else
            {
                // Not in the set
                t[x][y].R = 0;
                t[x][y].G = 0;
                t[x][y].B = i * (255 / 60);
            }
        }
    }
}


int main(int argc, char* argv[]) {

    int thread_count;
    static Pixel t[WIDTH][HEIGHT] = {};

    std::cout << "Thread count: ";
    std::cin >> thread_count;


  
    if (!sdl->init("Mandelbrot set", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT))
    {
        std::cout << "SDL init failed" << std::endl;
    }

    
    while (sdl->pressQuit() == 0)
    {
        SDL_RenderClear(sdl->getRenderer());
        moveCamera();

        auto start = std::chrono::steady_clock::now();

        t[WIDTH][HEIGHT] = {};
       
        std::vector<std::thread> threads;

        for (uint32_t i = 0; i < thread_count; i++)
        {
            threads.push_back(std::thread{thread_main, i, thread_count, std::ref(t)});
        }

        for (uint32_t i = 0; i < thread_count; i++)
        {
            threads[i].join();
        }
        auto end = std::chrono::steady_clock::now();

        for (int x = 0; x < WIDTH; x++) {
            for (int y = 0; y < HEIGHT; y++) {
                SDL_SetRenderDrawColor(sdl->getRenderer(), t[x][y].R, t[x][y].G, t[x][y].B, 255);
                SDL_RenderDrawPoint(sdl->getRenderer(), x, y);
            }
        }

        SDL_RenderPresent(sdl->getRenderer());

        std::cout << "Time: "
            << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
            << "ms" << std::endl;
    }
    
    return 0;
}