#ifndef BASE_OBJECT_H_
#define BASE_OBJECT_H_

#include"BaseFunction.h"
class BaseObject //Parent class
{
public:
	BaseObject();
	~BaseObject();
	void SetRect(const int &x, const int &y)
	{
		rect_.x = x;
		rect_.y = y;
	}

	SDL_Rect GetRect() const
	{
		return rect_;
	}

	SDL_Texture* GetObject() const
	{
		return object_;
	}

	bool LoadImg(string path, SDL_Renderer* des);

	bool loadFromRenderedText(string textureText, SDL_Color textColor, TTF_Font* font);


	void Render(SDL_Renderer* des, const SDL_Rect* clip = NULL);

	void Free();
protected:
	SDL_Texture* object_;//stores texture

	SDL_Rect rect_;		/* rect_.x và rect_.y là vị trí vật thể trên màn hình
							rect_.h và rect_.w là chiều cao chiều rộng của vật thể
						*/
};

#endif 


