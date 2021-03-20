#include "scrolllist.hpp"

UI::ScrollList::ScrollList(const unsigned id,
                           const glm::ivec2& size,
                           const glm::ivec2& position)
    : UI::GenericComponent{COMPONENT_SCROLL_LIST, id, position, size},
      UI::ComponentList{}
{}

UI::ScrollList::~ScrollList()
{}

void UI::ScrollList::updateComponents()
{
    
}

void UI::ScrollList::update(const Camera& camera, const EventWrapper& events)
{

}

void UI::ScrollList::draw(const Graphics& graphics, const Transform& transform) const noexcept
{
    
}
