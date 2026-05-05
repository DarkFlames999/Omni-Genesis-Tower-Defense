#include "InputHandler.h"

bool InputHandler::isHeldKey(HeldKeyBinding& binding, sf::Event event)
{
    if ((event.type == sf::Event::KeyPressed && event.key.code == binding.key) ||
        (event.type == sf::Event::KeyReleased && event.key.code == binding.key))
    {
        if (!binding.isHolding)
        {
            binding.timer.restart();
        }
    }

    if (event.type == sf::Event::KeyPressed && event.key.code == binding.key)
    {
        binding.isHolding = true;
        if (binding.timer.getElapsedTime().asSeconds() >= binding.timeToHold && binding.isHolding)
        {
            binding.holdAchieved = true;
            return binding.holdAchieved;
        }
    }
    else if (event.type == sf::Event::KeyReleased && event.key.code == binding.key && binding.isHolding)
    {
        binding.isHolding = false;
        binding.holdAchieved = false;
        return binding.holdAchieved;
    }
    return false;
}

bool InputHandler::isDoubleTap(DoubleTapBinding& binding, sf::Event event)
{
    if (event.type == sf::Event::KeyPressed && event.key.code == binding.key)
    {
        if (!binding.firstTap) //If the first tape hasn't occured yet, it registers it as occured, to allow for the second tap to happen.
        {
            binding.firstTap = true;
            binding.tapCount = 1.f;
            binding.timer.restart();
        }
        else if(binding.firstTap && !binding.doubleTapOccured && binding.releaseCount == 1.f)
        {
            binding.tapCount = 2.f;
        }
    }

    if(event.type == sf::Event::KeyReleased && event.key.code == binding.key && binding.firstTap && !binding.doubleTapOccured && binding.releaseCount == 0.f && binding.tapCount == 1.f)
    {
        binding.releaseCount = 1.f;
    }

    if (binding.tapCount >= 2.f && binding.releaseCount >= 1.f && binding.firstTap)
    {

        if(event.type == sf::Event::KeyReleased && event.key.code == binding.key)
        {
            binding.releaseCount = 2.f;
            binding.doubleTapOccured = true;
            if(binding.doubleTapOccured && binding.releaseCount >= 2.f) {return binding.doubleTapOccured;}
        }
    }

    if ((binding.timer.getElapsedTime().asSeconds() > binding.timeToDoubleTap) || (binding.firstTap && binding.doubleTapOccured && binding.releaseCount >= 2.f))// resets if threshhold isn't met in time
    {
        binding.firstTap = false;
        binding.doubleTapOccured = false;
        binding.tapCount = 0.f;
        binding.releaseCount = 0.f;
        return false;
    }
    return false;
}

void InputHandler::ImplementHeldKey(sf::Keyboard::Key Key, float TimeToHold)
{
    HeldKeyBinding newHeldKeyBinding;
    newHeldKeyBinding.key = Key;
    newHeldKeyBinding.timeToHold = TimeToHold;
    mHeldKeyBindings.push_back(newHeldKeyBinding);
}

void InputHandler::ImplementDoubleTap(sf::Keyboard::Key Key, float TimeToDoubleTap)
{
    DoubleTapBinding newDoubleTapBinding;
    newDoubleTapBinding.key = Key;
    newDoubleTapBinding.timeToDoubleTap = TimeToDoubleTap;
    mDoubleTapBindings.push_back(newDoubleTapBinding);
}
    
void InputHandler::update(sf::Event event)
{
    for (HeldKeyBinding& heldKeyBinding : mHeldKeyBindings)
    {
        isHeldKey(heldKeyBinding, event);
    }
    for (DoubleTapBinding& doubleTapBinding : mDoubleTapBindings)
    {
        isDoubleTap(doubleTapBinding, event);
    }
}