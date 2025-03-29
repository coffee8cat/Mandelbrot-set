#include <SFML/Graphics.hpp>
#include <complex>
#include <iostream>

const int WIDTH = 800;
const int HEIGHT = 600;
const int MAX_ITER = 255;
const int R2_MAX = 10;

float zoom = 1.0f;
float offsetX = 0.0f, offsetY = 0.0f;

void computeMandelbrot(std::vector<sf::Uint8>& pixels) {
    for (int iy = 0; iy < HEIGHT; iy++) {
        for (int ix = 0; ix < WIDTH; ix++) {
            float x0 = (ix - WIDTH / 2.0f) * 3.0f / (WIDTH * zoom) + offsetX;
            float y0 = (iy - HEIGHT / 2.0f) * 3.0f / (HEIGHT * zoom) + offsetY;
            float x = x0, y = y0;

            size_t N = 0;
            for (; N < MAX_ITER; N++) {
                float x2 = x * x, y2 = y * y, xy = x * y, r2 = x2 + y2;
                if (r2 > R2_MAX) break;

                x = x2 - y2 + x0;
                y = xy + xy + y0;
            }
            // (20, 100, 170)       - set (N = MAX_ITER = 254, N % 2 == 0)
            // (130, 60, 220)      - N % 2 == 0
            // (170, 20, 100)       - N % 2 == 1
            // Градиент бордово-синий
            float t = static_cast<float>(N) / MAX_ITER; // Нормализуем [0,1]
            sf::Uint8 R = static_cast<sf::Uint8>(130 - 110 * t + 40  * (N % 2) * (1 - t));  // От 100 до 255 (бордовый)
            sf::Uint8 G = static_cast<sf::Uint8>(60  + 40 * t  - 40  * (N % 2) * (1 - t));   // Почти отсутствует (темные оттенки)
            sf::Uint8 B = static_cast<sf::Uint8>(220 - 50 * t  - 120 * (N % 2) * (1 - t));  // От 255 до 100 (синий → фиолетовый)

            int index = (iy * WIDTH + ix) * 4;
            pixels[index]     = R;  // Красный
            pixels[index + 1] = G;  // Зеленый
            pixels[index + 2] = B;  // Синий
            pixels[index + 3] = 255; // Альфа (прозрачность)
        }
    }
}


int main() {
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
            // Управление камерой
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::W) offsetY -= 0.1f / zoom;
                if (event.key.code == sf::Keyboard::S) offsetY += 0.1f / zoom;
                if (event.key.code == sf::Keyboard::A) offsetX -= 0.1f / zoom;
                if (event.key.code == sf::Keyboard::D) offsetX += 0.1f / zoom;
                if (event.key.code == sf::Keyboard::Q) zoom *= 1.1f;
                if (event.key.code == sf::Keyboard::E) zoom /= 1.1f;
            }
        }

        computeMandelbrot(pixels);
        texture.update(pixels.data());

        window.clear();
        window.draw(sprite);
        window.display();

        std::cout << "FPS: " << 1.0f / clock.restart().asSeconds() << "\r";
    }
    return 0;
}
