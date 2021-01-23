#include "inventory.hpp"

Inventory::Inventory(SDL_Renderer* renderer,
                     TextureHandler& textures,
					 EventWrapper& events,
                     WinSize& winSize)
    : winSize{winSize},
      animation{false},
      hotbarSlots{10},
      maxSlots{hotbarSlots*5},
      visible{true},
      showInventoryMenu{false},
      hotbarPos{0},
      itemHeld{},
      renderer{renderer},
      textures{textures},
      events{events}
{
	items.resize(maxSlots, Item{});

	inventorySlide = Transition{static_cast<double>(winSize.h), 25.6};
	hotbarSlide = Transition{0, 25.6};
}

Inventory::~Inventory() {}

Item& Inventory::getSelectedItem()
{
	return items[hotbarPos];
}

void Inventory::addItem(int id)
{
	constexpr int maxCount = 99;

	auto found = std::find_if(items.begin(), items.end(), [&](Item item)
        {
            if (!item.enabled) return false; 
            return item.block.id == id ? item.count < maxCount : false;
        });

	if (found != items.end())
    {
		found->addCount();
	}
    else
    {
		for (Item& i: items)
        {
			if (!i.enabled)
            {
				i = Item{renderer, id};
				break;
			}
		}
	}
}

void Inventory::update(Timer& timer)
{
	// hotbar_slot keys
	for (int i = 5; i < 15; ++i)
    {
		if (events.keyState[i])
        {
			hotbarPos = i-5;
		}
	}

    // Scroll hotbar
    if (!showInventoryMenu) shiftHotbarPos(-events.vmouse.scroll);

	if (events.keyState[4] || events.buttonState[8])
    {
		showInventoryMenu = !showInventoryMenu;
		animation = showInventoryMenu;
	}

	inventorySlide.update(timer);
	hotbarSlide.update(timer);
}

void Inventory::shiftHotbarPos(int shift)
{
    const int newPos = hotbarPos + shift;
    hotbarPos = newPos < 0 ? hotbarSlots-1 : newPos % hotbarSlots;
}

void Inventory::drawInventoryMenu()
{
    inventorySlide.value = animation ? 0 : winSize.h;

	int slide = inventorySlide.get();

	constexpr int OFFSET = 1;
    
	if (slide < winSize.h - OFFSET)
    {
		int scale = 3; // TODO Fix with hovered position

		constexpr int MENU_WIDTH = 170;
		constexpr int MENU_HEIGHT = 90;
		
		const int MAX_X = (winSize.w - (MENU_WIDTH * scale)) / 2;
		const int MAX_Y = (winSize.h - (MENU_HEIGHT * scale)) / 2;

		// First draw BG shadow
		SDL_Rect shadow{0, static_cast<int>(slide), winSize.w, winSize.h};
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 180);
        SDL_RenderFillRect(renderer, &shadow);

		// Render inventory menu
		SDL_Rect src{0, 0, MENU_WIDTH, MENU_HEIGHT};
		SDL_Rect dest{MAX_X, MAX_Y + static_cast<int>(slide), src.w * scale, src.h * scale};
        SDL_RenderCopy(renderer, textures.getTexture(11), &src, &dest);

		std::vector<int> pos = getHoveredPos(events.vmouse.x, events.vmouse.y,
                                               MAX_X, MAX_Y + slide, true);
		
		if (events.vmouse.clicked == 1 && pos[0] != -1 && pos[1] != -1)
        {
			auto& it = items[pos[0] + (pos[1] * hotbarSlots)];
			if (!itemHeld.enabled)
            {
				// Move item
				itemHeld = it;
				it = Item{};
			}
            else
            {
				// Swap items
			    Item tmp = it;
				it = itemHeld;
				itemHeld = tmp;
			}
		}
		
		if (itemHeld.enabled)
            itemHeld.render(renderer, textures, events.vmouse.x-17, events.vmouse.y-17, 35, 35);
	}
}

// returns std::vector<int>{-1, -1} if nothing is being hovered
std::vector<int> Inventory::getHoveredPos(int x, int y, int cornerX, int cornerY, bool draw = false)
{
	constexpr int TILE_SIZE = 42;

	// Start gap
	constexpr int SX_GAP = 18;
	constexpr int SY_GAP = 18;

	// Splitter gap
	constexpr int SP_GAP = 6;
	const int TOTAL = maxSlots / 10;

	std::vector<int> returner;
	
	// Loop through slots
	for (int i = 0; i < hotbarSlots; ++i)
    {
		for (int j = 0; j < TOTAL; ++j)
        {
			// Check Generic::collision<int>
			int new_x = cornerX + (i * (TILE_SIZE + SP_GAP) + SX_GAP);
			int new_y = cornerY + (j * (TILE_SIZE + SP_GAP) + SY_GAP);

			// Draw item
			if (items[i + (j * hotbarSlots)].enabled)
            {
				items[i + (j * hotbarSlots)].render(renderer, textures, new_x + 3, new_y + 3,
                                                    TILE_SIZE - 7, TILE_SIZE - 7);
			}

			if (Generic::collision<int>(x, y, 1, 1,
						  new_x, new_y, TILE_SIZE, TILE_SIZE))
            {

				if (draw)
                {
				    SDL_Rect rect{new_x, new_y, TILE_SIZE, TILE_SIZE};
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 40);
                    SDL_RenderFillRect(renderer, &rect);
				}

				returner = {i, j};
			}
		}
	}

	if (returner.size() == 2)
    {
		return returner;
	}

	return std::vector<int>{-1, -1};
}

void Inventory::drawHotbar()
{
	constexpr int BLOCK_S = 40;
    const int MAX_X = (winSize.w - ((hotbarSlots + 1) * BLOCK_S + 3)) / 2;
	constexpr int OFFSET = -1;

    hotbarSlide.value = animation ? -BLOCK_S + OFFSET : 0;

	int slide = hotbarSlide.get();
	
	for (int i = 0; i < hotbarSlots; ++i)
    {
	    SDL_Rect src{0, 0, 16, 16};
		if (hotbarPos == i)
        {
			src.y = 16;
		}
	    SDL_Rect block{i * (BLOCK_S + 3) + MAX_X, slide, BLOCK_S, BLOCK_S};
        SDL_RenderCopy(renderer, textures.getTexture(10), &src, &block);

		if (items[i].enabled) {
			items[i].render(renderer, textures, block.x + 5, slide + block.y + 5, 30, 30);
		}
	}
}
