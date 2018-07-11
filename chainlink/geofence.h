#pragma once
#ifndef GEOFENCE_H_
#define GEOFENCE_H_
#include "gpsdata.h"

double gcsToRadians(Position input);

class Geofence {
public:
	Geofence();
	Position getCurrentLatitude();
	Position getCurrentLongitude();
	double getDistance();
	bool getIsActive();
	double getMaximumDistance();
	Position getReferenceLatitude();
	Position getReferenceLongitude();
	bool isOutOfBounds();
	void setCurrent(Position latitude, Position longitude);
	void setCurrentLatitude(Position latitude);
	void setCurrentLongitude(Position longitude);
	void setIsActive(bool active);
	void setMaximumDistance(double distance);
	void setReference(Position latitude, Position longitude);
	void setReferenceLatitude(Position latitude);
	void setReferenceLongitude(Position longitude);

private:
	Position currentLatitude;
	Position currentLongitude;
	bool isActive;
	double maximumDistance;
	Position referenceLatitude;
	Position referenceLongitude;
};

#endif // !GEOFENCE_H_

