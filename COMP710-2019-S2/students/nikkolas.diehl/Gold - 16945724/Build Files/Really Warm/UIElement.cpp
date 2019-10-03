#define _CRTDBG_MAP_ALLOC
#include "UIElement.h"

UIElement::UIElement()
: mx_box()
, spriteExists(false)
, animatedSpriteExists(false)
, font(0)
{
}
UIElement::~UIElement()
{
	delete mx_box;
	mx_box = nullptr;
	
	if (spriteExists) {
		delete mx_sprite;
	}
	mx_sprite = nullptr;

	if (animatedSpriteExists) {
		delete mx_animatedSprite;
	}
	mx_animatedSprite = nullptr;
}

bool
UIElement::Initialise_S(Sprite* sprite)
{
	assert(sprite);
	mx_sprite = sprite;

	spriteExists = true;

	return (true);
}

bool 
UIElement::Initialise_An(AnimatedSprite * sprite)
{
	assert(sprite);
	mx_animatedSprite = sprite;

	animatedSpriteExists = true;

	return (true);
}

void 
UIElement::CreateAnimatedUIElement(std::string fileName, BackBuffer * backBuffer, 
								   std::string UIText, int x, int y, double scale, 
								   int frameCount, int panelCount, float frameSpeed, int _font)
{
	//Create empty box
	mx_box = new Rectangle();
	mx_s_uiText = UIText;

	//Initialise the UI element
	if (!fileName.empty()) {
		Initialise_An(backBuffer->CreateAnimatedSprite(fileName.c_str(), frameCount, panelCount, frameSpeed));
	}

	SetSize(static_cast<int>(mx_animatedSprite->GetFrameWidth() * scale), static_cast<int>(mx_animatedSprite->GetFrameHeight() * scale));
	SetPos(x - static_cast<int>(mx_animatedSprite->GetFrameWidth() * scale) / 2, y); //Center the buttons

	//Set text wrap limit
	wrapLimit = static_cast<int>(mx_animatedSprite->GetFrameWidth() * 0.95);

	//Set up sprite sheet data
	mx_animatedSprite->SetSpriteScaler(scale);
	mx_animatedSprite->SetCurrentPanel(1);

	//Set up text font
	font = _font;
}

//File name, backbuffer, x, y and width height as a scaler from 0 to 1
void
UIElement::CreateUIElement(std::string fileName, BackBuffer* backBuffer, 
						   std::string UIText, int x, int y, double scale, int _font)
{
	//Create empty box
	mx_box = new Rectangle();
	mx_s_uiText = UIText;

	//Initialise the UI element
	if (!fileName.empty()) {
		Initialise_S(backBuffer->CreateSprite(fileName.c_str()));
	}

	SetSize(static_cast<int>(mx_sprite->GetWidth() * scale), static_cast<int>(mx_sprite->GetHeight() * scale));
	SetPos(x - static_cast<int>(mx_sprite->GetWidth() * scale) / 2, y);

	//Set text wrap limit
	wrapLimit = static_cast<int>(mx_sprite->GetWidth() * 0.95);

	//Set up sprite data
	mx_sprite->SetSpriteScaler(scale);

	//Set up text font
	font = _font;
}

void
UIElement::DrawUIElement(BackBuffer& backBuffer, unsigned char alpha, bool Y_TOP)
{
	//If sprite exists than draw sprite
	if (spriteExists)
	{
		assert(mx_sprite);
		//Set x, y, length and height if custom
		mx_sprite->SetX(static_cast<int>(mx_box->X()));
		mx_sprite->SetY(static_cast<int>(mx_box->Y()));

		mx_sprite->Draw(backBuffer, alpha);
	}

	//If animated sprite exists, than draw animated sprite
	if (animatedSpriteExists) {
		assert(mx_animatedSprite);
		//Set x, y, length and height if custom
		mx_animatedSprite->SetX(static_cast<int>(mx_box->X()));
		mx_animatedSprite->SetY(static_cast<int>(mx_box->Y()));

		mx_animatedSprite->Draw(backBuffer, alpha);
	}

	//If text is not null then draw text
	if (!mx_s_uiText.empty())
	{
		backBuffer.SetTextColour(255, 255, 255);
		double textBufferX = mx_box->X() + (mx_box->W() / 2);
		double textBufferY = 0;

		//Position top or middle
		if (Y_TOP) {
			textBufferY = mx_box->Y() + (mx_box->H() * 0.05);
		}
		else {
			textBufferY = mx_box->Y() + (mx_box->H() / 3);
		}

		backBuffer.DrawText(
			static_cast<int>(textBufferX), 
			static_cast<int>(textBufferY),
			mx_s_uiText.c_str(), font, true, wrapLimit
		);
	}
}

Rectangle* 
UIElement::GetBox()
{
	return mx_box;
}

AnimatedSprite* 
UIElement::GetAnimatedSprite()
{
	assert(mx_animatedSprite);

	return mx_animatedSprite;
}

void 
UIElement::SetSize(int w, int h)
{
	mx_box->W() = w;
	mx_box->H() = h;
}

void 
UIElement::SetPos(int x, int y)
{
	mx_box->X() = x;
	mx_box->Y() = y;
}

void 
UIElement::UpdateElementText(std::string _newText)
{
	mx_s_uiText = _newText;
}
