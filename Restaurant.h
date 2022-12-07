#pragma once
#include <string>
using namespace std;

struct Restaurant
{
	double weight;
	string name;
	string city;
	string area;
	double rating;
	int ratingCount;
	string phoneNumber;
	string cuisine;
	int costFor2;
	double longitude;
	double latitude;
	Restaurant(string Name, string City, string Area, double Rating, int RatingCount, string Phone, string Cuisine, int Cost, double Longitude, double Latitude);

};

