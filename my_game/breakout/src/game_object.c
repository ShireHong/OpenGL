#include "game_object.h"

GameObject::GameObject()
	:position(0, 0), size(1, 1), velocity(0.0f), color(1.0f), rotation(0.0f), sprite(), issolid(false), destroyed(false)
{

}

GameObject::GameObject(glm::vec2 pos, glm::vec2 sz, texture2D sprite, glm::vec3 clr, glm::vec2 vel) 
    : position(pos), size(sz), velocity(vel), color(clr), rotation(0.0f), sprite(sprite), issolid(false), destroyed(false) { }

void GameObject::Draw(SpriteRenderer &renderer)
{
    renderer.DrawSprite(this->sprite, this->position, this->size, this->rotation, this->color);
}