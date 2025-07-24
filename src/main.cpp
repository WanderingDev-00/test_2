#include<iostream>
#include<SDL3/SDL.h>
#include<SDL3/SDL_main.h>
#include<SDL3_image/SDL_image.h>
#include<string>

class ltexture
{public:
	ltexture();
	~ltexture();

	bool loadfromfile(std::string path);

	void destroy();
	void render(float x,float y);

	int getwidth();
	int getheight();
	bool isloaded();

private: SDL_Texture* mtexture;
	   int mwidth;
	   int mheight;

};
SDL_Window* gwindow{ nullptr };
SDL_Renderer* grenderer{ nullptr };
ltexture gpngtexture;

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
	SDL_DestroyTexture(mtexture);
	mtexture = nullptr;
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

bool init()
{
	bool success{ true };

	if (SDL_Init(SDL_INIT_VIDEO) == false)
	{
		SDL_Log("sdl could not be intialized sdl error :%s\n", SDL_GetError());
		success = false;

	}
	else
		
	{
		int kscreenwidth = 680;
		int  kscreenheight = 480;
		if (SDL_CreateWindowAndRenderer("sdl3", kscreenwidth, kscreenheight, 0, &gwindow, &grenderer) == false)
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
	if (gpngtexture.loadfromfile("assets/FACELESS_SLIME.png") == false)
	{
		SDL_Log("unable to load png");
		success = false;

	}
	return success;
}
void close()
{
	gpngtexture.destroy();
	SDL_DestroyRenderer(grenderer);
	grenderer = nullptr;
	SDL_DestroyWindow(gwindow);
	gwindow = nullptr;
	SDL_Quit();
	
}


int main(int argc,char* argv[])
{
	int exitcode{ 0 };
	if (init() == false)
	{
		SDL_Log("unable to inztialize \n");
		exitcode = 1;
	}
	else
	{
		if (loadmedia() == false)
		{
			SDL_Log("unable to load meadia \n");
			exitcode = 2;
		}
		else
		{
			float a = 0;
			float b = 0;
			bool quit(false);
			SDL_Event e;
			SDL_zero(e);
			while (quit == false)
			{
				while (SDL_PollEvent(&e) == true)
				{


					if (e.type == SDL_EVENT_KEY_DOWN)
					{
						if (e.key.scancode == SDL_SCANCODE_W) {
							b--;
							break;/* pressed what would be "W" on a US QWERTY keyboard. Move forward! */
						}
						else if (e.key.scancode == SDL_SCANCODE_S) {
							b++;
							break;/* pressed what would be "S" on a US QWERTY keyboard. Move backward! */
						}
						else if (e.key.scancode == SDL_SCANCODE_A)
						{
							a--;
							break;

						}
						else if (e.key.scancode == SDL_SCANCODE_D)
						{
							a++;
						}
						
					}
					else if (e.type == SDL_EVENT_QUIT)
					{
						quit = true;
					}
					else
					{
						continue;
					}
				}
				SDL_SetRenderDrawColor(grenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(grenderer);
				gpngtexture.render(a, b);
				SDL_RenderPresent(grenderer);





			}
		}
	}
	close();
	return exitcode;
}

