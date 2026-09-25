#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <cmath>

constexpr float GRAVITY = 1000.0f;
constexpr float BOUNCE_LOSS = 0.75f;
constexpr float FRICTION_LOSS = 0.75f;
constexpr float TRAVEL_SPEED = 0.5f;

void Quit(SDL_Renderer *renderer, SDL_Window *window, SDL_Texture *ball);
double get_delta_time(Uint64 *lastTime);
void Render_Texture(SDL_Renderer *renderer, SDL_Texture *ball,
                    SDL_FRect srcrect, SDL_FRect dstrect);

int main(int argc, char *argv[]) {
  int WINDOW_WIDTH = 600;
  int WINDOW_HEIGHT = 800;

  const float ballX = 300.0f;
  const float ballY = 400.0f;
  const float ball_size = 100.0f;

  SDL_Window *window = NULL;
  SDL_Renderer *renderer = NULL;
  SDL_Texture *ball = NULL;

  double dT = 0.0;
  struct {
    float X = 0.0f;
    float Y = 0.0f;
  } velocity;
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("SDL_Init failed: %s", SDL_GetError());
    return -1;
  }

  if (!SDL_CreateWindowAndRenderer("jumping cat animation", WINDOW_WIDTH,
                                   WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE, &window,
                                   &renderer)) {
    SDL_Log("SDL_CreateWindowAndRenderer failed: %s", SDL_GetError());
    SDL_Quit();
    return -1;
  }

  SDL_Surface *surface = SDL_LoadPNG("C:/Users/DELL/dev/BouncingBall/ball.png");

  if (!surface) {
    SDL_Log("PNG loading failed: %s", SDL_GetError());
    return -1;
  }

  ball = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_DestroySurface(surface);

  if (!ball) {
    SDL_Log("Texture creation failed: %s", SDL_GetError());
    return -1;
  }

  SDL_FRect srcrect{0.0f, 0.0f, 1120.0f, 1120.0f};
  SDL_FRect dstrect{ballX, ballY, ball_size, ball_size};

  SDL_Event event;
  Uint64 lastTime = SDL_GetPerformanceCounter();
  SDL_FRect *ballrect = &dstrect;
  float targetX;
  float targetY;
  bool isMovingToTarget = false;
  float distanceY = 0.0f;

  float distanceX = 0.0f;
  while (true) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) {
        Quit(renderer, window, ball);
        return 0;
      } else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        if (event.button.button == SDL_BUTTON_LEFT) {
          targetX = event.button.x - (ball_size / 2.0f);
          targetY = event.button.y - (ball_size / 2.0f);
          isMovingToTarget = true;
        }
      } else if (event.type == SDL_EVENT_WINDOW_RESIZED) {
        SDL_GetWindowSize(window, &WINDOW_WIDTH, &WINDOW_HEIGHT);
      }
    }

    dT = get_delta_time(&lastTime);

    velocity.Y += GRAVITY * dT;

    ballrect->y += velocity.Y * dT;
    ballrect->x += velocity.X * dT;

    if (isMovingToTarget && ballrect->y + ball_size >= WINDOW_HEIGHT) {
      distanceY = ballrect->y - targetY;
      distanceX = ballrect->x - targetX;

      velocity.Y = -(distanceY / TRAVEL_SPEED);
      velocity.X = -(distanceX / TRAVEL_SPEED);

      isMovingToTarget = false;

    } else {

      // Bottom clamp
      if (ballrect->y + ball_size >= WINDOW_HEIGHT) {
        ballrect->y = WINDOW_HEIGHT - ball_size;

        if (velocity.Y < 350.0f) {
          velocity.Y *= -1.0f;
          velocity.X = 0.0f;
        } else {
          velocity.X *= FRICTION_LOSS;
          velocity.Y *= -BOUNCE_LOSS;
          if (std::abs(velocity.X) < 80.0f) {
            velocity.X = 0.0f;
          }
        }
      }

      // Top clamp
      if (ballrect->y <= 0) {
        ballrect->y = 0;
        velocity.Y *= -1.0f;
      }

      // Right clamp
      if (ballrect->x + ball_size >= WINDOW_WIDTH) {
        ballrect->x = WINDOW_WIDTH - ball_size;
        velocity.X *= -FRICTION_LOSS;
      }

      // Left clamp
      if (ballrect->x <= 0) {
        ballrect->x = 0;
        velocity.X *= -FRICTION_LOSS;
      }
    }

    Render_Texture(renderer, ball, srcrect, dstrect);
  }
}

void Render_Texture(SDL_Renderer *renderer, SDL_Texture *ball,
                    SDL_FRect srcrect, SDL_FRect dstrect) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
  SDL_RenderTexture(renderer, ball, &srcrect, &dstrect);
  SDL_RenderPresent(renderer);
}
double get_delta_time(Uint64 *lastTime) {
  Uint64 currentTime = SDL_GetPerformanceCounter();
  double dT =
      (double)(currentTime - *lastTime) / (double)SDL_GetPerformanceFrequency();
  *lastTime = currentTime;
  return dT;
}
void Quit(SDL_Renderer *renderer, SDL_Window *window, SDL_Texture *ball) {
  SDL_DestroyTexture(ball);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
