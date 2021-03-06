#include "generic.hpp"

bool Generic::collision(SDL_Rect r1, SDL_Rect r2)
{
    return Generic::collision(r1.x, r1.y, r1.w, r1.h, r2.x, r2.y, r2.w, r2.h);
}

GridCollision_t Generic::gridCollision(const unsigned width, const unsigned height,
                                       glm::vec2 box, const glm::vec2& size)
{
    uint8_t add = 0; // 1 = X, 2 = Y, 3 = XY
    // negative fix
    if (box.x < 0) {
        box.x -= width + 1;
        add = add | 1;
    }   
    if (box.y < 0) {
        box.y -= height + 1;
        add = add | 2;
    }

    // Constrain coordinates
    box.x = std::fmod(box.x, width);
    box.y = std::fmod(box.y, height);

    return {static_cast<unsigned>(((size.x + box.x) / width) + ((add & 1) == 1)),
        static_cast<unsigned>(((size.y + box.y) / height) + ((add & 2) == 2))};
}

void Generic::serializeUnsigned(const unsigned value, const unsigned length,
                                std::function<void(uint8_t)> appendData)
{
    constexpr uint8_t BYTE_SIZE = 8;

    for (unsigned in = 0, maxLen = 0; maxLen < length; ++maxLen)
    {
        appendData(static_cast<uint8_t>((value >> in) & 0xff));
        in += BYTE_SIZE;
    }
}

void Generic::serializeSigned(const int value, const unsigned length,
                              std::function<void(uint8_t)> appendData)
{
    constexpr uint8_t BYTE_SIZE = 8;
    
    unsigned value_Uint = std::abs(value);
    for (unsigned in = 0, maxLen = 0; maxLen < length; ++maxLen)
    {
        uint8_t data = (value_Uint >> in) & 0xff;
        if (maxLen+1 == length) data ^= (value<0)<<7;
        appendData(data);
        in += BYTE_SIZE;
    }
}

double Generic::lerp(double v1, double v2, double t)
{
    return v1 + t * (v2 - v1);
}

#ifndef __HEADLESS
void Generic::Render::renderRepeating(const Graphics& graphics,
                                      const int texture, const int clipWidth,
                                      const int clipHeight, const int offsetX, const int offsetY,
                                      const int width, const int height, const int gap, const int top,
                                      const bool verticle, const int srcX, const int srcY,
                                      int srcW, int srcH) noexcept
{
    srcW = srcW == -1 ? width : srcW;
    srcH = srcH == -1 ? height : srcH;
	
	// Create a grid of tiles as the background
	for (int i = -1; (width + gap) * i < clipWidth + (width + gap); i++) // Horizontal grid
    {
		for (int j = -1; verticle ? (height + gap) * j < clipHeight + (height + gap) : j < 0; j++) // Vertical grid
        {
			SDL_Rect src{srcX, srcY, srcW, srcH};
            
		    SDL_Rect block{(offsetX % (width + gap)) + ((width + gap) * i),
				verticle ? ((offsetY % (height + gap)) + top) + ((height + gap) * j) : top + offsetY,
				width, height};

            //SDL_RenderCopy(renderer, textures.getTexture(texture)->texture, &src, &block);
		}
	}
}
void Generic::Render::generateSquare(std::vector<Vertex>& mesh, float b1, float b2, float e1, float e2,
                                     float tb1, float tb2, float te1, float te2)
{
    mesh.emplace_back(glm::vec3(b1, e2, 0.0f), glm::vec2(tb1, te2));
    mesh.emplace_back(glm::vec3(e1, e2, 0.0f), glm::vec2(te1, te2));
    mesh.emplace_back(glm::vec3(e1, b2, 0.0f), glm::vec2(te1, tb2));
            
    mesh.emplace_back(glm::vec3(e1, b2, 0.0f), glm::vec2(te1, tb2));
    mesh.emplace_back(glm::vec3(b1, b2, 0.0f), glm::vec2(tb1, tb2));
    mesh.emplace_back(glm::vec3(b1, e2, 0.0f), glm::vec2(tb1, te2));
}
// OpenGL wrappers
#endif
