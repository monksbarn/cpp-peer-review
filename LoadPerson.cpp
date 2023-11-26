#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <string_view>
#include <map>

using namespace std;

struct Person {
   string name;
   int age;
};

struct PersonFilter {
   int min_age;
   int max_age;
   string_view name_filter;
};

class DBQuery {
public:
   DBQuery(string_view query);
};

struct DBLogLevel {
   DBLogLevel();
};

class DataBase {
public:
   DataBase();
   string_view GetName() const;
   const int GetTimeout() const;
   bool IsAllowException() const;
   const DBLogLevel& GetLogLevel() const;
private:
   string_view name;
   int connection_timeout;
   bool allow_exception;
   DBLogLevel log_level_;
};

class DBHandler {
public:
   DBHandler();
   bool IsOK() const;
   string Quote(string_view filter) const;
   template<typename Key, typename Value>
   map<Key, Value> LoadRows(DBQuery&) const;

};

class DBConnector {
public:
   DBConnector(bool, const DBLogLevel&);
   DBHandler Connector();
   DBHandler ConnectTmp(string_view name, int timeout) const;
   DBHandler Connect(string_view name, int timeout) const;
};

/////////////////////////////////////////////////////////////////////////////////////всё, что выше, можно, наверное, не смотреть...

vector<Person> LoadPersons(const DataBase& db, const PersonFilter& person_filter) {

   DBConnector connector(db.IsAllowException(), db.GetLogLevel());
   DBHandler handler;

   if (db.GetName().substr(0, 4) == ("tmp."s)) {
      handler = connector.ConnectTmp(db.GetName(), db.GetTimeout());
   }
   else {
      handler = connector.Connect(db.GetName(), db.GetTimeout());
   }
   if (!db.IsAllowException() && !handler.IsOK()) {
      return {};
   }

   ostringstream query_str;
   query_str << "from Persons "s
      << "select Name, Age "s
      << "where Age between "s << person_filter.min_age << " and "s << person_filter.max_age << " "s
      << "and Name like '%"s << handler.Quote(person_filter.name_filter) << "%'"s;
   DBQuery query(query_str.str());

   vector<Person> persons;
   for (auto [name, age] : handler.LoadRows<string, int>(query)) {
      persons.push_back({ move(name), age });
   }
   return persons;
}