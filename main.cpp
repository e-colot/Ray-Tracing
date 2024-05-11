#include <iostream>
#include <fstream> 
#include <vector>
#include "map.h"
#include "graphics.h"

int main(int argc, char* argv[]) {

    bool ray = false;
    bool dB = false;
    bool optimize = true;
    int optimize_order = 2;

    Graphics* g;

    Map* m;

    TILE_SIZE = 0.1f;

    if (EXERCISE) {
        g = new Graphics("Rays, in V^2");
        m = new Map(g);
        m->show_rays(Vector(32, 70), Vector(47, 15), false);
    }
    else {
        if (ray) {
            g = new Graphics("TODO");
            m = new Map(g);
            m->show_rays(Vector(9.5f, 6.9f), Vector(11.1f, 4.1f), dB);
        }
        else {
            g = new Graphics("Data rate");
            m = new Map(g);
            if (optimize) {
                m->optimize_placement(optimize_order, 0.05f, dB);
            }
            else {
                //m->show_data_rate(vectorVect{ Vector(12.5f, 4.0f), Vector(2.0f, 2.5f) }, dB);
                m->show_data_rate(vectorVect{ Vector(9.4f, 7.0f) }, dB);
            }
        }
    }

    g->start();

    delete m;
    delete g;

    return 0;
}
