#pragma once

#include "ray.h"
#include <vector>

class Antenna;
class RealAntenna;
class VirtualAntenna; 

using antennaVect = std::vector<Antenna*>;
using rayVect = std::vector<Ray*>;

class Antenna {

    // ---------- CONSTRUCTORS ----------

public:
    Antenna();

    // ---------- DESTRUCTORS ----------

public:
    virtual ~Antenna() = default;

    // ---------- ACCESSORS ----------

public:
    Vector get_pos() const;
    Wall* get_wall() const;
    RealAntenna* get_src() const;

    // ---------- METHODS ----------

public:
    void create_ray(Antenna* receiver, const wallVect& all_walls);
protected:
    virtual void add_ray(Ray* ray_to_add) = 0;

    // ---------- ATTRIBUTES ----------

protected:
    Vector pos;
    RealAntenna* src;
    Wall* wall;

};




class RealAntenna : public Antenna {
    friend VirtualAntenna; // allows a VirtualAntenna to add a ray to a RealAntenna

    // ---------- CONSTRUCTORS ----------

public:
    RealAntenna();
    RealAntenna(const Vector& position);

    // ---------- DESTRUCTORS ----------

public:
    ~RealAntenna() override;

    // ---------- ACCESSORS ----------

public:
    antennaVect get_virtual_network() const;
    rayVect get_rays() const;
    float get_min_attenuation() const;
    float get_max_attenuation() const;

    // ---------- MUTATORS ----------

public :
    void set_min_attenuation(float min_att);
    void set_max_attenuation(float max_att);

    // ---------- METHODS ----------

public:
    void virtualize(Wall* wall_to_bounce);
    float get_binary_rate() const;
    void reset(); // allows the antenna to start again calculating rays and the binary rate
private:
    void add_ray(Ray* ray_to_add) override;

    // ---------- ATTRIBUTES ----------

private:
    antennaVect virtual_network;
    rayVect rays;
    float emission_factor;
    float min_attenuation;
    float max_attenuation;

};




class VirtualAntenna : public Antenna {

    // ---------- CONSTRUCTORS ----------

public:
    VirtualAntenna();
    VirtualAntenna(RealAntenna* realAntenna, Wall* reflectedWall);

    // ---------- DESTRUCTORS ----------

public:
    ~VirtualAntenna() override = default;

    // ---------- METHODS ----------

private:
    void add_ray(Ray* ray_to_add) override;

};
