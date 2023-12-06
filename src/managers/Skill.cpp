#include "Skill.h"

#include "../managers/SoundManager.h"
#include "../managers/FontManager.h"

#include "../utils/Mouse.h"
#include "../utils/GraphicsOperations.h"

#include "../systems/ProceduralGenerationSystem.h"
#include "../systems/ExperienceSystem.h"

#include "../components/tagComponents.h"

void Skill::initialize()
{
    this->loadTexturesIntoManager();
    this->initializeIconSprite();
    this->initializeText();

    this->addCircleSegment();
}

void Skill::initializeText()
{
    this->nameText = this->getConfiguredText(this->name, NAME_CHARACTER_SIZE);
    this->centerText(this->nameText);
    this->nameText.setPosition(this->iconSprite.getPosition().x, this->iconSprite.getPosition().y + MARGIN);

    this->addDescriptionLine(this->descriptions[0]);
}

void Skill::addCircleSegment()
{
    auto angle = 360.f / this->maxLevel;
    this->circleSegments.push_back(std::make_unique<GUICircleSegment>(
        this->iconPosition,
        90.f,
        this->currentLevel * angle,
        angle + (this->currentLevel * angle),
        SEGMENT_THICKNESS,
        this->getStoneColor()
    ));
}   

void Skill::addActiveStars()
{
    for (auto& star : this->stars)
    {
        auto newStar = star->getStar();
        this->activeStars.push_back(std::make_unique<GUIStar>(newStar));
    }

    this->stars.clear();
}

void Skill::initStarsForSkill()
{
    unsigned int numStars = ProceduralGenerationSystem::GetRandomNumber(10, 20);

    for (int i = 0; i < numStars; i++)
    {
        bool flicker = ProceduralGenerationSystem::GetRandomNumber(0, 2);
        sf::Color color = sf::Color::White;

        if (i == 0 || ProceduralGenerationSystem::GetRandomNumber(1,4) == 4)
        {
            color = this->getStoneColor();
        }

        GUIStar star(
            this->calculateStarPosition(),
            color,
            ProceduralGenerationSystem::GetRandomNumber(1.f, 4.f),
            !flicker,
            ProceduralGenerationSystem::GetRandomNumber(2.f, 10.f)
        );

        this->stars.push_back(std::make_unique<GUIStar>(star));
    }

    this->isStarExists = true;
}

void Skill::update()
{
    this->updateHoverState();

    if (!this->isMaxLevelReached)
    {
        
    }

    if (!this->isMaxLevelReached && this->dialogBox.getState() == GUIDialogBoxState::Hidden)
    {
        if (this->hover)
        {
            if (!this->isStarExists && !this->isMaxLevelReached)
            {
                this->initStarsForSkill();
                SoundManager::getInstance().playSound("SkillHover");
            }

            this->iconSprite.setTexture(TextureManager::getInstance().getTexture(std::string(this->iconTextureName + "Hover")));
            
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                if (ExperienceSystem::getSkillPoints(this->registry) > 0)
                {
                    this->dialogBox.setMessage(this->getMessagesForRequirements(this->requirements[this->currentLevel]));

                    this->dialogBox.setType(GUIDialogBoxType::YesNo);
                    this->dialogBox.setState(GUIDialogBoxState::Idle);
                    this->dialogBox.setTarget(this->name);
                }
                else
                {
                    SoundManager::getInstance().playSound("Error");
                    this->dialogBox.setMessage({"You don't have any Skill Points"});
                    this->dialogBox.setType(GUIDialogBoxType::Ok);
                    this->dialogBox.setState(GUIDialogBoxState::Idle);
                    this->dialogBox.setTarget(this->name);
                }
            }
        }
        else
        {
            if (this->isStarExists && !this->isMaxLevelReached)
            {
                this->isStarExists = false;
                this->stars.clear();
            }

            this->iconSprite.setTexture(TextureManager::getInstance().getTexture(this->iconTextureName));
        }
    }

    if (this->dialogBox.getState() != GUIDialogBoxState::Hidden && this->dialogBox.getTarget() == this->name)
    {
        if (this->dialogBox.getState() == GUIDialogBoxState::Yes)
        {
            this->dialogBox.setState(GUIDialogBoxState::Hidden);

            auto& playerStones = this->registry.get<StoneInventory>(this->registry.view<Player>().front());

            if (this->requirements[this->currentLevel] == RequirementType::OrangeStone)
            {
                if (playerStones.orangeStones > 0)
                    playerStones.orangeStones--;
                else
                {
                    this->dialogBox.setMessage({"You don't have the Orange Stone to unlock " + this->name});
                    this->dialogBox.setType(GUIDialogBoxType::Ok);
                    this->dialogBox.setState(GUIDialogBoxState::Idle);
                    return;
                }
            }
            else if (this->requirements[this->currentLevel] == RequirementType::GreenStone)
            {
                if (playerStones.greenStones > 0)
                    playerStones.greenStones--;
                else
                {
                    this->dialogBox.setMessage({"You don't have the Green Stone to unlock " + this->name});
                    this->dialogBox.setType(GUIDialogBoxType::Ok);
                    this->dialogBox.setState(GUIDialogBoxState::Idle);
                    return;
                }
            }
            else if (this->requirements[this->currentLevel] == RequirementType::YellowStone)
            {
                if (playerStones.orangeStones > 0 && playerStones.greenStones > 0)
                {
                    playerStones.orangeStones--;
                    playerStones.greenStones--;
                }
                else
                {
                    this->dialogBox.setMessage({"You don't have the Orange and Green Stone to unlock " + this->name});
                    this->dialogBox.setType(GUIDialogBoxType::Ok);
                    this->dialogBox.setState(GUIDialogBoxState::Idle);
                    return;
                }
            }

            SkillSystem::modifySkill(this->registry, this->onActivateFunctions[this->currentLevel].first, this->onActivateFunctions[this->currentLevel].second);

            if (this->currentLevel == 0)
            {
                SkillManager::getInstance(this->window, this->registry).unlockSkills(this->skillsToUnlock);
            }

            this->circleSegments.at(this->currentLevel)->setState(CircleSegmentState::Active);

            ExperienceSystem::removeSkillPoint(this->registry);
            SoundManager::getInstance().playSound("SkillUnlock");

            this->addActiveStars();

            this->currentLevel++;

            if (this->currentLevel < this->maxLevel)
                this->addCircleSegment();

            if (this->currentLevel >= this->maxLevel)
            {
                if (!this->multiLevel)
                    if (this->descriptionTexts.size() > 1)
                        this->descriptionTexts.erase(this->descriptionTexts.begin(), this->descriptionTexts.begin() + 1);

                this->isMaxLevelReached = true;
                this->iconSprite.setTexture(TextureManager::getInstance().getTexture(std::string(this->iconTextureName + "Active")));
                this->descriptionTexts[this->descriptionTexts.size() - 1].setFillColor(sf::Color::White);
            }
            else
            {
                if (!this->multiLevel)
                    if (this->descriptionTexts.size() > 1)
                        this->descriptionTexts.erase(this->descriptionTexts.begin(), this->descriptionTexts.begin() + 1);

                this->descriptionTexts[this->descriptionTexts.size() - 1].setFillColor(sf::Color::White);

                this->addDescriptionLine(this->descriptions[this->currentLevel]);
            }
        }
        else if (this->dialogBox.getState() == GUIDialogBoxState::No)
        {
            this->dialogBox.setState(GUIDialogBoxState::Hidden);
        }
        else if (this->dialogBox.getState() == GUIDialogBoxState::Ok)
        {
            this->dialogBox.setState(GUIDialogBoxState::Hidden);
        }
    }

    //TODO: add dt
    for (auto& circleSegment : this->circleSegments)
    {
        circleSegment->update(0.016f);
    }

    for (auto& star : this->stars)
    {
        star->update(0.016f);
    }
}

void Skill::draw()
{
    for (auto& star : this->stars)
        this->window.draw(*star);

    this->window.draw(this->iconSprite);

    this->window.draw(this->nameText);

    sf::View view = window.getView();

    this->window.setView(this->window.getDefaultView());

    if (this->hover)
        for (auto& descriptionText : this->descriptionTexts)
            this->window.draw(descriptionText);

    this->window.setView(view);

    for (auto& circleSegment : this->circleSegments)
        this->window.draw(*circleSegment);
}

void Skill::loadTexturesIntoManager()
{
    TextureManager::getInstance().loadTexture(this->iconTextureName, ASSETS_PATH + std::string("skillAssets/") + this->iconTextureName + ".png");
    TextureManager::getInstance().loadTexture(this->iconTextureName + "Hover", ASSETS_PATH + std::string("skillAssets/") + this->iconTextureName + "Hover.png");
    TextureManager::getInstance().loadTexture(this->iconTextureName + "Active", ASSETS_PATH + std::string("skillAssets/") + this->iconTextureName + "Active.png");
}

void Skill::initializeIconSprite()
{
    this->iconSprite = CreateSprite(this->iconTextureName);
    this->iconSprite.setPosition(this->iconPosition);
}

sf::Text Skill::getConfiguredText(std::string string, unsigned int characterSize, sf::Color textColor)
{
    sf::Text text;
    text.setFont(FontManager::getInstance().getFont("font"));
    text.setString(string);
    text.setCharacterSize(characterSize);
    text.setFillColor(textColor);
    text.setOutlineThickness(1);
    text.setOutlineColor(sf::Color::Black);
    return text;
}

void Skill::centerText(sf::Text& text)
{
    text.setOrigin(
        text.getGlobalBounds().width / 2.f,
        text.getGlobalBounds().height / 2.f
    );
    text.setPosition(
        this->window.getSize().x / 2.f,
        this->window.getSize().y / 2.f
    );
}

sf::Color Skill::getStoneColor()
{
    if (this->requirements[this->currentLevel] == RequirementType::OrangeStone)
        return ORANGE_STONE_COLOR;
    else if (this->requirements[this->currentLevel] == RequirementType::GreenStone)
        return GREEN_STONE_COLOR;
    else if (this->requirements[this->currentLevel] == RequirementType::YellowStone)
        return YELLOW_STONE_COLOR;
    else
        return DEFAULT_STONE_COLOR;
}

sf::Vector2f Skill::calculateStarPosition()
{
    float offset = !ProceduralGenerationSystem::GetRandomNumber(0,2) ? ProceduralGenerationSystem::GetRandomNumber(1.1f, 2.f) : 1.f;

    return sf::Vector2f(
                this->iconPosition.x + (ProceduralGenerationSystem::GetRandomNumber(-300.f, 300.f) * offset),
                this->iconPosition.y + (ProceduralGenerationSystem::GetRandomNumber(-300.f, 300.f) * offset)
            );
}

void Skill::addDescriptionLine(const std::string descriptionTextLine)
{
    this->descriptionTexts.push_back(this->getConfiguredText(descriptionTextLine, DESCRIPTION_CHARACTER_SIZE, INACTIVE_DESCRIPTION_COLOR));
    this->centerText(this->descriptionTexts[this->descriptionTexts.size() - 1]);

    for (int i = 0; i < this->descriptionTexts.size(); i++)
    {
        this->descriptionTexts[this->descriptionTexts.size() - 1 - i].setPosition(
            this->window.getSize().x / 2.f,
            this->window.getSize().y - MARGIN - (this->descriptionTexts[this->descriptionTexts.size() - 1 - i].getGlobalBounds().height * (i + 1))
        );
    }
}

void Skill::updateHoverState()
{
    if (utils::isMouseOverSprite(this->iconSprite, utils::getMousePositionInWindow(this->window)) && this->dialogBox.getState() == GUIDialogBoxState::Hidden)
    {
        if (this->currentLevel < this->maxLevel)
            this->circleSegments[this->currentLevel]->setState(CircleSegmentState::Hover);
        this->hover = true;
    }
    else
    {
        if (this->currentLevel < this->maxLevel)
            this->circleSegments[this->currentLevel]->setState(CircleSegmentState::Hidden);
        this->hover = false;
    }
}

std::vector<std::string> Skill::getMessagesForRequirements(RequirementType requirementType)
{
    std::vector<std::string> messages;
    messages.push_back("Are you sure you want to unlock " + std::string(this->name + "?"));

    switch (requirementType)
    {
    case RequirementType::None:
        break;
    case RequirementType::OrangeStone:
        messages.insert(messages.begin(), std::string(this->name + " require Orange Stone"));
        break;
    case RequirementType::GreenStone:
        messages.insert(messages.begin(), std::string(this->name + " require Green Stone"));
        break;
    case RequirementType::YellowStone:
        messages.insert(messages.begin(), std::string(this->name + " require Orange and Green Stone"));
        break;
    case RequirementType::RedStone:
        throw std::invalid_argument("Red Stone is not implemented");
        break;
    case RequirementType::BlueStone:
        throw std::invalid_argument("Blue Stone is not implemented");
        break;
    case RequirementType::PowerfulOrangeStone:
        throw std::invalid_argument("Powerful Orange Stone is not implemented");
        break;
    case RequirementType::PowerfulGreenStone:
        throw std::invalid_argument("Powerful Green Stone is not implemented");
        break;
    case RequirementType::PowerfulYelowStone:
        throw std::invalid_argument("Powerful Yellow Stone is not implemented");
        break;
    case RequirementType::PowerfulRedStone:
        throw std::invalid_argument("Powerful Red Stone is not implemented");
        break;
    case RequirementType::PowerfulBlueStone:
        throw std::invalid_argument("Powerful Blue Stone is not implemented");
        break;
    default:
        throw std::invalid_argument("Invalid requirement type");
        break;
    }

    return messages;
}