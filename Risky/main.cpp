#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <time.h>
#include <iostream>
#include "ResourcePath.hpp"

using namespace sf;
using namespace std;

int field[50][50][50] = { 0 };

int& f(int x, int y, int z) { return field[y + 2][x + 2][z]; }
int& f(Vector3i v) { return f(v.x, v.y, v.z); }


bool isOpen(int x, int y, int z)
{
    for (int i = -1; i <= 1; i++)
        for (int j = -1; j <= 1; j++)
            if (f(x + 2, y + i, z) > 0 && f(x - 2, y + j, z) > 0) return 0;

    for (int i = -1; i <= 1; i++)
        for (int j = -1; j <= 1; j++)
            if (f(x + i, y + j, z + 1) > 0) return 0;

    return 1;
}

int main()
{
    srand(time(0));

    sf::RenderWindow window(sf::VideoMode(740, 700), "Comprxprx");
    
    window.setFramerateLimit(15);

    sf::Font font;
    if (!font.loadFromFile("/Users/tony/Downloads/Arial/arialn/Arialn.ttf"))
    {
        return 1;
    }
    
    // Header logo
    RectangleShape Headerlogo;
    Headerlogo.setPosition(150, 10);
    Headerlogo.setSize(Vector2f(429,270));
    Texture header_pic;
    header_pic.loadFromFile("/Users/tony/Desktop/riskyy/GameResources/HeaderLogo.png");
    Headerlogo.setTexture(&header_pic);
    
    // Menuscreen Background
    RectangleShape game_background;
    game_background.setSize(Vector2f(750, 540));
    Texture game_background_pic;
    game_background_pic.loadFromFile("/Users/tony/Desktop/riskyy/GameResources/background.png");
    game_background.setTexture(&game_background_pic);

    
    // Sprites
    Sprite cat2;
    Texture cat_2;
    int xpos_cat2 = 0;
    cat_2.loadFromFile("/Users/tony/Desktop/riskyy/GameResources/cat2.png");
    cat2.setTexture(cat_2);
    cat2.setTextureRect(IntRect(0,0,220.8, 168));
    cat2.setPosition(50, 300);
    
    // Start button
    sf::RectangleShape startButton(sf::Vector2f(197.50, 47));
    startButton.setPosition(270, 350);
    Texture start_button;
    start_button.loadFromFile("/Users/tony/Desktop/riskyy/GameResources/startbutton.png");
    startButton.setTexture(&start_button);
    
    // How to play button ** Add texture
    RectangleShape htpButton(Vector2f(187.50, 37));
    htpButton.setPosition(270, 400);
    Texture htp_button;
    htp_button.loadFromFile("/Users/tony/Desktop/riskyy/GameResources/howtoplay.png");
    htpButton.setTexture(&htp_button);
    
    
    // Quit button
    sf::RectangleShape quitButton(sf::Vector2f(200, 50));
    quitButton.setPosition(270, 450);
    quitButton.setFillColor(Color::Blue);
    
    
    
    // Setting button

    // Game's zone
    Texture t1, t2;
        t1.loadFromFile(resourcePath() + "tiles.png");
        t2.loadFromFile(resourcePath() + "bg.jpeg");
        Sprite s(t1), sBackground(t2);
        int w = 48, h = 66;
        int stepX = w / 2 - 2, stepY = h / 2 - 2;
        float offX = 4.6, offY = 7.1; // z offset 4.6 def
        Vector3i v1, v2;
        std::vector<Vector3i> moves;
    ////load from file////
      std::fstream myfile(resourcePath() + "map.txt");
      for (int y = 0; y < 18; y++)
          for (int x = 0; x < 30; x++)
          {
              char a;
              myfile >> a;
              int n = a - '0';
              for (int z = 0; z < n; z++)
                  if (f(x - 1, y - 1, z))
                      f(x - 1, y, z) = f(x, y - 1, z) = 0;
                  else
                      f(x, y, z) = 1;
          }

      ////create map//////
      for (int k = 1;; k++)
      {
          std::vector<Vector3i> opens;
          for (int z = 0; z < 10; z++)
              for (int y = 0; y < 18; y++)
                  for (int x = 0; x < 30; x++)
                      if (f(x, y, z) > 0 && isOpen(x, y, z)) opens.push_back(Vector3i(x, y, z));

          int n = opens.size();
          if (n < 2) break;
          int a = 0, b = 0;
          while (a == b) { a = rand() % n; b = rand() % n; }
          f(opens[a]) = -k;  if (k > 34) k++;
          f(opens[b]) = -k;
          k %= 14;
      }

      for (int z = 0; z < 10; z++)
          for (int y = 0; y < 18; y++)
              for (int x = 0; x < 30; x++) f(x, y, z) *= -1;



    
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                if (startButton.getGlobalBounds().contains(mousePos))
                {
                    // Transition to game loop
                    window.clear();
                    window.close();
                    sf::RenderWindow gameWindow(sf::VideoMode(740, 570), "Comprxprx");

                    while (gameWindow.isOpen())
                    {
                        // Game's source code
                        Event e;
                               while (gameWindow.pollEvent(e))
                               {
                                   if (e.type == Event::Closed)
                                       gameWindow.close();

                                   if (e.type == Event::MouseButtonPressed)
                                       if (e.key.code == Mouse::Left)
                                           for (int z = 0; z < 10; z++)
                                           {
                                               Vector2i pos = Mouse::getPosition(gameWindow) - Vector2i(30, 0); // 30 - desk offset
                                               int x = (pos.x - z * offX) / stepX;
                                               int y = (pos.y + z * offY) / stepY;

                                               for (int i = 0; i < 2; i++)
                                                   for (int j = 0; j < 2; j++)
                                                       if (f(x - i, y - j, z) > 0 && isOpen(x - i, y - j, z))
                                                           v1 = Vector3i(x - i, y - j, z);

                                               if (v1 == v2) continue;

                                               int a = f(v1), b = f(v2);
                                               if (a == b)
                                               {
                                                   f(v1) *= -1; moves.push_back(v1);
                                                   f(v2) *= -1; moves.push_back(v2);
                                               }
                                               v2 = v1;
                                           }
                               }

                               gameWindow.clear();
                               gameWindow.draw(sBackground);
                               for (int z = 0; z < 10; z++)
                                   for (int x = 30; x >= 0; x--)
                                       for (int y = 0; y < 18; y++)
                                       {
                                           int k = f(x, y, z) - 1;
                                           if (k < 0) continue;
                                           s.setTextureRect(IntRect(k * w, 0, w, h));
                                           if (isOpen(x, y, z)) s.setTextureRect(IntRect(k * w, h, w, h));
                                           s.setPosition(x * stepX + z * offX, y * stepY - z * offY);
                                           s.move(30, 0); //desk offset
                                           gameWindow.draw(s);
                                       }

                               gameWindow.display();
                        sf::Event gameEvent;
                        while (gameWindow.pollEvent(gameEvent))
                        {
                            if (gameEvent.type == sf::Event::Closed)
                            {
                                gameWindow.close();
                            }
                        }
                    }
                }
                else if (quitButton.getGlobalBounds().contains(mousePos))
                {
                    window.close();
                }
            }
        }

        window.draw(game_background);
        window.draw(Headerlogo);
        window.draw(htpButton);
        window.draw(startButton);
        window.draw(quitButton);
        window.draw(cat2);
        xpos_cat2 += 220.8; // ตั้งไซส์สไปรท์ให้ขนาดเท่ากันแล้วปรับ xpos ให้เท่ากันหมด
        if (xpos_cat2 >= 1104) xpos_cat2 = 0;
        cat2.setTextureRect(IntRect(xpos_cat2, 0, 220.8, 168));
        
        window.display();
    }

    return 0;
}


