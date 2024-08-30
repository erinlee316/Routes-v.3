// Assignment 7, Routes v.3

// Program will use a static class to find the shortest route from
// San Francisco to New York City. The program will use two static
// recursive functions and output both any route and shortest route.

// c++ libraries
#include <iostream>
#include <vector>
#include <algorithm>    // used for sorting
#include <set>          // used for STL set
using namespace std;

// c library
#include <cstring>

// class variable for leg
class Leg
{
  // private member variables
  const char* const startCity;
  const char* const endCity;
  const double distance;

  // public access
  public:
  Leg(const char* const start, const char* const end, const double dist) : startCity(start), 
                                                                           endCity(end), 
                                                                           distance(dist) {}
  Leg& operator=(const Leg&);
  double getDistance() const {return distance;}
  void output(ostream&) const;

  // gives classes access to leg member variables
  friend class Route;
  friend class ShortestRoute;
};

// class variable for route
class Route
{
  // private member variables
  vector<const Leg*> legs;
  const double totalDistance;

  // public access
  public:
  Route(const Leg&);
  Route(const Route&, const Leg&);
  Route& operator=(const Route&);
  double getDistance () const {return totalDistance;}
  void output(ostream&) const;

  // give class access to route member variables
  friend bool operator<(const Route&, const Route&);
};

// class variable for shortest route
class ShortestRoute
{
  // private member variable
  static const int legSize;
  static const Leg l[];

  // public access
  public:
  static const Route anyRoute(const char* const, const char* const);
  static const Route shortestRoute(const char* const, const char* const);
};

// declare static const array with 40 leg obejcts
const Leg ShortestRoute::l[] = 
{
  Leg("Memphis", "Orlando", 684),
  Leg("Miles City", "Fargo", 430),
  Leg("Fargo", "Minneapolis", 214),
  Leg("Fargo", "Chicago", 569),
  Leg("Denver", "Omaha", 484),
  Leg("Denver", "Oklahoma City", 504),
  Leg("Memphis", "Washington", 763),
  Leg("New Orleans", "Orlando", 533),
  Leg("San Francisco", "Las Angeles", 352),
  Leg("Minneapolis", "Cleveland", 630),
  Leg("Chicago", "Cleveland", 307),
  Leg("Cleveland", "Pittsburgh", 115),
  Leg("Pittsburgh", "New York City", 315),
  Leg("Chicago", "Pittsburgh", 409),
  Leg("San Francisco", "Portland", 536),
  Leg("Reno", "Salt Lake City", 427),
  Leg("Las Angeles", "Phoenix", 357),
  Leg("San Francisco", "Reno", 186),
  Leg("El Paso", "Houston", 674),
  Leg("El Paso", "Oklahoma City", 576),
  Leg("Helena", "Miles City", 294),
  Leg("Helena", "Fargo", 721),
  Leg("Miles City", "Omaha", 606),
  Leg("Reno", "Phoenix", 601),
  Leg("Portland", "Helena", 516),
  Leg("Salt Lake City", "Denver", 371),
  Leg("Salt Lake City", "Miles City", 494),
  Leg("Omaha", "Chicago", 435),
  Leg("Omaha", "St. Louis", 357),
  Leg("Omaha", "Memphis", 531),
  Leg("Oklahoma City", "Memphis", 421),
  Leg("Houston", "Memphis", 485),
  Leg("Houston", "New Orleans", 318),
  Leg("St. Louis", "Philadelphia", 809),
  Leg("Reno", "Phoenix", 601),
  Leg("Phoenix", "Denver", 586),
  Leg("Phoenix", "El Paso", 345),
  Leg("Chicago", "Philadelphia", 664),
  Leg("Memphis", "Philadelphia", 879),
  Leg("Orlando", "Washington", 759),
  Leg("Washington", "New York City", 203),
  Leg("Philadelphia", "New York City", 80),
  Leg("San Francisco", "New York City", 21000)
};

// declare and initialize static variable
const int ShortestRoute::legSize = sizeof(ShortestRoute::l) / sizeof(ShortestRoute::l[0]);

// function prototypes
void id();
ostream& roundingOne(ostream&);
ostream& roundingOff(ostream&);

int main()
{
  // function call for identification information
  id();

  // finds any route using recursive function
  const Route route1 = ShortestRoute::anyRoute("San Francisco", "New York City");
  cout << "Any Route:\n";
  route1.output(cout);
  cout << endl;

  // finds shortest route using recursive function
  const Route route2 = ShortestRoute::shortestRoute("San Francisco", "New York City");
  cout << "Shortest Route:\n";
  route2.output(cout);
  cout << endl;

  return 0;
}

// function will display identification information
void id()
{
  cout << "Programmer: Erin Lee" << endl;
  cout << "Programmer's ID: 1805820" << endl;
  cout << "File: " << __FILE__ << endl;
}

// function will round numbers to one decimal digit
ostream& roundingOne(ostream& out)
{
  out.setf(ios::fixed);
  out.precision(1);
  return out;
}

// function will turn off rounding back to default
ostream& roundingOff(ostream& out)
{
  out.unsetf(ios::fixed);
  out.precision(6);
  return out;
}

// getter function will display output to console 
void Leg::output(ostream& out) const
{
  out << "Leg: " << startCity << " to " << endCity << ", " 
      << distance << " miles." << endl;
}

// assignment operator function for leg class
// used for copying information from one leg object to another leg object
Leg& Leg::operator=(const Leg& copyThis)
{
  // declare reference variable to host object
  Leg& host = *this;

  // check if host is not same as parameter
  // if not, remove const factor from host and assign copy to parameter 
  if (this != &copyThis)
  {
    const_cast<const char*&>(host.startCity) = copyThis.startCity;
    const_cast<const char*&>(host.endCity) = copyThis.endCity;
    const_cast<double&>(host.distance) = copyThis.distance;
  }

  // return reference variable
  return host;
}

// route constructor will append address of parameter as last object in bag 
Route::Route(const Leg& l) : totalDistance(l.getDistance()) 
{
  legs.push_back(&l);
}

// route constructor creates new route by adding new leg to existing route
// only works if cities in new leg match end cities of last leg 
Route::Route(const Route& r, const Leg& l) : legs(r.legs), 
                                             totalDistance(r.totalDistance + l.distance)
{
  // use try catch to catch error and exception
  try
  {
    // check if endCity of last leg in route and startCity of leg parameter are the same
    if (strcmp(legs.back()->endCity, l.startCity) != 0)

      // throw exception if they are different
      throw "Cities don't match!";

    // append address of parameter as last object in bag
    legs.push_back(&l);

    // remove const factor and update total distance of route
    const_cast<double&>(totalDistance) += l.getDistance();
  }

  // exception
  catch(const char* ex)
  {
    cout << ex << endl;
  }
}

// getter function will output routes
void Route::output(ostream& out) const
{
  // declare and initialize variable
  double total = 0;
  
  // move through entire leg array
  for (int i = 0; i < legs.size(); i++)
  {
    // check if current leg is last leg in route
    // if not, output start city
    if (i != legs.size() - 1)
      out << legs[i]->startCity << " to ";

    // if last leg in route, output start and end city
    else  
      out << legs[i]->startCity << " to " << legs[i]->endCity << ", ";

    // calculate and update total distance of route
    total += legs[i]->getDistance();
  }

  // output connecting routes with total distance
  out << roundingOne << total << roundingOff << " miles." << endl;
}

// assignment operator function for route class
// used for copying information from one route object to another route object
Route& Route::operator=(const Route& copyThis)
{
  // declare reference variable to host object
  Route& host = *this;

  // check if host is not same as parameter
  // if not, remove const factor from host's member variables and assign to parameter's member variables
  if (this != &copyThis)
  {
    host.legs = copyThis.legs;
    const_cast<double&>(host.totalDistance) = copyThis.getDistance();
  }

  // return reference variable
  return host;
}

// function will find any route from San Francisco to New York City
const Route ShortestRoute::anyRoute(const char* const start, const char* const end) 
{
  // loop through all legs in bag
  for (int i = 0; i < ShortestRoute::legSize; i++) 
  {
    // check if endCity of current leg matches parameter
    if (strcmp(ShortestRoute::l[i].endCity, end) == 0) 
    {
      // check if startCity of current leg matches parameter
      if (strcmp(ShortestRoute::l[i].startCity, start) == 0) 
      {
        // they match!
        // create route object and return object 
        Route r(ShortestRoute::l[i]);
        return r;
      }

      // only endCity matches
      else 
      {
        // create route object and recursively find any route with same start city
        // return that route object
        Route any(ShortestRoute::anyRoute(start, ShortestRoute::l[i].startCity),l[i]);
        return any;
      }
    }
  }

  // throw exception if no routes found
  throw "No route found!";
}

// function will find the shortest route from San Francisco to New York City
const Route ShortestRoute::shortestRoute(const char* const start, const char* const end)
{  
  // declare set variable
  set<Route> allRoutes;

  // loop through all legs in bag
  for (int i = 0; i < ShortestRoute::legSize; i++) 
  {
    // check if endCity of current leg matches parameter
    if (strcmp(ShortestRoute::l[i].endCity, end) == 0) 
    {
      // check if startCity of current leg matches parameter
      if (strcmp(ShortestRoute::l[i].startCity, start) == 0) 
      {
        // they match!
        // create route object bag and insert object in set
        Route r(ShortestRoute::l[i]);
        allRoutes.insert(r);
      }
      
      // only endCity matched 
      else 
      {
        // create route object bag and recursively find shortest route
        // insert object into set
        Route shortest(ShortestRoute::shortestRoute(start, ShortestRoute::l[i].startCity), l[i]);
        allRoutes.insert(shortest);
      }
    }
  }
  
  // set is not empty, return minimum/shortest route
  if (!allRoutes.empty())
    return *allRoutes.begin();
  
  // throw exception if set is empty
  throw "Shortest route not found!";
}

// overloading operator to sort the set from ascending order
bool operator<(const Route& left, const Route& right)
{
  return left.totalDistance < right.totalDistance;
}