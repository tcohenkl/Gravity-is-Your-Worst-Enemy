/* 
 * File:   collision.cpp
 * Author: Nick (original version), ahnonay (SFML2 compatibility), Paweł Syska (C++17 refactor + naming convention)
 */
#include <SFML/Graphics.hpp>
#include <map>
#include <vector>
#include <array>
#include "Collision.h"

namespace Collision
{
    using TextureMask = std::vector<sf::Uint8>;

    static sf::Uint8 getPixel(const TextureMask& mask, const sf::Texture& tex, uint32_t x, uint32_t y) {
        if (x > tex.getSize().x || y > tex.getSize().y)
            return 0;

        return mask[x + y * tex.getSize().x];
    }

    class BitmaskRegistry
    {
    public:
        auto& create(const sf::Texture& tex, const sf::Image& img) {
            auto mask = TextureMask(tex.getSize().y * tex.getSize().x);

            for (uint32_t y = 0; y < tex.getSize().y; ++y)
            {
                for (uint32_t x = 0; x < tex.getSize().x; ++x)
                    mask[x + y * tex.getSize().x] = img.getPixel(x, y).a;
            }

            // store and return ref to the mask
            return (bitmasks[&tex] = std::move(mask));
        }

        auto& get(const sf::Texture& tex) {
            auto pair = bitmasks.find(&tex);
            if (pair == bitmasks.end())
            {
                return create(tex, tex.copyToImage());
            }

            return pair->second;
        }

    private:
        std::map<const sf::Texture*, TextureMask> bitmasks;
    };

    // Gets global instance of BitmaskRegistry.
    static BitmaskRegistry& bitmasks() {
        static BitmaskRegistry instance;
        return instance;
    }

    bool pixelPerfectTest(const sf::Sprite& sprite1, const sf::Sprite& sprite2, sf::Uint8 alphaLimit) {
        sf::FloatRect intersection;
        if (!sprite1.getGlobalBounds().intersects(sprite2.getGlobalBounds(), intersection))
            return false;

        auto s1SubRect = sprite1.getTextureRect();
        auto s2SubRect = sprite2.getTextureRect();

        auto& mask1 = bitmasks().get(*sprite1.getTexture());
        auto& mask2 = bitmasks().get(*sprite2.getTexture());

        // Loop through our pixels
        for (auto i = intersection.left; i < intersection.left + intersection.width; ++i) {
            for (auto j = intersection.top; j < intersection.top + intersection.height; ++j) {

                auto s1v = sprite1.getInverseTransform().transformPoint(i, j);
                auto s2v = sprite2.getInverseTransform().transformPoint(i, j);

                // Make sure pixels fall within the sprite's subrect
                if (s1v.x > 0 && s1v.y > 0 && s2v.x > 0 && s2v.y > 0 &&
                    s1v.x < s1SubRect.width && s1v.y < s1SubRect.height &&
                    s2v.x < s2SubRect.width && s2v.y < s2SubRect.height) {

                        if (getPixel(mask1, *sprite1.getTexture(), (int)(s1v.x) + s1SubRect.left, (int)(s1v.y) + s1SubRect.top) > alphaLimit &&
                            getPixel(mask2, *sprite2.getTexture(), (int)(s2v.x) + s2SubRect.left, (int)(s2v.y) + s2SubRect.top) > alphaLimit)
                            return true;

                }
            }
        }
        return false;
    }


    bool singlePixelTest(const sf::Sprite& sprite, sf::Vector2f& mousePosition, sf::Uint8 alphaLimit) {
		if (!sprite.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
            return false;
    
		auto subRect = sprite.getTextureRect();
		auto& mask = bitmasks().get(*sprite.getTexture());

		auto sv = sprite.getInverseTransform().transformPoint(mousePosition.x, mousePosition.y);
		// Make sure pixels fall within the sprite's subrect
		if (sv.x > 0 && sv.y > 0 && sv.x < subRect.width && sv.y < subRect.height) {
			return getPixel(mask, *sprite.getTexture(), static_cast<int>(sv.x) + subRect.left, static_cast<int>(sv.y) + subRect.top) > alphaLimit;
		}
		return false;
	}
}
