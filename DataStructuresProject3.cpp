
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <map>
#include "Restaurant.h"
using namespace std;

void swap(vector<Restaurant>& arr, Restaurant* a, Restaurant* b) {
	Restaurant t = *a;
	*a = *b;
	*b = t;
}

int partition(vector<Restaurant>& arr, int low, int high) {
	double pivot = arr[low].weight;
	int up = low;
	int down = high;
	while (up < down) {
		for (int j = up; j < high; j++) {
			if (arr[up].weight > pivot) {
				break;
			}
			up++;
		}
		for (int j = high; j > low; j--) {
			if (arr[down].weight < pivot) {
				break;
			}
			down--;
		}
		if (up < down) {
			swap(arr, &arr[up], &arr[down]);
		}
	}
	swap(arr, &arr[low], &arr[down]);
	return down;
}

void quickSort(vector<Restaurant>& arr, int low, int high) {
	if (low < high) {
		int pivot = partition(arr, low, high);
		quickSort(arr, low, pivot - 1);
		quickSort(arr, pivot + 1, high);
	}
}

double distance(Restaurant& object, double longitude, double latitude) {
	double r = 0.01745327; // PI/180
	latitude = latitude * r;
	double la2 = object.latitude;
	la2 = la2 * r;
	longitude = longitude * r;
	double lo2 = object.longitude;
	lo2 = lo2 * r;
	double er = 6371.01;
	double distanceValue = er * acos((sin(latitude) * sin(la2)) + (cos(latitude) * cos(la2) * cos(longitude - lo2)));
	return distanceValue;
}

// rating in this function is the user input, which should be passed to this function
vector<Restaurant> getsortedlist(map<string, vector<Restaurant>>& citytorestaurant, string city, double rating, double longitude, double latitude) {
	vector<Restaurant> restaurantsincity = citytorestaurant[city];
	vector<Restaurant> finalrestaurants;
	for (int i = 0; i < restaurantsincity.size(); i++) {
		if (restaurantsincity[i].rating >= rating) {
			finalrestaurants.push_back(restaurantsincity[i]);
		}
	}
	for (int j = 0; j < finalrestaurants.size(); j++) {
		finalrestaurants[j].weight = finalrestaurants[j].costFor2;
		finalrestaurants[j].weight += (5 - finalrestaurants[j].rating) * 500;
		finalrestaurants[j].weight += distance(finalrestaurants[j], longitude, latitude) * 1000;
	}
	return finalrestaurants;
}

void merge(vector<Restaurant>& arr, int beg, int middle, int end) {

	int length1 = middle - beg + 1;
	int length2 = end - middle;

	vector<Restaurant>leftArr;
	vector<Restaurant>rightArr;

	for (int i = 0; i < length1; i++)
		leftArr.push_back(arr[beg + i]);
	for (int j = 0; j < length2; j++)
		rightArr.push_back(arr[middle + 1 + j]);

	int i, j, k;
	i = 0;
	j = 0;
	k = beg;

	while (i < length1 && j < length2) {
		if (leftArr[i].weight <= rightArr[j].weight) {
			arr[k] = leftArr[i];
			i++;
		}
		else {
			arr[k] = rightArr[j];
			j++;
		}
		k++;
	}

	while (i < length1) {
		arr[k] = leftArr[i];
		i++;
		k++;
	}

	while (j < length2) {
		arr[k] = rightArr[j];
		j++;
		k++;
	}
}

void mergeSort(vector<Restaurant>& arr, int beg, int end) {
	if (beg < end) {
		int mid = beg + (end - beg) / 2;

		mergeSort(arr, beg, mid);
		mergeSort(arr, mid + 1, end);
		merge(arr, beg, mid, end);
	}
}

void display(vector<Restaurant>& arr, int size) {
	for (int i = 0; i < size; i++)
		cout << arr[i].name << " ";
	cout << endl;
}

int main()
{
	clock_t clock;
	bool box1isSelected = false;
	bool box2isSelected = false;
	bool box3isSelected = false;
	bool box4isSelected = false;
	bool box5isSelected = false;
	bool mergesort = true;
	bool quicksort = false;
	bool screen1 = true;
	string cityInput = "";
	string distanceInput = "";
	string ratingInput = "";
	string latitudeInput = "";
	string longitudeInput = "";
	vector<Restaurant> restaurants;
	vector<Restaurant> finalRestaurants;
	map<string, vector<Restaurant>> citytoRestaurant;
	ifstream file("test3_allpoints.csv");
	if (file.is_open()) {
		//get header
		string lineFromFile;
		getline(file, lineFromFile);

		while (getline(file, lineFromFile)) {
			istringstream stream(lineFromFile);
			string url;
			string name;
			string city;
			string area;
			string tempRating;
			double rating;
			string tempRatingCount;
			int ratingCount;
			string phoneNumber;
			string cuisine;
			string tempCost;
			int costFor2;
			string address;
			string timings;
			string onlineOrder;
			string reservation;
			string deliveryOnly;
			string tempLongitude;
			double longitude;
			string tempLatitude;
			double latitude;

			getline(stream, url, ',');
			getline(stream, name, ',');
			getline(stream, city, ',');
			getline(stream, area, ',');
			getline(stream, tempRating, ',');
			rating = stod(tempRating);
			getline(stream, tempRatingCount, ',');
			ratingCount = stoi(tempRatingCount);
			getline(stream, phoneNumber, ',');
			getline(stream, cuisine, ',');
			getline(stream, tempCost, ',');
			costFor2 = stoi(tempCost);
			getline(stream, tempLongitude, ',');
			longitude = stod(tempLongitude);
			getline(stream, tempLatitude, ',');
			latitude = stod(tempLatitude);

			Restaurant newRestaurant(name, city, area, rating, ratingCount, phoneNumber, cuisine, costFor2, longitude, latitude);
			restaurants.push_back(newRestaurant);
			citytoRestaurant[city].push_back(newRestaurant);

		}
	}

			sf::RenderWindow window(sf::VideoMode(1000, 500), "InputBoard");
			sf::Font font1;
			if (!font1.loadFromFile("arial.ttf")) {
				cout << "font not loaded\n";
			}
			sf::Font font2;
			if (!font2.loadFromFile("AB_Ultrachic_by_redfonts.ttf")) {
				cout << "font not loaded\n";
			}
			sf::Text title;
			title.setFont(font2);
			title.setString("Restaurant Finder");
			title.setCharacterSize(50);
			title.setFillColor(sf::Color::Blue);
			title.setStyle(sf::Text::Bold | sf::Text::Underlined);

			sf::Text question;
			question.setFont(font1);
			question.setString("Input your Preferences:");
			question.setCharacterSize(30);
			question.setFillColor(sf::Color::White);
			question.setStyle(sf::Text::Underlined);
			question.setPosition(0.f, 70.f);

			sf::Text cityString;
			cityString.setFont(font1);
			cityString.setString("City: ");
			cityString.setCharacterSize(25);
			cityString.setFillColor(sf::Color(252, 112, 6));
			cityString.setPosition(0.f, 120.f);

			sf::Text location;
			location.setFont(font1);
			location.setString("Location Distance: ");
			location.setCharacterSize(25);
			location.setFillColor(sf::Color(252, 112, 6));
			location.setPosition(0.f, 155.f);

			sf::Text ratingText;
			ratingText.setFont(font1);
			ratingText.setString("Rating: ");
			ratingText.setCharacterSize(25);
			ratingText.setFillColor(sf::Color(252, 112, 6));
			ratingText.setPosition(0.f, 190.f);

			sf::RectangleShape rectangle1(sf::Vector2f(100.f, 25.f));
			rectangle1.setPosition(75.f, 120.f);
			rectangle1.setFillColor(sf::Color::White);

			sf::RectangleShape rectangle2(sf::Vector2f(100.f, 25.f));
			rectangle2.setPosition(225.f, 155.f);
			rectangle2.setFillColor(sf::Color::White);

			sf::RectangleShape rectangle3(sf::Vector2f(100.f, 25.f));
			rectangle3.setPosition(90.f, 190.f);
			rectangle3.setFillColor(sf::Color::White);

			sf::RectangleShape rectangle4(sf::Vector2f(100.f, 25.f));
			rectangle4.setPosition(110.f, 280.f);
			rectangle4.setFillColor(sf::Color::White);
			
			sf::RectangleShape rectangle5(sf::Vector2f(100.f, 25.f));
			rectangle5.setPosition(130.f, 315.f);
			rectangle5.setFillColor(sf::Color::White);

			sf::RectangleShape sortButton(sf::Vector2f(250.f, 25.f));
			sortButton.setPosition(50.f, 450.f);
			sortButton.setFillColor(sf::Color::White);
			sortButton.setOutlineColor(sf::Color(252, 112, 6));
			sortButton.setOutlineThickness(10);

			sf::Text sortButtonText;
			sortButtonText.setPosition(60.f, 450.f);
			sortButtonText.setString("Change Sorting Method");
			sortButtonText.setFont(font1);
			sortButtonText.setFillColor(sf::Color::Blue);
			sortButtonText.setCharacterSize(20);
			sortButtonText.setStyle(sf::Text::Bold);

			sf::Text currSort;
			currSort.setPosition(320.f, 450.f);
			currSort.setCharacterSize(20);
			currSort.setFont(font1);
			currSort.setFillColor(sf::Color::White);

			sf::Text inputLocation;
			inputLocation.setFont(font1);
			inputLocation.setString("Input Your Location:");
			inputLocation.setCharacterSize(30);
			inputLocation.setFillColor(sf::Color::White);
			inputLocation.setPosition(0.f, 230.f);
			inputLocation.setStyle(sf::Text::Underlined);

			sf::Text inputLatitude;
			inputLatitude.setFont(font1);
			inputLatitude.setString("Latitude: ");
			inputLatitude.setCharacterSize(25);
			inputLatitude.setFillColor(sf::Color(59, 171, 18));
			inputLatitude.setPosition(0.f, 280.f);

			sf::Text inputLongitude;
			inputLongitude.setFont(font1);
			inputLongitude.setString("Longitude: ");
			inputLongitude.setCharacterSize(25);
			inputLongitude.setFillColor(sf::Color(59, 171, 18));
			inputLongitude.setPosition(0.f, 315.f);

			sf::Texture IndiaTexture;
			if (!IndiaTexture.loadFromFile("India.png")) {
				cout << "image not loaded\n";
			}
			sf::Sprite India;
			India.setTexture(IndiaTexture);
			India.setPosition(400.f, 10.f);
			India.setScale(0.5f, 0.5f);

			sf::Texture enterTexture;
			if (!enterTexture.loadFromFile("EnterButton.png")) {
				cout << "image not loaded\n";
			}
			enterTexture.setSmooth(true);
			sf::Sprite enterButton;
			enterButton.setTexture(enterTexture);
			enterButton.setPosition(50.f, 350.f);
			enterButton.setScale(0.5f, 0.5f);

			sf::Text cityOutput;
			cityOutput.setFont(font1);
			cityOutput.setCharacterSize(20);
			cityOutput.setFillColor(sf::Color::Black);
			cityOutput.setPosition(75.f, 120.f);
			cityOutput.setString(cityInput);

			sf::Text distanceOutput;
			distanceOutput.setFont(font1);
			distanceOutput.setCharacterSize(20);
			distanceOutput.setFillColor(sf::Color::Black);
			distanceOutput.setPosition(225.f, 155.f);
			distanceOutput.setString(distanceInput);

			sf::Text ratingOutput;
			ratingOutput.setFont(font1);
			ratingOutput.setCharacterSize(20);
			ratingOutput.setFillColor(sf::Color::Black);
			ratingOutput.setPosition(90.f, 190.f);
			ratingOutput.setString(ratingInput);

			sf::Text latitudeOutput;
			latitudeOutput.setFont(font1);
			latitudeOutput.setCharacterSize(20);
			latitudeOutput.setFillColor(sf::Color::Black);
			latitudeOutput.setPosition(110.f, 280.f);
			latitudeOutput.setString(latitudeInput);

			sf::Text longitudeOutput;
			longitudeOutput.setFont(font1);
			longitudeOutput.setCharacterSize(20);
			longitudeOutput.setFillColor(sf::Color::Black);
			longitudeOutput.setPosition(130.f, 315.f);
			longitudeOutput.setString(longitudeInput);

			sf::Text title2;
			title2.setFont(font2);
			title2.setCharacterSize(50);
			title2.setFillColor(sf::Color::Blue);
			title2.setPosition(0.f, 0.f);
			title2.setString("Your Recommended Restaurants:");
			title2.setStyle(sf::Text::Bold | sf::Text::Underlined);

			sf::RectangleShape box(sf::Vector2f(950.f, 300.f));
			box.setFillColor(sf::Color::White);
			box.setOutlineColor(sf::Color(252, 112, 6));
			box.setOutlineThickness(10);
			box.setPosition(30.f, 75.f);

			sf::Text first;
			first.setFillColor(sf::Color(59, 171, 18));
			first.setString("1.");
			first.setPosition(30.f, 75.f);
			first.setFont(font1);
			first.setCharacterSize(25);

			sf::Text second;
			second.setFillColor(sf::Color(59, 171, 18));
			second.setString("2.");
			second.setPosition(30.f, 105.f);
			second.setFont(font1);
			second.setCharacterSize(25);

			sf::Text third;
			third.setFillColor(sf::Color(59, 171, 18));
			third.setString("3.");
			third.setPosition(30.f, 135.f);
			third.setFont(font1);
			third.setCharacterSize(25);

			sf::Text forth;
			forth.setFillColor(sf::Color(59, 171, 18));
			forth.setString("4.");
			forth.setPosition(30.f, 165.f);
			forth.setFont(font1);
			forth.setCharacterSize(25);

			sf::Text fifth;
			fifth.setFillColor(sf::Color(59, 171, 18));
			fifth.setString("5.");
			fifth.setPosition(30.f, 195.f);
			fifth.setFont(font1);
			fifth.setCharacterSize(25);

			sf::Text sixth;
			sixth.setFillColor(sf::Color(59, 171, 18));
			sixth.setString("6.");
			sixth.setPosition(30.f, 225.f);
			sixth.setFont(font1);
			sixth.setCharacterSize(25);

			sf::Text seventh;
			seventh.setFillColor(sf::Color(59, 171, 18));
			seventh.setString("7.");
			seventh.setPosition(30.f, 255.f);
			seventh.setFont(font1);
			seventh.setCharacterSize(25);

			sf::Text eighth;
			eighth.setFillColor(sf::Color(59, 171, 18));
			eighth.setString("8.");
			eighth.setPosition(30.f, 285.f);
			eighth.setFont(font1);
			eighth.setCharacterSize(25);

			sf::Text ninth;
			ninth.setFillColor(sf::Color(59, 171, 18));
			ninth.setString("9.");
			ninth.setPosition(30.f, 315.f);
			ninth.setFont(font1);
			ninth.setCharacterSize(25);

			sf::Text tenth;
			tenth.setFillColor(sf::Color(59, 171, 18));
			tenth.setString("10.");
			tenth.setPosition(30.f, 345.f);
			tenth.setFont(font1);
			tenth.setCharacterSize(25);

			while (window.isOpen()) {
				window.clear();

				if (screen1 == true) {
					window.draw(title);
					window.draw(question);
					window.draw(cityString);
					window.draw(location);
					window.draw(ratingText);
					window.draw(rectangle1);
					window.draw(rectangle2);
					window.draw(rectangle3);
					window.draw(inputLocation);
					window.draw(inputLatitude);
					window.draw(inputLongitude);
					window.draw(rectangle4);
					window.draw(rectangle5);
					window.draw(India);
					window.draw(distanceOutput);
					window.draw(cityOutput);
					window.draw(ratingOutput);
					window.draw(latitudeOutput);
					window.draw(longitudeOutput);
					window.draw(enterButton);
					window.draw(sortButton);
					window.draw(sortButtonText);
					if (mergesort == true) {
						currSort.setString("Current Sort Method: Merge Sort");
					}
					else if (quicksort == true) {
						currSort.setString("Current Sort Method: Quick Sort");
					}
					window.draw(currSort);

					sf::Event event;
					while (window.pollEvent(event)) {
						if (event.type == sf::Event::Closed) {
							window.close();
						}
						else if (event.type == sf::Event::MouseButtonPressed) {
							sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
							//if you click in box1
							if (mousePosition.x >= 75.f && mousePosition.x <= 175.f && mousePosition.y >= 120.f && mousePosition.y <= 145.f) {
								box1isSelected = !box1isSelected;
								box2isSelected = false;
								box3isSelected = false;
								box4isSelected = false;
								box5isSelected = false;
							}
							if (mousePosition.x >= 225.f && mousePosition.x <= 325.f && mousePosition.y >= 155.f && mousePosition.y <= 180.f) {
								box1isSelected = false;
								box2isSelected = true;
								box3isSelected = false;
								box4isSelected = false;
								box5isSelected = false;
							}
							if (mousePosition.x >= 90.f && mousePosition.x <= 190.f && mousePosition.y >= 190.f && mousePosition.y <= 215.f) {
								box1isSelected = false;
								box2isSelected = false;
								box3isSelected = true;
								box4isSelected = false;
								box5isSelected = false;
							}
							if (mousePosition.x >= 110.f && mousePosition.x <= 210.f && mousePosition.y >= 280.f && mousePosition.y <= 305.f) {
								box1isSelected = false;
								box2isSelected = false;
								box3isSelected = false;
								box4isSelected = true;
								box5isSelected = false;
							}
							if (mousePosition.x >= 130.f && mousePosition.x <= 230.f && mousePosition.y >= 315.f && mousePosition.y <= 340.f) {
								box1isSelected = false;
								box2isSelected = false;
								box3isSelected = false;
								box4isSelected = false;
								box5isSelected = true;
							}
							if (mousePosition.x >= 70 && mousePosition.x <= 395.f && mousePosition.y >= 360.f && mousePosition.y <= 420) {
								box1isSelected = false;
								box2isSelected = false;
								box3isSelected = false;
								box4isSelected = false;
								box5isSelected = false;
								screen1 = false;
								screen1 = false;
								window.close();
							}
							if (mousePosition.x >= 60.f && mousePosition.x <= 310.f && mousePosition.y >= 440.f && mousePosition.y <= 485.f) {
								box1isSelected = false;
								box2isSelected = false;
								box3isSelected = false;
								box4isSelected = false;
								box5isSelected = false;
								mergesort = !mergesort;
								quicksort = !quicksort;
							}
						}
						if (event.type == sf::Event::TextEntered) {
							if (box1isSelected == true) {
								cityInput += (char)event.text.unicode;
								cityOutput.setString(cityInput);
							}
							else if (box2isSelected == true) {
								distanceInput += (char)event.text.unicode;
								distanceOutput.setString(distanceInput);
							}
							else if (box3isSelected == true) {
								ratingInput += (char)event.text.unicode;
								ratingOutput.setString(ratingInput);
							}
							else if (box4isSelected == true) {
								latitudeInput += (char)event.text.unicode;
								latitudeOutput.setString(latitudeInput);
							}
							else if (box5isSelected == true) {
								longitudeInput += (char)event.text.unicode;
								longitudeOutput.setString(longitudeInput);
							}
						}
					}
				}
				window.display();
			}
			std::clock_t start;
			double duration;
			if (mergesort == true) {
				finalRestaurants.clear();
				finalRestaurants = getsortedlist(citytoRestaurant, cityInput, stod(ratingInput), stod(longitudeInput), stod(latitudeInput));
				start = std::clock();
				mergeSort(finalRestaurants, 0, finalRestaurants.size() - 1);
				duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
				cout << "Merge Sort Time: " << duration;
			}
			else if (quicksort == true) {
				finalRestaurants.clear();
				finalRestaurants = getsortedlist(citytoRestaurant, cityInput, stod(ratingInput), stod(longitudeInput), stod(latitudeInput));
				start = std::clock();
				quickSort(finalRestaurants, 0, finalRestaurants.size() - 1);
				duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
				cout << "Quick Sort Time: " << duration;
			}
			
			sf::RenderWindow window2(sf::VideoMode(1000, 500), "DisplayBoard");
			
			while (window2.isOpen()) {								
									for (int i = 0; i < 10; i++) {
										string helper;
										helper += finalRestaurants[i].name;
										helper += " | ";
										helper += finalRestaurants[i].city;
										helper += " | ";
										helper += finalRestaurants[i].cuisine;
										helper += " | ";
										helper += to_string(finalRestaurants[i].rating);
										if (i == 0) {
											first.setString("1. " + helper);
										}
										else if (i == 1) {
											second.setString("2. " + helper);
										}
										else if (i == 2) {
											third.setString("3. " + helper);
										}
										else if (i == 3) {
											forth.setString("4. " + helper);
										}
										else if (i == 4) {
											fifth.setString("5. " + helper);
										}
										else if (i == 5) {
											sixth.setString("6. " + helper);
										}
										else if (i == 6) {
											seventh.setString("7. " + helper);
										}
										else if (i == 7) {
											eighth.setString("8. " + helper);
										}
										else if (i == 8) {
											ninth.setString("9. " + helper);
										}
										else if (i == 9) {
											tenth.setString("10. " + helper);
										}
									}
									
				window2.draw(title2);
				window2.draw(box);
				window2.draw(first);
				window2.draw(second);
				window2.draw(third);
				window2.draw(forth);
				window2.draw(fifth);
				window2.draw(sixth);
				window2.draw(seventh);
				window2.draw(eighth);
				window2.draw(ninth);
				window2.draw(tenth);

				window2.display();
			}
			
		
	
}

