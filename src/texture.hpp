#ifndef TEXTURE_HPP
#define TEXTURE_HPP

// Note: Dev is responsible for initializing get_texture;
template<class TextureType>
class Texture {
public:
	// Most commonly a string for the filename of the texture will be here
	Texture() {};
	virtual ~Texture() {};

	virtual TextureType& get_texture() { return texture; }; // MUST BE DEFINED
	virtual void free_texture() {};
protected:
	TextureType texture;
};

#endif
