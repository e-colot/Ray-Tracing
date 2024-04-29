#include <iostream>
#include <fstream> 
#include <vector>

#include "map.h"
#include "graphics.h"

int main(int argc, char* argv[]) {
    
    bool ray = false;

    Graphics g = Graphics("Data Rate");

    Map* m;

    if (ray) {
        if (EXERCISE) {
            m = new Map(Vector(32, 70), Vector(47, 15), &g);
        }
        else {
            m = new Map(Vector(10.9f, 3.9f), Vector(11.1f, 4.1f), &g);
        }
        m->show_rays();
    }
    else {
        m = new Map(&g);
        // m->show_data_rate(Vector(11.4f, 4.4f));
        m->optimize_placement(2);
    }

    g.start();

    return 0;
}
