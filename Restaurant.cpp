#include "Restaurant.h"

Restaurant::Restaurant(string Name, string City, string Area, double Rating, int RatingCount, string Phone, string Cuisine, int Cost, double Longitude, double Latitude) {
	name = Name;
	city = City;
	area = Area;
	rating = Rating;
	ratingCount = RatingCount;
	phoneNumber = Phone;
	cuisine = Cuisine;
	costFor2 = Cost;
	longitude = Longitude;
	latitude = Latitude;
	weight = 0;
}
