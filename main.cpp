#include <iostream>
#include <fstream> 
#include <vector>
#include "map.h"
#include "graphics.h"

int main(int argc, char* argv[]) {

    bool ray = true;
    bool optimize = false;
    int optimize_order = 2;

    Graphics g = Graphics("Data Rate");

    Map* m;

    if (EXERCISE) {
        m = new Map(Vector(32, 70), Vector(47, 15), &g);
        m->show_rays();
    }
    else {
        if (ray) {
            m = new Map(Vector(9.5f, 6.9f), Vector(11.1f, 4.1f), &g);
            m->show_rays();
        }
        else {
            m = new Map(&g);
            if (optimize) {
                m->optimize_placement(optimize_order);
            }
            else {
                m->show_data_rate(Vector(12.0f, 5.0f), Vector(5.5f, 5.0f), true);
            }
        }
    }

    g.start();

    return 0;
}
