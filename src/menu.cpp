#include "menu.hpp"

enum STATE
{
	MAIN_MENU,
	CONFIG_MENU
};

enum CONFIG_ID
{
	CB_RENDER_SHADOWS,
	CB_RENDER_PARTICLES,
	CB_SHOW_CHUNK_DEBUG,
};

Menu::Menu(SDL_Renderer* renderer, TextureHandler& textures, EventWrapper& events, Timer& timer, WinSize& winSize)
    : state{0},
      winSize{winSize},
      timer{timer},
      offsetX{0},
      offsetY{0},
      BUTTON_W{200},
      showPlayBuffer{false},
      bgX{0},
      bgY{0},
      backButton{nullptr},
      renderer{renderer},
      textures{textures},
      events{events},
      back{40, 40, 400, 400},
      boxWidth{660},
      boxHeight{460},
      prevMidLeft{0},
      midLeft{0},
      contentLeft{0},
      padLeft{180},
      header{nullptr},
      paragraph{nullptr}
{		
	//************************************************
	// Resize optionState to match optionStr size
	//************************************************
	this->optionStr = {"Play", "About", "Config", "Exit"};
	optionState.resize(optionStr.size());
	buttonGroup.resize(optionStr.size());

	//************************************************
	// Setup buttons in main menu
	//************************************************
	const int offsetY = 48;
	
	for (size_t i = 0; i < buttonGroup.size(); ++i)
    {
		buttonGroup[i] = std::make_unique<Button>(Button(renderer, i, 0, 30+(offsetY*i), BUTTON_W));
		buttonGroup[i]->setText(optionStr[i]);
	}

	//************************************************
	// Setup options
	//************************************************
	backButton = std::make_unique<Button>(Button(renderer, -1, 0, 30, 150));
	backButton->setText("Return");

	// Setup checkboxes
	Checkbox* showShadows = new Checkbox(renderer, CB_RENDER_SHADOWS, "Show Shadows", 0, 0, 30);
	Checkbox* showParticles = new Checkbox(renderer, CB_RENDER_PARTICLES, "Show Particles", 0, 0, 30);
	Checkbox* showInfo = new Checkbox(renderer, CB_SHOW_CHUNK_DEBUG, "Show Chunk Debug Info", 0, 0, 30);
	if (constants::config.getInt(CONFIG_SHOW_SHADOWS) == 1) showShadows->checked = true;
	if (constants::config.getInt(CONFIG_SHOW_PARTICLES) == 1) showParticles->checked = true;
	if (constants::config.getInt(CONFIG_SHOW_CHUNK_DEBUG) == 1) showInfo->checked = true;

	// Insert checkboxes
	cElements.push_back(showShadows);
	cElements.push_back(showParticles);
	cElements.push_back(showInfo);

	//************************************************
	// Setup random block
	//************************************************
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist(0, constants::blockInfo.size()-1);

	int rand_id = constants::blockInfo[dist(rng)].id;
	randomBlock = Item(renderer, rand_id);

	//************************************************
	// Setup paragraph string
	//************************************************
	for (auto &i: constants::content)
    {
		pString += i + "\n";
	}
    SDL_Color textColor{255, 255, 255, 255};
	header = new Text(renderer, constants::header, textColor, constants::fontHandler.getFont(5));
	paragraph = new Text(renderer, pString, textColor, constants::fontHandler.getFont(3), 240);

	// Ensure all elements are properly updated (ex: offsets)
	update();
}

Menu::~Menu()
{
	// Delete all config elements
	for (auto i: cElements)
    {
		delete i;
	}
}

void Menu::updateConfigElements(int id, int value)
{
	switch(id)
    {
	case CB_RENDER_SHADOWS:
		constants::config.set(CONFIG_SHOW_SHADOWS, value);
		break;
	case CB_RENDER_PARTICLES:
		constants::config.set(CONFIG_SHOW_PARTICLES, value);
		break;
	case CB_SHOW_CHUNK_DEBUG:
		constants::config.set(CONFIG_SHOW_CHUNK_DEBUG, value);
		break;
	default:
		break;
	}
}

void Menu::update(bool updateAnimations)
{
	offsetX = 0;
	offsetY = (winSize.h / 2) - (boxHeight / 2);

	const int left = (winSize.w / 2) + 30;
	constexpr int top = 80;
	constexpr int gap = 50;
	
	prevMidLeft = midLeft;
	midLeft = (winSize.w / 2) - padLeft;
	contentLeft = midLeft - 100;
	
	if (prevMidLeft != midLeft)
    {
	    back.x = contentLeft-50;
	    back.y = 0;
		back.w = boxWidth;
		back.h = boxHeight;
		back.updatePair();
	}

	// Set background
	if (state == MAIN_MENU)
    {
		for (auto &i: buttonGroup)
        {
			i->setX( (winSize.w/2) + 30 );
			i->update(events, timer, offsetX, offsetY);
		}
	}
    else if (state == CONFIG_MENU)
    {
		backButton->setX( (winSize.w/2) + 30 );
		backButton->update(events, timer, offsetX, offsetY);
		for (size_t i = 0; i < cElements.size(); ++i)
        {
			cElements[i]->setX(left);
			cElements[i]->setY(top+(gap*i));
			cElements[i]->update(events, timer, offsetX, offsetY);

			cElements[i]->onChange(updateConfigElements);
		}

		if (backButton->clicked)
        {
			setState(MAIN_MENU);
		}

		
	}

	// Update animation for background
	if (updateAnimations)
    { // Removes stutter if we must call multiple updates in a single frame
		shift.tick(timer, 20);
	}
}


void Menu::setState(int state)
{
	update(false); // If we don't update before we change state it thinks that the button is
	// forever pressed (or the caller of this function in general)
	this->state = state;
	// Update again before we render, or else things may looks strange for a frame
	update(false);
}

void Menu::renderSidebar()
{
	// Draw line
	constexpr int vert_gap = 30;
    SDL_Rect line{offsetX+midLeft+padLeft, offsetY+vert_gap, 2, boxHeight-(vert_gap*2)};

    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderFillRect(renderer, &line);

	// Lets render a random block and some text
	randomBlock.render(renderer, textures, offsetX+contentLeft, offsetY+35, 60, 60);

	header->draw(offsetX+contentLeft+70, offsetY+45);

	paragraph->draw(offsetX+contentLeft+5, offsetY+120);
}

int Menu::getPressed()
{
	for (auto &i: buttonGroup)
    {
		if (i->clicked)
        {
			return i->id;
		}
	}
	return -1;
}


void Menu::renderMainMenu()
{
	// Render all buttons
	for (auto &i: buttonGroup)
    {
		i->render(renderer, textures, offsetX, offsetY);
	}
}

void Menu::renderConfigMenu()
{
	backButton->render(renderer, textures, offsetX, offsetY);
	// Render all elements, no matter the type
	for (auto &i: cElements)
    {
		i->render(renderer, textures, offsetX, offsetY);    
	}
}

void Menu::renderBackground()
{
    bgX += 100 * timer.deltaTime.s;
    Generic::Render::renderRepeating(renderer, textures, 8, winSize.w, winSize.h,
                                     bgX, bgY, 40, 40, 0, 0,
                                     true, constants::blockImgSize, constants::blockImgSize);
}

void Menu::render()
{
    if (!showPlayBuffer)
    {
        renderBackground();
    }
    else
    {
        SDL_Rect bg{0, 0, winSize.w, winSize.h};

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 120);
        SDL_RenderFillRect(renderer, &bg);
    }
    
    back.render(renderer, textures, offsetX, offsetY);
	renderSidebar();

	if (state == MAIN_MENU)
    {
		renderMainMenu();
	}
    else if (state == CONFIG_MENU)
    {
		renderConfigMenu();
	}
}

