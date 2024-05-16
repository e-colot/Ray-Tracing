#include <iostream>
#include <fstream> 
#include <vector>
#include "map.h"
#include "graphics.h"

#pragma warning(push)
#pragma warning(disable : 4100) // disables the warning bcs it don't use argc and argv (without which SDL don't work)
int main(int argc, char* argv[]) {

    bool exercise = false;
    bool ray = false;
    bool dB = true;
    bool optimize = false;
    bool lift = false;
    int antenna_number = 2;
    char precision_level = 0; // 0 = fast, 1 = intermediate, 2 = precise
    float tile_size = 0.5f;
    bool power = true;

    CHANGE_TYPE(exercise);

    Graphics* g;

    Map* m;


    if (EXERCISE) {
        g = new Graphics("Rays, in V^2");
        m = new Map(g);
        m->show_rays(Vector(32, 70), Vector(47, 15), tile_size, dB);
    }
    else {
        if (ray) {
            g = new Graphics("Rays");
            m = new Map(g, lift);
            m->show_rays(Vector(13.0f, 4.0f), Vector(14.0f, 7.0f), tile_size, dB);
        }
        else {
            g = new Graphics("Data rate");
            m = new Map(g, lift);
            if (optimize) {
                m->optimize_placement(antenna_number, 0.05f, precision_level, dB);
            }
            else {
                if (power) {
                    m->show_power(vectorVect{ Vector(13.0f, 4.05f), Vector(5.8f, 5.0f) }, dB, tile_size);
                }
                else {
                    m->show_data_rate(vectorVect{ Vector(13.0f, 4.05f), Vector(5.8f, 5.0f) }, dB, tile_size);
                }
            }
        }
    }

    g->start();

    delete m;
    delete g;

    return 0;
}
#pragma warning(pop)
