#include<iostream>
#include<SDL3/SDL.h>
#include<SDL3/SDL_main.h>
#include<SDL3_image/SDL_image.h>
#include<string>
#include<SDL3/SDL_keyboard.h>
#include<SDL3/SDL_keycode.h>

class ltexture
{
public:
	ltexture();
	~ltexture();

	bool loadfromfile(std::string path);

	void destroy();
	void render(float x, float y);

	int getwidth();
	int getheight();
	bool isloaded();

private:
	SDL_Texture* mtexture;
	int mwidth;
	int mheight;
};

SDL_Window* gwindow{ nullptr };
SDL_Renderer* grenderer{ nullptr };
ltexture guptexture, gdowntexture, glefttexture, grighttexture;

ltexture::ltexture() :
	mtexture{ nullptr },
	mwidth{ 0 },
	mheight{ 0 }
{
}

ltexture::~ltexture()
{
	destroy();
}

bool ltexture::loadfromfile(std::string path)
{
	destroy();
	if (SDL_Surface* loadedsurface = IMG_Load(path.c_str()); loadedsurface == nullptr)
	{
		SDL_Log("Unable to load image %s! SDL_image error: %s\n", path.c_str(), SDL_GetError());
	}
	else
	{
		if (mtexture = SDL_CreateTextureFromSurface(grenderer, loadedsurface); mtexture == nullptr)
		{
			SDL_Log("Unable to create texture from loaded pixels! SDL error: %s\n", SDL_GetError());
		}
		else
		{
			mwidth = loadedsurface->w;
			mheight = loadedsurface->h;
		}
		SDL_DestroySurface(loadedsurface);
	}
	return mtexture != nullptr;
}

void ltexture::destroy()
{
	if (mtexture) {
		SDL_DestroyTexture(mtexture);
		mtexture = nullptr;
	}
	mwidth = 0;
	mheight = 0;
}

void ltexture::render(float x, float y)
{
	SDL_FRect dstRect{ x, y, static_cast<float>(mwidth), static_cast<float>(mheight) };
	SDL_RenderTexture(grenderer, mtexture, nullptr, &dstRect);
}

int ltexture::getwidth()
{
	return mwidth;
}

int ltexture::getheight()
{
	return mheight;
}

bool ltexture::isloaded()
{
	return mtexture != nullptr;
}

constexpr int kScreenWidth = 680;
constexpr int kScreenHeight = 480;

bool init()
{
	bool success{ true };
	if (SDL_Init(SDL_INIT_VIDEO) == false)
	{
		SDL_Log("SDL could not be initialized! SDL error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		if (SDL_CreateWindowAndRenderer("sdl3", kScreenWidth, kScreenHeight, 0, &gwindow, &grenderer) == 0)
		{
			SDL_Log("Window could not be created! SDL error: %s\n", SDL_GetError());
			success = false;
		}
	}
	return success;
}

bool loadmedia()
{
	bool success{ true };

	//Load directional images
	if (!guptexture.loadfromfile("assets/up.png"))
	{
		SDL_Log("Unable to load up image!\n");
		success = false;
	}
	if (!gdowntexture.loadfromfile("assets/down.png"))
	{
		SDL_Log("Unable to load down image!\n");
		success = false;
	}
	if (!glefttexture.loadfromfile("assets/left.png"))
	{
		SDL_Log("Unable to load left image!\n");
		success = false;
	}
	if (!grighttexture.loadfromfile("assets/right.png"))
	{
		SDL_Log("Unable to load right image!\n");
		success = false;
	}

	return success;
}

void close()
{
	guptexture.destroy();
	gdowntexture.destroy();
	glefttexture.destroy();
	grighttexture.destroy();

	if (grenderer) {
		SDL_DestroyRenderer(grenderer);
		grenderer = nullptr;
	}
	if (gwindow) {
		SDL_DestroyWindow(gwindow);
		gwindow = nullptr;
	}
	SDL_Quit();
}

int main(int argc, char* argv[])
{
	int exitcode{ 0 };
	if( init() == false )
	{
		SDL_Log("Unable to initialize\n");
		exitcode = 1;
	}
	else
	{
		if  (loadmedia() == false)
		{
			SDL_Log("Unable to load media\n");
			exitcode = 2;
		}
		else
		{
			float a = 0;
			float b = 0;
			bool quit = false;
			ltexture* currentTexture = &guptexture;
			SDL_Color bgColor{ 0xFF, 0xFF, 0xFF, 0xFF };
			SDL_Event e;
			while (quit == false)
			{
				while (SDL_PollEvent(&e))
				{
					//If event is quit type
					if (e.type == SDL_EVENT_QUIT)
					{
						quit = true;
					}
					//On keyboard key press
					else if (e.type == SDL_EVENT_KEY_DOWN)
					{
						//Set texture
						if (e.key.key == SDLK_UP)
						{
							currentTexture = &guptexture;
						}
						else if (e.key.key == SDLK_DOWN)
						{
							currentTexture = &gdowntexture;
						}
						else if (e.key.key == SDLK_LEFT)
						{
							currentTexture = &glefttexture;
						}
						else if (e.key.key == SDLK_RIGHT)
						{
							currentTexture = &grighttexture;
						}
					}


					//Reset background color to white
					bgColor.r = 0xFF;
					bgColor.g = 0xFF;
					bgColor.b = 0xFF;

					//Set background color based on key state
					const bool* keyStates = SDL_GetKeyboardState(nullptr);
					if (keyStates[SDL_SCANCODE_UP])
					{
						//Red
						bgColor.r = 0xFF;
						bgColor.g = 0x00;
						bgColor.b = 0x00;
					}
					else if (keyStates[SDL_SCANCODE_DOWN])
					{
						//Green
						bgColor.r = 0x00;
						bgColor.g = 0xFF;
						bgColor.b = 0x00;
					}
					else if (keyStates[SDL_SCANCODE_LEFT])
					{
						//Yellow
						bgColor.r = 0xFF;
						bgColor.g = 0xFF;
						bgColor.b = 0x00;
					}
					else if (keyStates[SDL_SCANCODE_RIGHT])
					{
						//Blue
						bgColor.r = 0x00;
						bgColor.g = 0x00;
						bgColor.b = 0xFF;
					}
					SDL_SetRenderDrawColor(grenderer, bgColor.r, bgColor.g, bgColor.b, 0xFF);
					SDL_RenderClear(grenderer);

					//Render image on screen
					currentTexture->render((kScreenWidth - currentTexture->getwidth()) / 2.f, (kScreenHeight - currentTexture->getheight()) / 2.f);

					//Update screen
					SDL_RenderPresent(grenderer);
				}
			}
		}
	}
	close();
	return exitcode;
}