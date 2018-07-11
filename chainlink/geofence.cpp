#include "geofence.h"
#include <cmath>
#include "gpsdata.h"

Geofence::Geofence()
{
	maximumDistance = 1000;
}

Position Geofence::getCurrentLatitude() { return currentLatitude; }
Position Geofence::getCurrentLongitude() { return currentLongitude; }

double Geofence::getDistance()
{
	double distance;
	double phi_1, phi_2;
	double lambda_1, lambda_2;
	const double RADIUS = 6371;
	double temporary;

	phi_2 = gcsToRadians(currentLatitude);
	phi_1 = gcsToRadians(referenceLatitude);
	lambda_2 = gcsToRadians(currentLongitude);
	lambda_1 = gcsToRadians(referenceLongitude);

	distance = sin(0.5 * (lambda_2 - lambda_1));
	distance = distance * distance;
	distance = distance * cos(phi_1) * cos(phi_2);
	distance = distance + sin(0.5 * (phi_2 - phi_1)) * sin(0.5 * (phi_2 - phi_1));
	distance = asin(distance);
	distance = 2 * RADIUS * distance;

	return distance; 
}

bool Geofence::getIsActive() { return isActive; }
double Geofence::getMaximumDistance() { return maximumDistance; }
Position Geofence::getReferenceLatitude() { return referenceLatitude; }
Position Geofence::getReferenceLongitude() { return referenceLongitude; }

bool Geofence::isOutOfBounds()
{
	bool output;

	if (getDistance() > maximumDistance) {
		output = true;
	} else {
		output = false;
	}

	return output;
}

void Geofence::setCurrent(Position latitude, Position longitude)
{
	currentLatitude = latitude;
	currentLongitude = longitude;
}

void Geofence::setCurrentLatitude(Position latitude) { currentLatitude = latitude; }
void Geofence::setCurrentLongitude(Position longitude) { currentLongitude = longitude; }
void Geofence::setIsActive(bool active) { isActive = active; }
void Geofence::setMaximumDistance(double distance) { maximumDistance = distance; }

void Geofence::setReference(Position latitude, Position longitude)
{
	referenceLatitude = latitude;
	referenceLongitude = longitude;
}

void Geofence::setReferenceLatitude(Position latitude) { referenceLatitude = latitude; }
void Geofence::setReferenceLongitude(Position longitude) { referenceLongitude = longitude; }

double gcsToRadians(Position input)
{
	double radians;
	const double PI_CONSTANT = 3.141592653589793;

	radians = input.getDegrees();
	radians += input.getMinutes() / 60.0;
	radians = radians * PI_CONSTANT;
	radians = radians / 180.0;
	
	if (input.getDirection() == 'N') {
		// Do nothing
	} else if (input.getDirection() == 'S') {
		radians *= -1.0;
	} else if (input.getDirection() == 'E') {
		// Do nothing
	} else if (input.getDirection() == 'W') {
		radians *= -1.0;
	}

	return radians;
}
