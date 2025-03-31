#include "graphics_mandelbrot.h"
#include "params.h"

void launch_Mandelbrot() {
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Mandelbrot Set");
    std::vector<sf::Uint8> pixels(WIDTH * HEIGHT * 4);
    sf::Texture texture;
    texture.create(WIDTH, HEIGHT);
    sf::Sprite sprite(texture);

    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            // Camera movement
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::W) offsetY -= 0.1 / zoom;
                if (event.key.code == sf::Keyboard::S) offsetY += 0.1 / zoom;
                if (event.key.code == sf::Keyboard::A) offsetX -= 0.1 / zoom;
                if (event.key.code == sf::Keyboard::D) offsetX += 0.1 / zoom;
                if (event.key.code == sf::Keyboard::E) zoom *= 1.1;
                if (event.key.code == sf::Keyboard::Q) zoom /= 1.1;
            }
        }
        SIMD_m256s_computeMandelbrot(pixels);

        texture.update(pixels.data());

        window.clear();
        window.draw(sprite);
        window.display();

        printf("FPS: %lf \r", 1.0 / clock.restart().asSeconds());
    }
}
