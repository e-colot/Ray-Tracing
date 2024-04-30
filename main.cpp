#include <iostream>
#include <fstream> 
#include <vector>

#include "map.h"
#include "graphics.h"

int main(int argc, char* argv[]) {
    
    bool ray = false;
    bool optimize = true;
    int optimize_order = 2;

    Graphics g = Graphics("Data Rate");

    Map* m;

    if (EXERCISE) {
        m = new Map(Vector(32, 70), Vector(47, 15), &g);
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
                m->show_data_rate(Vector(12.5f, 4.0f));
            }
        }
    }

    g.start();

    return 0;
}
