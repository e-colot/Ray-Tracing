#pragma once

#include "ray.h"
#include <vector>

class Antenna;
class RealAntenna;
class VirtualAntenna; 

using antennaVect = std::vector<Antenna*>;
using rayVect = std::vector<const Ray*>;

class Antenna {

    // ---------- CONSTRUCTORS ----------

public:
    Antenna();
    Antenna(const Vector& position, RealAntenna* source, const Wall* w);

    // ---------- DESTRUCTORS ----------

public:
    virtual ~Antenna() = default;

    // ---------- ACCESSORS ----------

public:
    const Vector get_pos() const;
    const Wall* get_wall() const;
    RealAntenna* get_src() const;

    // ---------- METHODS ----------

public:
    void create_ray(const Antenna* receiver, const wallVect& all_walls);
protected:
    virtual void add_ray(const Ray* ray_to_add) = 0;

    // ---------- ATTRIBUTES ----------

protected:
    const Vector pos;
    RealAntenna* src;
    const Wall* wall;

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
    const rayVect get_rays() const;
    double get_min_attenuation() const;
    double get_max_attenuation() const;

    // ---------- MUTATORS ----------

public :
    void set_min_attenuation(double min_att);
    void set_max_attenuation(double max_att);

    // ---------- METHODS ----------

public:
    void virtualize(const Wall* wall_to_bounce);
    double get_binary_rate() const;
    void reset(); // allows the antenna to start again calculating rays and the binary rate
private:
    void add_ray(const Ray* ray_to_add) override;

    // ---------- ATTRIBUTES ----------

private:
    antennaVect virtual_network;
    rayVect rays;
    double emission_factor;
    double min_attenuation;
    double max_attenuation;

};




class VirtualAntenna : public Antenna {

    // ---------- CONSTRUCTORS ----------

public:
    VirtualAntenna();
    VirtualAntenna(RealAntenna* realAntenna, const Wall* reflectedWall);

    // ---------- DESTRUCTORS ----------

public:
    ~VirtualAntenna() override = default;

    // ---------- METHODS ----------

private:
    void add_ray(const Ray* ray_to_add) override;

};
