#include <SFML/Graphics.hpp>
#include<iostream>
#include <fstream>
#include <time.h>
#include <vector>
using namespace sf;
using namespace std;

int field[50][50][50] = { 0 };

int &f(int x, int y, int z) { return field[y + 2][x + 2][z]; }
int &f(Vector3i v) { return f(v.x, v.y, v.z); }


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

// เหลือ เพิ่มถาดใส่ไผ่, หน้าเมนู
int main()
{
    srand(time(0));

    RenderWindow app(VideoMode(740, 570), "Comprxprx");

    Texture t1, t2;
    t1.loadFromFile("C:/Users/r1chwo0n/Documents/project game/tiles1.PNG");
    t2.loadFromFile("C:/Users/r1chwo0n/Documents/project game/BG2.jpg");
    Sprite s(t1), sBackground(t2);
    int w = 48, h = 66; // tiles size
    int stepX = w / 2 - 2, stepY = h / 2 - 2; // เลื่อนไปซ้ายขวา
    float offX = 4.6, offY = 7.1; // z offset
    Vector3i v1, v2;
    vector<Vector3i> moves;

    ////load from file////
    fstream myfile("C:/Users/r1chwo0n/Project game/12 Mahjong Solitaire/files/map.txt");
    for (int y = 0; y < 18; y++)
        for (int x = 0; x < 30; x++)
        {
            char a;  myfile >> a;
            int n = a - '0';
            for (int z = 0; z < n; z++)
                if (f(x - 1, y - 1, z)) f(x - 1, y, z) = f(x, y - 1, z) = 0;
                else f(x, y, z) = 1;
        }

    ////create map//////
    for (int k = 1;; k++)
    {
        vector<Vector3i> opens;
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
        k %= 15;
    }

    for (int z = 0; z < 10; z++)
        for (int y = 0; y < 18; y++)
            for (int x = 0; x < 30; x++) f(x, y, z) *= -1;

    while (app.isOpen())
    {
        Event e;
        while (app.pollEvent(e))
        {
            if (e.type == Event::Closed)
                app.close();

            //move back        
            if (e.type == Event::MouseButtonPressed)
                if (e.key.code == Mouse::Left)
                    for (int z = 0; z < 10; z++)
                    {
                        Vector2i pos = Mouse::getPosition(app) - Vector2i(30, 0); // 30 - desk offset
                        int x = (pos.x - z * offX) / stepX;
                        int y = (pos.y + z * offY) / stepY;

                        for (int i = 0; i < 2; i++)
                            for (int j = 0; j < 2; j++)
                                if (f(x - i, y - j, z) > 0 && isOpen(x - i, y - j, z))
                                    v1 = Vector3i(x - i, y - j, z);

                        if (v1 == v2) continue;

                        int a = f(v1), b = f(v2);
                        if (a == b || (a > 34 && a < 39 && b>34 && b < 39) || (a >= 39 && b >= 39))
                        {
                            f(v1) = f(v2) = -1; // turn the tiles
                        }
                        else // not a matching pair, push the selected tile to moves vector
                        {
                            v2 = v1;
                            moves.push_back(v1);
                            f(v1) *= -1; // turn the tile
                        }
                    }
            vector<Vector3i> keep;

                
        }

        app.clear();
        app.draw(sBackground);
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
                    app.draw(s);
                }

        app.display();
    }

    return 0;
}