#include <iostream>
#include <fstream> 
#include <vector>
#include "map.h"
#include "graphics.h"

#pragma warning(push)
#pragma warning(disable : 4100) // disables the warning bcs it don't use argc and argv (without which SDL don't work)
int main(int argc, char* argv[]) {

    bool ray = false;
    bool dB = true;
    bool optimize = true;
    bool lift = false;
    int optimize_order = 2;

    Graphics* g;

    Map* m;

    TILE_SIZE = 0.5f;

    if (EXERCISE) {
        g = new Graphics("Rays, in V^2");
        m = new Map(g);
        m->show_rays(Vector(32, 70), Vector(47, 15), false);
    }
    else {
        if (ray) {
            g = new Graphics("Rays");
            m = new Map(g, lift);
            m->show_rays(Vector(13.0f, 4.0f), Vector(14.0f, 7.0f), dB);
        }
        else {
            g = new Graphics("Data rate");
            m = new Map(g, lift);
            if (optimize) {
                m->optimize_placement(optimize_order, 0.05f, dB);
            }
            else {
                m->show_data_rate(vectorVect{ Vector(13.0f, 4.05f), Vector(5.8f, 5.0f) }, dB);
            }
        }
    }

    g->start();

    delete m;
    delete g;

    return 0;
}
#pragma warning(pop)
