#include <iostream>
#include<stdexcept>
#include <SFML/Graphics.hpp>
#include <SFML/Config.hpp>
#include<fstream>
#include<memory>
#include <sstream>


struct GameWindowDimension{
    int width{};
    int height{};
    GameWindowDimension() = default;
    GameWindowDimension(const int &width, const int &height): width(width), height(height) {}
};


struct Entity {
    std::unique_ptr<sf::Shape> shape{};
    sf::Color shapeColor{};
    sf::Text label;
    float xVelocity{};
    float yVelocity{};
    float maximumXDirection{};
    float maximumYDirection{};
};


struct GameConfig {
    GameWindowDimension window{};
    std::vector<Entity> entities{};
    sf::Font font{};
    unsigned int fontSize{36};
    sf::Color fontColor{sf::Color::White};
    bool lsdAcidHighMode{false};

};

static bool toBool(const std::string &str) {
    return str == "true" || str == "True" || str == "TRUE" || str == "1";
}

 static bool readConfigFile(GameConfig &parsedConfig, const std::string& configFilePath) {
    try {
        std::ifstream configFile(configFilePath);
        if (configFile.is_open()) {
            std::string line;
            int lineCount = 0;
            while (std::getline(configFile, line)) {
                lineCount++;

                std::istringstream iss(line);
                std::string token;
                if (!(iss >> token)) continue; // Crazy syntax: If the iss >> token fails to extract the word then skip, otherwise extract the token
                if (token == "Window") {
                    int width, height;
                    if (!(iss >> width >> height)) {
                        std::cout<<"Parsing error on line. "<<lineCount<<" Cannot parse token: <"<< token<< ">"<<std::endl;
                        return false;
                    }

                    parsedConfig.window.width = width;
                    parsedConfig.window.height = height;
                    std::cout<<"Processed token : "<<token<<std::endl;

                }

                if (token == "Font") {
                    std::string fontPath; unsigned int fontSize; unsigned int red; unsigned int green; unsigned int blue;
                    if (!(iss >> fontPath >> fontSize >> red >> green >> blue)) {
                        std::cout<<"Parsing error on line. "<<lineCount<<" Cannot parse token: <"<< token<< ">"<<std::endl;
                        throw std::runtime_error ("Check the format/data/spacing etc!");

                    }
                    parsedConfig.font.loadFromFile(fontPath);
                    parsedConfig.fontSize = fontSize;
                    parsedConfig.fontColor = sf::Color(red, green, blue);
                    std::cout<<"Processed token : "<<token<<std::endl;
                }

                if (token == "Circle") {
                    // Get the following
                    // <Token> <Label: Text inside the rendered entity> <width> <height> <x:velocity> <y:velocity> <r> <g> <b> <radius>
                    std::string label; float x, y; float xVelocity, yVelocity; unsigned int red, green, blue; int radius;
                    if (!(iss >> label >> x >> y >> xVelocity >> yVelocity >> red >> green >> blue >> radius)) {
                        std::cout<<"Parsing error on line. "<<lineCount<<" Cannot parse token: <"<< token<< ">"<<std::endl;
                        throw std::runtime_error ("Check the format/data/spacing etc!");
                    }

                    Entity circleEntity;
                    circleEntity.shape = std::make_unique<sf::CircleShape>(radius);
                    circleEntity.shape->setPosition(x, y);
                    circleEntity.label = sf::Text(label, parsedConfig.font, parsedConfig.fontSize);
                    circleEntity.xVelocity = xVelocity;
                    circleEntity.yVelocity = yVelocity;
                    circleEntity.shapeColor = sf::Color(red, green, blue);
                    circleEntity.shape->setFillColor(circleEntity.shapeColor);

                    // Calculate the maximum x and y direction for bounding within game window
                    circleEntity.maximumXDirection = static_cast<float>(parsedConfig.window.width) - static_cast<float>(radius) * 2.0f;
                    circleEntity.maximumYDirection = static_cast<float>(parsedConfig.window.height) - static_cast<float>(radius) * 2.0f;

                    parsedConfig.entities.emplace_back(std::move(circleEntity));
                    std::cout<<"Processed token : "<<token<<std::endl;

                }

                if (token == "Rectangle") {
                    // Get the following
                    // <Token> <Label: Text inside the rendered entity> <width> <height> <x:velocity> <y:velocity> <r> <g> <b> <width> <height>
                    std::string label; float x, y; float xVelocity, yVelocity; unsigned int red, green, blue,width, height;
                    if (!(iss >> label >> x >> y >> xVelocity >> yVelocity >> red >> green >> blue >> width >> height)) {
                        std::cout<<"Parsing error on line. "<<lineCount<<" Cannot parse token: <"<< token<< ">"<<std::endl;
                        throw std::runtime_error ("Check the format/data/spacing etc!");
                    }

                    Entity rectangleEntity;
                    rectangleEntity.shape = std::make_unique<sf::RectangleShape>(sf::Vector2f(width, height));
                    rectangleEntity.shape->setPosition(x, y);
                    rectangleEntity.label = sf::Text(label, parsedConfig.font, parsedConfig.fontSize);
                    rectangleEntity.shapeColor = sf::Color(red, green, blue);
                    rectangleEntity.shape->setFillColor(rectangleEntity.shapeColor);

                    rectangleEntity.xVelocity = xVelocity;
                    rectangleEntity.yVelocity = yVelocity;

                    // Calculate the maximum x and y direction for bounding within game window
                    rectangleEntity.maximumXDirection = static_cast<float>(parsedConfig.window.width) - static_cast<float>(width) * 2.0f;
                    rectangleEntity.maximumYDirection = static_cast<float>(parsedConfig.window.height) - static_cast<float>(height) * 2.0f;


                    parsedConfig.entities.emplace_back(std::move(rectangleEntity));
                    std::cout<<"Processed token : "<<token<<std::endl;
                }

                if(token == "LsdAcidHighMode") {
                    std::string lsdAcidHighMode;
                    if (!(iss >> lsdAcidHighMode )) {
                        std::cout<<"Parsing error on line. "<<lineCount<<" Cannot parse token: <"<< token<< ">"<<std::endl;
                        throw std::runtime_error ("Check the format/data/spacing etc!");
                    }

                    parsedConfig.lsdAcidHighMode = toBool(lsdAcidHighMode);
                    std::cout<<"Processed token : "<<token<<std::endl;
                }


            }
            configFile.close();
        }
        return true;
    }
    catch (const std::exception &e) {
        std::cout<<"Failed to load config file or config file has issues: "<<e.what()<<std::endl;
        return false;
    }

}

static void centerTextInShape(const sf::Shape& shape, sf::Text& text) {
     // Recenter text origin to its visual center
     sf::FloatRect tlb = text.getLocalBounds();
     text.setOrigin(tlb.left + tlb.width / 2.f, tlb.top + tlb.height / 2.f);

     // Put text at shape's global center (works for any transform/origin)
     sf::FloatRect gb = shape.getGlobalBounds();
     text.setPosition(gb.left + gb.width / 2.f, gb.top + gb.height / 2.f);
 }

static void updateAndBounce(Entity& e, const GameWindowDimension& win) {
     // move
     e.shape->move(e.xVelocity, e.yVelocity);

     // bounds after move
     sf::FloatRect gb = e.shape->getGlobalBounds();
     sf::Vector2f pos = e.shape->getPosition();

     // X (independent checks)
     if (gb.left < 0.f) {
         pos.x += -gb.left;                  // clamp inside
         e.xVelocity = -e.xVelocity;
     }
     if (gb.left + gb.width > win.width) {
         float over = (gb.left + gb.width) - win.width;
         pos.x -= over;
         e.xVelocity = -e.xVelocity;
     }

     // Y
     gb = e.shape->getGlobalBounds();        // (optional) refresh
     if (gb.top < 0.f) {
         pos.y += -gb.top;
         e.yVelocity = -e.yVelocity;
     }
     if (gb.top + gb.height > win.height) {
         float over = (gb.top + gb.height) - win.height;
         pos.y -= over;
         e.yVelocity = -e.yVelocity;
     }

     e.shape->setPosition(pos);
 }

// Utility for weird usecases
void centerShapeInGameWindow(sf::Shape& shape, const GameWindowDimension& win) {

    // Get the bounding box of the shape (accounts for size, scale, origin, etc.) Kind of better to use this since every shape has a bounding box around it.
    sf::FloatRect bounds = shape.getGlobalBounds();

    // Compute centered position
    float x = (win.width  / 2.f) - (bounds.width  / 2.f);
    float y = (win.height / 2.f) - (bounds.height / 2.f);

    shape.setPosition(x, y);
}


int main() {

    std::cout<< SFML_VERSION_MAJOR << "." << SFML_VERSION_MINOR << "." << SFML_VERSION_PATCH<<"\n";

    GameConfig parsedConfig{};
    bool parsingResult = readConfigFile(parsedConfig,"resources/config.txt");
    if (parsingResult) {
        std::cout<<"Length of entities: "<<parsedConfig.entities.size()<<std::endl;
    }

     GameWindowDimension windowDimension = GameWindowDimension(parsedConfig.window.width, parsedConfig.window.height);

     sf::RenderWindow window(sf::VideoMode(windowDimension.width, windowDimension.height), "Bouncing Shapes");


    // This is for Testing only!
    std::unique_ptr<sf::Shape> rectangleShape =
         std::make_unique<sf::RectangleShape>(sf::Vector2f(200.f, 200.f));
    rectangleShape->setFillColor(sf::Color::White);

    centerShapeInGameWindow(*rectangleShape, windowDimension);




    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        for (auto &e: parsedConfig.entities) {
            centerTextInShape(*e.shape, e.label);
            updateAndBounce(e, windowDimension);
        }
        if (!parsedConfig.lsdAcidHighMode)
            window.clear();

        for (auto& e : parsedConfig.entities) {

            window.draw(*e.shape);
            window.draw(e.label);
        }

        window.draw(*rectangleShape);
        window.display();
    }
}
