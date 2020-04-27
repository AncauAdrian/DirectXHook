#pragma once
#include <string>
#include <vector>

class Poly {
	std::vector<float> vertx, verty;
	std::string name;

public:
	Poly(std::string name);

	Poly(const Poly& p);

	void add(float x, float y);

	std::vector<float> const& getX() const;

	std::vector<float> const& getY() const;

	std::string const& getName() const;

	bool containsPoint(float testx, float testy);
};

std::vector<Poly> loadPolys();