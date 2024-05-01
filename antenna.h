#pragma once

#include "ray.h"
#include <vector>

class Antenna;
class RealAntenna;
class VirtualAntenna; 

using antennaVect = std::vector<Antenna*>;
using rayVect = std::vector<const Ray*>;

class Antenna {
    // Attributes
protected:
    const Vector pos; // Position of the antenna
    RealAntenna* src; // Source antenna
    const Wall* wall; // Wall associated with the antenna

    // Constructors
public:
    Antenna(); // Default constructor
    Antenna(const Vector& position, RealAntenna* source, const Wall* w); // Constructor with position, source, and wall

    // Destructor
public:
    virtual ~Antenna() = default; // Virtual destructor

    // Accessors
public:
    const Vector get_pos() const; // Returns the position of the antenna
    const Wall* get_wall() const; // Returns the wall associated with the antenna
    RealAntenna* get_src() const; // Returns the source antenna

    // Methods
public:
    void create_ray(const Antenna* receiver, const wallVect& all_walls); // Creates a ray from this antenna to the receiver
protected:
    virtual void add_ray(const Ray* ray_to_add) = 0; // Pure virtual method to add a ray to the antenna's rays
};




class RealAntenna : public Antenna {
    friend VirtualAntenna; // Allows a VirtualAntenna to add a ray to a RealAntenna
    // Attributes
private:
    antennaVect virtual_network; // Virtual network of antennas
    rayVect rays; // Rays emitted by the antenna
    double emission_factor; // Emission factor of the antenna
    double min_attenuation; // Minimum attenuation of the antenna
    double max_attenuation; // Maximum attenuation of the antenna

    // Constructors
public:
    RealAntenna(); // Default constructor
    RealAntenna(const Vector& position); // Constructor with position

    // Destructor
public:
    ~RealAntenna() override; // Destructor

    // Accessors
public:
    antennaVect get_virtual_network() const; // Returns the virtual network of antennas
    const rayVect get_rays() const; // Returns the rays emitted by the antenna
    double get_min_attenuation() const; // Returns the minimum attenuation
    double get_max_attenuation() const; // Returns the maximum attenuation

    // Mutators
public:
    void set_min_attenuation(double min_att); // Sets the minimum attenuation
    void set_max_attenuation(double max_att); // Sets the maximum attenuation

    // Methods
public:
    void virtualize(const Wall* wall_to_bounce); // Creates a VirtualAntenna by mirroring this antenna through the wall
    double get_binary_rate() const; // Returns the binary rate of the antenna
    void reset(); // Resets the antenna to start calculating rays and the binary rate again

private:
    void add_ray(const Ray* ray_to_add) override; // Adds a ray to the antenna's rays
};





class VirtualAntenna : public Antenna {
    // Constructors
public:
    VirtualAntenna(); // Default constructor
    VirtualAntenna(RealAntenna* realAntenna, const Wall* reflectedWall); // Constructor with real antenna and reflected wall

    // Destructor
public:
    ~VirtualAntenna() override = default; // Destructor

    // Method
private:
    void add_ray(const Ray* ray_to_add) override; // Adds a ray to the antenna's rays
};

