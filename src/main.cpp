#include <iostream>
#include <vector>
#include "include/map.h"

using namespace std;

string MAP = "/home/liam/Documents/School/5607/hw4/maps/base.txt";

void DrawMap(Map* m) {
    for(int r = 0; r < m->getHeight(); r++) {
        for(int c = 0; c < m->getWidth(); c++) {
            GameObject* o = m->getObject(r, c);
            if (!o)
                cout << " ";
            else
                o->Draw();
        }
        cout << endl;
    }
}

int main() {
    Map* m = new Map(MAP);
    Player* p = m->getPlayer();

    char action;
    do {
        cin >> action;
        ivec2 pos = p->getPosition();
        ivec2 n = pos;
        if (action == 'a')
            n.x -= 1;
        else if (action == 'd')
            n.x += 1;
        else if (action == 's')
            n.y += 1;
        else if (action == 'w')
            n.y += 1;
        GameObject* g = m->getObject(n.x, n.y);
        if (!g) {
            p->setPosition(n);
        }

        DrawMap(m);
    } while (action != 'q');


    return 0;
}
