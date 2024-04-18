#include "BaseObject.h"
//Parent class
BaseObject::BaseObject()
{
	object_ = NULL;
	rect_.x = 0;
	rect_.y = 0;
	rect_.w = 0;
	rect_.h = 0;
}

BaseObject::~BaseObject()
{
	Free();
}

bool BaseObject::LoadImg(string path, SDL_Renderer* des)
{
	Free();
	SDL_Texture* newTexture = NULL;
	SDL_Surface* loadSurface = IMG_Load(path.c_str());
	if (loadSurface != NULL)
	{
		//Tranparent
		SDL_SetColorKey(loadSurface, SDL_TRUE, SDL_MapRGB(loadSurface->format, COLLOR_KEY_R, COLLOR_KEY_G, COLLOR_KEY_B));
		newTexture = SDL_CreateTextureFromSurface(des, loadSurface);
		if (newTexture != NULL) {
			rect_.w = loadSurface->w;
			rect_.h = loadSurface->h;
		}
		SDL_FreeSurface(loadSurface);
	}
	object_ = newTexture;
	return object_ != NULL;

}




void BaseObject::Render(SDL_Renderer* des, const SDL_Rect* clip) {
	SDL_Rect renderQuad = { rect_.x, rect_.y, rect_.w, rect_.h };
	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}
	SDL_RenderCopy(des, object_, clip, &renderQuad);
}



void BaseObject::Free()
{
	if (object_ != NULL)
	{
		SDL_DestroyTexture(object_);
		object_ = NULL;
		rect_.w = 0;
		rect_.h = 0;
	}
}
